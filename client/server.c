/*
Author: Mike Bok
Email: freaknigh@hotmail.com
Website: www.nighsoft.net
License and Rights: Full licensing and Rights available at www.nighsoft.net.
If licensing and rights not available at www.nighsoft.net, then all the following apply as bulleted:
-all rights reserved.
-you can not modify source code in this file.
-you can compile and use this source code.
*/

#include "main.h"

void connect_server_process()
{
	char rtn_str[40];

	//no more
	game.server_address[0] = 0;
	
	//yay
	server[game.server_id].set_zone_sun_damages = 0;
	map.zoom_level = 0;

	sprintf ( rtn_str,"1,%d",game.user_id );
	send_con_server ( rtn_str );

}

void disconnect_server_process()
{
	if ( !login.visible && !switch_server.visible && !server_select.visible )
	{
		char message[200] = "... Warning : Disconnected from the sub game server, please log off and reconnect ..";

		add_private_chat ( 1,message );
	}
}

void read_planet_entry ( char *cur_line )
{
	read_planet_db_entry ( server[game.server_id].planet, & ( server[game.server_id].home_class ), cur_line );
}

void read_galaxy_entry ( char *cur_line )
{
	read_galaxy_db_entry ( server[game.server_id].zone, cur_line );
}

void read_gun_entry ( int server_num, char *cur_line )
{
	read_gun_db_entry ( server[server_num].gun, cur_line );
}

void read_missile_entry ( int server_num, char *cur_line )
{
	read_missile_db_entry ( server[server_num].missile, cur_line );
}

void read_ship_entry ( char *cur_line )
{
	read_ship_db_entry ( server[game.server_id].ship, cur_line );
}

void set_good_prices ( int server_number, int planet_number, char *message )
{
	int i, k, point;
	char temp_str[1024];

	//check for fucking around
	if ( planet_number < 0 ) return;
	if ( planet_number >= PLANET_MAX ) return;
	if ( server_number < 0 ) return;
	if ( server_number >= MAX_SERVER ) return;

	i = strlen ( message );

	for ( point=k=0;point<=i && k<10;k++ )
	{
		split ( temp_str,message,',',&point );
		if ( k<10 )
			server[server_number].planet[planet_number].good_price[k] = atoi ( temp_str );
		else
			server[server_number].planet[planet_number].contra_price[k-10] = atoi ( temp_str );
	}
}

void set_good_ammounts ( int server_number, int planet_number, char *message )
{
	int i, k, point;
	char temp_str[1024];

	//check for fucking around
	if ( planet_number < 0 ) return;
	if ( planet_number >= PLANET_MAX ) return;
	if ( server_number < 0 ) return;
	if ( server_number >= MAX_SERVER ) return;

	i = strlen ( message );

	for ( point=k=0;point<=i && k<10;k++ )
	{
		split ( temp_str,message,',',&point );
		server[server_number].planet[planet_number].good_ammount[k] = atoi ( temp_str );
	}
}

void update_planet_good_ammount ( int s, int p, int i, int ammount )
{
	//check for fucking around
	if ( p < 0 ) return;
	if ( p >= PLANET_MAX ) return;
	if ( s < 0 ) return;
	if ( s >= MAX_SERVER ) return;

	//set it
	server[s].planet[p].good_ammount[i] = ammount;

	//now see if we have to display it
	if ( trade.visible && trade.good_selected == i && !trade.info_visible )
		trade_update_good_ammount();
}

void set_sector_goods ( int s, int z, char *message )
{
	int s_i, i, point = 0, len;
	char temp_str[1025];

	//set stuff
	len = strlen ( message );
	s_i = game.server_id;

	//check for fucking around
	if ( s < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z < 0 ) return;
	if ( z >= ZONE_MAX ) return;
	if ( s_i < 0 ) return;
	if ( s_i >= MAX_SERVER ) return;

	for ( i=0;i<10 && point <= len; i++ )
	{
		//take it
		split ( temp_str,message,',',&point );

		//fuck it
		server[s_i].zone[z].sector[s].good_amount[i] = atoi ( temp_str );

		//draw this good if we are suposed to
		space_redraw_good_number_if_required ( i,1 );
	}

	if ( update_space_good_buttons() )
		draw_space_good_buttons();
}

void set_sector_contras ( int s, int z, char *message )
{
	int s_i, i, point = 0, len;
	char temp_str[1025];

	//set stuff
	len = strlen ( message );
	s_i = game.server_id;

	//check for fucking around
	if ( s < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z < 0 ) return;
	if ( z >= ZONE_MAX ) return;
	if ( s_i < 0 ) return;
	if ( s_i >= MAX_SERVER ) return;

	for ( i=0;i<10 && point <= len; i++ )
	{
		//take it
		split ( temp_str,message,',',&point );

		//fuck it
		server[s_i].zone[z].sector[s].contra_amount[i] = atoi ( temp_str );

		//draw this good if we are suposed to
		space_redraw_good_number_if_required ( i,1 );
	}

	if ( update_space_good_buttons() )
		draw_space_good_buttons();
}

void set_sector_good ( int s, int z, int g, int amount )
{
	int s_i;

	//set stuff
	s_i = game.server_id;

	//check for fucking around
	if ( g < 0 ) return;
	if ( g >= 10 ) return;
	if ( s < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z < 0 ) return;
	if ( z >= ZONE_MAX ) return;
	if ( s_i < 0 ) return;
	if ( s_i >= MAX_SERVER ) return;

	//fuck it
	server[s_i].zone[z].sector[s].good_amount[g] = amount;

	//draw this good if we are suposed to
	space_redraw_good_number_if_required ( g,1 );

	if ( update_space_good_buttons() )
		draw_space_good_buttons();
}

void set_sector_contra ( int s, int z, int g, int amount )
{
	int s_i;

	//set stuff
	s_i = game.server_id;

	//check for fucking around
	if ( g < 0 ) return;
	if ( g >= 10 ) return;
	if ( s < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z < 0 ) return;
	if ( z >= ZONE_MAX ) return;
	if ( s_i < 0 ) return;
	if ( s_i >= MAX_SERVER ) return;

	//fuck it
	server[s_i].zone[z].sector[s].contra_amount[g] = amount;

	//draw this good if we are suposed to
	space_redraw_good_number_if_required ( g,1 );

	if ( update_space_good_buttons() )
		draw_space_good_buttons();
}

void server_set_sat_info ( char *message )
{
	int point = 0, do_repair = 0, i, p;
	char temp_str[1025];
	struct server_satellite_db old_sat;

	if ( sat_editor.visible ) old_sat = planet_sat.sat;

	//we set our own p here
	split ( temp_str, message, ',', &point );
	p = atoi ( temp_str );

	if ( p<0 ) return;
	if ( p>=PLANET_MAX ) return;

	split ( temp_str, message, ',', &point );
	planet_sat.sat.level = atoi ( temp_str );

	split ( temp_str, message, ',', &point );
	temp_str[20] = 0;
	strcpy ( planet_sat.sat.guild_name, temp_str );

	split ( temp_str, message, ',', &point );
	planet_sat.sat.money = atoi ( temp_str );

	split ( temp_str, message, ',', &point );
	planet_sat.sat.money_collected = atoi ( temp_str );

	for ( i=0;i<SAT_LEVEL_MAX * SAT_GUN_INC;i++ )
	{
		split ( temp_str, message, ',', &point );
		planet_sat.sat.gun[i] = atoi ( temp_str );

		split ( temp_str, message, ',', &point );
		planet_sat.sat.gun_destroyed[i] = atoi ( temp_str );

		//check
		if ( planet_sat.sat.gun[i]<-1 ) planet_sat.sat.gun[i] = -1;
		if ( planet_sat.sat.gun[i]>=GUN_MAX ) planet_sat.sat.gun[i] = -1;
	}
	for ( i=0;i<SAT_LEVEL_MAX * SAT_MISSILE_INC;i++ )
	{
		split ( temp_str, message, ',', &point );
		planet_sat.sat.missile[i] = atoi ( temp_str );

		split ( temp_str, message, ',', &point );
		planet_sat.sat.missile_left[i] = atoi ( temp_str );

		//check
		if ( planet_sat.sat.missile[i]<-1 ) planet_sat.sat.missile[i] = -1;
		if ( planet_sat.sat.missile[i]>=MISSILE_MAX ) planet_sat.sat.missile[i] = -1;
	}

	//hull
	split ( temp_str, message, ',', &point );
	planet_sat.sat.hull = atoi ( temp_str );

	if ( sat_editor.visible ) //ok lets find the changes and display them properly
	{
		int redraw_hull, redraw_money, redraw_viewing, redraw_info;

		redraw_hull = 0;
		redraw_money = 0;
		redraw_viewing = 0;
		redraw_info = 0;

		if ( planet_sat.sat.hull != old_sat.hull ) redraw_hull = 1;

		if ( planet_sat.sat.money != old_sat.money ) redraw_money = 1;

		if ( planet_sat.sat.money_collected != old_sat.money_collected ) redraw_info = 1;

		if ( planet_sat.sat.level != old_sat.level ) {redraw_info = 1; redraw_viewing = 1;}

		if ( sat_editor.viewing_guns )
			for ( i=0;i<planet_sat.sat.level * SAT_GUN_INC;i++ )
				if ( planet_sat.sat.gun[i] != old_sat.gun[i] || planet_sat.sat.gun_destroyed[i] != old_sat.gun_destroyed[i] )
				{
					redraw_viewing = 1;
					break;
				}

		if ( sat_editor.viewing_missiles )
			for ( i=0;i<planet_sat.sat.level * SAT_MISSILE_INC;i++ )
				if ( planet_sat.sat.missile[i] != old_sat.missile[i] || planet_sat.sat.missile_left[i] != old_sat.missile_left[i] )
				{
					redraw_viewing = 1;
					break;
				}

		if ( redraw_hull ) sat_draw_hull();
		if ( redraw_money ) sat_draw_money_label();
		if ( redraw_viewing ) {sat_draw_weapon_icons(); sat_draw_weapon_selected();}
		if ( redraw_info ) {sat_draw_info_labels(); sat_draw_purchase_or_upgrade_button();}

		if ( redraw_hull || redraw_money || redraw_viewing || redraw_info ) sdl_flip_mutex();

	}
}

void server_set_sat_hull ( int amount )
{
	planet_sat.sat.hull = amount;

	if ( !sat_editor.visible ) return;

	sat_draw_hull();
	sdl_flip_mutex();
}

void server_set_sat_money ( int amount )
{
	planet_sat.sat.money = amount;

	if ( !sat_editor.visible ) return;

	sat_draw_money_label();
	sdl_flip_mutex();
}

void server_set_gun_destroyed ( int g )
{
	int sh_sel;

	//eee
	sh_sel = game.ship_sel;

	//check my balls...
	if ( g<0 ) return;
	if ( g>=16 ) return;

	//set my balls....
	game.ship[sh_sel].gun_destroyed[g] = 1;

	//now for some drawing
	space_draw_gun_destroyed ( g );

	//shittt might want to know
	space_set_hit_actual_max();

	set_average_gun_speed(); //in case you plan on useing them guns
}

void server_clear_out_planet_trade_disasters()
{
	int p, g, is_low, i, s;

	//setters
	s = game.server_id;

	for ( p=0;p<PLANET_MAX;p++ )
		if ( server[s].planet[p].name[0] && !server[s].planet[p].is_gate )
			server[s].planet[p].trade_disaster_max = 0;
}

void server_add_planet_trade_disaster ( int p, int g, int is_low, int i )
{
	int d, s;

	//setters
	s = game.server_id;

	//checks
	if ( p<0 ) return;
	if ( g<0 ) return;
	if ( is_low<0 ) return;
	if ( i<0 ) return;
	if ( p>=PLANET_MAX ) return;
	if ( g>=10 ) return;
	if ( is_low>=2 ) return;
	if ( i>=MAX_TRADE_DISASTER ) return;

	d = server[s].planet[p].trade_disaster_max;

	if ( d >= PLANET_TRADE_DISASTER_MAX ); //already got the max set?

	server[s].planet[p].trade_disaster[d].g = g;
	server[s].planet[p].trade_disaster[d].is_low = is_low;
	server[s].planet[p].trade_disaster[d].i = i;

	server[s].planet[p].trade_disaster_max++; //set higher
}

void set_sun_damages ( struct server_zone_db *the_zone )
{
	int z, s, s2;

	//first clear the damages
	for ( z=0;z<ZONE_MAX;z++ )
		for ( s=0;s<SECTOR_MAX;s++ )
			the_zone[z].sector[s].sun_damage = 0;

	//now set the damages
	for ( z=0;z<ZONE_MAX;z++ )
		for ( s=0;s<SECTOR_MAX;s++ )
			if ( the_zone[z].sector[s].x && the_zone[z].sector[s].is_sun )
				for ( s2=0;s2<SECTOR_MAX;s2++ )
				{
					int dist;

					//checks
					if ( !the_zone[z].sector[s2].x ) continue;

					//oooO
					dist = distance ( the_zone[z].sector[s].x,the_zone[z].sector[s].y,the_zone[z].sector[s2].x,the_zone[z].sector[s2].y );

					if ( dist > SUN_RADIUS ) continue;

					the_zone[z].sector[s2].sun_damage += SUN_DAMAGE * ( SUN_RADIUS - dist ) / SUN_RADIUS;
				}
}

void set_victom_shield_level ( int shield_level )
{
	int u;

	//set
	u = space.engaging;

	//check
	if ( u<0 ) return;

	//pissing around?
	if ( shield_level<0 ) shield_level = 0;
	if ( shield_level>4 ) shield_level = 4;

	//set
	space.user[u].shield_level = shield_level;
	space.user[u].refresh_shield = 1;
}

void set_victom_jump_status ( int jump_status )
{
	int u;

	//set
	u = space.engaging;

	//check
	if ( u<0 ) return;

	space.user[u].jump_status = jump_status;
}

void dock_jump_bar()
{
	double the_time;

	//do the dock
	space.jump_time_start += JUMP_DOCK_TIME;

	the_time = current_time();

	//do a check
	if ( space.jump_time_start > the_time )
		space.jump_time_start = the_time;

	//clean up
	space.clear_jump_bar = 1;
}

void server_send_ask_goodprices ( char *planet_name )
{
	char rtn_str[2000];

	sprintf ( rtn_str,"19,%s", planet_name );
	send_con_server ( rtn_str );
}

void server_ping_request()
{
	char send_str[20] = "20,";

	send_con_server ( send_str );
}

void server_set_sector_has_player(int z, int s, int has_player)
{
	int s_id;
	
	s_id = game.server_id;
	
	if ( s < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z < 0 ) return;
	if ( z >= ZONE_MAX ) return;
	if ( s_id < 0 ) return;
	if ( s_id >= MAX_SERVER ) return;
	
	server[s_id].zone[z].sector[s].has_player = has_player;
	
	//should fix a bug
	//SDL_LockMutex ( space_mutex );
	
	if(map.visible && z == game.zone)
	{
		map_redraw();
		sdl_flip_mutex();
	}
	
	//SDL_UnlockMutex ( space_mutex );
}

void server_set_sector_has_guild_member(int z, int s, int has_guild_member)
{
	int s_id;
	
	s_id = game.server_id;
	
	if ( s < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z < 0 ) return;
	if ( z >= ZONE_MAX ) return;
	if ( s_id < 0 ) return;
	if ( s_id >= MAX_SERVER ) return;
	
	server[s_id].zone[z].sector[s].has_guild_member = has_guild_member;
	
	//should fix a bug
	//SDL_LockMutex ( space_mutex );
	
	if(map.visible && z == game.zone)
	{
		map_redraw();
		sdl_flip_mutex();
	}
	
	//SDL_UnlockMutex ( space_mutex );
}

void server_clear_has_guild_member_sectors()
{
	int s_id;
	int z,s;
	
	s_id = game.server_id;
	
	if ( s_id < 0 ) return;
	if ( s_id >= MAX_SERVER ) return;
	
	for ( z=0;z<ZONE_MAX;z++ )
		for ( s=0;s<SECTOR_MAX;s++ )
			server[s_id].zone[z].sector[s].has_guild_member = 0;
	
	//SDL_LockMutex ( space_mutex );
	
	if(map.visible && z == game.zone)
	{
		map_redraw();
		sdl_flip_mutex();
	}
	
	//SDL_UnlockMutex ( space_mutex );
}
