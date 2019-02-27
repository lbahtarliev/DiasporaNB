#include "main.h"

int online_users()
{
	int i, total = 0;

	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] != -1 ) total++;

	return total;
}

void con_user_connect ( int con )
{
	char rtn_str[100];

	sprintf ( rtn_str, "2,%s:: %d users", game.hub_name, online_users() );
	send_con_user ( con,rtn_str );

}

void user_logoff ( int con )
{
	int i, k;

	//stuff that needs to happen if this connection was live
	if ( user_con_id[con] != -1 )
	{
		user[user_con_id[con]].debug = -1; //con stuff

		//take him off the damn servers
		user_logoff_all_server ( user_con_id[con] );
	}


	user_con_id[con] = -1;
	user_con_version[con] = 0;

	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id == con ) //user stuff
		{
			sell_all_user_goods ( i );

			user[i].con_id = -1;
			user[i].debug = -1;
			user[i].landed = 1;
			user[i].guild_request = -1;
			user[i].zone = -1;
			user[i].sector = -1;
			user[i].lease_logged_in = 0;

			//clear all ship sends
			for ( k=0;k<10;k++ )
				user[i].ship[k].sent_info = 0;
		}
}

void user_logoff_duplicates ( int u )
{
	int i;

	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] == u )
			user_logoff ( i );
}

int find_lease_login(char *login, char *password)
{
	int i;
	char lcase_login[21];
	char lcase_login2[21];
	
	//blank chars not allowed
	if(!login[0]) return -1;
	if(!password[0]) return -1;
	
	//cappin
	login[20] = 0;
	password[20] = 0;
	
	lcase(lcase_login, login);
	
	for ( i=0;i<=user_max;i++ )
	{
		lcase(lcase_login2, user[i].lease_login);
		
		if(strcmp(lcase_login, lcase_login2)) continue;
		if(strcmp(password, user[i].lease_password)) continue;
		
		return i;
	}
	
	return -1;
}

int find_login(char *login, char *password)
{
	int i;
	char lcase_login[21];
	char lcase_login2[21];
	
	//blank chars not allowed
	if(!login[0]) return -1;
	if(!password[0]) return -1;
	
	//cappin
	login[20] = 0;
	password[20] = 0;
	
	lcase(lcase_login, login);
	
	for ( i=0;i<=user_max;i++ )
	{
		lcase(lcase_login2, user[i].login);
		
		if(strcmp(lcase_login, lcase_login2)) continue;
		if(strcmp(password, user[i].password)) continue;
		
		return i;
	}
	
	return -1;
}

void user_login ( int con, char *login, char *password )
{
	int i, k, found = 1;
	char rtn_str[1025], temp_str[50];
	int is_lease = 0;

	i = find_login(login, password);
	
	//tell the poor bastard what he has won
	if(i == -1)
	{
		//ok but is it a lease login?
		i = find_lease_login(login, password);
		
		if(i != -1)
		{
			//well isn't this cute, its a lease login
			
			//dont log in if the main login is already in
			if(user[i].con_id > -1 && !user[i].lease_logged_in)
			{
				strcpy ( rtn_str,"2,Main user currently online" );
				send_con_user ( con,rtn_str );
				return;
			}
			
			//we have to set this because the special implications come later
			is_lease = 1;
		}
		else
		{
			//die mother fucka die mother fucka die!
			strcpy ( rtn_str,"2,Login incorrect" );
			send_con_user ( con,rtn_str );
			return;
		}
	}

	printf ( "user logged in:%s:%d\n", user[i].username,i );
	
	//clean the connection
	user_logoff ( con );

	//no duplicates!
	user_logoff_duplicates ( i );

	//special lease stuff
	if(is_lease)
	{
		user[i].lease_logged_in = 1;
		user[i].time_lease_logged = time ( 0 );
		get_con_user_ip ( con, user[i].user_ip_lease );
		user[i].the_s_addr_lease = client_user[con].c_in.sin_addr.s_addr;
	}
	
	//Set user initials
	user_con_id[con] = i;
	user[i].con_id = con;
	user[i].landed = 1;
	user[i].guild_request = -1;
	user[i].time_logged = time ( 0 );
	get_con_user_ip ( con,user[i].user_ip );
	user[i].the_s_addr = client_user[con].c_in.sin_addr.s_addr;
	while ( found )
	{
		user[i].user_id = rand() + rand();
		found = 0;
		for ( k=0;k<=user_max;k++ )
			if ( user[i].user_id == user[k].user_id && i != k ) found = 1;
	}

	//now prepare the information that needs sent

	sprintf ( rtn_str,"3,%d",user[i].user_id );
	send_con_user ( con,rtn_str );

	/*if (user[i].server_id >= 0 && server[user[i].server_id].con_id >= 0) //check for previous server
	{
		sprintf(rtn_str,"4,%s",server[user[i].server_id].server_ip);
	}
	else*/
	//{
	strcpy ( rtn_str,"5" ); // online and officaited server list
	for ( k=0;k<=server_con_top;k++ )
		if ( server_con_id[k] != -1 && server[server_con_id[k]].db_loaded )
		{
			sprintf ( temp_str,",%d,%s,%s,%d,%d", server_con_id[k], server[server_con_id[k]].servername, server[server_con_id[k]].server_ip, server[server_con_id[k]].x, server[server_con_id[k]].y );
			strcat ( rtn_str,temp_str );
			found = 1;
		}

	if ( !found ) strcat ( rtn_str, "," ); //correctorize the return string so the client doesnt mess up
	send_con_user ( con,rtn_str );
	//}

	found = 0;
	strcpy ( rtn_str,"6" ); // full server list
	for ( k=0;k<=server_max;k++ )
	{
		sprintf ( temp_str,",%s", server[k].servername );
		strcat ( rtn_str,temp_str );
		found = 1;
	}

	if ( !found ) strcat ( rtn_str, "," ); //correctorize the return string so the client doesnt mess up
	send_con_user ( con,rtn_str );

	if ( user[i].home_id > -1 && user[i].home_planet > -1 ) //send home info if there is a home
	{
		sprintf ( rtn_str,"7,%d,%d", user[i].home_id, user[i].home_planet );
		send_con_user ( user[i].con_id, rtn_str );
	}

	send_user_name_guild ( i );

	sprintf ( rtn_str,"13,%d", user[i].money );
	send_con_user ( user[i].con_id, rtn_str );

	send_user_ship_info_to_user ( i );

	send_user_hanger_info ( i );

	user_send_guild_info ( con );

	user_send_guild_balance ( con );

	user_send_exp ( i );

	user_send_activated_user ( i );

	send_user_private_chat_list ( i );
	
	user_send_plan_list(i);
	
	user_do_noob_message ( i );
	
	user_do_lease_time_message(i);
}

void user_check_ship_validity ( int u )
{
	int sh_sel, s;
	int i,j,k;

	sh_sel = user[u].ship_selected;
	s = user[u].server_id;

	if ( user[u].ship[sh_sel].ship_id == -1 || user[u].ship[sh_sel].brand == -1 || user[u].ship[sh_sel].type == -1 )
	{
		//find the first ship that is
		for ( i=0;i<10;i++ )
			if ( user[u].ship[i].ship_id > -1 && user[u].ship[i].brand > -1 && user[u].ship[i].type > -1 )
				break;

		if ( i==10 ) //bubba you got no legs!
		{
			//we goin home forest?
			if ( s == user[u].home_id )
			{
				user[u].last_planet = user[u].home_planet;
			}
			else // ;(
			{
				user[u].last_planet = server_get_server_gate ( s );

				if ( user[u].last_planet == -1 ) //no server gate?
					user[u].last_planet = server_get_least_ranged_planet ( s );

				if ( user[u].last_planet == -1 ) //no planet at all!?
					user[u].last_planet = 0;
			}
		}
		else //mmm  leeggss..........
		{
			//this could be a huge problem
			//if you also do not let the server know of this update.
			user[u].ship_selected = i;
			
			send_user_ship_info_to_user(u);
		}
	}
}

int user_to_id ( char *username ) //return user # when given the name
{
	int i;
	char temp_str[2][21];

	username[20] = 0;
	
	lcase ( temp_str[0],username );

	for ( i=0;i<=user_max;i++ )
	{
		lcase ( temp_str[1],user[i].username );
		if ( str_match ( temp_str[0],temp_str[1] ) )
			return i; //if found return the # and leave the function
	}

	return -1; //if not found, return -1
}

int user_login_to_id ( char *login )
{
	int i;
	char temp_str[2][21];

	login[20] = 0;
	
	lcase ( temp_str[0],login );

	for ( i=0;i<=user_max;i++ )
	{
		if ( str_match ( temp_str[0],user[i].login ) )
			return i; //if found return the # and leave the function
	}

	return -1; //if not found, return -1
}

int guild_name_to_id ( char *guildname ) //return guild # when given the name
{
	int i;
	char temp_str[2][21];

	guildname[20] = 0; //cappin

	lcase ( temp_str[0],guildname );

	for ( i=0;i<=guild_max;i++ )
	{
		lcase ( temp_str[1],guild[i].name );
		if ( str_match ( temp_str[0],temp_str[1] ) )
			return i; //if found return the # and leave the function
	}

	return -1; //if not found, return -1
}

int userid_to_id ( int user_id )
{
	int i;

	//the dir case
	if ( !user_id ) return -1;

	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] != -1 && user_id == user[user_con_id[i]].user_id )
			break;

	if ( i>user_con_top ) return -1; //id not found

	//found
	return user_con_id[i];
}

void send_user_debug ( int con, char *message )
{
	int i;

	for ( i=0;i<=admin_con_top;i++ )
		if ( admin_con_id[i] != -1 && admin[admin_con_id[i]].user_debug == user_con_id[con] )
			send_con_admin ( admin_con_id[i],message );
}

void create_user ( int con, char *login, char *password, char *username, char *email )
{
	int i, k, point = 0;
	char temp_str[2][51], rtn_str[200];
	int time_until_creation, time_hours, time_minutes, time_seconds;

	time_until_creation = user_creation_limit ( client_user[con].c_in.sin_addr.s_addr );

	if ( time_until_creation > 0 )
	{
		time_hours = time_until_creation / 3600;
		time_minutes = ( time_until_creation / 60 ) - ( time_hours * 60 );
		time_seconds = time_until_creation % 60;

		sprintf ( rtn_str,"2,Please wait another %d hours, %d minutes, and %d seconds before attempting to create another user.", time_hours, time_minutes, time_seconds );
		send_con_user ( con,rtn_str );
		return;
	}


	if ( user_max+1 >= MAX_USER )
	{
		strcpy ( rtn_str,"2,Maximum user count reached." );
		send_con_user ( con,rtn_str );
		return;
	}

	login[20] = '\0';
	password[20] = '\0';
	username[20] = '\0';
	email[50] = '\0';

	lcase ( login,login );
	lcase ( temp_str[0],username );

	//speaks for itself
	if ( login[0] == '\0' || password[0] == '\0' || username[0] == '\0' || email[0] == '\0' )
	{
		strcpy ( rtn_str,"2,Invalid entry or entries." );
		send_con_user ( con,rtn_str );
		return;
	}

	//login used?
	i = find_just_any_login(login);
	if( i != -1)
	{
		strcpy ( rtn_str,"2,Login already taken, please try another." );
		send_con_user ( con,rtn_str );
		return;
	}
	
	//username used?
	i = user_to_id(temp_str[0]);
	if( i != -1)
	{
		strcpy ( rtn_str,"2,Username already taken, please try another." );
		send_con_user ( con,rtn_str );
		return;
	}

	//it a botname?
	if ( is_game_reserved_name ( username ) )
	{
		strcpy ( rtn_str,"2,Username reserved for game use, please try another." );
		send_con_user ( con,rtn_str );
		return;
	}

	//barcode?
	if ( user_likely_barcode ( username ) )
	{
		strcpy ( rtn_str,"2,Username can not have I and l in the same name." );
		send_con_user ( con,rtn_str );
		return;
	}

	//bad spacing?
	if ( user_bad_spacing ( username ) )
	{
		strcpy ( rtn_str,"2,Username has improper space usage." );
		send_con_user ( con,rtn_str );
		return;
	}
	
	//got none ascii's?
	if(has_bad_user_characters(username) || has_bad_user_characters(login) || has_bad_user_characters(password) || has_bad_user_characters(email))
	{
		strcpy ( rtn_str,"2,Can not have ';' in an entry" );
		send_con_user ( con,rtn_str );
		return;
	}

	user_max += 1;

	strcpy ( user[user_max].login,login );
	strcpy ( user[user_max].password,password );
	strcpy ( user[user_max].username,username );
	strcpy ( user[user_max].email,email );
	user[user_max].lease_login[0] = 0;
	user[user_max].lease_password[0] = 0;
	user[user_max].money = MIN_MONEY;//2000000;
	user[user_max].exp = 0;//125;
	user[user_max].kills = 0;
	get_con_user_ip ( con,user[user_max].user_ip );
	user[user_max].the_s_addr = client_user[con].c_in.sin_addr.s_addr;
	strcpy ( user[user_max].user_ip_creator,user[user_max].user_ip );
	user[user_max].the_s_addr_creator = user[user_max].the_s_addr;
	user[user_max].server_id 	= -1;
	user[user_max].guild_id 	= 0;
	user[user_max].guild_rank 	= 0;
	user[user_max].guild_balance 	= 0;
	user[user_max].user_class 	= 0;
	user[user_max].user_class_points   = 0;
	user[user_max].banned 		= 0;
	user[user_max].ship_selected 	= 0;
	user[user_max].home_planet 	= -1;
	user[user_max].home_id 		= -1;
	user[user_max].last_planet 	= -1;
	user[user_max].version = user_con_version[con];
	user[user_max].activated = 0;
	user[user_max].time_logged = time ( 0 );
	user[user_max].time_created = time ( 0 );
	for ( i=0;i<10;i++ )
	{
		user[user_max].ship[i].ship_id 	= -1;
		user[user_max].ship[i].brand 	= -1;
		user[user_max].ship[i].type 	= -1;
		for ( k=0;k<16;k++ )
		{
			user[user_max].ship[i].gun_id[k] 	= -1;
			user[user_max].ship[i].gun[k] 		= -1;
			user[user_max].ship[i].gun_destroyed[k] = 0;
			user[user_max].ship[i].missile_id[k] 	= -1;
			user[user_max].ship[i].missile[k] 	= -1;
			user[user_max].ship[i].missile_left[k] 	= 0;
			user[user_max].ship[i].misc[k] 		= -1;
		}
	}
	user_clear_private_chats_raw ( user_max );
	user_clear_decorations ( user_max );

	game.rewrite_user_db = 1;

	//send to jabber hell
	mysql_make_jabber_insert ( user[user_max].username, user[user_max].password );

	user_login ( con,login,password ); //hallaluya
}

extern void purchase_ship ( int con, int ship_brand, int ship_kind )
{
	int user_id, k, temp_int, u, g, sh, s, p;
	char rtn_str[1000], temp_str[200];
	int cost;
	int guild_tax;

	u = user_id = user_con_id[con];
	sh = user[u].ship_selected;
	s = user[u].server_id;
	p = user[u].last_planet;
	g = user[u].guild_id;

	if(user_check_lease_loggedin(u)) return;
	if ( ship_brand < 0 || ship_brand > SHIP_MAX - 1 ) return; //invalid inputs, blah
	if ( ship_kind < 0 || ship_kind > 7 ) return;
	if ( u == -1 ) return; //if somehow the user logged off then leave (extreme chance)
	if ( user[u].server_id == -1 ) return;
	if ( user[u].last_planet == -1 ) return;
	if ( !user[u].landed ) return;
	if ( user[u].ship[sh].brand > -1 && user[u].ship[sh].type > -1 && user[u].ship[sh].ship_id > -1 ) return; //if already have ship then exit...
	if((user_at_home_planet(u) && !user[u].has_plan[s][ship_brand]) || !user_at_home_planet(u)) 
	{
		if ( !server[s].planet[p].ship[ship_brand].have ) return; //if server planet doesnt have this ship brand...
		if ( server[s].planet[p].ship[ship_brand].max < ship_kind ) return; //if server planet doesnt have this ship kind...
		if ( server[s].planet[p].ship[ship_brand].exclude[ship_kind] ) return; //espicialy excluded?
	}
	if ( user[u].exp < server[s].ship[ship_brand].exp[ship_kind] ) return; //if not enough exp...
	if ( user[u].money < server[s].ship[ship_brand].cost[ship_kind] ) return; //if not enough money...

	//deactivated user stuff
	if ( !user[u].activated )
	{
		if ( server[s].ship[ship_brand].exp[ship_kind] > DEACTIVATED_EXP_LIMIT ) return;
		if ( server[s].ship[ship_brand].cost[ship_kind] > DEACTIVATED_PRICE_LIMIT ) return;
	}

	//Give the player their ship.
	user_award_ship ( user_id, sh, s, ship_brand, ship_kind );

	//set this
	cost = server[s].ship[ship_brand].cost[ship_kind];
	guild_tax = ( int ) ( cost * 0.05 );

	user[user_id].money -= cost;

	//taxes!
	if ( user[user_id].money >= MIN_MONEY && user[user_id].guild_balance >= 0 )
		pay_planet_and_user_guild ( g,s,p,guild_tax );

	//reupdate user on money status
	user_send_money ( con );
}

void user_award_ship ( int u, int sh, int s, int s_c, int s_k )
{
	int k;
	int con;

	//set it
	con = user[u].con_id;

	//give the ship
	user[u].ship[sh].ship_id 	= s;
	user[u].ship[sh].brand 		= s_c;
	user[u].ship[sh].type 		= s_k;
	for ( k=0;k<16;k++ )
	{
		user[u].ship[sh].gun_id[k] 	= -1;
		user[u].ship[sh].gun[k] 	= -1;
		user[u].ship[sh].gun_destroyed[k] = 0;
		user[u].ship[sh].missile_id[k] 	= -1;
		user[u].ship[sh].missile[k] 	= -1;
		user[u].ship[sh].missile_left[k] = 0;
		user[u].ship[sh].misc[k] 	= -1;
	}
	user[u].ship[sh].hull_max = server[s].ship[s_c].hull[s_k];
	user[u].ship[sh].hull = user[u].ship[sh].hull_max;
	user[u].ship[sh].shield_max = server[s].ship[s_c].shield[s_k];
	user[u].ship[sh].cargo_max = server[s].ship[s_c].cargo[s_k];
	user[u].ship[sh].speed_max = server[s].ship[s_c].speed[s_k];

	//clear cargo
	clear_all_user_ship_goods ( u,sh );

	//forget what these do
	user_assign_ship_destroy_code ( u,sh );
	user[u].ship[sh].sent_info = 1;

	//reupdate user and server about user current ship status if ship selected
	if ( user[u].ship_selected == sh )
	{
		send_user_ship_info_to_user ( u );
		send_user_ship_info_to_server ( u );
	}

	//reupdate user on hanger status
	send_user_hanger_ship ( u, sh );

	//set this
	game.rewrite_user_db = 1;

	//update!
	mysql_update_ship_entry ( s, s_c, s_k );
}

extern void purchase_ship_mod ( int con, int mod_type, int mod_number )
{
	int user_id, k, temp_int, ship_kind, ship_brand;
	char rtn_str[200];
	int u, s, p, g, sh, sh_sel, sh_k, sh_t, sh_id;
	int guild_tax;
	int cost;

	u = user_id = user_con_id[con];
	s = user[u].server_id;
	p = user[u].last_planet;
	g = user[u].guild_id;

	sh_sel = user[u].ship_selected;
	sh_id = user[u].ship[sh_sel].ship_id;
	sh_t = ship_brand = user[u].ship[sh_sel].brand;
	sh_k = ship_kind = user[u].ship[sh_sel].type;

	if(user_check_lease_loggedin(u)) return;
	if ( ship_brand == -1 || ship_kind == -1 || sh_id == -1 ) return; //no ship
	if ( user_id == -1 ) return; //if somehow the user logged off then leave (extreme chance)
	if ( user[user_id].server_id == -1 ) return;
	if ( user[user_id].last_planet == -1 ) return;
	if ( !user[user_id].landed ) return;

	switch ( mod_type )
	{
		case 1: //gun
			if ( !server[s].planet[user[user_id].last_planet].gun[mod_number] ) break;//if server planet doesnt have gun type...
			if ( user[u].money < server[s].gun[mod_number].cost ) break;//if not enough money...
			if ( user[u].exp < gun_exp_requirement ( server[s].gun, mod_number, server[sh_id].ship[sh_t].exp[sh_k] ) ) break;//if not enough exp...

			temp_int = server[sh_id].ship[ship_brand].gun_ammount[ship_kind];
			for ( k=0;k<temp_int;k++ ) //find first avaible gun slot...
				if ( user[user_id].ship[user[user_id].ship_selected].gun_id[k] == -1 || user[user_id].ship[user[user_id].ship_selected].gun[k] == -1 ) break;

			if ( k==temp_int ) break; //if no avaible gun slots...

			cost = server[s].gun[mod_number].cost;
			user[user_id].money -= cost;
			guild_tax = ( int ) ( cost * 0.05 );
			if ( user[user_id].money >= MIN_MONEY && user[user_id].guild_balance >= 0 )
				pay_planet_and_user_guild ( g,s,p,guild_tax );

			//send the user their money status
			user_send_money ( con );

			//do it then
			user_award_gun ( u, sh_sel, s, mod_number, k );

			break;
		case 2: //missile
			if ( !server[s].planet[user[user_id].last_planet].missile[mod_number] ) break;//if server planet doesnt have gun type...
			if ( user[user_id].money < server[s].missile[mod_number].cost ) break;//if not enough money...
			if ( user[u].exp < missile_exp_requirement ( server[s].missile, mod_number, server[sh_id].ship[sh_t].exp[sh_k] ) ) break;//if not enough exp...

			temp_int = server[sh_id].ship[ship_brand].missile_ammount[ship_kind];
			for ( k=0;k<temp_int;k++ ) //find first avaible gun slot...
				if ( user[u].ship[sh_sel].missile_id[k] == -1 || user[u].ship[sh_sel].missile[k] == -1 ) break;

			if ( k==temp_int ) break; //if no avaible missile slots...

			cost = server[s].missile[mod_number].cost;
			user[user_id].money -= cost;
			guild_tax = ( int ) ( cost * 0.05 );
			if ( user[user_id].money >= MIN_MONEY && user[user_id].guild_balance >= 0 )
				pay_planet_and_user_guild ( g,s,p,guild_tax );

			//do it then
			user_award_missile ( u, sh_sel, s, mod_number, k );

			break;
		case 3: //misc
			if ( user[user_id].money < misc_pod_cost ( mod_number ) ) break;//if not enough money...
			if ( user[user_id].exp < misc_pod_exp ( mod_number ) + server[sh_id].ship[ship_brand].exp[ship_kind] ) break; //not enough exp

			temp_int = server[sh_id].ship[ship_brand].misc_ammount[ship_kind];
			for ( k=0;k<temp_int;k++ ) //find first avaible misc slot...
				if ( user[user_id].ship[user[user_id].ship_selected].misc[k] == -1 ) break;

			if ( k==temp_int ) break; //if no avaible misc slots...

			//check for already maxed shields-1-5/jump-3-7
			if ( mod_number == 3 && user[user_id].ship[user[user_id].ship_selected].speed_max > 24 ) break;
			if ( mod_number == 7 && user[user_id].ship[user[user_id].ship_selected].speed_max > 23 ) break;
			if ( mod_number == 1 && user[user_id].ship[user[user_id].ship_selected].shield_max > 90 ) break;
			if ( mod_number == 5 && user[user_id].ship[user[user_id].ship_selected].shield_max > 88 ) break;

			//give the player their misc pod
			user[user_id].ship[user[user_id].ship_selected].misc[k] = mod_number;

			cost = misc_pod_cost ( mod_number );
			user[user_id].money -= cost;
			guild_tax = ( int ) ( cost * 0.05 );
			if ( user[user_id].money >= MIN_MONEY && user[user_id].guild_balance >= 0 )
				pay_planet_and_user_guild ( g,s,p,guild_tax );

			//set the maxs (ie shield or hull) for this ship
			set_user_ship_misc ( user_id, user[user_id].ship_selected );

			//tell the server about the update
			send_user_ship_info_to_server ( user_id );

			//tell the client
			//mod_type,slot_number,misc_value
			sprintf ( rtn_str, "10,2,%d,%d", k, mod_number );
			send_con_user ( con, rtn_str );
			send_user_hanger_ship ( user_id, user[user_id].ship_selected );

			//send the user their money status
			user_send_money ( con );

			//do this db stuff
			game.rewrite_user_db = 1;

			break;
	}
}

void user_award_gun ( int u, int sh, int s, int wep, int k )
{
	char rtn_str[1024];
	int con;
	int sh_id, sh_c, sh_k;

	//set some stuff
	con = user[u].con_id;
	sh_id = user[u].ship[sh].ship_id;
	sh_c = user[u].ship[sh].brand;
	sh_k = user[u].ship[sh].type;

	//it a good ship?
	if ( sh_id < 0 ) return;
	if ( sh_c < 0 ) return;
	if ( sh_k < 0 ) return;

	//give the player their missile
	user[u].ship[sh].gun_id[k] = s;
	user[u].ship[sh].gun[k] = wep;
	user[u].ship[sh].gun_destroyed[k] = 0;

	//send the server the news
	send_server_user_gun_info ( s,wep,u,k );

	if ( con > -1 )
	{
		int i;
		int weapons = 0;
		int max_guns, max_missiles;

		max_missiles = server[sh_id].ship[sh_c].missile_ammount[sh_k];
		max_guns = server[sh_id].ship[sh_c].gun_ammount[sh_k];

		//send the user the news
		//mod_type,slot_number,gun/miss/misc_id,gun/miss/misc_value, missile_left
		sprintf ( rtn_str, "10,0,%d,%d,%d,0", k, s, wep );
		send_con_user ( con, rtn_str );

		//is this their first weapon?
		for ( i=0;i<max_missiles;i++ )
		{
			if ( user[u].ship[sh].missile_id[k] == -1 ) break;
			if ( user[u].ship[sh].missile[k] == -1 ) break;
			weapons++;
		}
		for ( i=0;i<max_guns;i++ )
		{
			if ( user[u].ship[sh].gun_id[k] == -1 ) break;
			if ( user[u].ship[sh].gun[k] == -1 ) break;
			weapons++;
		}

		if ( weapons == 1 )
			send_user_hanger_ship ( u, sh ); //tell the user that ship is now equiped
	}

	//do this db stuff
	game.rewrite_user_db = 1;
}

void user_award_missile ( int u, int sh, int s, int wep, int k )
{
	char rtn_str[1024];
	int con;
	int sh_id, sh_c, sh_k;

	//set some stuff
	con = user[u].con_id;
	sh_id = user[u].ship[sh].ship_id;
	sh_c = user[u].ship[sh].brand;
	sh_k = user[u].ship[sh].type;

	//it a good ship?
	if ( sh_id < 0 ) return;
	if ( sh_c < 0 ) return;
	if ( sh_k < 0 ) return;

	//give the player their missile
	user[u].ship[sh].missile_id[k] 	= s;
	user[u].ship[sh].missile[k] = wep;
	user[u].ship[sh].missile_left[k] = 4;

	//send the server the news
	//username,mod_type,slot_number,gun/miss/misc_acc,gun/miss/misc_spd,gun/miss/misc_dam, missile_left
	send_server_user_missile_info ( s,wep,u,k );

	if ( con > -1 )
	{
		int i;
		int weapons = 0;
		int max_guns, max_missiles;

		max_missiles = server[sh_id].ship[sh_c].missile_ammount[sh_k];
		max_guns = server[sh_id].ship[sh_c].gun_ammount[sh_k];

		//send the user the news
		//mod_type,slot_number,gun/miss/misc_id,gun/miss/misc_value, missile_left
		sprintf ( rtn_str, "10,1,%d,%d,%d,4", k, s, wep );
		send_con_user ( con, rtn_str );

		//is this their first weapon?
		for ( i=0;i<max_missiles;i++ )
		{
			if ( user[u].ship[sh].missile_id[k] == -1 ) break;
			if ( user[u].ship[sh].missile[k] == -1 ) break;
			weapons++;
		}
		for ( i=0;i<max_guns;i++ )
		{
			if ( user[u].ship[sh].gun_id[k] == -1 ) break;
			if ( user[u].ship[sh].gun[k] == -1 ) break;
			weapons++;
		}

		if ( weapons == 1 )
			send_user_hanger_ship ( u, sh ); //tell the user that ship is now equiped
	}

	//do this db stuff
	game.rewrite_user_db = 1;
}

extern void send_user_ship_info_to_user ( int user_id )
{
	int k, temp_int, ship_kind, ship_brand;
	char rtn_str[1000], temp_str[200];
	int u, sh_sel, sh_id;

	u = user_id;
	sh_sel = user[u].ship_selected;
	sh_id = user[u].ship[sh_sel].ship_id;
	ship_brand = user[u].ship[sh_sel].brand;
	ship_kind = user[u].ship[sh_sel].type;

	if ( ship_brand == -1 || ship_kind == -1 ) //if has no ship, then send noship packets and exit function
	{
		//setup client info packet
		sprintf ( rtn_str, "8,%d,-1", user[user_id].ship_selected );

		if ( user[user_id].con_id > -1 )
			send_con_user ( user[user_id].con_id, rtn_str );

		return;
	}

	//send gun and missile info if needed
	if ( !user[user_id].ship[sh_sel].sent_info )
	{
		int h, g, gun_max, missile_max;

		gun_max = server[sh_id].ship[ship_brand].gun_ammount[ship_kind];
		missile_max = server[sh_id].ship[ship_brand].missile_ammount[ship_kind];

		for ( h=0;h<gun_max;h++ )
			if ( user[user_id].ship[sh_sel].gun_id[h] > -1 && user[user_id].ship[sh_sel].gun[h] > -1 )
			{
				for ( g=0;g<h;g++ ) //find duplicate
					if ( user[user_id].ship[sh_sel].gun_id[h] == user[user_id].ship[sh_sel].gun_id[g] && user[user_id].ship[sh_sel].gun[h] == user[user_id].ship[sh_sel].gun[g] )
						break;

				if ( g == h ) //no duplicate found
					send_user_gun_info ( user[user_id].con_id, user[user_id].ship[sh_sel].gun_id[h], user[user_id].ship[sh_sel].gun[h] );

			}

		for ( h=0;h<missile_max;h++ )
			if ( user[user_id].ship[sh_sel].missile_id[h] > -1 && user[user_id].ship[sh_sel].missile[h] > -1 )
			{
				for ( g=0;g<h;g++ ) //find duplicate
					if ( user[user_id].ship[sh_sel].missile_id[h] == user[user_id].ship[sh_sel].missile_id[g] && user[user_id].ship[sh_sel].missile[h] == user[user_id].ship[sh_sel].missile[g] )
						break;

				if ( g == h ) //no duplicate found
					send_user_missile_info ( user[user_id].con_id, user[user_id].ship[sh_sel].missile_id[h], user[user_id].ship[sh_sel].missile[h] );
			}

		user[user_id].ship[sh_sel].sent_info = 1;
	}

	//setup client info packet

	//ship_selected,ship_id,ship_brand,ship_kind,cost,exp,hull,hull_base,shields,range,speed,evasion,cargo,max_guns,max_missiles,max_misc,gun0_id,gun0_acc,gun0_spd,gun0_dam,missile0_id,missile0_acc,missile0_spd,missile0_dam,misc0_number
	sprintf ( rtn_str, "8,%d,%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
	          user[user_id].ship_selected,
	          user[user_id].ship[sh_sel].ship_id,
	          user[user_id].ship[sh_sel].brand,
	          user[user_id].ship[sh_sel].type,
	          server[sh_id].ship[ship_brand].name,
	          server[sh_id].ship[ship_brand].cost[ship_kind],
	          server[sh_id].ship[ship_brand].exp[ship_kind],
	          user[user_id].ship[sh_sel].hull,
	          server[sh_id].ship[ship_brand].hull[ship_kind],
	          server[sh_id].ship[ship_brand].shield[ship_kind],
	          server[sh_id].ship[ship_brand].range[ship_kind],
	          server[sh_id].ship[ship_brand].speed[ship_kind],
	          server[sh_id].ship[ship_brand].evasion[ship_kind],
	          server[sh_id].ship[ship_brand].cargo[ship_kind],
	          server[sh_id].ship[ship_brand].gun_ammount[ship_kind],
	          server[sh_id].ship[ship_brand].missile_ammount[ship_kind],
	          server[sh_id].ship[ship_brand].misc_ammount[ship_kind] );

	temp_int = server[sh_id].ship[ship_brand].gun_ammount[ship_kind]; //Send Gun info
	for ( k=0;k<temp_int;k++ )
		if ( user[user_id].ship[sh_sel].gun[k] > -1 && user[user_id].ship[sh_sel].gun_id[k] > -1 ) //if have gun
		{
			sprintf ( temp_str,",%d,%d,%d",
			          user[user_id].ship[sh_sel].gun_id[k],
			          user[user_id].ship[sh_sel].gun[k],
			          user[user_id].ship[sh_sel].gun_destroyed[k] );
			strcat ( rtn_str,temp_str );
		}
		else
		{
			strcat ( rtn_str,",-1,-1,0" );
		}

	temp_int = server[sh_id].ship[ship_brand].missile_ammount[ship_kind]; //Send Missile Info
	for ( k=0;k<temp_int;k++ )
		if ( user[user_id].ship[sh_sel].missile[k] > -1 && user[user_id].ship[sh_sel].missile_id[k] > -1 ) //if have gun
		{
			sprintf ( temp_str,",%d,%d,%d", user[user_id].ship[sh_sel].missile_id[k],
			          user[user_id].ship[sh_sel].missile[k],
			          user[user_id].ship[sh_sel].missile_left[k] );
			strcat ( rtn_str,temp_str );
		}
		else
		{
			strcat ( rtn_str,",-1,-1,0" );
		}

	temp_int = server[sh_id].ship[ship_brand].misc_ammount[ship_kind]; //Send Misc Info
	for ( k=0;k<temp_int;k++ )
	{
		sprintf ( temp_str,",%d", user[user_id].ship[sh_sel].misc[k] );
		strcat ( rtn_str,temp_str );
	}

	if ( user[user_id].con_id > -1 ) //if user is connected...
	{
		send_con_user ( user[user_id].con_id, rtn_str );

		//send the user their cargo info
		user_send_cargo_info ( user[user_id].con_id );
	}


}

extern void send_user_ship_info_to_server ( int u )
{
	int k, temp_int, ship_kind, ship_brand;
	char rtn_str[1000] = "", temp_str[200];
	int s, s_con;
	int s_sel, s_sh;
	int w, w_id;

	s = user[u].server_id;

	if ( s<0 || s>=MAX_SERVER ) {printd ( "Send_ship_info_error: !s" ); return;}

	s_con = server[s].con_id;

	if ( s_con < 0 ) {printd ( "Send_ship_info_error: !s_con" ); return;}

	s_sel = user[u].ship_selected;
	s_sh = user[u].ship[s_sel].ship_id;
	ship_brand = user[u].ship[s_sel].brand;
	ship_kind = user[u].ship[s_sel].type;

	if ( s_sh == -1 || ship_brand == -1 || ship_kind == -1 ) //if has no ship, then send noship packets and exit function
	{
		//setup server info packet
		sprintf ( rtn_str, "6,%s,-1", user[u].username );

		send_con_server ( s_con, rtn_str );

		return;
	}

	//setup server info packet

	//username,ship_id,ship_brand,ship_kind,ship_class_name,max_hull,hull,max_shields,range,max_speed,evasion,max_guns,max_missiles
	sprintf ( rtn_str, "6,%s,%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d",
	          user[u].username,
	          user[u].ship[s_sel].ship_id,
	          user[u].ship[s_sel].brand,
	          user[u].ship[s_sel].type,
	          server[s_sh].ship[ship_brand].name,
	          user[u].ship[s_sel].hull_max,
	          user[u].ship[s_sel].hull,
	          user[u].ship[s_sel].shield_max,
	          server[s_sh].ship[ship_brand].range[ship_kind],
	          user[u].ship[s_sel].speed_max,
	          server[s_sh].ship[ship_brand].evasion[ship_kind],
	          server[s_sh].ship[ship_brand].gun_ammount[ship_kind],
	          server[s_sh].ship[ship_brand].missile_ammount[ship_kind],
	          user[u].ship[s_sel].destroy_code );

	temp_int = server[s_sh].ship[ship_brand].gun_ammount[ship_kind];
	for ( k=0;k<temp_int;k++ )
		if ( user[u].ship[s_sel].gun[k] > -1 && user[u].ship[s_sel].gun_id[k] > -1 ) //if have gun
		{
			w = user[u].ship[s_sel].gun[k];
			w_id = user[u].ship[s_sel].gun_id[k];

			sprintf ( temp_str,",%d,%d,%d,%d,%d",
			          server[w_id].gun[w].accuracy,
			          server[w_id].gun[w].speed,
			          server[w_id].gun[w].damage,
			          server[w_id].gun[w].type,
			          user[u].ship[s_sel].gun_destroyed[k] );
			strcat ( rtn_str,temp_str );
		}
		else
		{
			strcat ( rtn_str,",0,0,0,0,0" );
		}

	temp_int = server[s_sh].ship[ship_brand].missile_ammount[ship_kind];
	for ( k=0;k<temp_int;k++ )
		if ( user[u].ship[s_sel].missile[k] > -1 && user[u].ship[s_sel].missile_id[k] > -1 ) //if have gun
		{
			w = user[u].ship[s_sel].missile[k];
			w_id = user[u].ship[s_sel].missile_id[k];

			sprintf ( temp_str,",%d,%d,%d,%d,%d",
			          server[w_id].missile[w].accuracy,
			          server[w_id].missile[w].speed,
			          server[w_id].missile[w].damage,
			          server[w_id].missile[w].type,
			          user[u].ship[s_sel].missile_left[k] );
			strcat ( rtn_str,temp_str );
		}
		else
		{
			strcat ( rtn_str,",0,0,0,0,0" );
		}


	send_con_server ( s_con, rtn_str );
}

extern void send_user_hanger_info ( int user_id )
{
	int k, z, ship_range;
	char rtn_str[1000], temp_str[400], temp_ship_name[21];

	//ship_id,ship_brand,ship_kind,ship_name,max_hull,hull,max_cargo,cargo,max_speed,range,max_shields,has_weapons0-1
	strcpy ( rtn_str,"11" );
	for ( k=0;k<10;k++ )
	{
		if ( user[user_id].ship[k].ship_id > -1 && user[user_id].ship[k].brand > -1 && user[user_id].ship[k].type > -1 ) //has ship
		{
			ship_range = server[user[user_id].ship[k].ship_id].ship[user[user_id].ship[k].brand].range[user[user_id].ship[k].type];
			strcpy ( temp_ship_name,server[user[user_id].ship[k].ship_id].ship[user[user_id].ship[k].brand].name );
		}
		else
		{
			ship_range = 0;
			strcpy ( temp_ship_name,"" );
		}

		sprintf ( temp_str, ",%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d",
		          user[user_id].ship[k].ship_id,
		          user[user_id].ship[k].brand,
		          user[user_id].ship[k].type,
		          temp_ship_name,
		          user[user_id].ship[k].hull_max,
		          user[user_id].ship[k].hull,
		          user[user_id].ship[k].cargo_max,
		          0, //set to zero until cargo is inpletmented
		          user[user_id].ship[k].speed_max,
		          ship_range,
		          user[user_id].ship[k].shield_max,
		          ( user[user_id].ship[k].gun[0] > -1 || user[user_id].ship[k].missile[0] > -1 ) ); //last input is a boolean expresion that returns 0 or 1

		strcat ( rtn_str, temp_str );
	}

	if ( user[user_id].con_id > -1 ) //if user is connected...
		send_con_user ( user[user_id].con_id, rtn_str );
}

extern void send_user_hanger_ship ( int user_id, int ship_number )
{
	int ship_range;
	char rtn_str[400], temp_ship_name[21];

	//ship_id,ship_brand,ship_kind,ship_name,max_hull,hull,max_cargo,cargo,max_speed,range,max_shields,has_weapons0-1
	if ( user[user_id].ship[ship_number].ship_id > -1 && user[user_id].ship[ship_number].brand > -1 && user[user_id].ship[ship_number].type > -1 ) //has ship
	{
		ship_range = server[user[user_id].ship[ship_number].ship_id].ship[user[user_id].ship[ship_number].brand].range[user[user_id].ship[ship_number].type];
		strcpy ( temp_ship_name,server[user[user_id].ship[ship_number].ship_id].ship[user[user_id].ship[ship_number].brand].name );
	}
	else
	{
		ship_range = 0;
		strcpy ( temp_ship_name,"" );
	}

	sprintf ( rtn_str, "14,%d,%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d",
	          ship_number,
	          user[user_id].ship[ship_number].ship_id,
	          user[user_id].ship[ship_number].brand,
	          user[user_id].ship[ship_number].type,
	          temp_ship_name,
	          user[user_id].ship[ship_number].hull_max,
	          user[user_id].ship[ship_number].hull,
	          user[user_id].ship[ship_number].cargo_max,
	          0, //set to zero until cargo is inpletmented
	          user[user_id].ship[ship_number].speed_max,
	          ship_range,
	          user[user_id].ship[ship_number].shield_max,
	          ( user[user_id].ship[ship_number].gun[0] > -1 || user[user_id].ship[ship_number].missile[0] > -1 ) ); //last input is a boolean expresion that returns 0 or 1

	if ( user[user_id].con_id > -1 ) //if user is connected...
		send_con_user ( user[user_id].con_id, rtn_str );
}

extern void set_user_ship_misc ( int user_id, int ship_number )
{
	int k, temp_int, temp_misc, ship_brand, ship_kind, ship_id;

	if ( user[user_id].ship[ship_number].ship_id == -1 || user[user_id].ship[ship_number].brand == -1 || user[user_id].ship[ship_number].type == -1 ) return; //has no ship...

	ship_brand = user[user_id].ship[ship_number].brand;
	ship_kind = user[user_id].ship[ship_number].type;
	ship_id = user[user_id].ship[ship_number].ship_id;

	user[user_id].ship[ship_number].hull_max 	= server[ship_id].ship[ship_brand].hull[ship_kind]; //reset all maxes
	user[user_id].ship[ship_number].shield_max 	= server[ship_id].ship[ship_brand].shield[ship_kind];
	user[user_id].ship[ship_number].cargo_max 	= server[ship_id].ship[ship_brand].cargo[ship_kind];
	user[user_id].ship[ship_number].speed_max 	= server[ship_id].ship[ship_brand].speed[ship_kind];

	temp_int = server[ship_id].ship[ship_brand].misc_ammount[ship_kind];
	for ( k=0;k<temp_int;k++ )
	{
		switch ( user[user_id].ship[ship_number].misc[k] )
		{
			case 0: //hull
			case 4:
				user[user_id].ship[ship_number].hull_max += misc_pod_value ( user[user_id].ship[ship_number].misc[k] );
				break;
			case 1: //shield
			case 5:
				user[user_id].ship[ship_number].shield_max += misc_pod_value ( user[user_id].ship[ship_number].misc[k] );
				break;
			case 2: //cargo
			case 6:
				user[user_id].ship[ship_number].cargo_max += misc_pod_value ( user[user_id].ship[ship_number].misc[k] );
				break;
			case 3: //speed
			case 7:
				user[user_id].ship[ship_number].speed_max += misc_pod_value ( user[user_id].ship[ship_number].misc[k] );
				break;
		}
	}

	//OOKK a few checks...
	if ( user[user_id].ship[ship_number].hull > user[user_id].ship[ship_number].hull_max )
	{
		user[user_id].ship[ship_number].hull = user[user_id].ship[ship_number].hull_max;
		user_send_hull ( user_id );
	}

	if ( user_id_good_amount ( user_id ) > user[user_id].ship[ship_number].cargo_max )
		sell_all_user_ship_goods ( user_id, ship_number );

}

extern void set_user_ship_selected ( int con, int new_selection )
{
	int user_id, u;

	u = user_id = user_con_id[con];//make life easier

	if ( user_id<0 ) return; //doesnt hurt to check
	if ( new_selection<0 || new_selection>9 ) return; //check for bullshit

	//sell the goods for the current ship
	sell_all_user_ship_goods ( u,user[u].ship_selected );

	user[user_id].ship_selected = new_selection;

	send_user_ship_info_to_server ( user_id ); //tell the world
	send_user_ship_info_to_user ( user_id );

}

extern void sell_user_ship_mod ( int con, int mod_type, int mod_number )
{
	int user_id, i, j, temp_ammount, temp_ammount2;
	char rtn_str[50];
	int u, s, sh_sel, sh_id, sh_t, sh_k;

	u = user_id = user_con_id[con];//make life easier
	s = user[u].server_id;
	sh_sel = user[u].ship_selected;
	sh_id = user[u].ship[sh_sel].ship_id;
	sh_t = user[u].ship[sh_sel].brand;
	sh_k = user[u].ship[sh_sel].type;

	if(user_check_lease_loggedin(u)) return;
	if (s == -1) return;
	
	if ( sh_id == -1 || sh_t == -1 || sh_k == -1 ) return; //has no ship...
	if ( !user[user_id].landed ) return; //make sure is landed

	switch ( mod_type )
	{
		case 0: //chassis
			sell_user_ship_chassis ( user_id );

			break;
		case 1: //gun
			temp_ammount = server[sh_id].ship[sh_t].gun_ammount[sh_k];
			if ( mod_number > temp_ammount - 1 ) break; //check if appropriate number
			if ( mod_number < 0 ) break;
			if ( user[user_id].ship[sh_sel].gun[mod_number] < 0 ) break;
			if ( user[user_id].ship[sh_sel].gun_id[mod_number] < 0 ) break;

			//give money and remove item.
			user[user_id].money += ( int ) ( server[sh_id].gun[user[user_id].ship[sh_sel].gun[mod_number]].cost * 0.75 );
			user[user_id].ship[sh_sel].gun[mod_number] = -1;
			user[user_id].ship[sh_sel].gun_id[mod_number] = -1;

			//send the server the news
			//username,mod_type,slot_number,gun/miss/misc_acc,gun/miss/misc_spd,gun/miss/misc_dam,
			sprintf ( rtn_str, "7,%s,1,%d,-1", user[user_id].username, mod_number );
			if ( server[s].con_id > -1 ) //if this server is connected...
				send_con_server ( server[s].con_id, rtn_str );

			//send the user the news
			//mod_type,slot_number,gun/miss/misc_id,gun/miss/misc_value
			sprintf ( rtn_str, "10,0,%d,-1", mod_number );
			send_con_user ( con, rtn_str );

			//send the user their money status
			sprintf ( rtn_str,"13,%d", user[user_id].money );
			send_con_user ( con, rtn_str );

			for ( i=0;i<temp_ammount;i++ ) //look for a gun
				if ( user[user_id].ship[sh_sel].gun[mod_number] > -1 && user[user_id].ship[sh_sel].gun[mod_number] > -1 )
					break;

			temp_ammount2 = server[sh_id].ship[sh_t].missile_ammount[sh_k];

			for ( j=0;j<temp_ammount2;j++ ) //look for a missile
				if ( user[user_id].ship[sh_sel].missile[mod_number] > -1 && user[user_id].ship[sh_sel].missile[mod_number] > -1 )
					break;

			if ( i==temp_ammount && j==temp_ammount ) //if we searched and found no gun or missile
				send_user_hanger_ship ( user_id, sh_sel ); //tell the user that ship is empty

			//do this db stuff
			game.rewrite_user_db = 1;

			break;
		case 2: //missile
			temp_ammount = server[sh_id].ship[sh_t].missile_ammount[sh_k];
			if ( mod_number > temp_ammount - 1 ) break; //check if appropriate number
			if ( mod_number < 0 ) break;
			if ( user[user_id].ship[sh_sel].missile[mod_number] < 0 ) break;
			if ( user[user_id].ship[sh_sel].missile_id[mod_number] < 0 ) break;

			//give money and remove item.
			user[user_id].money += ( int ) ( server[sh_id].missile[user[user_id].ship[sh_sel].missile[mod_number]].cost * 0.75 );
			user[user_id].ship[sh_sel].missile[mod_number] = -1;
			user[user_id].ship[sh_sel].missile_id[mod_number] = -1;

			//send the server the news
			//username,mod_type,slot_number,gun/miss/misc_acc,gun/miss/misc_spd,gun/miss/misc_dam,
			sprintf ( rtn_str, "7,%s,1,%d,-1", user[user_id].username, mod_number );
			if ( server[s].con_id > -1 ) //if this server is connected...
				send_con_server ( server[s].con_id, rtn_str );

			//send the user the news
			//mod_type,slot_number,gun/miss/misc_id,gun/miss/misc_value
			sprintf ( rtn_str, "10,1,%d,-1", mod_number );
			send_con_user ( con, rtn_str );

			//send the user their money status
			sprintf ( rtn_str,"13,%d", user[user_id].money );
			send_con_user ( con, rtn_str );

			for ( i=0;i<temp_ammount;i++ ) //look for a gun
				if ( user[user_id].ship[sh_sel].missile[mod_number] > -1 && user[user_id].ship[sh_sel].missile[mod_number] > -1 )
					break;

			temp_ammount2 = server[sh_id].ship[sh_t].gun_ammount[sh_k];

			for ( j=0;j<temp_ammount2;j++ ) //look for a missile
				if ( user[user_id].ship[sh_sel].missile[mod_number] > -1 && user[user_id].ship[sh_sel].missile[mod_number] > -1 )
					break;

			if ( i==temp_ammount && j==temp_ammount ) //if we searched and found no gun or missile
				send_user_hanger_ship ( user_id, sh_sel ); //tell the user that ship is empty

			//do this db stuff
			game.rewrite_user_db = 1;

			break;
		case 3: //misc
			if ( mod_number > server[sh_id].ship[sh_t].misc_ammount[sh_k] - 1 ) break; //check if appropriate number
			if ( mod_number < 0 ) break;
			if ( user[user_id].ship[sh_sel].misc[mod_number] < 0 ) break;

			//give money and remove item.
			user[user_id].money += ( int ) ( misc_pod_cost ( user[user_id].ship[sh_sel].misc[mod_number] ) * 0.75 );
			user[user_id].ship[sh_sel].misc[mod_number] = -1;

			//set the maxs (ie shield or hull) for this ship
			set_user_ship_misc ( user_id, sh_sel );

			//tell the server about the update
			send_user_ship_info_to_server ( user_id );

			//tell the client
			//mod_type,slot_number,misc_value
			sprintf ( rtn_str, "10,2,%d,-1", mod_number );
			send_con_user ( con, rtn_str );
			send_user_hanger_ship ( user_id, sh_sel );

			//send the user their money status
			sprintf ( rtn_str,"13,%d", user[user_id].money );
			send_con_user ( con, rtn_str );

			//do this db stuff
			game.rewrite_user_db = 1;

			break;
	}
}

void user_sell_ship ( int u, int sh )
{
	int i, j, temp_ammount;
	char rtn_str[50];
	int sh_s, sh_t, sh_k;
	int w_s, w;
	int invalid_ship;

	if ( u < 0 ) return;
	if ( u > user_max ) return;
	if ( ( sh < 0 ) || ( sh >= 10 ) )
	{
		char print_message[500];

		sprintf ( print_message, "user %s tried to sell a ship that is not in the hanger (entry=%d)", user[u].username, sh );
		printd ( print_message );
		return;
	}

	sh_s = user[u].ship[sh].ship_id;
	sh_t = user[u].ship[sh].brand;
	sh_k = user[u].ship[sh].type;

	//ok I guess we should put some checks in...
	invalid_ship = 0;
	if ( sh_s < 0 ) invalid_ship = 1;
	if ( sh_t < 0 ) invalid_ship = 1;
	if ( sh_k < 0 ) invalid_ship = 1;
	if ( sh_s >= MAX_SERVER ) invalid_ship = 1;
	if ( sh_t >= SHIP_MAX ) invalid_ship = 1;
	if ( sh_k >= 8 ) invalid_ship = 1;

	//now does the ship have a name?
	if ( !invalid_ship && !server[sh_s].ship[sh_t].name[0] ) invalid_ship = 1;

	//so is it an invalid ship?
	if ( invalid_ship )
	{
		char print_message[500];

		sprintf ( print_message, "user %s tried to sell a ship that is invalid", user[u].username );
		printd ( print_message );
		return;
	}

	//sell all guns
	temp_ammount = server[sh_s].ship[sh_t].gun_ammount[sh_k];
	for ( i=0;i<temp_ammount;i++ )
	{
		w_s = user[u].ship[sh].gun_id[i];
		w = user[u].ship[sh].gun[i];

		if ( w_s < 0 ) break;
		if ( w < 0 ) break;
		if ( w_s >= MAX_SERVER ) break;
		if ( w >= GUN_MAX ) break;
		if ( !server[w_s].gun[w].name[0] ) break;

		user[u].money += ( int ) ( server[w_s].gun[w].cost * 0.75 );
		user[u].ship[sh].gun[i] = -1;
		user[u].ship[sh].gun_id[i] = -1;
	}

	//sell all missiles
	temp_ammount = server[sh_s].ship[sh_t].missile_ammount[sh_k];
	for ( i=0;i<temp_ammount;i++ )
	{
		w_s = user[u].ship[sh].missile_id[i];
		w = user[u].ship[sh].missile[i];

		if ( w_s < 0 ) break;
		if ( w < 0 ) break;
		if ( w_s >= MAX_SERVER ) break;
		if ( w >= MISSILE_MAX ) break;
		if ( !server[w_s].missile[w].name[0] ) break;

		user[u].money += ( int ) ( server[w_s].missile[w].cost * 0.75 );
		user[u].ship[sh].missile[i] = -1;
		user[u].ship[sh].missile_id[i] = -1;
	}

	//sell all misc items
	temp_ammount = server[sh_s].ship[sh_t].misc_ammount[sh_k];
	for ( i=0;i<temp_ammount;i++ )
	{
		w = user[u].ship[sh].misc[i];

		if ( w < 0 ) break;
		if ( w >= 8 ) break;

		user[u].money += ( int ) ( misc_pod_cost ( w ) * 0.75 );
		user[u].ship[sh].misc[i] = -1;
	}

	//sell and remove ship
	//lots of checks because of claims of no cash returns and randomly lost money
	if ( server[sh_s].ship[sh_t].cost[sh_k] > 0 )
		user[u].money += ( int ) ( server[sh_s].ship[sh_t].cost[sh_k] * 0.75 );
	user[u].ship[sh].ship_id = -1;
	user[u].ship[sh].brand   = -1;
	user[u].ship[sh].type    = -1;

	user_check_reward_min_money ( u );

	//do this db stuff
	game.rewrite_user_db = 1;

	//ship on the face
	mysql_update_ship_entry ( sh_s, sh_t, sh_k );
}

void sell_user_ship_chassis ( int user_id )
{
	int i, j, temp_ammount;
	char rtn_str[50];
	
	//wipe the goods out
	clear_all_user_ship_goods ( user_id, user[user_id].ship_selected );

    //sell the ship
	user_sell_ship ( user_id, user[user_id].ship_selected );

	//people who need to know
	send_user_ship_info_to_user ( user_id );
	send_user_ship_info_to_server ( user_id );

	//reupdate user on hanger status
	send_user_hanger_ship ( user_id, user[user_id].ship_selected );

	//reupdate user on money status
	sprintf ( rtn_str,"13,%d", user[user_id].money );
	send_con_user ( user[user_id].con_id, rtn_str );
}

extern void configure_user_ships()
{
	int i, user_id, ship_brand, ship_kind, ship_id;

	for ( user_id=0; user_id<=user_max; user_id++ ) //go through all users
		for ( i=0;i<10;i++ )		 //add configure all their ships
		{
			ship_brand 	= user[user_id].ship[i].brand;
			ship_kind 	= user[user_id].ship[i].type;
			ship_id 	= user[user_id].ship[i].ship_id;

			if ( ship_id < 0 || ship_id > MAX_SERVER - 1 ) continue;
			if ( ship_brand < 0 || ship_brand > SHIP_MAX - 1 ) continue; //check for ship
			if ( ship_kind < 0 || ship_kind > 7 ) continue;

			set_user_ship_misc ( user_id,i ); //those MK additions we all love

			clear_all_user_ship_goods ( user_id,i );

			user[user_id].ship[i].hull = user[user_id].ship[i].hull_max; //free repair
		}
}

void repair_user ( int con )
{
	int u,i,g,s,p, temp_cost;
	char rtn_str[48];
	int guild_tax = 0;

	u = i = user_con_id[con];
	p = user[u].last_planet;
	s = user[u].server_id;
	g = user[u].guild_id;

	if(s<0) return;
	
	//check for ship
	if ( user[i].ship[user[i].ship_selected].ship_id < 0 ) return;
	if ( user[i].ship[user[i].ship_selected].brand < 0 ) return;
	if ( user[i].ship[user[i].ship_selected].type < 0 ) return;

	temp_cost = 10 * ( user[i].ship[user[i].ship_selected].hull_max - user[i].ship[user[i].ship_selected].hull );

	//no repairs here son
	if ( temp_cost == 0 ) return;

	//got enough?
	if ( temp_cost > user[i].money )
	{
		if ( user[i].money > 0 ) //all for partial repair if we can
		{
			//reset cost
			temp_cost = user[i].money / 10;
			user[i].money -= temp_cost * 10;
			user[i].ship[user[i].ship_selected].hull += temp_cost;

			//must be checked..
			if ( user[i].ship[user[i].ship_selected].hull > user[i].ship[user[i].ship_selected].hull_max )
				user[i].ship[user[i].ship_selected].hull = user[i].ship[user[i].ship_selected].hull_max;
		}
		else
		{
			return;
		}
	}
	else
	{
		//do the stuff
		user[i].money -= temp_cost;
		user[i].ship[user[i].ship_selected].hull = user[i].ship[user[i].ship_selected].hull_max;

	}

	//taxes
	if ( user[i].money >= MIN_MONEY && user[i].guild_balance >= 0 )
	{
		guild_tax = ( int ) ( temp_cost * 0.05 );
		pay_planet_and_user_guild ( g,s,p,guild_tax );
	}

	//tell the server
	if ( user[i].server_id > -1 && server[user[i].server_id].con_id > -1 )
	{
		sprintf ( rtn_str, "9,%s,%d", user[i].username, user[i].ship[user[i].ship_selected].hull );
		send_con_server ( server[user[i].server_id].con_id, rtn_str );
	}

	//update money
	sprintf ( rtn_str,"13,%d", user[i].money );
	send_con_user ( con, rtn_str );

	//tell the user
	//server is now responsible for telling the user
	//user_send_hull(i);

	//do this db stuff
	game.rewrite_user_db = 1;
}

void user_send_hull ( int u )
{
	char rtn_str[500];

	if ( user[u].con_id < 0 ) return;

	sprintf ( rtn_str, "15,%d", user[u].ship[user[u].ship_selected].hull );
	send_con_user ( user[u].con_id, rtn_str );
}

void reload_user_missile ( int con, int wep_num )
{
	int i, temp_cost, max_reload;
	char rtn_str[48];
	int cost, guild_tax;
	int u, s, p, g;

	u = i = user_con_id[con];
	p = user[u].last_planet;
	s = user[u].server_id;
	g = user[u].guild_id;

	if(s<0) return;
	
	//check for ship
	if ( user[i].ship[user[i].ship_selected].ship_id < 0 ) return;
	if ( user[i].ship[user[i].ship_selected].brand < 0 ) return;
	if ( user[i].ship[user[i].ship_selected].type < 0 ) return;

	//check if valid wep_num
	if ( wep_num < 0 ) return;
	if ( wep_num >= server[user[i].ship[user[i].ship_selected].ship_id].ship[user[i].ship[user[i].ship_selected].brand].missile_ammount[user[i].ship[user[i].ship_selected].type] ) return;

	//.....
	if ( user[i].ship[user[i].ship_selected].missile_left[wep_num] >= 4 ) return;

	max_reload = 4 - user[i].ship[user[i].ship_selected].missile_left[wep_num];

	//check price
	temp_cost = server[user[i].ship[user[i].ship_selected].missile_id[wep_num]].missile[user[i].ship[user[i].ship_selected].missile[wep_num]].cost / 4;

	if ( temp_cost )
		max_reload = user[i].money / temp_cost;
	else
		max_reload = 4;

	if ( max_reload > 4 - user[i].ship[user[i].ship_selected].missile_left[wep_num] )
		max_reload = 4 - user[i].ship[user[i].ship_selected].missile_left[wep_num];

	if ( max_reload == 0 ) return;

	user[i].ship[user[i].ship_selected].missile_left[wep_num] += max_reload;

	cost = max_reload * temp_cost;
	user[i].money -= cost;
	if ( user[i].money >= MIN_MONEY && user[i].guild_balance >= 0 )
	{
		guild_tax = ( int ) ( cost * 0.05 );
		pay_planet_and_user_guild ( g,s,p,guild_tax );
	}

	//update money
	sprintf ( rtn_str,"13,%d", user[i].money );
	send_con_user ( con, rtn_str );

	//tell server
	if ( user[i].server_id > -1 && server[user[i].server_id].con_id > -1 )
	{
		sprintf ( rtn_str,"10,%s,%d,%d", user[i].username, wep_num, user[i].ship[user[i].ship_selected].missile_left[wep_num] );
		send_con_server ( server[user[i].server_id].con_id, rtn_str );
	}

	//tell user
	sprintf ( rtn_str,"17,%d,%d", wep_num, user[i].ship[user[i].ship_selected].missile_left[wep_num] );
	send_con_user ( con, rtn_str );

	//do this db stuff
	game.rewrite_user_db = 1;
}

extern void repair_user_gun ( int con, int g )
{
	int i, temp_cost;
	char rtn_str[48];
	int sh_sel, sh_id, sh_t, sh_k;
	int g_id, g_t;
	int cost, guild_tax;
	int u, s, p, guild_id;

	u = i = user_con_id[con];
	p = user[u].last_planet;
	s = user[u].server_id;
	guild_id = user[u].guild_id;

	if(s<0) return;
	
	//set a bitch
	sh_sel = user[i].ship_selected;

	//ship exist?
	if ( user[i].ship[sh_sel].ship_id < 0 ) return;
	if ( user[i].ship[sh_sel].type < 0 ) return;
	if ( user[i].ship[sh_sel].brand < 0 ) return;

	//sseettt
	sh_id = user[i].ship[sh_sel].ship_id;
	sh_t = user[i].ship[sh_sel].brand;
	sh_k = user[i].ship[sh_sel].type;


	//check the num
	if ( g<0 ) return;
	if ( g>=server[sh_id].ship[sh_t].gun_ammount[sh_k] ) return;

	//set more shit!
	g_id = user[i].ship[sh_sel].gun_id[g];
	g_t = user[i].ship[sh_sel].gun[g];

	//we even have this gun?
	if ( g_id<0 ) return;
	if ( g_t<0 ) return;

	//is it destroyed?
	if ( !user[i].ship[sh_sel].gun_destroyed[g] ) return;

	//got enough cash?
	temp_cost = ( int ) ( 0.25 * server[g_id].gun[g_t].cost );
	if ( user[i].money < temp_cost ) return;
	user[i].money -= temp_cost;
	if ( user[i].money >= MIN_MONEY && user[i].guild_balance >= 0)
	{
		guild_tax = ( int ) ( temp_cost * 0.05 );
		pay_planet_and_user_guild ( guild_id,s,p,guild_tax );
	}


	//set
	user[i].ship[sh_sel].gun_destroyed[g] = 0;

	//tell the people.
	relay_user_gun_repaired ( i,g );

	//monneyy
	user_send_money ( con );

	//do this db stuff
	game.rewrite_user_db = 1;
}

void relay_user_gun_repaired ( int u, int g )
{
	char rtn_str[200];
	int s;

	s = user[u].server_id;

	if(s<0) return;
	
	//tell server
	if ( server[s].con_id > -1 )
	{
		sprintf ( rtn_str,"28,%s,%d",user[u].username, g );
		send_con_server ( server[s].con_id,rtn_str );
	}


	//tell client
	sprintf ( rtn_str,"16,%d",g );
	send_con_user ( user[u].con_id,rtn_str );
}

extern void send_user_gun_info ( int con, int wep_id, int wep_num )
{
	char rtn_str[128];

	sprintf ( rtn_str, "18,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d",
	          wep_id,
	          wep_num,
	          server[wep_id].gun[wep_num].name,
	          server[wep_id].gun[wep_num].cost,
	          server[wep_id].gun[wep_num].damage,
	          server[wep_id].gun[wep_num].speed,
	          server[wep_id].gun[wep_num].accuracy,
	          server[wep_id].gun[wep_num].exp_static,
	          server[wep_id].gun[wep_num].exp_added,
	          server[wep_id].gun[wep_num].tech_level,
	          server[wep_id].gun[wep_num].type );
	send_con_user ( con, rtn_str );
}

void send_user_missile_info ( int con, int wep_id, int wep_num )
{
	char rtn_str[128];

	sprintf ( rtn_str, "19,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d",
	          wep_id,
	          wep_num,
	          server[wep_id].missile[wep_num].name,
	          server[wep_id].missile[wep_num].cost,
	          server[wep_id].missile[wep_num].damage,
	          server[wep_id].missile[wep_num].speed,
	          server[wep_id].missile[wep_num].accuracy,
	          server[wep_id].missile[wep_num].exp_static,
	          server[wep_id].missile[wep_num].exp_added,
	          server[wep_id].missile[wep_num].tech_level,
	          server[wep_id].missile[wep_num].type );
	send_con_user ( con, rtn_str );
}

void user_purchase_goods ( int con, int i, int ammount )
{
	int p,s,sh,u,g, cost; //planet/server/ship num
	char rtn_str[1000];
	int guild_tax;

	//is good_num valid?
	if ( i < 0 ) return;
	if ( i > 9 ) return;
	if ( ammount < 1 ) return;

	u = user_con_id[con];
	p = user[u].last_planet;
	s = user[u].server_id;
	sh = user[u].ship_selected;
	g = user[u].guild_id;

	if(user_check_lease_loggedin(u)) return;
	
	//is planet and server valid?
	if ( p < 0 ) return;
	if ( s < 0 ) return;

	//set what the cost will be
	cost = ( server[s].planet[p].good_price[i] + TRADE_POINT_LOSS ) * ammount;

	//check if we have ship at all
	if ( user[u].ship[sh].ship_id < 0 ) return;
	//check if we landed
	if ( !user[u].landed ) return;
	//check if is a gate
	if ( server[s].planet[p].is_gate ) return;
	//check if planet has the goods
	if ( server[s].planet[p].good_ammount[i] < ammount ) return;
	//check if we have room
	if ( user[u].ship[sh].cargo_max - user_good_ammount ( con ) < ammount ) return;
	//check if we have money
	if ( user[u].money - cost < 0 ) return;

	//well i guess were good then.

	//give goods
	user[u].money -= cost;
	user[u].ship[sh].cargo[i] += ammount;
	server[s].planet[p].good_ammount[i] -= ammount;

	//payout the guilds!
	if ( user[u].money >= MIN_MONEY && user[u].guild_balance >= 0 )
	{
		guild_tax = TRADE_POINT_LOSS * ammount;
		pay_planet_and_user_guild ( g,s,p,guild_tax );
	}

	//tell server
	server_update_good_ammount ( s,p,i );

	//tell client
	user_send_money ( con );
	user_update_good_ammount ( con,i );

	//server wants to know this too
	set_and_send_server_user_has_goods ( u,1 );

	//do this db stuff
	game.rewrite_user_db = 1;
}

void user_sell_goods ( int con, int good_num, int ammount )
{
	int u,g,s,p,sh,i;
	int guild_tax;

	//is good_num valid?
	if ( good_num < 0 ) return;
	if ( good_num > 9 ) return;
	if ( ammount < 1 ) return;

	u = user_con_id[con];
	p = user[u].last_planet;
	s = user[u].server_id;
	sh = user[u].ship_selected;
	i = good_num;
	g = user[u].guild_id;

	//is planet and server valid?
	if ( p < 0 ) return;
	if ( s < 0 ) return;

	//check if we have ship at all
	if ( user[u].ship[sh].ship_id < 0 ) return;
	//check if we landed
	if ( !user[u].landed ) return;
	//check if is a gate
	if ( server[s].planet[p].is_gate ) return;
	//check if we have this much of the good
	if ( user[u].ship[sh].cargo[i] < ammount ) return;

	//i guess we're good

	//sell goods
	user[u].money += ( server[s].planet[p].good_price[i] - TRADE_POINT_LOSS ) * ammount;
	user[u].ship[sh].cargo[i] -= ammount;
	server[s].planet[p].good_ammount[i] += ammount;

	//payout the guilds!
	if ( user[u].money >= MIN_MONEY && user[u].guild_balance >= 0 )
	{
		guild_tax = TRADE_POINT_LOSS * ammount;
		pay_planet_and_user_guild ( g,s,p,guild_tax );
	}

	//tell server
	server_update_good_ammount ( s,p,i );

	//tell client
	user_send_money ( con );
	user_update_good_ammount ( con,i );

	//yyeeee
	set_and_send_server_user_has_goods ( u,user_has_goods ( u ) );

	//do this db stuff
	game.rewrite_user_db = 1;
}

void user_update_good_ammount ( int con, int good_num )
{
	char rtn_str[200];
	int i;

	i = user_con_id[con];

	//19
	sprintf ( rtn_str,"20,%d,%d,%d",user[i].ship_selected,good_num,user[i].ship[user[i].ship_selected].cargo[good_num] );
	send_con_user ( con, rtn_str );

}

void user_update_contra_ammount ( int con, int g )
{
	char rtn_str[200];
	int i;

	i = user_con_id[con];

	//19
	sprintf ( rtn_str,"26,%d,%d,%d",user[i].ship_selected, g, user[i].ship[user[i].ship_selected].contra[g] );

	send_con_user ( con, rtn_str );
}

int user_good_ammount ( int con )
{
	int u;

	u = user_con_id[con];

	return user_id_good_amount ( u );
}

int user_id_good_amount ( int u )
{
	int i;
	int ammount = 0;

	if ( user[u].ship[user[u].ship_selected].ship_id < 0 )
		return 0;

	for ( i=0;i<10;i++ )
	{
		ammount += user[u].ship[user[u].ship_selected].cargo[i];
		ammount += user[u].ship[user[u].ship_selected].contra[i];
	}

	return ammount;
}

int user_cargo_amount ( int u )
{
	int i;
	int ammount = 0;

	if ( user[u].ship[user[u].ship_selected].ship_id < 0 )
		return 0;

	for ( i=0;i<10;i++ )
	{
		ammount += user[u].ship[user[u].ship_selected].cargo[i];
		ammount += user[u].ship[user[u].ship_selected].contra[i];
	}

	return ammount;
}

void user_send_money ( int con )
{
	char rtn_str[200];
	int u;

	if ( con < 0 ) return;
	if ( con > user_con_top ) return;

	u = user_con_id[con];

	if ( u < 0 ) return;
	if ( u > user_max ) return;

	sprintf ( rtn_str,"13,%d", user[u].money );
	send_con_user ( con, rtn_str );
}

void user_send_cargo_info ( int con )
{
	char send_str[1000];
	int u,sh;

	u = user_con_id[con];
	sh = user[u].ship_selected;

	//setup the send string for the selected ship
	sprintf ( send_str, "9,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
	          sh,
	          user[u].ship[sh].cargo[0],
	          user[u].ship[sh].cargo[1],
	          user[u].ship[sh].cargo[2],
	          user[u].ship[sh].cargo[3],
	          user[u].ship[sh].cargo[4],
	          user[u].ship[sh].cargo[5],
	          user[u].ship[sh].cargo[6],
	          user[u].ship[sh].cargo[7],
	          user[u].ship[sh].cargo[8],
	          user[u].ship[sh].cargo[9] );

	send_con_user ( con,send_str );
}

void user_register_guild ( int con, char *new_guild )
{
	char lnew_guild[21], lcurrent_guild[21];
	int i,k,u,s;
	char rtn_str[1000];

	u = user_con_id[con];
	s = user[u].server_id;

	//make sure we are landed
	if ( !user[u].landed ) return;

	//logged on a server?
	if ( s < -1 ) return;
	if ( s >= MAX_SERVER ) return;
	if ( server[s].con_id < 0 ) return;

	//non crashable planet?
	if ( user[u].last_planet < -1 ) return;
	if ( user[u].last_planet >= PLANET_MAX ) return;

	//planet has guildhalls?
	if ( !server[s].planet[user[u].last_planet].has_guild_halls ) return;

	if(user_check_lease_loggedin(u)) return;
	
	//check the fools money
	if ( user[u].money < GUILD_COST )
	{
		sprintf ( rtn_str,"2,%d more credits required.", GUILD_COST - user[u].money );
		send_con_user ( con,rtn_str );
		return;
	}

	//check if they owe their current bank
	if ( user[u].guild_balance < 0 )
	{
		sprintf ( rtn_str,"2,Please pay current bank depts." );
		send_con_user ( con,rtn_str );
		return;
	}

	//don't care for blank entries.
	if ( !new_guild[0] ) return;

	//cap it friends
	new_guild[20] = 0;

	lcase ( lnew_guild,new_guild );

	//see if we can find
	for ( i=0;i<=guild_max;i++ )
	{
		lcase ( lcurrent_guild, guild[i].name );

		//guild already found?
		if ( !strcmp ( lcurrent_guild,lnew_guild ) )
		{
			sprintf ( rtn_str,"2,%s has already been created.",guild[i].name );
			send_con_user ( con,rtn_str );
			return;
		}
	}

	//make the guild
	guild_max++;
	strcpy ( guild[guild_max].name,new_guild );
	strcpy ( guild[guild_max].owner,user[u].username );
	sprintf ( guild[guild_max].website,"www.%s.com",new_guild );
	guild[guild_max].bank_money = 50000;
	guild[guild_max].bank_lend = 100000;
	strcpy ( guild[guild_max].rank_name[5],"Owner" );
	strcpy ( guild[guild_max].rank_name[4],"General" );
	strcpy ( guild[guild_max].rank_name[3],"Recruiter" );
	strcpy ( guild[guild_max].rank_name[2],"Leader" );
	strcpy ( guild[guild_max].rank_name[1],"Grunt" );
	strcpy ( guild[guild_max].rank_name[0],"Scout" );
	for ( i=0;i<6;i++ )
		for ( k=0;k<5;k++ )
			guild[guild_max].rank_allow[i][k] = 0;
	strcpy ( guild[guild_max].enemy[2],"Rin" );
	strcpy ( guild[guild_max].enemy[1],"Klyn" );
	strcpy ( guild[guild_max].enemy[0],"Miner" );

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


	//remove them from their current guild
	user_leave_guild ( u );

	//add the player
	user[u].guild_id = guild_max;
	user[u].guild_rank = 5;

	//remove their cash
	user[u].money -= GUILD_COST;

	//tell everyone the news
	sprintf ( rtn_str,"2,The universe recognizes your command." );
	send_con_user ( con,rtn_str );

	send_user_name_guild ( u );
	user_send_guild_info ( con );
	user_send_guild_balance ( con );
	user_send_money ( con );

	//tell the server
	server_update_user_guild ( s,u );

	//db stuff
	game.rewrite_guild_db = 1;
}

void send_user_name_guild ( int u )
{
	char rtn_str[1000];
	int g;

	g = user[u].guild_id;

	if ( g > -1 && g <= guild_max )
	{
		sprintf ( rtn_str,"12,%s,%s,%d", user[u].username, guild[g].name, user[u].guild_rank );
		send_con_user ( user[u].con_id, rtn_str );
	}
	else
	{
		sprintf ( rtn_str,"12,%s,Rogue,0", user[u].username );
		send_con_user ( user[u].con_id, rtn_str );
	}
}

int user_leave_guild ( int u )
{
	int g;

	g = user[u].guild_id;

	//check if they are in a guild
	if ( g > guild_max ) return 0;
	if ( g < 0 ) return 0;

	//get them out
	user[u].guild_id = 0;
	user[u].guild_rank = 0;

	//bye bye decos
	user_clear_decorations ( u );

	//db stuff
	game.rewrite_user_db = 1;
	game.rewrite_guild_db = 1;

	//since the good soul is in a guild, lets get our money back god fucking damnit
	if ( user[u].guild_balance > 0 )
	{
		if ( guild[g].bank_money >= user[u].guild_balance )
		{
			guild[g].bank_money -= user[u].guild_balance;
			user[u].money += user[u].guild_balance;
			user[u].guild_balance = 0;
		}
		else //bank doesnt have our money ;(
		{
			if ( !guild[g].bank_money ) return 0; //bank has no money? no money changes then

			user[u].money += guild[g].bank_money;
			user[u].guild_balance = 0;
			guild[g].bank_money = 0;
		}

		return 1; //return 1 meaning there was some cash differences made
	}
	else if ( user[u].guild_balance < 0 ) //cough it up
	{
		guild[g].bank_money -= user[u].guild_balance;
		user[u].money += user[u].guild_balance;
		user[u].guild_balance = 0;
		return 1; //return 1 meaning there was some cash differences made
	}

	return 0;
}

void user_relay_guild_listings ( int con )
{
	int i;
	char rtn_str[200];

	//no guilds?
	if ( guild_max < 0 ) return;

	for ( i=0;i<=guild_max;i++ )
	{
		//i,name,worth,players
		sprintf ( rtn_str,"22,%d,%s,%d,%d", i, guild[i].name, guild_worth ( i ), guild_players ( i ) );
		send_con_user ( con,rtn_str );
	}

	//tell the user to redraw the guild listings
	strcpy ( rtn_str,"33," );
	send_con_user ( con,rtn_str );

	user_send_specific_guild_info ( con,0 );
}

void user_request_guild_info ( int con, char *guild_name )
{
	char lcurrent_guild[21], lnew_guild[21];
	int i;

	lcase ( lnew_guild,guild_name );

	//see if we can find
	for ( i=0;i<=guild_max;i++ )
	{
		lcase ( lcurrent_guild, guild[i].name );

		//guild already found?
		if ( !strcmp ( lcurrent_guild,lnew_guild ) )
		{
			user_send_specific_guild_info ( con,i );
			return;
		}
	}
}

void user_send_specific_guild_info ( int con, int g )
{
	char rtn_str[1000];
	int i;

	sprintf ( rtn_str, "23,%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d",
	          guild[g].name,
	          guild[g].owner,
	          guild[g].website,
	          guild[g].bank_money,
	          guild[g].bank_lend,
	          guild[g].rank_name[0],
	          guild[g].rank_name[1],
	          guild[g].rank_name[2],
	          guild[g].rank_name[3],
	          guild[g].rank_name[4],
	          guild[g].rank_name[5],
	          guild[g].enemy[0],
	          guild[g].enemy[1],
	          guild[g].enemy[2],
	          guild[g].rank_allow[0][0],
	          guild[g].rank_allow[0][1],
	          guild[g].rank_allow[0][2],
	          guild[g].rank_allow[0][3],
	          guild[g].rank_allow[0][4],
	          guild[g].rank_allow[1][0],
	          guild[g].rank_allow[1][1],
	          guild[g].rank_allow[1][2],
	          guild[g].rank_allow[1][3],
	          guild[g].rank_allow[1][4],
	          guild[g].rank_allow[2][0],
	          guild[g].rank_allow[2][1],
	          guild[g].rank_allow[2][2],
	          guild[g].rank_allow[2][3],
	          guild[g].rank_allow[2][4],
	          guild[g].rank_allow[3][0],
	          guild[g].rank_allow[3][1],
	          guild[g].rank_allow[3][2],
	          guild[g].rank_allow[3][3],
	          guild[g].rank_allow[3][4],
	          guild[g].rank_allow[4][0],
	          guild[g].rank_allow[4][1],
	          guild[g].rank_allow[4][2],
	          guild[g].rank_allow[4][3],
	          guild[g].rank_allow[4][4],
	          guild[g].rank_allow[5][0],
	          guild[g].rank_allow[5][1],
	          guild[g].rank_allow[5][2],
	          guild[g].rank_allow[5][3],
	          guild[g].rank_allow[5][4],
	          guild_players ( g ) );

	for ( i=0;i<MAX_DECO;i++ )
	{
		char cat_str[500];

		sprintf ( cat_str,",%s,%d", guild[g].deco_name[i], guild[g].deco_activated[i] );
		strcat ( rtn_str,cat_str );
	}

	send_con_user ( con,rtn_str );
}

void user_send_guild_info ( int con )
{
	int g;
	int u, r;
	int i;
	char rtn_str[1000];
	char cat_str[1000];

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;



	//check if they have the right allowence
	if ( !user_deserve_guild_info ( u,g,r ) )
	{
		//send the guilds balance for all those who might actually deserve atleast that..
		user_send_guilds_money ( u );

		return;
	}

	//send it
	sprintf ( rtn_str, "21,%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d",
	          guild[g].name,
	          guild[g].owner,
	          guild[g].website,
	          guild[g].bank_money,
	          guild[g].bank_lend,
	          guild[g].rank_name[0],
	          guild[g].rank_name[1],
	          guild[g].rank_name[2],
	          guild[g].rank_name[3],
	          guild[g].rank_name[4],
	          guild[g].rank_name[5],
	          guild[g].enemy[0],
	          guild[g].enemy[1],
	          guild[g].enemy[2],
	          guild[g].rank_allow[0][0],
	          guild[g].rank_allow[0][1],
	          guild[g].rank_allow[0][2],
	          guild[g].rank_allow[0][3],
	          guild[g].rank_allow[0][4],
	          guild[g].rank_allow[1][0],
	          guild[g].rank_allow[1][1],
	          guild[g].rank_allow[1][2],
	          guild[g].rank_allow[1][3],
	          guild[g].rank_allow[1][4],
	          guild[g].rank_allow[2][0],
	          guild[g].rank_allow[2][1],
	          guild[g].rank_allow[2][2],
	          guild[g].rank_allow[2][3],
	          guild[g].rank_allow[2][4],
	          guild[g].rank_allow[3][0],
	          guild[g].rank_allow[3][1],
	          guild[g].rank_allow[3][2],
	          guild[g].rank_allow[3][3],
	          guild[g].rank_allow[3][4],
	          guild[g].rank_allow[4][0],
	          guild[g].rank_allow[4][1],
	          guild[g].rank_allow[4][2],
	          guild[g].rank_allow[4][3],
	          guild[g].rank_allow[4][4],
	          guild[g].rank_allow[5][0],
	          guild[g].rank_allow[5][1],
	          guild[g].rank_allow[5][2],
	          guild[g].rank_allow[5][3],
	          guild[g].rank_allow[5][4] );

	for ( i=0;i<MAX_DECO;i++ )
	{
		sprintf ( cat_str,",%s,%d", guild[g].deco_name[i], guild[g].deco_activated[i] );
		strcat ( rtn_str,cat_str );
	}

	sprintf ( cat_str,"%c",13 );
	strcat ( rtn_str,cat_str );


	send_con_user ( con,rtn_str );

}

int user_deserve_guild_info ( int u, int g, int r )
{
	if ( guild[g].rank_allow[r][0] ) return 1;
	if ( guild[g].rank_allow[r][1] ) return 1;
	if ( guild[g].rank_allow[r][2] ) return 1;
	if ( guild[g].rank_allow[r][4] ) return 1;
	if ( !strcmp ( guild[g].owner,user[u].username ) ) return 1;

	return 0;
}

void user_edit_guild_more ( int con, char *message )
{
	int g;
	int u, r, i, k, point = 0;
	char temp_str[1030];

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//set the guild enemies
	for ( i=0;i<3;i++ )
	{
		split ( temp_str,message,',',&point );
		temp_str[20] = 0;
		strcpy ( guild[g].enemy[i],temp_str );
	}

	//notify all other only "editors" of the change
	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] != -1 )
		{
			u = user_con_id[i];

			//in same guild?
			if ( g != user[u].guild_id ) continue;

			//let this handle it from here.
			user_send_guild_info ( i );
		}

	//db stuff
	game.rewrite_guild_db = 1;
}

void user_edit_guild ( int con, char *message )
{
	int g;
	int u, r, i, k, point;
	char temp_str[17][1030];

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//edit guild
	i=strlen ( message );
	for ( k=0;k<17;k++ )
		temp_str[k][0] = '\0';
	for ( point=k=0;point<=i && k<17;k++ )
		split ( temp_str[k],message,',',&point );

	//cappin
	temp_str[0][200] = 0;
	temp_str[2][20] = 0;
	temp_str[3][20] = 0;
	temp_str[4][20] = 0;
	temp_str[5][20] = 0;
	temp_str[6][20] = 0;
	temp_str[7][20] = 0;

	//strcpy(guild[g].name,temp_str[0]); //NOTE!!! a faster way you could read in the db file is to use split and "read" directly into user[g].whatever for all string variables
	//strcpy(guild[g].owner,temp_str[1]);
	strcpy ( guild[g].website,temp_str[0] );
	//guild[g].bank_money = atoi(temp_str[3]);
	guild[g].bank_lend = atoi ( temp_str[1] );
	strcpy ( guild[g].rank_name[0],temp_str[2] );
	strcpy ( guild[g].rank_name[1],temp_str[3] );
	strcpy ( guild[g].rank_name[2],temp_str[4] );
	strcpy ( guild[g].rank_name[3],temp_str[5] );
	strcpy ( guild[g].rank_name[4],temp_str[6] );
	strcpy ( guild[g].rank_name[5],temp_str[7] );
	for ( k=0;k<5;k++ ) guild[g].rank_allow[0][k] = temp_str[8][k] - '0';
	for ( k=0;k<5;k++ ) guild[g].rank_allow[1][k] = temp_str[9][k] - '0';
	for ( k=0;k<5;k++ ) guild[g].rank_allow[2][k] = temp_str[10][k] - '0';
	for ( k=0;k<5;k++ ) guild[g].rank_allow[3][k] = temp_str[11][k] - '0';
	for ( k=0;k<5;k++ ) guild[g].rank_allow[4][k] = temp_str[12][k] - '0';
	for ( k=0;k<5;k++ ) guild[g].rank_allow[5][k] = temp_str[13][k] - '0';

	//another check
	for ( i=0;i<6;i++ )
		for ( k=0;k<5;k++ )
			if ( guild[g].rank_allow[i][k] && guild[g].rank_allow[i][k] != 1 )
				guild[g].rank_allow[i][k] = 1;

	//notify all other only "editors" of the change
	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] != -1 )
		{
			u = user_con_id[i];

			//in same guild?
			if ( g != user[u].guild_id ) continue;

			//let this handle it from here.
			user_send_guild_info ( i );
		}

	//db stuff
	game.rewrite_guild_db = 1;
}

void user_withdraw_bank ( int con, int ammount )
{
	int u,g,r;
	int i;

	//waste our time?
	if ( ammount <= 0 ) return;

	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][3] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//bank going to allow this?
	if ( -guild[g].bank_lend > user[u].guild_balance - ammount ) return;
	if ( guild[g].bank_money - ammount < 0 ) return;

	//make the transfer
	guild[g].bank_money -= ammount;
	user[u].money += ammount;
	user[u].guild_balance -= ammount;

	//let yourself know of your finacial changes
	user_send_guild_balance ( con );
	user_send_money ( con );

	//let everyone else know that should need to know
	notify_guild_money ( g );


	//db stuff
	game.rewrite_guild_db = 1;
	game.rewrite_user_db = 1;
}

void user_deposit_bank ( int con, int ammount )
{
	int u,g,r;
	int i;

	//waste our time?
	if ( ammount <= 0 ) return;

	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][3] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//you got the money for it?
	if ( ( user[u].money - MIN_MONEY ) - ammount < 0 ) return;

	//make the transfer
	guild[g].bank_money += ammount;
	user[u].money -= ammount;
	user[u].guild_balance += ammount;

	//let yourself know of your finacial changes
	user_send_guild_balance ( con );
	user_send_money ( con );

	//let everyone else know that should need to know
	notify_guild_money ( g );

	//db stuff
	game.rewrite_guild_db = 1;
	game.rewrite_user_db = 1;
}

void notify_guild_money ( int g )
{
	int i,u;


	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] != -1 )
		{
			u = user_con_id[i];

			//in same guild?
			if ( g != user[u].guild_id ) continue;

			user_send_guilds_money ( u );
		}
}

void user_send_guilds_money ( int u )
{
	char rtn_str[200];
	int g, r, con;

	//sets
	g = user[u].guild_id;
	r = user[u].guild_rank;
	con = user[u].con_id;

	//checkes
	if ( g<0 ) return;
	if ( r<0 ) return;
	if ( r>5 ) return;
	if ( con<0 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][3] && strcmp ( guild[g].owner,user[u].username ) ) return;

	sprintf ( rtn_str,"24,%d",guild[g].bank_money );
	send_con_user ( con,rtn_str );
}

void user_send_guild_balance ( int con )
{
	int u;
	char rtn_str[500];

	u = user_con_id[con];

	if ( u<0 ) return;
	if ( u>MAX_USER ) return;

	sprintf ( rtn_str,"25,%d",user[u].guild_balance );
	send_con_user ( con,rtn_str );

	user[u].guild_balance_update = 0;
}

int user_exp_algo ( int user_exp, int enemy_exp )
{
	int difference;

	//set the cap
	enemy_exp = enemy_exp + enemy_exp;

	//one simple check
	if ( user_exp > enemy_exp ) return 0;

	//set diff
	difference = enemy_exp - user_exp;

	//checks
	if ( difference >= 400 )
		return 4;
	else if ( difference >= 300 )
		return 3;
	else if ( difference >= 200 )
		return 2;
	else
		return 1;
}

void user_send_exp ( int u )
{
	char rtn_str[200];

	if(user[u].con_id < 0) return;
	
	sprintf ( rtn_str,"27,%d",user[u].exp );
	send_con_user ( user[u].con_id,rtn_str );
}

void user_logoff_all_server ( int u )
{
	int i;
	char off_str[200];

	sprintf ( off_str,"21,%s%c",user[u].username,13 );

	for ( i=0;i<=server_con_top;i++ )
		if ( server_con_id[i] != -1 )
			raw_send_con_server ( i,off_str );
}

int user_worth ( int u )
{
	int i;
	int collected_worth = 0;

	//money
	collected_worth = user[u].money;

	//guild
	collected_worth += user[u].guild_balance;

	//ships
	for ( i=0;i<10;i++ )
		collected_worth += user_ship_worth ( u,i );

	return collected_worth;
}

int user_ship_worth ( int u, int s )
{
	int k;
	int id, type, kind;
	int collected_worth = 0;
	int m_top, m, m_id;
	int top;

	id = user[u].ship[s].ship_id;
	type = user[u].ship[s].brand;
	kind = user[u].ship[s].type;

	//check it ship is real
	if ( id < 0 ) return 0;
	if ( type < 0 ) return 0;
	if ( kind < 0 ) return 0;
	if ( id >= MAX_SERVER ) return 0;
	if ( type >= SHIP_MAX ) return 0;
	if ( kind >= 8 ) return 0;

	//well it is there
	collected_worth = ( int ) ( 0.75 * server[id].ship[type].cost[kind] );

	//guns
	top = server[id].ship[type].gun_ammount[kind];
	for ( k=0;k<top;k++ )
	{
		m = user[u].ship[s].gun[k];
		m_id = user[u].ship[s].gun_id[k];

		//breakers
		if ( m_id < 0 ) continue;
		if ( m < 0 ) continue;
		if ( m_id >= MAX_SERVER ) continue;
		if ( m >= GUN_MAX ) continue;

		collected_worth += ( int ) ( 0.75 * server[m_id].gun[m].cost );
	}

	//missiles
	top = server[id].ship[type].missile_ammount[kind];
	for ( k=0;k<top;k++ )
	{
		m = user[u].ship[s].missile[k];
		m_id = user[u].ship[s].missile[k];

		//breakers
		if ( m_id < 0 ) continue;
		if ( m < 0 ) continue;
		if ( m_id >= MAX_SERVER ) continue;
		if ( m >= MISSILE_MAX ) continue;

		collected_worth += ( int ) ( 0.75 * server[m_id].missile[m].cost );
	}

	//misc
	top = server[id].ship[type].misc_ammount[kind];
	for ( k=0;k<top;k++ )
	{
		m = user[u].ship[s].misc[k];

		//breakers
		if ( m < 0 ) continue;
		if ( m >= 8 ) continue;

		collected_worth += ( int ) ( 0.75 * misc_pod_cost ( m ) );
	}
	//return...
	return collected_worth;
}

int user_ship_worth_lost ( int u, int s )
{
	int k;
	int id, type, kind;
	int collected_worth = 0;
	int m_top, m, m_id;
	int top;

	id = user[u].ship[s].ship_id;
	type = user[u].ship[s].brand;
	kind = user[u].ship[s].type;

	//check it ship is real
	if ( id < 0 ) return 0;
	if ( type < 0 ) return 0;
	if ( kind < 0 ) return 0;
	if ( id >= MAX_SERVER ) return 0;
	if ( type >= SHIP_MAX ) return 0;
	if ( kind >= 8 ) return 0;

	//well it is there
	collected_worth = ( int ) ( 0.25 * server[id].ship[type].cost[kind] );

	//guns
	top = server[id].ship[type].gun_ammount[kind];
	for ( k=0;k<top;k++ )
	{
		m = user[u].ship[s].gun[k];
		m_id = user[u].ship[s].gun_id[k];

		//breakers
		if ( m_id < 0 ) continue;
		if ( m < 0 ) continue;
		if ( m_id >= MAX_SERVER ) continue;
		if ( m >= GUN_MAX ) continue;

		collected_worth += ( int ) ( 0.25 * server[m_id].gun[m].cost );
	}

	//missiles
	top = server[id].ship[type].missile_ammount[kind];
	for ( k=0;k<top;k++ )
	{
		m = user[u].ship[s].missile[k];
		m_id = user[u].ship[s].missile[k];

		//breakers
		if ( m_id < 0 ) continue;
		if ( m < 0 ) continue;
		if ( m_id >= MAX_SERVER ) continue;
		if ( m >= MISSILE_MAX ) continue;

		collected_worth += ( int ) ( 0.25 * server[m_id].missile[m].cost );
	}

	//misc
	top = server[id].ship[type].misc_ammount[kind];
	for ( k=0;k<top;k++ )
	{
		m = user[u].ship[s].misc[k];

		//breakers
		if ( m < 0 ) continue;
		if ( m >= 8 ) continue;

		collected_worth += ( int ) ( 0.25 * misc_pod_cost ( m ) );
	}
	//return...
	return collected_worth;
}

void user_retrive_stats ( int con, char *username )
{
	int u, i, g;
	char rtn_str[1000];

	u = user_to_id ( username );

	if ( u != -1 )
	{
		char *player_name, *guild_name, *guild_rank;
		int worth, exp, kills;
		char *home_planet, *home_server, *last_server;
		char *ship_name;
		int online, activated;
		int ship_id, ship_type, ship_kind;
		int class_points;
		char *not_found = "Not Found";

		//start setting stuff
		player_name = user[u].username;
		guild_name = guild[user[u].guild_id].name;
		guild_rank = guild[user[u].guild_id].rank_name[user[u].guild_rank];

		worth = user_worth ( u );
		exp = user[u].exp;
		kills = user[u].kills;

		home_planet = not_found;
		home_server = not_found;
		last_server = not_found;

		if ( user[u].home_id > -1 )
		{
			if ( user[u].home_planet > -1 )
				home_planet = server[user[u].home_id].planet[user[u].home_planet].name;
			home_server = server[user[u].home_id].servername;
		}
		if ( user[u].server_id > -1 )
			last_server = server[user[u].server_id].servername;

		if ( user[u].con_id > -1 )
			online = 1;
		else
			online = 0;

		activated = user[u].activated;

		ship_id = user[u].ship[user[u].ship_selected].ship_id;
		ship_type = user[u].ship[user[u].ship_selected].brand;
		ship_kind = user[u].ship[user[u].ship_selected].type;

		if ( ship_id > -1 && ship_type > -1 && ship_id < MAX_SERVER && ship_type < SHIP_MAX )
			ship_name = server[ship_id].ship[ship_type].name;
		else
			ship_name = not_found;

		class_points =  user[u].user_class_points;

		//prepare the string
		sprintf ( rtn_str,"28,%s,%s,%s,%d,%d,%d,%s,%s,%s,%d,%d,%d,%d,%d,%s,%d",
		          player_name,
		          guild_name,
		          guild_rank,
		          worth,
		          exp,
		          kills,
		          home_planet,
		          home_server,
		          last_server,
		          online,
		          activated,
		          ship_id,
		          ship_type,
		          ship_kind,
		          ship_name,
		          class_points
		        );

		g = user[u].guild_id;

		//add on the decorations
		if ( g >= 0 && g <= guild_max ) // must have a valid guild
			for ( i=0;i<MAX_DECO;i++ )
				if ( user[u].has_deco[i] && guild[g].deco_name[i][0] && guild[g].deco_activated[i] )
				{
					char cat_str[500];

					sprintf ( cat_str,",%d,%s", i, guild[g].deco_name[i] );
					strcat ( rtn_str, cat_str );
				}

	}
	else
		sprintf ( rtn_str,"28,Player Not Found" );

	send_con_user ( con,rtn_str );
}

void user_retrive_stats_list ( int con, int list_type )
{
	int i, k, z, list[10];
	char rtn_str[1000] = "29";
	char append_str[100];

	//clear the list
	for ( i=0;i<10;i++ )
		list[i] = -1;

	switch ( list_type )
	{
		case 1:
			//money
			//get the first one
			list[0] = 0;
			for ( k=0;k<=user_max;k++ )
				if ( user[k].money >= user[list[0]].money )
					list[0] = k;

			//collect the rest
			for ( i=1;i<10;i++ )
				for ( k=0;k<=user_max;k++ )
					if ( user[k].money <= user[list[i-1]].money )
					{
						if ( list[i] != -1 && user[k].money < user[list[i]].money ) continue;

						//now if we already have this fool
						for ( z=0;z<i;z++ )
							if ( k == list[z] )
								break;

						if ( z != i ) continue;

						//set it
						list[i] = k;
					}

			//prepare out message
			for ( i=0;i<10;i++ )
				if ( list[i] != -1 )
				{
					sprintf ( append_str,",%s,%d,%d", user[list[i]].username, user[list[i]].money, user[list[i]].user_class );
					strcat ( rtn_str,append_str );
				}

			//send
			send_con_user ( con,rtn_str );

			break;
		case 2:
			//exp
			//get the first one
			list[0] = 0;
			for ( k=0;k<=user_max;k++ )
				if ( user[k].exp >= user[list[0]].exp )
					list[0] = k;

			//collect the rest
			for ( i=1;i<10;i++ )
				for ( k=0;k<=user_max;k++ )
					if ( user[k].exp <= user[list[i-1]].exp )
					{
						if ( list[i] != -1 && user[k].exp < user[list[i]].exp ) continue;

						//now if we already have this fool
						for ( z=0;z<i;z++ )
							if ( k == list[z] )
								break;

						if ( z != i ) continue;

						//set it
						list[i] = k;
					}

			//prepare out message
			for ( i=0;i<10;i++ )
				if ( list[i] != -1 )
				{
					sprintf ( append_str,",%s,%d,%d", user[list[i]].username, user[list[i]].exp, user[list[i]].user_class );
					strcat ( rtn_str,append_str );
				}

			//send
			send_con_user ( con,rtn_str );
			break;
		case 3:
			//kills
			//get the first one
			list[0] = 0;
			for ( k=0;k<=user_max;k++ )
				if ( user[k].kills >= user[list[0]].kills )
					list[0] = k;

			//collect the rest
			for ( i=1;i<10;i++ )
				for ( k=0;k<=user_max;k++ )
					if ( user[k].kills <= user[list[i-1]].kills )
					{
						if ( list[i] != -1 && user[k].kills < user[list[i]].kills ) continue;

						//now if we already have this fool
						for ( z=0;z<i;z++ )
							if ( k == list[z] )
								break;

						if ( z != i ) continue;

						//set it
						list[i] = k;
					}

			//prepare out message
			for ( i=0;i<10;i++ )
				if ( list[i] != -1 )
				{
					sprintf ( append_str,",%s,%d,%d", user[list[i]].username, user[list[i]].kills, user[list[i]].user_class );
					strcat ( rtn_str,append_str );
				}

			//send
			send_con_user ( con,rtn_str );
			break;
	}
}

void user_update_home_info ( int u )
{
	char rtn_str[200];

	sprintf ( rtn_str,"7,%d,%d", user[u].home_id, user[u].home_planet );
	send_con_user ( user[u].con_id, rtn_str );
}

void user_add_kill ( int u, int the_class, int addition )
{
	int new_home;

	//simple add
	user[u].kills++;

	//dont give a shit
	if ( addition < 1 ) return;

	//now for something sickening
	if ( user[u].user_class != the_class )
	{
		//no problem
		user[u].user_class_points += addition;
	}
	else
	{
		//life hater.
		user[u].user_class_points -= addition;

		//oh so your a traitor now?
		if ( user[u].user_class_points < 0 )
		{
			//toggle hell
			if ( user[u].user_class == 1 )
				user[u].user_class = 2;
			else
				user[u].user_class = 1;


			//set a NEW home planet
			if ( ( new_home = server_random_home_planet ( user[u].home_id,user[u].user_class ) ) != -1 )
			{
				user[u].home_planet = new_home;

				//now tell everyone who needs to know
				user_update_home_info ( u );
			}
		}
	}
}

void send_bounty_list ( int con )
{
	int i;
	char temp_str[1024] = "30,";

	for ( i=0;i<MAX_BOUNTY;i++ )
		if ( bounty[i].user_id >= 0 )
		{
			char ya_str[50];

			sprintf ( ya_str,"%s,%d,", user[bounty[i].user_id].username, bounty[i].price );
			strcat ( temp_str,ya_str );
		}

	send_con_user ( con, temp_str );
}

void user_add_bounty ( int con, char *victom_name, int price )
{
	int u, v_u, b_u;
	char rtn_str[300];

	u = user_con_id[con];
	v_u = user_to_id ( victom_name );

	if(user_check_lease_loggedin(u)) return;
	
	//stupid check
	if ( price <= 0 ) return;

	//return user not found
	if ( v_u == -1 )
	{
		sprintf ( rtn_str,"2,User Not Found" );
		send_con_user ( con,rtn_str );
		return;
	}
		
	//return not enough money.
	if ( (user[u].money + user[u].guild_balance) - price < MIN_MONEY )
	{
		sprintf ( rtn_str,"2,Funds Needed" );
		send_con_user ( con,rtn_str );
		return;
	}

	//find if already on list
	b_u = user_to_bounty ( v_u );

	//if on list, add to ammount
	if ( b_u != -1 )
	{
		append_bounty ( b_u,price );

		sprintf ( rtn_str,"2,Funds Transfered" );
		send_con_user ( con,rtn_str );

		//tell player new bounty list
		send_bounty_list ( con );

		//money stuff
		user[u].money -= price;
		user_send_money ( con );
	}
	else //if not on list, then create
	{
		if ( create_bounty ( v_u,price ) )
		{
			//tell them the deal was good
			sprintf ( rtn_str,"2,Bounty Added" );
			send_con_user ( con,rtn_str );

			//tell player new bounty list
			send_bounty_list ( con );

			//money stuff
			user[u].money -= price;
			user_send_money ( con );
		}
		else
		{
			//tell them the deal was bad
			sprintf ( rtn_str,"2,Too Many Bounties" );
			send_con_user ( con,rtn_str );
		}
	}
}

void append_bounty ( int i, int price )
{
	bounty[i].price += price;

	//should we increase the lifetime?
	if ( price >= MIN_BOUNTY_LIFETIME_INC_PRICE )
	{
		bounty[i].lifetime += MAX_BOUNTY_LIFETIME_INC;

		//over the limit?
		if ( bounty[i].lifetime > MAX_BOUNTY_LIFETIME )
			bounty[i].lifetime = MAX_BOUNTY_LIFETIME;
	}

	game.rewrite_bounty_db = 1;
}

int create_bounty ( int u, int price )
{
	int i;

	i = free_bounty();

	if ( i == -1 ) return 0;

	bounty[i].user_id = u;
	bounty[i].price = price;
	bounty[i].lifetime = MAX_BOUNTY_LIFETIME;

	game.rewrite_bounty_db = 1;

	return 1;
}

void reduce_bounty_lifetime()
{
	int i;

	for ( i=0;i<MAX_BOUNTY;i++ )
		if ( bounty[i].user_id >= 0 )
		{
			bounty[i].lifetime--;

			if ( bounty[i].lifetime <= 0 )
				remove_bounty ( i );
		}

	game.rewrite_bounty_db = 1;
}

void remove_bounty ( int i )
{
	bounty[i].user_id = -1;
	bounty[i].price = 0;
	bounty[i].lifetime = 0;

	game.rewrite_bounty_db = 1;
}

int free_bounty()
{
	int i;

	for ( i=0;i<MAX_BOUNTY;i++ )
		if ( bounty[i].user_id == -1 ) break;

	if ( i==MAX_BOUNTY ) return -1;

	return i;
}

int user_to_bounty ( int userid )
{
	int i;

	if ( userid == -1 ) return -1;

	for ( i=0;i<MAX_BOUNTY;i++ )
		if ( bounty[i].user_id == userid ) break;

	if ( i==MAX_BOUNTY ) return -1;

	return i;
}

void reward_kill_reward ( int u, int v_u )
{
	char rtn_str[500];
	int the_amount;

	if ( u<0 ) return;
	if ( v_u<0 ) return;

	//what is their real money value
	the_amount = user[v_u].money + user[v_u].guild_balance;

	//do they really own enough?
	if ( the_amount < MIN_MONEY )
		return;

	//if they have money in the bank, do not make it apart of what they lose
	if ( user[v_u].guild_balance > 0 )
		the_amount = user[v_u].money;

	the_amount = ( int ) ( the_amount * KILL_REWARD );

	//ok does this move put the dier under min?
	if ( user[v_u].money - the_amount < MIN_MONEY )
		the_amount = user[v_u].money - MIN_MONEY;

	//if so set to only what they have available

	//is the_amount 0?
	if ( the_amount <= 0 )
		return;

	//do the exchange
	user[v_u].money -= the_amount;
	user[u].money += the_amount;

	//make this string...
	sprintf ( rtn_str,"... Kill Reward: %d ..", the_amount );

	//tell em all
	user_send_money ( user[u].con_id );
	user_send_money ( user[v_u].con_id );
	send_user_public_chat ( user[u].con_id,1,rtn_str );

	//tell the dier
	sprintf ( rtn_str,"... %d DC lost to %s from a kill reward ..", the_amount, user[u].username );
	send_user_private_chat ( user[v_u].con_id,1,rtn_str );

	//database
	game.rewrite_user_db = 1;
}

void reward_bounty ( int b_u, int u, int max_amount )
{
	char rtn_str[500];
	int the_amount;

	//checks
	if ( b_u < 0 ) return;
	if ( b_u >= MAX_BOUNTY ) return;
	if ( u < 0 ) return;
	if ( u >= MAX_USER ) return;
	if ( bounty[b_u].user_id < 0 || !bounty[b_u].lifetime || !bounty[b_u].price ) return;

	the_amount = bounty[b_u].price;

	if ( the_amount > max_amount )
		the_amount = max_amount;

	//make this string...
	sprintf ( rtn_str,"... Bounty Reward: %d ..", the_amount );

	//give the money
	user[u].money += the_amount;

	bounty[b_u].price -= the_amount;

	//kill the bounty
	if ( bounty[b_u].price <= 0 )
		remove_bounty ( b_u );
	else
		game.rewrite_bounty_db = 1;

	//tell the world
	user_send_money ( user[u].con_id );
	send_user_public_chat ( user[u].con_id,1,rtn_str );
}

void send_user_public_chat ( int con, int color, char *message )
{
	char temp_str[1025];

	if ( con < 0 ) return;

	sprintf ( temp_str,"31,%d,%s", color, message );
	send_con_user ( con, temp_str );
}

void send_user_private_chat ( int con, int color, char *message )
{
	char temp_str[1025];

	if ( con < 0 ) return;

	sprintf ( temp_str,"32,%d,%s", color, message );
	send_con_user ( con, temp_str );
}

int guild_worth ( int i )
{
	int u, collected = 0;

	for ( u=0;u<=user_max;u++ )
		if ( user[u].guild_id == i )
			collected += user_worth ( u );

	return collected;
}

int guild_players ( int i )
{
	int u, collected = 0;

	for ( u=0;u<=user_max;u++ )
		if ( user[u].guild_id == i )
			collected++;

	return collected;
}

void user_transfer_credits ( int con, char *to_name, int amount )
{
	int u, to_u, to_con;
	char rtn_str[500];

	//bullshit?
	if ( amount < 1 ) return;

	u = user_con_id[con];
	to_u = user_to_id ( to_name );

	if(user_check_lease_loggedin(u)) return;
	
	//sex?
	if ( u==to_u ) return;

	//our friend exist?
	if ( to_u == -1 )
	{
		sprintf ( rtn_str,"... Player %s Not Found ..",to_name );
		//tell um
		send_user_public_chat ( con,1,rtn_str );
		return;
	}

	//got the money?
	if ( user[u].money - amount < MIN_MONEY )
	{
		int c_avail;

		c_avail = user[u].money - MIN_MONEY;

		if ( c_avail < 0 ) c_avail = 0;

		sprintf ( rtn_str,"... Credits Available For Transfer: %d ..", c_avail );
		send_user_public_chat ( con,1,rtn_str );
		return;
	}

	//owe the bank?
	if ( user[u].guild_balance < 0 )
	{
		send_user_public_chat ( con,1,"... Please Remove Guild Bank Debt .." );
		send_user_public_chat ( con,1,rtn_str );
		return;
	}

	//well then...
	to_con = user[to_u].con_id;

	//make the transfer
	user[u].money -= amount;
	user[to_u].money += amount;

	//tells the folks
	if ( to_con > -1 )
	{
		sprintf ( rtn_str,"... %s Has Transferred %d Credits to Your Account  ..", user[u].username, amount );
		send_user_private_chat ( to_con,3,rtn_str );

		user_send_money ( to_con );
	}

	if ( amount > 1 )
		sprintf ( rtn_str,"... %d Credits Have Been Transferred to %s  ..", amount, user[to_u].username );
	else
		sprintf ( rtn_str,"... 1 Credit Has Been Transferred to %s  ..", user[to_u].username );
	send_user_private_chat ( con,3,rtn_str );

	user_send_money ( con );

	//the db should know
	game.rewrite_user_db = 1;
}

void guild_enlist_user ( int con, char *victom_name )
{
	int u, g, r, is_owner, v_u, v_g, v_r, v_is_owner, v_con;
	char rtn_str[500];

	//find all that is good
	if ( ( v_u = user_to_id ( victom_name ) ) == -1 ) return;
	if ( ( v_con = user[v_u].con_id ) == -1 ) return;
	v_g = user[v_u].guild_id;
	v_r = user[v_u].guild_rank;
	v_is_owner = !strcmp ( user[v_u].username,guild[v_g].owner );

	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;
	is_owner = !strcmp ( user[u].username,guild[g].owner );

	if(user_check_lease_loggedin(u)) return;
	
	//do the checking needed to know
	if ( u==v_u ) return; //cant do this shit to yourself
	if ( v_g == g ) return;
	if ( !is_owner && !guild[g].rank_allow[r][0] ) return;

	//give the request
	user[v_u].guild_request = g;

	//tell the people
	sprintf ( rtn_str,"... %s Has Requested You to Join %s, type :gaccept::%s to Join ..", user[u].username, guild[g].name, guild[g].name );
	send_user_private_chat ( v_con,3,rtn_str );
	sprintf ( rtn_str,"... Guild Request Has Been Sent to %s ..", user[v_u].username );
	send_user_private_chat ( con,3,rtn_str );

	//db
	game.rewrite_user_db = 1;
}

void guild_remove_user ( int con, char *victom_name )
{
	int u, g, r, is_owner, v_u, v_g, v_r, v_is_owner, v_con;
	char rtn_str[500];
	int i;

	//find all that is good
	if ( ( v_u = user_to_id ( victom_name ) ) == -1 ) return;
	v_con = user[v_u].con_id;
	v_g = user[v_u].guild_id;
	v_r = user[v_u].guild_rank;
	v_is_owner = !strcmp ( user[v_u].username,guild[v_g].owner );

	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;
	is_owner = !strcmp ( user[u].username,guild[g].owner );

	if(user_check_lease_loggedin(u)) return;
	
	if ( u==v_u ) return; //cant do this shit to yourself
	if ( v_g != g ) return; //now same guild
	if ( !is_owner && !guild[g].rank_allow[r][1] ) return;
	if ( v_r>=r && !is_owner ) return;
	if ( v_is_owner ) return;

	//remove!
	user_leave_guild ( v_u );

	if ( v_con > -1 )
	{
		//tell folks
		send_user_name_guild ( v_u );
		user_send_guild_info ( v_con );
		user_send_guild_balance ( v_con );

		//tell the server
		server_update_user_guild ( user[v_u].server_id,v_u );

		//actaully message the player
		sprintf ( rtn_str,"... You Have Been Removed From %s ..", guild[g].name );
		send_user_private_chat ( v_con,3,rtn_str );
	}

	//and the guild
	sprintf ( rtn_str,"... %s Has Been Removed From %s By %s ..", user[v_u].username, guild[g].name, user[u].username );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].guild_id == g && i != v_u )
			send_user_private_chat ( user[i].con_id,3,rtn_str );

	//db
	game.rewrite_user_db = 1;
}

void guild_promote_user ( int con, char *victom_name )
{
	int u, g, r, is_owner, v_u, v_g, v_r, v_is_owner, v_con;
	char rtn_str[500];

	//find all that is good
	if ( ( v_u = user_to_id ( victom_name ) ) == -1 ) return;
	v_con = user[v_u].con_id;
	v_g = user[v_u].guild_id;
	v_r = user[v_u].guild_rank;
	v_is_owner = !strcmp ( user[v_u].username,guild[v_g].owner );

	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;
	is_owner = !strcmp ( user[u].username,guild[g].owner );

	if(user_check_lease_loggedin(u)) return;
	
	if ( u==v_u ) return; //cant do this shit to yourself
	if ( v_g != g ) return; //now same guild
	if ( !is_owner && !guild[g].rank_allow[r][1] ) return;
	if ( v_r>=r ) return;
	if ( v_r>=5 ) return;

	//do the reetu
	user[v_u].guild_rank++;

	//bogus check...
	if ( user[v_u].guild_rank > 5 ) user[v_u].guild_rank = 5;

	if ( v_con > -1 )
	{
		//tell the people!
		send_user_name_guild ( v_u );
		user_send_guild_info ( v_con );

		//tell the server
		server_update_user_guild ( user[v_u].server_id,v_u );

		//actaully message the player
		sprintf ( rtn_str,"... You Have Been Promoted to %s ..", guild[g].rank_name[user[v_u].guild_rank] );
		send_user_private_chat ( v_con,3,rtn_str );
	}

	//and the guild
	sprintf ( rtn_str,"... %s Has Been Promoted to %s ..", user[v_u].username, guild[g].rank_name[user[v_u].guild_rank] );
	send_user_private_chat ( con,3,rtn_str );

	//db
	game.rewrite_user_db = 1;
}

void guild_demote_user ( int con, char *victom_name )
{
	int u, g, r, is_owner, v_u, v_g, v_r, v_is_owner, v_con;
	char rtn_str[500];

	//find all that is good
	if ( ( v_u = user_to_id ( victom_name ) ) == -1 ) return;
	v_con = user[v_u].con_id;
	v_g = user[v_u].guild_id;
	v_r = user[v_u].guild_rank;
	v_is_owner = !strcmp ( user[v_u].username,guild[v_g].owner );

	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;
	is_owner = !strcmp ( user[u].username,guild[g].owner );

	if(user_check_lease_loggedin(u)) return;
	
	if ( u==v_u ) return; //cant do this shit to yourself
	if ( v_g != g ) return; //now same guild
	if ( !is_owner && !guild[g].rank_allow[r][1] ) return;
	if ( v_r>=r && !is_owner ) return;
	if ( v_r<=0 ) return;
	if ( v_is_owner ) return;

	//demote the mother
	user[v_u].guild_rank--;

	if ( v_con >-1 )
	{
		//tell the people!
		send_user_name_guild ( v_u );
		user_send_guild_info ( v_con );

		//tell the server
		server_update_user_guild ( user[v_u].server_id,v_u );

		//actaully message the player
		sprintf ( rtn_str,"... You Have Been Demoted to %s ..", guild[g].rank_name[user[v_u].guild_rank] );
		send_user_private_chat ( v_con,3,rtn_str );
	}

	//and the guild
	sprintf ( rtn_str,"... %s Has Been Demoted to %s ..", user[v_u].username, guild[g].rank_name[user[v_u].guild_rank] );
	send_user_private_chat ( con,3,rtn_str );

	//db
	game.rewrite_user_db = 1;
}

int guild_rejoin_owner ( int u, char *guildname )
{
	int g;
	int con;
	int i;
	char lcase_user[21];
	char lcase_owner[21];

	//u good?
	if ( u<0 ) return 0;
	if ( u>user_max ) return 0;

	//set this
	con = user[u].con_id;

	//cap this
	guildname[20] = 0;

	g = guild_name_to_id ( guildname );

	//not even found
	if ( g==-1 ) return 0;

	//lcase for compare
	lcase ( lcase_user, user[u].username );
	lcase ( lcase_owner, guild[g].owner );

	//check
	if ( strcmp ( lcase_user, lcase_owner ) ) return 0;

	//well set then

	//leave guild
	user_leave_guild ( u );

	//enter new
	user[u].guild_id = g;
	user[u].guild_rank = 5;

	if ( con > -1 )
	{
		char rtn_str[1025];

		//tell folks
		send_user_name_guild ( u );
		user_send_guild_info ( con );
		user_send_guild_balance ( con );

		//tell the server
		server_update_user_guild ( user[u].server_id, u );

		//actaully message the player
		sprintf ( rtn_str,"... Welcome Back Your Lordship .." );
		send_user_private_chat ( con,3,rtn_str );

		//and the guild
		sprintf ( rtn_str,"... %s Has Returned to the Guild ..", user[u].username );
		for ( i=0;i<=user_max;i++ )
			if ( user[i].con_id > -1 && user[i].guild_id == user[u].guild_id && i != u )
				send_user_private_chat ( user[i].con_id,3,rtn_str );
	}

	//db
	game.rewrite_user_db = 1;

	return 1;
}

void guild_user_accept ( int con, char *guildname )
{
	int u, g, s, i;
	char lcase_guildname[1025], lcase_new_guildname[21];
	char rtn_str[1025];

	//cap!
	guildname[20] = 0;

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;
	s = user[u].server_id;

	if(user_check_lease_loggedin(u)) return;
	
	if(s<0) return;
	
	//this guy rejoining his old guild?
	if ( guild_rejoin_owner ( u, guildname ) ) return;

	//tired... checks
	if ( user[u].guild_request < 0 ) return;
	if ( user[u].guild_request==g ) return;

	//check if we named it right
	lcase ( lcase_guildname, guildname );
	lcase ( lcase_new_guildname, guild[user[u].guild_request].name );
	if ( strcmp ( lcase_new_guildname,lcase_guildname ) ) return;

	//well okay!

	//leave guild
	user_leave_guild ( u );

	//enter new
	user[u].guild_id = user[u].guild_request;
	user[u].guild_rank = 0;

	//clear this
	user[u].guild_request = 0;

	//tell folks
	send_user_name_guild ( u );
	user_send_guild_info ( con );
	user_send_guild_balance ( con );

	//tell the server
	server_update_user_guild ( user[u].server_id,u );

	//actaully message the player
	sprintf ( rtn_str,"... Welcome to the %s Guild ..",guild[user[u].guild_id].name );
	send_user_private_chat ( con,3,rtn_str );

	//and the guild
	sprintf ( rtn_str,"... %s Has Joined the Guild ..",user[u].username );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].guild_id == user[u].guild_id && i != u )
			send_user_private_chat ( user[i].con_id,3,rtn_str );

	//db
	game.rewrite_user_db = 1;
}

void user_uni_chat ( int con, char *message )
{
	int u, g, i;
	char rtn_str[1225];

	//set stuff
	u = user_con_id[con];
	user[u].uni_chat_on = 1;

	if ( flood_check ( u ) )
		return;

	sprintf ( rtn_str,"31,2,Universal Chat : %s : : %s%c", user[u].username, message, 13 );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].uni_chat_on )
			raw_send_con_user ( user[i].con_id,rtn_str );
}

void user_guild_chat ( int con, char *message )
{
	int u, g, i;
	char rtn_str[1225];

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;

	if ( flood_check ( u ) )
		return;

	sprintf ( rtn_str,"32,3,Guild Chat : %s : : %s%c", user[u].username, message, 13 );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].guild_id == g )
			raw_send_con_user ( user[i].con_id,rtn_str );
}

void user_return_online_guild_list ( int con )
{
	int u, g, i;
	char rtn_str[1225], *append_comma = ",";

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;

	sprintf ( rtn_str,"32,3,Guild Online List : " );
	for ( i=0;i<=user_con_top;i++ )
	{
		int v_u;

		v_u = user_con_id[i];

		if ( v_u > -1 && user[v_u].guild_id == g )
		{
			strcat ( rtn_str, user[v_u].username );
			strcat ( rtn_str, append_comma );
		}
	}

	send_con_user ( con, rtn_str );
}

void user_return_guild_list ( int con )
{
	int u, g, i;
	char rtn_str[1225], *append_comma = ",";

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;

	sprintf ( rtn_str,"32,3,Guild List : " );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].guild_id == g )
		{
            //size check
            if(strlen(rtn_str) + strlen(user[i].username) + 2 > 1224) break;
            
			strcat ( rtn_str, user[i].username );
			strcat ( rtn_str, append_comma );
		}

	send_con_user ( con, rtn_str );
}

int user_award_guild_reward ( int u, char *the_guild )
{
	int i, g;
	char temp_lcase[21], lcase_guild[21];

	g = user[u].guild_id;

	//simple check
	if ( g < 0 ) return 0;

	//lcase the guild
	lcase ( lcase_guild, the_guild );

	//check the list from the top
	for ( i=2;i>=0;i-- )
	{
		lcase ( temp_lcase, guild[g].enemy[i] );

		if ( str_match ( temp_lcase, lcase_guild ) )
		{
			int reward_amt = 0;
			char rtn_str[200];
			//reward

			switch ( i )
			{
				case 0:
					reward_amt = 2000;
					break;
				case 1:
					reward_amt = 3000;
					break;
				case 2:
					reward_amt = 5000;
					break;
			}

			//make the change
			user[u].money += reward_amt;

			//guild deserves some too
			pay_guild ( g, reward_amt / 10 );

			//tell the fool
			sprintf ( rtn_str,"... Guild Reward : %d ..",reward_amt );
			send_user_public_chat ( user[u].con_id, 1, rtn_str );

			//backend tell
			user_send_money ( user[u].con_id );

			//db!
			game.rewrite_guild_db = 1;
			game.rewrite_user_db = 1;

			return 1;
		}
	}

	return 0;
}

void user_purchase_upgrade_sat ( int con )
{
	int u, g, s, p;

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;
	s = user[u].server_id;
	p = user[u].last_planet;

	if(user_check_lease_loggedin(u)) return;
	
	//the dumb checks
	if ( g<0 ) return;
	if ( s<0 ) return;
	if ( p<0 ) return;
	if ( g>=MAX_GUILD ) return;
	if ( s>=MAX_SERVER ) return;
	if ( p>=PLANET_MAX ) return;
	if ( !server[s].planet[p].name[0] ) return;  //planet exist?
	if ( !guild[g].name[0] ) return;  //guild exist?

	//checks
	if ( !user[u].landed ) return;
	if ( user[u].guild_balance < 0 ) return;

	//is this a purchase or upgrade?
	if ( server[s].planet[p].sat.guild_id < 0 )
	{
		int i;

		//got money?
		if ( user[u].money - MIN_MONEY < SAT_COST_INC ) return;

		//make the purchase
		server[s].planet[p].sat.guild_id = g;
		server[s].planet[p].sat.level = 1;
		server[s].planet[p].sat.hull = SAT_HULL_INC;
		server[s].planet[p].sat.money = 50000;
		server[s].planet[p].sat.money_collected = 0;

		//clear the weapons
		for ( i=0;i<SAT_LEVEL_MAX * SAT_GUN_INC;i++ )
		{
			server[s].planet[p].sat.gun[i] = -1;
			server[s].planet[p].sat.gun_destroyed[i] = -1;
		}
		for ( i=0;i<SAT_LEVEL_MAX * SAT_MISSILE_INC;i++ )
		{
			server[s].planet[p].sat.missile[i] = -1;
			server[s].planet[p].sat.missile_left[i] = 0;
		}

		//now tell the server and all users that need to know
		server_send_sat_info ( s, p );

		//now for the user purchase stuff
		user[u].money -= SAT_COST_INC;

		user_send_money ( con );

		//set rewrite db flag
		server[s].rewrite_sat_db = 1;

		//let the cool site know
		mysql_update_sat_entry ( s,p );
	}
	else //upgrade
	{
		if ( !user_can_edit_sat ( u, g, s, p ) ) return;
		//server already too large
		if ( server[s].planet[p].sat.level >= SAT_LEVEL_MAX ) return;
		//got money?
		if ( user[u].money - MIN_MONEY < SAT_COST_INC ) return;

		//make the upgrade
		server[s].planet[p].sat.level++;
		server[s].planet[p].sat.hull = SAT_HULL_INC * server[s].planet[p].sat.level;
		server[s].planet[p].sat.money += 50000;

		//now tell the server and all users that need to know
		server_send_sat_info ( s, p );

		//now for the user purchase stuff
		user[u].money -= SAT_COST_INC;

		user_send_money ( con );

		//set rewrite db flag
		server[s].rewrite_sat_db = 1;

		//let the cool site know
		mysql_update_sat_entry ( s,p );
	}
}

void user_deposit_sat_money ( int con, int amount )
{
	int u, g, s, p;
	int old_money;
	int bank_max;
	int bank_difference;

	//simple stupid check
	if ( amount <= 0 ) return;

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;
	s = user[u].server_id;
	p = user[u].last_planet;

	if(user_check_lease_loggedin(u)) return;
	
	//the dumb checks
	if ( g<0 ) return;
	if ( s<0 ) return;
	if ( p<0 ) return;
	if ( g>=MAX_GUILD ) return;
	if ( s>=MAX_SERVER ) return;
	if ( p>=PLANET_MAX ) return;
	if ( !server[s].planet[p].name[0] ) return;  //planet exist?
	if ( !guild[g].name[0] ) return;  //guild exist?

	//already at the max?
	bank_max = server[s].planet[p].sat.level * SAT_MAX_BANK_INC;
	if ( bank_max <= server[s].planet[p].sat.money ) return;

	//redo amount?
	bank_difference = bank_max - server[s].planet[p].sat.money;
	if ( bank_difference < amount )
		amount = bank_difference;


	//checks
	if ( !user[u].landed ) return;
	if ( !user_can_edit_sat ( u, g, s, p ) );
	if ( user[u].money - MIN_MONEY < amount ) return;
	if (user[u].guild_balance < 0 ) return;

	//set it
	old_money = server[s].planet[p].sat.money;

	//do the deposit
	server[s].planet[p].sat.money += amount;

	//now tell the server and all users that need to know
	server_send_sat_info ( s, p );
	
	//tell all the users who repaired the sat
	server_send_sat_deposit_info ( s, p, user[u].username, amount );

	//scar the user
	user[u].money -= amount;

	user_send_money ( con );

	//set rewrite db flag
	server[s].rewrite_sat_db = 1;

	//let the cool site know
	if ( !old_money )
		mysql_update_sat_entry ( s,p );
}

void user_buy_sell_sat_weapon ( int con, int is_missile, int i, int w )
{
	int u, g, s, p, r;
	int wep_max;

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;
	r = user[u].guild_rank;
	s = user[u].server_id;
	p = user[u].last_planet;

	if(user_check_lease_loggedin(u)) return;
	
	//the dumb checks
	if ( g<0 ) return;
	if ( s<0 ) return;
	if ( p<0 ) return;
	if ( g>=MAX_GUILD ) return;
	if ( s>=MAX_SERVER ) return;
	if ( p>=PLANET_MAX ) return;
	if ( !server[s].planet[p].name[0] ) return;  //planet exist?
	if ( !guild[g].name[0] ) return;  //guild exist?
	if ( r<0 ) return;
	if ( r>5 ) return;

	//checks
	if ( !user[u].landed ) return;
	if ( !user_can_edit_sat ( u, g, s, p ) );

	//ok missile or gun
	if ( !is_missile ) //is gun
	{
		wep_max = server[s].planet[p].sat.level * SAT_GUN_INC;

		//i ok?
		if ( i < 0 ) return;
		if ( i >= wep_max ) return;

		//purchase or sell?
		if ( server[s].planet[p].sat.gun[i] > -1 ) //sell
		{
			//we good for this?
			if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

			w = server[s].planet[p].sat.gun[i];

			//clear this weapon slot
			server[s].planet[p].sat.gun[i] = -1;
			server[s].planet[p].sat.gun_destroyed[i] = 0;

			//do a tiny check or two
			if ( w >= GUN_MAX ) return;
			if ( !server[s].gun[w].name[0] ) return;

			//give the sat the money
			server[s].planet[p].sat.money += ( int ) ( server[s].gun[w].cost * 0.75 );

			//cap it
			regulate_sat_bank_max(s,p);
			
			//now tell the server and all users that need to know
			server_send_sat_info ( s, p );

			//set rewrite db flag
			server[s].rewrite_sat_db = 1;
		}
		else // purchase
		{
			//check that w
			if ( w < 0 ) return;
			if ( w >= GUN_MAX ) return;

			//this gun exist, and does the planet have it
			if ( !server[s].gun[w].name[0] ) return;
			if ( !server[s].planet[p].gun[w] ) return;

			//got money?
			if ( user[u].money - MIN_MONEY < server[s].gun[w].cost ) return;
			if ( user[u].guild_balance < 0 ) return;

			//make the purchase
			server[s].planet[p].sat.gun[i] = w;
			server[s].planet[p].sat.gun_destroyed[i] = 0;

			//now tell the server and all users that need to know
			server_send_sat_info ( s, p );

			//make user suffer
			user[u].money -= server[s].gun[w].cost;

			user_send_money ( con );

			//set rewrite db flag
			server[s].rewrite_sat_db = 1;
		}
	}
	else //missile
	{
		wep_max = server[s].planet[p].sat.level * SAT_MISSILE_INC;

		//i ok?
		if ( i < 0 ) return;
		if ( i >= wep_max ) return;

		//purchase or sell?
		if ( server[s].planet[p].sat.missile[i] > -1 ) //sell
		{
			//we good for this?
			if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

			w = server[s].planet[p].sat.missile[i];

			//clear this weapon slot
			server[s].planet[p].sat.missile[i] = -1;
			server[s].planet[p].sat.missile_left[i] = 0;

			//do a tiny check or two
			if ( w >= MISSILE_MAX ) return;
			if ( !server[s].missile[w].name[0] ) return;

			//give the sat the money
			server[s].planet[p].sat.money += ( int ) ( server[s].missile[w].cost * 0.75 );

			//cap it
			regulate_sat_bank_max(s,p);
			
			//now tell the server and all users that need to know
			server_send_sat_info ( s, p );

			//set rewrite db flag
			server[s].rewrite_sat_db = 1;
		}
		else // purchase
		{
			//check that w
			if ( w < 0 ) return;
			if ( w >= MISSILE_MAX ) return;

			//this gun exist, and does the planet have it
			if ( !server[s].missile[w].name[0] ) return;
			if ( !server[s].planet[p].missile[w] ) return;

			//got money?
			if ( user[u].money - MIN_MONEY < server[s].missile[w].cost ) return;
			if ( user[u].guild_balance < 0 ) return;

			//make the purchase
			server[s].planet[p].sat.missile[i] = w;
			server[s].planet[p].sat.missile_left[i] = 4;

			//now tell the server and all users that need to know
			server_send_sat_info ( s, p );

			//make user suffer
			user[u].money -= server[s].missile[w].cost;

			user_send_money ( con );

			//set rewrite db flag
			server[s].rewrite_sat_db = 1;
		}
	}
}

void user_repair_reload_sat_weapon ( int con, int is_missile, int i )
{
	int u, g, s, p;
	int wep_max, w;

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;
	s = user[u].server_id;
	p = user[u].last_planet;

	if(user_check_lease_loggedin(u)) return;
	
	//the dumb checks
	if ( g<0 ) return;
	if ( s<0 ) return;
	if ( p<0 ) return;
	if ( g>=MAX_GUILD ) return;
	if ( s>=MAX_SERVER ) return;
	if ( p>=PLANET_MAX ) return;
	if ( !server[s].planet[p].name[0] ) return;  //planet exist?
	if ( !guild[g].name[0] ) return;  //guild exist?

	//checks
	if ( !user[u].landed ) return;
	if ( !user_can_edit_sat ( u, g, s, p ) );
	if ( user[u].guild_balance < 0 ) return;

	//ok missile or gun
	if ( is_missile )
	{
		int max_reload, cost_per_missile, k;

		wep_max = server[s].planet[p].sat.level * SAT_MISSILE_INC;

		//i ok?
		if ( i < 0 ) return;
		if ( i >= wep_max ) return;

		//w?
		w = server[s].planet[p].sat.missile[i];
		if ( w < 0 ) return;
		if ( w >= MISSILE_MAX ) return;

		//dumb question but does the missile exist, and this planet have it?
		if ( !server[s].missile[w].name[0] ) return;
		if ( !server[s].planet[p].missile[w] ) return;

		//this weapon busted?
		if ( server[s].planet[p].sat.missile_left[i] >= 4 ) return;

		//now for the reloading process

		//set these
		cost_per_missile = server[s].missile[w].cost / 4;
		max_reload = 4 - server[s].planet[p].sat.missile_left[i];

		for ( k=0;k<max_reload;k++ )
			if ( user[u].money - MIN_MONEY >= cost_per_missile )
			{
				//do a reload
				server[s].planet[p].sat.missile_left[i]++;

				//scar user
				user[u].money -= cost_per_missile;
			}

		//stupid check
		if ( server[s].planet[p].sat.missile_left[i] > 4 ) server[s].planet[p].sat.missile_left[i] = 4;

		//now tell the server and all users that need to know
		server_send_sat_info ( s, p );

		//ya
		user_send_money ( con );
	}
	else
	{
		wep_max = server[s].planet[p].sat.level * SAT_GUN_INC;

		//i ok?
		if ( i < 0 ) return;
		if ( i >= wep_max ) return;

		//w?
		w = server[s].planet[p].sat.gun[i];
		if ( w < 0 ) return;
		if ( w >= GUN_MAX ) return;

		//dumb question but does the gun exist, and this planet have it?
		if ( !server[s].gun[w].name[0] ) return;
		if ( !server[s].planet[p].gun[w] ) return;

		//this weapon busted?
		if ( !server[s].planet[p].sat.gun_destroyed[i] ) return;

		//got the money?
		if ( user[u].money - MIN_MONEY < server[s].gun[w].cost * 0.1 ) return;

		//make the repair
		server[s].planet[p].sat.gun_destroyed[i] = 0;

		//now tell the server and all users that need to know
		server_send_sat_info ( s, p );

		//scar user
		user[u].money -= ( int ) ( server[s].gun[w].cost * 0.1 );

		//ya
		user_send_money ( con );
	}
}

int user_can_edit_sat ( int u, int g, int s, int p )
{
	//planet have a guild?
	if ( server[s].planet[p].sat.guild_id < 0 ) return 0;

	//same guild?
	if ( server[s].planet[p].sat.guild_id != g ) return 0;

	return 1;
}

int is_game_reserved_name ( char *name )
{
	char l_name[21];

	name[20] = 0; //cap

	lcase ( l_name, name );

	if ( str_match ( l_name, "satellite" ) ) return 1;

	if ( is_game_bot_name ( name ) ) return 1;

	return 0;
}

int is_game_bot_name ( char *name )
{
	int s, b_t, b_u;
	char l_name[21], l_bot[21];

	name[20] = 0; //cap

	lcase ( l_name, name );

	for ( s=0;s<MAX_SERVER;s++ )
		if ( server[s].servername[0] )
			for ( b_t=0;b_t<BOT_TYPE_MAX;b_t++ )
				if ( server[s].bot_type[b_t].name[0] )
					for ( b_u=0;b_u<BOT_MAX;b_u++ )
						if ( server[s].bot_type[b_t].bot[b_u].name[0] )
						{
							lcase ( l_bot, server[s].bot_type[b_t].bot[b_u].name );

							if ( str_match ( l_bot, l_name ) ) return 1;
						}

	return 0;
}

void user_enter_planet_guild ( int con, int check_p )
{
	int u, g_new;
	int s;
	char rtn_str[1000];
	int i;

	u = user_con_id[con];
	s = user[u].server_id;

	if(user_check_lease_loggedin(u)) return;
	
	if(s<0) return;
	
	//user landed?
	if ( !user[u].landed ) return;

	//check planets
	if ( user[u].last_planet != check_p ) return;

	//this planet got a guild?
	if ( !server[s].planet[check_p].guild[0] ) return;

	//this hub got the guild?
	g_new = guild_name_to_id ( server[s].planet[check_p].guild );

	if ( g_new == -1 ) return;

	//already in this guild?
	if ( g_new == user[u].guild_id ) return;

	//got the money?
	if ( user[u].money < 15000 ) return;

	//lose some money
	user[u].money -= 15000;

	//leave guild
	user_leave_guild ( u );

	//enter new
	user[u].guild_id = g_new;
	user[u].guild_rank = 0;

	//tell folks
	send_user_name_guild ( u );
	user_send_guild_info ( con );
	user_send_guild_balance ( con );

	//tell the server
	server_update_user_guild ( user[u].server_id,u );

	//and the guild
	sprintf ( rtn_str,"... %s Has Joined the Guild ..",user[u].username );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].guild_id == user[u].guild_id && i != u )
			send_user_private_chat ( user[i].con_id,3,rtn_str );

	//what is our money hmm
	user_send_money ( con );
}

void sell_all_user_goods ( int u )
{
	int i;

	for ( i=0;i<10;i++ )
		sell_all_user_ship_goods ( u,i );
}

void clear_all_user_ship_goods ( int u, int sh )
{
	int i, con;

	con = user[u].con_id;

	if ( sh == user[u].ship_selected && con > -1 )
	{
		for ( i=0;i<10;i++ )
		{
			if ( user[u].ship[sh].cargo[i] ) //send!
			{
				user[u].ship[sh].cargo[i] = 0;
				user_update_good_ammount ( con,i );
			}

			if ( user[u].ship[sh].contra[i] ) //send!
			{
				user[u].ship[sh].contra[i] = 0;
				user_update_contra_ammount ( con,i );
			}
		}
	}
	else
	{
		for ( i=0;i<10;i++ )
		{
			user[u].ship[sh].cargo[i] = 0;
			user[u].ship[sh].contra[i] = 0;
		}
	}


	//send if needed
	if ( sh == user[u].ship_selected )
		set_and_send_server_user_has_goods ( u,0 );
}

void sell_all_user_ship_goods ( int u, int sh )
{
	int i, p, s, g;
	int temp_amount[10];
	int change_made = 0;

	s = user[u].server_id;
	p = user[u].last_planet;
	g = user[u].guild_id;

	if(s<0) return;
	
	//checks...
	//ship exist?
	if ( user[u].ship[sh].ship_id < 0 ) return;
	if ( user[u].ship[sh].brand < 0 ) return;
	if ( user[u].ship[sh].type < 0 ) return;

	//set and clear
	for ( i=0;i<10;i++ )
		temp_amount[i] = user[u].ship[sh].cargo[i];

	clear_all_user_ship_goods ( u,sh );

	//user landed?
	if ( user[u].landed ) //okay then no problem....
	{
		int total_amount = 0;

		//server and planet ok?
		if ( s<0 ) return;
		if ( p<0 ) return;
		if ( !server[s].servername[0] ) return;
		if ( !server[s].planet[p].name[0] ) return;
		if ( server[s].planet[p].is_gate ) return;

		for ( i=0;i<10;i++ )
			if ( temp_amount[i] )
			{
				//simple price check
				if ( server[s].planet[p].good_price[i] < 1 ) continue;

				//give it back to the server
				server[s].planet[p].good_ammount[i] += temp_amount[i];

				//eh
				server_update_good_ammount ( s,p,i );

				//return some money
				user[u].money += ( server[s].planet[p].good_price[i] - TRADE_POINT_LOSS ) * temp_amount[i];

				//well
				game.rewrite_user_db = 1;
				change_made = 1;

				total_amount += temp_amount[i];
			}

		if ( change_made && user[u].con_id > -1 )
		{
			int guild_tax;

			user_send_money ( user[u].con_id );

			//guilds!
			if ( user[u].money >= MIN_MONEY && user[u].guild_balance >= 0 )
			{
				guild_tax = TRADE_POINT_LOSS * total_amount / 2;
				pay_planet_and_user_guild ( g,s,p,guild_tax );
			}
		}
	}


}

void user_assign_ship_destroy_code ( int u, int sh )
{
	int temp_code;
	int i;

	temp_code = rand();

	//fight our way through
	for ( i=0;i<10;i++ )
		if ( user[u].ship[i].destroy_code == temp_code )
			break;

	while ( i!=10 )
	{
		//try harder
		temp_code = rand();

		for ( i=0;i<10;i++ )
			if ( user[u].ship[i].destroy_code == temp_code )
				break;
	}

	user[u].ship[sh].destroy_code = temp_code;
}

void assign_all_user_ship_destroy_codes()
{
	int u, sh;

	for ( u=0;u<=user_max;u++ )
		for ( sh=0;sh<10;sh++ )
			user_assign_ship_destroy_code ( u,sh );
}

int user_has_goods ( int u )
{
	int i, sh;

	sh = user[u].ship_selected;

	//ship exist?
	if ( user[u].ship[sh].ship_id < 0 ) return 0;
	if ( user[u].ship[sh].type < 0 ) return 0;
	if ( user[u].ship[sh].brand < 0 ) return 0;

	for ( i=0;i<10;i++ )
		if ( user[u].ship[sh].cargo[i] || user[u].ship[sh].contra[i] ) return 1;

	return 0;

}

void user_sell_all_current_ship_goods ( int con )
{
	int u;
	int i, p, s, g, sh;
	int change_made, total_amount;

	u = user_con_id[con];

	s = user[u].server_id;
	p = user[u].last_planet;
	g = user[u].guild_id;
	sh = user[u].ship_selected;

	//landed?
	if ( !user[u].landed ) return;

	//ship exist?
	if ( user[u].ship[sh].ship_id < 0 ) return;
	if ( user[u].ship[sh].brand < 0 ) return;
	if ( user[u].ship[sh].type < 0 ) return;

	//server and planet ok?
	if ( s<0 ) return;
	if ( p<0 ) return;
	if ( !server[s].servername[0] ) return;
	if ( !server[s].planet[p].name[0] ) return;
	if ( server[s].planet[p].is_gate ) return;

	//sell_all_user_ship_goods(u, user[u].ship_selected);

	//start the selling
	change_made = 0;
	total_amount = 0;
	for ( i=0;i<10;i++ )
		if ( user[u].ship[sh].cargo[i] > 0 )
		{
			//simple price check
			if ( server[s].planet[p].good_price[i] < 1 ) continue;

			//needed for tax purposes
			total_amount += user[u].ship[sh].cargo[i];

			//give it back to the server
			server[s].planet[p].good_ammount[i] += user[u].ship[sh].cargo[i];

			//return some money
			user[u].money += ( server[s].planet[p].good_price[i] - TRADE_POINT_LOSS ) * user[u].ship[sh].cargo[i];

			//clear it
			user[u].ship[sh].cargo[i] = 0;

			//tell people of the new status
			user_update_good_ammount ( con,i );
			server_update_good_ammount ( s,p,i );

			//well
			game.rewrite_user_db = 1;
			change_made = 1;
		}

	if ( change_made )
	{
		int guild_tax;

		user_send_money ( con );

		//guilds!
		if ( user[u].money >= MIN_MONEY && user[u].guild_balance >= 0 )
		{
			guild_tax = TRADE_POINT_LOSS * total_amount / 2;
			pay_planet_and_user_guild ( g,s,p,guild_tax );
		}

		//server has to know
		set_and_send_server_user_has_goods ( u,user_has_goods ( u ) );
	}
}

void user_send_home ( int u )
{
	int s, z, k;
	int temp[PLANET_MAX];
	int con;
	char rtn_str[1000];

	s = user[u].server_id;
	con = user_con_id[u];

	//check if homeplanet is on this server, if not then make it so
	if ( s > -1 && user[u].home_id != s ) //user has no home then we must find one
	{
		for ( k=z=0;k<PLANET_MAX;k++ )
		{
			if ( server[s].planet[k].is_home && user[u].user_class == server[s].planet[k].home_class )
			{
				temp[z] = k;
				z++;
			}
		}

		if ( z ) //if we found some home planets
		{
			user[u].home_id = s;
			user[u].home_planet = temp[rand() % z];
			user[u].last_planet = user[u].home_planet;
		}
		else
		{
			;//if the server has no home planets set then wtf?
		}

		if ( con > -1 ) //let a monkey know if connected
		{
			//send the user the updated home planet info
			sprintf ( rtn_str,"7,%d,%d", s, user[u].home_planet );
			send_con_user ( con, rtn_str );
		}
	}
	else
	{
		//well if it is their home server
		user[u].last_planet = user[u].home_planet;
	}
}

int user_check_reward_min_money ( int u )
{
	int i;

	//got debt?
	if ( user[u].guild_balance ) return 0;

	//you negative?
	if ( user[u].money < 0 ) return 0;

	//got a ship?
	for ( i=0;i<10;i++ )
		if ( user[u].ship[i].ship_id > -1 && user[u].ship[i].brand > -1 && user[u].ship[i].type > -1 )
			break;

	if ( i!=10 ) return 0;

	//under the min?
	if ( user[u].money > MIN_MONEY ) return 0;

	//reward!
	user[u].money = MIN_MONEY;

	return 1;
}

void user_send_activated_user ( int u )
{
	int con;
	char rtn_str[200];

	con = user[u].con_id;

	if ( con<0 ) return;

	sprintf ( rtn_str,"37,%d", user[u].activated );
	send_con_user ( con,rtn_str );
}

void user_send_activated_server ( int u )
{
	int s, s_con;
	char rtn_str[200];

	s = user[u].server_id;

	if ( s<0 ) return;

	s_con = server[s].con_id;

	if ( s_con<0 ) return;

	sprintf ( rtn_str,"32,%d,%d", user[u].user_id, user[u].activated );
	send_con_server ( s_con,rtn_str );
}

void user_set_zone_sector ( int u, int z, int s )
{
	if ( s<0 ) return;
	if ( z<0 ) return;
	if ( s>=SECTOR_MAX ) return;
	if ( z>=ZONE_MAX ) return;

	user[u].zone = z;
	user[u].sector = s;
}

void user_drop_good ( int u, int g, int amount )
{
	int i, z, s, ss;

	//set some shit
	i = user[u].server_id;
	z = user[u].zone;
	s = user[u].sector;

	//got any killers?
	if ( s<0 ) return;
	if ( z<0 ) return;
	if ( g<0 ) return;
	if ( s>=SECTOR_MAX ) return;
	if ( z>=ZONE_MAX ) return;
	if ( g>=10 ) return;
	if ( i<0 ) return;

	//double check amount..
	if ( amount < 1 ) return;

	//user launched?
	if ( user[u].landed ) {printf ( "junkie landed\n" ); return;}

	//we dumping?
	if ( user[u].money < MIN_MONEY ) {printf ( "mon landed\n" ); return;}
	
	//owe the bank?
	if ( user[u].guild_balance < 0 ) return;

	//k for the good checking...
	ss = user[u].ship_selected;

	//the user have that much of the cargo?
	if ( user[u].ship[ss].cargo[g] < amount ) {printf ( "carg landed\n" ); return;}

	//well make the exchange..
	server[i].zone[z].sector[s].good_amount[g] += amount;
	user[u].ship[ss].cargo[g] -= amount;

	//update a bitch
	server_send_sector_good_amount ( i,s,z,g );

	//and another bitch
	user_update_good_ammount ( user[u].con_id,g );

	//check and tell a bitch
	set_and_send_server_user_has_goods ( u,user_has_goods ( u ) );
}

void user_drop_contra ( int u, int g, int amount, int g_price )
{
	int i, z, s, ss;

	//set some shit
	i = user[u].server_id;
	z = user[u].zone;
	s = user[u].sector;

	//got any killers?
	if ( s<0 ) return;
	if ( z<0 ) return;
	if ( g<0 ) return;
	if ( s>=SECTOR_MAX ) return;
	if ( z>=ZONE_MAX ) return;
	if ( g>=10 ) return;
	if ( i<0 ) return;

	//g_price messed up?
	if ( g_price<0 ) g_price = 0;

	//double check amount..
	if ( amount < 1 ) return;

	//user launched?
	if ( user[u].landed ) return;

	//we dumping?
	if ( !g_price && user[u].money < MIN_MONEY ) return;
	
	//we owe the bank?
	if ( user[u].guild_balance < 0 ) return;

	//k for the good checking...
	ss = user[u].ship_selected;

	//the user have that much of the cargo?
	if ( user[u].ship[ss].contra[g] < amount ) return;

	//this a sale?
	if ( g_price )
	{
		char rtn_str[1000];
		int total_awarded;

		//set
		total_awarded = amount * g_price;

		//make the money
		user[u].money += total_awarded;

		//lose the goods
		user[u].ship[ss].contra[g] -= amount;

		//send user money
		user_send_money ( user[u].con_id );

		if ( user[u].con_id > -1 )
		{
			//give them a little public message
			sprintf ( rtn_str,"... You have been awarded %d DC ..", total_awarded );
			send_user_public_chat ( user[u].con_id, 1, rtn_str );
		}
	}
	else //its a normal drop
	{
		//well make the exchange..
		server[i].zone[z].sector[s].contra_amount[g] += amount;
		user[u].ship[ss].contra[g] -= amount;

		//update a bitch
		server_send_sector_contra_amount ( i,s,z,g );
	}

	//and another bitch
	user_update_contra_ammount ( user[u].con_id,g );

	//check and tell a bitch
	set_and_send_server_user_has_goods ( u,user_has_goods ( u ) );
}

void user_drop_all_goods ( int u )
{
	int g;
	int sh_sel;

	sh_sel = user[u].ship_selected;

	//ship exist?
	if ( user[u].ship[sh_sel].ship_id < 0 ) return;
	if ( user[u].ship[sh_sel].brand < 0 ) return;
	if ( user[u].ship[sh_sel].type < 0 ) return;

	//we launched?
	if ( user[u].landed ) return;

	//drop the goods
	for ( g=0;g<10;g++ )
		if ( user[u].ship[sh_sel].cargo[g] > 0 )
			user_drop_good ( u,g,user[u].ship[sh_sel].cargo[g] );

	//drop the contras
	for ( g=0;g<10;g++ )
		if ( user[u].ship[sh_sel].contra[g] > 0 )
			user_drop_contra ( u,g,user[u].ship[sh_sel].contra[g], 0 );

}

void relay_sat_under_attack_warning ( int s, int p )
{
	int cur_time;
	char rtn_str[1024];
	int g, i;

	//set stuff
	cur_time = time ( 0 );
	g = server[s].planet[p].sat.guild_id;

	//is it about that time?
	if ( cur_time - server[s].planet[p].sat.last_warning_time < SAT_WARNING_TIME ) return;

	//prepare the message
	sprintf ( rtn_str,"32,3,Guild Warning : Satellite : : Satellite of %s on %s is under attack%c", server[s].planet[p].name, server[s].servername, 13 );

	//relay the message
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].guild_id == g )
			raw_send_con_user ( user[i].con_id,rtn_str );

	//set new time
	server[s].planet[p].sat.last_warning_time = cur_time;
}

void user_add_chat_user ( int con, char *username )
{
	int u, v_u, i, j;
	char rtn_str[2000];

	u = user_con_id[con];
	v_u = user_to_id ( username );

	//checks
	if ( v_u == -1 ) return;
	if ( u == v_u ) return;

	//user already on the list?
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] == v_u )
		{
			//let the user know of this
			if ( user[u].private_chat_status[i] == 1 )
				sprintf ( rtn_str,"32,1,... Private Chat : Already awaiting private chat link with %s ..", user[v_u].username );
			else if ( user[u].private_chat_status[i] == 2 )
				sprintf ( rtn_str,"32,1,... Private Chat : Private chat link already established with %s ..", user[v_u].username );
			send_con_user ( con,rtn_str );

			//exit
			return;
		}

	//find an empty spot
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] < 0 )
			break;

	if ( i==MAX_PRIVATE_CHAT_USERS )
	{
		//let the user know of this
		sprintf ( rtn_str,"32,1,... Private Chat : No more available private chat link slots, max:%d ..", MAX_PRIVATE_CHAT_USERS );
		send_con_user ( con,rtn_str );

		//exit
		return;
	}

	//well we have found a spot to add this fool -----

	//add to the list
	user[u].private_chat_user[i] = v_u;

	//now are we on their list?
	for ( j=0;j<MAX_PRIVATE_CHAT_USERS;j++ )
		if ( user[v_u].private_chat_user[j] == u )
			break;

	if ( j==MAX_PRIVATE_CHAT_USERS ) //we not on their list
	{
		//set statys on our list
		user[u].private_chat_status[i] = 1;
		send_user_private_chat_entry ( u,i );

		//notify us
		sprintf ( rtn_str,"32,1,... Private Chat : Awaiting private chat link with %s ..", user[v_u].username );
		send_con_user ( con,rtn_str );

		//notify them
		sprintf ( rtn_str,"32,1,... Private Chat : %s is requesting private chat link ..", user[u].username );
		send_con_user ( user[v_u].con_id,rtn_str );
	}
	else //we on their list
	{
		//set status no our list
		user[u].private_chat_status[i] = 2;
		send_user_private_chat_entry ( u,i );

		//set status on their list
		user[v_u].private_chat_status[j] = 2;
		send_user_private_chat_entry ( v_u,j );

		//notify us
		sprintf ( rtn_str,"32,1,... Private Chat : Private chat link established with %s ..", user[v_u].username );
		send_con_user ( con,rtn_str );

		//notify them
		sprintf ( rtn_str,"32,1,... Private Chat : Private chat link established with %s ..", user[u].username );
		send_con_user ( user[v_u].con_id,rtn_str );
	}
}

void user_remove_chat_user ( int con, char *username )
{
	int u, v_u, i, j;

	char rtn_str[2000];

	u = user_con_id[con];
	v_u = user_to_id ( username );

	//checks
	if ( v_u == -1 )
	{
		sprintf ( rtn_str,"32,1,... Private Chat : %s not found ..", username );
		send_con_user ( con,rtn_str );

		return;
	}
	if ( u == v_u ) return;

	//see if we can find this user
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] == v_u )
			break;

	if ( i==MAX_PRIVATE_CHAT_USERS )
	{
		sprintf ( rtn_str,"32,1,... Private Chat : %s not on the list ..", user[v_u].username );
		send_con_user ( con,rtn_str );

		return;
	}

	//okay remove from our list
	user[u].private_chat_user[i] = -1;

	//now determine what kind of setup we really had
	if ( user[u].private_chat_status[i] == 1 ) //awaiting connecting
	{
		//remove our link
		user[u].private_chat_status[i] = 0;
		send_user_private_chat_remove ( u,user[v_u].username );

		//notify us
		sprintf ( rtn_str,"32,1,... Private Chat : Awaiting link dropped for %s ..", user[v_u].username );
		send_con_user ( con,rtn_str );

		//notify them
		sprintf ( rtn_str,"32,1,... Private Chat : Link invitation canceled by %s ..", user[u].username );
		send_con_user ( user[v_u].con_id,rtn_str );
	}
	else //had connection
	{
		//remove our link
		user[u].private_chat_status[i] = 0;
		send_user_private_chat_remove ( u,user[v_u].username );

		//remove their link
		for ( j=0;j<MAX_PRIVATE_CHAT_USERS;j++ )
			if ( user[v_u].private_chat_user[j] == u )
			{
				user[v_u].private_chat_status[j] = 1;
				send_user_private_chat_entry ( v_u,j );
				break;
			}

		//notify us
		sprintf ( rtn_str,"32,1,... Private Chat : Private chat link dropped with %s ..", user[v_u].username );
		send_con_user ( con,rtn_str );

		//notify them
		sprintf ( rtn_str,"32,1,... Private Chat : Private chat link dropped with %s ..", user[u].username );
		send_con_user ( user[v_u].con_id,rtn_str );
	}
}

void user_list_online_chat_user ( int con )
{
	int u, i;
	char rtn_str[2000];
	int did_add;

	u = user_con_id[con];

	did_add = 0;
	strcpy ( rtn_str,"32,1,... Private Chat : Online Links :: " );
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] > -1 && user[u].private_chat_status[i] == 2 )
		{
			int v_u;

			v_u = user[u].private_chat_user[i];

			//check
			if ( user[v_u].con_id < 0 )
				continue;

			strcat ( rtn_str,user[user[u].private_chat_user[i]].username );
			strcat ( rtn_str,", " );
			did_add = 1;
		}
	if ( did_add )
	{
		int len;

		len = strlen ( rtn_str );

		rtn_str[len-2] = ' ';
		rtn_str[len-1] = 0;
	}
	strcat ( rtn_str,".." );
	send_con_user ( con,rtn_str );
}

void user_list_chat_user ( int con )
{
	int u, i;
	char rtn_str[2000];
	int did_add;

	u = user_con_id[con];

	//compile list of awaiting
	did_add = 0;
	strcpy ( rtn_str,"32,1,... Private Chat : Awaiting link :: " );
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] > -1 && user[u].private_chat_status[i] == 1 )
		{
			strcat ( rtn_str,user[user[u].private_chat_user[i]].username );
			strcat ( rtn_str,", " );
			did_add = 1;
		}
	if ( did_add )
	{
		int len;

		len = strlen ( rtn_str );

		rtn_str[len-2] = ' ';
		rtn_str[len-1] = 0;
	}
	strcat ( rtn_str,".." );
	send_con_user ( con,rtn_str );

	//compile list of connected
	did_add = 0;
	strcpy ( rtn_str,"32,1,... Private Chat : Link established :: " );
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] > -1 && user[u].private_chat_status[i] == 2 )
		{
			strcat ( rtn_str,user[user[u].private_chat_user[i]].username );
			strcat ( rtn_str,", " );
			did_add = 1;
		}
	if ( did_add )
	{
		int len;

		len = strlen ( rtn_str );

		rtn_str[len-2] = ' ';
		rtn_str[len-1] = 0;
	}
	strcat ( rtn_str,".." );
	send_con_user ( con,rtn_str );
}

void user_relay_private_chat ( int con, char *message )
{
	int i, u;
	char rtn_str[2000];

	u = user_con_id[con];

	//prepare the message
	sprintf ( rtn_str,"32,0,%s : %s%c", user[u].username, message, 13 );

	//relay the message
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] > -1 && user[u].private_chat_status[i] == 2 )
			raw_send_con_user ( user[user[u].private_chat_user[i]].con_id,rtn_str );
}

void clear_all_user_private_chats()
{
	int u, i;

	for ( u=0;u<=user_max;u++ )
		for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		{
			user[u].private_chat_user[i] = -1;
			user[u].private_chat_status[i] = 0;
		}
}

void user_clear_private_chats_raw ( int u )
{
	int i;

	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
	{
		user[u].private_chat_user[i] = -1;
		user[u].private_chat_status[i] = 0;
	}
}

void send_user_private_chat_list ( int u )
{
	int i;
	char rtn_str[2000] = "36";
	char append_str[200];
	int did_add = 0;

	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( user[u].private_chat_user[i] > -1 && user[u].private_chat_status[i] == 2 )
		{
			sprintf ( append_str,",%s,%d", user[user[u].private_chat_user[i]].username, user[u].private_chat_status[i] );
			strcat ( rtn_str,append_str );
			did_add = 1;
		}

	if ( did_add )
		send_con_user ( user[u].con_id, rtn_str );
}

void send_user_private_chat_entry ( int u, int i )
{
	char rtn_str[200];
	int v_u;

	v_u = user[u].private_chat_user[i];

	if ( v_u < 0 ) return; //wtf

	sprintf ( rtn_str,"34,%s,%d", user[user[u].private_chat_user[i]].username, user[u].private_chat_status[i] );
	send_con_user ( user[u].con_id, rtn_str );
}

void send_user_private_chat_remove ( int u, char *username )
{
	char rtn_str[200];

	sprintf ( rtn_str,"35,%s", username );
	send_con_user ( user[u].con_id, rtn_str );
}

void user_change_password ( int con, char *old_password, char *new_password )
{
	int u;
	char rtn_str[200];

	u = user_con_id[con];

	if ( u < 0 ) return;

	if(user_check_lease_loggedin(u)) return;
	
	//cap
	old_password[20] = 0;
	new_password[20] = 0;

	if ( !str_match ( old_password, user[u].password ) )
	{
		sprintf ( rtn_str,"... Error : Old password incorrect .." );
		send_user_private_chat ( con,1,rtn_str );

		return;
	}
	
	if(!new_password[0])
	{
		sprintf ( rtn_str,"... Error : new password format incorrect .." );
		send_user_private_chat ( con,1,rtn_str );

		return;
	}

	//DO THE DEED
	strcpy ( user[u].password, new_password );

	sprintf ( rtn_str,"... Password successfully changed .." );
	send_user_private_chat ( con,1,rtn_str );

	game.rewrite_user_db = 1;
}

void user_retrive_short_user_stats ( int con, char *search_user )
{
	int u;
	char rtn_str[1000];

	//first try finding the requested
	u = user_to_id ( search_user );

	if ( u == -1 || search_user[0] == 0 ) //use our own
		u = user_con_id[con];

	//it was found
	sprintf ( rtn_str,"... Stat Viewer : %s :: Exp:%d DC:%d ..", user[u].username, user[u].exp, user[u].money );
	send_user_public_chat ( con,1,rtn_str );
}

void user_retrive_short_guild_reward_list ( int con, char *search_guild )
{
	int g;
	int u;
	char rtn_str[1000];

	u = user_con_id[con];

	//first try finding the requested
	g = guild_name_to_id ( search_guild );

	if ( g == -1 || search_guild[0] == 0 ) //use our own
		g = user[u].guild_id;

	//eh?
	if ( g < 0 ) return;
	if ( g > guild_max ) return;

	//it was found
	sprintf ( rtn_str,"... Guild Reward 2k: %s ..", guild[g].enemy[0] );
	send_user_public_chat ( con,1,rtn_str );

	sprintf ( rtn_str,"... Guild Reward 3k: %s ..", guild[g].enemy[1] );
	send_user_public_chat ( con,1,rtn_str );

	sprintf ( rtn_str,"... Guild Reward 5k: %s ..", guild[g].enemy[2] );
	send_user_public_chat ( con,1,rtn_str );

	sprintf ( rtn_str,"... Guild Stat Viewer : %s :: Guild Rewards ..", guild[g].name );
	send_user_public_chat ( con,1,rtn_str );

}

void user_edit_guild_deco ( int con, int d_i, char *deco_name, int deco_activated )
{
	int g;
	int u, r, i, k, point;

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//capping and checkin
	if ( d_i < 0 ) return;
	if ( d_i >= MAX_DECO ) return;

	if ( deco_activated )
		deco_activated = 1;

	deco_name[30] = 0;

	//make the change
	guild[g].deco_activated[d_i] = deco_activated;
	strcpy ( guild[g].deco_name[d_i], deco_name );

	//set for db
	game.rewrite_guild_db = 1;

	//tell the guild
	guild_anouce_deco_change ( g, d_i );
}

void guild_anouce_deco_change ( int g, int d_i )
{
	int i;
	char rtn_str[500];

	//checkin
	if ( d_i < 0 ) return;
	if ( d_i >= MAX_DECO ) return;
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make the string
	sprintf ( rtn_str,"38,%d,%s,%d%c", d_i, guild[g].deco_name[d_i], guild[g].deco_activated[d_i], 13 );

	//send the string
	for ( i=0;i<=user_con_top;i++ )
		if ( user_con_id[i] != -1 && g == user[user_con_id[i]].guild_id )
			raw_send_con_user ( i, rtn_str );
}

int decoration_to_id ( int g, char *deco_name )
{
	int i;
	char lcase_deco[31];

	//simple
	if ( !deco_name[0] ) return -1;

	//cap
	deco_name[30] = 0;

	//make lcase
	lcase ( lcase_deco, deco_name );

	//run through
	for ( i=0;i<MAX_DECO;i++ )
		if ( guild[g].deco_name[i][0] )
		{
			char inner_lcase[31];

			lcase ( inner_lcase, guild[g].deco_name[i] );

			if ( str_match ( inner_lcase, lcase_deco ) )
				return i;
		}

	return -1; //none found
}

void guild_award_deco ( int con, char *player_name, char *deco_name )
{
	int g;
	int u, r, i, k, point;
	int v_u, d_i, v_g, v_con;
	char rtn_str[500];

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//cap em
	player_name[20] = 0;
	deco_name[30] = 0;

	v_u = user_to_id ( player_name );
	d_i = decoration_to_id ( g, deco_name );

	//player not found?
	if ( v_u == -1 )
	{
		sprintf ( rtn_str,"... Player not found .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//set v_g
	v_g = user[v_u].guild_id;
	v_con = user[v_u].con_id;

	if ( g != v_g )
	{
		sprintf ( rtn_str,"... Player not in same guild .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//deco not found?
	if ( d_i == -1 )
	{
		sprintf ( rtn_str,"... Decoration not found .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//they already have it?
	if ( user[v_u].has_deco[d_i] )
	{
		sprintf ( rtn_str,"... Player already has that decoration .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//give them it
	user[v_u].has_deco[d_i] = 1;

	//tell you
	sprintf ( rtn_str,"... Player %s awarded decoration : %s ..", user[v_u].username, guild[g].deco_name[d_i] );
	send_user_private_chat ( con, 1, rtn_str );

	//tell them
	if ( v_con != -1 )
	{
		sprintf ( rtn_str,"Guild : Decoration Awarded : : %s", guild[g].deco_name[d_i] );
		send_user_private_chat ( v_con, 3, rtn_str );
	}

	//save db
	game.rewrite_user_db = 1;
}

void guild_remove_deco ( int con, char *player_name, char *deco_name )
{
	int g;
	int u, r, i, k, point;
	int v_u, d_i, v_g, v_con;
	char rtn_str[500];

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) ) return;

	//cap em
	player_name[20] = 0;
	deco_name[30] = 0;

	v_u = user_to_id ( player_name );
	d_i = decoration_to_id ( g, deco_name );

	//player not found?
	if ( v_u == -1 )
	{
		sprintf ( rtn_str,"... Player not found .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//set v_g
	v_g = user[v_u].guild_id;
	v_con = user[v_u].con_id;

	if ( g != v_g )
	{
		sprintf ( rtn_str,"... Player not in same guild .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//deco not found?
	if ( d_i == -1 )
	{
		sprintf ( rtn_str,"... Decoration not found .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//they already have it?
	if ( !user[v_u].has_deco[d_i] )
	{
		sprintf ( rtn_str,"... Player does not have that decoration .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	//give them it
	user[v_u].has_deco[d_i] = 0;

	//tell you
	sprintf ( rtn_str,"... Player %s's decoration removed : %s ..", user[v_u].username, guild[g].deco_name[d_i] );
	send_user_private_chat ( con, 1, rtn_str );

	//tell them
	if ( v_con != -1 )
	{
		sprintf ( rtn_str,"Guild : Decoration Removed : : %s", guild[g].deco_name[d_i] );
		send_user_private_chat ( v_con, 3, rtn_str );
	}

	//save db
	game.rewrite_user_db = 1;
}

void guild_list_bugs ( int con )
{
	int g;
	int u, r, i, k, point;
	int s, p;
	char rtn_str[500], planet_str[2000];

	u = user_con_id[con];

	g = user[u].guild_id;

	r = user[u].guild_rank;

	if(user_check_lease_loggedin(u)) return;
	
	//check if they are in a guild
	if ( g > guild_max ) return;
	if ( g < 0 ) return;

	//make sure their rank doesn't break anything ... ?
	if ( r < 0 ) return;
	if ( r > 5 ) return;

	//check if they have the right allowence
	if ( !guild[g].rank_allow[r][4] && strcmp ( guild[g].owner,user[u].username ) )
	{
		sprintf ( rtn_str,"... Guild edit rank required for that command .." );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	for ( s=0;s<=server_max;s++ )
		if ( server[s].con_id > -1 )
		{
			//now collect the planets
			planet_str[0] = 0;
			for ( p=0;p<PLANET_MAX;p++ )
				if ( server[s].planet[p].name[0] )
				{
					char cat_str[50];

					//checks
					if ( server[s].planet[p].sat.guild_id == g ) continue;
					if ( server[s].planet[p].sat.bug_guild_id != g ) continue;

					sprintf ( cat_str," %s,", server[s].planet[p].name );
					strcat ( planet_str, cat_str );
				}

			if ( planet_str[0] ) //found some, remove that last comma
				planet_str[strlen ( planet_str ) - 1] = 0;

			sprintf ( rtn_str,"... Planets bugged on %s :%s ..", server[s].servername, planet_str );
			send_user_public_chat ( con, 1, rtn_str );
		}
}

void guild_list_deco ( int con, char *player_name )
{
	int u, g, i;
	char rtn_str[500];
	char deco_str[2000];
	char cat_str[500];

	if ( !player_name[0] )
	{
		u = user_con_id[con];
	}
	else
	{
		u = user_to_id ( player_name );
	}

	//check
	if ( u == -1 )
	{
		sprintf ( rtn_str,"... Player not found .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	g = user[u].guild_id;

	if ( g > guild_max ) return;
	if ( g < 0 ) return;


	deco_str[0] = 0;
	for ( i=0;i<MAX_DECO;i++ )
		if ( user[u].has_deco[i] && guild[g].deco_name[i][0] && guild[g].deco_activated[i] )
		{
			sprintf ( cat_str," %s,", guild[g].deco_name[i] );
			strcat ( deco_str, cat_str );
		}

	if ( deco_str[0] ) //found some, remove that last comma
		deco_str[strlen ( deco_str ) - 1] = 0;

	sprintf ( rtn_str,"... Decorations owned by %s :%s ..", user[u].username, deco_str );
	send_user_public_chat ( con, 1, rtn_str );
}

void guild_list_decog ( int con, char *guild_name )
{
	int u, g, i;
	char rtn_str[500];
	char deco_str[2000];
	char cat_str[500];

	u = user_con_id[con];

	if ( !guild_name[0] )
	{
		g = user[u].guild_id;
	}
	else
	{
		g = guild_name_to_id ( guild_name );
	}

	//check
	if ( g < 0 || g > guild_max )
	{
		sprintf ( rtn_str,"... Guild not found .." );
		send_user_public_chat ( con, 1, rtn_str );
		return;
	}

	deco_str[0] = 0;
	for ( i=0;i<MAX_DECO;i++ )
		if ( guild[g].deco_name[i][0] && guild[g].deco_activated[i] )
		{
			sprintf ( cat_str," %s,", guild[g].deco_name[i] );
			strcat ( deco_str, cat_str );
		}

	if ( deco_str[0] ) //found some, remove that last comma
		deco_str[strlen ( deco_str ) - 1] = 0;

	sprintf ( rtn_str,"... Decorations available in %s :%s ..", guild[g].name, deco_str );
	send_user_public_chat ( con, 1, rtn_str );
}

void user_clear_decorations ( int u )
{
	int i;

	for ( i=0;i<MAX_DECO;i++ )
		user[u].has_deco[i] = 0;
}

void guild_clear_decorations ( int g )
{
	int i;

	for ( i=0;i<MAX_DECO;i++ )
	{
		guild[g].deco_name[i][0] = 0;
		guild[g].deco_activated[i] = 0;
	}
}

void user_bug_sat ( int con )
{
	int u, g, s, p;

	//set stuff
	u = user_con_id[con];
	g = user[u].guild_id;
	s = user[u].server_id;
	p = user[u].last_planet;

	//the dumb checks
	if ( g<0 ) return;
	if ( s<0 ) return;
	if ( p<0 ) return;
	if ( g>=MAX_GUILD ) return;
	if ( s>=MAX_SERVER ) return;
	if ( p>=PLANET_MAX ) return;
	if ( !server[s].planet[p].name[0] ) return;  //planet exist?
	if ( !guild[g].name[0] ) return;  //guild exist?

	//checks
	if ( !user[u].landed ) return;

	//already bought?
	if ( server[s].planet[p].sat.guild_id >= 0 ) return;

	//allready bugged?
	if ( server[s].planet[p].sat.bug_guild_id >= 0 ) return;

	//set the monkey! woo
	server[s].planet[p].sat.bug_guild_id = g;

	server[s].rewrite_sat_db = 1;
}

void user_request_pc_time ( int con )
{
	double time_till;
	int minutes_till;
	char rtn_str[500];

	time_till = ( 60 * 60 ) - ( current_time() - game.last_pricechange );
	minutes_till = ( ( int ) time_till / 60 );

	sprintf ( rtn_str,"... Time until price change: %d minutes %.1lf seconds ..", minutes_till, time_till - ( minutes_till * 60 ) );
	send_user_public_chat ( con, 1, rtn_str );
}

void clear_all_user_uni_chat()
{
	int u;

	for ( u=0;u<=MAX_USER;u++ )
		user[u].uni_chat_on = 1;
}

void user_toggle_uni_chat ( int con )
{
	int u;
	char rtn_str[500];

	u = user_con_id[con];

	if ( user[u].uni_chat_on )
	{
		user[u].uni_chat_on = 0;

		sprintf ( rtn_str,"... Universal chat off .." );
		send_user_public_chat ( con, 2, rtn_str );
	}
	else
	{
		user[u].uni_chat_on = 1;

		sprintf ( rtn_str,"... Universal chat on .." );
		send_user_public_chat ( con, 2, rtn_str );
	}
}

void insert_all_users_jabber()
{
	int u;

	for ( u=0;u<=user_max;u++ )
		mysql_make_jabber_insert ( user[u].username,user[u].password );
}

int ship_owned_amount ( int s, int sh, int k )
{
	int u, i;
	int the_amount = 0;

	//checkin
	if ( s<0 ) return 0;
	if ( sh<0 ) return 0;
	if ( k<0 ) return 0;
	if ( s>=MAX_SERVER ) return 0;
	if ( sh>=SHIP_MAX ) return 0;
	if ( k>=8 ) return 0;

	for ( u=0;u<=user_max;u++ )
		for ( i=0;i<10;i++ )
		{
			if ( user[u].ship[i].ship_id != s ) continue;
			if ( user[u].ship[i].brand != sh ) continue;
			if ( user[u].ship[i].type != k ) continue;

			the_amount++;
		}

	return the_amount;
}

void user_award_mfc ( char *user_name, int exp_added, int money_added )
{
	int u;

	if ( exp_added < 0 ) return;
	if ( money_added < 0 ) return;

	if ( !exp_added && !money_added ) return;

	u = user_to_id ( user_name );

	if ( u == -1 ) return;

	if ( MFC_EXP_MAX < user[u].exp ) money_added = 0;

	user[u].money += money_added;
	user[u].exp += exp_added;

	printf ( "user:%s awarded-exp=%d -money=%d, now has exp=%d money=%d\n", user[u].username, exp_added, money_added, user[u].exp, user[u].money );

	if ( user[u].con_id > -1 )
	{
		char rtn_str[5000];

		sprintf ( rtn_str,"... Marketing script payout Exp:%d DC:%d ..", exp_added, money_added );
		send_user_private_chat ( user[u].con_id,1,rtn_str );

		user_send_money ( user[u].con_id );
		user_send_exp ( u );
	}

	game.rewrite_user_db = 1;
}

void user_get_dsamount ( int con )
{
	char rtn_str[2000];
	int u;

	u = user_con_id[con];
	
	if(user_check_lease_loggedin(u)) return;

	sprintf ( rtn_str,"... Donation Standards owned : %d ..", user[u].standards );
	send_user_public_chat ( con, 1, rtn_str );
}

void user_transferds ( int con, char *playername, int amount )
{
	char rtn_str[2000];
	int u, v_u;
	int to_con;

	//checks
	if ( amount < 1 ) return;

	u = user_con_id[con];
	v_u = user_to_id ( playername );
	
	if(user_check_lease_loggedin(u)) return;

	if ( user[u].standards <= 0 )
	{
		sprintf ( rtn_str,"... Your character has no DS, to acquire DS please visit www.nighsoft.net .." );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( user[u].standards < amount )
	{
		sprintf ( rtn_str,"... %d DS more required, to acquire DS please visit www.nighsoft.net ..", amount - user[u].standards );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( v_u == -1 )
	{
		sprintf ( rtn_str,"... DS recipient not found : %s ..", playername );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	//well make the transfer
	user[u].standards -= amount;
	user[v_u].standards += amount;

	sprintf ( rtn_str,"%s transfered %d to %s, %s has %d ds, %s has %d ds", user[u].username, amount, user[v_u].username, user[u].username, user[u].standards, user[v_u].username, user[v_u].standards );
	printd_ds ( rtn_str );

	//tell the people
	to_con = user[v_u].con_id;
	if ( to_con > -1 )
	{
		if ( amount > 1 )
			sprintf ( rtn_str,"... %s Has Transferred %d Donation Standards to Your Account  ..", user[u].username, amount );
		else
			sprintf ( rtn_str,"... %s Has Transferred 1 Donation Standard to Your Account  ..", user[u].username );
		send_user_private_chat ( to_con,3,rtn_str );
	}

	if ( amount > 1 )
		sprintf ( rtn_str,"... %d Donation Standards Have Been Transferred to %s  ..", amount, user[v_u].username );
	else
		sprintf ( rtn_str,"... 1 Donation Standard Has Been Transferred to %s  ..", user[v_u].username );
	send_user_private_chat ( con,3,rtn_str );

	//save shit
	game.rewrite_user_db = 1;
}

void user_buyactivation ( int con, char *playername )
{
	int u, v_u;
	char rtn_str[2000];

	u = user_con_id[con];
	v_u = user_to_id ( playername );
	
	if(user_check_lease_loggedin(u)) return;

	if ( user[u].standards <= 0 )
	{
		sprintf ( rtn_str,"... Your character has no DS, to acquire DS please visit www.nighsoft.net .." );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( user[u].standards < DS_FOR_ACTIVATION )
	{
		sprintf ( rtn_str,"... %d DS more required, to acquire DS please visit www.nighsoft.net ..", DS_FOR_ACTIVATION - user[u].standards );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	//check user stuff
	if ( playername[0] && v_u == -1 )
	{
		sprintf ( rtn_str,"... Activation recipient %s not found ..", playername );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( !playername[0] ) v_u = u;

	//make the actual activation
	if ( activate_user ( v_u ) )
	{
		//make them lose the ds
		user[u].standards -= DS_FOR_ACTIVATION;

		sprintf ( rtn_str,"%s Activated %s, has now %d ds", user[u].username, user[v_u].username, user[u].standards );
		printd_ds ( rtn_str );

		if ( u != v_u )
		{
			sprintf ( rtn_str,"... Activation of %s completed ..", user[v_u].username );
			send_user_public_chat ( con, 1, rtn_str );
		}
	}
	else
	{
		sprintf ( rtn_str,"... %s has already been activated ..", user[v_u].username );
		send_user_public_chat ( con, 1, rtn_str );
	}
}

void user_buyexp ( int con, char *playername, int amount )
{
	int u, v_u, to_con, total_awarded;
	char rtn_str[2000];

	if ( amount < 1 ) return;

	u = user_con_id[con];
	v_u = user_to_id ( playername );
	
	if(user_check_lease_loggedin(u)) return;

	if ( user[u].standards <= 0 )
	{
		sprintf ( rtn_str,"... Your character has no DS, to acquire DS please visit www.nighsoft.net .." );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( user[u].standards < amount )
	{
		sprintf ( rtn_str,"... %d DS more required, to acquire DS please visit www.nighsoft.net ..", amount - user[u].standards );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	//check user stuff
	if ( playername[0] && v_u == -1 )
	{
		sprintf ( rtn_str,"... Exp recipient %s not found ..", playername );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( !playername[0] ) v_u = u;

	//give the monkey what they fing want.
	total_awarded = amount * EXP_FOR_DS;
	user[u].standards -= amount;
	user[v_u].exp += total_awarded;

	sprintf ( rtn_str,"%s Exp'd %d %s, has now %d ds", user[u].username, amount, user[v_u].username, user[u].standards );
	printd_ds ( rtn_str );

	//tell the shitter
	to_con = user[v_u].con_id;

	if ( to_con > -1 )
	{
		user_send_exp ( v_u );

		sprintf ( rtn_str,"... You have been awarded %d Experience ..", total_awarded );
		send_user_private_chat ( to_con, 1, rtn_str );
	}

	if ( u != v_u )
	{
		sprintf ( rtn_str,"... %s has been awarded %d Experience ..", user[v_u].username, total_awarded );
		send_user_private_chat ( con, 1, rtn_str );
	}
}

void user_buydc ( int con, char *playername, int amount )
{
	int u, v_u, to_con, total_awarded;
	char rtn_str[2000];

	if ( amount < 1 ) return;

	u = user_con_id[con];
	v_u = user_to_id ( playername );
	
	if(user_check_lease_loggedin(u)) return;

	if ( user[u].standards <= 0 )
	{
		sprintf ( rtn_str,"... Your character has no DS, to acquire DS please visit www.nighsoft.net .." );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( user[u].standards < amount )
	{
		sprintf ( rtn_str,"... %d DS more required, to acquire DS please visit www.nighsoft.net ..", amount - user[u].standards );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	//check user stuff
	if ( playername[0] && v_u == -1 )
	{
		sprintf ( rtn_str,"... DC recipient %s not found ..", playername );
		send_user_public_chat ( con, 1, rtn_str );

		return;
	}

	if ( !playername[0] ) v_u = u;

	//give the monkey what they fing want.
	total_awarded = amount * DC_FOR_DS;
	user[u].standards -= amount;
	user[v_u].money += total_awarded;

	sprintf ( rtn_str,"%s DC'd %d %s, has now %d ds", user[u].username, amount, user[v_u].username, user[u].standards );
	printd_ds ( rtn_str );

	//tell the shitter
	to_con = user[v_u].con_id;

	if ( to_con > -1 )
	{
		user_send_money ( to_con );

		sprintf ( rtn_str,"... You have been awarded %d DC ..", total_awarded );
		send_user_private_chat ( to_con, 1, rtn_str );
	}

	if ( u != v_u )
	{
		sprintf ( rtn_str,"... %s has been awarded %d DC ..", user[v_u].username, total_awarded );
		send_user_private_chat ( con, 1, rtn_str );
	}
}

int activate_user ( int u )
{
	int g, i;
	int con;
	char rtn_str[2000];

	//check?
	if ( u<0 ) return 0;
	if ( u>=MAX_USER ) return 0;

	if ( user[u].activated ) return 0;

	//later purposes
	con = user[u].con_id;
	g = user[u].guild_id;

	//do it
	user[u].activated = 1;

	//give them some reward
	user[u].money += 2500000;
	user[u].exp += 200;

	//mo nay
	user_send_money ( con );
	user_send_exp ( u );

	//tell who all that might care
	user_send_activated_user ( u );
	user_send_activated_server ( u );

	//tell the guild
	sprintf ( rtn_str,"32,3,Guild Message : : %s has been activated%c", user[u].username, 13 );
	for ( i=0;i<=user_max;i++ )
		if ( user[i].con_id > -1 && user[i].guild_id == g )
			raw_send_con_user ( user[i].con_id,rtn_str );

	//tell that player
	sprintf ( rtn_str,"... Congratulations, your account has been activated .." );
	send_user_private_chat ( con, 1, rtn_str );

	//do this
	game.rewrite_user_db = 1;

	return 1;
}

void award_user_ds ( char *playername, int amount )
{
	int u, con;
	char rtn_str[2000];

	u = user_to_id ( playername );

	printf ( "ds awarding called for %s : %d\n", playername, amount );

	if ( amount < 1 ) return;

	if ( u == -1 ) return;

	//set
	con = user[u].con_id;

	//give the shitter
	user[u].standards += amount;

	//bastard know
	if ( con > -1 )
	{

		sprintf ( rtn_str,"... Your account has been credited %d DS, spend wisely ..", amount );
		send_user_private_chat ( con, 1, rtn_str );
	}

	//log'in
	sprintf ( rtn_str,"%s credited %d ds from website, now has %d ds", user[u].username, amount, user[u].standards );
	printd_ds ( rtn_str );

	//bastard save
	game.rewrite_user_db = 1;
}

int flood_check ( int u )
{
	double cur_time;

	cur_time = current_time();

	if ( cur_time - user[u].last_message_time < FLOOD_LIMIT )
	{
		user[u].last_message_time = cur_time;

		return 1;
	}
	else
	{
		user[u].last_message_time = cur_time;

		return 0;
	}
}

int user_likely_barcode ( char *name )
{
	int i;
	int got_I = 0;
	int got_l = 0;

	for ( i=0; name[i];i++ )
	{
		if ( name[i] == 'I' ) got_I = 1;
		if ( name[i] == 'l' ) got_l = 1;
	}

	return got_I && got_l;
}

int user_bad_spacing ( char *name )
{
	int i;
	int len;

	if ( !name ) return 0;

	len = strlen ( name );

	//nothing?
	if ( !len ) return 0;

	//check front
	if ( name[0] == ' ' ) return 1;

	//check end
	if ( name[len-1] == ' ' ) return 1;

	//check for doubles
	for ( i=0;i<len-1;i++ )
		if ( name[i] == ' ' && name[i+1] == ' ' ) return 1;

	return 0;
}

void ping_user_con ( int con )
{
	char send_str[5] = {'3', '9', ',', 13, 0};

	raw_send_con_user ( con, send_str );
}

void ping_all_users()
{
	int i;
	char send_str[5] = {'3', '9', ',', 13, 0};

	for ( i=0;i<=user_con_top;i++ )
		raw_send_con_user ( i, send_str );
}

void user_ping_response ( int con )
{

}

void user_do_noob_message ( int u )
{
	int the_time = time ( 0 );
	const int max_time = 60 * 60 * 24 * 3;
	const int max_exp = 5;
	char rtn_str[1024];
	
	if ( the_time - user[u].time_created > max_time ) return;
	if ( user[u].exp > max_exp ) return;
	if ( user[u].con_id == -1 ) return;

	sprintf ( rtn_str,"... Please type in ctrl + u to use the universal chat if you need any help .." );
	send_user_private_chat ( user[u].con_id, 3, rtn_str );

	sprintf ( rtn_str,"... Welcome to Project Diaspora! .." );
	send_user_private_chat ( user[u].con_id, 2, rtn_str );
}

void user_purchase_plans(int con, int plan_check)
{
	int u;
	int s, p, pl;
	
	if(plan_check<0) return;
	if(plan_check>=SHIP_MAX) return;
	
	u = user_con_id[con];
	s = user[u].server_id;
	p = user[u].last_planet;
	
	if(user_check_lease_loggedin(u)) return;
	
	//at a real place?
	if(s<0) return;
	if(p<0) return;
	
	if(!user[u].landed) return;
	
	//begin the process
	pl = server[s].planet[p].ship_plan;
	
	//plan check?
	if(server[s].planet[p].ship_plan != plan_check) return;
	
	//already have it?
	if(user[u].has_plan[s][pl]) return;
	
	//cost kills exp?
	if(user[u].money < server[s].planet[p].ship_plan_cost) return;
	if(user[u].exp < server[s].planet[p].ship_plan_exp) return;
	if(user[u].kills < server[s].planet[p].ship_plan_kills) return;
	
	//take money
	user[u].money -= server[s].planet[p].ship_plan_cost;
	
	//give the user
	user[u].has_plan[s][pl] = 1;
	
	//tell the user
	user_send_plan_list(u);
	user_send_money(con);
	
	//db
	game.rewrite_user_db = 1;
}

void user_send_plan_list(int u)
{
	char send_str[1024] = "40,";
	char temp_str[100];
	int s, sh;
	int con;
	
	con = user[u].con_id;
	
	if(con<0) return;
	
	for(s=0;s<MAX_SERVER;s++)
		for(sh=0;sh<SHIP_MAX;sh++)
			if(user[u].has_plan[s][sh])
	{
		sprintf(temp_str, "%d,%d,", s, sh);
		strcat(send_str, temp_str);
	}
	
	send_con_user(con, send_str);
}

int user_at_home_planet(int u)
{
	if(user[u].home_id == user[u].server_id && user[u].home_planet == user[u].last_planet) return 1;
	
	return 0;
}

int has_bad_user_characters(char *message)
{
	int len;
	int i;
	
	len = strlen(message);
	
	for(i=0;i<len;i++)
	{
		if(message[i] == ';') return 1;
	}
	
	return 0;
}

int find_just_any_login(char *login)
{
	char lcase_login[21];
	char lcase_login2[21];
	int i;
	
	login[20] = 0;
	
	if(!login[0]) return -1;
	
	lcase(lcase_login, login);
	
	for ( i=0;i<=user_max;i++ )
	{
		//normal?
		lcase(lcase_login2, user[i].login);
		if(!strcmp(lcase_login, lcase_login2)) return i;
		
		//lease?
		lcase(lcase_login2, user[i].lease_login);
		if(!strcmp(lcase_login, lcase_login2)) return i;
	}
	
	return -1;
}

void user_set_lease_login(int con, char *new_lease_login, char *new_lease_password)
{
	int u;
	int i;
	char rtn_str[1024];
	
	//capping in the name of the lord
	new_lease_login[20] = 0;
	new_lease_password[20] = 0;
	
	u = user_con_id[con];
	
	if(user_check_lease_loggedin(u)) return;
	
	if(!new_lease_login[0] || !new_lease_password[0])
	{
		user[u].lease_login[0] = 0;
		user[u].lease_password[0] = 0;
		
		sprintf ( rtn_str,"... Lease login cleared .." );
		send_user_public_chat ( user[u].con_id, 1, rtn_str );
		
		game.rewrite_user_db = 1;
		return;
	}
	
	//it already used?
	i = find_just_any_login(new_lease_login);
	
	//if a login was found and it isn't us, or is our main login then no
	if(i != -1 && (i != u || str_match_lcase(new_lease_login, user[u].login)))
	{
		sprintf ( rtn_str,"... Login already used .." );
		send_user_public_chat ( user[u].con_id, 1, rtn_str );
		return;
	}
	
	//set it
	strcpy(user[u].lease_login, new_lease_login);
	strcpy(user[u].lease_password, new_lease_password);
	
	sprintf ( rtn_str,"... Lease login set .." );
	send_user_public_chat ( user[u].con_id, 1, rtn_str );
	
	game.rewrite_user_db = 1;
}

int user_check_lease_loggedin(int u)
{
	char rtn_str[1024];
	
	if(user[u].lease_logged_in)
	{
		if(user[u].con_id > -1)
		{
			sprintf ( rtn_str,"... Can not perform action .." );
			send_user_private_chat ( user[u].con_id, 1, rtn_str );
		}
		
		return 1;
	}
	
	return 0;
}

void user_do_lease_time_message(int u)
{
	char rtn_str[1024];
	int days, hours, minutes, seconds;
	int cur_time;
	int dif_time;
	
	if(user[u].lease_logged_in) return;
	if(user[u].con_id < 0) return;
	if(!user[u].lease_login[0]) return;
	if(!user[u].lease_password[0]) return;
	
	cur_time = time(0);
	dif_time = cur_time - user[u].time_lease_logged;
	
	days = dif_time / (60 * 60 * 24);
	dif_time = dif_time % (60 * 60 * 24);
	hours = dif_time / (60 * 60);
	dif_time = dif_time % (60 * 60);
	minutes = dif_time / 60;
	dif_time = dif_time % 60;
	seconds = dif_time;
	
	if(days > 0)
		sprintf ( rtn_str,"... Lease login last used %d day(s) ago by %s ..", days, user[u].user_ip_lease );
	else if(hours > 0)
		sprintf ( rtn_str,"... Lease login last used %d hour(s) ago by %s ..", hours, user[u].user_ip_lease );
	else if(minutes > 0)
		sprintf ( rtn_str,"... Lease login last used %d minute(s) ago by %s ..", minutes, user[u].user_ip_lease );
	else if(seconds > 0)
		sprintf ( rtn_str,"... Lease login last used %d second(s) ago by %s ..", seconds, user[u].user_ip_lease );
	else
		sprintf ( rtn_str,"... Lease login last used %d second(s) ago by %s ..", seconds, user[u].user_ip_lease );
	
	send_user_private_chat ( user[u].con_id, 1, rtn_str );
}

