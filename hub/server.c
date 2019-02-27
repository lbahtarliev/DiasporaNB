#include "main.h"

int online_servers()
{
	int i, total = 0;
	
	for(i=0;i<=server_con_top;i++)
		if (server_con_id[i] != -1) total++;
		
	return total;
}

void server_logoff(int con)
{
	int i;
	int s;
	
	if (server_con_id[con] != -1) server[server_con_id[con]].debug = -1;
	server_con_id[con] = -1;
	server_con_version[con] = 0;
	
	for(i=0;i<=server_max;i++)
		if (server[i].con_id == con)
		{
			server[i].debug = -1;
			server[i].con_id = -1;
			
			//this command is done
			mysql_update_server_entry(i);
		}
}

void server_logoff_duplicates(int s)
{
	int i;
	
	for(i=0;i<=server_con_top;i++)
		if (server_con_id[i] == s)
			server_logoff(i);
}

void server_login(int con, char *login, char *password)
{
	int i;
	char rtn_str[150];
	
	for(i=0;i<=server_max;i++)
		if(str_match(login,server[i].login) && str_match(password,server[i].password)) break;
	
	if(i > server_max) return;
	
	//clear this con
	server_logoff(con);
	
	//no duplicates!!!!!!!!
	server_logoff_duplicates(i);
	
	//set this con up
	server_con_id[con] = i;
	server[i].con_id = con;
	if (server[i].real_address[0]) //if a real address is set
		strcpy(server[i].server_ip,server[i].real_address); //then use it
	else //otherwise get the one as we see it
		get_con_server_ip(con, server[i].server_ip);
	
	sprintf(rtn_str,"2,Greetings %s, may I take your jacket?", server[i].servername);
	send_con_server(con,rtn_str);
	
	//reference number
	sprintf(rtn_str,"11,%d", i);
	send_con_server(con,rtn_str);
	
	//send good prices etc
	server_send_good_contra_info(i);
	
	//send sat info
	server_send_all_sat_info(i);
	
	//ask for sector info
	sprintf(rtn_str,"18,");
	send_con_server(con,rtn_str);
	
	//this command is done
	mysql_update_server_entry(i);
}

void send_server_debug(int con, char *message)
{
;
}

void retrieve_user_info(int con, char *rtn_con, int user_id, unsigned long s_addrw)
{
	int i, k, z, found = 1, success = 0, temp[PLANET_MAX];
	int s;
	char rtn_str[1000];
	char ip_str[100];
	char ip_str2[100];
	
	for(i=0;i<=user_con_top;i++)
		if (user_con_id[i] != -1 && user_id == user[user_con_id[i]].user_id) 
	{
		//must also have the same ip the server claimed he has
		if(s_addrw != client_user[i].c_in.sin_addr.s_addr) 
		{
			get_con_user_ip(i, ip_str);
			printf(">> %s attempted login takeover of %s:%s  <<\n", ip_from_s_addr(s_addrw, ip_str2), user[user_con_id[i]].username, ip_str);
			continue;
		}
		
		success = 1;
		break;
	}
		
	if (!success)
	{
		sprintf(rtn_str,"3,%s,%d,", rtn_con, user_id);
		return;
	}
	
	i = user_con_id[i];
	s = server_con_id[con];
	
	if (user[i].server_id != s) //if you are jumping to this server
	{
		//sell all user goods
		sell_all_user_goods(i);
        
		user[i].server_id = s;
		user[i].last_planet = server_get_server_gate(s);
		
		if(user[i].last_planet == -1) //no server gate?
		    user[i].last_planet = server_get_least_ranged_planet(s);
		    
		if(user[i].last_planet == -1) //no planet at all!?
				user[i].last_planet = 0;
	}
	
	//give user new id
	/* temporarly removed.
	while(found)
	{
		user[i].user_id = rand() + rand();
		found = 0;
		for(k=0;k<=user_max;k++)
			if (user[i].user_id == user[k].user_id && i != k) found = 1;
	}*/
	
	if (user[i].home_id < 0 || user[i].home_planet < 0) //user has no home then we must find one
	{
		for(k=z=0;k<PLANET_MAX;k++)
		{
			if (server[s].planet[k].is_home && server[s].home_class == server[s].planet[k].home_class)
			{
				temp[z] = k;
				z++;
			}
		}
		
		if (z) //if we found some home planets
		{
			user[i].home_id = s;
			user[i].home_planet = temp[rand() % z];
			user[i].last_planet = user[i].home_planet;
			user[i].user_class = server[s].home_class;
		}
		else
		{
			printf("this server %d has no home planets set\n",s);//if the server has no home planets set then wtf?
		}
		
		//send the user the updated home planet info
		sprintf(rtn_str,"7,%d,%d", user[i].home_id, user[i].home_planet);
		send_con_user(user[i].con_id, rtn_str);
	}
	
	//even more planet stuff....
	user_check_ship_validity(i);
	
	//information sent to client
	
	//sprintf(rtn_str,"3,%d",user[i].user_id);
	//send_con_user(user[i].con_id,rtn_str);
	
	//information sent to server
	
	//information server needs to know initialy about the user connecting
	if (user[i].guild_id < 0 || user[i].guild_id > guild_max) 
		sprintf(rtn_str,"3,%s,%d,%s,,%d,,0", rtn_con, user_id, user[i].username, user[i].last_planet);
	else
		sprintf(rtn_str,"3,%s,%d,%s,%s,%d,%s,%d", 
            rtn_con, 
            user_id, 
            user[i].username, 
            guild[user[i].guild_id].name, 
            user[i].last_planet, 
            guild[user[i].guild_id].rank_name[user[i].guild_rank],
            user[i].guild_rank);
            
	send_con_server(con,rtn_str);
	
	send_user_ship_info_to_server(i);
	
	user_send_activated_server(i);
	
	//give user new id, send user server information, set user serverid 
}

int server_random_home_planet(int s_id, int the_class)
{
    int k, z, temp[PLANET_MAX];
    
    if(s_id < 0) return -1;
    if(s_id >= MAX_SERVER) return -1;
        
    for(k=z=0;k<PLANET_MAX;k++)
		{
			if (server[s_id].planet[k].is_home && the_class == server[s_id].planet[k].home_class)
			{
				temp[z] = k;
				z++;
			}
		}
		
		if (z) //if we found some home planets
            return temp[rand() % z];
		else
			return -1;
}

void set_user_launch(int user_id, int is_landed, int new_planet)
{
	int i;
	
	for(i=0;i<=user_con_top;i++)
		if (user_con_id[i] != -1 && user_id == user[user_con_id[i]].user_id) 
			break;
	
	if (i>user_con_top) return; //id not found
	
	i = user_con_id[i];
	
	user[i].landed = is_landed;
	
	if(is_landed)
		user[i].last_planet = new_planet;
}

void set_user_gun_destroyed(int user_id, int g)
{
     int u;
     int sh_sel;
     
     u = userid_to_id(user_id);
     
     if(u == -1) return; //not found
     
     //bogus check
     if(g<0) return;
     if(g>=16) return;
     
     //make the move
     sh_sel = user[u].ship_selected;
     
     //set it
     user[u].ship[sh_sel].gun_destroyed[g] = 1;
     
     //save it
     game.rewrite_user_db = 1;
}

void set_user_hull(int user_id, int new_hull)
{
	int i;
	
	for(i=0;i<=user_con_top;i++)
		if (user_con_id[i] != -1 && user_id == user[user_con_id[i]].user_id) 
			break;
	
	if (i>user_con_top) return; //id not found
	
	i = user_con_id[i];
	
	//check if this user has a ship currently
	if(user[i].ship[user[i].ship_selected].ship_id < 0) return;
	if(user[i].ship[user[i].ship_selected].brand < 0) return;
	if(user[i].ship[user[i].ship_selected].type < 0) return;
	
	//check for fucking around
	if(user[i].ship[user[i].ship_selected].hull_max < new_hull)
		new_hull = user[i].ship[user[i].ship_selected].hull_max;
	
	if(new_hull < 0)
		new_hull = 0;
	
	user[i].ship[user[i].ship_selected].hull = new_hull;
}

void destroy_user_disconnected(int con, char *username, int destroy_code, int z, int s, int killer_id)
{
	int u;
	int sh;
	int k_i;
	int the_worth_lost;
	
	u = user_to_id(username);
	k_i = userid_to_id(killer_id);
	
	if(u==-1)
	{
		printf("destroy_user_disconnected:user not found\n");
		return;
	}
	
	if(u==k_i)
	{
		printf("destroy_user_disconnected:error, killer found to be the victim\n");
		return;
	}
	
	//find the code
	for(sh=0;sh<10;sh++)
		if(user[u].ship[sh].destroy_code == destroy_code)
		break;
		
	if(sh==10) 
	{
		printf("destroy_user_disconnected:ship not found\n");
		return; //not found
	}
	
	//ship exist?
	if(user[u].ship[sh].ship_id < 0) return;
	if(user[u].ship[sh].brand < 0) return;
	if(user[u].ship[sh].type < 0) return;
	
	//we'll need this
	the_worth_lost = user_ship_worth_lost(u, user[u].ship_selected);
	
	//"launch" the user to get by some stuff
	user[u].landed = 0;
	
	//settt this
	user_set_zone_sector(u,z,s);
	
	//drop!
	user_drop_all_goods(u);
	
	//sell ship
	user_sell_ship(u,sh);
	
	//send a bitch you know where
	user_send_home(u);
	
	//"reland" the user
	user[u].landed = 1;
	
	award_killer(k_i, u, the_worth_lost);
	
	//do this db stuff
	game.rewrite_user_db = 1;	
}

void destroy_user(int con, int user_id, int killer_id)
{
	int i, k_i, k, f, z, temp[PLANET_MAX];
	char rtn_str[1000];
	int the_worth, the_worth_lost;
	
	//find user
	for(i=0;i<=user_con_top;i++)
		if (user_con_id[i] != -1 && user_id == user[user_con_id[i]].user_id) 
			break;
	
	if (i>user_con_top) return; //id not found	
		
	//switch over to user mem loc
	i = user_con_id[i];
	f = server_con_id[con];
	
	//we need this
	the_worth_lost = user_ship_worth_lost(i, user[i].ship_selected);
	the_worth = the_worth_lost * 3;
	
	//drop the goods
	user_drop_all_goods(i);
	
	//sell ship
	sell_user_ship_chassis(i);
	
	//"land" user
	user[i].landed = 1;
	
	//send a bitch you know where
	user_send_home(i);
	
	//tell the server what planet this character should now be at
	sprintf(rtn_str,"8,%s,%d", user[i].username, user[i].home_planet);
	send_con_server(con, rtn_str);
	
	//tell the dier
	sprintf(rtn_str,"... %d DC salvaged from a ship wreckage  ..", the_worth);
	send_user_private_chat(user[i].con_id,1,rtn_str);
	
	//award the killer
	k_i = userid_to_id(killer_id);
	award_killer(k_i, i, the_worth_lost);
	
	//do this db stuff
	game.rewrite_user_db = 1;
}

void award_killer(int k_i, int i, int the_worth_lost)
{
	int b_u;
	int money_owned;
	
	//checks
	if(k_i < 0) return;
	if(i < 0) return;
	if(k_i >= MAX_USER) return;
	if(i >= MAX_USER) return;
	
	//simple kill reward
	reward_kill_reward(k_i, i);
		
	b_u = user_to_bounty(i);
		
	//victom on the bounty list?
	if(b_u != -1)
	{
		//checks
		reward_bounty(b_u, k_i, the_worth_lost / 2);
	}
}

void set_user_payloads(int user_id, char *message)
{
	int i, z, temp_missile_ammount, point = 0;
	char temp_str[1025];
	
	for(i=0;i<=user_con_top;i++)
		if (user_con_id[i] != -1 && user_id == user[user_con_id[i]].user_id) 
			break;
	
	if (i>user_con_top) return; //id not found
	
	i = user_con_id[i];
	
	//check if this user has a ship currently
	if(user[i].ship[user[i].ship_selected].ship_id < 0) return;
	if(user[i].ship[user[i].ship_selected].brand < 0) return;
	if(user[i].ship[user[i].ship_selected].type < 0) return;
	
	temp_missile_ammount = server[user[i].ship[user[i].ship_selected].ship_id].ship[user[i].ship[user[i].ship_selected].brand].missile_ammount[user[i].ship[user[i].ship_selected].type];
	
	for(z=0;z<temp_missile_ammount;z++)
	{
		split(temp_str,message,',',&point);
		user[i].ship[user[i].ship_selected].missile_left[z] = atoi(temp_str);
		
		if (user[i].ship[user[i].ship_selected].missile_left[z] > 4)
			user[i].ship[user[i].ship_selected].missile_left[z] = 4;
		if(user[i].ship[user[i].ship_selected].missile_left[z] < 0)
			user[i].ship[user[i].ship_selected].missile_left[z] = 0;
	}
}

void do_price_change()
{
	int i, j, k, d, difference;
	char rtn_str[1024];
	
	//set the time
	game.last_pricechange = current_time();
	
	//set all the planet's good ammounts, even for the offline servers
	for(i=0;i<=server_max;i++)
		for(j=0;j<PLANET_MAX;j++)
			if(server[i].planet[j].name[0] != '\0')
			{
			    //normal good shit
            	for(k=0;k<10;k++)
				{
					
					difference = server[i].planet[j].good_price_high[k] - server[i].planet[j].good_price_low[k];
					if (difference) server[i].planet[j].good_price[k] = (rand() % difference) + server[i].planet[j].good_price_low[k];
					
					difference = server[i].planet[j].good_ammount_high[k] - server[i].planet[j].good_ammount_low[k];
					if (difference) server[i].planet[j].good_ammount[k] = (rand() % difference) + server[i].planet[j].good_ammount_low[k];
					
					difference = server[i].planet[j].contra_price_high[k] - server[i].planet[j].contra_price_low[k];
					if (difference) server[i].planet[j].contra_price[k] = (rand() % difference) + server[i].planet[j].contra_price_low[k];
					
					difference = server[i].planet[j].contra_ammount_high[k] - server[i].planet[j].contra_ammount_low[k];
					if (difference) server[i].planet[j].contra_ammount[k] = (rand() % difference) + server[i].planet[j].contra_ammount_low[k];
				}
				
				//now for the crowd pleaser
				server[i].planet[j].trade_disaster_max = (rand() % PLANET_TRADE_DISASTER_MAX) + 1;
				for(d=0;d<server[i].planet[j].trade_disaster_max;d++)
				{
                    int g, is_low;

				    //set it
                    set_random_trade_disaster(&(server[i].planet[j].trade_disaster[d]));
				    
				    //now do the goodness
				    g = server[i].planet[j].trade_disaster[d].g;
				    is_low = server[i].planet[j].trade_disaster[d].is_low;
				    
				    if(is_low)
				    {
    				    //set price
    				    difference = (int)((server[i].planet[j].good_price_high[g] - server[i].planet[j].good_price_low[g]) * 0.10);
    				    if (difference) server[i].planet[j].good_price[g] = (rand() % difference) + server[i].planet[j].good_price_low[g];
    				    
    				    //set amount
    					difference = (int)((server[i].planet[j].good_ammount_high[g] - server[i].planet[j].good_ammount_low[g]) * 0.10);
    					if (difference) server[i].planet[j].good_ammount[g] = (rand() % difference) + server[i].planet[j].good_ammount_low[g];
                    }
                    else
				    {
    				    //set price
    				    difference = (int)((server[i].planet[j].good_price_high[g] - server[i].planet[j].good_price_low[g]) * 0.10);
    				    if (difference) server[i].planet[j].good_price[g] = server[i].planet[j].good_price_high[g] - (rand() % difference);
    				    
    				    //set amount
    					difference = (int)((server[i].planet[j].good_ammount_high[g] - server[i].planet[j].good_ammount_low[g]) * 0.10);
    					if (difference) server[i].planet[j].good_ammount[g] = server[i].planet[j].good_ammount_high[g] - (rand() % difference);
                    }
                }
            }
				
	//now send to connected servers.
	for(i=0;i<=server_max;i++)
		if(server[i].con_id > -1)
			server_send_good_contra_info(i);
}

void server_send_good_contra_info(int i)
{
	int j, d;
	char rtn_str[1000];
	char clr_str[50] = "30,";
	
	//first tell them to clear out the disaster info
	send_con_server(server[i].con_id, clr_str);
	
	for(j=0;j<PLANET_MAX;j++)
		if(server[i].planet[j].name[0] != '\0')
	{
		sprintf(rtn_str, "4,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			j,
			server[i].planet[j].good_price[0],
			server[i].planet[j].good_price[1],
			server[i].planet[j].good_price[2],
			server[i].planet[j].good_price[3],
			server[i].planet[j].good_price[4],
			server[i].planet[j].good_price[5],
			server[i].planet[j].good_price[6],
			server[i].planet[j].good_price[7],
			server[i].planet[j].good_price[8],
			server[i].planet[j].good_price[9],
			server[i].planet[j].contra_price[0],
			server[i].planet[j].contra_price[1],
			server[i].planet[j].contra_price[2],
			server[i].planet[j].contra_price[3],
			server[i].planet[j].contra_price[4],
			server[i].planet[j].contra_price[5],
			server[i].planet[j].contra_price[6],
			server[i].planet[j].contra_price[7],
			server[i].planet[j].contra_price[8],
			server[i].planet[j].contra_price[9]);
		
		
		send_con_server(server[i].con_id, rtn_str);
					
		sprintf(rtn_str, "5,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			j,
			server[i].planet[j].good_ammount[0],
			server[i].planet[j].good_ammount[1],
			server[i].planet[j].good_ammount[2],
			server[i].planet[j].good_ammount[3],
			server[i].planet[j].good_ammount[4],
			server[i].planet[j].good_ammount[5],
			server[i].planet[j].good_ammount[6],
			server[i].planet[j].good_ammount[7],
			server[i].planet[j].good_ammount[8],
			server[i].planet[j].good_ammount[9]);
					
		send_con_server(server[i].con_id, rtn_str);
		
		//now setup a disaster packet
		for(d=0;d<server[i].planet[j].trade_disaster_max;d++)
		{
			sprintf(rtn_str, "31,%d,%d,%d,%d",
				j,
				server[i].planet[j].trade_disaster[d].g,
				server[i].planet[j].trade_disaster[d].is_low,
				server[i].planet[j].trade_disaster[d].i);
			
			send_con_server(server[i].con_id, rtn_str);
		}
	}
}

void server_update_good_ammount(int i, int planet_num, int good_num)
{
	char rtn_str[200];
	
	sprintf(rtn_str,"12,%d,%d,%d",planet_num,good_num,server[i].planet[planet_num].good_ammount[good_num]);
	send_con_server(server[i].con_id, rtn_str);
}

void server_clear_sector_amounts(int con)
{
	int i, s, z, g;
	
	i = server_con_id[con];
	
	for(z=0;z<ZONE_MAX;z++)
		for(s=0;s<SECTOR_MAX;s++)
			for(g=0;g<10;g++)
	{
		server[i].zone[z].sector[s].good_amount[g] = 0;
		server[i].zone[z].sector[s].contra_amount[g] = 0;
	}
	
}

void server_set_good_amounts(int con, int s, int z, char *message)
{
	int i, g;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
}

void server_set_contra_amounts(int con, int s, int z, char *message)
{
	int i, g;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
}

void server_set_good_amount(int con, int s, int z, int good_num, int good_amount)
{
	int i;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	//and again
	if(good_num < 0) return;
	if(good_num > 9) return;
	
	server[i].zone[z].sector[s].good_amount[good_num] = good_amount;
	
	//update a bitch
	server_send_sector_good_amount(i,s,z,good_num);
}

void server_set_contra_amount(int con, int s, int z, int good_num, int good_amount)
{
	int i;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	//and again
	if(good_num < 0) return;
	if(good_num > 9) return;
	
	server[i].zone[z].sector[s].contra_amount[good_num] = good_amount;
	
	//update a bitch
	server_send_sector_contra_amount(i,s,z,good_num);
}

void server_add_good_amount(int con, int s, int z, int good_num, int good_amount)
{
	int i;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	//and again
	if(good_num < 0) return;
	if(good_num > 9) return;
	
	server[i].zone[z].sector[s].good_amount[good_num] += good_amount;
	
	//update a bitch
	server_send_sector_good_amount(i,s,z,good_num);
}

void server_add_contra_amount(int con, int s, int z, int good_num, int good_amount)
{
	int i;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	//and again
	if(good_num < 0) return;
	if(good_num > 9) return;
	
	server[i].zone[z].sector[s].contra_amount[good_num] += good_amount;
	
	//update a bitch
	server_send_sector_contra_amount(i,s,z,good_num);
}

void server_send_sector_good_amount(int i, int s, int z, int g)
{
	char rtn_str[1000];
	
	sprintf(rtn_str,"14,%d,%d,%d,%d",s,z,g,server[i].zone[z].sector[s].good_amount[g]);
	send_con_server(server[i].con_id, rtn_str);
}

void server_send_sector_contra_amount(int i, int s, int z, int g)
{
	char rtn_str[1000];
	
	sprintf(rtn_str,"15,%d,%d,%d,%d",s,z,g,server[i].zone[z].sector[s].contra_amount[g]);
	send_con_server(server[i].con_id, rtn_str);
}

void server_bot_pickup_good(int con, int t, int u, int s, int z, int g, int amount)
{
	int i;
	char rtn_str[1000];
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	//and again
	if(g < 0) return;
	if(g > 9) return;
	if(amount <= 0) return;
	
	//that sector got the goods? 
	if(server[i].zone[z].sector[s].good_amount[g] < amount) return;
	
	//make the exchange
	server[i].zone[z].sector[s].good_amount[g] -= amount;
	
	//update a bitch
	server_send_sector_good_amount(i,s,z,g);
	
	//tell the server that bot has goods!
	sprintf(rtn_str,"19,%d,%d,%d,%d",t,u,g,amount);
	send_con_server(con, rtn_str);
}

void server_bot_pickup_contra(int con, int t, int u, int s, int z, int g, int amount)
{
	int i;
	char rtn_str[1000];
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	//and again
	if(g < 0) return;
	if(g > 9) return;
	if(amount <= 0) return;
	
	//that sector got the goods? 
	if(server[i].zone[z].sector[s].contra_amount[g] < amount) return;
	
	//make the exchange
	server[i].zone[z].sector[s].contra_amount[g] -= amount;
	
	//update a bitch
	server_send_sector_contra_amount(i,s,z,g);
	
	//tell the server that bot has goods!
	sprintf(rtn_str,"20,%d,%d,%d,%d",t,u,g,amount);
	send_con_server(con, rtn_str);
}

void server_user_pickup_good(int con, int user_id, int s, int z, int g, int amount)
{
	int i, u, ss;
	
	//got any killers?
	if(s<0) return;
	if(z<0) return;
	if(g<0) return;
	if(s>=SECTOR_MAX) return;
	if(z>=ZONE_MAX) return;
	if(g>=10) return;
	
	//amount real...?
	if(amount < 1) return;
	
	i = server_con_id[con];
	
	//this user exist.. ?
	if((u = userid_to_id(user_id)) == -1) return;
	
	//that user on this server?
	if(user[u].server_id != i) return;
	
	//user launched?
	if(user[u].landed) return;
	
	//k for the good checking...
	
	//got that good here?
	if(server[i].zone[z].sector[s].good_amount[g] < amount) return;
	
	ss = user[u].ship_selected;
	
	//the user got the room?
	if(user[u].ship[ss].cargo_max - user_cargo_amount(u) < amount) return;
	
	//well make the exchange..
	server[i].zone[z].sector[s].good_amount[g] -= amount;
	user[u].ship[ss].cargo[g] += amount;
	
	//update a bitch
	server_send_sector_good_amount(i,s,z,g);
	
	//and another bitch
	user_update_good_ammount(user[u].con_id,g);
	
	//tell a bitch
	set_and_send_server_user_has_goods(u,1);
}

void server_user_pickup_contra(int con, int user_id, int s, int z, int g, int amount)
{
	int i, u, ss;
	
	//got any killers?
	if(s<0) return;
	if(z<0) return;
	if(g<0) return;
	if(s>=SECTOR_MAX) return;
	if(z>=ZONE_MAX) return;
	if(g>=10) return;
	
	//amount real...?
	if(amount < 1) return;
	
	i = server_con_id[con];
	
	//this user exist.. ?
	if((u = userid_to_id(user_id)) == -1) return;
	
	//that user on this server?
	if(user[u].server_id != i) return;
	
	//user launched?
	if(user[u].landed) return;
	
	//k for the good checking...
	
	//got that good here?
	if(server[i].zone[z].sector[s].contra_amount[g] < amount) return;
	
	ss = user[u].ship_selected;
	
	//the user got the room?
	if(user[u].ship[ss].cargo_max - user_cargo_amount(u) < amount) return;
	
	//well make the exchange..
	server[i].zone[z].sector[s].contra_amount[g] -= amount;
	user[u].ship[ss].contra[g] += amount;
	
	//update a bitch
	server_send_sector_contra_amount(i,s,z,g);
	
	//and another bitch
	user_update_contra_ammount(user[u].con_id,g);
	
	//tell a bitch
	set_and_send_server_user_has_goods(u,1);
}

void server_user_drop_good(int con, int user_id, int s, int z, int g, int amount)
{
	int i, u;
	
	//amount real...?
	if(amount < 1) return;
	
	i = server_con_id[con];

	//this user exist.. ?
	if((u = userid_to_id(user_id)) == -1) return;
	
	//that user on this server?
	if(user[u].server_id != i) return;
	
	//set the zone/sector
	user_set_zone_sector(u,z,s);
	
	//drop the good;
	user_drop_good(u, g, amount);
}

void server_user_drop_contra(int con, int user_id, int s, int z, int g, int amount, int g_price)
{
	int i, u;
	
	//amount real...?
	if(amount < 1) return;
	
	i = server_con_id[con];

	//this user exist.. ?
	if((u = userid_to_id(user_id)) == -1) return;
	
	//that user on this server?
	if(user[u].server_id != i) return;
	
	//set the zone/sector
	user_set_zone_sector(u,z,s);
	
	//drop the good;
	user_drop_contra(u, g, amount, g_price);
}

void server_user_give_bot_exp(int con, int user_id, int exp, int the_class, char *the_guild)
{
	int i, u, addition;
	char rtn_str[1024];
	
	//this user exist.. ?
	if((u = userid_to_id(user_id)) == -1) return;
	
	//so check for rewards first
	user_award_guild_reward(u, the_guild);
	
	//well for the sake of checking something
	if(exp <= 0) return;
	
	i = server_con_id[con];
	
	//that user on this server?
	if(user[u].server_id != i) return;
	
	//well do the stuff
	addition = user_exp_algo(user[u].exp,exp);
	
	//do class / kills stuff
    	user_add_kill(u, the_class, addition);
	
	//anything even happening?
	if(addition <= 0) return;
	
	//make the addition
	user[u].exp += addition;
	
	//and tell the user about it
	user_send_exp(u);
	
	//tell the server to tell the client
	sprintf(rtn_str,"22,%d,%d,%d", user_id, addition, user[u].exp);
	send_con_server(con,rtn_str);
	
	//do this db stuff
	game.rewrite_user_db = 1;
}

void server_update_user_guild(int s, int u)
{
    int g, r;
    char *username, *guild_name, *guild_rank_name;
    char rtn_str[200];
    
    //yaa
    if(s < 0) return;
    if(u < 0) return;
    if(s >= MAX_SERVER) return;
    if(u >= MAX_USER) return;
    
    g = user[u].guild_id;
    r = user[u].guild_rank;
    
    //some checks never hurt
    if(g < 0) return;
    if(r < 0) return;
    if(g >= MAX_GUILD) return;
    if(r >= 6) return;
    
    username = user[u].username;
    guild_name = guild[g].name;
    guild_rank_name = guild[g].rank_name[r];
    
    sprintf(rtn_str,"13,%s,%s,%s,%d", username, guild_name, guild_rank_name,r);
	send_con_server(server[s].con_id,rtn_str);
}

void server_send_all_sat_info(int s)
{
     int p;
     
     for(p=0;p<PLANET_MAX;p++)
          if(server[s].planet[p].name[0])
               server_send_sat_info(s,p);
}

void server_send_sat_deposit_info(int s, int p, char *username, int deposit_amount)
{
     char rtn_str[1025];
     
     sprintf(rtn_str, "35,%d,%s,%d", p, username, deposit_amount);
         
     send_con_server(server[s].con_id, rtn_str);
}

void server_send_sat_info(int s, int p)
{
     char rtn_str[1025], append_str[200];
     int i, g;
     
     g = server[s].planet[p].sat.guild_id;
     
     //standard stuff
     if(g > -1 && g < MAX_GUILD)
         sprintf(rtn_str,"23,%d,%d,%s,%d,%d",
         p,
         server[s].planet[p].sat.level,
         guild[g].name,
         server[s].planet[p].sat.money,
         server[s].planet[p].sat.money_collected);
     else
         sprintf(rtn_str,"23,%d,%d,,%d,%d",
         p,
         server[s].planet[p].sat.level,
         server[s].planet[p].sat.money,
         server[s].planet[p].sat.money_collected);

     //guns
     for(i=0;i<SAT_LEVEL_MAX * SAT_GUN_INC;i++)
     {
          sprintf(append_str, ",%d,%d", server[s].planet[p].sat.gun[i], server[s].planet[p].sat.gun_destroyed[i]);
          strcat(rtn_str, append_str);
     }

     //missile
     for(i=0;i<SAT_LEVEL_MAX * SAT_MISSILE_INC;i++)
     {
          sprintf(append_str, ",%d,%d", server[s].planet[p].sat.missile[i], server[s].planet[p].sat.missile_left[i]);
          strcat(rtn_str, append_str);
     }
     
     sprintf(append_str, ",%d", server[s].planet[p].sat.hull);
     strcat(rtn_str, append_str);

     send_con_server(server[s].con_id, rtn_str);
     printf("sent sat:%s\n", rtn_str);
}

void server_relay_sat_repair(int s, int p)
{
     char rtn_str[200];
     
     if(s<0) return;
     if(s>=MAX_SERVER) return;
     if(p<0) return;
     if(p>=PLANET_MAX) return;
     
     sprintf(rtn_str, "24,%d,%d", p, server[s].planet[p].sat.money);
     
     send_con_server(server[s].con_id, rtn_str);
}

void server_relay_sat_hull(int s, int p)
{
    char rtn_str[50];
    
    sprintf(rtn_str,"26,%d,%d",p, server[s].planet[p].sat.hull);

    send_con_server(server[s].con_id, rtn_str);
}

void server_relay_sat_money(int s, int p)
{
    char rtn_str[50];
    
    sprintf(rtn_str,"27,%d,%d",p, server[s].planet[p].sat.money);

    send_con_server(server[s].con_id, rtn_str);
}

int server_get_server_gate(int s)
{
    int i;
    
    for(i=0;i<PLANET_MAX;i++)
        if(server[s].planet[i].is_server_gate)
            return i;
            
    return -1;
}

int server_gate_random_planet(int s)
{
    int i, p[PLANET_MAX], p_max;
    
    for(i=p_max=0;i<PLANET_MAX;i++)
        if(server[s].planet[i].name[0])
        {
             p[p_max] = i;
             p_max++;                          
        }
        
    if(p_max)
        return p[rand() % p_max];
    else
        return -1;
}

int server_get_least_ranged_planet(int s)
{
    int i, p, p_range;
    int z, sec;
    
    p = -1;
    
    for(i=0;i<PLANET_MAX;i++)
        if(server[s].planet[i].name[0])
        {
             int i_range;
             
             //find this planets sector
             for(z=0;z<ZONE_MAX;z++)
                 if(server[s].zone[z].name[0])
                     for(sec=0;sec<SECTOR_MAX;sec++)
                         if(server[s].zone[z].sector[sec].planet == i)
                             i_range = server[s].zone[z].sector[sec].range;
             
             if(p == -1) {p=i; p_range = i_range; continue;}
             if(i_range < p_range) {p=i; p_range = i_range; continue;}
        }
        
    return p;
}

void server_set_sat_hull(int con, int p, int new_hull)
{
     int s;
     
     s = server_con_id[con];
     
     //checks
     if(p<0 || p>=PLANET_MAX) return;

     //planet have a sat?

     if(server[s].planet[p].sat.guild_id < 0) return;
     if(server[s].planet[p].sat.hull <= 0) return;
     if(server[s].planet[p].sat.level <= 0) return;

     //set the hull
     server[s].planet[p].sat.hull = new_hull;

     //check hull
     if(server[s].planet[p].sat.hull > server[s].planet[p].sat.level * SAT_HULL_INC) 
         server[s].planet[p].sat.hull = server[s].planet[p].sat.level * SAT_HULL_INC;
     if(server[s].planet[p].sat.hull < 0) 
         server[s].planet[p].sat.hull = 0;
         
     //destroy the sat?
     if(!server[s].planet[p].sat.hull)
         destroy_sat(s,p);
         
     //well tell the crew
     relay_sat_under_attack_warning(s,p);
}

void server_set_sat_missile_left(int con, int p, int w, int amount)
{
     int s;
     
     s = server_con_id[con];
     
     //checks
     if(p<0 || p>=PLANET_MAX) return;
     
     //w good?
     if(w<0) return;
     if(w>=SAT_LEVEL_MAX * SAT_MISSILE_INC) return;
     
     //amount need fixed?
     if(amount<0) amount = 0;
     if(amount>4) amount = 4;
     
     //set it
     server[s].planet[p].sat.missile_left[w] = amount;
}

void server_set_sat_gun_destroyed(int con, int p, int w, int is_destroyed)
{
     int s;
     
     s = server_con_id[con];
     
     //checks
     if(p<0 || p>=PLANET_MAX) return;
     
     //w good?
     if(w<0) return;
     if(w>=SAT_LEVEL_MAX * SAT_GUN_INC) return;
     
     //is_destroyed need fixed?
     if(is_destroyed<0) is_destroyed = 0;
     if(is_destroyed>1) is_destroyed = 1;
     
     //set it
     server[s].planet[p].sat.gun_destroyed[w] = is_destroyed;
}

void destroy_sat(int s, int p)
{
	int w;
	
	
	//we unbugging it?
	if((server[s].planet[p].sat.level * 100) / SAT_LEVEL_MAX >= (rand() % 100) + 1)
		server[s].planet[p].sat.bug_guild_id = -1;
	
	//well start by clearing the stats
	server[s].planet[p].sat.guild_id = -1;
	server[s].planet[p].sat.hull = 0;
	server[s].planet[p].sat.level = 0;
	server[s].planet[p].sat.money = 0;
	server[s].planet[p].sat.money_collected = 0;
	
	for(w=0;w<SAT_LEVEL_MAX * SAT_GUN_INC;w++)
	{
		server[s].planet[p].sat.gun[w] = -1;
		server[s].planet[p].sat.gun_destroyed[w] = 0;
	}
	for(w=0;w<SAT_LEVEL_MAX * SAT_MISSILE_INC;w++)
	{
		server[s].planet[p].sat.missile[w] = -1;
		server[s].planet[p].sat.missile_left[w] = 0;
	}
	
	//tell the server
	server_send_sat_info(s,p);
	
	//magic.
	mysql_update_sat_entry(s,p);
	
	//set rewrite db flag
	server[s].rewrite_sat_db = 1;
}

void award_all_sat_taxes()
{
     int s, p;
     
     
	for(s=0;s<=server_max;s++)
		if(server[s].con_id > -1)
			for(p=0;p<PLANET_MAX;p++)
				if(server[s].planet[p].name[0] && server[s].planet[p].sat.guild_id > -1)
				{
					int award_amount;
					
					award_amount = server[s].planet[p].sat.level * SAT_TAX_INC;
					
					pay_planet_guild(s, p, award_amount);
					
					//tell the server
					send_server_sat_money_collected(s,p);
				}
}

void send_server_sat_money_collected(int s, int p)
{
     char rtn_str[200];
     
     sprintf(rtn_str,"25,%d,%d",p,server[s].planet[p].sat.money_collected);
     
     send_con_server(server[s].con_id, rtn_str);
}

void award_guild_money(int g, int amount)
{
     int u;
     int total_deposited;
     
     //simple checks
     if(g<0 || g>=MAX_GUILD) return;
     if(amount <= 0) return;
     
     //eh well add to the bank
     guild[g].bank_money += amount;
     
     //give the owner 20% if he is in the guild
     u = user_to_id(guild[g].owner);
     
     //welp
     if(u > -1)
     {
          //this user in this guild still?
          if(user[u].guild_id == g)
          {
              //well update the balance
              user[u].guild_balance += (int)(0.20 * amount);
	      user[u].guild_balance_update = 1;
          }
     }
     
     //now give the rest of the guild what they are owed
     
     //collect the total deposit
     total_deposited = 0;
     for(u=0;u<=user_max;u++)
         if(user[u].guild_id == g)
             if(user[u].guild_balance > 0)
                 total_deposited += user[u].guild_balance;
                 
                 
     //guild got anyone who deserves this loot?
     if(total_deposited)
         for(u=0;u<=user_max;u++)
             if(user[u].guild_id == g)
                 if(user[u].guild_balance > 0)
                 {
                     user[u].guild_balance += (int)((0.5 * user[u].guild_balance / total_deposited) * amount);
		     user[u].guild_balance_update = 1;
                 }
                 
     game.rewrite_guild_db = 1;
}

void check_and_repair_all_sats()
{
	int s, p;
	
	for(s=0;s<=server_max;s++)
		if(server[s].con_id > -1 && server[s].servername[0])
		for(p=0;p<PLANET_MAX;p++)
		if(server[s].planet[p].name[0])
		{
			int w;
			int hull_max;
			int gun_max, missile_max;
			int done_update = 0;
			
			//sat owned?
			if(server[s].planet[p].sat.level <= 0) continue;
			if(server[s].planet[p].sat.guild_id < 0) continue;
			
			//got money?
			if(server[s].planet[p].sat.money <= 0) continue;
			
			//check hull
			hull_max = server[s].planet[p].sat.level * SAT_HULL_INC;
			if(server[s].planet[p].sat.hull < server[s].planet[p].sat.level * SAT_HULL_INC)
			{
			int hull_diff, temp_cost;
			
			//got the money for the repair?
			hull_diff = hull_max - server[s].planet[p].sat.hull;
			temp_cost = hull_diff * 10;
			if(temp_cost <= server[s].planet[p].sat.money)
			{
				//repair
				server[s].planet[p].sat.hull = hull_max;
				
				//lose money
				server[s].planet[p].sat.money -= hull_diff * 10;
				
				//give the little reminder
				server_relay_sat_repair(s,p);
				
				//and set this
				server[s].rewrite_sat_db = 1;
			}
			else //only repairing some here
			{
					if(!server[s].planet[p].sat.money) continue;
					
					//repair
					server[s].planet[p].sat.hull += server[s].planet[p].sat.money / 10;
					
					//double check hull
					if(server[s].planet[p].sat.hull > hull_max)
						server[s].planet[p].sat.hull = hull_max;
					
					//lose money
					server[s].planet[p].sat.money = 0;
					
					//give the little reminder
					server_relay_sat_hull(s,p);
					server_relay_sat_money(s,p);
					
					mysql_update_sat_entry(s,p);
					
					//and set this
					server[s].rewrite_sat_db = 1;
			}
			}
			
			//check guns
			gun_max = server[s].planet[p].sat.level * SAT_GUN_INC;
			for(w=0;w<gun_max;w++)
			if(server[s].planet[p].sat.gun_destroyed[w])
			{
				int temp_cost, w_i;
				
				//double check the weapon....
				w_i = server[s].planet[p].sat.gun[w];
				if(w_i<0 || w_i>=GUN_MAX) continue;
				
				temp_cost = (int)(0.25 * server[s].gun[w_i].cost);
				
				//got the money?
				if(server[s].planet[p].sat.money < temp_cost) continue;
				
				//make the repair
				server[s].planet[p].sat.gun_destroyed[w] = 0;
				
				//take the money
				server[s].planet[p].sat.money -= temp_cost;
				
				//tell the people
				done_update = 1;
				
				//and set this
				server[s].rewrite_sat_db = 1;
			}
			
			//check missiles
			gun_max = server[s].planet[p].sat.level * SAT_MISSILE_INC;
			for(w=0;w<gun_max;w++)
			if(server[s].planet[p].sat.missile_left[w] < 4)
			{
				int temp_cost, w_i;
				
				//double check the weapon....
				w_i = server[s].planet[p].sat.missile[w];
				if(w_i<0 || w_i>=MISSILE_MAX) continue;
				
				temp_cost = (int)((1.0 * (4 - server[s].planet[p].sat.missile_left[w]) / 4) * server[s].missile[w_i].cost);
				
				//got the money?
				if(server[s].planet[p].sat.money < temp_cost) continue;
				
				//make the repair
				server[s].planet[p].sat.missile_left[w] = 4;
				
				//take the money
				server[s].planet[p].sat.money -= temp_cost;
				
				//tell the people
				done_update = 1;
				
				//and set this
				server[s].rewrite_sat_db = 1;
			}
			
			if(done_update) server_send_sat_info(s,p);
		}
}

void set_and_send_server_user_has_goods(int u, int has_goods)
{
     int s;
     char rtn_str[200];
     
     //check
     if(user[u].has_goods && has_goods) return;
     if(!user[u].has_goods && !has_goods) return;
     
     //okay set
     user[u].has_goods = has_goods;
     
     s = user[u].server_id;
     
     //exist?
     if(s<0) return;
     
     //server connected?
     if(server[s].con_id < 0) return;
     
     //okay send
     sprintf(rtn_str,"29,%d,%d",user[u].user_id,user[u].has_goods);
     send_con_server(server[s].con_id, rtn_str);
}

void check_and_create_bot_guilds()
{
    int s, b_t, g;     
    
	for(s=0;s<=server_max;s++)
		if(server[s].servername[0] && server[s].db_loaded)
		for(b_t=0;b_t<BOT_TYPE_MAX;b_t++)
			if(server[s].bot_type[b_t].name[0])
			{
				g = guild_name_to_id(server[s].bot_type[b_t].guild);
				
				if(g>-1) continue; //this guild already exists
				
				//well it doesn't exist so lets make it
				create_bot_guild(s,b_t);
			}
}

void create_bot_guild(int s, int b_t)
{
     int b, i ,k;
     char *guild_owner, *nobody = "";
     
     //find the owner
     guild_owner = nobody;
     for(b=0;b<BOT_MAX;b++)
         if(server[s].bot_type[b_t].bot[b].name[0])
         {
             guild_owner = server[s].bot_type[b_t].bot[b].name;
             break;
         }
         
	//make the guild
	guild_max++;
	strcpy(guild[guild_max].name,server[s].bot_type[b_t].guild);
	strcpy(guild[guild_max].owner,guild_owner);
	sprintf(guild[guild_max].website,"www.nighsoft.net");
	guild[guild_max].bank_money = 50000;
	guild[guild_max].bank_lend = 100000;
	strcpy(guild[guild_max].rank_name[5],"Owner");
	strcpy(guild[guild_max].rank_name[4],"General");
	strcpy(guild[guild_max].rank_name[3],"Recruiter");
	strcpy(guild[guild_max].rank_name[2],"Leader");
	strcpy(guild[guild_max].rank_name[1],"Grunt");
	strcpy(guild[guild_max].rank_name[0],"Scout");
	for(i=0;i<6;i++)
		for(k=0;k<5;k++)
			guild[guild_max].rank_allow[i][k] = 0;
	strcpy(guild[guild_max].enemy[2],"");
	strcpy(guild[guild_max].enemy[1],"");
	strcpy(guild[guild_max].enemy[0],"");
	
	//set some default ranks
	guild[guild_max].rank_allow[5][0] = 1;
	guild[guild_max].rank_allow[5][1] = 1;
	guild[guild_max].rank_allow[5][2] = 1;
	guild[guild_max].rank_allow[5][3] = 1;
	guild[guild_max].rank_allow[5][4] = 1;
	
	guild[guild_max].rank_allow[4][0] = 1;
	guild[guild_max].rank_allow[4][1] = 1;
	guild[guild_max].rank_allow[4][2] = 1;
	guild[guild_max].rank_allow[4][3] = 1;
	
	guild[guild_max].rank_allow[3][0] = 1;
	guild[guild_max].rank_allow[3][1] = 1;
	guild[guild_max].rank_allow[3][3] = 1;
	
	guild[guild_max].rank_allow[2][0] = 1;
	guild[guild_max].rank_allow[2][3] = 1;
	
	guild[guild_max].rank_allow[1][3] = 1;
	
	game.rewrite_guild_db = 1;
}

int server_name_to_id(char *servername) //return user # when given the name
{
	int i;
	char temp_str[2][21];
	
	lcase(temp_str[0],servername);
	
	for(i=0;i<=server_max;i++) 
	{
		lcase(temp_str[1],server[i].servername);
		if (str_match(temp_str[0],temp_str[1]))
			return i; //if found return the # and leave the function
	}
	
	return -1; //if not found, return -1
}

void set_user_zone_sector(int con, int user_id, int zone, int sector)
{
    int u;
    
    //this user exist.. ?
    if((u = userid_to_id(user_id)) == -1) return;
    
    user_set_zone_sector(u,zone,sector);
}

void send_server_user_gun_info(int s, int g, int u, int ship_g)
{
    char rtn_str[200];
    int sh_sel;
    
    sh_sel = user[u].ship_selected;
    
    //username,mod_type,slot_number,gun/miss/misc_acc,gun/miss/misc_spd,gun/miss/misc_dam,
    sprintf(rtn_str, "7,%s,0,%d,%d,%d,%d,%d,%d", 
        user[u].username, 
        ship_g, 
        server[s].gun[g].accuracy, 
        server[s].gun[g].speed, 
        server[s].gun[g].damage,
        server[s].gun[g].type,
        user[u].ship[sh_sel].gun_destroyed[ship_g]);
    
    if (server[s].con_id > -1) //if this server is connected...
        send_con_server(server[s].con_id, rtn_str);
}

void send_server_user_missile_info(int s, int g, int u, int ship_g)
{
    char rtn_str[200];
    int sh_sel;
    
    sh_sel = user[u].ship_selected;
    
    //username,mod_type,slot_number,gun/miss/misc_acc,gun/miss/misc_spd,gun/miss/misc_dam,
    sprintf(rtn_str, "7,%s,1,%d,%d,%d,%d,%d,%d", 
        user[u].username, 
        ship_g, 
        server[s].missile[g].accuracy, 
        server[s].missile[g].speed, 
        server[s].missile[g].damage,
        server[s].missile[g].type,
        user[u].ship[sh_sel].missile_left[ship_g]);
    
    if (server[s].con_id > -1) //if this server is connected...
        send_con_server(server[s].con_id, rtn_str);
}

void pay_guild(int g, int amount)
{
	if(g<0) return;
	if(g>guild_max) return;
	if(amount<=0) return;
	
	guild[g].pay_out += amount;
}

void pay_planet_guild(int s, int p, int amount)
{
	int g_b, g;
	
	if(s<0) return;
	if(p<0) return;
	if(s>=MAX_SERVER) return;
	if(p>=PLANET_MAX) return;
	
	g = server[s].planet[p].sat.guild_id;
	g_b = server[s].planet[p].sat.bug_guild_id;
	
	if(g < 0) return;
	if(g > guild_max) return;
	if(g_b < 0) g_b = -1;
	if(g_b > guild_max) g_b = -1;
	
	//award the guild(s)
	if(g_b == -1 || g_b == g)
	{
		//award only the real owner
		pay_guild(g, amount);
	}
	else
	{
		amount = amount / 2;
		pay_guild(g, amount);
		pay_guild(g_b, amount);
	}
	
	server[s].planet[p].sat.money_collected += amount;
}

void pay_planet_and_user_guild(int g, int s, int p, int amount)
{
	amount = amount / 2;
	
	pay_planet_guild(s,p, amount);
	pay_guild(g,amount);
}

void clear_all_guild_payouts()
{
	int g;
	
	for(g=0;g<=guild_max;g++)
		if(guild[g].name[0])
			guild[g].pay_out = 0;
}

void pay_all_guilds()
{
	int g;
	
	for(g=0;g<=guild_max;g++)
		if(guild[g].name[0])
	{
		//salaries
		guild_pay_ranks(g);
		
		//checkin
		if(guild[g].pay_out <= 0) continue;
		
		award_guild_money(g, guild[g].pay_out);
		guild[g].pay_out = 0;
		
		game.rewrite_guild_db = 1;
	}
	
	//yep
	guild_anouce_new_balances();
}

#define RANK_PAYOUT5 0.02
#define RANK_PAYOUT4 0.015
#define RANK_PAYOUT3 0.01
#define RANK_PAYOUT2 0.0075
#define RANK_PAYOUT1 0.005
#define RANK_PAYOUT0 0.0025

#define RANK_PAYOUT_TOTAL (RANK_PAYOUT5 + RANK_PAYOUT4 + RANK_PAYOUT3 + RANK_PAYOUT2 + RANK_PAYOUT1 + RANK_PAYOUT0)

#define RANK_PAYOUT_MIN 10000

void guild_pay_ranks(int g)
{
	int unaccessable;
	int r, u;
	int in_rank[6];
	double rank_payout[6];
	double rank_paytotal[6];
	
	unaccessable = guild_unaccessable_bank_amount(g);
	
	if(!unaccessable) return;
	
	//clear out
	for(r=0;r<6;r++)
	{
		in_rank[r] = 0;
		rank_paytotal[r] = 0;
	}
	
	//set
	rank_payout[0] = RANK_PAYOUT0;
	rank_payout[1] = RANK_PAYOUT1;
	rank_payout[2] = RANK_PAYOUT2;
	rank_payout[3] = RANK_PAYOUT3;
	rank_payout[4] = RANK_PAYOUT4;
	rank_payout[5] = RANK_PAYOUT5;
	
	//you can't do it
	for(u=0;u<=user_max;u++)
		if(user[u].guild_id == g)
	{
		r = user[u].guild_rank;
		
		if(r<0) continue;
		if(r>5) continue;
		if(!guild[g].rank_allow[r][3]) continue;
		
		in_rank[r]++;
	}
	
	//set the payout
	for(r=0;r<6;r++)
		if(in_rank[r])
			rank_paytotal[r] = (unaccessable * rank_payout[r]) / in_rank[r];
	
	//pay the rankers
	for(u=0;u<=user_max;u++)
		if(user[u].guild_id == g)
	{
		r = user[u].guild_rank;
		
		if(r<0) continue;
		if(r>5) continue;
		if(!guild[g].rank_allow[r][3]) continue;
		
		user[u].guild_balance += (int)rank_paytotal[r];
		user[u].guild_balance_update = 1;
	}

}

int guild_unaccessable_bank_amount(int g)
{
	int u;
	int bank_owes = 0;
	int bank_owed = 0;
	int unaccessable;
	int returnable;
	
	//first determine what the bank is owed / owes
	for(u=0;u<=user_max;u++)
		if(user[u].guild_id == g)
	{
		if(user[u].guild_balance > 0)
			bank_owes += user[u].guild_balance;
		else
			bank_owed -= user[u].guild_balance;
	}
	
	unaccessable = (guild[g].bank_money + bank_owed) - bank_owes;
	returnable = (int)(RANK_PAYOUT_TOTAL * unaccessable);
	
	if(returnable >= RANK_PAYOUT_MIN)
		return unaccessable;
	else
		return 0;
}

void guild_anouce_new_balances()
{
	int con;
	
	for(con = 0;con<=user_con_top;con++)
	{
		int u;
		
		u = user_con_id[con];
		
		if(u<0) return;
		if(u>MAX_USER) return;
		
		if(user[u].guild_balance_update)
			user_send_guild_balance(con);
	}
}

void regulate_all_server_sat_bank_maxs()
{
	int s;

	for(s=0;s<=server_max;s++)
		if(server[s].servername[0])
			server_regulate_sat_bank_maxs(s);
}

void regulate_sat_bank_max(int s, int p)
{
	int bank_max;
	
	if(server[s].planet[p].sat.guild_id < 0) return;
	if(server[s].planet[p].sat.level <= 0) return;
		
	bank_max = server[s].planet[p].sat.level * SAT_MAX_BANK_INC;
		
	if(bank_max < server[s].planet[p].sat.money)
	{
		int g;
		int bank_difference;
			
		g = server[s].planet[p].sat.guild_id;
		bank_difference = server[s].planet[p].sat.money - bank_max;
			
		if(g < 0) return;
		if(g >= MAX_GUILD) return;
		if(bank_difference <= 0) return;
			
		guild[g].bank_money += bank_difference;
		server[s].planet[p].sat.money -= bank_difference;
			
		game.rewrite_guild_db = 1;
		server[s].rewrite_sat_db = 1;
	}
}

void server_regulate_sat_bank_maxs(int s)
{
	int p;
	
	for(p=0;p<PLANET_MAX;p++)
		if(server[s].planet[p].name[0])
			regulate_sat_bank_max(s, p);
}

void server_user_award_random_drop(int con, int user_id, int s_c, int s_k, int wep, enum BOT_DROP_TYPE drop_type)
{
	int u, s, i, sh;
	int sh_id, sh_c, sh_k;
	int max;
	int k;
	
	//this user exist.. ?
	if((u = userid_to_id(user_id)) == -1) return;
	
	//what server is this
	s = server_con_id[con];
	
	switch(drop_type)
	{
		case no_type:
			break;
		case is_gun:
			//sending bullshit?
			if(wep < 0) return;
			if(wep >= GUN_MAX) return;
			if(!server[s].gun[wep].name[0]) return;
			
			//even got a ship selected?
			sh = user[u].ship_selected;
			sh_id = user[u].ship[sh].ship_id;
			sh_c = user[u].ship[sh].brand;
			sh_k = user[u].ship[sh].type;
			if(sh_id == -1) return;
			if(sh_c == -1) return;
			if(sh_k == -1) return;
			
			//gun slot available?
			//missile slot available?
			max = server[sh_id].ship[sh_c].gun_ammount[sh_k];
			for(k=0;k<max;k++) //find first avaible gun slot...
				if (user[u].ship[sh].gun_id[k] == -1 || user[u].ship[sh].gun[k] == -1) 
					break;
			
			if (k==max) return; //if no available missile slots...
			
			//give it!
			user_award_gun(u, sh, s, wep, k);
			
			//notify in chat
			if(user[u].con_id > -1)
			{
				char rtn_str[1024];
				
				sprintf(rtn_str,"... Item Reward: The %s gun ..", server[s].gun[wep].name);
				send_user_public_chat(user[u].con_id, 1, rtn_str);
			}
			
			break;
		case is_missile:
			//sending bullshit?
			if(wep < 0) return;
			if(wep >= MISSILE_MAX) return;
			if(!server[s].missile[wep].name[0]) return;
			
			//even got a ship selected?
			sh = user[u].ship_selected;
			sh_id = user[u].ship[sh].ship_id;
			sh_c = user[u].ship[sh].brand;
			sh_k = user[u].ship[sh].type;
			if(sh_id == -1) return;
			if(sh_c == -1) return;
			if(sh_k == -1) return;
			
			//missile slot available?
			max = server[sh_id].ship[sh_c].missile_ammount[sh_k];
			for(k=0;k<max;k++) //find first avaible gun slot...
				if (user[u].ship[sh].missile_id[k] == -1 || user[u].ship[sh].missile[k] == -1) 
					break;

			if (k==max) return; //if no available missile slots...
			
			//give it!
			user_award_missile(u, sh, s, wep, k);
			
			//notify in chat
			if(user[u].con_id > -1)
			{
				char rtn_str[1024];
				
				sprintf(rtn_str,"... Item Reward: The %s missile ..", server[s].missile[wep].name);
				send_user_public_chat(user[u].con_id, 1, rtn_str);
			}
			
			break;
		case is_ship:
			//sending bullshit?
			if(s_c < 0) return;
			if(s_k < 0) return;
			if(s_c >= SHIP_MAX) return;
			if(s_k >= 8) return;
			if(!server[s].ship[s_c].name[0]) return;
			
			//even aloud to have it?
			if(!user[u].activated)
			{
				if(server[s].ship[s_c].exp[s_k] > DEACTIVATED_EXP_LIMIT) return;
				if(server[s].ship[s_c].cost[s_k] > DEACTIVATED_PRICE_LIMIT) return;
			}
			
			//ship slot available?
			for(k=0;k<10;k++)
			{
				if(user[u].ship[k].ship_id == -1) break;
				if(user[u].ship[k].brand == -1) break;
				if(user[u].ship[k].type == -1) break;
			}
			
			//none found?
			if(k==10) return;
			
			//give it!
			user_award_ship(u, k, s, s_c, s_k);
			
			//notify
			if(user[u].con_id > -1)
			{
				char rtn_str[1024];
				char ship_str[50];
				
				sprintf(rtn_str,"... Item Reward: The %s %s ship ..", server[s].ship[s_c].name, ship_name(s_k, ship_str));
				send_user_public_chat(user[u].con_id, 1, rtn_str);
			}
			
			break;
	}
}

//return server # when given the name
int server_to_id(char *servername)
{
	int i;
	char temp_str[2][21];
	
	lcase(temp_str[0],servername);
	
	for(i=0;i<=server_max;i++) 
	{
		lcase(temp_str[1],server[i].servername);
		if (str_match(temp_str[0],temp_str[1]))
			return i; //if found return the # and leave the function
	}
	
	//if not found, return -1
	return -1; 
}

int planet_to_id(int s, char *planetname)
{
    int i;
	char temp_str[2][1024];
	
    //checks
    if(s>=MAX_SERVER) return -1;
    if(s<0) return -1;
    
    //no such planet
    if(!planetname[0]) return -1;
    
	lcase(temp_str[0],planetname);
	
	for(i=0;i<PLANET_MAX;i++) 
        if(server[s].planet[i].name[0])
	{
		lcase(temp_str[1], server[s].planet[i].name);
		if (str_match(temp_str[0],temp_str[1]))
			return i; //if found return the # and leave the function
			
	}
    
    return -1;
}

void ping_server_con(int con)
{
	char send_str[5] = {'3', '3', ',', 13, 0};
	
	raw_send_con_server(con, send_str);
}

void ping_all_servers()
{
	int i;
	char send_str[5] = {'3', '3', ',', 13, 0};
	
	for(i=0;i<=server_con_top;i++)
		raw_send_con_server(i, send_str);
}

void server_ping_response(int con)
{
	
}

void server_ping_request(int con)
{
	char send_str[5] = {'3', '4', ',', 13, 0};
	
	raw_send_con_server(con, send_str);
}

void server_clear_sector_contraband(int con, int z, int s)
{
	int i, g;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	for(g=0;g<10;g++)
		if(server[i].zone[z].sector[s].contra_amount[g])
	{
		server[i].zone[z].sector[s].contra_amount[g] = 0;
		server_send_sector_contra_amount(i,s,z,g);
	}
}

void server_clear_sector_goods(int con, int z, int s)
{
	int i, g;
	
	i = server_con_id[con];
	
	//check for death 
	if(s < 0) return;
	if(s >= SECTOR_MAX) return;
	if(z < 0) return;
	if(z >= ZONE_MAX) return;
	
	for(g=0;g<10;g++)
		if(server[i].zone[z].sector[s].good_amount[g])
	{
		server[i].zone[z].sector[s].good_amount[g] = 0;
		server_send_sector_good_amount(i,s,z,g);
	}
}
