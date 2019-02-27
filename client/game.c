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

#ifdef _WIN32
	#include "win_clipboard.h"
#endif

void draw_space_user_ship ( int i );
char *good_description ( int good_number, char *dest );

void clear_user_misc ( int i )
{
	int k;

	for ( k=0;k<16;k++ )
	{
		game.ship[i].gun[k] = -1;
		game.ship[i].gun_id[k] = -1;
		game.ship[i].missile[k] = -1;
		game.ship[i].missile_id[k] = -1;
		game.ship[i].misc[k] = -1;
		game.ship[i].misc_id[k] = -1;
	}
}

void set_user_misc() //this func applys misc-pod changes
{
	int i, temp_misc;

	game.ship[game.ship_sel].cargo = game.ship[game.ship_sel].cargo_base; //set back to initials
	game.ship[game.ship_sel].hull_max = game.ship[game.ship_sel].hull_base;
	game.ship[game.ship_sel].speed = game.ship[game.ship_sel].speed_base;
	game.ship[game.ship_sel].shield = game.ship[game.ship_sel].shield_base;

	for ( i=0;i<game.ship[game.ship_sel].misc_ammount;i++ )
	{
		temp_misc = game.ship[game.ship_sel].misc[i];

		while ( temp_misc > 3 )
			temp_misc -= 4;

		if ( temp_misc < 0 ) //skip this entry if invalid
			continue;

		switch ( temp_misc )
		{
			case 0: //hull
				game.ship[game.ship_sel].hull_max += misc_pod_value ( game.ship[game.ship_sel].misc[i] );
				break;
			case 1: //shield
				game.ship[game.ship_sel].shield += misc_pod_value ( game.ship[game.ship_sel].misc[i] );
				break;
			case 2: //cargo
				game.ship[game.ship_sel].cargo += misc_pod_value ( game.ship[game.ship_sel].misc[i] );
				break;
			case 3: //jump
				game.ship[game.ship_sel].speed += misc_pod_value ( game.ship[game.ship_sel].misc[i] );
				break;
		}
	}

	//set jump_time for the launched screen
	set_user_jump_time();
}

void set_user_jump_time()
{
	int sh_sel;

	sh_sel = game.ship_sel;

	space.jump_time = ship_total_seconds ( game.ship[sh_sel].speed, game.ship[sh_sel].kind );
}

void add_chat ( int text_color, char *message )
{
	if ( check_ignore_normal ( text_color, message ) ) return;

	if ( bar.visible )
	{
		add_display_box ( screen, &bar.display_box[0], text_color, message );
		sdl_flip_mutex();
	}

	if ( space.visible )
	{
		add_display_box ( screen, &space.display_box[0], text_color, message );
		sdl_flip_mutex();
	}

	if ( map.visible )
	{
		add_display_box ( NULL, &space.display_box[0], text_color, message );
		sdl_flip_mutex();
	}
}

void add_private_chat ( int text_color, char *message )
{
	if ( check_ignore_private ( text_color, message ) ) return;

	if ( bar.visible )
	{
		add_display_box ( screen, &bar.display_box[1], text_color, message );
		add_display_box ( NULL, &space.display_box[1], text_color, message );
		sdl_flip_mutex();
	}
	else if ( space.visible )
	{
		add_display_box ( screen, &space.display_box[1], text_color, message );
		add_display_box ( NULL, &bar.display_box[1], text_color, message );
		sdl_flip_mutex();
	}
	else //add to but dont write to screen
	{
		add_display_box ( NULL, &bar.display_box[1], text_color, message );
		add_display_box ( NULL, &space.display_box[1], text_color, message );
	}
}

void clear_space_user_struct ( struct space_user *the_user )
{
	int i;

	the_user->ship_id 	= -1;
	the_user->ship_kind 	= -1;
	the_user->ship_type 	= -1;
	the_user->ship_name[0] 	= '\0';
	the_user->username[0] 	= '\0';
	the_user->guild[0] 	= '\0';
	the_user->destroy 	= 0;
	the_user->hull_percent 	= 100;
	the_user->attacking 	= -1;
	the_user->shield_level  = 0;
	the_user->refresh_shield= 1;
	the_user->jump_status 	= 0;
	the_user->sos		= 0;

	for ( i=0;i<16;i++ )
		the_user->missile_firing[i] = 0;
}

void load_sector ( int new_zone, int new_sector, char *user_list )
{
	int i, point = 0, user_len, user_limit, temp_loc;
	char temp_str[1025];
	struct space_user temp_usr;

	if ( new_zone < 0 || new_zone >= ZONE_MAX ) return; //client killer!
	if ( new_sector < 0 || new_sector >= SECTOR_MAX ) return; //client killer!

	//ehh sound
	if ( !launching.visible )
		play_sound ( sound_jump );

	//god this is anouying
	if ( !space.viewing_cargo )
	{
		space_clear_middle_box();
		space.show_ship = 1;
	}

	//get these buttons ready
	space.draw_goods_grey = 1;
	space.draw_contra_grey = 1;

	//remove any users that may be selected in space
	space.user_sel = -1;

	//we also arent attacking anyone yet
	space.engaging = -1;

	//just doesnt need to be done
	space.clear_jump_bar = 0;

	//missiles shouldnt be firing...
	for ( i=0;i<16;i++ )
		space.missile_firing[i] = 0;

	//check if sector has a planet
	if ( server[game.server_id].zone[new_zone].sector[new_sector].planet > -1 )
	{
		space.at_planet = 1;
		user_limit = 39;
	}
	else
	{
		space.at_planet = 0;
		user_limit = 40;
	}

	//clear user information
	space.user_ammount = 0;
	for ( i=0;i<40;i++ )
		clear_space_user_struct ( & ( space.user[i] ) );

	//load user information
	user_len = strlen ( user_list );
	for ( i=0;point<user_len && i<user_limit;i++ )
	{
		clear_space_user_struct ( &temp_usr );

		//ship id,type,kind,class_name,user_name,user_guild
		split ( temp_str, user_list, ',', &point );
		temp_usr.ship_id = atoi ( temp_str );
		split ( temp_str, user_list, ',', &point );
		temp_usr.ship_type = atoi ( temp_str );
		split ( temp_str, user_list, ',', &point );
		temp_usr.ship_kind = atoi ( temp_str );
		split ( temp_str, user_list, ',', &point );
		temp_str[20] = '\0';
		strcpy ( temp_usr.ship_name, temp_str );
		split ( temp_str, user_list, ',', &point );
		temp_str[20] = '\0';
		strcpy ( temp_usr.username, temp_str );
		split ( temp_str, user_list, ',', &point );
		temp_str[20] = '\0';
		strcpy ( temp_usr.guild, temp_str );

		//check if field type
		if ( temp_usr.ship_type == -2 )
		{
			temp_usr.ship_type = temp_usr.ship_kind + SHIP_MAX;
			temp_usr.ship_kind = 0;
		}

		//check if valid
		if ( temp_usr.ship_id < -2 || temp_usr.ship_id >= MAX_SERVER ) continue;
		if ( temp_usr.ship_type < 0 || temp_usr.ship_type >= SHIP_MAX + FIELD_TYPE_MAX ) continue;
		if ( temp_usr.ship_kind < 0 || temp_usr.ship_kind >= 8 ) continue;

		//check for special per designed "ships"
		if ( temp_usr.ship_id == -1 && ( temp_usr.ship_kind >= SPECIAL_FSHIP_MAX || temp_usr.ship_kind < 0 ) ) continue;

		//runes
		if ( temp_usr.ship_id == -2 && ( temp_usr.ship_type > 3 ) ) continue;

		//set this number
		space.user_ammount++;

		//give random "image"
		temp_usr.image = rand() % 5;

		//is sos?
		if ( check_sos_raw ( temp_usr.username ) > -1 )
			temp_usr.sos = 1;

		//stuff that needs set...
		temp_usr.destroy 	= 0;
		temp_usr.hull_percent 	= 100;
		temp_usr.attacking 	= -1;
		temp_usr.attack_x = 0;
		temp_usr.attack_y = 0;
		temp_usr.attack_position = 0;

		//find random place to put.
		while ( 1 ) //loop until found
		{
			if ( space.at_planet )
				temp_loc = ( rand() % 39 ) + 1;
			else
				temp_loc = rand() % 40;

			if ( space.user[temp_loc].ship_id == -1  && !space_user_is_special ( temp_loc ) )
			{
				space.user[temp_loc] = temp_usr;
				break;
			}
		}
	}

	//load loc and posibly planet
	load_sector_loc ( game.server_id, new_zone, new_sector );

	//place any users on the loc
	space.redraw_loc = 1;

	//reset some things...
	space.jump_ok = 0;
	map.scan_sector = -1;
	space.jump_time_start = current_time();

	//set this
	game.sector = new_sector;
	game.zone = new_zone;

	//reset this hell
	map.shift_x = 0;
	map.shift_y = 0;
	map.do_shift = 0;

	//set sector links
	map_set_local_sector_info();

	//display a little warning
	strcpy ( temp_str,"... Warning :  Entering sun's radius .." );
	if ( server[game.server_id].zone[game.zone].sector[game.sector].sun_damage )
		add_chat ( 1,temp_str );

	load_space();
}

void load_sector_locs_safe ( int s_id, int z )
{
	int i;
	char temp_str[1025];

	if ( !server[s_id].zone[z].loc_safe[0] ) //if image not loaded
		for ( i=0;i<LOC_MAX;i++ ) //then load em up
		{
			sprintf ( temp_str, "server/%s/backs/locs/safe%d_%d.jpg", server[s_id].servername, z, i + 1 );
			server[s_id].zone[z].loc_safe[i] = IMG_Load ( temp_str );

			//check if we hit the end of the line
			if ( !server[s_id].zone[z].loc_safe[i] )
			{
				server[s_id].zone[z].loc_safe_max = i;
				break;
			}
		}

	//still not loaded... nif time
	if ( !server[s_id].zone[z].loc_safe[0] )
	{
		if ( !game.nif_image.no_loc_safe_img )
			game.nif_image.no_loc_safe_img = IMG_Load ( "graphics/nif/nif_safe_loc.jpg" );

		server[s_id].zone[z].loc_safe[0] = game.nif_image.no_loc_safe_img;
		server[s_id].zone[z].loc_safe_max = 1;
	}
}

void load_sector_locs ( int s_id, int z )
{
	int i;
	char temp_str[1025];

	if ( !server[s_id].zone[z].loc[0] ) //if image not loaded
		for ( i=0;i<LOC_MAX;i++ ) //then load em up
		{
			sprintf ( temp_str, "server/%s/backs/locs/loc%d_%d.jpg", server[s_id].servername, z, i + 1 );
			server[s_id].zone[z].loc[i] = IMG_Load ( temp_str );

			//check if we hit the end of the line
			if ( !server[s_id].zone[z].loc[i] )
			{
				server[s_id].zone[z].loc_max = i;
				break;
			}
		}

	//still not loaded... nif time
	if ( !server[s_id].zone[z].loc[0] )
	{
		if ( !game.nif_image.no_loc_img )
			game.nif_image.no_loc_img = IMG_Load ( "graphics/nif/nif_loc.jpg" );

		server[s_id].zone[z].loc[0] = game.nif_image.no_loc_img;
		server[s_id].zone[z].loc_max = 1;
	}
}

void draw_sector_loc ( int s_id, int z, int s, SDL_Surface *the_loc )
{
	char temp_str[1025];

	SDL_BlitSurface ( the_loc, NULL, space.loc, NULL );

	if ( space.at_planet ) //check if we need to put the planet image on top...
	{
		space.last_loc_planet = server[s_id].zone[z].sector[s].planet;

		space.was_sun = 0;

		if ( !server[s_id].planet[space.last_loc_planet].loc_img ) // not found so load it
		{
			sprintf ( temp_str, "server/%s/backs/locs/p%d_1.gif", server[s_id].servername, space.last_loc_planet );
			server[s_id].planet[space.last_loc_planet].loc_img = IMG_Load ( temp_str );
		}

		if ( !server[s_id].planet[space.last_loc_planet].loc_img ) // still not found so load it
		{
			if ( !game.nif_image.no_loc_p_img )
				game.nif_image.no_loc_p_img = IMG_Load ( "graphics/nif/nif_p.gif" );

			SDL_BlitSurface ( game.nif_image.no_loc_p_img, NULL, space.loc, NULL );
		}
		else //use the real image
		{
			SDL_BlitSurface ( server[s_id].planet[space.last_loc_planet].loc_img, NULL, space.loc, NULL );
		}
	}
	else
	{
		if ( server[s_id].zone[z].sector[s].is_sun )
		{
			SDL_BlitSurface ( space.loc_sun, NULL, space.loc, NULL );
			space.was_sun = 1;
		}
		else
		{
			space.was_sun = 0;
		}

		space.last_loc_planet = -1;
	}
}

void load_sector_loc ( int s_id, int z, int s )
{
	int i, temp_loc;
	char temp_str[1025];

	if ( server[s_id].zone[z].sector[s].shield ) //if zone is a safe area
	{
		load_sector_locs_safe ( s_id, z );

		temp_loc = rand() % ( server[s_id].zone[z].loc_safe_max );

		//check if useing the new loc is required

		if ( server[s_id].zone[z].sector[s].is_sun != space.was_sun || server[s_id].zone[z].sector[s].planet != space.last_loc_planet || z != space.last_loc_zone || !space.last_loc_safe )
		{
			draw_sector_loc ( s_id, z, s, server[s_id].zone[z].loc_safe[temp_loc] );

			space.last_loc_number = temp_loc;
			space.last_loc_safe = 1;
			space.last_loc_zone = z;
		}
		else //not required so we'll roll the dice.
		{
			if ( ! ( rand() % 3 ) && space.last_loc_number != temp_loc ) //1 in 3
			{
				draw_sector_loc ( s_id, z, s, server[s_id].zone[z].loc_safe[temp_loc] );

				space.last_loc_number = temp_loc;
				space.last_loc_safe = 1;
				space.last_loc_zone = z;
			}
		}

	}
	else // not a safe area...
	{
		load_sector_locs ( s_id, z );

		temp_loc = rand() % ( server[s_id].zone[z].loc_max );

		//check if useing the new loc is required

		if ( server[s_id].zone[z].sector[s].is_sun != space.was_sun || server[s_id].zone[z].sector[s].planet != space.last_loc_planet || z != space.last_loc_zone || space.last_loc_safe )
		{
			draw_sector_loc ( s_id, z, s, server[s_id].zone[z].loc[temp_loc] );

			space.last_loc_number = temp_loc;
			space.last_loc_safe = 0;
			space.last_loc_zone = z;
		}
		else //not required so we'll roll the dice.
		{
			if ( ! ( rand() % 3 ) && space.last_loc_number != temp_loc ) //1 in 3
			{
				draw_sector_loc ( s_id, z, s, server[s_id].zone[z].loc[temp_loc] );

				space.last_loc_number = temp_loc;
				space.last_loc_safe = 0;
				space.last_loc_zone = z;
			}
		}
	}

}

void add_to_sector ( char *new_user )
{
	int point = 0, temp_loc;
	char temp_str[1025], temp_filename[200];
	struct space_user temp_usr;
	SDL_Rect img_location;
	SDL_Rect destination;

	if ( space.at_planet && space.user_ammount >= 39 ) return; //already have max users
	if ( !space.at_planet && space.user_ammount >= 40 ) return;

	clear_space_user_struct ( &temp_usr );

	//ship id,type,kind,class_name,user_name,user_guild
	split ( temp_str, new_user, ',', &point );
	temp_usr.ship_id = atoi ( temp_str );
	split ( temp_str, new_user, ',', &point );
	temp_usr.ship_type = atoi ( temp_str );
	split ( temp_str, new_user, ',', &point );
	temp_usr.ship_kind = atoi ( temp_str );
	split ( temp_str, new_user, ',', &point );
	temp_str[20] = '\0';
	strcpy ( temp_usr.ship_name, temp_str );
	split ( temp_str, new_user, ',', &point );
	temp_str[20] = '\0';
	strcpy ( temp_usr.username, temp_str );
	split ( temp_str, new_user, ',', &point );
	temp_str[20] = '\0';
	strcpy ( temp_usr.guild, temp_str );

	//check if field type
	if ( temp_usr.ship_type == -2 )
	{
		temp_usr.ship_type = temp_usr.ship_kind + SHIP_MAX;
		temp_usr.ship_kind = 0;
	}

	//check if valid
	if ( temp_usr.ship_id < -1 || temp_usr.ship_id >= MAX_SERVER ) return;
	if ( temp_usr.ship_type < 0 || temp_usr.ship_type >= SHIP_MAX + FIELD_TYPE_MAX ) return;
	if ( temp_usr.ship_kind < 0 || temp_usr.ship_kind >= 8 ) return;

	//check for special per designed "ships"
	if ( temp_usr.ship_id == -1 && ( temp_usr.ship_kind >= SPECIAL_FSHIP_MAX || temp_usr.ship_kind < 0 ) ) return;

	//is sos?
	if ( check_sos_raw ( temp_usr.username ) > -1 )
		temp_usr.sos = 1;

	//give random "image"
	temp_usr.image = ( rand() % 5 );

	//were not destroying her yet so
	temp_usr.destroy = 0;
	temp_usr.hull_percent = 100;

	//they aren't attacking yet...
	temp_usr.attacking = -1;
	temp_usr.attack_x = 0;
	temp_usr.attack_y = 0;
	temp_usr.attack_position = 0;


	//find random place to put.
	while ( 1 ) //loop until found
	{
		if ( space.at_planet )
			temp_loc = ( rand() % 39 ) + 1;
		else
			temp_loc = rand() % 40;

		if ( space.user[temp_loc].ship_id == -1 && !space_user_is_special ( temp_loc ) )
		{
			//now add this user

			//stop the space_thread stuff
			//SDL_LockMutex ( space_mutex );

			//set this number
			space.user_ammount++;

			space.user[temp_loc] = temp_usr;

			//draw em'
			draw_space_user_ship ( temp_loc );

			//put loc on backround and fresh screen
			destination.x = 97;
			destination.y = 65;

			if ( space.visible )
			{
				SDL_BlitSurface ( space.temp_loc, NULL, screen, &destination );
				sdl_flip_mutex();
			}

			//SDL_UnlockMutex ( space_mutex );

			break;
		}
	}

}

void remove_from_sector ( char *gone_user )
{
	int i;
	SDL_Rect destination, user_spot;

	//find this suposed user
	for ( i=0;i<40;i++ )
		if ( str_match ( gone_user, space.user[i].username ) )
		{
			//stop the space_thread stuff
			//SDL_LockMutex ( space_mutex );

			//stop attacking
			if ( i == space.engaging ) space_disengage_user();

			//well our little girlie was found, that fine little girl that she is
			//now begon! you sweet thing,
			clear_space_user_struct ( & ( space.user[i] ) );

			//check if this user is selected remove if so
			if ( i == space.user_sel )
			{
				space.user_sel = -1;

				//and clear those little boxes like engage/chat/transfer
				SDL_FillRect ( screen, &space.button[45], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
				SDL_FillRect ( screen, &space.button[46], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
				SDL_FillRect ( screen, &space.button[47], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
			}

			//clear that bit
			user_spot = space.button[i];
			user_spot.x -= 97;
			user_spot.y -= 65;
			user_spot.w = 60;
			user_spot.h = 50;
			SDL_BlitSurface ( space.loc, &user_spot, space.temp_loc, &user_spot );

			destination.x = 97;
			destination.y = 65;

			if ( space.visible )
			{
				SDL_BlitSurface ( space.temp_loc, NULL, screen, &destination );
				sdl_flip_mutex();
			}

			space.user_ammount--;
			if ( space.user_ammount<0 )
				space.user_ammount = 0;

			//SDL_UnlockMutex ( space_mutex );
		}
}

void space_user_hit_us ( char *their_name )
{
	//force their fire on us

	//play a tune
	play_sound ( sound_impact[rand() % 5] );
}

int space_user_is_special ( int u_s )
{
	return ( space.user[u_s].ship_id == -1 && space.user[u_s].ship_kind > -1 && space.user[u_s].ship_kind < SPECIAL_FSHIP_MAX );
}

void space_select_user_to_scan ( int u )
{
	if ( space.user[u].destroy ) return; //dont select dieing user

	if ( space.user[u].ship_id != -1 || space_user_is_special ( u ) )
		set_text_box_text ( NULL, &map.text_box, space.user[u].username );
}

void space_select_user ( int user_selected )
{
	char hover_str[500], temp_str[21];

	if ( space.user_sel == user_selected ) return;
	if ( space.user[user_selected].destroy ) return; //dont select dieing user

	//check if anyone is there
	if ( space.user[user_selected].ship_id != -1 || space_user_is_special ( user_selected ) )
	{
		//check if we need to clear the redraw the newly unselected
		if ( space.user_sel != -1 )
		{
			SDL_Rect user_spot;

			//clear this area
			user_spot = space.button[space.user_sel];
			user_spot.x -= 97;
			user_spot.y -= 65;
			if ( space.visible )
				SDL_BlitSurface ( space.temp_loc, &user_spot, screen, &space.button[space.user_sel] );
		}

		//draw in the buttons
		if ( space.engaging == user_selected )
			SDL_BlitSurface ( space.disengage_img, NULL, screen, &space.button[45] );
		else
			SDL_BlitSurface ( space.engage_img, NULL, screen, &space.button[45] );
		SDL_BlitSurface ( space.chat_img, NULL, screen, &space.button[46] );
		SDL_BlitSurface ( space.trade_img, NULL, screen, &space.button[47] );

		space.user_sel = user_selected;
		space.selection_box_i = 0;
		space.transferring = 0;

		//draw username if needed
		if ( space.motion_sel != user_selected )
		{
			space.motion_sel = user_selected;
			space.info_label_mode = -1;
			if ( !space_user_is_special ( user_selected ) )
				sprintf ( hover_str, "%s : %s %s :: %s", space.user[space.user_sel].username, space.user[space.user_sel].ship_name, ship_name ( space.user[space.user_sel].ship_kind, temp_str ), space.user[space.user_sel].guild );
			else if ( space.user[space.user_sel].ship_kind == 0 || space.user[space.user_sel].ship_kind == 1 )
				sprintf ( hover_str, "%s : Defense System :: %s", space.user[space.user_sel].username, space.user[space.user_sel].guild );

			draw_label ( screen, hover_str, &space.info_label, 0, 230, 0 );
		}

		play_sound ( sound_button );
	}
}

void space_engage_user()
{
	SDL_Rect box_fill;
	char user_attacking[24] = "9,";
	char no_gun_warning[300] = "... No Weapons Equiped ..";
	char in_field_warning[300] = "... Warning :  Weapon Damage Effected by Sector Field ..";

	if ( space.engaging >= 0 )
	{
		int was_engaging;

		was_engaging = space.engaging;

		space_disengage_user();

		if ( was_engaging == space.user_sel )
			return; //don't reengage...
	}

	//check if user has any guns at all
	if ( !ship_has_weapons ( game.ship_sel ) )
	{
		add_chat ( 1,no_gun_warning );

		return;
	}

	//force field?
	if ( server[game.server_id].zone[game.zone].sector[game.sector].shield && !str_match ( space.user[space.user_sel].username,"Satellite" ) )
	{
		add_chat ( 1,in_field_warning );
	}

	//engage the user
	space.engaging = space.user_sel;
	strcat ( user_attacking,space.user[space.engaging].username );

	//tell the server
	send_con_server ( user_attacking );

	//black out the area where the guns go
	box_fill.y = 277;
	box_fill.x = 215;
	box_fill.w = 165;
	box_fill.h = 109;
	SDL_FillRect ( screen, &box_fill, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

	//draw in the pictures
	space_draw_weapons();

	//don't want this anymore
	space_clear_middle_box();

	//change engage to disengage
	SDL_BlitSurface ( space.disengage_img, NULL, screen, &space.button[45] );

	sdl_flip_mutex();
}

void space_toggle_ship_sector()
{
	//toggle it
	if ( space.view_sector_goods )
	{
		space.view_sector_goods = 0;
		space.view_cargo_sector = 0;
	}
	else
	{
		space.view_sector_goods = 1;
		space.view_cargo_sector = 1;
	}

	//draw it
	if ( space.view_sector_goods )
		SDL_BlitSurface ( space.viewing_sector, NULL, screen, &space.button[48] );
	else
		SDL_BlitSurface ( space.viewing_ship, NULL, screen, &space.button[48] );

	//redraw good box?
	space_draw_good_box_if_suposed_to();

	sdl_flip_mutex();
}

void space_draw_gun_destroyed ( int g )
{
	//got the gun?
	if ( game.ship[game.ship_sel].gun[g] < 0 ) return;
	if ( game.ship[game.ship_sel].gun_id[g] < 0 ) return;

	if ( space.visible && !space.weapon_viewing && space.engaging > -1 )
	{
		rectangleRGBA ( screen, space.button[g + 53].x, space.button[g + 53].y, space.button[g + 53].x + 33, space.button[g + 53].y + 23, 255, 0 , 0, 255 );
		sdl_flip_mutex();
	}
}

void space_draw_weapons()
{
	int i;
	char temp_filename[200];
	SDL_Rect destination;

	destination.x = 215;
	destination.y = 277;

	if ( !space.weapon_viewing ) //viewing guns
	{
		//draw veiwing guns image
		SDL_BlitSurface ( space.viewing_guns, NULL, screen, &destination );

		//draw gun icons
		for ( i=0;i<game.ship[game.ship_sel].gun_ammount;i++ ) //show those gun pods
			if ( game.ship[game.ship_sel].gun[i] > -1 && game.ship[game.ship_sel].gun[i] < GUN_MAX && game.ship[game.ship_sel].gun_id[i] > -1 && game.ship[game.ship_sel].gun_id[i] < MAX_SERVER )
			{
				destination = space.button[i + 53];
				destination.x++;
				destination.y++;

				if ( load_gun_pod_img ( i ) ) //if loaded
				{
					//fix a tiny bug....
					if ( space.missile_firing[i] || game.ship[game.ship_sel].missile_left[i] <= 0 )
						rectangleRGBA ( screen, space.button[i + 53].x, space.button[i + 53].y, space.button[i + 53].x + 33, space.button[i + 53].y + 23, 0, 0 , 0, 255 );

					SDL_BlitSurface ( server[game.ship[game.ship_sel].gun_id[i]].gun[game.ship[game.ship_sel].gun[i]].gun_pod_img, NULL, screen, &destination );
				}
				else //if not loaded
				{
					SDL_BlitSurface ( game.nif_image.no_gun_pod, NULL, screen, &destination );
				}

				//now for the little red destroyed box
				if ( game.ship[game.ship_sel].gun_destroyed[i] )
					rectangleRGBA ( screen, space.button[i + 53].x, space.button[i + 53].y, space.button[i + 53].x + 33, space.button[i + 53].y + 23, 255, 0 , 0, 255 );
			}
			else
			{
				SDL_FillRect ( screen, &space.button[i + 53], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
			}

		for ( ;i<16;i++ ) //blank out remaining spots
			SDL_FillRect ( screen, &space.button[i + 53], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	}
	else //viewing missiles
	{
		//draw veiwing missiles image
		SDL_BlitSurface ( space.viewing_missiles, NULL, screen, &destination );

		//draw missile icons
		for ( i=0;i<game.ship[game.ship_sel].missile_ammount;i++ ) //show those missile pods
			if ( game.ship[game.ship_sel].missile[i] > -1 && game.ship[game.ship_sel].missile[i] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[i] > -1 && game.ship[game.ship_sel].missile_id[i] < MAX_SERVER )
			{
				destination = space.button[i + 53];
				destination.x++;
				destination.y++;

				if ( load_missile_pod_img ( i ) ) //if loaded
					SDL_BlitSurface ( server[game.ship[game.ship_sel].missile_id[i]].missile[game.ship[game.ship_sel].missile[i]].missile_pod_img, NULL, screen, &destination );
				else
					SDL_BlitSurface ( game.nif_image.no_missile_pod, NULL, screen, &destination );

				//draw red box around missiles with empty payloads
				if ( game.ship[game.ship_sel].missile_left[i] <= 0 )
					rectangleRGBA ( screen, space.button[i + 53].x, space.button[i + 53].y, space.button[i + 53].x + 33, space.button[i + 53].y + 23, 255, 0 , 0, 255 );
				else if ( game.ship[game.ship_sel].gun_destroyed[i] ) //bug from hell
					rectangleRGBA ( screen, space.button[i + 53].x, space.button[i + 53].y, space.button[i + 53].x + 33, space.button[i + 53].y + 23, 0, 0 , 0, 255 );
			}
			else
			{
				SDL_FillRect ( screen, &space.button[i + 53], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
			}

		for ( ;i<16;i++ ) //blank out remaining spots
			SDL_FillRect ( screen, &space.button[i + 53], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	}
}

void space_disengage_user()
{
	SDL_Rect box_fill, destination;
	char temp_str[50], temp_filename[100], dis_code[5] = "10,";
	int ship_is_there = 0;

	ship_is_there = ( space.user[space.engaging].ship_id > -1 || space_user_is_special ( space.engaging ) );

	//tell the server
	if ( space.engaging > -1 && ship_is_there )
		send_con_server ( dis_code );

	//black out the area where the guns go
	box_fill.y = 277;
	box_fill.x = 215;
	box_fill.w = 165;
	box_fill.h = 107;
	if ( space.visible )
		SDL_FillRect ( screen, &box_fill, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

	//for now this is needed
	space.show_ship = 1;

	//draw in the pictures
	//name and ship
	if ( space.show_ship && space.visible )
	{
		sprintf ( temp_str,"User :: %s", game.username );
		draw_label ( screen, temp_str, &space.user_label, 0, 240, 240 );
		sprintf ( temp_str,"Guild :: %s", game.guild.name );
		draw_label ( screen, temp_str, &space.guild_label, 0, 240, 240 );

		destination.x = 216;
		destination.y = 279;

		//load image if required
		if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind] )
		{
			sprintf ( temp_filename, "server/%s/ships/%s/%s.jpg", server[game.server_id].servername, ship_name ( game.ship[game.ship_sel].kind, temp_str ), server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name );
			server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
		}

		//if image still not loaded, then load nif
		if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind] )
		{
			if ( !space.ship_nif_img[game.ship[game.ship_sel].kind] ) //load if required
			{
				sprintf ( temp_filename, "graphics/nif/nif_space_%s.jpg", ship_name ( game.ship[game.ship_sel].kind, temp_str ) );
				space.ship_nif_img[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
			}

			SDL_BlitSurface ( space.ship_nif_img[game.ship[game.ship_sel].kind], NULL, screen, &destination );
		}
		else
		{
			SDL_BlitSurface ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind], NULL, screen, &destination );
		}
	}

	//redraw engage button if selecting a user otherwise blank
	if ( space.engaging > -1 && ship_is_there && space.visible )
		SDL_BlitSurface ( space.engage_img, NULL, screen, &space.button[45] );

	//and stop the shooting
	space.engaging = -1;

	if ( space.visible )
		sdl_flip_mutex();
}

void space_clear_middle_box()
{
	space.viewing_cargo = 0;
	space.show_ship = 0;
}

void space_do_show_ship()
{
	SDL_Rect destination;
	char temp_str[1000], temp_filename[1000];

	//clear the box
	space_clear_middle_box();

	sprintf ( temp_str,"User :: %s", game.username );
	draw_label ( screen, temp_str, &space.user_label, 0, 240, 240 );
	sprintf ( temp_str,"Guild :: %s", game.guild.name );
	draw_label ( screen, temp_str, &space.guild_label, 0, 240, 240 );

	destination.x = 216;
	destination.y = 279;

	//load image if required
	if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind] )
	{
		sprintf ( temp_filename, "server/%s/ships/%s/%s.jpg", server[game.ship[game.ship_sel].server_id].servername, ship_name ( game.ship[game.ship_sel].kind, temp_str ), server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name );
		server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
	}

	//if image still not loaded, then load nif
	if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind] )
	{
		if ( !space.ship_nif_img[game.ship[game.ship_sel].kind] ) //load if required
		{
			sprintf ( temp_filename, "graphics/nif/nif_space_%s.jpg", ship_name ( game.ship[game.ship_sel].kind, temp_str ) );
			space.ship_nif_img[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
		}

		SDL_BlitSurface ( space.ship_nif_img[game.ship[game.ship_sel].kind], NULL, screen, &destination );
	}
	else
	{
		SDL_BlitSurface ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].space_img[game.ship[game.ship_sel].kind], NULL, screen, &destination );
	}

	//show a bitch
	space.show_ship = 1;
}

void space_draw_good_box_if_suposed_to()
{
	if ( !space.viewing_cargo ) return;
	if ( space.engaging != -1 ) return;
	if ( !space.visible ) return;


	space_draw_good_box ( space.view_cargo_goods,space.view_cargo_sector );
}

void space_draw_good_box ( int viewing_goods, int viewing_sector )
{
	int i;
	SDL_Surface *the_choosen_backround;
	SDL_Rect destination;

	//clear the box
	space_clear_middle_box();

	//set some stuff
	space.view_cargo_goods = viewing_goods;
	space.view_cargo_sector = viewing_sector;

	//find the backround we want
	if ( viewing_goods )
	{
		if ( viewing_sector )
			the_choosen_backround = space.viewing_goods_space;
		else
			the_choosen_backround = space.viewing_goods_ship;
	}
	else
	{
		if ( viewing_sector )
			the_choosen_backround = space.viewing_contras_space;
		else
			the_choosen_backround = space.viewing_contras_ship;
	}

	//draw that backrund
	destination.x = 216;
	destination.y = 279;
	SDL_BlitSurface ( the_choosen_backround, NULL, screen, &destination );


	//draw in all the good amounts
	for ( i=0;i<10;i++ )
		space_draw_good_number ( viewing_goods, viewing_sector, i );

	//do the text stuff
	if ( !space.text_box[2].selected )
		unselect_text_box ( screen, &space.text_box[2] );
	else
		select_text_box ( screen, &space.text_box[2] );

	//mutex!
	sdl_flip_mutex();

	//Now set it to "viewing"
	space.viewing_cargo = 1;
}

int space_redraw_good_number_if_required ( int g, int is_sector_change )
{
	if ( !space.viewing_cargo ) return 0;
	if ( space.engaging != -1 ) return 0;
	if ( !space.visible ) return 0;
	if ( space.view_cargo_sector != is_sector_change ) return 0;

	space_draw_good_number ( space.view_cargo_goods, space.view_cargo_sector, g );

	return 1;
}

void space_draw_good_number ( int viewing_goods, int viewing_sector, int g )
{
	char temp_str[50];
	int s_i,s,z, ss;

	s_i = game.server_id;
	s = game.sector;
	z = game.zone;
	ss = game.ship_sel;

	//good_label
	if ( viewing_goods )
	{
		if ( viewing_sector )
			sprintf ( temp_str,"%d", server[s_i].zone[z].sector[s].good_amount[g] );
		else
			sprintf ( temp_str,"%d", game.ship[ss].good_ammount[g] );
	}
	else
	{
		if ( viewing_sector )
			sprintf ( temp_str,"%d", server[s_i].zone[z].sector[s].contra_amount[g] );
		else
			sprintf ( temp_str,"%d", game.ship[ss].contra_ammount[g] );
	}

	if ( temp_str[0] != '0' )
		draw_right_label ( screen,temp_str,&space.good_label[g],0,150,0 );
	else
		draw_right_label ( screen,temp_str,&space.good_label[g],150,0,0 );
}

int space_good_select_animate()
{
	int g;
	static int c_i = 0, c_d = 1;
	const int c_max = 70;

	//checks..
	if ( !space.viewing_cargo ) return 0;
	if ( space.engaging != -1 ) return 0;
	if ( !space.visible ) return 0;

	//bs
	if ( c_d )
		c_i += 4;
	else
		c_i -= 4;

	if ( c_i >= c_max ) c_d = 0;
	if ( c_i <= 0 ) c_d = 1;


	//set um
	g = space.good_viewing + 69;


	//draw the monkey box...
	rectangleRGBA ( screen, space.button[g].x, space.button[g].y, space.button[g].x + space.button[g].w, space.button[g].y + space.button[g].h, 255 - ( c_i + c_i ), 127 - ( c_i ),0,255 );

	return 1;
}

void space_auto_do_good ( int g )
{
	int s_i,s,z, ss, amount, space_left;

	s_i = game.server_id;
	s = game.sector;
	z = game.zone;
	ss = game.ship_sel;

	//checks
	if ( !space.viewing_cargo ) return;
	if ( space.engaging != -1 ) return;
	if ( !space.visible ) return;

	space_left = game.ship[game.ship_sel].cargo - ship_total_good_ammount ( game.ship_sel );

	//contra or goods?
	if ( space.view_cargo_goods ) //goods
	{
		if ( space.view_cargo_sector ) //space
		{
			amount = server[s_i].zone[z].sector[s].good_amount[g];

			if ( amount > space_left ) amount = space_left;

			space_pickup_good ( space.view_cargo_goods, space.good_viewing, amount );
		}
		else //ship
		{
			space_drop_good ( space.view_cargo_goods, space.good_viewing, game.ship[ss].good_ammount[g] );
		}
	}
	else //contra
	{
		if ( space.view_cargo_sector ) //space
		{
			amount = server[s_i].zone[z].sector[s].contra_amount[g];

			if ( amount > space_left ) amount = space_left;

			space_pickup_good ( space.view_cargo_goods, space.good_viewing, server[s_i].zone[z].sector[s].contra_amount[g] );
		}
		else //ship
		{
			space_drop_good ( space.view_cargo_goods, space.good_viewing, game.ship[ss].contra_ammount[g] );
		}
	}
}

void space_select_good ( int g )
{
	//checks
	if ( space.engaging != -1 ) return;
	if ( !space.visible ) return;
	if ( !space.viewing_cargo ) return;

	//clear out old good space
	space_clear_good ( space.good_viewing );

	//well set it...
	space.good_viewing = g;
}

void space_clear_good ( int g )
{
	SDL_Surface *the_choosen_backround;
	SDL_Rect source;
	SDL_Rect destination;
	int ng;

	//settting shit
	ng = g + 69;

	source.x = space.button[ng].x - 216;
	source.y = space.button[ng].y - 279;
	source.w = space.button[ng].w + 1;
	source.h = space.button[ng].h + 1;

	destination.x = space.button[ng].x;
	destination.y = space.button[ng].y;
	destination.w = space.button[ng].w;
	destination.h = space.button[ng].h;

	//find the backround we want
	if ( space.view_cargo_goods )
	{
		if ( space.view_cargo_sector )
			the_choosen_backround = space.viewing_goods_space;
		else
			the_choosen_backround = space.viewing_goods_ship;
	}
	else
	{
		if ( space.view_cargo_sector )
			the_choosen_backround = space.viewing_contras_space;
		else
			the_choosen_backround = space.viewing_contras_ship;
	}

	//draw that backrund
	SDL_BlitSurface ( the_choosen_backround, &source, screen, &destination );
}

void space_do_drop_good ( int amount )
{
	if ( space.view_cargo_sector )
		space_pickup_good ( space.view_cargo_goods, space.good_viewing, amount );
	else
		space_drop_good ( space.view_cargo_goods, space.good_viewing, amount );
}

void space_drop_good ( int is_good, int g, int amount )
{
	char rtn_str[1000];

	if ( amount<1 ) return;

	if ( is_good )
		sprintf ( rtn_str,"17,%d,%d",g,amount );
	else
		sprintf ( rtn_str,"18,%d,%d",g,amount );

	send_con_server ( rtn_str );
}

void space_pickup_good ( int is_good, int g, int amount )
{
	char rtn_str[1000];

	if ( amount<1 ) return;

	if ( is_good )
		sprintf ( rtn_str,"15,%d,%d",g,amount );
	else
		sprintf ( rtn_str,"16,%d,%d",g,amount );

	send_con_server ( rtn_str );
}

void space_draw_gun_lines ( int x, int y )
{
	int i;

	//set random destination
	x += space.button[space.engaging].x;
	y += space.button[space.engaging].y;

	for ( i=0;i<game.ship[game.ship_sel].gun_ammount;i++ ) //show those gun pods
	{
		if ( game.ship[game.ship_sel].gun[i] < 0 ) continue;
		if ( game.ship[game.ship_sel].gun[i] >= GUN_MAX ) continue;
		if ( game.ship[game.ship_sel].gun_id[i] < 0 ) continue;
		if ( game.ship[game.ship_sel].gun_id[i] >= MAX_SERVER ) continue;
		if ( game.ship[game.ship_sel].gun_destroyed[i] ) continue;


		switch ( i )
		{
			case 0:
				space_draw_line ( 6,0, x, y );
				break;
			case 1:
				space_draw_line ( 6,1, x+18, y );
				break;
			case 2:
				space_draw_line ( 194,0, x, y+6 );
				break;
			case 3:
				space_draw_line ( 194,1, x+18, y+6 );
				break;
			case 4:
				space_draw_line ( 120,2, x+4, y );
				break;
			case 5:
				space_draw_line ( 480,2, x+14, y );
				break;
			case 6:
				space_draw_line ( 120,3, x+4, y+6 );
				break;
			case 7:
				space_draw_line ( 480,3, x+14, y+6 );
				break;
			case 8:
				space_draw_line ( 240,2, x+7, y );
				break;
			case 9:
				space_draw_line ( 360,2, x+11, y );
				break;
			case 10:
				space_draw_line ( 240,3, x+7, y+6 );
				break;
			case 11:
				space_draw_line ( 360,3, x+11, y+6 );
				break;
			case 12:
				space_draw_line ( 67,1, x+24, y+2 );
				break;
			case 13:
				space_draw_line ( 133,1, x+24, y+4 );
				break;
			case 14:
				space_draw_line ( 67,0, x, y+2 );
				break;
			case 15:
				space_draw_line ( 133,0, x, y+4 );
				break;
		}
	}


}

void space_draw_line_to_user ( int user_number )
{
	int k, initial_x, initial_y, x , y;

	x = space.user[user_number].attack_x;
	y = space.user[user_number].attack_y;
	initial_x = space.button[user_number].x + 30;
	initial_y = space.button[user_number].y + 25;

	for ( k=5;k>0;k-- )
	{
		lineRGBA ( screen, initial_x + k, initial_y, x, y, 0, 235 - ( k * 36 ), 10, 255 );
		lineRGBA ( screen, initial_x - k, initial_y, x, y, 0, 235 - ( k * 36 ), 10, 255 );
	}
	lineRGBA ( screen, initial_x, initial_y, x, y, 0, 235, 10, 255 );
}

void space_draw_line ( int position, int side, int x, int y )
{
	int k, initial_x, initial_y;

	//set (x,y) for the side
	switch ( side )
	{
		case 0: //left
			initial_x = 97;
			initial_y = 65 + position;
			break;
		case 1: //right
			initial_x = 696;
			initial_y = 65 + position;
			break;
		case 2: //top
			initial_x = 97 + position;
			initial_y = 65;
			break;
		case 3: //bottom
			initial_x = 97 + position;
			initial_y = 264;
			break;
	}

	//draw lines outer most first
	if ( side == 1 || side == 0 ) //if left or right
		for ( k=6;k>0;k-- )
		{
			lineRGBA ( screen, initial_x, initial_y + k, x, y, 0, 235 - ( k * 30 ), 10, 255 );
			lineRGBA ( screen, initial_x, initial_y - k, x, y, 0, 235 - ( k * 30 ), 10, 255 );
		}
	else //bottom or top
		for ( k=6;k>0;k-- )
		{
			lineRGBA ( screen, initial_x + k, initial_y, x, y, 0, 235 - ( k * 30 ), 10, 255 );
			lineRGBA ( screen, initial_x - k, initial_y, x, y, 0, 235 - ( k * 30 ), 10, 255 );
		}
	lineRGBA ( screen, initial_x, initial_y, x, y, 0, 235, 10, 255 );
}

void space_fire_missile ( int missile_number )
{
	double the_time;
	char rtn_str[7];

	if ( missile_number >= game.ship[game.ship_sel].missile_ammount ) return;
	if ( game.ship[game.ship_sel].missile[missile_number] < 0 ) return;
	if ( game.ship[game.ship_sel].missile_id[missile_number] < 0 ) return;
	if ( game.ship[game.ship_sel].missile_left[missile_number] <= 0 ) return;
	if ( space.engaging < 0 ) return;
	if ( space.missile_firing[missile_number] == 1 ) return;

	//tell the server
	sprintf ( rtn_str,"12,%d", missile_number );
	send_con_server ( rtn_str );

	game.ship[game.ship_sel].missile_left[missile_number]--;

	space.missile_x[missile_number] = space.button[space.engaging].x + 15 + ( rand() % 30 );
	space.missile_y[missile_number] = space.button[space.engaging].y + 12 + ( rand() % 25 );

	space.missile_time[missile_number] = current_time();

	space.missile_firing[missile_number] = 1;

	if ( game.ship[game.ship_sel].missile_left[missile_number] == 0 )
		rectangleRGBA ( screen, space.button[missile_number + 53].x, space.button[missile_number + 53].y, space.button[missile_number + 53].x + 33, space.button[missile_number + 53].y + 23, 255, 0 , 0, 255 );

	play_sound ( sound_missile );
}

void space_draw_enemy_missile ( int u, int missile_number, double ratio )
{
	int i, size, x, y;
	int initial_x = space.button[u].x + 30;
	int initial_y = space.button[u].y + 25;
	//int &x = space.user[u].missile_x[missile_number];
	//int &y = space.user[u].missile_y[missile_number];

	//initial_x += 97;
	//initial_y += 65;

	x = ( int ) ( ( space.user[u].missile_x[missile_number] - initial_x ) * ratio + initial_x );
	y = ( int ) ( ( space.user[u].missile_y[missile_number] - initial_y ) * ratio + initial_y );

	if ( space.user[u].attacking == -2 )
		size = ( int ) ( ( ratio ) * 6 + 1 );
	else
		size = 2;

	//draw missiles
	for ( i=size;i>0;i-- )
		filledCircleRGBA ( screen,x,y,i,0,220 * i / size,220, 255 );

	//space.button[i].x + 30, space.button[i].y + 25
}

void space_draw_missile ( int missile_number, double ratio )
{
	int i, size, x, y, initial_x, initial_y;

	switch ( missile_number )
	{
		case 0:
			initial_x = 7;
			initial_y = 50;
			break;
		case 1:
			initial_x = 593;
			initial_y = 50;
			break;
		case 2:
			initial_x = 7;
			initial_y = 150;
			break;
		case 3:
			initial_x = 593;
			initial_y = 150;
			break;
		case 4:
			initial_x = 100;
			initial_y = 7;
			break;
		case 5:
			initial_x = 500;
			initial_y = 7;
			break;
		case 6:
			initial_x = 100;
			initial_y = 193;
			break;
		case 7:
			initial_x = 500;
			initial_y = 193;
			break;
		case 8:
			initial_x = 7;
			initial_y = 100;
			break;
		case 9:
			initial_x = 593;
			initial_y = 100;
			break;
		case 10:
			initial_x = 200;
			initial_y = 7;
			break;
		case 11:
			initial_x = 400;
			initial_y = 7;
			break;
		case 12:
			initial_x = 200;
			initial_y = 193;
			break;
		case 13:
			initial_x = 400;
			initial_y = 193;
			break;
		case 14:
			initial_x = 300;
			initial_y = 7;
			break;
		case 15:
			initial_x = 300;
			initial_y = 193;
			break;
	}

	initial_x += 97;
	initial_y += 65;

	x = ( int ) ( ( space.missile_x[missile_number] - initial_x ) * ratio + initial_x );
	y = ( int ) ( ( space.missile_y[missile_number] - initial_y ) * ratio + initial_y );

	size = ( int ) ( ( 1 - ratio ) * 6 + 1 );

	//draw missiles
	for ( i=size;i>0;i-- )
		filledCircleRGBA ( screen,x,y,i,0,220 * i / size,220, 255 );

	//draw "timer box"
	if ( space.weapon_viewing == 1 && game.ship[game.ship_sel].missile_left[missile_number] > 0 )
		rectangleRGBA ( screen, space.button[missile_number + 53].x, space.button[missile_number + 53].y, space.button[missile_number + 53].x + 33, space.button[missile_number + 53].y + 23, ( int ) ( 255 * ( 1 - ratio ) ), 0 , 0, 255 );


}

void set_average_gun_speed()
{
	int i, k=0, sum = 0;

	for ( i=0;i<game.ship[game.ship_sel].gun_ammount;i++ ) //show those gun pods
		if ( game.ship[game.ship_sel].gun[i] > -1 && game.ship[game.ship_sel].gun[i] < GUN_MAX && game.ship[game.ship_sel].gun_id[i] > -1 && game.ship[game.ship_sel].gun_id[i] < MAX_SERVER )
		{
			//gar checks
			if ( game.ship[game.ship_sel].gun_destroyed[i] ) continue;

			if ( server[game.ship[game.ship_sel].gun_id[i]].gun[game.ship[game.ship_sel].gun[i]].type==4 )
				sum += 100 - ( ( 100 - server[game.ship[game.ship_sel].gun_id[i]].gun[game.ship[game.ship_sel].gun[i]].speed ) / 4 );
			else
				sum += server[game.ship[game.ship_sel].gun_id[i]].gun[game.ship[game.ship_sel].gun[i]].speed;
			k++;
		}

	//make average and turn it into seconds
	if ( k != 0 )
	{
		game.ship[game.ship_sel].gun_speed = sum / k;
		game.ship[game.ship_sel].gun_speed = 100 - game.ship[game.ship_sel].gun_speed;
		game.ship[game.ship_sel].gun_speed *= .1;
	}
	else
	{
		game.ship[game.ship_sel].gun_speed = 0;
	}
}

void set_missile_speed_times()
{
	int i;

	for ( i=0;i<game.ship[game.ship_sel].missile_ammount;i++ )
		if ( game.ship[game.ship_sel].missile[i] > -1 && game.ship[game.ship_sel].missile[i] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[i] > -1 && game.ship[game.ship_sel].missile_id[i] < MAX_SERVER )
		{
			space.missile_speed[i] = 100 - server[game.ship[game.ship_sel].missile_id[i]].missile[game.ship[game.ship_sel].missile[i]].speed;
			space.missile_speed[i] *= .1;
		}
}

void transfer_money_to ( char *user_name, int amount )
{
	char reetu_str[200];

	sprintf ( reetu_str,"14,%s,%d",user_name,amount );
	send_con_hub ( reetu_str );
}

void setup_private_comms ( struct displaybox *display_box )
{
	char message[55] = "... INITIATING SYSTEMS ...";

	add_display_box ( NULL, display_box, 1, message );
	message[0] = '\0';
	add_display_box ( NULL, display_box, 1, message );
	strcpy ( message,"... INITIALISING PRIVATE COMMS ..." );
	add_display_box ( NULL, display_box, 1, message );
	message[0] = '\0';
	add_display_box ( NULL, display_box, 1, message );
	strcpy ( message,"... PRIVATE COMMS ONLINE ..." );
	add_display_box ( NULL, display_box, 1, message );
	message[0] = '\0';
	add_display_box ( NULL, display_box, 1, message );
}

void setup_public_comms ( struct displaybox *display_box )
{
	char message[] = "... INITIALISING PUBLIC COMMS ...";

	add_display_box ( NULL, display_box, 1, message );
	message[0] = '\0';
	add_display_box ( NULL, display_box, 1, message );
	strcpy ( message,"... PUBLIC COMMS ONLINE ..." );
	add_display_box ( NULL, display_box, 1, message );
	message[0] = '\0';
	add_display_box ( NULL, display_box, 1, message );
	add_display_box ( NULL, display_box, 1, message );
	strcpy ( message,"... SCANNING AREA ..." );
	add_display_box ( NULL, display_box, 1, message );
	message[0] = '\0';
	add_display_box ( NULL, display_box, 1, message );
}

void set_shield ( int new_level )
{
	char temp_str[10];
	int old_level;

	//should we be doing shit?
	if ( shields_are_disabled() ) return;

	//set this
	old_level = space.shield_level;

	//set new
	space.shield_level = new_level;

	//draw
	draw_our_shield ( old_level );

	//tell server
	sprintf ( temp_str, "11,%d", new_level );
	send_con_server ( temp_str );
}

void draw_our_shield ( int old_level )
{
	char temp_str[10];

	//remove old
	SDL_BlitSurface ( space.shield_off[old_level], NULL, screen, &space.button[40 + old_level] );

	//draw the monkey
	SDL_BlitSurface ( space.shield_on[space.shield_level], NULL, screen, &space.button[40 + space.shield_level] );
	sprintf ( temp_str,"%.0lf", game.ship[game.ship_sel].shield * ( space.shield_level / 4.0 ) );
	draw_center_label ( screen, temp_str, &space.shield_label, 0, 240, 240 );

	//draw screen
	sdl_flip_mutex();

	//ert
	play_sound ( sound_shield );
}

void destroy_user ( char *victom )
{
	int i;

	if ( !victom ) return;
	if ( !victom[0] ) return;

	//scan em
	for ( i=0;i<40;i++ )
		if ( str_match ( victom, space.user[i].username ) )
		{
			//sync
			//SDL_LockMutex ( space_mutex );

			//stop attacking the dead person
			if ( i == space.engaging )
				space_disengage_user();

			//no selecting the dead person
			if ( i == space.user_sel )
				space.user_sel = -1;

			clear_space_user_struct ( & ( space.user[i] ) );

			//kill kill kill the white man
			space.user[i].destroy_time = current_time();
			space.user[i].destroy = 1;
			space.user[i].attacking = -1;

			play_sound ( sound_explosion );

			space.user_ammount--;
			if ( space.user_ammount<0 )
				space.user_ammount = 0;

			//SDL_UnlockMutex ( space_mutex );
		}
}

void set_hull ( int new_hull )
{
	char temp_str[32];
	SDL_Rect hull_fill;

	game.ship[game.ship_sel].hull = new_hull;
	hanger.ship[game.ship_sel].hull = new_hull;

	if ( space.visible )
	{
		sprintf ( temp_str,"%d", game.ship[game.ship_sel].hull );
		draw_center_label ( screen, temp_str, &space.hull_label, 0, 240, 240 );
		hull_fill.x = 139;
		hull_fill.w = 24;
		hull_fill.h = ( short unsigned int ) ( 60 - ( 60 * ( 1.0 * game.ship[game.ship_sel].hull / game.ship[game.ship_sel].hull_max ) ) );
		hull_fill.y = 316;
		SDL_FillRect ( screen, &hull_fill, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

		sdl_flip_mutex();
	}

	if ( repair.visible )
	{
		hull_fill.x = 472;
		hull_fill.y = 84;
		hull_fill.w = ( short unsigned int ) ( 176.0 * game.ship[game.ship_sel].hull / game.ship[game.ship_sel].hull_max );
		hull_fill.h = 12;
		SDL_FillRect ( screen, &hull_fill, SDL_MapRGB ( screen->format, 246, 246, 62 ) );

		//draw repair button if needed
		if ( game.ship[game.ship_sel].hull_max - game.ship[game.ship_sel].hull > 0 )
			SDL_BlitSurface ( repair.repair_ship_img, NULL, screen, &repair.button[1] );
		else //blank it out
			SDL_FillRect ( screen, &repair.button[1], SDL_MapRGB ( screen->format, 0, 0, 0 ) );

		//draw damage cost
		sprintf ( temp_str,"%d", ( game.ship[game.ship_sel].hull_max - game.ship[game.ship_sel].hull ) * 10 );
		draw_right_label ( screen, temp_str, &repair.cost_label, 82, 177, 188 );

		sdl_flip_mutex();
	}

	//ya
	hanger.redraw = 1;
}

void set_enemy_hull ( int new_percent, int guns_hit )
{
	if ( space.engaging > -1 )
	{
		space.user[space.engaging].hull_percent = new_percent;

		if ( guns_hit )
		{
			space.hit_enemy = 1;
			space.hit_max = guns_hit;
		}
	}
}

void set_user_engage ( char *username, char *victom )
{
	int i, j;
	const double quick_entrance_buffer = 1.0;

	//scan em
	for ( i=0;i<40;i++ )
		if ( str_match ( username, space.user[i].username ) )
			break;

	if ( i == 40 )
		return;

	for ( j=0;j<40;j++ )
		if ( str_match ( victom, space.user[j].username ) )
			break;

	if ( j == 40 )
		return;

	//sync
	//SDL_LockMutex ( space_mutex );

	space.user[i].attacking = j;

	//okay how much random start time should we have
	if ( current_time() - space.form_load_time <= quick_entrance_buffer )
		space.user[i].attack_time = current_time() - ( 1.0 + ( 1.7 * random_chance() ) );
	else
		space.user[i].attack_time = current_time() - ( 2.7 + ( 0.3 * random_chance() ) );

	space.user[i].attack_x = 0;
	space.user[i].attack_y = 0;

	//SDL_UnlockMutex ( space_mutex );
}

void set_user_engage_us ( char *username )
{
	int i;
	SDL_Rect destination;

	//scan em
	for ( i=0;i<40;i++ )
		if ( str_match ( username, space.user[i].username ) )
			break;

	if ( i == 40 )
		return;

	//sync
	//SDL_LockMutex ( space_mutex );

	//change their picture if needed
	if ( space.user[i].image != 2 )
	{
		space.user[i].image = 2;

		//as it states..
		clear_space_user_ship ( i );
		draw_space_user_ship ( i );

		//put loc on backround and fresh screen
		destination.x = 97;
		destination.y = 65;

		if ( space.visible )
		{
			SDL_BlitSurface ( space.temp_loc, NULL, screen, &destination );
			sdl_flip_mutex();
		}
	}

	space.user[i].attacking = -2;
	space.user[i].attack_time = current_time();

	//SDL_UnlockMutex ( space_mutex );

	play_sound ( sound_warning );
}



void set_user_disengage ( char *username )
{
	int i;
	SDL_Rect destination;

	//scan em
	for ( i=0;i<40;i++ )
		if ( str_match ( username, space.user[i].username ) )
			break;

	if ( i == 40 )
		return;

	//sync
	//SDL_LockMutex ( space_mutex );

	//change their picture if needed
	if ( space.user[i].attacking == -2 )
	{
		space.user[i].image = ( rand() % 5 );

		//as it states..
		clear_space_user_ship ( i );
		draw_space_user_ship ( i );

		//put loc on backround and fresh screen
		destination.x = 97;
		destination.y = 65;

		SDL_BlitSurface ( space.temp_loc, NULL, screen, &destination );
		sdl_flip_mutex();
	}

	space.user[i].attacking = -1;

	//SDL_UnlockMutex ( space_mutex );
}

void clear_space_user_ship ( int i )
{
	SDL_Rect img_location;

	//draw onto the loc
	img_location = space.button[i];

	img_location.x -= 97;
	img_location.y -= 65;

	//clear their space
	SDL_BlitSurface ( space.loc, &img_location, space.temp_loc, &img_location );
}

void draw_space_user_ship ( int i )
{
	char temp_str[25], temp_filename[200];
	SDL_Rect img_location;
	int sh_id, sh_t, sh_k;
	int i_num;

	sh_id = space.user[i].ship_id;
	sh_t = space.user[i].ship_type;
	sh_k = space.user[i].ship_kind;
	i_num = space.user[i].image;

	//set correct cords
	img_location = space.button[i];

	img_location.x -= 97;
	img_location.y -= 65;

	if ( sh_id == -1 )
	{
		if ( sh_k < 0 ) return;
		if ( sh_k >= SPECIAL_FSHIP_MAX ) return;

		//special ship!
		SDL_BlitSurface ( special_fship ( sh_k ), NULL, space.temp_loc, &img_location );

		return;
	}

	//now what image to draw..
	if ( space.user[i].sos )
	{
		//this function really shouldn't ever be able to return 0
		if ( load_fship_sos ( sh_id, sh_t, sh_k, i_num, space.user[i].ship_name ) )
			SDL_BlitSurface ( server[sh_id].ship[sh_t].fship_sos_img[sh_k][i_num], NULL, space.temp_loc, &img_location );
	}
	else
	{
		if ( load_fship ( sh_id, sh_t, sh_k, i_num, space.user[i].ship_name ) )
			SDL_BlitSurface ( server[sh_id].ship[sh_t].fship_img[sh_k][i_num], NULL, space.temp_loc, &img_location );
		else
			SDL_BlitSurface ( hanger.no_fship[sh_k][i_num], NULL, space.temp_loc, &img_location );
	}
}

int load_fship_sos ( int s, int s_t, int s_k, int i, char *the_ship_name )
{
	//already loaded fools
	if ( server[s].ship[s_t].fship_sos_img[s_k][i] ) return 1;

	if ( load_fship ( s, s_t, s_k, i, the_ship_name ) )
	{
		server[s].ship[s_t].fship_sos_img[s_k][i] = add_red_glow ( server[s].ship[s_t].fship_img[s_k][i] );

		return 1;
	}
	else
	{
		if ( hanger.no_fship[s_k][i] )
		{
			server[s].ship[s_t].fship_sos_img[s_k][i] = add_red_glow ( hanger.no_fship[s_k][i] );

			return 1;
		}
		else
			return 0;;
	}
}

int load_fship ( int server_id, int ship_type, int ship_kind, int ship_face, char *the_ship_name )
{
	char temp_str[25], temp_filename[200];

	//check if image loaded already.
	if ( !server[server_id].ship[ship_type].fship_img[ship_kind][ship_face] )
	{
		if ( ship_type < SHIP_MAX )
			sprintf ( temp_filename, "server/%s/fships/%s/%s%d.gif", server[server_id].servername, ship_name ( ship_kind, temp_str ), the_ship_name,  ship_face + 1 );
		else
			sprintf ( temp_filename, "server/%s/fships/Field/%s%d.gif", server[server_id].servername, the_ship_name,  ship_face + 1 );
		server[server_id].ship[ship_type].fship_img[ship_kind][ship_face] = IMG_Load ( temp_filename );
	}

	//check if we were able to load it
	if ( !server[server_id].ship[ship_type].fship_img[ship_kind][ship_face] )
	{
		if ( !hanger.no_fship[ship_kind][ship_face] ) //if the no_ship img isnt loaded... then load it
		{
			if ( ship_type < SHIP_MAX )
			{
				sprintf ( temp_filename, "graphics/nif/nif_f%s%d.gif",  ship_name ( ship_kind, temp_str ),  ship_face + 1 );
				hanger.no_fship[ship_kind][ship_face] = IMG_Load ( temp_filename );
			}
		}

		return 0;
	}
	else //draw correct image since it was found
	{
		return 1;
	}
}

void repair_draw_weapons()
{
	int i;
	SDL_Rect destination;

	repair.redraw_weapons = 0;

	destination.w = 30;
	destination.h = 20;

	//x = 625
	//y1 = 131
	//y2 = 306
	//dy = 175
	//dyr = 145
	//i = 21
	for ( i=0;i<16;i++ )
	{
		//setup destination
		destination.x = 408 + ( 120 * ( i>7 ) );
		destination.y = ( 130 + ( i * 22 ) ) - ( 176 * ( i>7 ) );
		destination.w = 30;
		destination.h = 20;

		//draw guns
		if ( game.ship[game.ship_sel].gun[i] > -1 && game.ship[game.ship_sel].gun[i] < GUN_MAX && game.ship[game.ship_sel].gun_id[i] > -1 && game.ship[game.ship_sel].gun_id[i] < MAX_SERVER && i < game.ship[game.ship_sel].gun_ammount )
		{

			if ( load_gun_pod_img ( i ) )
				SDL_BlitSurface ( server[game.ship[game.ship_sel].gun_id[i]].gun[game.ship[game.ship_sel].gun[i]].gun_pod_img, NULL, repair.backround, &destination );
			else
				SDL_BlitSurface ( game.nif_image.no_gun_pod, NULL, repair.backround, &destination );
		}
		else //blank that pod area out since no weapon
		{
			SDL_FillRect ( repair.backround, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
		}

		destination.y += 211;

		//draw missiles
		if ( game.ship[game.ship_sel].missile[i] > -1 && game.ship[game.ship_sel].missile[i] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[i] > -1 && game.ship[game.ship_sel].missile_id[i] < MAX_SERVER && i < game.ship[game.ship_sel].missile_ammount )
		{
			if ( load_missile_pod_img ( i ) )
				SDL_BlitSurface ( server[game.ship[game.ship_sel].missile_id[i]].missile[game.ship[game.ship_sel].missile[i]].missile_pod_img, NULL, repair.backround, &destination );
			else
				SDL_BlitSurface ( game.nif_image.no_missile_pod, NULL, repair.backround, &destination );
		}
		else
		{
			SDL_FillRect ( repair.backround, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
		}
	}
}

int load_gun_pod_img ( int i )
{
	return load_gun_pod_img_specific ( game.ship[game.ship_sel].gun_id[i], game.ship[game.ship_sel].gun[i] );
}

int load_gun_pod_img_specific ( int s, int w )
{
	char temp_filename[200];

	if ( !server[s].gun[w].gun_pod_img ) //what?! no image loaded!
	{
		sprintf ( temp_filename, "server/%s/shop/guns/icons/%s.jpg", server[s].servername, server[s].gun[w].name );
		server[s].gun[w].gun_pod_img = IMG_Load ( temp_filename );
	}

	if ( !server[s].gun[w].gun_pod_img ) //still not loaded... wah
	{
		//check to load the nif image..
		if ( !game.nif_image.no_gun_pod )
			game.nif_image.no_gun_pod = IMG_Load ( "graphics/nif/nif_gun_pod.jpg" );

		return 0;
	}
	else //or is it?
	{
		return 1;
	}
}

int load_missile_pod_img ( int i )
{
	return load_missile_pod_img_specific ( game.ship[game.ship_sel].missile_id[i], game.ship[game.ship_sel].missile[i] );
}

int load_missile_pod_img_specific ( int s, int w )
{
	char temp_filename[200];

	if ( !server[s].missile[w].missile_pod_img ) //what?! no image loaded!
	{
		sprintf ( temp_filename, "server/%s/shop/missiles/icons/%s.jpg", server[s].servername, server[s].missile[w].name );
		server[s].missile[w].missile_pod_img = IMG_Load ( temp_filename );
	}

	if ( !server[s].missile[w].missile_pod_img ) //still not loaded... wah
	{
		//check to load the nif image..
		if ( !game.nif_image.no_missile_pod )
			game.nif_image.no_missile_pod = IMG_Load ( "graphics/nif/nif_missile_pod.jpg" );

		return 0;
	}
	else //or is it?
	{
		return 1;
	}
}

void map_draw_scan()
{
	int i;

	//draw text
	draw_label ( screen, map.scan_label.message, &map.scan_label, 255, 255, 255 );

	//draw new area
	//check which selection box to draw
	if ( map.scan_sector == game.sector )
	{
		map.scan_location.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[map.scan_sector].x - map.sector_x ) ) - 19;
		map.scan_location.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[map.scan_sector].y - map.sector_y ) ) - 19;
		map.scan_location.w = 38;
		map.scan_location.h = 38;

		map.scan_location.x += map.shift_x;
		map.scan_location.y += map.shift_y;

		SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
		SDL_BlitSurface ( map.scan_large, NULL, screen, &map.scan_location );
	}
	else
	{
		for ( i=0;i<5;i++ )
			if ( map.sector_link[i] == map.scan_sector )
				break;

		if ( i==5 ) //its a tiny selection
		{
			map.scan_location.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[map.scan_sector].x - map.sector_x ) ) - 9;
			map.scan_location.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[map.scan_sector].y - map.sector_y ) ) - 9;
			map.scan_location.w = 18;
			map.scan_location.h = 18;

			map.scan_location.x += map.shift_x;
			map.scan_location.y += map.shift_y;

			if ( map.scan_location.x < 80 && map.scan_location.y < 105 ) //up left
			{
				map.scan_location.x = 85;
				map.scan_location.y = 125;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_up_left, NULL, screen, &map.scan_location );

				return;
			}
			if ( map.scan_location.x > 715 && map.scan_location.y < 105 ) //up right
			{
				map.scan_location.x = 694;
				map.scan_location.y = 125;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_up_right, NULL, screen, &map.scan_location );

				return;
			}
			if ( map.scan_location.x < 80 && map.scan_location.y > 550 ) //down left
			{
				map.scan_location.x = 85;
				map.scan_location.y = 516;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_down_left, NULL, screen, &map.scan_location );

				return;
			}
			if ( ( map.scan_location.x > 610 && map.scan_location.y > 550 ) || ( map.scan_location.x > 715 && map.scan_location.y > 505 ) ) // down right
			{
				map.scan_location.x = 694;
				map.scan_location.y = 516;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_down_right, NULL, screen, &map.scan_location );

				return;
			}
			//check for left/right/up/down
			if ( map.scan_location.y < 105 ) //up
			{
				map.scan_location.y = 125;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_up, NULL, screen, &map.scan_location );

				return;
			}
			if ( map.scan_location.y > 550 && map.scan_location.x >= 610 ) //down low
			{
				map.scan_location.y = 546;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_down, NULL, screen, &map.scan_location );

				return;
			}
			if ( map.scan_location.y > 505 && map.scan_location.x < 610 ) //down high
			{
				map.scan_location.y = 516;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_down, NULL, screen, &map.scan_location );

				return;
			}
			if ( map.scan_location.x >= 715 ) //right
			{
				map.scan_location.x = 694;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_right, NULL, screen, &map.scan_location );

				return;
			}
			if ( map.scan_location.x < 80 ) //left
			{
				map.scan_location.x = 85;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_left, NULL, screen, &map.scan_location );

				return;
			}

			SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
			SDL_BlitSurface ( map.scan_tiny, NULL, screen, &map.scan_location );
		}
		else //its a "small" neighbooring selection
		{
			map.scan_location.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[map.scan_sector].x - map.sector_x ) ) - 14;
			map.scan_location.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[map.scan_sector].y - map.sector_y ) ) - 14;
			map.scan_location.w = 28;
			map.scan_location.h = 28;

			map.scan_location.x += map.shift_x;
			map.scan_location.y += map.shift_y;

			SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
			SDL_BlitSurface ( map.scan_small, NULL, screen, &map.scan_location );
		}
	}
}

void map_set_scan ( int new_sector, char *message )
{
	int i;
	SDL_Rect temp_location;

	if ( map.visible )
	{
		//clear old scan
		if ( new_sector == -1 )
		{
			map.scan_sector = -1;
			map_redraw();
		}

		//draw text
		draw_label ( screen, message, &map.scan_label, 255, 255, 255 );

		//hub got fucked up
		if ( new_sector < 0 || new_sector >= SECTOR_MAX ) {sdl_flip_mutex(); return;}
		//nothing needs changed if...
		if ( map.scan_sector == new_sector ) {sdl_flip_mutex(); return;}

		//"redraw" old area
		if ( map.scan_sector >= 0 )
		{
			temp_location = map.scan_location;
			temp_location.x = 0;
			temp_location.y = 0;
			SDL_BlitSurface ( map.scan_last, &temp_location, screen, &map.scan_location );
		}



		//draw new area
		//check which selection box to draw
		if ( new_sector == game.sector )
		{
			map.scan_location.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[new_sector].x - map.sector_x ) ) - 19;
			map.scan_location.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[new_sector].y - map.sector_y ) ) - 19;
			map.scan_location.w = 38;
			map.scan_location.h = 38;
			map.scan_location.x += map.shift_x;
			map.scan_location.y += map.shift_y;

			SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
			SDL_BlitSurface ( map.scan_large, NULL, screen, &map.scan_location );

		}
		else
		{
			for ( i=0;i<5;i++ )
				if ( map.sector_link[i] == new_sector )
					break;

			if ( i==5 ) //its a tiny selection
			{
				map.scan_location.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[new_sector].x - map.sector_x ) ) - 9;
				map.scan_location.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[new_sector].y - map.sector_y ) ) - 9;
				map.scan_location.w = 18;
				map.scan_location.h = 18;
				map.scan_location.x += map.shift_x;
				map.scan_location.y += map.shift_y;

				if ( map.scan_location.x < 80 && map.scan_location.y < 105 ) //up left
				{
					map.scan_location.x = 85;
					map.scan_location.y = 125;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_up_left, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( map.scan_location.x > 715 && map.scan_location.y < 105 ) //up right
				{
					map.scan_location.x = 694;
					map.scan_location.y = 125;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_up_right, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( map.scan_location.x < 80 && map.scan_location.y > 550 ) //down left
				{
					map.scan_location.x = 85;
					map.scan_location.y = 516;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_down_left, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( ( map.scan_location.x > 600 && map.scan_location.y > 550 ) || ( map.scan_location.x > 715 && map.scan_location.y > 505 ) ) // down right
				{
					map.scan_location.x = 694;
					map.scan_location.y = 486;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_down_right, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				//check for left/right/up/down
				if ( map.scan_location.y < 105 ) //up
				{
					map.scan_location.y = 125;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_up, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( map.scan_location.y > 550 && map.scan_location.x >= 610 ) //down low
				{
					map.scan_location.y = 546;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_down, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( map.scan_location.y > 505 && map.scan_location.x < 610 ) //down high
				{
					map.scan_location.y = 516;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_down, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( map.scan_location.x >= 610 ) //right
				{
					map.scan_location.x = 694;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_right, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}
				if ( map.scan_location.x <= 80 ) //left
				{
					map.scan_location.x = 85;

					SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
					SDL_BlitSurface ( map.scan_left, NULL, screen, &map.scan_location );

					map.scan_sector = new_sector;
					return;
				}

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_tiny, NULL, screen, &map.scan_location );
			}
			else //its a "small" neighbooring selection
			{
				map.scan_location.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[new_sector].x - map.sector_x ) ) - 14;
				map.scan_location.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[new_sector].y - map.sector_y ) ) - 14;
				map.scan_location.w = 28;
				map.scan_location.h = 28;
				map.scan_location.x += map.shift_x;
				map.scan_location.y += map.shift_y;

				SDL_BlitSurface ( screen, &map.scan_location, map.scan_last, NULL );
				SDL_BlitSurface ( map.scan_small, NULL, screen, &map.scan_location );
			}
		}

		//set stuff
		map.scan_sector = new_sector;
	}
	else //map not visible
	{
		//set for when map is loaded
		strcpy ( map.scan_label.message, message );
		map.scan_sector = new_sector;
	}

}

void send_public_chat ( char *message )
{
	int i, len;
	char rtn_str[500];

	//check for other command
	if ( check_for_command ( message ) ) return;

	len = strlen ( message );

	//check for ldm
	for ( i=1;i<22 && i<len;i++ )
		if ( message[i-1] == ':' && message[i] == ':' )
		{
			message[i-1] = '\0';
			sprintf ( rtn_str, "14,%s,%s", message, message + i + 1 );

			//put it
			strcpy ( game.last_ldm, message );

			//well it works
			jabber_send ( message, message + i + 1 );

			message[i-1] = ':';
			send_con_server ( rtn_str );
			return;
		}

	//send normaly
	sprintf ( rtn_str, "4,%s", message );
	send_con_server ( rtn_str );
}

void trade_draw_news_or_shop()
{
	if ( trade.viewing_news )
		trade_draw_news();
	else
		trade_draw_shop();
}

void trade_toggle_news_and_shop()
{
	if ( trade.viewing_news )
		trade.viewing_news = 0;
	else
		trade.viewing_news = 1;

	trade_draw_news_or_shop();
	sdl_flip_mutex();
}

void trade_draw_news()
{
	int i, p, s, d;
	char planet_name[50], good_highest[10], good_lowest[10];;
	SDL_Rect destination;

	s = game.server_id;
	p = planet.last_planet;

	//draw a button
	SDL_BlitSurface ( trade.goto_trade_but, NULL, screen, &trade.button[11] );

	//you heard the man
	trade_clear_space();

	//clear out all the boxes
	clear_display_box ( screen, &trade.news_disaster_display );

	//add all trade planet news
	for ( i=0;i<PLANET_TRADE_MAX;i++ )
	{
		int l_p;

		l_p = server[s].planet[p].trade_planet[i];

		if ( l_p < 0 ) continue;
		if ( l_p >= PLANET_MAX ) continue;

		planet_name[0] = 0;
		for ( d=0;d<server[s].planet[l_p].trade_disaster_max;d++ )
		{
			int g, is_low, text_i;

			//set stuff
			g = server[s].planet[l_p].trade_disaster[d].g;
			is_low = server[s].planet[l_p].trade_disaster[d].is_low;
			text_i = server[s].planet[l_p].trade_disaster[d].i;

			add_display_box ( screen, &trade.news_disaster_display, 0, get_trade_disaster_text ( g, is_low, text_i ) );
			if ( d+1 != server[s].planet[l_p].trade_disaster_max )
				add_display_box ( screen, &trade.news_disaster_display, 0, planet_name );
		}
		sprintf ( planet_name,"%s News ::", server[s].planet[l_p].name );
		add_display_box ( screen, &trade.news_disaster_display, 1, planet_name );

		planet_name[0] = 0;
		add_display_box ( screen, &trade.news_disaster_display, 0, planet_name );
	}

	//now add this planets entries
	planet_name[0] = 0;
	for ( d=0;d<server[s].planet[p].trade_disaster_max;d++ )
	{
		int g, is_low, text_i;

		//set stuff
		g = server[s].planet[p].trade_disaster[d].g;
		is_low = server[s].planet[p].trade_disaster[d].is_low;
		text_i = server[s].planet[p].trade_disaster[d].i;

		add_display_box ( screen, &trade.news_disaster_display, 0, get_trade_disaster_text ( g, is_low, text_i ) );
		if ( d+1 != server[s].planet[p].trade_disaster_max )
			add_display_box ( screen, &trade.news_disaster_display, 0, planet_name );
	}
	sprintf ( planet_name,"Local News ::" );
	add_display_box ( screen, &trade.news_disaster_display, 1, planet_name );

	//now have sex with a tree
	for ( i=d=0;i<PLANET_TRADE_MAX;i++ )
	{
		int l_p, t;
		int diff[10];
		int highest, lowest;

		l_p = server[s].planet[p].trade_planet[i];

		if ( l_p < 0 ) continue;
		if ( l_p >= PLANET_MAX ) continue;

		//collect the differences
		for ( t=0;t<10;t++ )
			diff[t] = ( server[s].planet[l_p].good_price[t] - server[s].planet[p].good_price[t] ) - ( TRADE_POINT_LOSS * 2 );

		//find the low/high
		highest = 0;
		lowest = 0;

		for ( t=1;t<10;t++ )
		{
			if ( diff[highest] < diff[t] ) highest = t;
			if ( diff[lowest] > diff[t] ) lowest = t;
		}

		//display name
		sprintf ( planet_name,"%s Trade Table ::", server[s].planet[l_p].name );
		draw_label ( screen, planet_name, &trade.best_trade_planet_name_label[d], 114, 1, 1 );

		//display differences
		destination = trade.best_trade_planet_name_label[d].location;
		//destination.y -= 5;
		for ( t=0;t<10;t++ )
		{
			destination.x = 520 + ( t * 17 );
			SDL_BlitSurface ( game.game_image.mini_small_good[t], NULL, screen, &destination );
			if ( diff[t] < 0 )
				rectangleRGBA ( screen, destination.x, destination.y, destination.x + destination.w, destination.y + destination.h, 255 * diff[t] / diff[lowest],0,0, 255 );
			if ( diff[t] > 0 )
				rectangleRGBA ( screen, destination.x, destination.y, destination.x + destination.w, destination.y + destination.h, 0, 255 * diff[t] / diff[highest],0, 255 );
		}

		//display best choice
		good_mini_name ( highest, good_highest );
		good_mini_name ( lowest, good_lowest );
		sprintf ( planet_name,"Suggested purchases :: %s here, %s at %s", good_highest, good_lowest, server[s].planet[l_p].name );
		draw_label ( screen, planet_name, &trade.best_trade_suggested_label[d], 0, 240, 240 );

		//now increase this one
		d++;
	}
}

void trade_clear_space()
{
	SDL_Rect the_way_out; //is through

	the_way_out.x = 350;
	the_way_out.y = 90;
	the_way_out.w = 715 - 350;
	the_way_out.h = 535 - 90;

	if ( trade.viewing_news )
		SDL_FillRect ( screen, &the_way_out, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	else
		SDL_BlitSurface ( trade.backround, &the_way_out, screen, &the_way_out );
}

void trade_draw_shop()
{
	int i;
	SDL_Rect destination;

	//draw a button
	SDL_BlitSurface ( trade.goto_news_but, NULL, screen, &trade.button[11] );

	//you heard the man
	trade_clear_space();

	//draw mini icons
	for ( i=0;i<10;i++ )
		SDL_BlitSurface ( game.game_image.mini_good[i], NULL, screen, &trade.button[i] );

	//draw images
	//small planet icon
	destination.x = 367;
	destination.y = 300;
	load_map_planet ( game.server_id, planet.last_planet );
	SDL_BlitSurface ( server[game.server_id].planet[planet.last_planet].hyper_img, NULL, screen, &destination );

	//ship icon
	destination.x = 360;
	destination.y = 400;
	if ( load_fship ( game.ship[game.ship_sel].server_id, game.ship[game.ship_sel].type, game.ship[game.ship_sel].kind, 0, server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name ) )
		SDL_BlitSurface ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].fship_img[game.ship[game.ship_sel].kind][0], NULL, screen, &destination );
	else if ( game.ship[game.ship_sel].kind > -1 )
		SDL_BlitSurface ( hanger.no_fship[game.ship[game.ship_sel].kind][0], NULL, screen, &destination );

	//Draw selection.
	trade_draw_selection ( trade.good_selected );

	//draw text
	for ( i=0;i<2;i++ )
		if ( trade.text_box[i].selected )
			select_text_box ( screen, &trade.text_box[i] );
		else
			unselect_text_box ( screen, &trade.text_box[i] );

	//we like this
	trade_draw_sell_all_button();

	//set this
	trade.info_visible = 0;
}

void trade_draw_sell_all_button()
{
	//we got any goods?
	if ( ship_total_good_ammount ( game.ship_sel ) )
		SDL_BlitSurface ( trade.sell_all_but, NULL, screen, &trade.button[12] );
	else
		SDL_FillRect ( screen, &trade.button[12], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void trade_update_good_ammount()
{
	char temp_str[512];

	sprintf ( temp_str, "%s :: %d units @ %d", server[game.server_id].planet[planet.last_planet].name, server[game.server_id].planet[planet.last_planet].good_ammount[trade.good_selected], server[game.server_id].planet[planet.last_planet].good_price[trade.good_selected] + TRADE_POINT_LOSS );
	draw_label ( screen, temp_str, &trade.trade_label[1], 68, 212, 197 );

	sdl_flip_mutex();
}

void trade_update_player_good_ammount()
{
	char temp_str[512], i, p, s;
	int space_left, any_good_purchasable;

	sprintf ( temp_str,"%d", game.ship[game.ship_sel].cargo - ship_total_good_ammount ( game.ship_sel ) );
	draw_right_label ( screen, temp_str, &trade.cargo_label, 107, 216, 210 );

	trade_display_selected_good_amounts();

	//now see what textbox we should default to
	space_left = game.ship[game.ship_sel].cargo - ship_total_good_ammount ( game.ship_sel );
	if ( space_left == game.ship[game.ship_sel].cargo ) //empty, so can only buy
	{
		if ( !trade.text_box[0].selected )
		{
			trade.text_box[0].selected = 1;
			trade.text_box[1].selected = 0;


			select_text_box ( screen, &trade.text_box[0] );
			unselect_text_box ( screen, &trade.text_box[1] );
		}
	}
	else
	{
		if ( !space_left )
		{
			any_good_purchasable = 0;
		}
		else //check
		{
			p = planet.last_planet;
			s = game.server_id;

			any_good_purchasable = 0;

			for ( i=0;i<10;i++ )
				if ( server[s].planet[p].good_price[i] + TRADE_POINT_LOSS < game.money && server[s].planet[p].good_ammount[i] )
					{any_good_purchasable = 1; break;}
		}

		if ( !any_good_purchasable ) //full, so can only sell
		{
			if ( !trade.text_box[1].selected )
			{
				trade.text_box[0].selected = 0;
				trade.text_box[1].selected = 1;


				unselect_text_box ( screen, &trade.text_box[0] );
				select_text_box ( screen, &trade.text_box[1] );
			}
		}
	}

	trade_draw_sell_all_button();

	sdl_flip_mutex();
}

void trade_draw_selection ( int new_selection )
{
	int i, temp_int, price, space_left;
	SDL_Rect destination;
	char temp_str[512];
	char temp_name[20];

	//blank old selection box
	i = trade.good_selected;
	rectangleRGBA ( screen, trade.button[i].x - 1, trade.button[i].y - 1, trade.button[i].x + trade.button[i].w + 2, trade.button[i].y + trade.button[i].h + 2, 0, 0 , 0, 255 );

	//set the selection
	trade.good_selected = new_selection;

	//set name
	good_name ( new_selection, temp_name );

	//draw selection box
	i = new_selection;
	rectangleRGBA ( screen, trade.button[i].x - 1, trade.button[i].y - 1, trade.button[i].x + trade.button[i].w + 2, trade.button[i].y + trade.button[i].h + 2, 255, 0 , 0, 255 );

	//large good image
	destination.x = 367;
	destination.y = 150;
	SDL_BlitSurface ( trade.large_good[new_selection], NULL, screen, &destination );

	//draw the texts...
	draw_label ( screen, temp_name, &trade.trade_label[0], 68, 212, 197 );

	//draw display boxes
	clear_display_box ( screen, &trade.trade_display[0] );
	add_display_box ( screen, &trade.trade_display[0], 0, good_description ( new_selection, temp_str ) );

	trade_display_selected_good_amounts();
}

void trade_display_selected_good_amounts()
{
	int i, temp_int, price, space_left, g, cargo_available;
	char temp_str[512];
	char temp_name[20];

	g = trade.good_selected;

	good_name ( g, temp_name );

	sprintf ( temp_str, "%s :: %d units @ %d", server[game.server_id].planet[planet.last_planet].name, server[game.server_id].planet[planet.last_planet].good_ammount[g], server[game.server_id].planet[planet.last_planet].good_price[g] + TRADE_POINT_LOSS );
	draw_label ( screen, temp_str, &trade.trade_label[1], 68, 212, 197 );

	sprintf ( temp_str, "Cargo Hold :: %d units @ %d", game.ship[game.ship_sel].good_ammount[g], server[game.server_id].planet[planet.last_planet].good_price[g] - TRADE_POINT_LOSS );
	draw_label ( screen, temp_str, &trade.trade_label[2], 68, 212, 197 );

	//calculate how much we can get
	space_left = game.ship[game.ship_sel].cargo - ship_total_good_ammount ( game.ship_sel );
	price = server[game.server_id].planet[planet.last_planet].good_price[g] + TRADE_POINT_LOSS;
	cargo_available = server[game.server_id].planet[planet.last_planet].good_ammount[g];

	if ( price )
	{
		temp_int = game.money / price;

		while ( temp_int * price > game.money )
			temp_int--;

		if ( temp_int > space_left )
			temp_int = space_left;

		if ( temp_int > cargo_available )
			temp_int = cargo_available;
	}
	else
	{
		temp_int = space_left;
	}

	clear_display_box ( screen, &trade.trade_display[1] );
	sprintf ( temp_str, "%d units of %s are available on %s at a cost of %d per unit. You can currently purchase up to %d units.",  server[game.server_id].planet[planet.last_planet].good_ammount[g], temp_name, server[game.server_id].planet[planet.last_planet].name, server[game.server_id].planet[planet.last_planet].good_price[g] + TRADE_POINT_LOSS, temp_int );
	add_display_box ( screen, &trade.trade_display[1], 0, temp_str );

	clear_display_box ( screen, &trade.trade_display[2] );
	sprintf ( temp_str, "%d units of %s are stored in your cargo bays to sell at a price of %d per unit.",  game.ship[game.ship_sel].good_ammount[g], temp_name, server[game.server_id].planet[planet.last_planet].good_price[g] -  TRADE_POINT_LOSS );
	add_display_box ( screen, &trade.trade_display[2], 0, temp_str );
}

void load_good_images()
{
	int i;
	char file_name[200];

	//leave if already loaded.
	if ( game.game_image.mini_good[0] ) return;

	for ( i=0;i<10;i++ )
	{
		sprintf ( file_name, "graphics/goods%d.jpg", i );
		game.game_image.mini_good[i] = IMG_Load ( file_name );

		sprintf ( file_name, "graphics/goods_small%d.jpg", i );
		game.game_image.mini_small_good[i] = IMG_Load ( file_name );

		sprintf ( file_name, "graphics/igoods%d.jpg", i );
		game.game_image.mini_contra[i] = IMG_Load ( file_name );
	}
}

int load_map_planet ( int server_id, int planet_id )
{
	char file_name[200];

	if ( !server[server_id].planet[planet_id].hyper_img )
	{
		sprintf ( file_name, "server/%s/buttons/hyper/p%d.gif", server[server_id].servername, planet_id );
		server[server_id].planet[planet_id].hyper_img = IMG_Load ( file_name );

		if ( !server[server_id].planet[planet_id].hyper_img )
			return 0;
		else
			return 1;
	}
	else
	{
		return 1;
	}
}

char *good_mini_name ( int good_number, char *dest )
{
	switch ( good_number )
	{
		case 0:
			strcpy ( dest,"Ts" );
			break;
		case 1:
			strcpy ( dest,"Gs" );
			break;
		case 2:
			strcpy ( dest,"Cn" );
			break;
		case 3:
			strcpy ( dest,"Kw" );
			break;
		case 4:
			strcpy ( dest,"Pa" );
			break;
		case 5:
			strcpy ( dest,"Zy" );
			break;
		case 6:
			strcpy ( dest,"Mg" );
			break;
		case 7:
			strcpy ( dest,"Ve" );
			break;
		case 8:
			strcpy ( dest,"Gs" );
			break;
		case 9:
			strcpy ( dest,"Ge" );
			break;
	}

	return dest;
}

char *good_name ( int good_number, char *dest )
{
	switch ( good_number )
	{
		case 0:
			strcpy ( dest,"Tynosine" );
			break;
		case 1:
			strcpy ( dest,"Gryn Stone" );
			break;
		case 2:
			strcpy ( dest,"Cryonite" );
			break;
		case 3:
			strcpy ( dest,"Kengo Weed" );
			break;
		case 4:
			strcpy ( dest,"Paralium" );
			break;
		case 5:
			strcpy ( dest,"Zynine" );
			break;
		case 6:
			strcpy ( dest,"Medical Goods" );
			break;
		case 7:
			strcpy ( dest,"Vensium" );
			break;
		case 8:
			strcpy ( dest,"Gen Synth" );
			break;
		case 9:
			strcpy ( dest,"Gen Elium" );
			break;
	}

	return dest;
}

char *contra_name ( int contra_number, char *dest )
{
	switch ( contra_number )
	{
		case 0:
			strcpy ( dest,"Guns" );
			break;
		case 1:
			strcpy ( dest,"Kaos Germ" );
			break;
		case 2:
			strcpy ( dest,"Kytrilium" );
			break;
		case 3:
			strcpy ( dest,"Terraform Capsules" );
			break;
		case 4:
			strcpy ( dest,"Blood Packets" );
			break;
		case 5:
			strcpy ( dest,"Carnivore Pods" );
			break;
		case 6:
			strcpy ( dest,"Red Boosters" );
			break;
		case 7:
			strcpy ( dest,"Oxy Pills" );
			break;
		case 8:
			strcpy ( dest,"Net Infectors" );
			break;
		case 9:
			strcpy ( dest,"Body Parts" );
			break;
	}

	return dest;
}

char *good_description ( int good_number, char *dest )
{
	switch ( good_number )
	{
		case 0:
			strcpy ( dest,"Tynosine is a birefringent, tetragonal, scheelite-type crystal. It does form naturally but is now manufactured to increase availability and quality. Different dopants dramatically change its optical and electrical properties. For this reason it is widely used in circuitry ." );
			break;
		case 1:
			strcpy ( dest,"This mineral is highly radioactive and so is a valuable source of energy. It is used on spacecraft and installations through the galaxy as a core heating system." );
			break;
		case 2:
			strcpy ( dest,"A valuable naturally-occurring oil used as a fuel source. Demand for Cryonite has recently grown greater than its ability for self-recycling, leading to higher prices and environmental concerns." );
			break;
		case 3:
			strcpy ( dest,"Kengo Weed was found to have amazing nutritional properties with its high levels of protein, vitamins and rehydration capacity. It is now harvested and is the known planets' main food resource." );
			break;
		case 4:
			strcpy ( dest,"This is the strongest and most resilient metal on the known planets and so is widely used for manufacturing, especially in ship construction." );
			break;
		case 5:
			strcpy ( dest,"An inert liquid metal, Zynine is mainly used for manufacturing and lubrication. Its beauty means that it is also used as a display of wealth." );
			break;
		case 6:
			strcpy ( dest,"In these war-ridden times medical goods have become a valuable commodity. Hoarding of medicines is frequent, leading to prices fluctuating wildly. A war will always push the price up." );
			break;
		case 7:
			strcpy ( dest,"Archaeological excavations have shown that this stone was used as a form of currency on some planets. It has found new value as a precious metal and is used for jewellry and sculpture." );
			break;
		case 8:
			strcpy ( dest,"Despite ongoing research, it is a poor substitute for Gen Elium, but can still be useful in industry." );
			break;
		case 9:
			strcpy ( dest,"This is the material extracted from the relics by Bai Genko and named after his son and Peleus captain Bai Gen. Research into its properties has lead to improvements of jump drives and shielding. It is not found on any of the known planets, but mining conglomerates are investing heavily in the search for the planet it originated from." );
			break;
	}

	return dest;
}

int ship_total_good_ammount ( int ship_number )
{
	int i, temp_int = 0;

	for ( i=0;i<10;i++ )
	{
		temp_int += game.ship[ship_number].good_ammount[i];
		temp_int += game.ship[ship_number].contra_ammount[i];
	}

	return temp_int;
}

void guildlisting_display ( char *guild_name, char *website, char *owner, int bank_money, int bank_lend, char **rank_name, int **rank_allow )
{

}

void guildlisting_add_entry ( char *guild_name )
{

}

void guildlisting_clear_entries()
{
	int i;


}

void load_sounds()
{
	sound_explosion = make_sound ( "sounds/boom.wav" );
	sound_engine = make_sound ( "sounds/burner.wav" );
	sound_jump = make_sound ( "sounds/hyper.wav" );
	sound_warning = make_sound ( "sounds/warning.wav" );
	sound_gun = make_sound ( "sounds/gun.wav" );
	sound_enemy_gun[0] = make_sound ( "sounds/efire/g0.wav" );
	sound_enemy_gun[1] = make_sound ( "sounds/efire/g1.wav" );
	sound_missile = make_sound ( "sounds/missile.wav" );
	sound_shield = make_sound ( "sounds/shield.wav" );
	sound_button = make_sound ( "sounds/namepress.wav" );
	sound_impact[0] = make_sound ( "sounds/impacts/i1.wav" );
	sound_impact[1] = make_sound ( "sounds/impacts/i2.wav" );
	sound_impact[2] = make_sound ( "sounds/impacts/i3.wav" );
	sound_impact[3] = make_sound ( "sounds/impacts/i4.wav" );
	sound_impact[4] = make_sound ( "sounds/impacts/i5.wav" );
}

void stat_viewer_display_list()
{
	int i;
	SDL_Rect destination;

	destination.x = 357;

	for ( i=0;i<10;i++ )
	{
		if ( stat_viewer.list_class[i] == -1 ) continue;
		if ( stat_viewer.list_name_label[i].message[0] == '\0' ) continue;

		destination.y = stat_viewer.list_attr_label[i].location.y;

		draw_label ( screen,stat_viewer.list_name_label[i].message,&stat_viewer.list_name_label[i], 107, 216, 210 );
		draw_right_label ( screen,stat_viewer.list_attr_label[i].message,&stat_viewer.list_attr_label[i], 107, 216, 210 );
		SDL_BlitSurface ( stat_viewer.class_img[stat_viewer.list_class[i]], NULL, screen, &destination );
	}
}

void stat_viewer_left_deco()
{
	//check norm
	if ( stat_viewer.deco_chosen -1 < 0 ) return;

	//do deed
	stat_viewer.deco_chosen--;

	//draw
	stat_viewer_draw_deco ( stat_viewer.deco_chosen );

	//flip
	sdl_flip_mutex();
}

void stat_viewer_right_deco()
{
	//check user
	if ( stat_viewer.deco_chosen + 1 >= stat_viewer.deco_max ) return;

	//check norm
	if ( stat_viewer.deco_chosen + 1 >= MAX_DECO ) return;

	//do deed
	stat_viewer.deco_chosen++;

	//draw
	stat_viewer_draw_deco ( stat_viewer.deco_chosen );

	//flip
	sdl_flip_mutex();
}

void stat_viewer_draw_deco_left()
{
	if ( stat_viewer_is_left_deco() )
		SDL_BlitSurface ( stat_viewer.sel_left, NULL, screen, &stat_viewer.button[14] );
	else
		SDL_FillRect ( screen, &stat_viewer.button[14], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void stat_viewer_draw_deco_right()
{
	if ( stat_viewer_is_right_deco() )
		SDL_BlitSurface ( stat_viewer.sel_right, NULL, screen, &stat_viewer.button[15] );
	else
		SDL_FillRect ( screen, &stat_viewer.button[15], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void stat_viewer_draw_deco_img()
{
	int deco_num;
	SDL_Rect destination;

	destination.x = stat_viewer.button[14].x + 24;
	destination.y = stat_viewer.button[14].y - 6;
	destination.w = 105;
	destination.h = 30;

	deco_num = stat_viewer.deco_i[stat_viewer.deco_chosen];

	if ( !stat_viewer.deco_max )
	{
		//clear
		SDL_FillRect ( screen, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	}
	else
	{
		//load deco
		if ( load_deco ( deco_num ) )
		{
			//draw deco
			SDL_BlitSurface ( game.game_image.deco_img[deco_num], NULL, screen, &destination );
		}
	}
}

void stat_viewer_draw_deco ( int deco_num )
{
	char *temp_str = "";

	//set
	stat_viewer.deco_chosen = deco_num;

	//buttons
	stat_viewer_draw_deco_right();
	stat_viewer_draw_deco_left();

	//draw this shit
	stat_viewer_draw_deco_img();

	//draw text
	if ( stat_viewer.deco_max )
		draw_label ( screen,stat_viewer.deco_name[deco_num],&stat_viewer.deco_label, 107, 216, 210 );
	else
		draw_label ( screen,temp_str,&stat_viewer.deco_label, 107, 216, 210 );
}

int stat_viewer_is_left_deco()
{
	if ( stat_viewer.deco_chosen )
		return 1;
	else
		return 0;
}

int stat_viewer_is_right_deco()
{
	if ( stat_viewer.deco_chosen + 1 >= stat_viewer.deco_max ) return 0;

	//check norm
	if ( stat_viewer.deco_chosen + 1 >= MAX_DECO ) return 0;

	return 1;
}

void stat_viewer_draw_pointer()
{
	SDL_Rect destination;

	destination.x = 436;
	destination.y = 67;
	destination.w = 240;
	destination.h = 100;

	//clear out the monkey
	SDL_BlitSurface ( stat_viewer.backround, &destination, screen, &destination );

	//draw the monkey
	destination.x = 552;
	destination.y = 98;

	destination.x -= 20; //half the pointer width
	destination.x += stat_viewer.player_class_points; //half the pointer width

	//draw it
	SDL_BlitSurface ( stat_viewer.pointer_img, NULL, screen, &destination );
}

void stat_viewer_clear_ship_area()
{
	SDL_Rect destination;

	destination.x = 87;
	destination.y = 335;
	destination.w = 280;
	destination.h = 190;

	SDL_FillRect ( screen, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

	sdl_flip_mutex();
}

void hub_retrive_user_stats ( char *message )
{
	int point = 0, len;
	char temp_str[1025];
	int exp;

	len = strlen ( message );

	if ( !stat_viewer.visible ) return;

	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.player_name_label, 107, 216, 210 );

	//already cleared so clear it all
	if ( point>=len )
	{
		draw_label ( screen,"",&stat_viewer.guild_name_label, 107, 216, 210 );
		draw_label ( screen,"",&stat_viewer.guild_rank_label, 107, 216, 210 );

		draw_label ( screen,"",&stat_viewer.player_worth_label, 107, 216, 210 );
		draw_label ( screen,"",&stat_viewer.player_exp_label, 107, 216, 210 );
		draw_label ( screen,"",&stat_viewer.player_kills_label, 107, 216, 210 );

		draw_label ( screen,"",&stat_viewer.player_home_planet_label, 107, 216, 210 );
		draw_label ( screen,"",&stat_viewer.player_home_server_label, 107, 216, 210 );
		draw_label ( screen,"",&stat_viewer.player_last_server_label, 107, 216, 210 );

		draw_label ( screen,"",&stat_viewer.player_online_label, 107, 216, 210 );
		draw_label ( screen,"",&stat_viewer.player_actvated_label, 107, 216, 210 );

		stat_viewer.player_class_points = 0;
		stat_viewer_draw_pointer();

		sdl_flip_mutex();

		return;
	}

	//fun!
	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.guild_name_label, 107, 216, 210 );
	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.guild_rank_label, 107, 216, 210 );

	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.player_worth_label, 107, 216, 210 );
	split ( temp_str,message,',',&point );
	exp = atoi ( temp_str );
	draw_label ( screen,temp_str,&stat_viewer.player_exp_label, 107, 216, 210 );
	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.player_kills_label, 107, 216, 210 );

	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.player_home_planet_label, 107, 216, 210 );
	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.player_home_server_label, 107, 216, 210 );
	split ( temp_str,message,',',&point );
	draw_label ( screen,temp_str,&stat_viewer.player_last_server_label, 107, 216, 210 );

	split ( temp_str,message,',',&point );
	stat_viewer.player_online = atoi ( temp_str );
	split ( temp_str,message,',',&point );
	stat_viewer.player_activated = atoi ( temp_str );

	if ( stat_viewer.player_online )
		draw_label ( screen,"Yes",&stat_viewer.player_online_label, 0,150,0 );
	else
		draw_label ( screen,"No",&stat_viewer.player_online_label, 150, 0, 0 );

	if ( stat_viewer.player_activated )
		draw_label ( screen,"Yes",&stat_viewer.player_actvated_label, 0,150,0 );
	else
		draw_label ( screen,"No",&stat_viewer.player_actvated_label, 150, 0, 0 );


	split ( temp_str,message,',',&point );
	stat_viewer.s_id = atoi ( temp_str );
	split ( temp_str,message,',',&point );
	stat_viewer.s_type = atoi ( temp_str );
	split ( temp_str,message,',',&point );
	stat_viewer.s_kind = atoi ( temp_str );
	split ( temp_str,message,',',&point );
	temp_str[20] = 0;
	strcpy ( stat_viewer.s_name,temp_str );

	split ( temp_str,message,',',&point );
	stat_viewer.player_class_points = atoi ( temp_str );

	//now load in deco's
	for ( stat_viewer.deco_max = 0; point<len; stat_viewer.deco_max++ )
	{
		split ( temp_str,message,',',&point );
		stat_viewer.deco_i[stat_viewer.deco_max] = atoi ( temp_str );
		split ( temp_str,message,',',&point );
		temp_str[30] = 0;
		strcpy ( stat_viewer.deco_name[stat_viewer.deco_max], temp_str );
	}

	//check shit
	if ( stat_viewer.s_id < 0 ) stat_viewer.s_id = -1;
	if ( stat_viewer.s_type < 0 ) stat_viewer.s_type = -1;
	if ( stat_viewer.s_kind < 0 ) stat_viewer.s_kind = -1;
	if ( stat_viewer.s_id >= MAX_SERVER ) stat_viewer.s_id = -1;
	if ( stat_viewer.s_type >= SHIP_MAX ) stat_viewer.s_type = -1;
	if ( stat_viewer.s_kind >= 8 ) stat_viewer.s_kind = -1;

	if ( stat_viewer.s_id != -1 && stat_viewer.s_type != -1 && stat_viewer.s_kind != -1 )
		strcpy ( server[stat_viewer.s_id].ship[stat_viewer.s_type].name, stat_viewer.s_name );;

	if ( stat_viewer.player_class_points < -100 ) stat_viewer.player_class_points = -100;
	if ( stat_viewer.player_class_points > 100 ) stat_viewer.player_class_points = 100;

	stat_viewer.load_ship = 1;

	stat_viewer_draw_pointer();

	//ye
	draw_exp_rank_img ( exp, 655, 210 );

	//YE
	stat_viewer_draw_deco ( 0 );

	sdl_flip_mutex();
}

void hub_retrive_stats_list ( char *message )
{
	int i;
	char temp_str[3][1025];
	int point = 0, len;
	SDL_Rect destination;

	len = strlen ( message );

	destination.x = 357;

	for ( i=0;i<10;i++ )
	{
		temp_str[0][0] = 0;
		temp_str[1][0] = 0;
		temp_str[2][0] = 0;
		if ( point < len )
		{
			//collect
			split ( temp_str[0],message,',',&point );
			split ( temp_str[1],message,',',&point );
			split ( temp_str[2],message,',',&point );
		}

		if ( !temp_str[0][0] ) //nooopppeee
			continue;

		stat_viewer.list_class[i] = atoi ( temp_str[2] );

		//checks....
		if ( stat_viewer.list_class[i] > 1 ) stat_viewer.list_class[i] = 1;
		if ( stat_viewer.list_class[i] < 0 ) stat_viewer.list_class[i] = 0;

		destination.y = stat_viewer.list_attr_label[i].location.y;

		draw_label ( screen,temp_str[0],&stat_viewer.list_name_label[i], 107, 216, 210 );
		draw_right_label ( screen,temp_str[1],&stat_viewer.list_attr_label[i], 107, 216, 210 );

		SDL_BlitSurface ( stat_viewer.class_img[stat_viewer.list_class[i]], NULL, screen, &destination );
	}

	sdl_flip_mutex();
}


void bounty_draw_raw_lists()
{
	int i;
	const int left_x = 85, right_x = 418, interval_x1 = 180 - left_x, interval_x2 = 277 - left_x;
	const int start_y = 90, interval_y = 20;
	SDL_Rect destination;


	for ( i=0;i<bounty.list_max;i++ )
	{
		if ( i<19 )
		{
			destination.x = left_x + interval_x2;
			destination.y = start_y + ( i * interval_y );
		}
		else
		{
			destination.x = right_x + interval_x2;
			destination.y = start_y + ( ( i-19 ) * interval_y );
		}


		//name
		draw_label ( screen,bounty.name_list_label[i].message,&bounty.name_list_label[i], 107, 216, 210 );

		//price
		draw_right_label ( screen,bounty.price_list_label[i].message,&bounty.price_list_label[i], 107, 216, 210 );

		//box back
		SDL_BlitSurface ( bounty.bounty_text_img, NULL, screen, &destination );

		//text box
		if ( bounty.text_box[i].selected )
			select_text_box ( screen, &bounty.text_box[i] );
		else
			unselect_text_box ( screen, &bounty.text_box[i] );
	}
}

void bounty_load_list ( char *message )
{
	int i, k, point = 0, len;
	char temp_str[38][1025];
	char temp_price_str[38][1025];
	int temp_price[38];
	int new_order[38];
	SDL_Rect clear_space;

	clear_space.x = 83;
	clear_space.y = 87;
	clear_space.w = 715 - 83;
	clear_space.h = 480 - 87;

	//clear list space
	SDL_FillRect ( screen,&clear_space,SDL_MapRGBA ( screen->format,0,0,0,255 ) );
	
	//load the list
	len = strlen ( message );
	for ( k=i=0;k<38 && point <= len-1; k++)
	{
		split ( temp_str[i],message,',',&point );
		split ( temp_price_str[i],message,',',&point );

		if ( !temp_str[i][0] || !temp_price_str[i][0] ) continue;

		//cappy
		temp_str[i][20] = 0;
		temp_price_str[i][20] = 0;

		new_order[i] = i;

		temp_price[i] = atoi ( temp_price_str[i] );
		
		i++;
	}
	
	bounty.list_max = i;

	//reorder
	for ( i=0;i<bounty.list_max - 1;i++ )
		for ( k=i+1;k<bounty.list_max;k++ )
			if ( temp_price[new_order[i]] < temp_price[new_order[k]] )
			{
				int swap_new_order;

				swap_new_order = new_order[i];
				new_order[i] = new_order[k];
				new_order[k] = swap_new_order;
			}
	
	//copy
	for ( i=0;i<bounty.list_max;i++ )
	{
		strcpy ( bounty.name_list_label[i].message,temp_str[new_order[i]] );
		strcpy ( bounty.price_list_label[i].message,temp_price_str[new_order[i]] );
	}
	
	//draw the list
	bounty_draw_raw_lists();
	
	//flip it
	sdl_flip_mutex();
}

void bounty_request()
{
	char rtn_str[10] = "26,";

	send_con_hub ( rtn_str );
}

void bounty_add ( char *victom_name, int price )
{
	char rtn_str[500];

	sprintf ( rtn_str,"27,%s,%d",victom_name,price );
	send_con_hub ( rtn_str );
}

void bounty_clear_all_text_boxes()
{
	int i;

	for ( i=0;i<40;i++ )
		clear_text_box ( NULL, &bounty.text_box[i] );

	bounty.text_box[38].selected = 1;
	bounty.status_label.message[0] = '\0';
}

void guild_editor_reset_variables()
{
	int i, k;

	strcpy ( guild_editor.website, game.guild.website );
	guild_editor.bank_money = game.guild.bank_money;
	guild_editor.bank_lend = game.guild.bank_lend;

	for ( i=0;i<6;i++ )
		strcpy ( guild_editor.rank_name[i], game.guild.rank_name[i] );

	for ( i=0;i<30;i++ )
	{
		int h, v;

		k = i + 2;

		h = i % 6;
		v = i / 6;

		guild_editor.rank_allow[h][v] = game.guild.rank_allow[h][v];
	}
}

int load_deco ( int deco_num )
{
	char file_name[300];

	if ( !game.game_image.deco_img[deco_num] )
	{
		sprintf ( file_name,"graphics/deco/deco%d.jpg",deco_num );

		game.game_image.deco_img[deco_num] = IMG_Load ( file_name );
	}

	if ( game.game_image.deco_img[deco_num] )
		return 1;
	else
		return 0;
}

void guild_editor_more_next_deco()
{
	if ( guild_editor_more.deco_choosen + 1 >= MAX_DECO )
		return;

	guild_editor_more.deco_choosen++;

	guild_editor_more_draw_deco ( guild_editor_more.deco_choosen );

	sdl_flip_mutex();
}

void guild_editor_more_previous_deco()
{
	if ( guild_editor_more.deco_choosen - 1 < 0 )
		return;

	guild_editor_more.deco_choosen--;

	guild_editor_more_draw_deco ( guild_editor_more.deco_choosen );

	sdl_flip_mutex();
}

int guild_editor_more_deco_changes()
{
	int i;

	i = guild_editor_more.deco_choosen;

	//check if on / off is different
	if ( game.guild.deco_activated[i] != guild_editor_more.deco_activated )
		return 1;

	//check if description is different
	if ( strcmp ( guild_editor_more.text_box[3].message, game.guild.deco_name[i] ) )
		return 1;

	//do no changes
	return 0;
}

void guild_editor_more_draw_save_changes()
{
	//now which
	if ( guild_editor_more_deco_changes() )
		SDL_BlitSurface ( guild_editor_more.save_settings, NULL, screen, &guild_editor_more.button[5] );
	else
		SDL_FillRect ( screen, &guild_editor_more.button[5], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void guild_editor_more_send_deco_changes()
{
	char rtn_str[500];

	sprintf ( rtn_str,"46,%d,%s,%d", guild_editor_more.deco_choosen, guild_editor_more.text_box[3].message, guild_editor_more.deco_activated );

	send_con_hub ( rtn_str );
}

void guild_editor_more_toggle_deco()
{
	if ( guild_editor_more.deco_activated )
		guild_editor_more.deco_activated = 0;
	else
		guild_editor_more.deco_activated = 1;

	guild_editor_draw_deco_onoff();

	guild_editor_more_draw_save_changes();

	sdl_flip_mutex();
}

void guild_editor_draw_deco_onoff()
{
	if ( guild_editor_more.deco_activated )
		SDL_BlitSurface ( guild_editor_more.on[1], NULL, screen, &guild_editor_more.button[4] );
	else
		SDL_BlitSurface ( guild_editor_more.on[0], NULL, screen, &guild_editor_more.button[4] );
}

void guild_editor_more_draw_deco ( int deco_num )
{
	//check
	if ( deco_num < 0 )
		deco_num = 0;
	if ( deco_num >= MAX_DECO )
		deco_num = MAX_DECO - 1;

	//set the num
	guild_editor_more.deco_choosen = deco_num;

	//set the texts and such
	guild_editor_more.deco_activated = game.guild.deco_activated[deco_num];
	set_text_box_text ( screen, &guild_editor_more.text_box[3], game.guild.deco_name[deco_num] );


	//draw buttons
	if ( deco_num )
		SDL_BlitSurface ( guild_editor_more.sel_left, NULL, screen, &guild_editor_more.button[2] );
	else
		SDL_FillRect ( screen, &guild_editor_more.button[2], SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	if ( deco_num<MAX_DECO - 1 )
		SDL_BlitSurface ( guild_editor_more.sel_right, NULL, screen, &guild_editor_more.button[3] );
	else
		SDL_FillRect ( screen, &guild_editor_more.button[3], SDL_MapRGB ( screen->format, 0, 0, 0 ) );

	//load deco
	if ( load_deco ( deco_num ) )
	{
		SDL_Rect destination;

		destination.x = 244;
		destination.y = 290;

		//draw deco
		SDL_BlitSurface ( game.game_image.deco_img[deco_num], NULL, screen, &destination );
	}

	guild_editor_draw_deco_onoff();

	guild_editor_more_draw_save_changes();
}

void guild_editor_more_reset_text_boxes()
{
	int i;
	const int text_box_max = 4;


	set_text_box_text ( NULL, &guild_editor_more.text_box[0], game.guild.enemy[2] );
	set_text_box_text ( NULL, &guild_editor_more.text_box[1], game.guild.enemy[1] );
	set_text_box_text ( NULL, &guild_editor_more.text_box[2], game.guild.enemy[0] );

	guild_editor_more.text_box[0].selected = 1;

	select_text_box ( screen, &guild_editor_more.text_box[0] );
	for ( i=1;i<text_box_max;i++ )
		unselect_text_box ( screen, &guild_editor_more.text_box[i] );
}

void guild_editor_reset_text_boxes()
{
	int i;
	const int text_box_max = 8;
	char temp_str[50];

	if ( !guild_editor.visible ) return;

	sprintf ( temp_str, "%d", guild_editor.bank_lend );

	set_text_box_text ( NULL, &guild_editor.text_box[0], guild_editor.website );
	set_text_box_text ( NULL, &guild_editor.text_box[1], temp_str );

	for ( i=0;i<6;i++ )
		set_text_box_text ( NULL, &guild_editor.text_box[9 - ( i+2 ) ], guild_editor.rank_name[i] );

	guild_editor.text_box[0].selected = 1;

	select_text_box ( screen, &guild_editor.text_box[0] );
	for ( i=1;i<text_box_max;i++ )
		unselect_text_box ( screen, &guild_editor.text_box[i] );
}

void guild_editor_reset_toggles()
{
	int i;

	for ( i=0;i<30;i++ )
	{
		int k, h, v;

		k = i + 2;

		h = i % 6;
		v = i / 6;

		if ( guild_editor.rank_allow[h][v] )
			SDL_BlitSurface ( game.game_image.guild_toggle_on, NULL, screen, &guild_editor.button[k] );
		else
			SDL_BlitSurface ( game.game_image.guild_toggle_off, NULL, screen, &guild_editor.button[k] );
	}
}

void guild_editor_toggle_button ( int i )
{
	int k, h, v;

	h = ( i-2 ) % 6;
	v = ( i-2 ) / 6;

	if ( guild_editor.rank_allow[h][v] )
	{
		SDL_BlitSurface ( game.game_image.guild_toggle_off, NULL, screen, &guild_editor.button[i] );
		guild_editor.rank_allow[h][v] = 0;
	}
	else
	{
		SDL_BlitSurface ( game.game_image.guild_toggle_on, NULL, screen, &guild_editor.button[i] );
		guild_editor.rank_allow[h][v] = 1;
	}
}

void guild_editor_more_send_save()
{
	char rtn_str[1000];

	sprintf ( rtn_str, "36,%s,%s,%s",
	          guild_editor_more.text_box[2].message,
	          guild_editor_more.text_box[1].message,
	          guild_editor_more.text_box[0].message );

	send_con_hub ( rtn_str );
}

void guild_editor_send_save()
{
	char rtn_str[1000];

	sprintf ( rtn_str, "21,%s,%d,%s,%s,%s,%s,%s,%s,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d",
	          //guild_editor.name,
	          //guild_editor.owner,
	          guild_editor.text_box[0].message,
	          //guild_editor.bank_money,
	          atoi ( guild_editor.text_box[1].message ),
	          guild_editor.text_box[7].message,
	          guild_editor.text_box[6].message,
	          guild_editor.text_box[5].message,
	          guild_editor.text_box[4].message,
	          guild_editor.text_box[3].message,
	          guild_editor.text_box[2].message,
	          guild_editor.rank_allow[0][0],
	          guild_editor.rank_allow[0][1],
	          guild_editor.rank_allow[0][2],
	          guild_editor.rank_allow[0][3],
	          guild_editor.rank_allow[0][4],
	          guild_editor.rank_allow[1][0],
	          guild_editor.rank_allow[1][1],
	          guild_editor.rank_allow[1][2],
	          guild_editor.rank_allow[1][3],
	          guild_editor.rank_allow[1][4],
	          guild_editor.rank_allow[2][0],
	          guild_editor.rank_allow[2][1],
	          guild_editor.rank_allow[2][2],
	          guild_editor.rank_allow[2][3],
	          guild_editor.rank_allow[2][4],
	          guild_editor.rank_allow[3][0],
	          guild_editor.rank_allow[3][1],
	          guild_editor.rank_allow[3][2],
	          guild_editor.rank_allow[3][3],
	          guild_editor.rank_allow[3][4],
	          guild_editor.rank_allow[4][0],
	          guild_editor.rank_allow[4][1],
	          guild_editor.rank_allow[4][2],
	          guild_editor.rank_allow[4][3],
	          guild_editor.rank_allow[4][4],
	          guild_editor.rank_allow[5][0],
	          guild_editor.rank_allow[5][1],
	          guild_editor.rank_allow[5][2],
	          guild_editor.rank_allow[5][3],
	          guild_editor.rank_allow[5][4] );
	send_con_hub ( rtn_str );
}

void guild_listing_sort_name()
{
	int i, k, c;

	guild_listing.sort_method = c = 0;

	//clear the list
	for ( i=0;i<guild_listing.max_entry;i++ )
		guild_listing.sort_list[c][i] = i;

	//resort
	for ( i=0;i<guild_listing.max_entry;i++ )
		for ( k=i;k<guild_listing.max_entry;k++ )
			if ( strcmp ( guild_listing.guild_name[guild_listing.sort_list[c][i]],guild_listing.guild_name[guild_listing.sort_list[c][k]] ) > 0 )
			{
				int swap;

				swap = guild_listing.sort_list[c][i];
				guild_listing.sort_list[c][i] = guild_listing.sort_list[c][k];
				guild_listing.sort_list[c][k] = swap;
			}

	//now redraw the list
	guild_listing_draw_list();

	//flip
	sdl_flip_mutex();
}

void guild_listing_sort_worth()
{
	int i, k, c;

	guild_listing.sort_method = c = 1;

	//clear the list
	for ( i=0;i<guild_listing.max_entry;i++ )
		guild_listing.sort_list[c][i] = i;

	//resort
	for ( i=0;i<guild_listing.max_entry;i++ )
		for ( k=i;k<guild_listing.max_entry;k++ )
			if ( guild_listing.guild_worth[guild_listing.sort_list[c][i]] < guild_listing.guild_worth[guild_listing.sort_list[c][k]] )
			{
				int swap;

				swap = guild_listing.sort_list[c][i];
				guild_listing.sort_list[c][i] = guild_listing.sort_list[c][k];
				guild_listing.sort_list[c][k] = swap;
			}

	//now redraw the list
	guild_listing_draw_list();

	//flip
	sdl_flip_mutex();
}

void guild_listing_sort_players()
{
	int i, k, c;

	guild_listing.sort_method = c = 2;

	//clear the list
	for ( i=0;i<guild_listing.max_entry;i++ )
		guild_listing.sort_list[c][i] = i;

	//resort
	for ( i=0;i<guild_listing.max_entry;i++ )
		for ( k=i;k<guild_listing.max_entry;k++ )
			if ( guild_listing.guild_players[guild_listing.sort_list[c][i]] < guild_listing.guild_players[guild_listing.sort_list[c][k]] )
			{
				int swap;

				swap = guild_listing.sort_list[c][i];
				guild_listing.sort_list[c][i] = guild_listing.sort_list[c][k];
				guild_listing.sort_list[c][k] = swap;
			}

	//now redraw the list
	guild_listing_draw_list();

	//flip
	sdl_flip_mutex();
}

void guild_listing_previous()
{
	//can we go previous?
	if ( guild_listing.list_start <= 0 ) return;

	//set new start
	guild_listing.list_start -= MAX_GUILD_LISTING;

	//do a check
	if ( guild_listing.list_start < 0 ) guild_listing.list_start = 0;

	//draw the mofo!
	guild_listing_draw_list();

	//flip
	sdl_flip_mutex();
}

void guild_listing_next()
{
	//can we go next?
	if ( guild_listing.list_start + MAX_GUILD_LISTING >= guild_listing.max_entry ) return;

	//set new start
	guild_listing.list_start += MAX_GUILD_LISTING;

	//draw the mofo!
	guild_listing_draw_list();

	//flip
	sdl_flip_mutex();
}

void guild_listing_draw_guild()
{
	char money_str[20], lend_str[20], player_str[20];
	int i, h, v;

	if ( !guild_listing.visible ) return;

	sprintf ( money_str, "%d", guild_listing.bank_money );
	sprintf ( lend_str, "%d", guild_listing.bank_lend );
	sprintf ( player_str, "%d", guild_listing.players );

	draw_label ( screen, guild_listing.selected_guild_name, &guild_listing.guild_name_label, 15, 116, 102 );
	draw_label ( screen, guild_listing.owner_name, &guild_listing.guild_owner_label, 15, 116, 102 );
	draw_label ( screen, guild_listing.website, &guild_listing.guild_website_label, 15, 116, 102 );
	draw_label ( screen, money_str, &guild_listing.guild_money_label, 15, 116, 102 );
	draw_label ( screen, lend_str, &guild_listing.guild_lend_label, 15, 116, 102 );
	draw_label ( screen, player_str, &guild_listing.guild_players_label, 15, 116, 102 );

	draw_label ( screen, guild_listing.guild_enemy[0], &guild_listing.guild_enemy_label[0], 15, 116, 102 );
	draw_label ( screen, guild_listing.guild_enemy[1], &guild_listing.guild_enemy_label[1], 15, 116, 102 );
	draw_label ( screen, guild_listing.guild_enemy[2], &guild_listing.guild_enemy_label[2], 15, 116, 102 );

	draw_label ( screen, guild_listing.rank_name[0], &guild_listing.guild_rank_label[0], 15, 116, 102 );
	draw_label ( screen, guild_listing.rank_name[1], &guild_listing.guild_rank_label[1], 15, 116, 102 );
	draw_label ( screen, guild_listing.rank_name[2], &guild_listing.guild_rank_label[2], 15, 116, 102 );
	draw_label ( screen, guild_listing.rank_name[3], &guild_listing.guild_rank_label[3], 15, 116, 102 );
	draw_label ( screen, guild_listing.rank_name[4], &guild_listing.guild_rank_label[4], 15, 116, 102 );
	draw_label ( screen, guild_listing.rank_name[5], &guild_listing.guild_rank_label[5], 15, 116, 102 );

	for ( i=0;i<30;i++ )
	{
		SDL_Rect destination;

		h = i % 6;
		v = i / 6;

		destination.x = 481 + ( v * 53 );
		destination.y = 317 + ( ( 5-h ) * 23 );
		destination.w = 20;
		destination.h = 20;

		if ( guild_listing.rank_allow[h][v] )
			SDL_BlitSurface ( game.game_image.guild_toggle_on, NULL, screen, &destination );
		else
			SDL_BlitSurface ( game.game_image.guild_toggle_off, NULL, screen, &destination );
	}
}

void guild_listing_draw_list()
{
	int i, skew;

	for ( i=0,skew=guild_listing.list_start;i<MAX_GUILD_LISTING;i++,skew++ )
	{
		int c;

		if ( skew>=guild_listing.max_entry )
		{
			draw_label ( screen, "", &guild_listing.guild_listing_label[i], 107, 216, 210 );
			continue;
		}

		c = guild_listing.sort_list[guild_listing.sort_method][skew];

		if ( guild_listing.sort_method == -1 )
			c = skew;
		else
			c = guild_listing.sort_list[guild_listing.sort_method][skew];

		if ( !strcmp ( guild_listing.guild_name[c], guild_listing.selected_guild_name ) )
			draw_label ( screen, guild_listing.guild_name[c], &guild_listing.guild_listing_label[i], 255, 255, 0 );
		else
			draw_label ( screen, guild_listing.guild_name[c], &guild_listing.guild_listing_label[i], 107, 216, 210 );
	}

	//draw our buttons
	guild_listing_draw_previous_next();
}

void guild_listing_request_list()
{
	char rtn_str[10] = "20,";

	send_con_hub ( rtn_str );
}

void guild_listing_reset_variables()
{
	guild_listing.sort_method = -1;
	guild_listing.list_start = 0;

	clear_text_box ( NULL, &guild_listing.text_box );
}

void guild_listing_select_guild ( int i )
{
	char rtn_str[500];

	sprintf ( rtn_str, "28,%s", guild_listing.guild_listing_label[i].message );
	send_con_hub ( rtn_str );
}

void guild_listing_draw_previous_next()
{

	//next?
	if ( guild_listing.list_start + MAX_GUILD_LISTING >= guild_listing.max_entry )
		SDL_BlitSurface ( guild_listing.next_off, NULL, screen, &guild_listing.button[5] );
	else
		SDL_BlitSurface ( guild_listing.next_on, NULL, screen, &guild_listing.button[5] );

	//prev?
	if ( !guild_listing.list_start )
		SDL_BlitSurface ( guild_listing.prev_off, NULL, screen, &guild_listing.button[4] );
	else
		SDL_BlitSurface ( guild_listing.prev_on, NULL, screen, &guild_listing.button[4] );
}

void guild_listing_select_new_guild ( char *new_name )
{
	int i, sm;

	sm = guild_listing.sort_method;

	if ( sm == -1 )
	{
		//find a "clear" old selection
		for ( i=guild_listing.list_start;i<guild_listing.max_entry && i<guild_listing.list_start+MAX_GUILD_LISTING;i++ )
			if ( !strcmp ( guild_listing.guild_name[i], guild_listing.selected_guild_name ) )
				draw_label ( screen, guild_listing.guild_name[i], &guild_listing.guild_listing_label[i-guild_listing.list_start], 107, 216, 210 );

		//find new selection
		for ( i=guild_listing.list_start;i<guild_listing.max_entry && i<guild_listing.list_start+MAX_GUILD_LISTING;i++ )
			if ( !strcmp ( guild_listing.guild_name[i], new_name ) )
				draw_label ( screen, new_name, &guild_listing.guild_listing_label[i-guild_listing.list_start], 255, 255, 0 );
	}
	else
	{
		//find a "clear" old selection
		for ( i=guild_listing.list_start;i<guild_listing.max_entry && i<guild_listing.list_start+MAX_GUILD_LISTING;i++ )
			if ( !strcmp ( guild_listing.guild_name[guild_listing.sort_list[sm][i]], guild_listing.selected_guild_name ) )
				draw_label ( screen, guild_listing.guild_name[guild_listing.sort_list[sm][i]], &guild_listing.guild_listing_label[i-guild_listing.list_start], 107, 216, 210 );

		//find new selection
		for ( i=guild_listing.list_start;i<guild_listing.max_entry && i<guild_listing.list_start+MAX_GUILD_LISTING;i++ )
			if ( !strcmp ( guild_listing.guild_name[guild_listing.sort_list[sm][i]], new_name ) )
				draw_label ( screen, new_name, &guild_listing.guild_listing_label[i-guild_listing.list_start], 255, 255, 0 );
	}
}

void bar_select_player ( int i )
{
	//have we actaully even selected someone?
	if ( !bar.playername[i][0] ) return;

	//"unselect" selected
	bar_unselect();

	//set the i
	bar.selected_i = i;

	//"select" selected
	bar_select();

	//clear out selection type
	bar_clear_selection_type();

	//set selection type
	bar.selection_fresh = 1;

	//draw according name tag
	bar_draw_name_tag();

	//draw according buttons
	bar_draw_buttons();

	//flip the monkey
	sdl_flip_mutex();
}

void bar_clear_selection_type()
{
	bar.selection_fresh = 0;
	bar.selection_remove = 0;
	bar.selection_demote = 0;
	bar.selection_enlist = 0;
	bar.selection_transfer = 0;
}

void bar_draw_name_tag()
{
	char selection_name[200];
	int i;

	i = bar.selected_i;

	sprintf ( selection_name,"%s : %s of %s", bar.playername[i], bar.playerguild_rank[i], bar.playerguild[i] );

	//draw the label
	draw_label ( screen, selection_name, &bar.label, 107, 216, 210 );
}

void bar_draw_buttons()
{
	SDL_Surface *but1, *but2, *but3;

	//the clear monkeys
	but1 = bar.but1_clear;
	but2 = bar.but2_clear;
	but3 = bar.but3_clear;

	if ( bar.selection_fresh )
	{
		but1 = bar.but1_transfer;
		but2 = bar.but2_guild;
		but3 = bar.but3_chat;
	}

	if ( bar.selection_remove )
	{
		but1 = bar.but1_promote;
		but2 = bar.but2_remove;
		but3 = bar.but3_cancel;
	}

	if ( bar.selection_demote )
	{
		but1 = bar.but1_promote;
		but2 = bar.but2_demote;
		but3 = bar.but3_cancel;
	}

	if ( bar.selection_enlist )
	{
		but1 = bar.but1_enlist;
		but2 = bar.but2_clear;
		but3 = bar.but3_cancel;
	}

	if ( bar.selection_transfer )
	{
		but1 = bar.but1_transfer;
		but2 = bar.but2_clear;
		but3 = bar.but3_cancel;
	}

	//now for the etching in the rock face
	SDL_BlitSurface ( but1, NULL, screen, &bar.button[0] );
	SDL_BlitSurface ( but2, NULL, screen, &bar.button[1] );
	SDL_BlitSurface ( but3, NULL, screen, &bar.button[2] );
}

void bar_unselect()
{
	int i;

	i = bar.selected_i;

	//reasons to break
	if ( i == -1 ) return;
	if ( !bar.playername[i][0] ) return;

	draw_label ( screen, bar.playername[i], &bar.player_label[i], 255, 255, 255 );
}

void bar_select()
{
	int i;

	i = bar.selected_i;

	draw_label ( screen, bar.playername[i], &bar.player_label[i], 255, 255, 0 );
}

void bar_button_choice ( int b )
{
	int draw_n_flip = 0;


	switch ( b )
	{
		case 0:
			if ( bar.selection_fresh ) //transfer
			{
				bar_clear_selection_type();
				bar.selection_transfer = 1;
				draw_n_flip = 1;

				clear_text_box ( NULL, &bar.text_box[2] );
				break;
			}

			if ( bar.selection_enlist ) //enlist
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				guild_enlist_user ( bar.playername[bar.selected_i] );
				break;
			}

			if ( bar.selection_demote ) //promote
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				guild_promote_user ( bar.playername[i] );
				break;
			}

			if ( bar.selection_remove ) //promote
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				guild_promote_user ( bar.playername[i] );
				break;
			}

			if ( bar.selection_transfer ) //send transfer
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				transfer_money_to ( bar.playername[i],atoi ( bar.text_box[2].message ) );
				break;
			}
			break;
		case 1:
			if ( bar.selection_fresh ) //guild
			{
				bar_clear_selection_type();
				//make the proper choice
				bar_correct_guild_select();
				draw_n_flip = 1;
				break;
			}

			if ( bar.selection_enlist ) //nothing.
			{
				break;
			}

			if ( bar.selection_demote ) //demote
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				guild_demote_user ( bar.playername[i] );
				break;
			}

			if ( bar.selection_remove ) //remove
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				guild_remove_user ( bar.playername[i] );
				break;
			}

			if ( bar.selection_transfer ) //select textbox 3
			{
				int k, i;
				const int text_box_max = 3;

				i = 2;

				//already selected ?
				if ( bar.text_box[i].selected ) return;

				//transfer box?
				if ( i==2 && !bar_transfer_box_visible() ) return;

				//unselect those who are
				for ( k=0;k<text_box_max;k++ )
					if ( bar.text_box[k].selected )
						unselect_text_box ( screen, &bar.text_box[k] );

				//select the lover
				select_text_box ( screen, &bar.text_box[i] );

				//flip the mother
				sdl_flip_mutex();
				break;
			}
			break;
		case 2:
			if ( bar.selection_fresh ) //chat
			{
				int i;

				i = bar.selected_i;

				if ( i==-1 ) return;

				do_chat_button ( bar.playername[i] );

				break;
			}

			if ( bar.selection_enlist ) //cancel
			{
				bar_clear_selection_type();
				bar.selection_fresh = 1;
				draw_n_flip = 1;
				break;
			}

			if ( bar.selection_demote ) //cancel
			{
				bar_clear_selection_type();
				bar.selection_fresh = 1;
				draw_n_flip = 1;
				break;
			}

			if ( bar.selection_remove ) //cancel
			{
				bar_clear_selection_type();
				bar.selection_fresh = 1;
				draw_n_flip = 1;
				break;
			}

			if ( bar.selection_transfer ) //cancel
			{
				bar_clear_selection_type();
				bar.selection_fresh = 1;
				draw_n_flip = 1;
				break;
			}
			break;
	}

	if ( draw_n_flip )
	{
		bar_draw_buttons();

		sdl_flip_mutex();
	}
}

void bar_correct_guild_select()
{
	int i, r;

	i = bar.selected_i;
	r = game.guild_rank_id;

	//check?
	if ( i == -1 ) return;

	//first check if in same guild
	if ( !strcmp ( bar.playerguild[i], game.guild.name ) )
	{
		//same guild, so can we demote?
		if ( bar.playerguild_rank_id[i] && game.guild.rank_allow[r][1] ) bar.selection_demote = 1;

		//what about remove?
		//if(bar.playerguild_rank_id[i] == 5 && r == 5 && game.guild.rank_allow[r][2]) bar.selection_remove = 1;
		if ( bar.playerguild_rank_id[i] == 0 && game.guild.rank_allow[r][2] ) bar.selection_remove = 1;
	}
	else
	{
		//not same guild, can we enlist?
		if ( game.guild.rank_allow[r][0] ) bar.selection_enlist = 1;
	}

	//well were we able to make a choice?
	if ( !bar.selection_enlist && !bar.selection_demote && !bar.selection_remove ) bar.selection_fresh = 1;
}

int bar_transfer_box_visible()
{
	if ( bar.selection_transfer ) return 1;

	return 0;
}

void bar_clear_selection()
{
	bar_clear_selection_type();

	bar_draw_buttons();

	draw_label ( screen, "", &bar.label, 255, 255, 0 );
}

void guild_enlist_user ( char *username )
{
	char rtn_str[200];

	sprintf ( rtn_str,"29,%s",username );
	send_con_hub ( rtn_str );
}

void guild_remove_user ( char *username )
{
	char rtn_str[200];

	sprintf ( rtn_str,"30,%s",username );
	send_con_hub ( rtn_str );
}

void guild_promote_user ( char *username )
{
	char rtn_str[200];

	sprintf ( rtn_str,"31,%s",username );
	send_con_hub ( rtn_str );
}

void guild_demote_user ( char *username )
{
	char rtn_str[200];

	sprintf ( rtn_str,"32,%s",username );
	send_con_hub ( rtn_str );
}

void guild_user_accept ( char *guildname )
{
	char rtn_str[200];

	sprintf ( rtn_str,"33,%s",guildname );
	send_con_hub ( rtn_str );
}

void guild_list_ask()
{
	char rtn_str[200];

	sprintf ( rtn_str,"34," );
	send_con_hub ( rtn_str );
}

void guild_reward_list ( char *guildname )
{
	char rtn_str[200];

	sprintf ( rtn_str,"11,%s",guildname );
	send_con_hub ( rtn_str );
}

void short_stat_user ( char *username )
{
	char rtn_str[200];

	sprintf ( rtn_str,"10,%s",username );
	send_con_hub ( rtn_str );
}

void guild_online_list_ask()
{
	char rtn_str[200];

	sprintf ( rtn_str,"8," );
	send_con_hub ( rtn_str );
}

void guild_chat_send ( char *message )
{
	char rtn_str[200];

	sprintf ( rtn_str,"35,%s",message );
	send_con_hub ( rtn_str );
}

void uni_chat_send ( char *message )
{
	char rtn_str[200];

	sprintf ( rtn_str,"54,%s",message );
	send_con_hub ( rtn_str );
}

int check_for_command ( char *message )
{
	int len, i;
	const int min_command_len = 22;
	char the_command[1025];

	len = strlen ( message );

	//must be atleast this long
	if ( len < 4 ) return 0;

	//first char has to this
	if ( message[0] != ':' ) return 0;

	for ( i=1;i<len && i<min_command_len;i++ )
	{
		the_command[i-1] = 0;

		if ( message[i] == ':' && message[i+1] == ':' ) break;

		the_command[i-1] = message[i];
	}

	if ( ! ( message[i] == ':' && message[i+1] == ':' ) ) return 0;

	//okay we got a command folks!
	process_command ( the_command, message + ( i+2 ) );

	return 1;
}

int help_command ( char *message )
{
	char temp_str[3][1025];
	int found = 0;

	if ( !strcmp ( "g",message ) )
	{
		sprintf ( temp_str[1],":g::message" );
		sprintf ( temp_str[2],"Relay a message to your entire guild." );

		found = 1;
	}
	else if ( !strcmp ( "c",message ) )
	{
		sprintf ( temp_str[1],":c::message" );
		sprintf ( temp_str[2],"Same as typing into the private chat." );

		found = 1;
	}
	else if ( !strcmp ( "genlist",message ) )
	{
		sprintf ( temp_str[1],":genlist::playername" );
		sprintf ( temp_str[2],"Invinte playername to join your guild." );

		found = 1;
	}
	else if ( !strcmp ( "gremove",message ) )
	{
		sprintf ( temp_str[1],":gremove::playername" );
		sprintf ( temp_str[2],"Boot playername from your guild." );

		found = 1;
	}
	else if ( !strcmp ( "gpromote",message ) )
	{
		sprintf ( temp_str[1],":gpromote::playername" );
		sprintf ( temp_str[2],"Promote playername's rank your guild." );

		found = 1;
	}
	else if ( !strcmp ( "gdemote",message ) )
	{
		sprintf ( temp_str[1],":gdemote::playername" );
		sprintf ( temp_str[2],"Lower playername's rank your guild." );

		found = 1;
	}
	else if ( !strcmp ( "gaccept",message ) )
	{
		sprintf ( temp_str[1],":gaccept::guildname" );
		sprintf ( temp_str[2],"Accept an invitation to join a guild." );

		found = 1;
	}
	else if ( !strcmp ( "glist",message ) )
	{
		sprintf ( temp_str[1],":glist::" );
		sprintf ( temp_str[2],"List all the players in your guild." );

		found = 1;
	}
	else if ( !strcmp ( "ignore",message ) )
	{
		sprintf ( temp_str[1],":ignore::playername" );
		sprintf ( temp_str[2],"Do not display messages from this player." );

		found = 1;
	}
	else if ( !strcmp ( "unignore",message ) )
	{
		sprintf ( temp_str[1],":unignore::playername" );
		sprintf ( temp_str[2],"Allow messages to be displayed from this previously ignored player." );

		found = 1;
	}
	else if ( !strcmp ( "ilist",message ) )
	{
		sprintf ( temp_str[1],":ilist::" );
		sprintf ( temp_str[2],"List all of the players currently being ignored." );

		found = 1;
	}
	else if ( !strcmp ( "sos",message ) )
	{
		sprintf ( temp_str[1],":sos::playername or :sos::" );
		sprintf ( temp_str[2],"Put a red glow around this player, or list all players on the sos list." );

		found = 1;
	}
	else if ( !strcmp ( "unsos",message ) )
	{
		sprintf ( temp_str[1],":sos::playername" );
		sprintf ( temp_str[2],"Remove playername from the sos list." );

		found = 1;
	}
	else if ( !strcmp ( "transfer",message ) )
	{
		sprintf ( temp_str[1],":transfer::playername,amount" );
		sprintf ( temp_str[2],"Transfer DC to playername." );

		found = 1;
	}
	else if ( !strcmp ( "cadd",message ) )
	{
		sprintf ( temp_str[1],":cadd::playername" );
		sprintf ( temp_str[2],"Invite or accept playername into your private chat list." );

		found = 1;
	}
	else if ( !strcmp ( "cremove",message ) )
	{
		sprintf ( temp_str[1],":cremove::playername" );
		sprintf ( temp_str[2],"Remove playername from your private chat list." );

		found = 1;
	}
	else if ( !strcmp ( "clist",message ) )
	{
		sprintf ( temp_str[1],":clist::" );
		sprintf ( temp_str[2],"Information about your private chat list." );

		found = 1;
	}
	else if ( !strcmp ( "conline",message ) )
	{
		sprintf ( temp_str[1],":conline::" );
		sprintf ( temp_str[2],"A listing of all your online private chat links." );

		found = 1;
	}
	else if ( !strcmp ( "soundon",message ) )
	{
		sprintf ( temp_str[1],":soundon::" );
		sprintf ( temp_str[2],"Turn sound on if previously turned off." );

		found = 1;
	}
	else if ( !strcmp ( "soundoff",message ) )
	{
		sprintf ( temp_str[1],":soundoff::" );
		sprintf ( temp_str[2],"Turn all game sounds off." );

		found = 1;
	}
	else if ( !strcmp ( "engineon",message ) )
	{
		sprintf ( temp_str[1],":engineon::" );
		sprintf ( temp_str[2],"Turn engine sound on if previously turned off." );

		found = 1;
	}
	else if ( !strcmp ( "engineoff",message ) )
	{
		sprintf ( temp_str[1],":engineoff::" );
		sprintf ( temp_str[2],"Turn inspace engine sounds off." );

		found = 1;
	}
	else if ( !strcmp ( "changepassword",message ) )
	{
		sprintf ( temp_str[1],":changepassword::old_password,new_password" );
		sprintf ( temp_str[2],"Change your password with a simple security feature." );

		found = 1;
	}
	else if ( !strcmp ( "gonline",message ) )
	{
		sprintf ( temp_str[1],":gonline::" );
		sprintf ( temp_str[2],"List all the guild memebers that are online." );

		found = 1;
	}
	else if ( !strcmp ( "grewardlist",message ) )
	{
		sprintf ( temp_str[1],":grewardlist:: or :grewardlist::guildname" );
		sprintf ( temp_str[2],"Give the enemy guilds set for your guild, or specified guildname." );

		found = 1;
	}
	else if ( !strcmp ( "stat",message ) )
	{
		sprintf ( temp_str[1],":stat:: or :stat::playername" );
		sprintf ( temp_str[2],"Give the Exp and DC of either yourself, or specified playername." );

		found = 1;
	}
	else if ( !strcmp ( "gawarddeco",message ) )
	{
		sprintf ( temp_str[1],":gawarddeco::playername,decoration name" );
		sprintf ( temp_str[2],"Award a guild decoration to specified playername." );

		found = 1;
	}
	else if ( !strcmp ( "gremovedeco",message ) )
	{
		sprintf ( temp_str[1],":gremovedeco::playername,decoration name" );
		sprintf ( temp_str[2],"Remove a guild decoration from playername." );

		found = 1;
	}
	else if ( !strcmp ( "glistdeco",message ) )
	{
		sprintf ( temp_str[1],":glistdeco:: or :glistdeco::playername" );
		sprintf ( temp_str[2],"List all the guild decorations that either you or playername has." );

		found = 1;
	}
	else if ( !strcmp ( "glistdecog",message ) )
	{
		sprintf ( temp_str[1],":glistdecog:: or :glistdecog::guildname" );
		sprintf ( temp_str[2],"List all the guild decorations that are available." );

		found = 1;
	}
	else if ( !strcmp ( "glistbugs",message ) )
	{
		sprintf ( temp_str[1],":glistbugs::" );
		sprintf ( temp_str[2],"List all the planets the guild currently has bugs on." );

		found = 1;
	}
	else if ( !strcmp ( "goodprices",message ) )
	{
		sprintf ( temp_str[1],":goodprices::planetname" );
		sprintf ( temp_str[2],"List all base good prices of a planet." );

		found = 1;
	}
	else if ( !strcmp ( "pricechange",message ) )
	{
		sprintf ( temp_str[1],":pricechange::" );
		sprintf ( temp_str[2],"Return the amount of time left until the next price change." );

		found = 1;
	}
	else if ( !strcmp ( "u",message ) )
	{
		sprintf ( temp_str[1],":u::" );
		sprintf ( temp_str[2],"Relay an universal message." );

		found = 1;
	}
	else if ( !strcmp ( "utoggle",message ) )
	{
		sprintf ( temp_str[1],":utoggle::" );
		sprintf ( temp_str[2],"Turn universal message receiving on/off" );

		found = 1;
	}
	else if ( !strcmp ( "buyactivation",message ) )
	{
		sprintf ( temp_str[1],":buyactivation:: or :buyactivation::playername" );
		sprintf ( temp_str[2],"Spend 5 of your DS to activate yourself or playername" );

		found = 1;
	}
	else if ( !strcmp ( "buyexp",message ) )
	{
		sprintf ( temp_str[1],":buyexp::amount or :buyexp::amount,playername" );
		sprintf ( temp_str[2],"Spend amount of your ds to give exp to yourself or playername, 1 ds = 50 exp" );

		found = 1;
	}
	else if ( !strcmp ( "buydc",message ) )
	{
		sprintf ( temp_str[1],":buydc::amount or :buydc::amount,playername" );
		sprintf ( temp_str[2],"Spend amount of your DS to give DC to yourself or playername, 1 DS = 10,000,000 DC" );

		found = 1;
	}
	else if ( !strcmp ( "dsamount",message ) )
	{
		sprintf ( temp_str[1],":dsamount::" );
		sprintf ( temp_str[2],"Display how much DS your character has." );

		found = 1;
	}
	else if ( !strcmp ( "transferds",message ) )
	{
		sprintf ( temp_str[1],":transferds::playername,amount" );
		sprintf ( temp_str[2],"Transfer DS to playername." );

		found = 1;
	}
	else if ( !strcmp ( "leaselogin",message ) )
	{
		sprintf ( temp_str[1],":leaselogin::secondary_login,secondary_password" );
		sprintf ( temp_str[2],"Create a secondary login with limited capabilities." );

		found = 1;
	}

	if ( found )
	{
		sprintf ( temp_str[0],"" );
		add_chat ( 0,temp_str[0] );

		sprintf ( temp_str[0],"Command Usage:: %s", temp_str[1] );
		add_chat ( 0,temp_str[0] );

		sprintf ( temp_str[0],"" );
		add_chat ( 0,temp_str[0] );

		sprintf ( temp_str[0],"Command Purpose:: %s", temp_str[2] );
		add_chat ( 0,temp_str[0] );

		return 1;
	}
	else
	{
		return 0;
	}
}

void process_command ( char *command, char *message )
{
	char lcase_cmd[1025];
	char temp_str[2][1025];
	int point = 0;

	//send to the lower
	lcase ( lcase_cmd, command );

	if ( !strcmp ( "help",lcase_cmd ) || !strcmp ( "command",lcase_cmd ) || !strcmp ( "commands",lcase_cmd ) || !strcmp ( "commandlist",lcase_cmd ) )
	{
		if ( !help_command ( message ) )
		{
			sprintf ( temp_str[0],"" );
			add_chat ( 0,temp_str[0] );

			sprintf ( temp_str[0],"Help Usage:: :help::commandname" );
			add_chat ( 0,temp_str[0] );

			sprintf ( temp_str[0],"" );
			add_chat ( 0,temp_str[0] );

			sprintf ( temp_str[0],"Command List:: g, c, u, ignore, unignore, ilist, leaselogin, sos, unsos, transfer, genlist, gremove, gpromote, gdemote, gaccept, glist, gonline, grewardlist, glistbugs, gawarddeco, gremovedeco, glistdeco, glistdecog, cadd, cremove, clist, conline, utoggle, pricechange, goodprices, soundon, soundoff, engineon, engineoff, changepassword, stat, dsamount, transferds, buyactivation, buyexp, buydc" );
			add_chat ( 0,temp_str[0] );
		}
	}
	else if ( !strcmp ( "g",lcase_cmd ) )
	{
		guild_chat_send ( message );
	}
	else if ( !strcmp ( "c",lcase_cmd ) )
	{
		private_chat_send ( message );
	}
	else if ( !strcmp ( "u",lcase_cmd ) )
	{
		uni_chat_send ( message );
	}
	else if ( !strcmp ( "genlist",lcase_cmd ) )
	{
		guild_enlist_user ( message );
	}
	else if ( !strcmp ( "gremove",lcase_cmd ) )
	{
		guild_remove_user ( message );
	}
	else if ( !strcmp ( "gpromote",lcase_cmd ) )
	{
		guild_promote_user ( message );
	}
	else if ( !strcmp ( "gdemote",lcase_cmd ) )
	{
		guild_demote_user ( message );
	}
	else if ( !strcmp ( "gaccept",lcase_cmd ) )
	{
		guild_user_accept ( message );
	}
	else if ( !strcmp ( "glist",lcase_cmd ) )
	{
		guild_list_ask();
	}
	else if ( !strcmp ( "gonline",lcase_cmd ) )
	{
		guild_online_list_ask();
	}
	else if ( !strcmp ( "grewardlist",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		guild_reward_list ( temp_str[0] );
	}
	else if ( !strcmp ( "ignore",lcase_cmd ) )
	{
		add_ignore ( message );
	}
	else if ( !strcmp ( "unignore",lcase_cmd ) )
	{
		remove_ignore ( message );
	}
	else if ( !strcmp ( "ilist",lcase_cmd ) )
	{
		list_ignore();
	}
	else if ( !strcmp ( "sos",lcase_cmd ) )
	{
		if ( message[0] )
		{
			if ( add_sos ( message ) )
				update_space_user_sos ( message, 1 );
		}
		else
			list_sos();
	}
	else if ( !strcmp ( "unsos",lcase_cmd ) )
	{
		if ( remove_sos ( message ) )
			update_space_user_sos ( message, 0 );
	}
	else if ( !strcmp ( "transfer",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		transfer_money_to ( temp_str[0],atoi ( temp_str[1] ) );
	}
	else if ( !strcmp ( "cadd",lcase_cmd ) )
	{
		private_chat_add ( message );
	}
	else if ( !strcmp ( "cremove",lcase_cmd ) )
	{
		private_chat_remove ( message );
	}
	else if ( !strcmp ( "clist",lcase_cmd ) )
	{
		private_chat_list();
	}
	else if ( !strcmp ( "conline",lcase_cmd ) )
	{
		private_chat_online_list();
	}
	else if ( !strcmp ( "soundon",lcase_cmd ) )
	{
		game.sound_on = 1;
		write_config_info();
	}
	else if ( !strcmp ( "soundoff",lcase_cmd ) )
	{
		game.sound_on = 0;
		write_config_info();
	}
	else if ( !strcmp ( "engineon",lcase_cmd ) )
	{
		game.engine_on = 1;
		write_config_info();
	}
	else if ( !strcmp ( "engineoff",lcase_cmd ) )
	{
		game.engine_on = 0;
		write_config_info();
	}
	else if ( !strcmp ( "changepassword",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		change_password ( temp_str[0],temp_str[1] );
	}
	else if ( !strcmp ( "stat",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		short_stat_user ( temp_str[0] );
	}
	else if ( !strcmp ( "gawarddeco",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		guild_award_deco ( temp_str[0],temp_str[1] );
	}
	else if ( !strcmp ( "gremovedeco",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		guild_remove_deco ( temp_str[0],temp_str[1] );
	}
	else if ( !strcmp ( "glistdeco",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		guild_list_deco ( temp_str[0] );
	}
	else if ( !strcmp ( "glistdecog",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		guild_list_decog ( temp_str[0] );
	} //glistbugs
	else if ( !strcmp ( "glistbugs",lcase_cmd ) )
	{
		guild_list_bugs();
	}
	else if ( !strcmp ( "goodprices",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		server_send_ask_goodprices ( temp_str[0] );
	}
	else if ( !strcmp ( "pricechange",lcase_cmd ) )
	{
		hub_send_ask_pricechange();
	}
	else if ( !strcmp ( "utoggle",lcase_cmd ) )
	{
		hub_send_utoggle();
	}
	else if ( !strcmp ( "dsamount",lcase_cmd ) )
	{
		hub_send_dsamount();
	}
	else if ( !strcmp ( "transferds",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		hub_send_transferds ( temp_str[0], atoi ( temp_str[1] ) );
	}
	else if ( !strcmp ( "buyactivation",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		hub_send_buyactivation ( temp_str[0] );
	}
	else if ( !strcmp ( "buyexp",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		hub_send_buyexp ( temp_str[1], atoi ( temp_str[0] ) );
	}
	else if ( !strcmp ( "buydc",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		hub_send_buydc ( temp_str[1], atoi ( temp_str[0] ) );
	}
	else if ( !strcmp ( "leaselogin",lcase_cmd ) )
	{
		split ( temp_str[0],message,',',&point );
		split ( temp_str[1],message,',',&point );
		hub_send_lease_login ( temp_str[0], temp_str[1] );
	}
}

SDL_Surface *special_fship ( int ship_num )
{
	switch ( ship_num )
	{
		case 0: //sat
			if ( !game.game_image.fship_sat )
				game.game_image.fship_sat = IMG_Load ( "graphics/sat.gif" );

			return game.game_image.fship_sat;

			break;
		case 1: //no sat
			if ( !game.game_image.fship_sat_destroyed )
				game.game_image.fship_sat_destroyed = IMG_Load ( "graphics/sat_destroyed.gif" );

			return game.game_image.fship_sat_destroyed;

			break;
	}

	return NULL;
}

void sat_editor_redraw_entire_screen()
{
	//ya
	sat_set_default_variables();

	//purchase / upgrade button
	sat_draw_purchase_or_upgrade_button();

	//text box
	sat_reset_textbox();

	//money label
	sat_draw_money_label();

	//hull
	sat_draw_hull();

	//viewing button
	sat_draw_viewing_guns_or_missiles();

	//weapon icons
	sat_draw_weapon_icons();

	//weapon selected
	sat_draw_weapon_selected();

	//info labels
	sat_draw_info_labels();
}

void sat_draw_info_labels()
{
	char temp_str[200];

	if ( sat_is_ours() )
	{
		sprintf ( temp_str, "%d", planet_sat.sat.level );
		draw_right_label ( screen, temp_str, &sat_editor.current_level_label, 107, 216, 210 );
		sprintf ( temp_str, "%d", planet_sat.sat.level * SAT_TAX_INC );
		draw_right_label ( screen, temp_str, &sat_editor.annual_tax_label, 107, 216, 210 );
		sprintf ( temp_str, "%d", planet_sat.sat.money_collected );
		draw_right_label ( screen, temp_str, &sat_editor.taxes_collected_label, 107, 216, 210 );
	}
	else
	{
		draw_right_label ( screen, "", &sat_editor.current_level_label, 107, 216, 210 );
		draw_right_label ( screen, "", &sat_editor.annual_tax_label, 107, 216, 210 );
		draw_right_label ( screen, "", &sat_editor.taxes_collected_label, 107, 216, 210 );
	}
}

void sat_draw_sell_buy_img()
{
	int i;
	int wep_max;
	SDL_Rect *destination;

	destination = &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 4];
	i = sat_editor.weapon_selected;

	if ( sat_editor.viewing_guns ) wep_max = planet_sat.sat.level * SAT_GUN_INC;
	if ( sat_editor.viewing_missiles ) wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

	//black out
	if ( i >= wep_max || i < 0 )
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	else if ( sat_editor.viewing_guns && planet_sat.sat.gun[i] > -1 )
		SDL_BlitSurface ( sat_editor.wep_sell_but, NULL, screen, destination );
	else if ( sat_editor.viewing_missiles && planet_sat.sat.missile[i] > -1 )
		SDL_BlitSurface ( sat_editor.wep_sell_but, NULL, screen, destination );
	else
		SDL_BlitSurface ( sat_editor.wep_buy_but, NULL, screen, destination );
}

void sat_draw_repair_reload_img()
{
	int i;
	int wep_max;
	SDL_Rect *destination;

	destination = &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 5];
	i = sat_editor.weapon_selected;

	if ( sat_editor.viewing_guns ) wep_max = planet_sat.sat.level * SAT_GUN_INC;
	if ( sat_editor.viewing_missiles ) wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

	//black out
	if ( i >= wep_max || i < 0 )
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	else if ( sat_editor.viewing_guns && planet_sat.sat.gun_destroyed[i] && planet_sat.sat.gun[i] > -1 )
		SDL_BlitSurface ( sat_editor.wep_repair_but, NULL, screen, destination );
	else if ( sat_editor.viewing_missiles && !planet_sat.sat.missile_left[i] && planet_sat.sat.missile[i] > -1 )
		SDL_BlitSurface ( sat_editor.wep_reload_but, NULL, screen, destination );
	else
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void sat_toggle_weapons_viewing()
{
	if ( sat_editor.viewing_guns )
	{
		sat_editor.viewing_guns = 0;
		sat_editor.viewing_missiles = 1;
	}
	else
	{
		sat_editor.viewing_guns = 1;
		sat_editor.viewing_missiles = 0;
	}

	//viewing button
	sat_draw_viewing_guns_or_missiles();

	//weapon icons
	sat_draw_weapon_icons();

	//weapon selected
	sat_draw_weapon_selected();

	sdl_flip_mutex();
}

void sat_set_default_variables()
{
	//clear all first
	sat_editor.viewing_missiles = 0;
	sat_editor.upgrade_sat = 0;
	sat_editor.purchase_sat = 0;
	sat_editor.sel_right = 0;
	sat_editor.sel_left = 0;
	sat_editor.sell_wep = 0;
	sat_editor.buy_wep = 0;
	sat_editor.reload_wep = 0;
	sat_editor.repair_wep = 0;
	sat_set_weapon_variables();


	//defaults
	sat_editor.viewing_guns = 1;
	sat_editor.weapon_selected = 0;
}

void sat_set_weapon_variables()
{
	int i, s, p;

	s = game.server_id;
	p = planet.last_planet;

	//raw defaults
	sat_editor.min_gun = sat_editor.max_gun = -1;
	sat_editor.min_missile = sat_editor.max_missile = -1;

	//what is min gun?
	for ( i=0;i<GUN_MAX;i++ )
		if ( server[s].planet[p].gun[i] ) break;

	if ( i!=GUN_MAX )
		sat_editor.min_gun = i;

	//max gun?
	for ( i=GUN_MAX-1;i>=0;i-- )
		if ( server[s].planet[p].gun[i] ) break;

	if ( i!=-1 )
		sat_editor.max_gun = i;

	//min missile?
	for ( i=0;i<MISSILE_MAX;i++ )
		if ( server[s].planet[p].missile[i] ) break;

	if ( i!=MISSILE_MAX )
		sat_editor.min_missile = i;

	//max missile?
	for ( i=MISSILE_MAX-1;i>=0;i-- )
		if ( server[s].planet[p].missile[i] ) break;

	if ( i!=-1 )
		sat_editor.max_missile = i;

	//now set these
	sat_editor.gun_selected = sat_editor.min_gun;
	sat_editor.missile_selected = sat_editor.min_missile;
}

void sat_reset_textbox()
{
	clear_text_box ( screen, &sat_editor.text_box[0] );
	select_text_box ( screen, &sat_editor.text_box[0] );
}

void sat_draw_viewing_guns_or_missiles()
{
	if ( sat_editor.viewing_guns )
		SDL_BlitSurface ( sat_editor.viewing_guns_img, NULL, screen, &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 1] );

	if ( sat_editor.viewing_missiles )
		SDL_BlitSurface ( sat_editor.viewing_missiles_img, NULL, screen, &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 1] );
}

void sat_draw_money_label()
{
	char temp_str[200];

	sprintf ( temp_str,"%d",planet_sat.sat.money );
	draw_right_label ( screen, temp_str, &sat_editor.money_label, 107, 216, 210 );
}

void sat_draw_hull()
{
	SDL_Rect ohull_box, ihull_box;

	ohull_box.x = 362;
	ohull_box.y = 220;
	ohull_box.w = 581 - ohull_box.x;
	ohull_box.h = 16;
	ihull_box.x = ohull_box.x + 2;
	ihull_box.y = ohull_box.y + 2;
	ihull_box.w = ohull_box.w - 4;
	ihull_box.h = 12;

	if ( sat_is_ours() )
	{
		double hull_percent;

		hull_percent = 1.0 * planet_sat.sat.hull / ( planet_sat.sat.level * SAT_HULL_INC );

		ihull_box.w = ( short unsigned int ) ( ihull_box.w * hull_percent );

		SDL_FillRect ( screen, &ohull_box, SDL_MapRGB ( screen->format, 167, 169, 23 ) );
		SDL_FillRect ( screen, &ihull_box, SDL_MapRGB ( screen->format, 246, 246, 62 ) );
	}
	else //black out
	{
		SDL_FillRect ( screen, &ohull_box, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	}
}

void sat_draw_weapon_icons()
{
	SDL_Rect destination;
	int i, wep_max;

	//30x20

	destination.x = 398;
	destination.y = 281;
	destination.w = 31 * 6;
	destination.h = 21 * 6;

	//first clear out all that could have been
	destination.x--;
	destination.y--;
	destination.w++;
	destination.h++;
	SDL_FillRect ( screen, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	destination.x++;
	destination.y++;
	destination.w--;
	destination.h--;

	if ( sat_editor.viewing_guns )
	{
		wep_max = planet_sat.sat.level * SAT_GUN_INC;

		for ( i=0;i<wep_max;i++ )
		{
			if ( destination.x > 553 )
			{
				destination.x = 398;
				destination.y += 21;
			}

			if ( planet_sat.sat.gun[i] > -1 )
			{
				int s, w;

				s = game.server_id;
				w = planet_sat.sat.gun[i];

				if ( load_gun_pod_img_specific ( s,w ) )
				{
					SDL_BlitSurface ( server[s].gun[w].gun_pod_img, NULL, screen, &destination );
				}
				else //if not loaded
				{
					SDL_BlitSurface ( game.nif_image.no_gun_pod, NULL, screen, &destination );
				}

				//damaged or need of reloading?
				if ( planet_sat.sat.gun_destroyed[i] )
					rectangleRGBA ( screen, destination.x - 1, destination.y - 1, destination.x + 30, destination.y + 20, 255, 0 , 0, 255 );
			}
			else
			{
				SDL_BlitSurface ( sat_editor.empty_pod, NULL, screen, &destination );
			}

			//now make the addition!
			destination.x += 31;
		}
	}

	if ( sat_editor.viewing_missiles )
	{
		wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

		for ( i=0;i<wep_max;i++ )
		{
			if ( destination.x > 553 )
			{
				destination.x = 398;
				destination.y += 21;
			}

			if ( planet_sat.sat.missile[i] > -1 )
			{
				int s, w;

				s = game.server_id;
				w = planet_sat.sat.missile[i];

				if ( load_missile_pod_img_specific ( s,w ) )
				{
					SDL_BlitSurface ( server[s].missile[w].missile_pod_img, NULL, screen, &destination );
				}
				else //if not loaded
				{
					SDL_BlitSurface ( game.nif_image.no_missile_pod, NULL, screen, &destination );
				}

				//damaged or need of reloading?
				if ( !planet_sat.sat.missile_left[i] )
					rectangleRGBA ( screen, destination.x - 1, destination.y - 1, destination.x + 30, destination.y + 20, 255, 0 , 0, 255 );
			}
			else
			{
				SDL_BlitSurface ( sat_editor.empty_pod, NULL, screen, &destination );
			}

			//now make the addition!
			destination.x += 31;
		}
	}
}

void sat_draw_purchase_or_upgrade_button()
{
	SDL_Rect *destination;

	destination = &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) ];

	if ( !sat_is_owned() )
	{
		SDL_BlitSurface ( sat_editor.sat_purchase_but, NULL, screen, destination );

		destination = &sat_editor.button[7 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) ];
		//draw bug too
		SDL_BlitSurface ( sat_editor.sat_bug_but, NULL, screen, destination );
	}
	else if ( sat_is_ours() && planet_sat.sat.level < SAT_LEVEL_MAX )
	{
		SDL_BlitSurface ( sat_editor.sat_upgrade_but, NULL, screen, destination );

		//monkey
		destination = &sat_editor.button[7 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) ];

		//clear out the spot
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

		//asshole
		sat_draw_weapon_stats();
	}
	else
	{
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
	}
}

void sat_make_weapon_select ( int i )
{
	int wep_max;

	if ( sat_editor.viewing_guns ) wep_max = planet_sat.sat.level * SAT_GUN_INC;
	if ( sat_editor.viewing_missiles ) wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

	//maximum!
	if ( i >= wep_max ) return;

	//undraw current
	sat_undraw_weapon_selected();

	//set new
	sat_editor.weapon_selected = i;

	//draw new
	sat_draw_weapon_selected();

	sdl_flip_mutex();
}

void sat_undraw_weapon_selected()
{
	int i;
	int wep_max;

	if ( sat_editor.viewing_guns ) wep_max = planet_sat.sat.level * SAT_GUN_INC;
	if ( sat_editor.viewing_missiles ) wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

	i = sat_editor.weapon_selected;

	if ( i >= wep_max ) return;

	//if(i > -1)
	for ( i=0;i<wep_max;i++ ) //black out
	{
		int x, y;

		x = 398 + ( ( i%6 ) * 31 );
		y = 281 + ( ( i/6 ) * 21 );

		if ( sat_editor.viewing_missiles && !planet_sat.sat.missile_left[i] && planet_sat.sat.missile[i] > -1 )
			continue;
		if ( sat_editor.viewing_guns && planet_sat.sat.gun_destroyed[i] && planet_sat.sat.gun[i] > -1 )
			continue;

		rectangleRGBA ( screen, x - 1, y - 1, x + 30, y + 20, 0, 0 , 0, 255 );
	}

	for ( i=0;i<wep_max;i++ ) //red in
	{
		int x, y;

		x = 398 + ( ( i%6 ) * 31 );
		y = 281 + ( ( i/6 ) * 21 );

		if ( sat_editor.viewing_missiles && !planet_sat.sat.missile_left[i] && planet_sat.sat.missile[i] > -1 )
			rectangleRGBA ( screen, x - 1, y - 1, x + 30, y + 20, 255, 0 , 0, 255 );
		else if ( sat_editor.viewing_guns && planet_sat.sat.gun_destroyed[i] && planet_sat.sat.gun[i] > -1 )
			rectangleRGBA ( screen, x - 1, y - 1, x + 30, y + 20, 255, 0 , 0, 255 );
	}
}

void sat_draw_weapon_selected()
{
	int i;
	int wep_max;

	//draw the left hand side stuff first
	sat_draw_sell_buy_img();
	sat_draw_repair_reload_img();

	//slap in the info stats
	sat_draw_weapon_stats();

	//buttons?
	sat_draw_left_right_buttons();

	if ( sat_editor.viewing_guns ) wep_max = planet_sat.sat.level * SAT_GUN_INC;
	if ( sat_editor.viewing_missiles ) wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

	i = sat_editor.weapon_selected;

	if ( i > -1 && i < wep_max )
	{
		int x, y;

		x = 398 + ( ( i%6 ) * 31 );
		y = 281 + ( ( i/6 ) * 21 );

		rectangleRGBA ( screen, x - 1, y - 1, x + 30, y + 20, 255, 255 , 255, 255 );
	}
}

void sat_draw_weapon_stats()
{
	int i, w;
	int wep_max;
	char *name_str, accuracy_str[21] = "", speed_str[21] = "", damage_str[21] = "";

	if ( sat_editor.viewing_guns ) wep_max = planet_sat.sat.level * SAT_GUN_INC;
	if ( sat_editor.viewing_missiles ) wep_max = planet_sat.sat.level * SAT_MISSILE_INC;

	i = sat_editor.weapon_selected;

	name_str = accuracy_str; // null out for now

	//checks
	if ( i < 0 || i >= wep_max ) //show nothing stats
	{

	}
	else if ( ( sat_editor.viewing_guns && planet_sat.sat.gun[i] < 0 ) || ( sat_editor.viewing_missiles && planet_sat.sat.missile[i] < 0 ) ) // show nothing stats
	{
		if ( sat_editor.viewing_guns && sat_editor.gun_selected > -1 )
		{
			w = sat_editor.gun_selected;

			name_str = server[game.server_id].gun[w].name;
			sprintf ( accuracy_str,"%d",server[game.server_id].gun[w].accuracy );
			sprintf ( speed_str,"%d",server[game.server_id].gun[w].speed );
			sprintf ( damage_str,"%d",server[game.server_id].gun[w].damage );
		}

		if ( sat_editor.viewing_missiles && sat_editor.missile_selected > -1 )
		{
			w = sat_editor.missile_selected;

			name_str = server[game.server_id].missile[w].name;
			sprintf ( accuracy_str,"%d",server[game.server_id].missile[w].accuracy );
			sprintf ( speed_str,"%d",server[game.server_id].missile[w].speed );
			sprintf ( damage_str,"%d",server[game.server_id].missile[w].damage );
		}
	}
	else if ( sat_editor.viewing_guns )
	{
		//set it
		w = planet_sat.sat.gun[i];

		//checks...
		if ( w >= 0 && w < GUN_MAX )
		{
			//keep everything cleared
			name_str = server[game.server_id].gun[w].name;
			sprintf ( accuracy_str,"%d",server[game.server_id].gun[w].accuracy );
			sprintf ( speed_str,"%d",server[game.server_id].gun[w].speed );
			sprintf ( damage_str,"%d",server[game.server_id].gun[w].damage );
		}
	}
	else if ( sat_editor.viewing_missiles )
	{
		//set it
		w = planet_sat.sat.missile[i];

		//checks...
		if ( w >= 0 && w < MISSILE_MAX )
		{
			//keep everything cleared
			name_str = server[game.server_id].missile[w].name;
			sprintf ( accuracy_str,"%d",server[game.server_id].missile[w].accuracy );
			sprintf ( speed_str,"%d",server[game.server_id].missile[w].speed );
			sprintf ( damage_str,"%d",server[game.server_id].missile[w].damage );
		}
	}

	//name
	draw_center_label ( screen, name_str, &sat_editor.wep_name_label, 107, 216, 210 );

	//accuracy
	draw_right_label ( screen,accuracy_str,&sat_editor.wep_accuracy_label, 107, 216, 210 );

	//speed
	draw_right_label ( screen,speed_str,&sat_editor.wep_speed_label, 107, 216, 210 );

	//damage
	draw_right_label ( screen,damage_str,&sat_editor.wep_damage_label, 107, 216, 210 );

}

int sat_is_ours()
{
	if ( planet_sat.sat.level >= 0 ) return 1;

	return 0;
}

int sat_is_owned()
{
	if ( planet_sat.sat.level ) return 1;

	return 0;
}

void sat_choose_left()
{
	int i;
	int s, p;

	s = game.server_id;
	p = planet.last_planet;

	if ( !sat_show_left_but() ) return;

	if ( sat_editor.viewing_guns )
	{
		for ( i=sat_editor.gun_selected - 1;i>=sat_editor.min_gun;i-- )
			if ( server[s].planet[p].gun[i] )
				break;

		sat_editor.gun_selected = i;

		sat_draw_weapon_stats();
		sat_draw_left_right_buttons();
		sdl_flip_mutex();
	}

	if ( sat_editor.viewing_missiles )
	{
		for ( i=sat_editor.missile_selected - 1;i>=sat_editor.min_missile;i-- )
			if ( server[s].planet[p].missile[i] )
				break;

		sat_editor.missile_selected = i;

		sat_draw_weapon_stats();
		sat_draw_left_right_buttons();
		sdl_flip_mutex();
	}
}

void sat_choose_right()
{
	int i;
	int s, p;

	s = game.server_id;
	p = planet.last_planet;

	if ( !sat_show_right_but() ) return;

	if ( sat_editor.viewing_guns )
	{
		for ( i=sat_editor.gun_selected + 1;i<sat_editor.max_gun;i++ )
			if ( server[s].planet[p].gun[i] )
				break;

		sat_editor.gun_selected = i;

		sat_draw_weapon_stats();
		sat_draw_left_right_buttons();
		sdl_flip_mutex();
	}

	if ( sat_editor.viewing_missiles )
	{
		for ( i=sat_editor.missile_selected + 1;i<sat_editor.max_missile;i++ )
			if ( server[s].planet[p].missile[i] )
				break;

		sat_editor.missile_selected = i;

		sat_draw_weapon_stats();
		sat_draw_left_right_buttons();
		sdl_flip_mutex();
	}

}

void sat_send_bug_sat()
{
	char send_str[50] = "51,";
	SDL_Rect *destination;

	send_con_hub ( send_str );

	//monkey
	destination = &sat_editor.button[7 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) ];

	//clear out the spot
	SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

	//flip
	sdl_flip_mutex();
}

void sat_send_upgrade_purchase()
{
	char send_str[50] = "37,";

	send_con_hub ( send_str );
}

void sat_send_deposit ( int amount )
{
	char send_str[50];

	sprintf ( send_str,"38,%d",amount );

	send_con_hub ( send_str );
}

void sat_send_buy_sell()
{
	char send_str[50] = "39,";

	if ( sat_editor.viewing_guns )
		sprintf ( send_str,"39,0,%d,%d", sat_editor.weapon_selected, sat_editor.gun_selected );

	if ( sat_editor.viewing_missiles )
		sprintf ( send_str,"39,1,%d,%d", sat_editor.weapon_selected, sat_editor.missile_selected );

	send_con_hub ( send_str );
}

void sat_send_reload_repair()
{
	char send_str[50] = "40,";

	if ( sat_editor.viewing_guns )
		sprintf ( send_str,"40,0,%d",sat_editor.weapon_selected );

	if ( sat_editor.viewing_missiles )
		sprintf ( send_str,"40,1,%d",sat_editor.weapon_selected );

	send_con_hub ( send_str );
}

int sat_show_right_but()
{
	int w, i;

	if ( !sat_is_owned() ) return 0;

	i = sat_editor.weapon_selected;

	//banging
	if ( sat_editor.viewing_guns )
	{
		//set it
		w = planet_sat.sat.gun[i];

		//checks...
		if ( w > -1 ) return 0;
		if ( sat_editor.gun_selected == -1 ) return 0;
		if ( sat_editor.gun_selected >= sat_editor.max_gun ) return 0;

		//achcha hai
		return 1;
	}

	if ( sat_editor.viewing_missiles )
	{
		//set it
		w = planet_sat.sat.missile[i];

		//checks...
		if ( w > -1 ) return 0;
		if ( sat_editor.missile_selected == -1 ) return 0;
		if ( sat_editor.missile_selected >= sat_editor.max_missile ) return 0;

		//achcha hai
		return 1;
	}

	return 0;
}

int sat_show_left_but()
{
	int w, i;

	if ( !sat_is_owned() ) return 0;

	i = sat_editor.weapon_selected;

	//banging
	if ( sat_editor.viewing_guns )
	{
		//set it
		w = planet_sat.sat.gun[i];

		//checks...
		if ( w > -1 ) return 0;
		if ( sat_editor.gun_selected == -1 ) return 0;
		if ( sat_editor.gun_selected <= sat_editor.min_gun ) return 0;

		//achcha hai
		return 1;
	}

	if ( sat_editor.viewing_missiles )
	{
		//set it
		w = planet_sat.sat.missile[i];

		//checks...
		if ( w > -1 ) return 0;
		if ( sat_editor.missile_selected == -1 ) return 0;
		if ( sat_editor.missile_selected <= sat_editor.min_missile ) return 0;

		//achcha hai
		return 1;
	}

	return 0;
}

void sat_draw_left_right_buttons()
{
	SDL_Rect *destination;

	//right
	destination = &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 3];
	if ( sat_show_right_but() )
		SDL_BlitSurface ( sat_editor.wep_right_but, NULL, screen, destination );
	else
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

	//left
	destination = &sat_editor.button[1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 2];
	if ( sat_show_left_but() )
		SDL_BlitSurface ( sat_editor.wep_left_but, NULL, screen, destination );
	else
		SDL_FillRect ( screen, destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );
}

void space_set_hit_actual_max()
{
	int i;

	for ( i=space.hit_actual_max=0;i<game.ship[game.ship_sel].gun_ammount;i++ )
	{
		//all the checks
		if ( game.ship[game.ship_sel].gun[i] < 0 ) continue;
		if ( game.ship[game.ship_sel].gun[i] >= GUN_MAX ) continue;
		if ( game.ship[game.ship_sel].gun_id[i] < 0 ) continue;
		if ( game.ship[game.ship_sel].gun_id[i] >= MAX_SERVER ) continue;
		if ( game.ship[game.ship_sel].gun_destroyed[i] ) continue;

		//good for addin
		space.hit_actual_max++;
	}
}

void send_hub_enter_planet_guild()
{
	char rtn_str[50];

	sprintf ( rtn_str,"41,%d",planet.last_planet );
	send_con_hub ( rtn_str );
}

int map_check_sector_on_map ( int x, int y )
{
	if ( x < 80 || y < 105 || x > 715 || y > 550 ) return 0;
	if ( x > 610 && y > 505 ) return 0; //to work around the cancel button

	return 1;
}

void map_draw_planet_names()
{
	int i;

	for ( i=0;i<SECTOR_MAX;i++ )
		if ( server[game.server_id].zone[game.zone].sector[i].x ) //if sector is there
			if ( server[game.server_id].zone[game.zone].sector[i].planet > -1 )
			{
				SDL_Rect location;
				int p;

				p = server[game.server_id].zone[game.zone].sector[i].planet;

				location.x = 400 + ( server[game.server_id].zone[game.zone].sector[i].x - map.sector_x );
				location.y = 300 + ( server[game.server_id].zone[game.zone].sector[i].y - map.sector_y );
				location.x += map.shift_x;
				location.y += map.shift_y;
				if ( game.sector == i )
					location.y += 15;
				else
					location.y += 10;
				location.w = 0;
				location.h = 0;
				draw_raw_center_label ( screen, server[game.server_id].planet[p].name, location, 104, 148, 107 );
			}
}

void map_draw_sector_images()
{
	double offset_x, offset_y, hypo, temp_dbl, sun_offset_x, sun_offset_y;
	SDL_Rect destination;
	int i, k;
	int s_id, z;
	int range_max;

	s_id = game.server_id;
	z = game.zone;
	range_max = game.ship[game.ship_sel].range;

	if ( !game.activated && range_max > DEACTIVATED_RANGE_LIMIT )
		range_max = DEACTIVATED_RANGE_LIMIT;

	//draw all sectors
	offset_x = server[s_id].zone[z].hyper_sec->w / 2;
	offset_y = server[s_id].zone[z].hyper_sec->h / 2;
	sun_offset_x = map.sun_img->w / 2;
	sun_offset_y = map.sun_img->h / 2;
	for ( i=0;i<SECTOR_MAX;i++ )
		if ( server[s_id].zone[z].sector[i].x ) //if sector is there
		{
			int x, y;

			x = 400 + ( server[s_id].zone[z].sector[i].x - map.sector_x );
			y = 300 + ( server[s_id].zone[z].sector[i].y - map.sector_y );
			x += map.shift_x;
			y += map.shift_y;

			//check if within boundaries
			if ( !map_check_sector_on_map ( x, y ) ) continue;

			if ( !server[s_id].zone[z].sector[i].is_sun && server[s_id].zone[z].sector[i].planet < 0 )
			{
				destination.x = ( short int ) ( x - offset_x );
				destination.y = ( short int ) ( y - offset_y );

				//check if in jump range
				if ( server[s_id].zone[z].sector[i].range > range_max )
					SDL_BlitSurface ( game.game_image.hyper_s_grey, NULL, screen, &destination );
				else
					SDL_BlitSurface ( server[s_id].zone[z].hyper_sec, NULL, screen, &destination );
			}
			else if ( server[s_id].zone[z].sector[i].planet >= 0 && i!=game.sector ) //has planet
			{
				if ( load_map_planet ( s_id, server[s_id].zone[z].sector[i].planet ) )
				{
					destination.x = x - ( server[s_id].planet[server[s_id].zone[z].sector[i].planet].hyper_img->w / 2 );
					destination.y = y - ( server[s_id].planet[server[s_id].zone[z].sector[i].planet].hyper_img->h / 2 );
					SDL_BlitSurface ( server[s_id].planet[server[s_id].zone[z].sector[i].planet].hyper_img, NULL, screen, &destination );
				}
				else
				{
					//use nif image
					if ( !game.nif_image.no_hyper_p_img )
						game.nif_image.no_hyper_p_img = IMG_Load ( "graphics/nif/nif_hyper_p.gif" );

					if ( game.nif_image.no_hyper_p_img )
					{
						destination.x = x - ( game.nif_image.no_hyper_p_img->w / 2 );
						destination.y = y - ( game.nif_image.no_hyper_p_img->h / 2 );
						SDL_BlitSurface ( game.nif_image.no_hyper_p_img, NULL, screen, &destination );
					}
				}
			}
			else
			{
				if ( i!=game.sector )
				{
					destination.x = ( short int ) ( x - sun_offset_x );
					destination.y = ( short int ) ( y - sun_offset_y );
					SDL_BlitSurface ( map.sun_img, NULL, screen, &destination );
				}
			}
			
			//either way, is there an enemy there?
			if ( server[s_id].zone[z].sector[i].has_player && (server[s_id].zone[z].free_scan || server[s_id].zone[z].sector[i].free_scan) && i!=game.sector )
			{
				destination.x = ( short int ) ( x - 15 );
				destination.y = ( short int ) ( y );
				SDL_BlitSurface ( map.e_mark, NULL, screen, &destination );
			}
			
			if(server[s_id].zone[z].sector[i].has_guild_member && i!=game.sector)
			{
				destination.x = ( short int ) ( x + 10 );
				destination.y = ( short int ) ( y - 2 );
				SDL_BlitSurface ( map.g_shield, NULL, screen, &destination );
			}
		}
}

void map_draw_local_sectors()
{
	double offset_x, offset_y, hypo, temp_dbl, sun_offset_x, sun_offset_y;
	SDL_Rect destination;
	int i, k;
	int s_id, z;
	int range_max;
	char temp_filename[200], temp_str[21];

	s_id = game.server_id;
	z = game.zone;
	
	range_max = game.ship[game.ship_sel].range;

	//draw all sectors
	offset_x = server[s_id].zone[z].hyper_sec->w / 2;
	offset_y = server[s_id].zone[z].hyper_sec->h / 2;
	sun_offset_x = map.sun_img->w / 2;
	sun_offset_y = map.sun_img->h / 2;

	for ( i=0;i<5;i++ )
	{
		int s;

		s = server[s_id].zone[z].sector[game.sector].sector[i];
		//check if in jump range
		if ( server[s_id].zone[z].sector[s].range > range_max ) continue;


		if ( map.sector_link[i] >= 0 )
		{
			//draw lines
			offset_x = 400 - map.sector_link_x[i];
			offset_y = 300 - map.sector_link_y[i];
			hypo = pow ( pow ( offset_x,2 ) + pow ( offset_y,2 ), 0.5 );

			if ( offset_y && offset_x )
			{
				temp_dbl = -0.9 * offset_x / hypo;
				offset_x = 0.9 * offset_y / hypo;
				offset_y = temp_dbl;
			}
			else if ( fabs ( offset_x ) < 0.0001 )
			{
				offset_x = 1;
				offset_y = 0;
			}
			else if ( fabs ( offset_y ) < 0.0001 )
			{
				offset_x = 0;
				offset_y = 1;
			}

			for ( k=3;k>0;k-- )
			{
				short int x1, y1, x2, y2;

				x1 = ( short int ) ( ( 400 + ( k * offset_x ) ) + map.shift_x );
				y1 = ( short int ) ( ( 300 + ( k * offset_y ) ) + map.shift_y );
				x2 = ( short int ) ( ( map.sector_link_x[i] + ( k * offset_x ) ) + map.shift_x );
				y2 = ( short int ) ( ( map.sector_link_y[i] + ( k * offset_y ) ) + map.shift_y );

				lineRGBA ( screen, x1, y1, x2, y2, 1, 230 - ( k * 50 ), 250 - ( k * 50 ), 255 );

				x1 = ( short int ) ( ( 400 - ( k * offset_x ) ) + map.shift_x );
				y1 = ( short int ) ( ( 300 - ( k * offset_y ) ) + map.shift_y );
				x2 = ( short int ) ( ( map.sector_link_x[i] - ( k * offset_x ) ) + map.shift_x );
				y2 = ( short int ) ( ( map.sector_link_y[i] - ( k * offset_y ) ) + map.shift_y );

				lineRGBA ( screen, x1, y1, x2, y2, 1, 230 - ( k * 50 ), 250 - ( k * 50 ), 255 );
			}
			lineRGBA ( screen, 400 + map.shift_x, 300 + map.shift_y, map.sector_link_x[i] + map.shift_x, map.sector_link_y[i] + map.shift_y, 1, 240, 250, 255 );

			//draw little icon
			if ( server[s_id].zone[z].sector[map.sector_link[i]].planet >= 0 ) //has planet
			{
				if ( load_map_planet ( s_id, server[s_id].zone[z].sector[map.sector_link[i]].planet ) )
				{
					destination.x = map.sector_link_x[i] - ( server[s_id].planet[server[s_id].zone[z].sector[map.sector_link[i]].planet].hyper_img->w / 2 );
					destination.y = map.sector_link_y[i] - ( server[s_id].planet[server[s_id].zone[z].sector[map.sector_link[i]].planet].hyper_img->h / 2 );
					destination.x += map.shift_x;
					destination.y += map.shift_y;
					SDL_BlitSurface ( server[s_id].planet[server[s_id].zone[z].sector[map.sector_link[i]].planet].hyper_img, NULL, screen, &destination );
				}
				else
				{
					//use nif image
					if ( !game.nif_image.no_hyper_p_img )
						game.nif_image.no_hyper_p_img = IMG_Load ( "graphics/nif/nif_hyper_p.gif" );

					if ( game.nif_image.no_hyper_p_img )
					{
						destination.x = map.sector_link_x[i] - ( game.nif_image.no_hyper_p_img->w / 2 );
						destination.y = map.sector_link_y[i] - ( game.nif_image.no_hyper_p_img->h / 2 );
						destination.x += map.shift_x;
						destination.y += map.shift_y;
						SDL_BlitSurface ( game.nif_image.no_hyper_p_img, NULL, screen, &destination );
					}
				}
			}
			else //normal sector
			{
				if ( server[s_id].zone[z].sector[map.sector_link[i]].is_sun )
				{
					destination.x = ( short int ) ( map.sector_link_x[i] - sun_offset_x );
					destination.y = ( short int ) ( map.sector_link_y[i] - sun_offset_y );
					destination.x += map.shift_x;
					destination.y += map.shift_y;
					SDL_BlitSurface ( map.sun_img, NULL, screen, &destination );
				}
				else if ( server[s_id].zone[z].sector[map.sector_link[i]].shield )
				{
					destination.x = map.sector_link_x[i] - ( server[s_id].zone[z].hyper_safe->w / 2 );
					destination.y = map.sector_link_y[i] - ( server[s_id].zone[z].hyper_safe->h / 2 );
					destination.x += map.shift_x;
					destination.y += map.shift_y;
					SDL_BlitSurface ( server[s_id].zone[z].hyper_safe, NULL, screen, &destination );
				}
				else
				{
					destination.x = map.sector_link_x[i] - ( server[s_id].zone[z].hyper_sel->w / 2 );
					destination.y = map.sector_link_y[i] - ( server[s_id].zone[z].hyper_sel->h / 2 );
					destination.x += map.shift_x;
					destination.y += map.shift_y;
					SDL_BlitSurface ( server[s_id].zone[z].hyper_sel, NULL, screen, &destination );
				}
			}
			
			//either way, is there an enemy there?
			if( server[s_id].zone[z].sector[s].has_player && (server[s_id].zone[z].free_scan || server[s_id].zone[z].sector[s].free_scan) )
			{
				destination.x = ( short int ) ( map.sector_link_x[i] - 15 );
				destination.y = ( short int ) ( map.sector_link_y[i] );
				destination.x += map.shift_x;
				destination.y += map.shift_y;
				SDL_BlitSurface ( map.e_mark, NULL, screen, &destination );
			}
			
			if(server[s_id].zone[z].sector[s].has_guild_member)
			{
				destination.x = ( short int ) ( map.sector_link_x[i] + 10 );
				destination.y = ( short int ) ( map.sector_link_y[i] - 2);
				destination.x += map.shift_x;
				destination.y += map.shift_y;
				SDL_BlitSurface ( map.g_shield, NULL, screen, &destination );
			}
		}

	}

	//draw center image
	destination.x = 385;
	destination.y = 285;
	destination.x += map.shift_x;
	destination.y += map.shift_y;
	SDL_BlitSurface ( map.center_button, NULL, screen, &destination );

	//draw our little ship image
	if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].rad_img[game.ship[game.ship_sel].kind] )
	{
		sprintf ( temp_filename, "server/%s/radships/%s/%s.jpg", server[game.ship[game.ship_sel].server_id].servername, ship_name ( game.ship[game.ship_sel].kind, temp_str ), server[game.server_id].ship[game.ship[game.ship_sel].type].name );
		server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].rad_img[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
	}
	if ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].rad_img[game.ship[game.ship_sel].kind] )
	{
		destination.x = 400 - ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].rad_img[game.ship[game.ship_sel].kind]->w / 2 );
		destination.y = 300 - ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].rad_img[game.ship[game.ship_sel].kind]->h / 2 );
		destination.x += map.shift_x;
		destination.y += map.shift_y;
		SDL_BlitSurface ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].rad_img[game.ship[game.ship_sel].kind], NULL, screen, &destination );
	}
	else
	{
		;//nif image
	}
}

void map_redraw_side_hud()
{
	SDL_Rect destination;

	destination.x = 0;
	destination.y = 0;

	SDL_BlitSurface ( map.backround_cutout, NULL, screen, &destination );
}

void map_draw_nebula_and_sun()
{
	int i, k;
	int glow_radius, glow_int;
	int range_max;
	range_max = game.ship[game.ship_sel].range;

	if ( !map.zoom_level )
	{
		glow_radius = SUN_RADIUS;
		glow_int = 4;
	}
	else if ( map.zoom_level > 0 )
	{
		glow_radius = ( int ) ( SUN_RADIUS * pow ( ZOOM_IN_AMT, map.zoom_level ) );
		glow_int = ( int ) ( 4 * pow ( ZOOM_IN_AMT, map.zoom_level ) );
	}
	else if ( map.zoom_level < 0 )
	{
		glow_radius = ( int ) ( SUN_RADIUS * pow ( ZOOM_OUT_AMT, -map.zoom_level ) );
		glow_int = ( int ) ( 4 * pow ( ZOOM_OUT_AMT, -map.zoom_level ) );
	}

	for ( i=0;i<SECTOR_MAX;i++ )
		if ( server[game.server_id].zone[game.zone].sector[i].x ) //if sector is there
		{
			if ( server[game.server_id].zone[game.zone].sector[i].is_sun )
			{
				int x, y;

				x = 400 + ( server[game.server_id].zone[game.zone].sector[i].x - map.sector_x );
				y = 300 + ( server[game.server_id].zone[game.zone].sector[i].y - map.sector_y );
				x += map.shift_x;
				y += map.shift_y;

				for ( k=15;k<glow_radius;k+=glow_int )
					filledCircleRGBA ( screen, x, y, k, 255, 200, 0, 10 );
			}
		}

	for ( i=0;i<SECTOR_MAX;i++ )
		if ( server[game.server_id].zone[game.zone].sector[i].x ) //if sector is there
		{
			if ( server[game.server_id].zone[game.zone].sector[i].range > range_max ) continue;

			if ( server[game.server_id].zone[game.zone].sector[i].is_nebula )
			{
				SDL_Rect destination;

				destination.x = ( 400 + ( server[game.server_id].zone[game.zone].sector[i].x - map.sector_x ) ) - ( map.nebula_img->w / 2 );
				destination.y = ( 300 + ( server[game.server_id].zone[game.zone].sector[i].y - map.sector_y ) ) - ( map.nebula_img->h / 2 );
				destination.x += map.shift_x;
				destination.y += map.shift_y;

				SDL_BlitSurface ( map.nebula_img, NULL, screen, &destination );
			}
		}
}

void check_to_load_server_sun_damages()
{
	int s;

	s = game.server_id;

	if ( server[s].set_zone_sun_damages ) return;

	set_sun_damages ( server[s].zone );

	server[s].set_zone_sun_damages = 1;
}

void clear_ship_info ( int sh )
{
	int i, j, k;

	game.ship[sh].server_id = -1;
	game.ship[sh].type = -1;
	game.ship[sh].kind = -1;

	//guns/misc out
	for ( i=0;i<16;i++ )
	{
		game.ship[sh].gun[i] = -1;
		game.ship[sh].gun_id[i] = -1;
		game.ship[sh].gun_destroyed[i] = 0;

		game.ship[sh].missile[i] = -1;
		game.ship[sh].missile_id[i] = -1;
		game.ship[sh].missile_left[i] = 0;

		game.ship[sh].misc[i] = -1;
	}

	//banging
	set_user_misc(); //make changes to hull/shield/etc based on misc pods
	set_average_gun_speed(); //in case you plan on useing them guns
	set_missile_speed_times(); //in case you plan on useing them missiles
	repair.redraw_weapons = 1; //see em in the repair shop
	space.shield_level = 0; //reset shield every ship change
}

void map_zoom_in()
{
	int s, z, i;

	//check
	if ( map.zoom_level >= ZOOM_MAX ) return;

	//set
	map.zoom_level++;
	z = game.zone;
	s = game.server_id;

	//spank all sectors
	for ( z=0;z<ZONE_MAX;z++ )
		if ( server[s].zone[z].name[0] )
			for ( i=0;i<SECTOR_MAX;i++ )
				if ( server[s].zone[z].sector[i].x )
				{
					server[s].zone[z].sector[i].x = ( int ) ( server[s].zone[z].sector[i].x * ZOOM_IN_AMT );
					server[s].zone[z].sector[i].y = ( int ) ( server[s].zone[z].sector[i].y * ZOOM_IN_AMT );
				}

	//forget no one
	map.shift_x = ( int ) ( map.shift_x * ZOOM_IN_AMT );
	map.shift_y = ( int ) ( map.shift_y * ZOOM_IN_AMT );

	map_set_local_sector_info();

	//now redraw
	map_redraw();

	sdl_flip_mutex();
}

void map_zoom_out()
{
	int s, z, i;

	//check
	if ( map.zoom_level <= -ZOOM_MAX ) return;

	//set
	map.zoom_level--;
	z = game.zone;
	s = game.server_id;

	//spank all sectors
	for ( z=0;z<ZONE_MAX;z++ )
		if ( server[s].zone[z].name[0] )
			for ( i=0;i<SECTOR_MAX;i++ )
				if ( server[s].zone[z].sector[i].x )
				{
					server[s].zone[z].sector[i].x = ( int ) ( server[s].zone[z].sector[i].x * ZOOM_OUT_AMT );
					server[s].zone[z].sector[i].y = ( int ) ( server[s].zone[z].sector[i].y * ZOOM_OUT_AMT );
				}

	//forget no one
	map.shift_x = ( int ) ( map.shift_x * ZOOM_OUT_AMT );
	map.shift_y = ( int ) ( map.shift_y * ZOOM_OUT_AMT );

	map_set_local_sector_info();

	//now redraw
	map_redraw();

	sdl_flip_mutex();
}

void map_redraw()
{
	SDL_BlitSurface ( map.backround, NULL, screen, NULL );

	//hell ya
	map_draw_nebula_and_sun();

	//draw sectors ----------
	map_draw_sector_images();

	//draw connecting sector images
	map_draw_local_sectors();

	//draw scan stuff
	if ( map.scan_sector >= 0 )
		map_draw_scan();

	map_draw_planet_names();

	//think so
	map_redraw_side_hud();

	//draw textbox
	select_text_box ( screen, &map.text_box );
}

void map_set_local_sector_info()
{
	int z, s, i;

	z = game.zone;
	s = game.sector;

	map.sector_x = server[game.server_id].zone[z].sector[s].x;
	map.sector_y = server[game.server_id].zone[z].sector[s].y;
	for ( i=0;i<5;i++ )
	{
		map.sector_link[i] = server[game.server_id].zone[z].sector[s].sector[i];

		if ( map.sector_link[i] >= 0 )
		{
			map.sector_link_x[i] = 400 + ( server[game.server_id].zone[z].sector[map.sector_link[i]].x - map.sector_x );
			map.sector_link_y[i] = 300 + ( server[game.server_id].zone[z].sector[map.sector_link[i]].y - map.sector_y );
			map.button[i].x = map.sector_link_x[i] - 10;
			map.button[i].y = map.sector_link_y[i] - 10;
			map.button[i].x += map.shift_x;
			map.button[i].y += map.shift_y;
		}
		else
		{
			map.button[i].x = 0;
			map.button[i].y = 0;
		}
	}
}

void map_set_shift ( int x, int y, int button_down )
{
	static int x_save, y_save;

	if ( button_down )
	{
		if ( map_check_sector_on_map ( x,y ) )
		{
			x_save = x;
			y_save = y;
			map.do_shift = 1;
		}
	}
	else if ( map.do_shift )
	{
		map.shift_x += x - x_save;
		map.shift_y += y - y_save;

		map.do_shift = 0;

		map_set_local_sector_info();

		//now redraw
		map_redraw();

		sdl_flip_mutex();
	}
}

void trade_sell_all_goods()
{
	char rtn_str[20] = "6,";

	send_con_hub ( rtn_str );
}

int ship_has_weapons ( int sh )
{
	int i, wep_max;
	int s_id, s_k, s_t;

	s_id = game.ship[sh].server_id;
	s_t = game.ship[sh].type;
	s_k = game.ship[sh].kind;

	if ( s_id < 0 ) return 0;
	if ( s_k < 0 ) return 0;
	if ( s_t < 0 ) return 0;

	wep_max = game.ship[sh].gun_ammount;

	for ( i=0;i<wep_max;i++ )
		if ( game.ship[sh].gun[i] > -1 && game.ship[sh].gun_id[i] > -1 && !game.ship[sh].gun_destroyed[i] )
			break;

	if ( i!=wep_max )
		return 1;

	wep_max = game.ship[sh].missile_ammount;

	for ( i=0;i<wep_max;i++ )
		if ( game.ship[sh].missile[i] > -1 && game.ship[sh].missile_id[i] > -1 )
			break;

	if ( i!=wep_max )
		return 1;

	return 0;
}

double ship_equipment_worth ( int sh )
{
	double the_worth;
	int gun_max, missile_max, misc_max;
	int i;
	int w, w_id;

	//set shit
	the_worth = 0;
	gun_max = game.ship[sh].gun_ammount;
	missile_max = game.ship[sh].missile_ammount;
	misc_max = game.ship[sh].misc_ammount;

	for ( i=0;i<gun_max;i++ )
	{
		w = game.ship[sh].gun[i];
		w_id = game.ship[sh].gun_id[i];

		if ( w<0 ) continue;
		if ( w_id<0 ) continue;
		if ( w>=GUN_MAX ) continue;
		if ( w_id>=MAX_SERVER ) continue;

		the_worth += 0.75 * server[w_id].gun[w].cost;
	}

	for ( i=0;i<missile_max;i++ )
	{
		w = game.ship[sh].missile[i];
		w_id = game.ship[sh].missile_id[i];

		if ( w<0 ) continue;
		if ( w_id<0 ) continue;
		if ( w>=MISSILE_MAX ) continue;
		if ( w_id>=MAX_SERVER ) continue;

		the_worth += 0.75 * server[w_id].missile[w].cost;
	}

	for ( i=0;i<misc_max;i++ )
	{
		w = game.ship[sh].misc[i];

		if ( w<0 ) continue;
		if ( w>=8 ) continue;

		the_worth += 0.75 * misc_pod_cost ( w );
	}

	return the_worth;
}

void space_draw_enemy_jump_status()
{
	SDL_Rect obox, ibox;
	int i;
	int u;

	u = space.engaging;

	obox.w = 4;
	obox.h = 4;

	ibox.w = 2;
	ibox.h = 2;

	obox.x = space.button[u].x;
	obox.y = space.button[u].y + 5 + 5;

	ibox.x = obox.x + 1;
	ibox.y = obox.y + 1;

	//draw the outter
	if ( space.visible )
		SDL_FillRect ( screen, &obox, SDL_MapRGB ( screen->format, 99, 72, 0 ) );

	//draw the inner
	if ( space.user[u].jump_status )
		if ( space.visible )
			SDL_FillRect ( screen, &ibox, SDL_MapRGB ( screen->format, 255, 216, 109 ) );

}

void space_draw_enemy_shield()
{
	SDL_Rect obox, ibox;
	int i;
	int u;

	u = space.engaging;

	obox.w = 4;
	obox.h = 4;

	ibox.w = 2;
	ibox.h = 2;

	obox.x = space.button[u].x;
	obox.y = space.button[u].y + 5;

	ibox.x = obox.x + 1;
	ibox.y = obox.y + 1;

	for ( i=0;i<4;i++ )
	{
		//draw the outters
		if ( space.visible )
			SDL_FillRect ( screen, &obox, SDL_MapRGB ( screen->format, 20, 0, 171 ) );

		//draw the inners
		if ( i<space.user[u].shield_level )
		{
			ibox.x = obox.x + 1;

			if ( space.visible )
				SDL_FillRect ( screen, &ibox, SDL_MapRGB ( screen->format, 0, 220, 220 ) );
		}

		//now slap this
		obox.x += 5;
	}

	space.user[u].refresh_shield = 0;
}

void private_chat_send ( char *message )
{
	char rtn_str[2000];
	int i, len;

	//check for other command
	if ( check_for_command ( message ) ) return;

	len = strlen ( message );

	//check for ldm
	for ( i=1;i<22 && i<len;i++ )
		if ( message[i-1] == ':' && message[i] == ':' )
		{
			message[i-1] = '\0';
			sprintf ( rtn_str, "14,%s,%s", message, message + i + 1 );

			//put it
			strcpy ( game.last_ldm, message );

			//well it works
			jabber_send ( message, message + i + 1 );

			message[i-1] = ':';
			send_con_server ( rtn_str );
			return;
		}

	//add to our own chat
	sprintf ( rtn_str,"%s : %s",game.username ,message );
	add_private_chat ( 0,rtn_str );

	sprintf ( rtn_str, "45,%s", message );
	send_con_hub ( rtn_str );
}

void private_chat_add ( char *username )
{
	char rtn_str[2000];

	sprintf ( rtn_str, "42,%s", username );
	send_con_hub ( rtn_str );
}

void private_chat_remove ( char *username )
{
	char rtn_str[2000];

	sprintf ( rtn_str, "43,%s", username );
	send_con_hub ( rtn_str );
}

void private_chat_list()
{
	char rtn_str[2000] = "44,";

	send_con_hub ( rtn_str );
}

void private_chat_online_list()
{
	char rtn_str[2000] = "9,";

	send_con_hub ( rtn_str );
}

void clear_private_chat_entries()
{
	int i;

	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
	{
		game.private_chat_entry[i].username[0] = 0;
		game.private_chat_entry[i].status = 0;
	}
}

void private_chat_add_edit_entry ( char *username, int status )
{
	int i;

	//some checks
	if ( !username[0] ) return;
	if ( status != 1 && status != 2 ) return;

	//find empty slot
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( !game.private_chat_entry[i].username[0] )
			break;

	if ( i==MAX_PRIVATE_CHAT_USERS )
		return;

	//k well cap a bitch
	username[20] = 0;

	//use a bitch
	strcpy ( game.private_chat_entry[i].username, username );
	game.private_chat_entry[i].status = status;
}

void private_chat_remove_entry ( char *username )
{
	int i;

	//find and remove
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( str_match ( username, game.private_chat_entry[i].username ) )
		{
			game.private_chat_entry[i].username[0] = 0;
			game.private_chat_entry[i].status = 0;
		}
}

void private_chat_load_entry_list ( char *message )
{
	int i, point, len;
	char temp_str[2000];

	//do this
	clear_private_chat_entries();

	//now begin the addin
	point = 0;
	len = strlen ( message );
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS && point<len;i++ )
	{
		split ( temp_str, message, ',', &point );
		temp_str[20] = 0; //cap
		if ( !temp_str[0] ) continue; //exit if doesnt exist
		strcpy ( game.private_chat_entry[i].username, temp_str );
		split ( temp_str, message, ',', &point );
		game.private_chat_entry[i].status = atoi ( temp_str );
	}
}

void do_chat_button ( char *username )
{
	int i;

	//see if we have sexual relations with this folk
	for ( i=0;i<MAX_PRIVATE_CHAT_USERS;i++ )
		if ( str_match ( username, game.private_chat_entry[i].username ) )
			break;

	if ( i==MAX_PRIVATE_CHAT_USERS ) //not found
		private_chat_add ( username );
	else //found
		private_chat_remove ( username );
}

int next_shop_type_down()
{
	int i;
	int s, p;
	
	//only for the ship type	
	if ( shop.shop_sel ) return shop.ship_type_sel;
	
	s = game.server_id;
	p = planet.last_planet;
	
	//begin
	if(at_home_planet())
	{
		for(i = shop.ship_type_sel - 1; i>=0; i--)
			if(server[s].planet[p].ship[i].have || game.has_plan[s][i])
				return i;
	}
	else
	{
		for(i = shop.ship_type_sel - 1; i>=0; i--)
			if(server[s].planet[p].ship[i].have)
				return i;
	}
	
	//none found
	return shop.ship_type_sel;
}

int next_shop_type_up()
{
	int i;
	int s, p;
	
	//only for the ship type	
	if ( shop.shop_sel ) return shop.ship_type_sel;
	
	s = game.server_id;
	p = planet.last_planet;
	
	//begin
	if(at_home_planet())
	{
		for(i = shop.ship_type_sel + 1; i<SHIP_MAX; i++)
			if(server[s].planet[p].ship[i].have || game.has_plan[s][i])
				return i;
	}
	else
	{
		for(i = shop.ship_type_sel + 1; i<SHIP_MAX; i++)
			if(server[s].planet[p].ship[i].have)
				return i;
	}
	
	//none found
	return shop.ship_type_sel;
}

int next_shop_kind_down()
{
	int i;
	int s, p;
	
	s = game.server_id;
	p = planet.last_planet;
	
	switch ( shop.shop_sel )
	{
		case 0: //chassis
			for(i=shop.ship_kind_sel-1;i>=0;i--)
				if(shop_ship_available_for_purchase(shop.ship_type_sel, i))
					return i;
			
			//not found
			return shop.ship_kind_sel;
			
			break;
		case 1: //guns
			for(i=shop.gun_sel-1;i>=0;i--)
				if(server[s].planet[p].gun[i])
					return i;
			
			return shop.gun_sel;
			
			break;
		case 2: //missiles
			for(i=shop.missile_sel-1;i>=0;i--)
				if(server[s].planet[p].missile[i])
					return i;
			
			return shop.missile_sel;
			
			break;
		case 3: //misc
			if ( shop.misc_sel <= 0 ) return shop.misc_sel;

			return shop.misc_sel-1;
			break;
	}
}

int next_shop_kind_up()
{
	int i;
	int s, p;
	
	s = game.server_id;
	p = planet.last_planet;
	
	switch ( shop.shop_sel )
	{
		case 0: //chassis
			for(i=shop.ship_kind_sel+1;i<=7;i++)
				if(shop_ship_available_for_purchase(shop.ship_type_sel, i))
					return i;
			
			//not found
			return shop.ship_kind_sel;
			
			break;
		case 1: //guns
			for(i=shop.gun_sel+1;i<GUN_MAX;i++)
				if(server[s].planet[p].gun[i])
					return i;
		
			return shop.gun_sel;
		
			break;
		case 2: //missiles
			for(i=shop.missile_sel+1;i<MISSILE_MAX;i++)
				if(server[s].planet[p].missile[i])
					return i;
	
			return shop.missile_sel;
	
			break;
		case 3: //misc
			if ( shop.misc_sel >= 7 ) return shop.misc_sel;

			return shop.misc_sel+1;
			break;
	}
}

void shop_show_buy_button()
{
	int show_it, got_space;
	int i, s;
	int sh, sh_id, sh_t, sh_k, max;
	
	s = game.server_id;
	sh = game.ship_sel;
	sh_id = game.ship[sh].server_id;
	sh_t = game.ship[sh].type;
	sh_k = game.ship[sh].kind;
	
	show_it = 0;
	got_space = 0;
	
	switch ( shop.shop_sel )
	{
		case 0: //chassis
			if(game.money < server[s].ship[shop.ship_type_sel].cost[shop.ship_kind_sel]) break;
			if(game.exp < server[s].ship[shop.ship_type_sel].exp[shop.ship_kind_sel]) break;
			if(sh_id > -1) break;
			
			show_it = 1;
			
			break;
		case 1: //guns
			if(sh_id > -1 && sh_t > -1 && sh_k > -1)
			{
				max = server[sh_id].ship[sh_t].gun_ammount[sh_k];
				for ( i=0;i<max;i++ )
					if ( game.ship[sh].gun_id[i] < 0 || game.ship[sh].gun[i] < 0 ) //check for emptyness
				{
					got_space = 1;
					break;
				}
			}
			
			if(!got_space) break;
			if(game.money < server[s].gun[shop.gun_sel].cost) break;
			if(game.exp < gun_exp_requirement(server[s].gun, shop.gun_sel, server[sh_id].ship[sh_t].exp[sh_k])) break;
			
			show_it = 1;
			break;
		case 2: //missiles
			if(sh_id > -1 && sh_t > -1 && sh_k > -1)
			{
				max = server[sh_id].ship[sh_t].missile_ammount[sh_k];
				for ( i=0;i<max;i++ )
					if ( game.ship[sh].missile_id[i] < 0 || game.ship[sh].missile[i] < 0 ) //check for emptyness
				{
					got_space = 1;
					break;
				}
			}
			
			if(!got_space) break;
			if(game.money < server[s].missile[shop.missile_sel].cost) break;
			if(game.exp < missile_exp_requirement(server[s].missile, shop.missile_sel, server[sh_id].ship[sh_t].exp[sh_k])) break;
			
			show_it = 1;
			break;
		case 3: //misc
			if(sh_id > -1 && sh_t > -1 && sh_k > -1)
			{
				max = server[sh_id].ship[sh_t].misc_ammount[sh_k];
				for ( i=0;i<max;i++ )
					if ( game.ship[sh].misc[i] < 0 ) //check for emptyness
				{
					got_space = 1;
					break;
				}
			}
			
			if(!got_space) break;
			if( game.money < misc_pod_cost ( shop.misc_sel )) break;
			if(game.exp < misc_pod_exp ( shop.misc_sel ) + server[sh_id].ship[sh_t].exp[sh_k]) break;
			
			show_it = 1;
			break;
	}
	
	if(show_it)
		SDL_BlitSurface ( shop.buy_img, NULL, shop.temp_backround, &shop.button[1] );
	else
		SDL_BlitSurface ( shop.buy_blank, NULL, shop.temp_backround, &shop.button[1] );
}

void shop_show_type_left_right()
{
	int test_type;
	
	//only do this for the ship screen
	if(shop.shop_sel) return;
	
	test_type = shop.ship_type_sel;
	
	//are we currently at the bottom?
	if ( test_type == next_shop_type_down() )
		SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[3] );
	else
		SDL_BlitSurface ( shop.sel_left, NULL, shop.temp_backround, &shop.button[3] );

	//are we currently at the top?
	if ( test_type == next_shop_type_up() )
		SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[4] );
	else
		SDL_BlitSurface ( shop.sel_right, NULL, shop.temp_backround, &shop.button[4] );
	
}

void shop_show_kind_left_right()
{
	int test_kind;
	
	switch ( shop.shop_sel )
	{
		case 0: //chassis
			test_kind = shop.ship_kind_sel;
			break;
		case 1: //guns
			test_kind = shop.gun_sel;
			break;
		case 2: //missiles
			test_kind = shop.missile_sel;
			break;
		case 3: //misc
			test_kind = shop.misc_sel;
			break;
	}
	
	if ( test_kind == next_shop_kind_down() ) //if sellected first ship then remove left select button
		SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[5] );
	else
		SDL_BlitSurface ( shop.sel_left, NULL, shop.temp_backround, &shop.button[5] );

	if ( test_kind == next_shop_kind_up() )
		SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[6] );
	else
		SDL_BlitSurface ( shop.sel_right, NULL, shop.temp_backround, &shop.button[6] );
}

int shop_ship_available_for_purchase(int sh_t, int sh_k)
{
	int max;
	
	int &s = game.server_id;
	int &p = planet.last_planet;
	
	//we got the plan?
	if(at_home_planet() && game.has_plan[s][sh_t]) return 1;
	
	if(server[s].planet[p].ship[shop.ship_type_sel].max < 7)
		max = server[s].planet[p].ship[shop.ship_type_sel].max;
	else
		max = 7;
	
	//it excluded?
	if(server[s].planet[p].ship[sh_t].exclude[sh_k]) return 0;
	
	//over the max?
	if(sh_k>max) return 0;
	
	//then ok
	return 1;
}

void shop_choose_available_ship_kind()
{
	int sh_t, sh_k;
	int temp_kind;
	
	sh_t = shop.ship_type_sel;
	sh_k = shop.ship_kind_sel;
	
	//should we be even choosing another ship kind?
	if(shop_ship_available_for_purchase(sh_t, sh_k)) return;
	
	//go left
	temp_kind = next_shop_kind_down();
	if(temp_kind != sh_k)
	{
		shop.ship_kind_sel = temp_kind;
		return;
	}
	
	temp_kind = next_shop_kind_up();
	if(temp_kind != sh_k)
	{
		shop.ship_kind_sel = temp_kind;
		return;
	}
	
	//well I guess it didn't find anything good?
}

void shop_set_initial_selections()
{
	int i, j;
	int &s = game.server_id;
	int &p = planet.last_planet;
	
	//ship
	for(i=0;i<SHIP_MAX;i++)
		if(server[s].planet[p].ship[i].have)
	{
		shop.ship_type_sel = i;

		//consider plans?
		if(at_home_planet())
		{
			for(j=0;j<=server[s].planet[p].ship[i].max;j++)
				if(!server[s].planet[p].ship[i].exclude[j] || game.has_plan[s][i])
			{
				shop.ship_kind_sel = j;
				break;
			}
		}
		else
		{
			for(j=0;j<=server[s].planet[p].ship[i].max;j++)
				if(!server[s].planet[p].ship[i].exclude[j])
			{
				shop.ship_kind_sel = j;
				break;
			}
		}
		break;
	}
	
	//gun
	for(i=0;i<GUN_MAX;i++)
		if(server[s].planet[p].gun[i])
	{
		shop.gun_sel = i;
		break;
	}
	
	//missile
	for(i=0;i<MISSILE_MAX;i++)
		if(server[s].planet[p].missile[i])
	{
		shop.missile_sel = i;
		break;
	}
	
	//misc
	shop.misc_sel =  0;
}

int shop_can_be_set_to(int shop_type)
{
	int i;
	int &s = game.server_id;
	int &p = planet.last_planet;
	
	switch ( shop.shop_sel )
	{
		case 0: //chassis
			//it got any ships?
			if(at_home_planet())
			{
				for(i=0;i<SHIP_MAX;i++)
					if(server[s].planet[p].ship[i].have || game.has_plan[s][i])
						return 1;
			}
			else
			{
				for(i=0;i<SHIP_MAX;i++)
					if(server[s].planet[p].ship[i].have)
						return 1;
			}
			
			return 0;
			break;
		case 1: //guns
			//got any guns?
			for(i=0;i<GUN_MAX;i++)
				if(server[s].planet[p].gun[i])
					return 1;
			
			return 0;
			break;
		case 2: //missiles
			//got any missiles?
			for(i=0;i<MISSILE_MAX;i++)
				if(server[s].planet[p].missile[i])
					return 1;
			
			return 0;
			break;
		case 3: //misc
			return 1;
			break;
	}
}

void set_shop_type_down()
{
	int previous_kind_max;
	int temp_type;
	char temp_str[21];

	//if this isnt chassis then exit (only chassis uses upmost buttons)
	if ( shop.shop_sel ) return;
	
	//find the next type;
	temp_type = next_shop_type_down();
	if(temp_type != shop.ship_type_sel)
		shop.ship_type_sel = temp_type;
	else
		return;

	shop_show_type_left_right();

	shop_choose_available_ship_kind();
	draw_center_label ( shop.temp_backround, ship_name ( shop.ship_kind_sel, temp_str ), &shop.sel_left_label[1], 255, 255, 255 );

	shop_show_kind_left_right();

	shop_display_buy_ship_stats();

	shop.attempt_load_buy = 0;
	shop.rotate = 0; //reset index to start

	shop_show_buy_button();

	SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void set_shop_type_up()
{
	int previous_kind_max;
	char temp_str[21];
	int temp_type;

	if ( shop.shop_sel ) return; //if this isnt chassis then exit (only chassis uses upmost buttons)
	
	temp_type = next_shop_type_up();
	if(temp_type != shop.ship_type_sel)
		shop.ship_type_sel = temp_type;
	else
		return;

	shop_show_type_left_right();

	shop_choose_available_ship_kind();
	draw_center_label ( shop.temp_backround, ship_name ( shop.ship_kind_sel, temp_str ), &shop.sel_left_label[1], 255, 255, 255 );

	shop_show_kind_left_right();

	shop_display_buy_ship_stats();

	shop.attempt_load_buy = 0;
	shop.rotate = 0; //reset index to start

	shop_show_buy_button();

	SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void set_shop_kind_down()
{
	char temp_str[21];
	int i, show_buy_img;
	int temp_kind;

	switch ( shop.shop_sel )
	{
		case 0: //chassis
			temp_kind = next_shop_kind_down();
			if(temp_kind == shop.ship_kind_sel) return;
			shop.ship_kind_sel = temp_kind;

			shop_show_kind_left_right();

			//yayayaya
			shop_display_buy_ship_stats();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
		case 1: //guns
			temp_kind = next_shop_kind_down();
			if(temp_kind == shop.gun_sel) return;
			shop.gun_sel = temp_kind;

			shop_show_kind_left_right();

			//heyheyhey
			shop_display_buy_gun_stats();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
		case 2: //missiles
			temp_kind = next_shop_kind_down();
			if(temp_kind == shop.missile_sel) return;
			shop.missile_sel = temp_kind;

			shop_show_kind_left_right();

			//waaaahhh
			shop_display_buy_missile_stats();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
		case 3: //misc
			temp_kind = next_shop_kind_down();
			if(temp_kind == shop.misc_sel) return;
			shop.misc_sel = temp_kind;

			shop_show_kind_left_right();

			//ya
			shop_display_buy_misc();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
	}


	SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void set_shop_kind_up()
{
	char temp_str[21];
	int i, show_buy_img;
	int temp_kind;

	switch ( shop.shop_sel )
	{
		case 0: //chassis
			temp_kind = next_shop_kind_up();
			if(temp_kind == shop.ship_kind_sel) return;
			shop.ship_kind_sel = temp_kind;

			shop_show_kind_left_right();

			//as it states
			shop_display_buy_ship_stats();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
		case 1: //guns
			temp_kind = next_shop_kind_up();
			if(temp_kind == shop.gun_sel) return;
			shop.gun_sel = temp_kind;

			shop_show_kind_left_right();

			//as it states
			shop_display_buy_gun_stats();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
		case 2: //missiles
			temp_kind = next_shop_kind_up();
			if(temp_kind == shop.missile_sel) return;
			shop.missile_sel = temp_kind;

			shop_show_kind_left_right();

			//as it states
			shop_display_buy_missile_stats();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
		case 3: //misc
			temp_kind = next_shop_kind_up();
			if(temp_kind == shop.misc_sel) return;
			shop.misc_sel = temp_kind;

			shop_show_kind_left_right();

			shop_display_buy_misc();

			shop.attempt_load_buy = 0;
			shop.rotate = 0; //reset index to start

			shop_show_buy_button();

			break;
	}

	SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void shop_display_buy_ship_stats()
{
	char temp_str[200];

	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].cost[shop.ship_kind_sel] ); //Set Purchase Ship info
	draw_right_label ( shop.temp_backround, temp_str, &shop.buy_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].exp[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].hull[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[1], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].shield[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[2], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].range[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[3], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].speed[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[4], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].evasion[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[5], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].cargo[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[6], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].gun_ammount[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[7], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].missile_ammount[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[8], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[game.server_id].ship[shop.ship_type_sel].misc_ammount[shop.ship_kind_sel] );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[9], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, server[game.server_id].ship[shop.ship_type_sel].name, &shop.sel_left_label[0], 255, 255, 255 ); //Set Selection Names
	draw_center_label ( shop.temp_backround, ship_name ( shop.ship_kind_sel, temp_str ), &shop.sel_left_label[1], 255, 255, 255 );
}

void shop_display_buy_gun_stats()
{
	char temp_str[200];
	int i;
	int s, w, sh_exp;

	clear_display_box ( shop.temp_backround, & ( shop.left_display_box ) );

	s = game.server_id;
	w = shop.gun_sel;
	sh_exp = game.ship[game.ship_sel].exp;

	sprintf ( temp_str,"%d",server[s].gun[w].cost ); //Set Purchase Gun info and clear others
	draw_right_label ( shop.temp_backround, temp_str, &shop.buy_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d", gun_exp_requirement ( server[s].gun, w, sh_exp ) );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].accuracy );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[1], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].speed );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[2], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].damage );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[3], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].tech_level );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[4], 255, 255, 255 );
	draw_right_label ( shop.temp_backround, gun_type_name ( temp_str, server[s].gun[w].type ), &shop.stat_left_label[5], 255, 255, 255 );
	for ( i=6;i<10;i++ ) //clear the rest
		draw_right_label ( shop.temp_backround, "", &shop.stat_left_label[i], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, "", &shop.sel_left_label[0], 255, 255, 255 );
	draw_center_label ( shop.temp_backround, server[s].gun[w].name, &shop.sel_left_label[1], 255, 255, 255 );

	add_display_box ( shop.temp_backround, & ( shop.left_display_box ), 0, gun_type_description ( temp_str, server[s].gun[w].type ) );
}

void shop_display_buy_missile_stats()
{
	char temp_str[200];
	int i;
	int s, w, sh_exp;

	clear_display_box ( shop.temp_backround, & ( shop.left_display_box ) );

	s = game.server_id;
	w = shop.missile_sel;
	sh_exp = game.ship[game.ship_sel].exp;

	sprintf ( temp_str,"%d",server[s].missile[w].cost ); //Set Purchase Gun info and clear others
	draw_right_label ( shop.temp_backround, temp_str, &shop.buy_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d", missile_exp_requirement ( server[s].missile, w, sh_exp ) );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].accuracy );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[1], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].speed );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[2], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].damage );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[3], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].tech_level );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[4], 255, 255, 255 );
	draw_right_label ( shop.temp_backround, missile_type_name ( temp_str, server[s].missile[w].type ), &shop.stat_left_label[5], 255, 255, 255 );
	for ( i=6;i<10;i++ ) //clear the rest
		draw_right_label ( shop.temp_backround, "", &shop.stat_left_label[i], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, "", &shop.sel_left_label[0], 255, 255, 255 );
	draw_center_label ( shop.temp_backround, server[s].missile[w].name, &shop.sel_left_label[1], 255, 255, 255 );

	add_display_box ( shop.temp_backround, & ( shop.left_display_box ), 0, missile_type_description ( temp_str, server[s].missile[w].type ) );
}

void shop_display_buy_misc()
{
	char temp_str[200];
	int i;

	clear_display_box ( shop.temp_backround, & ( shop.left_display_box ) );

	switch ( shop.misc_sel )
	{
		case 0:
		case 4:
			draw_label ( shop.temp_backround, "Hull Increase ::", &shop.info_left_label[1], 255, 255, 255 );
			break;
		case 1:
		case 5:
			draw_label ( shop.temp_backround, "Shield Increase ::", &shop.info_left_label[1], 255, 255, 255 );
			break;
		case 2:
		case 6:
			draw_label ( shop.temp_backround, "Cargo Increase ::", &shop.info_left_label[1], 255, 255, 255 );
			break;
		case 3:
		case 7:
			draw_label ( shop.temp_backround, "Speed Increase ::", &shop.info_left_label[1], 255, 255, 255 );
			break;
	}

	sprintf ( temp_str,"%d",misc_pod_cost ( shop.misc_sel ) ); //Set Purchase Gun info and clear others
	draw_right_label ( shop.temp_backround, temp_str, &shop.buy_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d",misc_pod_exp ( shop.misc_sel ) + game.ship[game.ship_sel].exp );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",misc_pod_value ( shop.misc_sel ) );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_left_label[1], 255, 255, 255 );
	for ( i=2;i<10;i++ ) //clear the rest
		draw_right_label ( shop.temp_backround, "", &shop.stat_left_label[i], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, misc_pod_name ( shop.misc_sel, temp_str ), &shop.sel_left_label[1], 255, 255, 255 );
}

void shop_display_sell_ship()
{
	char temp_str[200];

	sprintf ( temp_str,"%.0lf", ( game.ship[game.ship_sel].cost * 0.75 ) + ship_equipment_worth ( game.ship_sel ) ); //Set Purchase Ship info
	draw_right_label ( shop.temp_backround, temp_str, &shop.sell_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].exp );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].hull );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[1], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].shield );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[2], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].range );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[3], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].speed );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[4], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].evasion );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[5], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].cargo );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[6], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].gun_ammount );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[7], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].missile_ammount );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[8], 255, 255, 255 );
	sprintf ( temp_str,"%d",game.ship[game.ship_sel].misc_ammount );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[9], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name, &shop.sel_right_label[0], 255, 255, 255 ); //Set Selection Names
	draw_center_label ( shop.temp_backround, ship_name ( game.ship[game.ship_sel].kind, temp_str ), &shop.sel_right_label[1], 255, 255, 255 );
}

void shop_display_sell_gun()
{
	char temp_str[200];
	int i;
	int s, w, sh_exp;

	//draw the red
	shop_draw_sell_item_selected ( shop.gun_pod_sel );

	clear_display_box ( shop.temp_backround, & ( shop.right_display_box ) );

	s = game.ship[game.ship_sel].gun_id[shop.gun_pod_sel];
	w = game.ship[game.ship_sel].gun[shop.gun_pod_sel];
	sh_exp = game.ship[game.ship_sel].exp;

	if ( s<0 || w<0 ) //clear out?
	{
		draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );
		SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
		for ( i=0;i<10;i++ )
			draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );
		draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );
		return;
	}

	sprintf ( temp_str,"%.0lf",server[s].gun[w].cost * .75 ); //Set Purchase Gun info and clear others
	draw_right_label ( shop.temp_backround, temp_str, &shop.sell_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d", gun_exp_requirement ( server[s].gun, w, sh_exp ) );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].accuracy );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[1], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].speed );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[2], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].damage );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[3], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].gun[w].tech_level );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[4], 255, 255, 255 );
	draw_right_label ( shop.temp_backround, gun_type_name ( temp_str, server[s].gun[w].type ), &shop.stat_right_label[5], 255, 255, 255 );
	for ( i=6;i<10;i++ ) //clear the rest
		draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 );
	draw_center_label ( shop.temp_backround, server[s].gun[w].name, &shop.sel_right_label[1], 255, 255, 255 );

	add_display_box ( shop.temp_backround, & ( shop.right_display_box ), 0, gun_type_description ( temp_str, server[s].gun[w].type ) );
}

void shop_display_sell_missile()
{
	char temp_str[200];
	int i;
	int s, w, sh_exp;

	//draw the red
	shop_draw_sell_item_selected ( shop.missile_pod_sel );

	clear_display_box ( shop.temp_backround, & ( shop.right_display_box ) );

	s = game.ship[game.ship_sel].missile_id[shop.missile_pod_sel];
	w = game.ship[game.ship_sel].missile[shop.missile_pod_sel];
	sh_exp = game.ship[game.ship_sel].exp;

	if ( s<0 || w<0 ) //clear out?
	{
		draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );
		SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
		for ( i=0;i<10;i++ )
			draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );
		draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );
		return;
	}

	sprintf ( temp_str,"%.0lf",server[s].missile[w].cost * .75 ); //Set Purchase Gun info and clear others
	draw_right_label ( shop.temp_backround, temp_str, &shop.sell_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d", missile_exp_requirement ( server[s].missile, w, sh_exp ) );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].accuracy );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[1], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].speed );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[2], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].damage );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[3], 255, 255, 255 );
	sprintf ( temp_str,"%d",server[s].missile[w].tech_level );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[4], 255, 255, 255 );
	draw_right_label ( shop.temp_backround, missile_type_name ( temp_str, server[s].missile[w].type ), &shop.stat_right_label[5], 255, 255, 255 );
	for ( i=6;i<10;i++ ) //clear the rest
		draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 );
	draw_center_label ( shop.temp_backround, server[s].missile[w].name, &shop.sel_right_label[1], 255, 255, 255 );

	add_display_box ( shop.temp_backround, & ( shop.right_display_box ), 0, missile_type_description ( temp_str, server[s].missile[w].type ) );
}

void shop_display_sell_misc()
{
	char temp_str[200];
	int i;
	int w;

	//draw the red
	shop_draw_sell_item_selected ( shop.misc_pod_sel );

	w = game.ship[game.ship_sel].misc[shop.misc_pod_sel];

	clear_display_box ( shop.temp_backround, & ( shop.right_display_box ) );

	if ( w<0 ) //clear out?
	{
		draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );
		SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
		for ( i=0;i<10;i++ )
			draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );
		draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );
		return;
	}

	switch ( w )
	{
		case 0:
		case 4:
			draw_label ( shop.temp_backround, "Hull Increase ::", &shop.info_right_label[1], 255, 255, 255 );
			break;
		case 1:
		case 5:
			draw_label ( shop.temp_backround, "Shield Increase ::", &shop.info_right_label[1], 255, 255, 255 );
			break;
		case 2:
		case 6:
			draw_label ( shop.temp_backround, "Cargo Increase ::", &shop.info_right_label[1], 255, 255, 255 );
			break;
		case 3:
		case 7:
			draw_label ( shop.temp_backround, "Speed Increase ::", &shop.info_right_label[1], 255, 255, 255 );
			break;
	}

	sprintf ( temp_str,"%.0lf",misc_pod_cost ( game.ship[game.ship_sel].misc[shop.misc_pod_sel] ) * .75 ); //Set Purchase Gun info and clear others
	draw_right_label ( shop.temp_backround, temp_str, &shop.sell_money_label, 255, 255, 255 );
	sprintf ( temp_str,"%d",misc_pod_exp ( game.ship[game.ship_sel].misc[shop.misc_pod_sel] ) + game.ship[game.ship_sel].exp );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[0], 255, 255, 255 );
	sprintf ( temp_str,"%d",misc_pod_value ( game.ship[game.ship_sel].misc[shop.misc_pod_sel] ) );
	draw_right_label ( shop.temp_backround, temp_str, &shop.stat_right_label[1], 255, 255, 255 );
	for ( i=2;i<10;i++ ) //clear the rest
		draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );

	draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 );
	draw_center_label ( shop.temp_backround, misc_pod_name ( game.ship[game.ship_sel].misc[shop.misc_pod_sel], temp_str ), &shop.sel_right_label[1], 255, 255, 255 );
}

void shop_display_sell_ship_stat_names()
{
	clear_display_box ( shop.temp_backround, & ( shop.right_display_box ) );

	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_right_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "Hull ::", &shop.info_right_label[1], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Shields ::", &shop.info_right_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Jump Range ::", &shop.info_right_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Speed ::", &shop.info_right_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Evasion ::", &shop.info_right_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Cargo ::", &shop.info_right_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Max Guns ::", &shop.info_right_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Max Missles ::", &shop.info_right_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Max Misc Pods ::", &shop.info_right_label[9], 255, 255, 255 );
}

void shop_display_sell_gun_stat_names()
{
	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_right_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "Accuracy ::", &shop.info_right_label[1], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Speed ::", &shop.info_right_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Damage ::", &shop.info_right_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Tech Level ::", &shop.info_right_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Weapon Type ::", &shop.info_right_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Description ::", &shop.info_right_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[9], 255, 255, 255 );
}

void shop_display_sell_missile_stat_names()
{
	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_right_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "Accuracy ::", &shop.info_right_label[1], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Speed ::", &shop.info_right_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Damage ::", &shop.info_right_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Tech Level ::", &shop.info_right_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Weapon Type ::", &shop.info_right_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Description ::", &shop.info_right_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[9], 255, 255, 255 );
}

void shop_display_sell_misc_stat_names()
{
	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_right_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "", &shop.info_right_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_right_label[9], 255, 255, 255 );
}

void shop_display_buy_ship_stat_names()
{
	clear_display_box ( shop.temp_backround, & ( shop.left_display_box ) );

	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_left_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "Hull ::", &shop.info_left_label[1], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Shields ::", &shop.info_left_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Jump Range ::", &shop.info_left_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Speed ::", &shop.info_left_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Evasion ::", &shop.info_left_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Cargo ::", &shop.info_left_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Max Guns ::", &shop.info_left_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Max Missles ::", &shop.info_left_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Max Misc Pods ::", &shop.info_left_label[9], 255, 255, 255 );
}

void shop_display_buy_gun_stat_names()
{
	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_left_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "Accuracy ::", &shop.info_left_label[1], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Speed ::", &shop.info_left_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Damage ::", &shop.info_left_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Tech Level ::", &shop.info_left_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Weapon Type ::", &shop.info_left_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Description ::", &shop.info_left_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[9], 255, 255, 255 );
}

void shop_display_buy_missile_stat_names()
{
	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_left_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "Accuracy ::", &shop.info_left_label[1], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Speed ::", &shop.info_left_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Damage ::", &shop.info_left_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Tech Level ::", &shop.info_left_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Weapon Type ::", &shop.info_left_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "Description ::", &shop.info_left_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[9], 255, 255, 255 );
}

void shop_display_buy_misc_stat_names()
{
	draw_label ( shop.temp_backround, "Exp Needed ::", &shop.info_left_label[0], 255, 255, 255 ); //Set obvious texts
	draw_label ( shop.temp_backround, "", &shop.info_left_label[2], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[3], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[4], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[5], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[6], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[7], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[8], 255, 255, 255 );
	draw_label ( shop.temp_backround, "", &shop.info_left_label[9], 255, 255, 255 );
}

void shop_display_sell_guns()
{
	int i;

	for ( i=0;i<game.ship[game.ship_sel].gun_ammount;i++ ) //show those gun pods
		if ( game.ship[game.ship_sel].gun[i] > -1 && game.ship[game.ship_sel].gun[i] < GUN_MAX && game.ship[game.ship_sel].gun_id[i] > -1 && game.ship[game.ship_sel].gun_id[i] < MAX_SERVER )
		{
			if ( !load_gun_pod_img ( i ) ) //still not loaded... wah
				SDL_BlitSurface ( game.nif_image.no_gun_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
			else //or is it?
				SDL_BlitSurface ( server[game.ship[game.ship_sel].gun_id[i]].gun[game.ship[game.ship_sel].gun[i]].gun_pod_img, NULL, shop.temp_backround, &shop.button[i + 11] );

		}
		else
		{
			SDL_BlitSurface ( shop.empty_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
		}

	for ( ;i<16;i++ ) //blank out remaining spots
		SDL_BlitSurface ( shop.blank_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
}

void shop_draw_sell_item_selected ( int i )
{
	if ( i<0 ) return;
	if ( i>=16 ) return;

	rectangleRGBA ( shop.temp_backround, shop.button[i + 11].x - 1, shop.button[i + 11].y - 1, shop.button[i + 11].x + shop.button[i + 11].w + 1, shop.button[i + 11].y + shop.button[i + 11].h + 1, 255, 0 , 0, 255 );
}

void shop_draw_sell_item_unselected ( int i )
{
	if ( i<0 ) return;
	if ( i>=16 ) return;

	rectangleRGBA ( shop.temp_backround, shop.button[i + 11].x - 1, shop.button[i + 11].y - 1, shop.button[i + 11].x + shop.button[i + 11].w + 1, shop.button[i + 11].y + shop.button[i + 11].h + 1, 0, 0 , 0, 255 );
}

void shop_blank_current_item_sell()
{
	switch ( shop.shop_sel )
	{
		case 1: // gun
			shop_draw_sell_item_unselected ( shop.gun_pod_sel );
			break;
		case 2: // missile
			shop_draw_sell_item_unselected ( shop.missile_pod_sel );
			break;
		case 3: // misc
			shop_draw_sell_item_unselected ( shop.misc_pod_sel );
			break;
	}
}

void shop_select_next_sell_item()
{
	int w;
	int max_w;
	int s_sel, s_id, s_t, s_k;

	s_sel = game.ship_sel;
	s_id = game.ship[s_sel].server_id;
	s_t = game.ship[s_sel].type;
	s_k = game.ship[s_sel].kind;

	switch ( shop.shop_sel )
	{
		case 1: // gun
			max_w = game.ship[s_sel].gun_ammount;

			for ( w=max_w-1;w>=0;w-- )
			{
				//checks
				if ( game.ship[s_sel].gun[w] < 0 ) continue;
				if ( game.ship[s_sel].gun_id[w] < 0 ) continue;
				if ( game.ship[s_sel].gun[w] >= GUN_MAX ) continue;
				if ( game.ship[s_sel].gun_id[w] >= MAX_SERVER ) continue;

				//guess we found one

				//unselect the old one
				shop_draw_sell_item_unselected ( shop.gun_pod_sel );

				//set the new one
				shop.gun_pod_sel = w;

				break;
			}

			break;
		case 2: // missile
			max_w = game.ship[s_sel].missile_ammount;

			for ( w=max_w-1;w>=0;w-- )
			{
				//checks
				if ( game.ship[s_sel].missile[w] < 0 ) continue;
				if ( game.ship[s_sel].missile_id[w] < 0 ) continue;
				if ( game.ship[s_sel].missile[w] >= MISSILE_MAX ) continue;
				if ( game.ship[s_sel].missile_id[w] >= MAX_SERVER ) continue;

				//guess we found one

				//unselect the old one
				shop_draw_sell_item_unselected ( shop.missile_pod_sel );

				//set the new one
				shop.missile_pod_sel = w;

				break;
			}

			break;
		case 3: // misc
			max_w = game.ship[s_sel].misc_ammount;

			for ( w=max_w-1;w>=0;w-- )
			{
				//checks
				if ( game.ship[s_sel].misc[w] < 0 ) continue;

				//guess we found one

				//unselect the old one
				shop_draw_sell_item_unselected ( shop.misc_pod_sel );

				//set the new one
				shop.misc_pod_sel = w;

				break;
			}

			break;
	}
}

void disable_shields()
{
	int old_level;

	//set the time
	space.shield_disable_time = time ( 0 ) + 1;

	//well should we do things at all?
	if ( !space.shield_level ) return;

	//set this
	old_level = space.shield_level;

	//and set
	space.shield_level = 0;

	//err
	draw_our_shield ( old_level );
}

int shields_are_disabled()
{
	return ( time ( 0 ) < space.shield_disable_time + SHIELD_DISABLE_TIME );
}

void draw_exp_rank_img ( int exp, int x, int y )
{
	int i;
	SDL_Rect destination;

	//set a shitter
	destination.x = x;
	destination.y = y;
	destination.w = 60;
	destination.h = 70;

	//find rank
	i = exp_rank ( exp );

	//it good?
	if ( i<0 )
	{
		//draw blank
		SDL_FillRect ( screen, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

		return;
	}

	if ( load_exp_rank_img ( i ) )
	{
		//draw it
		SDL_BlitSurface ( game.game_image.exp_rank[i], NULL, screen, &destination );

		return;
	}
	else
	{
		//draw blank
		SDL_FillRect ( screen, &destination, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

		return;
	}
}

int load_exp_rank_img ( int i )
{
	char file_name[300];

	if ( i<0 ) return 0;
	if ( i>=EXP_RANK_IMG_MAX ) return 0;

	sprintf ( file_name,"graphics/deco/rank%d.gif",i );

	game.game_image.exp_rank[i] = IMG_Load ( file_name );

	if ( game.game_image.exp_rank[i] )
		return 1;

	return 0;
}

int exp_rank ( int exp )
{
	if ( exp>6000 )
		return 13;

	if ( exp>5000 )
		return 12;

	if ( exp>4000 )
		return 11;

	if ( exp>3000 )
		return 10;

	if ( exp>2000 )
		return 9;

	if ( exp>1500 )
		return 8;

	if ( exp>1000 )
		return 7;

	if ( exp>500 )
		return 6;

	if ( exp>300 )
		return 5;

	if ( exp>200 )
		return 4;

	if ( exp>100 )
		return 3;

	if ( exp>50 )
		return 2;

	if ( exp>20 )
		return 1;

	if ( exp>10 )
		return 0;

	return -1;
}

void space_chat_button()
{
	int i;

	i = space.user_sel;

	if ( i==-1 ) return;

	do_chat_button ( space.user[i].username );
}

void change_password ( char *old_password, char *new_password )
{
	char temp_str[1025];

	//checks
	if ( strlen ( old_password ) > 20 )
	{
		sprintf ( temp_str,"... Warning :  Old password too long .." );
		add_chat ( 1,temp_str );

		return;
	}

	if ( strlen ( new_password ) > 20 )
	{
		sprintf ( temp_str,"... Warning :  New password too long .." );
		add_chat ( 1,temp_str );

		return;
	}

	sprintf ( temp_str,"7,%s,%s", old_password, new_password );
	send_con_hub ( temp_str );
}

void guild_award_deco ( char *player_name, char *deco_name )
{
	char rtn_str[2000];

	sprintf ( rtn_str,"47,%s,%s", player_name, deco_name );
	send_con_hub ( rtn_str );
}

void guild_remove_deco ( char *player_name, char *deco_name )
{
	char rtn_str[2000];

	sprintf ( rtn_str,"48,%s,%s", player_name, deco_name );
	send_con_hub ( rtn_str );
}

void guild_list_deco ( char *player_name )
{
	char rtn_str[2000];

	sprintf ( rtn_str,"49,%s", player_name );
	send_con_hub ( rtn_str );
}

void guild_list_bugs()
{
	char rtn_str[2000];

	sprintf ( rtn_str,"52," );
	send_con_hub ( rtn_str );
}

void guild_list_decog ( char *guild_name )
{
	char rtn_str[2000];

	sprintf ( rtn_str,"50,%s", guild_name );
	send_con_hub ( rtn_str );
}

void send_create_char()
{
	char rtn_str[150] = "2,";

	strcat ( rtn_str,create_char.text_box[0].message );
	strcat ( rtn_str,"," );
	strcat ( rtn_str,create_char.text_box[1].message );
	strcpy ( game.password,create_char.text_box[1].message );
	strcat ( rtn_str,"," );
	strcat ( rtn_str,create_char.text_box[2].message );
	strcat ( rtn_str,"," );
	//strcat(rtn_str,create_char.text_box[3].message);
	//strcat(rtn_str,",");
	strcat ( rtn_str,create_char.text_box[4].message );
	//strcat(rtn_str,",");
	//strcat(rtn_str,create_char.text_box[5].message);
	send_con_hub ( rtn_str );
}

struct textbox *our_ldm_func()
{
	if ( space.visible )
	{
		if ( space.text_box[0].selected )
			return & ( space.text_box[0] );

		if ( space.text_box[1].selected )
			return & ( space.text_box[1] );
	}
	if ( bar.visible )
	{
		if ( bar.text_box[0].selected )
			return & ( bar.text_box[0] );

		if ( bar.text_box[1].selected )
			return & ( bar.text_box[1] );
	}

	return 0;
}

void remove_ldm ( struct textbox *our_box )
{
	int i, len;
	char message[1000];

	strcpy ( message,our_box->message );

	len = strlen ( message );

	for ( i=1;i<22 && i<len;i++ )
		if ( message[i-1] == ':' && message[i] == ':' )
		{
			strcpy ( our_box->message, message + i + 1 );
			our_box->cursor -= i + 1;
			our_box->char_amt -= i + 1;

			if ( our_box->cursor < 0 ) our_box->cursor = 0;
			if ( our_box->char_amt < 0 ) our_box->char_amt = 0;

			return;
		}
}

void insert_ldm ( struct textbox *our_box, char *message )
{
	int len, msg_len;
	char new_message[1000];

	//check 1
	msg_len = strlen ( message );
	if ( !msg_len ) return;

	sprintf ( new_message,"%s::%s",message, our_box->message );

	//check 2
	len = strlen ( new_message );
	if ( len > our_box->max_char )
		new_message[our_box->max_char] = 0;

	strcpy ( our_box->message, new_message );

	our_box->cursor += msg_len + 2;
	our_box->char_amt += msg_len + 2;
}

void ctrl_l()
{
	struct textbox *our_box;

	our_box = our_ldm_func();

	if ( !our_box ) return;

	remove_ldm ( our_box );

	insert_ldm ( our_box, game.last_ldm );

	select_text_box ( screen, our_box );
	sdl_flip_mutex();
}

void ctrl_u()
{
	struct textbox *our_box;

	our_box = our_ldm_func();

	if ( !our_box ) return;

	remove_ldm ( our_box );

	insert_ldm ( our_box, ":u" );

	select_text_box ( screen, our_box );
	sdl_flip_mutex();
}

void ctrl_w()
{
    toggle_windowed();   
}

void ctrl_v()
{
#ifdef _WIN32
    //the windows version of crtl+v
    const int max_size = 2000;
    char data[max_size];
    int i;
    
//    data = clipget(bytes);
    nigh_clipget(data, max_size);
    
    for(i=0; i<strlen(data); i++)
        handle_key(data[i]);
#endif
}

void ctrl_c()
{
	struct textbox *our_box;

	our_box = our_ldm_func();

	if ( !our_box ) return;

	remove_ldm ( our_box );

	insert_ldm ( our_box, ":c" );

	select_text_box ( screen, our_box );
	sdl_flip_mutex();
}

void ctrl_g()
{
	struct textbox *our_box;

	our_box = our_ldm_func();

	if ( !our_box ) return;

	remove_ldm ( our_box );

	insert_ldm ( our_box, ":g" );

	select_text_box ( screen, our_box );
	sdl_flip_mutex();
}

void switch_server_populate()
{
#ifdef USE_LIBCURL
	const int input_size = 5000;
	char input_str[input_size] = "";

	//let em know
	draw_label ( screen, "Downloading server list, please wait...", &switch_server.info_label, 255, 255, 255 );
	sdl_flip_mutex();

	//get it
	fetch_url ( "http://hestia.nighsoft.net/nighsoft/pdias_content/server_list_plain.php", input_str, input_size );
	draw_label ( screen, "Pinging...  (Download Complete)", &switch_server.info_label, 255, 255, 255 );

	//process and save it
	switch_server_insert_array ( input_str );

	//display it
	switch_server.server_page = 0;
	switch_server_display_page();

	sdl_flip_mutex();

	switch_server_ping();
#else
	draw_raw_label ( screen, "Please recompile the client with libcurl...", switch_server.info_label.location, 255, 255, 255 );
	sdl_flip_mutex();
#endif

}

void switch_server_display_page()
{
	int i, j, offset, temp_max;
	float player_high, player_low;
	float ping_high, ping_low;
	float player_dif, ping_dif;

	offset = switch_server.server_page * SWITCH_SERVER_PAGE_DISPLAY;
	temp_max = offset + SWITCH_SERVER_PAGE_DISPLAY;
	if ( temp_max > switch_server.server_max )
		temp_max = switch_server.server_max;

	//find lows and highs
	ping_high = 0;
	player_high = 0;
	for ( i=0; i<switch_server.server_max;i++ )
	{
		if ( switch_server.server_players[i] > player_high )
			player_high = switch_server.server_players[i];

		if ( switch_server.server_ping[i] > ping_high )
			ping_high = switch_server.server_ping[i];
	}

	player_low = player_high;
	ping_low = ping_high;

	for ( i=0; i<switch_server.server_max;i++ )
	{
		if ( switch_server.server_players[i] && switch_server.server_players[i] < player_low )
			player_low = switch_server.server_players[i];

		if ( switch_server.server_ping[i] && switch_server.server_ping[i] < ping_low )
			ping_low = switch_server.server_ping[i];
	}

	player_dif = player_high - player_low;
	ping_dif = ping_high - ping_low;

	for ( i=0, j=offset;j<temp_max; i++, j++ )
	{
		char temp_str[200];

		draw_label ( screen, switch_server.server_name[j], &switch_server.server_name_label[i], 255, 255, 255 );
		draw_label ( screen, switch_server.server_ip[j], &switch_server.server_ip_label[i], 255, 255, 255 );

		sprintf ( temp_str, "%d", switch_server.server_players[j] );
		if ( switch_server.server_players[j] > 0 )
		{
			if ( player_dif > 0.1 )
				draw_label ( screen, temp_str, &switch_server.server_players_label[i], 0, ( int ) ( 150 + 100.0 * ( switch_server.server_players[j] - player_low ) / player_dif ), 0 );
			else
				draw_label ( screen, temp_str, &switch_server.server_players_label[i], 0, 255, 0 );
		}
		else
			draw_label ( screen, temp_str, &switch_server.server_players_label[i], 150, 0, 0 );

		sprintf ( temp_str, "%d", switch_server.server_ping[j] );
		if ( switch_server.server_ping[j] > 0 )
		{
			if ( ping_dif > 0.1 )
				draw_label ( screen, temp_str, &switch_server.server_ping_label[i], 0, ( int ) ( 150 + 100.0 * ( ping_high - switch_server.server_ping[j] ) / ping_dif ), 0 );
			else
				draw_label ( screen, temp_str, &switch_server.server_players_label[i], 0, 255, 0 );
		}
		else
			draw_label ( screen, temp_str, &switch_server.server_ping_label[i], 150, 0, 0 );
	}

	//clear the rest if there be any
	for ( ;i<SWITCH_SERVER_PAGE_DISPLAY; i++ )
	{
		draw_label ( screen, "", &switch_server.server_name_label[i], 255, 255, 255 );
		draw_label ( screen, "", &switch_server.server_ip_label[i], 255, 255, 255 );
	}
}

void switch_server_insert_array ( char *array )
{
	int len, point;
	int i;

	point = 0;
	len = strlen ( array );

	if ( !len ) return;

	for ( i=0; i<SWITCH_SERVER_MAX; i++ )
	{
		char temp_str[10000];
		int temp_int = 0;;
		int j;

		if ( point>=len ) break;

		//get num
		split ( temp_str, array, ' ', &point );
		if ( point>=len ) break;
		temp_int = atoi ( temp_str );

		//place into temp_str
		for ( j=0;j<temp_int && j<49;j++ ) switch_server.server_name[i][j] = array[point + j];
		switch_server.server_name[i][j] = 0;
		point += j + 1;
		if ( point>=len ) break;

		//get num
		split ( temp_str, array, ' ', &point );
		if ( point>=len ) return;
		temp_int = atoi ( temp_str );
		if ( point>=len ) break;

		//place into temp_str
		for ( j=0;j<temp_int && j<49;j++ ) switch_server.server_ip[i][j] = array[point + j];
		switch_server.server_ip[i][j] = 0;
		point += j + 1;
	}

	switch_server.server_max = i;

	for ( i=0; i<switch_server.server_max; i++ )
	{
		switch_server.server_ping[i] = 0;
		switch_server.server_players[i] = 0;
	}
}

void switch_server_left_button()
{
	if ( switch_server.server_page <= 0 ) return;

	switch_server.server_page--;
	switch_server_display_page();
	sdl_flip_mutex();
}

void switch_server_right_button()
{
	//if we are currently viewing the last amount, exit
	if ( SWITCH_SERVER_PAGE_DISPLAY >= switch_server.server_max - ( switch_server.server_page * SWITCH_SERVER_PAGE_DISPLAY ) ) return;

	switch_server.server_page++;
	switch_server_display_page();
	sdl_flip_mutex();
}

void switch_server_select_server ( int i )
{
	int s;
	char rtn_str[500];

	s = switch_server.server_page * SWITCH_SERVER_PAGE_DISPLAY + i;

	if ( s >= switch_server.server_max ) return;

	switch_server.server_selected = s;

	sprintf ( rtn_str, "%s server selected.", switch_server.server_name[s] );

	draw_label ( screen, rtn_str, &switch_server.info_label, 255, 255, 255 );

	sdl_flip_mutex();
}

void switch_server_leave_button()
{
	load_login();

	if ( switch_server.server_selected >= 0 )
	{
		strcpy ( game.hub_address,switch_server.server_ip[switch_server.server_selected] );
		write_host_info();
		close_con_hub();
		//connect_to_hub();
	}
}

void switch_server_ping()
{
	SDL_Thread *thread;

	thread = SDL_CreateThread ( switch_server_ping_thread, NULL );
}

int switch_server_ping_thread ( void *nothing )
{
	double start_time;
	int i;
	int ping_sock[SWITCH_SERVER_MAX];
	struct sockaddr_in c_in[SWITCH_SERVER_MAX];
	double connect_timeout = 1;
	int pings_set = 0;
	int players_set = 0;
	int made_player_after = 0;

#ifdef _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup ( MAKEWORD ( 2,2 ), &wsaData );
	if ( iResult != NO_ERROR )
	{
		draw_label ( screen, "WSAStartup Error.", &switch_server.info_label, 255, 255, 255 );

		sdl_flip_mutex();
		return 0;
	}

#endif


	//make data
	for ( i=0; i<switch_server.server_max && switch_server.visible; i++ )
	{
		struct hostent * host;
		u_long on_mode = 1;
		int ret;


		ping_sock[i] = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		if ( ping_sock[i] < 0 ) continue;

		host = gethostbyname ( switch_server.server_ip[i] );
		memcpy ( ( char* ) &c_in[i].sin_addr, ( char* ) host->h_addr, host->h_length );
		c_in[i].sin_family = AF_INET;
		c_in[i].sin_port = htons ( 4200 );

#ifdef _WIN32
		ret = ioctlsocket ( ping_sock[i], FIONBIO, &on_mode );
		if ( ret == SOCKET_ERROR )
			printf ( "ioctlsocket error on:%d:%d\n", i, WSAGetLastError() );
#else
		ioctl ( ping_sock[i], FIONBIO, &on_mode );
#endif
		//fcntl(ping_sock[i], F_SETFL, O_NONBLOCK);
	}

	start_time = current_time();
	//now do connections
	while ( switch_server.visible && connect_timeout > current_time() - start_time && pings_set != switch_server.server_max )
	{
		for ( i=0; i<switch_server.server_max && switch_server.visible; i++ )
		{
			int response;

			if ( ping_sock[i] < 0 ) continue;
			if ( switch_server.server_ping[i] )
			{
				int rcv_amt;
				char buf[1024];

				if ( switch_server.server_players[i] ) continue;

				rcv_amt = recv ( ping_sock[i], buf, 1023, 0 );

				if ( rcv_amt > 0 )
				{
					char *str_toked;
					buf[rcv_amt] = 0;

					str_toked = strtok ( buf, ":" );

					if ( str_toked )
					{
						str_toked = strtok ( 0, ":" );

						if ( str_toked && str_toked[0] && str_toked[1] )
						{
							switch_server.server_players[i] = atoi ( str_toked + sizeof ( char ) );
							players_set++;
						}
					}
				}
				continue;
			}

			response = connect ( ping_sock[i], ( struct sockaddr * ) &c_in[i], sizeof ( c_in[i] ) );

			//this is stupid
#ifdef _WIN32
			if ( WSAGetLastError() == 10056 )
			{
				switch_server.server_ping[i] = ( int ) ( 1000.0 * ( current_time() - start_time ) );
				pings_set++;
			}
#else
			if ( response >= 0 )
			{
				switch_server.server_ping[i] = ( int ) ( 1000.0 * ( current_time() - start_time ) );
				pings_set++;
			}
#endif
		}

		SDL_Delay ( 5 );
	}

	if ( switch_server.visible )
	{
		draw_label ( screen, "Querying...  (Pinging Complete)", &switch_server.info_label, 255, 255, 255 );
		switch_server_display_page();

		sdl_flip_mutex();
	}

	start_time = current_time();
	//now do connections
	while ( switch_server.visible && connect_timeout > current_time() - start_time && pings_set != players_set )
	{
		for ( i=0; i<switch_server.server_max && switch_server.visible; i++ )
		{
			int rcv_amt;
			char buf[1024];

			if ( ping_sock[i] < 0 ) continue;
			if ( !switch_server.server_ping[i] ) continue;
			if ( switch_server.server_players[i] ) continue;

			rcv_amt = recv ( ping_sock[i], buf, 1023, 0 );

			if ( rcv_amt > 0 )
			{
				char *str_toked;
				buf[rcv_amt] = 0;

				str_toked = strtok ( buf, ":" );

				if ( str_toked )
				{
					str_toked = strtok ( 0, ":" );

					if ( str_toked && str_toked[0] && str_toked[1] )
					{
						switch_server.server_players[i] = atoi ( str_toked + sizeof ( char ) );
						players_set++;
						made_player_after = 1;
					}
				}
			}
		}

		SDL_Delay ( 100 );
	}

	if ( switch_server.visible )
	{
		draw_label ( screen, "Querying Complete.", &switch_server.info_label, 255, 255, 255 );
		if ( made_player_after )
			switch_server_display_page();

		sdl_flip_mutex();
	}

	//close sockets
	for ( i=0; i<switch_server.server_max; i++ )
	{
		if ( ping_sock[i] < 0 ) continue;
		if ( !switch_server.server_ping[i] ) continue;
#ifdef _WIN32
		closesocket ( ping_sock[i] );
#else
		shutdown ( ping_sock[i], SHUT_RDWR );
		close ( ping_sock[i] );
#endif
	}
}

void add_ignore ( char *name )
{
	char added_msg[100] = "... User added to the ignore list ..";
	char not_added_msg[100] = "... User not added to the ignore list ..";

	//some checks
	if ( !name ) return;
	if ( !name[0] ) return;

	if ( add_ignore_raw ( name ) )
		add_chat ( 1, added_msg ); //added
	else
		add_chat ( 1, not_added_msg ); //not added
}

int add_ignore_raw ( char *name )
{
	int i;
	char lname[1000];

	//some checks
	if ( !name ) return 0;
	if ( !name[0] ) return 0;

	//does it already exist?
	if ( check_ignore_raw ( name ) > -1 ) return 0;

	//find a spot then
	for ( i=0;i<MAX_IGNORE && i<ignore_list_max; i++ )
		if ( !ignore_list[i] )
			break;

	//totally full.. ?
	if ( i==MAX_IGNORE ) return 0;

	//add in
	lcase ( lname, name );
	ignore_list[i] = strdup ( lname );

	//push this?
	if ( i==ignore_list_max )
		ignore_list_max++;

	//then save it
	save_ignore();

	return 1;
}

int check_ignore_raw ( char *name )
{
	int i;
	char lname[1000];

	//some checks
	if ( !name ) return -1;
	if ( !name[0] ) return -1;

	//make an lname version
	lcase ( lname, name );

	//find it then
	for ( i=0;i<MAX_IGNORE && i<ignore_list_max; i++ )
		if ( ignore_list[i] && !strcmp ( ignore_list[i], lname ) )
			return i;

	//not found i guess
	return -1;
}

void remove_ignore ( char *name )
{
	int i;
	char removed_msg[100] = "... User removed from the ignore list ..";
	char not_removed_msg[100] = "... User not found in the ignore list ..";

	//some checks
	if ( !name ) return;
	if ( !name[0] ) return;

	//find it..
	i = check_ignore_raw ( name );

	if ( i > -1 && ignore_list[i] )
	{
		free ( ignore_list[i] );
		add_chat ( 1, removed_msg );

		save_ignore();
	}
	else
		add_chat ( 1, not_removed_msg );
}

void list_ignore()
{
	int i;
	char output_str[1000] = "Ignore List : ";

	for ( i=0;i<MAX_IGNORE && i<ignore_list_max; i++ )
		if ( ignore_list[i] && ignore_list[i][0] )
		{
			//append it
			strcat ( output_str, ignore_list[i] );
			strcat ( output_str, ", " );
		}

	//display the list
	add_chat ( 1, output_str );
}

int check_ignore_normal ( int color, char *message )
{
	char found_name[1050];
	int message_size;
	int i;
	int k;

	//don't even check if we aren't ignoring anyone
	if ( !ignore_list_max ) return 0;

	message_size = strlen ( message );

	if ( color == 0 )
	{
		//check for normal chat ignore

		//find the name
		for ( i=0; i<message_size-1; i++ )
		{
			//copy into so far
			found_name[i] = message[i];

			if ( message[i] == ' ' && message[i+1] == ':' )
			{
				//cap
				found_name[i] = 0;

				//printf("ignore check normal:\"%s\"\n", found_name);

				//check
				if ( check_ignore_raw ( found_name ) > -1 )
					return 1;
				else
					return 0;
			}
		}
	}
	else if ( color == 2 )
	{
		if ( message_size < 17 ) return 0;

		//check for universal chat ignore
		i = 0;
		if ( message[i++] != 'U' ) return 0;
		if ( message[i++] != 'n' ) return 0;
		if ( message[i++] != 'i' ) return 0;
		if ( message[i++] != 'v' ) return 0;
		if ( message[i++] != 'e' ) return 0;
		if ( message[i++] != 'r' ) return 0;
		if ( message[i++] != 's' ) return 0;
		if ( message[i++] != 'a' ) return 0;
		if ( message[i++] != 'l' ) return 0;
		if ( message[i++] != ' ' ) return 0;
		if ( message[i++] != 'C' ) return 0;
		if ( message[i++] != 'h' ) return 0;
		if ( message[i++] != 'a' ) return 0;
		if ( message[i++] != 't' ) return 0;
		if ( message[i++] != ' ' ) return 0;
		if ( message[i++] != ':' ) return 0;
		if ( message[i++] != ' ' ) return 0;

		//printf("check_ignore_normal::uni chat found %d\n", i);

		//find the name
		for ( k=0; i<message_size-3; i++, k++ )
		{
			//copy into so far
			found_name[k] = message[i];

			if ( message[i] == ' ' && message[i+1] == ':' && message[i+2] == ' ' && message[i+3] == ':' )
			{
				//cap
				found_name[k] = 0;

				//printf("ignore check normal:\"%s\"\n", found_name);

				//check
				if ( check_ignore_raw ( found_name ) > -1 )
					return 1;
				else
					return 0;
			}
		}
	}

	return 0;
}

int check_ignore_private ( int color, char *message )
{
	char found_name[1050];
	int message_size;
	int i;
	int k;

	//don't even check if we aren't ignoring anyone
	if ( !ignore_list_max ) return 0;

	message_size = strlen ( message );

	if ( color == 2 )
	{
		//look for LDM
		if ( message_size < 6 ) return 0;

		//check for universal chat ignore
		i = 0;
		if ( message[i++] != 'L' ) return 0;
		if ( message[i++] != 'D' ) return 0;
		if ( message[i++] != 'M' ) return 0;
		if ( message[i++] != ' ' ) return 0;
		if ( message[i++] != ':' ) return 0;
		if ( message[i++] != ' ' ) return 0;

		//printf("check_ignore_normal::uni chat found %d\n", i);

		for ( k=0; i<message_size-2; i++, k++ )
		{
			//copy into so far
			found_name[k] = message[i];

			if ( message[i] == ' ' && message[i+1] == ':' && message[i+2] == ':' )
			{
				//cap
				found_name[k] = 0;

				//printf("ignore check normal:\"%s\"\n", found_name);

				//check
				if ( check_ignore_raw ( found_name ) > -1 )
					return 1;
				else
					return 0;
			}
		}
	}

	return 0;
}

void save_ignore()
{
	int i;
	FILE *fp;

	//load it
	fp = fopen ( "ignore_list.txt", "w" );

	if ( !fp )
	{
		printf ( "unable to save ignore_list.txt\n" );
		return;
	}

	//save them
	for ( i=0;i<MAX_IGNORE && i<ignore_list_max; i++ )
		if ( ignore_list[i] && ignore_list[i][0] )
			fprintf ( fp, "%s\n", ignore_list[i] );

	//close it
	fclose ( fp );
}

void load_ignore()
{
	int i;
	FILE *fp;
	const int size = 500;
	char message[500];

	//clear
	clear_ilist();

	//load it
	fp = fopen ( "ignore_list.txt", "r" );

	if ( !fp )
	{
		printf ( "unable to load ignore_list.txt\n" );
		return;
	}

	//fill
	while ( fgets ( message, size, fp ) )
	{
		clean_newline ( message, size );

		//is good?
		if ( !message[0] ) continue;

		//insert
		add_ignore_raw ( message );
	}

	//close
	fclose ( fp );
}

void clear_ilist()
{
	static int first_time = 1;
	int i;

	//clear it
	ignore_list_max = 0;

	if ( first_time )
	{
		first_time = 0;

		for ( i=0;i<MAX_IGNORE;i++ )
			ignore_list[i] = 0;
	}
	else
	{
		for ( i=0;i<MAX_IGNORE;i++ )
			if ( ignore_list[i] )
			{
				free ( ignore_list[i] );
				ignore_list[i] = 0;
			}
	}
}


int add_sos ( char *name )
{
	char added_msg[100] = "... User added to the sos list ..";
	char not_added_msg[100] = "... User not added to the sos list ..";

	//some checks
	if ( !name ) return 0;
	if ( !name[0] ) return 0;

	if ( add_sos_raw ( name ) )
	{
		add_chat ( 1, added_msg ); //added
		return 1;
	}
	else
	{
		add_chat ( 1, not_added_msg ); //not added
		return 0;
	}
}

int add_sos_raw ( char *name )
{
	int i;
	char lname[1000];

	//some checks
	if ( !name ) return 0;
	if ( !name[0] ) return 0;

	//does it already exist?
	if ( check_sos_raw ( name ) > -1 ) return 0;

	//find a spot then
	for ( i=0;i<MAX_SOS && i<sos_list_max; i++ )
		if ( !sos_list[i] )
			break;

	//totally full.. ?
	if ( i==MAX_SOS ) return 0;

	//add in
	lcase ( lname, name );
	sos_list[i] = strdup ( lname );

	//push this?
	if ( i==sos_list_max )
		sos_list_max++;

	//then save it
	save_sos();

	return 1;
}

int check_sos_raw ( char *name )
{
	int i;
	char lname[1000];

	//some checks
	if ( !name ) return -1;
	if ( !name[0] ) return -1;

	//make an lname version
	lcase ( lname, name );

	//find it then
	for ( i=0;i<MAX_SOS && i<sos_list_max; i++ )
		if ( sos_list[i] && !strcmp ( sos_list[i], lname ) )
			return i;

	//not found i guess
	return -1;
}

int remove_sos ( char *name )
{
	int i;
	char removed_msg[100] = "... User removed from the sos list ..";
	char not_removed_msg[100] = "... User not found in the sos list ..";

	//some checks
	if ( !name ) return 0;
	if ( !name[0] ) return 0;

	//find it..
	i = check_sos_raw ( name );

	if ( i > -1 && sos_list[i] )
	{
		free ( sos_list[i] );
		add_chat ( 1, removed_msg );

		save_sos();

		return 1;
	}
	else
	{
		add_chat ( 1, not_removed_msg );
		return 0;
	}
}

void list_sos()
{
	int i;
	char output_str[1000] = "Sos List : ";

	for ( i=0;i<MAX_SOS && i<sos_list_max; i++ )
		if ( sos_list[i] && sos_list[i][0] )
		{
			//append it
			strcat ( output_str, sos_list[i] );
			strcat ( output_str, ", " );
		}

	//display the list
	add_chat ( 1, output_str );
}

void save_sos()
{
	int i;
	FILE *fp;

	//load it
	fp = fopen ( "sos_list.txt", "w" );

	if ( !fp )
	{
		printf ( "unable to save sos_list.txt\n" );
		return;
	}

	//save them
	for ( i=0;i<MAX_SOS && i<sos_list_max; i++ )
		if ( sos_list[i] && sos_list[i][0] )
			fprintf ( fp, "%s\n", sos_list[i] );

	//close it
	fclose ( fp );
}

void load_sos()
{
	int i;
	FILE *fp;
	const int size = 500;
	char message[500];

	//clear
	clear_soslist();

	//load it
	fp = fopen ( "sos_list.txt", "r" );

	if ( !fp )
	{
		printf ( "unable to load sos_list.txt\n" );
		return;
	}

	//fill
	while ( fgets ( message, size, fp ) )
	{
		clean_newline ( message, size );

		//is good?
		if ( !message[0] ) continue;

		//insert
		add_sos_raw ( message );
	}

	//close
	fclose ( fp );
}

void clear_soslist()
{
	static int first_time = 1;
	int i;

	//clear it
	sos_list_max = 0;

	if ( first_time )
	{
		first_time = 0;

		for ( i=0;i<MAX_SOS;i++ )
			sos_list[i] = 0;
	}
	else
	{
		for ( i=0;i<MAX_SOS;i++ )
			if ( sos_list[i] )
			{
				free ( sos_list[i] );
				sos_list[i] = 0;
			}
	}
}

void update_space_user_sos ( char *name, int is_sos )
{
	int i;
	char lname[500];
	SDL_Rect destination, user_spot;

	//name bad?
	if ( !name ) return;
	if ( !name[0] ) return;

	//we in the bar?
	if ( bar.visible ) return;

	//we not in space or at the map?
	if ( !space.visible && !map.visible ) return;

	//first find them in space

	//lcase the name
	lcase ( lname, name );

	//skip the planet sector
	if ( space.at_planet )
		i=1;
	else
		i=0;

	for ( ;i<40;i++ )
	{
		char slname[500];

		//good char?
		if ( space.user[i].ship_id == -1 && !space_user_is_special ( i ) ) continue;
		if ( !space.user[i].username[0] ) continue;

		//our char?
		lcase ( slname, space.user[i].username );
		if ( strcmp ( slname, lname ) ) continue;

		//now check the sos status of
		if ( is_sos != space.user[i].sos )
		{
			space.user[i].sos = is_sos;

			//clear this area
			user_spot = space.button[i];
			user_spot.x -= 97;
			user_spot.y -= 65;
			if ( space.visible )
				SDL_BlitSurface ( space.loc, &user_spot, space.temp_loc, &user_spot );

			//i guess this is all we need
			draw_space_user_ship ( i );

			//put loc on backround and fresh screen
			destination.x = 97;
			destination.y = 65;

			//how add_to_sector does it, doesn't seem to be the best way...
			if ( space.visible )
			{
				SDL_BlitSurface ( space.temp_loc, NULL, screen, &destination );
				sdl_flip_mutex();
			}

			break;
		}
	}
}

void show_enemy_missile ( char *name )
{
	int i, k;
	char lname[500];
	const double min_fire_time = 2;
	const double random_additional_time = 1;

	if ( !name ) return;
	if ( !name[0] ) return;

	//cap name
	name[20] = 0;

	//find that user
	lcase ( lname, name );

	//skip the planet sector
	if ( space.at_planet )
		i=1;
	else
		i=0;

	for ( ;i<40;i++ )
	{
		char slname[500];
		int v_u;

		//good char?
		if ( space.user[i].ship_id == -1 && !space_user_is_special ( i ) ) continue;
		if ( !space.user[i].username[0] ) continue;

		//our char?
		lcase ( slname, space.user[i].username );
		if ( strcmp ( slname, lname ) ) continue;

		v_u = space.user[i].attacking;

		//they attacking anyone?
		if ( v_u == -1 ) return;

		//is their target good?
		if ( v_u > -1 )
		{
			if ( space.user[v_u].ship_id == -1 && !space_user_is_special ( v_u ) ) return;
			if ( !space.user[v_u].username[0] ) return;
		}


		//add a missile firing
		//find a free missile slot
		for ( k=0;k<16;k++ )
			if ( !space.user[i].missile_firing[k] )
			{
				//set it's location
				if ( v_u == -2 )
				{
					//what side of the loc is it going to
					switch ( rand() % 4 )
					{
						case 0: //left side
							space.user[i].missile_x[k] = 7;
							space.user[i].missile_y[k] = ( rand() % 190 ) + 5;
							break;
						case 1: //right side
							space.user[i].missile_x[k] = 593;
							space.user[i].missile_y[k] = ( rand() % 190 ) + 5;
							break;
						case 2: //top side
							space.user[i].missile_x[k] = ( rand() % 590 ) + 5;
							space.user[i].missile_y[k] = 7;
							break;
						case 3: //bottom side
							space.user[i].missile_x[k] = ( rand() % 590 ) + 5;
							space.user[i].missile_y[k] = 150;
							break;
					}

					//shift because it was previous set in relation to the loc
					space.user[i].missile_x[k] += 97;
					space.user[i].missile_y[k] += 65;
				}
				else
				{
					//what part of their area is it going to
					space.user[i].missile_x[k] = space.button[v_u].x + 15 + ( rand() % 30 );
					space.user[i].missile_y[k] = space.button[v_u].y + 12 + ( rand() % 25 );
				}

				//get it rolling
				space.user[i].missile_speed[k] = min_fire_time + ( random_additional_time * random_chance() );
				space.user[i].missile_time[k] = current_time();
				space.user[i].missile_firing[k] = 1;
				break;
			}
	}
}

void draw_space_good_buttons()
{
	if(!space.visible) return;
	
	if ( space.draw_goods_grey )
		SDL_BlitSurface ( space.trade_goods_grey_img, NULL, screen, &space.button[49] );
	else
		SDL_BlitSurface ( space.trade_goods_img, NULL, screen, &space.button[49] );

	if ( space.draw_contra_grey )
		SDL_BlitSurface ( space.contraband_grey_img, NULL, screen, &space.button[50] );
	else
		SDL_BlitSurface ( space.contraband_img, NULL, screen, &space.button[50] );
}

int update_space_good_buttons()
{
	int z, s, s_i;
	int i;
	int found_goods, found_contra;
	int return_change;

	s_i = game.server_id;
	z = game.zone;
	s = game.sector;

	//check for fucking around
	if ( s < 0 ) return 0;
	if ( s >= SECTOR_MAX ) return 0;
	if ( z < 0 ) return 0;
	if ( z >= ZONE_MAX ) return 0;
	if ( s_i < 0 ) return 0;
	if ( s_i >= MAX_SERVER ) return 0;

	found_goods = 0;
	found_contra = 0;

	for ( i=0;i<10;i++ )
	{
		if ( server[s_i].zone[z].sector[s].good_amount[i] ) found_goods = 1;
		if ( server[s_i].zone[z].sector[s].contra_amount[i] ) found_contra = 1;
	}

	return_change = 0;

	if ( space.draw_goods_grey == found_goods ) return_change = 1;
	if ( space.draw_contra_grey == found_contra ) return_change = 1;

	space.draw_goods_grey = !found_goods;
	space.draw_contra_grey = !found_contra;

	return return_change;
}

void draw_purchase_plans()
{
	char temp_str[500];
	int s, p, the_plan;
	SDL_Rect destination;
	
	s = game.server_id;
	p = planet.last_planet;
	the_plan = server[s].planet[p].ship_plan;
	
	//the requirements
	sprintf(temp_str, "%d", server[s].planet[p].ship_plan_cost);
	draw_raw_right_label ( screen, temp_str, purchase_plans.cost_label.location, 255, 255, 255 );
	sprintf(temp_str, "%d", server[s].planet[p].ship_plan_exp);
	draw_raw_right_label ( screen, temp_str, purchase_plans.exp_label.location, 255, 255, 255 );
	sprintf(temp_str, "%d", server[s].planet[p].ship_plan_kills);
	draw_raw_right_label ( screen, temp_str, purchase_plans.kills_label.location, 255, 255, 255 );
	
	//the buy button
	if(!purchase_plans_should_show_buy_button(s,p,the_plan))
		SDL_FillRect(screen, &purchase_plans.button[1], SDL_MapRGB(screen->format, 0, 0, 0));
	
	//the graphic
	if(!server[s].ship[the_plan].plan_img)
	{
		sprintf ( temp_str, "server/%s/backs/prints/%s.jpg", server[s].servername, server[s].ship[the_plan].name);
		server[s].ship[the_plan].plan_img = IMG_Load ( temp_str );
	}
	
	if(server[s].ship[the_plan].plan_img)
	{
		destination.x = 387;
		destination.y = 218;
		SDL_BlitSurface ( server[s].ship[the_plan].plan_img, NULL, screen, &destination );
	}
}

void do_purchase_plan()
{
	int s, p, the_plan;
	char send_str[100];
	
	s = game.server_id;
	p = planet.last_planet;
	the_plan = server[s].planet[p].ship_plan;
	
	if(!purchase_plans_should_show_buy_button(s,p,the_plan))
		return;
	
	//do the send
	sprintf(send_str, "62,%d", the_plan);
	send_con_hub(send_str);
}

int purchase_plans_should_show_buy_button(int s, int p, int pl)
{
	if(game.has_plan[s][pl])
		return 0;
	
	if(server[s].planet[p].ship_plan_cost && game.money < server[s].planet[p].ship_plan_cost)
		return 0;
	
	if(server[s].planet[p].ship_plan_exp && game.exp < server[s].planet[p].ship_plan_exp)
		return 0;
	
	if(server[s].planet[p].ship_plan_kills && game.kills < server[s].planet[p].ship_plan_kills)
		return 0;
	
	return 1;
}

void clear_plan_list()
{
	int s, pl;
	
	for(s=0;s<MAX_SERVER;s++)
		for(pl=0;pl<SHIP_MAX;pl++)
			game.has_plan[s][pl] = 0;
}

int at_home_planet()
{
	return ( game.home_id == game.server_id && game.home_planet == planet.last_planet );
}

void space_threadless()
{
	static int jump_set = 0, redraw, gun_x, gun_y, i, k;
	static int hit_on, hit_x[16], hit_y[16];
	static double interval_time, gun_time = 0, temp_time, temp_ratio, hit_time;
	static SDL_Rect jump_fill, user_spot, destination, odamage_box, idamage_box;
	const double hit_time_max = 0.5;
	static double the_time = 0;
	static int reset_backround = 0;

	if(!space.visible && !map.visible) 
	{
		the_time = 0;
		return;
	}
	
	destination.x = 97;
	destination.y = 65;

	jump_fill.x = 97;
	jump_fill.w = 24;

	odamage_box.w = 60;
	odamage_box.h = 4;

	idamage_box.h = 2;

	if(the_time > -0.00001 && the_time < 0.00001)
		the_time = current_time();

	//save old time
	interval_time = current_time() - the_time;
	
	//sync up with the old thread version
	if(interval_time < 0.1) return;
	
	//setup "time"
	the_time = current_time();

	//reset backround if requested
	if ( reset_backround )
	{
		if ( space.visible )
			SDL_BlitSurface ( space.temp_loc, NULL, screen, &destination );
		reset_backround = 0;
		redraw = 1;
	}

	//jump
	if ( the_time - space.jump_time_start < space.jump_time )
	{
		//how about now? eh? EH
		if ( space.clear_jump_bar )
		{
			jump_fill.h = 60;
			jump_fill.y = 316;
			if ( space.visible )
				SDL_FillRect ( screen, &jump_fill, SDL_MapRGB ( screen->format, 0, 0, 0 ) );

			space.clear_jump_bar = 0;
		}
		space.jump_time_start += interval_time * 0.25 * space.shield_level;
		jump_fill.h = ( short unsigned int ) ( 60 * ( ( the_time - space.jump_time_start ) / space.jump_time ) );
		jump_fill.y = 376 - jump_fill.h;
		if ( space.visible )
			SDL_FillRect ( screen, &jump_fill, SDL_MapRGB ( screen->format, 190, 26, 7 ) );
		redraw = 1;
	}
	else if ( !space.jump_ok ) //draw jump button
	{
		space.jump_ok = 1;
		jump_fill.h = 60;
		jump_fill.y = 316;
		if ( space.visible )
		{
			SDL_FillRect ( screen, &jump_fill, SDL_MapRGB ( screen->format, 190, 26, 7 ) );
			SDL_BlitSurface ( space.jump_img, NULL, screen, &space.button[51] );
		}
		redraw = 1;
	}

	//selection box
	if ( space.user_sel != -1 )
	{
		//increment
		space.selection_box_i++;
		if ( space.selection_box_i > 4 )
			space.selection_box_i = 0;

		//clear this area
		user_spot = space.button[space.user_sel];
		user_spot.x -= 97;
		user_spot.y -= 65;
		user_spot.w = 60;
		user_spot.h = 50;
		if ( space.visible )
			SDL_BlitSurface ( space.temp_loc, &user_spot, screen, &space.button[space.user_sel] );

		//draw the box
		if ( space.visible )
			rectangleRGBA ( screen, space.button[space.user_sel].x + space.selection_box_i, space.button[space.user_sel].y + space.selection_box_i, ( space.button[space.user_sel].x + 59 ) - ( space.selection_box_i * 2 ), ( space.button[space.user_sel].y + 49 ) - ( space.selection_box_i * 2 ), 0, 220, 240, 255 );

		redraw = 1;
	}

	//damage box
	if ( space.engaging > -1 )
	{
		odamage_box.x = space.button[space.engaging].x;
		odamage_box.y = space.button[space.engaging].y;
		idamage_box.x = odamage_box.x + 1;
		idamage_box.y = odamage_box.y + 1;
		idamage_box.w = ( short unsigned int ) ( 58 * space.user[space.engaging].hull_percent * 0.01 );

		//draw outter
		if ( space.visible )
			SDL_FillRect ( screen, &odamage_box, SDL_MapRGB ( screen->format, 0, 73, 70 ) );

		//draw inner
		if ( space.user[space.engaging].hull_percent > 50 )
		{
			if ( space.visible )
				SDL_FillRect ( screen, &idamage_box, SDL_MapRGB ( screen->format, ( unsigned char ) ( 510 * ( 1 - space.user[space.engaging].hull_percent * 0.01 ) ), 0,255 ) );
		}
		else
		{
			if ( space.visible )
				SDL_FillRect ( screen, &idamage_box, SDL_MapRGB ( screen->format, 255, 0, ( unsigned char ) ( 510 * space.user[space.engaging].hull_percent * 0.01 ) ) );
		}

		//now for shields
		space_draw_enemy_shield();

		//now for jump status
		space_draw_enemy_jump_status();

		redraw = 1;
	}

	//starting circle stuff
	if ( space.engaging > -1 && space.hit_enemy )
	{
		hit_on = space.hit_max;

		//set time
		hit_time = the_time;

		//set locs
		for ( i=0;i<space.hit_actual_max;i++ )
		{
			//set centers
			hit_x[i] = space.button[space.engaging].x + 30;
			hit_y[i] = space.button[space.engaging].y + 25;

			//set offsets
			hit_x[i] += ( rand() % 20 ) - 10;
			hit_y[i] += ( rand() % 24 ) - 12;
		}

		space.hit_enemy = 0;

		//force fire
		gun_time = 0;
	}
	//drawing circle stuff
	if ( the_time - hit_time < hit_time_max && space.visible )
	{
		double percent_done;

		percent_done = ( the_time - hit_time ) / hit_time_max;

		//draw
		for ( i=0;i<space.hit_actual_max;i++ )
			if ( i<hit_on )
		{
			for ( k= ( int ) ( ( 5 * percent_done ) + 1 );k<6;k++ )
				circleRGBA ( screen,hit_x[i],hit_y[i],k,255 - ( k* 20 ),0,0,150 );
		}
		else
		{
			for ( k= ( int ) ( ( 5 * percent_done ) + 1 );k<6;k++ )
				circleRGBA ( screen,hit_x[i],hit_y[i],k,0,255 - ( k* 20 ),0,150 );
		}

		reset_backround = 1;
		redraw = 1;
	}

	//destroy user
	for ( i=0;i<40;i++ )
		if ( space.user[i].destroy )
	{
			//clear that bit
		user_spot = space.button[i];
		user_spot.x -= 97;
		user_spot.y -= 65;
		if ( space.visible )
			SDL_BlitSurface ( space.loc, &user_spot, screen,  &space.button[i] );

		k = ( int ) ( 51.0 * ( ( the_time - space.user[i].destroy_time ) / 4.0 ) );

		if ( k<52 ) //display
		{
			if ( space.visible )
				SDL_BlitSurface ( space.destroy_img[k], NULL, screen, &space.button[i] );
		}
		else
		{
				//stuff stolen from remove_from_sector()
			space.user[i].ship_id 		= -1;
			space.user[i].ship_kind 	= -1;
			space.user[i].ship_type 	= -1;
			space.user[i].ship_name[0] 	= '\0';
			space.user[i].username[0] 	= '\0';
			space.user[i].guild[0] 		= '\0';
			space.user[i].destroy 		= 0;
			space.user[i].destroy_time	= 0;

				//stuff stolen from remove_from_sector()
			SDL_BlitSurface ( space.loc, &user_spot, space.temp_loc, &user_spot );
		}

		redraw = 1;
	}

	//enemy guns
	for ( i=0;i<40;i++ )
		if ( space.user[i].attacking > -1 )
	{
		int &v_u = space.user[i].attacking;

			//attacking monkey space?
		if ( !space.user[v_u].username[0] )
		{
			space.user[i].attacking = 0;
			for ( k=0;k<16;k++ )
				space.user[i].missile_firing[k] = 0;
			continue;
		}

			//do missiles
		for ( k=0;k<16;k++ )
			if ( space.user[i].missile_firing[k] )
		{
					//should it be?
			temp_ratio = ( the_time - space.user[i].missile_time[k] ) / space.user[i].missile_speed[k];

			if ( temp_ratio >= 1 )
			{
				space.user[i].missile_firing[k] = 0;
				continue;
			}

			if ( space.visible )
				space_draw_enemy_missile ( i, k, temp_ratio );

			reset_backround = 1;
			redraw = 1;
		}

			//do guns
		temp_time = the_time - space.user[i].attack_time;
		if ( temp_time < 3 ) continue;

		if ( !space.user[i].attack_x )
		{
			space.user[i].attack_x = space.button[v_u].x + 15 + ( rand() % 30 );
			space.user[i].attack_y = space.button[v_u].y + 12 + ( rand() % 25 );

			play_sound ( sound_enemy_gun[rand() % 2] );
		}

		if ( space.visible )
			space_draw_line_to_user ( i );

		if ( temp_time > 3.5 )
		{
			space.user[i].attack_time = the_time;

			space.user[i].attack_x = 0;
			space.user[i].attack_y = 0;
		}

		reset_backround = 1;
		redraw = 1;
	}
	else if ( space.user[i].attacking == -2 ) //attacking you
	{
			//do missiles
		for ( k=0;k<16;k++ )
			if ( space.user[i].missile_firing[k] )
		{
					//should it be?
			temp_ratio = ( the_time - space.user[i].missile_time[k] ) / space.user[i].missile_speed[k];

			if ( temp_ratio >= 1 )
			{
				space.user[i].missile_firing[k] = 0;
				continue;
			}

			if ( space.visible )
				space_draw_enemy_missile ( i, k, temp_ratio );

			reset_backround = 1;
			redraw = 1;
		}

			//do guns
		temp_time = the_time - space.user[i].attack_time;
		if ( temp_time < 3 ) continue;


		if ( !space.user[i].attack_position )
		{
			space.user[i].attack_side = ( rand() % 4 );
			if ( space.user[i].attack_side == 0 || space.user[i].attack_side == 1 )
				space.user[i].attack_position = ( rand() % 190 ) + 5;
			else
				space.user[i].attack_position = ( rand() % 590 ) + 5;

			play_sound ( sound_enemy_gun[rand() % 2] );
		}

		if ( space.visible )
			space_draw_line ( space.user[i].attack_position, space.user[i].attack_side, space.button[i].x + 30, space.button[i].y + 25 );

		if ( temp_time > 3.5 )
		{
			space.user[i].attack_time = the_time;

			space.user[i].attack_position = 0;
		}

		reset_backround = 1;
		redraw = 1;
	}


	//engage guns
	if ( space.engaging > -1 )
	{
		gun_time -= interval_time;

		if ( gun_time <= 0 )
		{
			gun_x = 3 + ( rand() % 30 );
			gun_y = 8 + ( rand() % 25 );
			gun_time = game.ship[game.ship_sel].gun_speed + 1; //set new interval

			play_sound ( sound_gun );
		}

		if ( game.ship[game.ship_sel].gun_speed - gun_time < -0.75 )
			if ( space.visible )
		{
			space_draw_gun_lines ( gun_x, gun_y ); //draw all lines
			reset_backround = 1;
		}

		if ( game.ship[game.ship_sel].gun_speed - gun_time >= -0.75 && ( game.ship[game.ship_sel].gun_speed - gun_time ) - interval_time < -0.75 )
		{
			if ( space.visible )
				space_draw_gun_lines ( gun_x, gun_y ); //draw all lines
			reset_backround = 1;
			redraw = 1;
		}

	}

	//engage missiles
	if ( space.engaging > -1 )
		for ( i=0;i<16;i++ )
			if ( space.missile_firing[i] )
	{
		temp_ratio = ( the_time - space.missile_time[i] ) / space.missile_speed[i];

		if ( temp_ratio >= 1 )
		{
			space.missile_firing[i] = 0;
			continue;
		}

		if ( space.visible )
			space_draw_missile ( i, temp_ratio );
		reset_backround = 1;
		redraw = 1;
	}


	//good selection the life made easy way
	if ( space_good_select_animate() )
		redraw = 1;

	//redraw screen if required
	if ( redraw && space.visible )
		sdl_flip_mutex();

}
