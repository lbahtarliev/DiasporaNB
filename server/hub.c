#include "main.h"

void connect_hub_process()
{
	char rtn_str[200];
	
	sprintf(rtn_str,"1,%d",VERSION);
	send_con_hub(rtn_str);
	
	strcpy(rtn_str,"2,");
	strcat(rtn_str,server_info.login);
	strcat(rtn_str,",");
	strcat(rtn_str,server_info.password);
	send_con_hub(rtn_str);
}

void hub_request_sector_info()
{
	//so do it
	send_hub_all_sector_info();
}

void handle_user_login(int con, int user_id, char *user_info)
{
	char temp_str[5][1025];
	char rtn_str[200];
	int point = 0;
	
	if (user_con[con].user_id != user_id) //if the server is giving us out of date info, then dismiss it
		return;
		
	if (user_info[0] == '\0')
	{
		return;//tell the user that their id is invalid and they should consider reconnecting
	}
	
	user_logoff(con); //clear the con's good name
	
	user_con[con].user_id = user_id; //reset id
	
	split(temp_str[0], user_info, ',', &point);
	split(temp_str[1], user_info, ',', &point);
	split(temp_str[2], user_info, ',', &point);
	split(temp_str[3], user_info, ',', &point);
	split(temp_str[4], user_info, ',', &point);	
	
	//cap some shit
	temp_str[0][20] = 0;
	temp_str[1][20] = 0;
	temp_str[2][20] = 0;
	temp_str[3][20] = 0;
	
	strcpy(user_con[con].username, temp_str[0]);
	strcpy(user_con[con].guild, temp_str[1]);
	user_con[con].last_planet = atoi(temp_str[2]);
	strcpy(user_con[con].guild_rank_name, temp_str[3]);
	user_con[con].guild_rank_id = atoi(temp_str[4]);
	
	//set this
	user_con[con].has_bot_guild = is_bot_guild(user_con[con].guild);
	
	if (!user_con[con].has_galaxy_info) //send it then
		send_user_galaxy_info(con);
	
	set_user_zone_sector(con); //life is good
	
	//give planet good info
	send_all_good_price_info(con);
	send_user_con_to_planet(con, user_con[con].last_planet);
	
	//should we tell everyone?
	if(users_online() <= LOW_PLAYER_COUNT)
         relay_public_entry(con);
	
	//give them the free_scan list
	user_send_free_scan_list(con);
	
	//you know
	user_clear_has_member_sectors(con);
	user_resend_has_member_sectors(con);
}

void hub_force_user_offline(char *username)
{
	int i;
	char temp_name[21];
	
	lcase(username,username);
	
	for(i=0;i<=user_con_top;i++)
	{
		lcase(temp_name,user_con[i].username);
		if(str_match(username, temp_name))
		{
			user_logoff(i);
			close_user_socket(i);
		}
	}
}

void set_user_ship(char *user_name, char *message)
{
	int i, j, point = 0;
	char temp_str[1025];
	
	for(i=0;i<=user_con_top;i++)
		if (str_match(user_name,user_con[i].username)) break;
	
	if (i>user_con_top) return; //user not found
	
	//username,ship_id,ship_brand,ship_kind,ship_class_name,max_hull,hull,max_shields,range,max_speed,evasion,max_guns,max_missiles
	split(temp_str, message, ',', &point);
	user_con[i].ship.server_id = atoi(temp_str);	
	split(temp_str, message, ',', &point);
	user_con[i].ship.type = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.kind = atoi(temp_str);
	split(temp_str, message, ',', &point);
	temp_str[20] = '\0';
	strcpy(user_con[i].ship.class_name, temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.hull_max = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.hull = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.shield_max = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.range = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.speed = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.evasion = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.gun_ammount = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.missile_ammount = atoi(temp_str);
	split(temp_str, message, ',', &point);
	user_con[i].ship.destroy_code = atoi(temp_str);
	
	//just in case
	user_con[i].destroy_on_recv = 0;
	
	//check if no ship
	if((user_con[i].ship.server_id < 0 || user_con[i].ship.server_id >= MAX_SERVER) ||
		   (user_con[i].ship.type < 0 || user_con[i].ship.type >= SHIP_MAX) ||
		   (user_con[i].ship.kind < 0 || user_con[i].ship.kind >= 8))
	{
		user_con[i].ship.server_id 	= -1;
		user_con[i].ship.type 		= -1;
		user_con[i].ship.kind 		= -1;
		return;
	}
	
	for(j=0;j<user_con[i].ship.gun_ammount;j++)
	{
		split(temp_str, message, ',', &point);
		user_con[i].ship.gun[j].accuracy = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.gun[j].speed = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.gun[j].damage = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.gun[j].type = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.gun[j].destroyed = atoi(temp_str);
	}
	
	for(j=0;j<user_con[i].ship.missile_ammount;j++)
	{
		split(temp_str, message, ',', &point);
		user_con[i].ship.missile[j].accuracy = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.missile[j].speed = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.missile[j].damage = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.missile[j].type = atoi(temp_str);
		split(temp_str, message, ',', &point);
		user_con[i].ship.missile[j].left = atoi(temp_str);
		
		set_missile_time(i, j);
	}
	
	//set shields
	user_con[i].ship.shield = 0;
	user_con[i].ship.shield_level = 0;
	
	//set gun speed and jump time
	set_average_gun_speed(i);
	set_user_jump_time(i);
}

void set_user_ship_mod(char *user_name, char *message)
{
	int i, j, point = 0, mod_type;
	char temp_str[1025];
	
	i = user_to_con(user_name);
	
	if (i == -1) return;
	
	//get the ol mod type
	split(temp_str,message,',',&point);
	mod_type = atoi(temp_str);
	
	switch (mod_type)
	{
		case 0: //guns
			//username,mod_type,slot_number,gun/miss/misc_acc,gun/miss/misc_spd,gun/miss/misc_dam,
			split(temp_str,message,',',&point);
			j = atoi(temp_str);
			if (j >= 16) break; //fffoolllkkss .. ?
			
			split(temp_str, message, ',', &point);
			user_con[i].ship.gun[j].accuracy = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.gun[j].speed = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.gun[j].damage = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.gun[j].type = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.gun[j].destroyed = atoi(temp_str);
			
			set_average_gun_speed(i);
			
			break;
		case 1: //missiles
			split(temp_str,message,',',&point);
			j = atoi(temp_str);
			if (j >= 16) break; //fffoolllkkss .. ?
			
			split(temp_str, message, ',', &point);
			user_con[i].ship.missile[j].accuracy = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.missile[j].speed = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.missile[j].damage = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.missile[j].type = atoi(temp_str);
			split(temp_str, message, ',', &point);
			user_con[i].ship.missile[j].left = atoi(temp_str);
			
			set_missile_time(i, j);
			
			break;
	}
}

void send_user_to_planet(char *user_name, int planet)
{
	int i;
	
	i = user_to_con(user_name);
	
	if (i == -1) return;
	
	//do this so their sector will be set right 
	user_con[i].sector = -1;
	user_con[i].zone = -1;
	
	send_user_con_to_planet(i, planet);
}

void send_user_con_to_planet(int con, int p)
{
	char rtn_str[200];
	
	//set it right
	user_con[con].last_planet = p;
	
	//send them the planet info
	send_good_ammount_info(con, p);
	user_send_sat_info(con, p);
	
	sprintf(rtn_str, "6,%d", p);
	send_con_user(con, rtn_str);
}

void set_user_hull(char *user_name, int new_hull)
{
	int i;
	char rtn_str[200];
	
	i = user_to_con(user_name);
	
	if (i == -1) return;
	
	//check for fucking around
	if(user_con[i].ship.hull_max < new_hull)
		new_hull = user_con[i].ship.hull_max;
	
	if(new_hull < 0)
		new_hull = 0;
	
	user_con[i].ship.hull = new_hull;
	
	//tell the client
	send_user_hull(i);
	
	//just in case
	if(user_con[i].ship.hull)
		user_con[i].destroy_on_recv = 0;
}

void reload_user_missile(char *user_name, int wep_num, int wep_amt)
{
	int i;
	
	if(wep_num < 0) return;
	if(wep_num >= 16) return;
	if(wep_amt < 0) wep_amt = 0;
	if(wep_amt > 4) wep_amt = 4;
	
	i = user_to_con(user_name);
	
	if (i == -1) return;
	
	user_con[i].ship.missile[wep_num].left = wep_amt;
}

void set_good_prices(int planet_number, char *message)
{
	int i, k, point;
	char temp_str[1024];
	
	//check for fucking around
	if(planet_number < 0) return;
	if(planet_number >= PLANET_MAX) return;
	
	i = strlen(message);
	
	for(point=k=0;point<=i && k<20;k++)
	{
		split(temp_str,message,',',&point);
		if(k<10)
			planet[planet_number].good_price[k] = atoi(temp_str);
		else
			planet[planet_number].contra_price[k-10] = atoi(temp_str);
	}
	
	//now tell all poor souls
	sprintf(temp_str, "23,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%c",
		planet_number,
		planet[planet_number].good_price[0],
		planet[planet_number].good_price[1],
		planet[planet_number].good_price[2],
		planet[planet_number].good_price[3],
		planet[planet_number].good_price[4],
		planet[planet_number].good_price[5],
		planet[planet_number].good_price[6],
		planet[planet_number].good_price[7],
		planet[planet_number].good_price[8],
		planet[planet_number].good_price[9],
	        13);
	
	for(i=0;i<=user_con_top;i++)
		if (user_con[i].username[0])
			raw_send_con_user(i, temp_str);
}

void set_good_ammounts(int planet_number, char *message)
{
	int i, k, point;
	char temp_str[1024];
	
	//check for fucking around
	if(planet_number < 0) return;
	if(planet_number >= PLANET_MAX) return;
	
	i = strlen(message);
	
	for(point=k=0;point<=i && k<10;k++)
	{
		split(temp_str,message,',',&point);
		planet[planet_number].good_ammount[k] = atoi(temp_str);
	}
	
	//now tell any poor soul that happens to be on this planet.
	sprintf(temp_str, "24,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%c",
		planet_number,
		planet[planet_number].good_ammount[0],
		planet[planet_number].good_ammount[1],
		planet[planet_number].good_ammount[2],
		planet[planet_number].good_ammount[3],
		planet[planet_number].good_ammount[4],
		planet[planet_number].good_ammount[5],
		planet[planet_number].good_ammount[6],
		planet[planet_number].good_ammount[7],
		planet[planet_number].good_ammount[8],
		planet[planet_number].good_ammount[9],
	       	13);
	
	for(i=0;i<=user_con_top;i++)
		if (user_con[i].username[0] && user_con[i].last_planet == planet_number && user_con[i].landed)
			raw_send_con_user(i, temp_str);
}

void update_good_ammount(int p, int i, int ammount)
{
	int k;
	
	//check if hub is busting our balls
	if(p < 0) return;
	if(p >= PLANET_MAX) return;
	if(i < 0) return;
	if(i > 9) return;
	
	//set it
	planet[p].good_ammount[i] = ammount;
	
	//tell all clients on this planet
	for(k=0;k<=user_con_top;k++)
		if (user_con[k].username[0] && user_con[k].landed && user_con[k].last_planet == p)
			user_update_good_ammount(k,p,i);
}

void update_user_guild(char *user_name, char *new_guild, char *new_rank, int new_rank_id)
{
	
	int i;
	char rtn_str[200];
	
	i = user_to_con(user_name);
	
	if (i == -1) return;
	
	//cap
	new_guild[20] = '\0';
	new_rank[20] = '\0';
	
	if(!user_con[i].landed && !user_con[i].has_bot_guild)
		annouce_guild_member_leave_sector(user_con[i].zone, user_con[i].sector, user_con[i].guild, -1);
	
	strcpy(user_con[i].guild,new_guild);
	strcpy(user_con[i].guild_rank_name,new_rank);
	user_con[i].guild_rank_id = new_rank_id;
	
	user_con[i].has_bot_guild = is_bot_guild(user_con[i].guild);
	user_clear_has_member_sectors(i);
	user_resend_has_member_sectors(i);
	
	if(!user_con[i].landed && !user_con[i].has_bot_guild)
		annouce_guild_member_enter_sector(user_con[i].zone, user_con[i].sector, user_con[i].guild, -1);
	
	//now check if bar, because we'll have to readd the asshole if so
	if(user_con[i].inbar)
	{
		char temp_str2[1025];
		int k;
		
		sprintf(temp_str2,"8,%s,%s,%s,%d%c",
		user_con[i].username,
		user_con[i].guild,
		user_con[i].guild_rank_name,
		user_con[i].guild_rank_id,
		13);
	
		for(k=0;k<=user_con_top;k++)
		{
			if (!user_con[k].inbar) continue; //not in bar
			if (!user_con[k].landed) continue; //not landed
			if (user_con[k].last_planet != user_con[i].last_planet) continue; //not on the same planet
	
			raw_send_con_user(k,temp_str2);
		}
	}
}

void update_sector_good(int s, int z, int good_num, int good_amount)
{
	//reality check 
	if(good_num < 0) return;
	if(good_num > 9) return;
	
	//drop those goods to sector
	zone[z].sector[s].good_amount[good_num] = good_amount;

	//tell everyone
	relay_single_good_amount_sector(s,z,good_num);
}

void update_sector_contra(int s, int z, int good_num, int good_amount)
{
	//reality check 
	if(good_num < 0) return;
	if(good_num > 9) return;
	
	//drop those goods to sector
	zone[z].sector[s].contra_amount[good_num] = good_amount;

	//tell everyone
	relay_single_contra_amount_sector(s,z,good_num);
}

void send_hub_all_sector_info()
{
	int s, z, i;
	char rtn_str[1000];
	
	//send the "clear all sectors" flag  
	sprintf(rtn_str,"11,");
	send_con_hub(rtn_str);
	
	//send any sectors that have any goods 
	for(z=0;z<ZONE_MAX;z++)
		for(s=0;s<SECTOR_MAX;s++)
	{
		//some monkey checking 
		if(sector_has_goods(s,z))
			send_hub_sector_goods_info(s,z);
		if(sector_has_contras(s,z))
			send_hub_sector_contras_info(s,z);
	}
}

int sector_has_contras(int s, int z)
{
	int i, number_of = 0;
	
	for(i=0;i<10;i++)
		if(zone[z].sector[s].good_amount[i])
			number_of++;
	
	return number_of;
}

int sector_has_goods(int s, int z)
{
	int i, number_of = 0;
	
	for(i=0;i<10;i++)
		if(zone[z].sector[s].contra_amount[i])
			number_of++;
	
	return number_of;
}

void send_hub_sector_goods_info(int s, int z)
{
	char rtn_str[1000];
	
	//goods
	sprintf(rtn_str,"12,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",s,z,
		zone[z].sector[s].good_amount[0],
		zone[z].sector[s].good_amount[1],
		zone[z].sector[s].good_amount[2],
		zone[z].sector[s].good_amount[3],
		zone[z].sector[s].good_amount[4],
		zone[z].sector[s].good_amount[5],
		zone[z].sector[s].good_amount[6],
		zone[z].sector[s].good_amount[7],
		zone[z].sector[s].good_amount[8],
		zone[z].sector[s].good_amount[9]);
	send_con_hub(rtn_str);
}

void send_hub_sector_contras_info(int s, int z)
{
	char rtn_str[1000];
	
	//goods
	sprintf(rtn_str,"13,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",s,z,
		zone[z].sector[s].contra_amount[0],
		zone[z].sector[s].contra_amount[1],
		zone[z].sector[s].contra_amount[2],
		zone[z].sector[s].contra_amount[3],
		zone[z].sector[s].contra_amount[4],
		zone[z].sector[s].contra_amount[5],
		zone[z].sector[s].contra_amount[6],
		zone[z].sector[s].contra_amount[7],
		zone[z].sector[s].contra_amount[8],
		zone[z].sector[s].contra_amount[9]);
	send_con_hub(rtn_str);
}

void hub_update_sector_goods(int s, int z, char *message)
{
	int i, point = 0, len;
	char temp_str[1000];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	len = strlen(message);
	
	for(i=0;i<10 && point<=len;i++)
	{
		split(temp_str,message,',',&point);
		zone[z].sector[s].good_amount[i] = atoi(temp_str);
	}
}

void hub_update_sector_contras(int s, int z, char *message)
{
	int i, point = 0, len;
	char temp_str[1000];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	len = strlen(message);
	
	for(i=0;i<10 && point<=len;i++)
	{
		split(temp_str,message,',',&point);
		zone[z].sector[s].contra_amount[i] = atoi(temp_str);
	}
}

void hub_send_clear_sector_contraband(int z, int s)
{
	char rtn_str[1000];
	
	sprintf(rtn_str,"33,%d,%d",z,s);
	send_con_hub(rtn_str);
}

void hub_send_clear_sector_goods(int z, int s)
{
	char rtn_str[1000];
	
	sprintf(rtn_str,"34,%d,%d",z,s);
	send_con_hub(rtn_str);
}

void hub_add_sector_good(int s, int z, int g, int amount)
{
	char rtn_str[1000];
	
	sprintf(rtn_str,"16,%d,%d,%d,%d",s,z,g,amount);
	send_con_hub(rtn_str);
}

void hub_add_sector_contra(int s, int z, int g, int amount)
{
	char rtn_str[1000];
	
	sprintf(rtn_str,"17,%d,%d,%d,%d",s,z,g,amount);
	send_con_hub(rtn_str);
}

void send_hub_user_kill_bot_exp(int user_id, int exp, int the_class, char *the_guild)
{
	char rtn_str[300];
	
	sprintf(rtn_str,"24,%d,%d,%d,%s",user_id,exp,the_class,the_guild);
	send_con_hub(rtn_str);
}

void send_hub_user_kill_bot_random_drop(int user_id, struct bot_drop_db drop_type)
{
	char rtn_str[300];
	
	sprintf(rtn_str,"30,%d,%d,%d,%d,%d",
		user_id,
		drop_type.ship_class,
		drop_type.ship_kind,
		drop_type.weapon,
		drop_type.drop_type);
	send_con_hub(rtn_str);
}

void hub_relay_user_exp(int userid, int addition, int exp)
{
	int u;
	char message[100];
	
	u = userid_to_con(userid);
	
	if(u == -1) return;
	
	sprintf(message,"... Experience +%d, Your Current Experience: %d ..", addition, exp);
	user_send_public_chat(u,1,message);
}

void hub_set_sat_info(int p, char *message)
{
     int point = 0, do_repair = 0, i;
     char temp_str[1025];
     int old_sat_level;
     
     //printf("jew monkey sat!%d:%s\n",p, message);
     
     if(p<0) return;
     if(p>=PLANET_MAX) return;
     
     //set some old stuff
     old_sat_level = planet[p].sat.level;
     
     //set the new stuff
     split(temp_str, message, ',', &point);
     planet[p].sat.level = atoi(temp_str);
     
     split(temp_str, message, ',', &point);
     temp_str[20] = 0;
     strcpy(planet[p].sat.guild_name, temp_str);
     
     split(temp_str, message, ',', &point);
     planet[p].sat.money = atoi(temp_str);
     
     split(temp_str, message, ',', &point);
     planet[p].sat.money_collected = atoi(temp_str);
     
     for(i=0;i<SAT_LEVEL_MAX * SAT_GUN_INC;i++)
     {
          split(temp_str, message, ',', &point);
          planet[p].sat.gun[i] = atoi(temp_str);
          
          split(temp_str, message, ',', &point);
          planet[p].sat.gun_destroyed[i] = atoi(temp_str);
          
          //check
          if(planet[p].sat.gun[i]<0) planet[p].sat.gun[i] = -1;
          if(planet[p].sat.gun[i]>=GUN_MAX) planet[p].sat.gun[i] = -1;
     }      
     for(i=0;i<SAT_LEVEL_MAX * SAT_MISSILE_INC;i++)
     {
          split(temp_str, message, ',', &point);
          planet[p].sat.missile[i] = atoi(temp_str);
          
          split(temp_str, message, ',', &point);
          planet[p].sat.missile_left[i] = atoi(temp_str);
          
          //check
          if(planet[p].sat.missile[i]<0) planet[p].sat.missile[i] = -1;
          if(planet[p].sat.missile[i]>=MISSILE_MAX) planet[p].sat.missile[i] = -1;
     } 
     
     //now the hull
     split(temp_str, message, ',', &point);
     planet[p].sat.hull = atoi(temp_str);
     
     //now tell anyone who might want to know
     relay_planet_sat_info(p);
     
     //now check some stuff
     //sat just get destroyed or bought?
     if(old_sat_level == 0 && planet[p].sat.level > 0) add_sat_to_sector(p);
     if(old_sat_level > 0 && planet[p].sat.level == 0) add_sat_to_sector(p);
}

void repair_sat(int p, int new_money)
{
     if(p<0) return;
     if(p>=PLANET_MAX) return;
     
     planet[p].sat.hull = planet[p].sat.level * SAT_HULL_INC;
     planet[p].sat.money = new_money;
     
     //now tell any guild members that are landed of a hull change.
    relay_planet_sat_hull(p);
    relay_planet_sat_money(p);
}

void hub_set_sat_hull(int p, int amount)
{
     if(p<0) return;
     if(p>=PLANET_MAX) return;
     
     planet[p].sat.hull = amount;
     
     //now tell any guild members that are landed of a hull change.
    relay_planet_sat_hull(p);
}

void hub_set_sat_money(int p, int amount)
{
     if(p<0) return;
     if(p>=PLANET_MAX) return;
     
     planet[p].sat.money = amount;
     
     //now tell any guild members that are landed of a hull change.
    relay_planet_sat_money(p);
}

void send_hub_sat_hull(int p)
{
     char rtn_str[200];
     
     sprintf(rtn_str,"25,%d,%d",p,planet[p].sat.hull);
     send_con_hub(rtn_str);
}

void add_sat_to_sector(int p)
{
     int s,z,escape;
     
     escape=0;
     //find the sector it is in
     for(z=0;z<ZONE_MAX;z++)
         if(zone[z].name[0])
         {
             for(s=0;s<SECTOR_MAX;s++)
                 if(zone[z].sector[s].planet == p)
                 {
                     escape = 1;
                     break;                             
                 }
             
             if(escape) break;
         }
     
     //planet not found?
     if(!escape) return; 
     
     //does it exist?
     if(planet[p].sat.level)
     {      
        //"destroy" it first
        relay_player_blow_up(s,z,"Satellite");
        
        //add it
        relay_user_enter_sector(z,s,-1,0,0,"","Satellite",planet[p].sat.guild_name,-1);
     }
     else //add its broken self
     {
         relay_user_enter_sector(z,s,-1,0,1,"","Satellite","Not Owned",-1);
     }
}

void hub_set_sat_money_collected(int p, int amount)
{
     
     //check
     if(p<0 || p>=PLANET_MAX) return;
     if(amount < 0) return;
     
     //set
     planet[p].sat.money_collected = amount;
     
     //tell the people
     relay_planet_sat_info(p);
}

void hub_set_sat_missile_left(int p, int w)
{
     char rtn_str[200];
     
     sprintf(rtn_str,"26,%d,%d,%d",p,w,planet[p].sat.missile_left[w]);
     
     send_con_hub(rtn_str);
}

void hub_set_sat_gun_destroyed(int p, int w)
{
     char rtn_str[200];
     
     sprintf(rtn_str,"27,%d,%d,%d",p,w,planet[p].sat.gun_destroyed[w]);
     
     send_con_hub(rtn_str);
}

void hub_repair_user_gun(char *user_name, int g)
{
	int i;
	char rtn_str[200];
	
	//check the g
	if(g<0) return;
	if(g>=16) return;
	
	i = user_to_con(user_name);
	
	if (i == -1) return;
	
	//set the monkey
	user_con[i].ship.gun[g].destroyed = 0;
	
	set_average_gun_speed(i);
}

void hub_set_player_has_goods(int userid, int has_goods)
{
    int u;
    
    u = userid_to_con(userid);
    
    if(u == -1) return;
    
    //set!
    user_con[u].has_goods = has_goods;
}

void hub_clear_out_planet_trade_disasters()
{
     int p, g, is_low, i;
     
     for(p=0;p<PLANET_MAX;p++)
         if(planet[p].name[0] && !planet[p].is_gate)
             planet[p].trade_disaster_max = 0;
}

void hub_add_planet_trade_disaster(int p, int g, int is_low, int i)
{
	int d;
	
	//checks
	if(p<0) return;
	if(g<0) return;
	if(is_low<0) return;
	if(i<0) return;
	if(p>=PLANET_MAX) return;
	if(g>=10) return;
	if(is_low>=2) return;
	if(i>=MAX_TRADE_DISASTER) return;
	
	d = planet[p].trade_disaster_max;
	
	if(d >= PLANET_TRADE_DISASTER_MAX); //already got the max set?
	
	planet[p].trade_disaster[d].g = g;
	planet[p].trade_disaster[d].is_low = is_low;
	planet[p].trade_disaster[d].i = i;
	
	planet[p].trade_disaster_max++; //set higher
}

void hub_set_user_activated(int userid, int activated)
{
	int u;
	
	u = userid_to_con(userid);

	if(u == -1) return;
	
	//set!
	user_con[u].activated = activated;
}

void send_hub_user_zone_sector(int u)
{
	char rtn_str[200];
	
	sprintf(rtn_str,"29,%d,%d,%d", user_con[u].user_id, user_con[u].zone, user_con[u].sector);
	
	send_con_hub(rtn_str);  
}

void hub_ping_request()
{
	char send_str[20] = "31,";
	//31
	
	send_con_hub(send_str);
}

void ping_hub()
{
	char send_str[20] = "32,";
	//31
	
	send_con_hub(send_str);
}

void hub_ping_response()
{
	
}

void clear_sector_contraband_if_needed(int z, int s)
{
	int i;
	
	for(i=0;i<10;i++)
		if(zone[z].sector[s].contra_amount[i])
			break;
	
	if(i==10) return;
	
	hub_send_clear_sector_contraband(z,s);
}

void hub_annouce_sat_deposit(int p, char *username, int deposit_amount)
{
    char rtn_str[1024];
	int i;
     int s,z,escape;
     
     //p good?
     if(p<0) return;
     if(p>=PLANET_MAX) return;
     
     //cap
     username[20] = 0;
     
     escape=0;
     //find the sector it is in
     for(z=0;z<ZONE_MAX;z++)
         if(zone[z].name[0])
         {
             for(s=0;s<SECTOR_MAX;s++)
                 if(zone[z].sector[s].planet == p)
                 {
                     escape = 1;
                     break;                             
                 }
             
             if(escape) break;
         }
     
     //planet not found?
     if(!escape) return;
     
//	sprintf ( rtn_str,"10,1,... %d Deposited to the %s Satellite by %s ..%c", deposit_amount, planet[p].name, username, 13 );
	sprintf ( rtn_str,"10,1,... Satellite Deposit : %d to %s by %s ..%c", deposit_amount, planet[p].name, username, 13 );

	for ( i=0;i<=user_con_top;i++ )
	{
		if ( !user_con[i].username[0] || user_con[i].landed ) continue; //if connected and launched
		if ( s != user_con[i].sector || user_con[i].zone != z ) continue; //same sector

		raw_send_con_user ( i, rtn_str );
	}
}
