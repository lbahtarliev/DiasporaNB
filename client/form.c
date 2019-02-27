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

void set_forms_invisible();
void key_login ( int c );
void key_create_char ( int c );
void key_bar ( int c );
void key_space ( int c );
void key_map ( int c );
void key_trade ( int c );
void key_guild_editor ( int c );
void key_guild_editor_more ( int c );
void key_guild_listing ( int c );
void key_guild_register ( int c );
void key_guild_bank ( int c );
void key_stat_viewer ( int c );
void key_bounty ( int c );
void key_sat_editor ( int c );
void load_create_char();
void load_leave_game();
void load_hanger();
void load_shop();
void load_bar();
void load_repair();
void load_landing();
void load_launching();
void load_map();
void load_trade();
void load_guild_halls();
void load_guild_editor();
void load_guild_editor_more();
void load_guild_listing();
void load_guild_bank();
void load_guild_register();
void load_stat_viewer();
void load_bounty();
void load_cb_info();
void load_planet_sat();
void load_planet_editor();
void load_sat_editor();
void load_planet_guild();
void load_switch_server();
void load_purchase_plans();
void button_switch_server ( int x, int y );
void button_planet_guild ( int x, int y );
void button_login ( int x, int y );
void button_create_char ( int x, int y );
void button_leave_game ( int x, int y );
void button_server_select ( int x, int y );
void button_planet ( int x, int y );
void button_planet_sat ( int x, int y );
void button_gen_home ( int x, int y );
void button_hanger ( int x, int y );
void button_shop ( int x, int y );
void button_bar ( int x, int y );
void button_repair ( int x, int y );
void button_space ( int x, int y );
void button_right_space ( int x, int y );
void button_map ( int x, int y );
void button_up_map ( int x, int y );
void button_death ( int x, int y );
void button_trade ( int x, int y );
void button_guild_halls ( int x, int y );
void button_guild_editor ( int x, int y );
void button_guild_editor_more ( int x, int y );
void button_guild_listing ( int x, int y );
void button_guild_bank ( int x, int y );
void button_guild_register ( int x, int y );
void button_stat_viewer ( int x, int y );
void button_bounty ( int x, int y );
void button_cb_info ( int x, int y );
void motion_space ( int x, int y );

void redraw_hanger();
void select_shop_sell_pod ( int pod_number );
int rotate_shop_buy_pic ( void *nothing );
int space_thread ( void *nothing );
int repair_thread ( void *nothing );
int stat_viewer_thread ( void *nothing );

//Memory
struct login login;
struct create_char create_char;
struct leave_game leave_game;
struct select_server server_select;
struct planet planet;
struct gen_home gen_home;
struct velic_home velic_home;
struct hanger hanger;
struct shop shop;
struct bar bar;
struct repair repair;
struct launching launching;
struct landing landing;
struct space space;
struct map map;
struct death death;
struct trade trade;
struct guild_halls guild_halls;
struct guild_editor guild_editor;
struct guild_editor_more guild_editor_more;
struct guild_listing guild_listing;
struct guild_bank guild_bank;
struct guild_register guild_register;
struct splash_screen splash_screen;
struct stat_viewer stat_viewer;
struct cb_info cb_info;
struct bounty bounty;
struct planet_sat planet_sat;
struct sat_editor sat_editor;
struct planet_editor planet_editor;
struct planet_guild planet_guild;
struct switch_server switch_server;
struct purchase_plans purchase_plans;

void set_forms_invisible()
{
	login.visible 		= 0;
	create_char.visible 	= 0;
	leave_game.visible 	= 0;
	server_select.visible	= 0;
	planet.visible 		= 0;
	gen_home.visible 	= 0;
	velic_home.visible 	= 0;
	hanger.visible 		= 0;
	shop.visible		= 0;
	bar.visible		= 0;
	repair.visible		= 0;
	launching.visible	= 0;
	landing.visible		= 0;
	space.visible		= 0;
	map.visible		= 0;
	death.visible		= 0;
	trade.visible		= 0;
	guild_halls.visible	= 0;
	guild_editor.visible	= 0;
	guild_editor_more.visible = 0;
	guild_listing.visible	= 0;
	guild_bank.visible	= 0;
	guild_register.visible	= 0;
	splash_screen.visible	= 0;
	stat_viewer.visible	= 0;
	cb_info.visible		= 0;
	bounty.visible		= 0;
	planet_sat.visible  	= 0;
	sat_editor.visible 	= 0;
	planet_editor.visible 	= 0;
	planet_guild.visible 	= 0;
	switch_server.visible 	= 0;
	purchase_plans.visible	= 0;
}

void load_purchase_plans()
{
	SDL_Rect destination;
	
	
	if ( !purchase_plans.backround )
		purchase_plans.backround = IMG_Load ( "graphics/purchase_plans.jpg" );
	
	destination.x = 400 - ( 480 / 2 );
	destination.y = 300 - ( 360 / 2 );
	
	set_forms_invisible();
	purchase_plans.visible = 1;
	SDL_BlitSurface ( purchase_plans.backround, NULL, screen, &destination );
	
	//screen only stuff
	draw_purchase_plans();
	
	//flip
	sdl_flip_mutex();
}

void button_purchase_plans ( int x, int y )
{
	int i;
	const int button_max = 2;

	for ( i=0;i<button_max;i++ )
		if ( x >= purchase_plans.button[i].x && x <= purchase_plans.button[i].x + purchase_plans.button[i].w && y >= purchase_plans.button[i].y && y <= purchase_plans.button[i].y + purchase_plans.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0: //exit
			load_planet ( planet.last_planet );
			break;
		case 1: //buy
			do_purchase_plan();
			break;
		default:
			break;
	}
}

void load_switch_server()
{
	SDL_Rect destination;

	if ( !switch_server.backround )
		switch_server.backround = IMG_Load ( "graphics/switchserver.jpg" );

	destination.x = 400 - ( 480 / 2 );
	destination.y = 300 - ( 360 / 2 );

	//must be set
	switch_server.server_selected = -1;

	set_forms_invisible();
	switch_server.visible = 1;
	SDL_BlitSurface ( switch_server.backround, NULL, screen, &destination );

	sdl_flip_mutex();

	//now begin list populating
	switch_server_populate();
}

void button_switch_server ( int x, int y )
{
	int i;
	const int button_max = 3 + SWITCH_SERVER_PAGE_DISPLAY;

	for ( i=0;i<button_max;i++ )
		if ( x >= switch_server.button[i].x && x <= switch_server.button[i].x + switch_server.button[i].w && y >= switch_server.button[i].y && y <= switch_server.button[i].y + switch_server.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0: //leave
			switch_server_leave_button();
			break;
		case 1: //left
			switch_server_left_button();
			break;
		case 2: //right
			switch_server_right_button();
			break;
		default:
			switch_server_select_server ( i - 3 );
			break;
	}
}

void load_planet_guild()
{
	SDL_Rect destination;
	char temp_str[50];

	if ( !planet_guild.backround )
		planet_guild.backround = IMG_Load ( "graphics/planet_guild.jpg" );

	destination.x = 400 - 150;
	destination.y = 300 - ( 225 / 2 );

	set_forms_invisible();
	planet_guild.visible = 1;
	SDL_BlitSurface ( planet_guild.backround, NULL, screen, &destination );

	//screen only stuff

	//draw guild name
	draw_right_label ( screen, server[game.server_id].planet[planet.last_planet].guild, &planet_guild.guild_name_label, 107, 216, 210 );

	//draw guild cost
	sprintf ( temp_str,"%d", 15000 );
	draw_right_label ( screen, temp_str, &planet_guild.entry_cost_label, 107, 216, 210 );

	//draw user money
	sprintf ( temp_str,"%d", game.money );
	draw_right_label ( screen, temp_str, &planet_guild.money_label, 107, 216, 210 );

	//draw guild name
	draw_right_label ( screen, game.guild.name, &planet_guild.your_guild_name_label, 107, 216, 210 );

	sdl_flip_mutex();
}

void button_planet_guild ( int x, int y )
{
	int i;
	const int button_max = 2;

	for ( i=0;i<button_max;i++ )
		if ( x >= planet_guild.button[i].x && x <= planet_guild.button[i].x + planet_guild.button[i].w && y >= planet_guild.button[i].y && y <= planet_guild.button[i].y + planet_guild.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0://exit
			load_planet ( planet.last_planet );
			break;
		case 1:// enter guild
			send_hub_enter_planet_guild();
			break;
	}
}

void load_bounty()
{
	char temp_str[50];



	if ( !bounty.backround )
	{
		bounty.backround = IMG_Load ( "graphics/bountyback.jpg" );
		bounty.bounty_text_img = IMG_Load ( "graphics/bounty_text.jpg" );
	}

	set_forms_invisible();
	bounty.visible = 1;
	SDL_BlitSurface ( bounty.backround, NULL, screen, NULL );

	//screen only stuff

	//draw money
	sprintf ( temp_str,"%d", game.money );
	draw_right_label ( screen, temp_str, &bounty.money_label, 107, 216, 210 );

	//clear stuff
	bounty_clear_all_text_boxes();

	//draw lists
	bounty_draw_raw_lists();

	//draw status label
	draw_center_label ( screen, bounty.status_label.message, &bounty.status_label, 107, 216, 210 );

	//draw our two text boxes
	if ( bounty.text_box[38].selected )
		select_text_box ( screen, &bounty.text_box[38] );
	else
		unselect_text_box ( screen, &bounty.text_box[38] );

	if ( bounty.text_box[39].selected )
		select_text_box ( screen, &bounty.text_box[39] );
	else
		unselect_text_box ( screen, &bounty.text_box[39] );

	sdl_flip_mutex();

	//now ask for the damn list
	bounty_request();
}

void button_bounty ( int x, int y )
{
	const int button_max = 2, text_box_max = 40;
	char rtn_str[10];
	int i, success = 0, button;

	for ( i=0;i<button_max;i++ )
		if ( x >= bounty.button[i].x && x <= bounty.button[i].x + bounty.button[i].w && y >= bounty.button[i].y && y <= bounty.button[i].y + bounty.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<text_box_max;i++ )
			if ( x >= bounty.text_box[i].location.x && x <= bounty.text_box[i].location.x + bounty.text_box[i].location.w && y >= bounty.text_box[i].location.y && y <= bounty.text_box[i].location.y + bounty.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
	{
		switch ( i )
		{
			case 0: //leave
				if ( game.home_id == game.server_id && game.home_planet == planet.last_planet )
				{
					switch ( server[game.server_id].planet[planet.last_planet].home_class )
					{
						default:
						case 1:
							load_gen_home();
							break;
						case 2:
							load_velic_home();
							break;
					}
				}
				else //not home planet
				{
					load_cb_info();
				}
				break;
			case 1: //submit
				bounty_add ( bounty.text_box[38].message,atoi ( bounty.text_box[39].message ) );
				break;
		}
	}
	else
	{
		int k;

		if ( i>=bounty.list_max  && i < text_box_max - 2 ) return;

		for ( k=0;k<text_box_max;k++ )
			if ( bounty.text_box[k].selected )
				unselect_text_box ( screen, &bounty.text_box[k] );

		select_text_box ( screen, &bounty.text_box[i] );

		sdl_flip_mutex();
	}
}

void key_bounty ( int c )
{
	int i;

	if ( c == 9 ) //tab
	{
		for ( i=0;i<40;i++ )
			if ( bounty.text_box[i].selected ) break;

		unselect_text_box ( screen, &bounty.text_box[i] );

		if ( i>=39 )
			{i=0;}
		else
			{i++;}

		if ( i>=bounty.list_max && i<38 ) i = 38;


		select_text_box ( screen, &bounty.text_box[i] );
		sdl_flip_mutex();
	}
	else if ( c == 13 ) //enter
	{
		for ( i=0;i<40;i++ )
			if ( bounty.text_box[i].selected ) break;

		if ( i==40 ) return;

		if ( i==38 || i==39 )
		{
			bounty_add ( bounty.text_box[38].message,atoi ( bounty.text_box[39].message ) );
		}
		else
		{
			bounty_add ( bounty.name_list_label[i].message,atoi ( bounty.text_box[i].message ) );
			clear_text_box ( screen, &bounty.text_box[i] );
		}
	}
	else
	{
		for ( i=0;i<40;i++ )
			if ( bounty.text_box[i].selected )
			{
				//never know
				if ( i>=bounty.list_max && i<38 ) continue;

				if ( i != 38 && ( ! ( c >= '0' && c <= '9' ) && ! ( c == 8 || c == 127 || c == 275 || c == 276 ) ) ) continue;

				add_text_box ( screen, &bounty.text_box[i], c );
				sdl_flip_mutex();
			}
	}

}

void load_cb_info()
{
	if ( !cb_info.backround )
		cb_info.backround = IMG_Load ( "graphics/cbback.jpg" );

	set_forms_invisible();
	cb_info.visible = 1;
	SDL_BlitSurface ( cb_info.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void button_cb_info ( int x, int y )
{
	int i;
	const int button_max = 3;
	char rtn_str[10];

	for ( i=0;i<button_max;i++ )
		if ( x >= cb_info.button[i].x && x <= cb_info.button[i].x + cb_info.button[i].w && y >= cb_info.button[i].y && y <= cb_info.button[i].y + cb_info.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0: //leave
			load_planet ( planet.last_planet );
			break;
		case 1: //stat viewer
			load_stat_viewer();
			break;
		case 2: //bounty
			load_bounty();
			break;
	}
}

void load_stat_viewer()
{
	if ( !stat_viewer.backround )
	{
		stat_viewer.backround = IMG_Load ( "graphics/statback.jpg" );
		stat_viewer.class_img[0] = IMG_Load ( "graphics/velic_stat.jpg" );
		stat_viewer.class_img[1] = IMG_Load ( "graphics/gen_stat.jpg" );
		stat_viewer.pointer_img = IMG_Load ( "graphics/pointer.gif" );
		stat_viewer.sel_left = IMG_Load ( "graphics/seldown.jpg" );
		stat_viewer.sel_right = IMG_Load ( "graphics/selup.jpg" );
	}

	set_forms_invisible();
	stat_viewer.visible = 1;
	SDL_BlitSurface ( stat_viewer.backround, NULL, screen, NULL );

	//screen stuff only

	//draw textbox
	select_text_box ( screen, &stat_viewer.text_box );

	//draw stats
	stat_viewer_display_list();

	sdl_flip_mutex();

	//start rotation thread
	if ( !stat_viewer.thread )
		stat_viewer.thread = SDL_CreateThread ( stat_viewer_thread, NULL );
}

int stat_viewer_thread ( void *nothing )
{
	SDL_Rect destination;
	char temp_str[21], temp_filename[512];
	int i = 0;
	int s_id = -1, s_type = -1, s_kind = -1;

	destination.x = 85;
	destination.y = 335;

	while ( stat_viewer.visible )
	{
		//we loading shit?
		if ( stat_viewer.load_ship )
		{
			s_id = stat_viewer.s_id;
			s_type = stat_viewer.s_type;
			s_kind = stat_viewer.s_kind;
			stat_viewer.load_ship = 0;

			//set some stuff...
			stat_viewer_clear_ship_area();
			i=0;
		}

		//pissing on my face?
		if ( s_id < 0 ) continue;
		if ( s_type < 0 ) continue;
		if ( s_kind < 0 ) continue;

		//check if we need to load
		if ( !server[s_id].ship[s_type].ship_img[s_kind][i] )
		{
			sprintf ( temp_filename, "server/%s/shop/ships/%s/%s%d.JPG", server[s_id].servername, ship_name ( s_kind, temp_str ), server[s_id].ship[s_type].name, i + 1 );
			server[s_id].ship[s_type].ship_img[s_kind][i] = IMG_Load ( temp_filename );
		}

		//if no starting index then display the "no ship" img
		if ( !server[s_id].ship[s_type].ship_img[s_kind][0] )
		{
			if ( !shop.no_ship[s_kind] ) //load the image if not already.
			{
				sprintf ( temp_filename, "graphics/nif/nif_%s.jpg", ship_name ( s_kind, temp_str ) );
				shop.no_ship[s_kind] = IMG_Load ( temp_filename );
			}
			SDL_BlitSurface ( shop.no_ship[s_kind], NULL, screen, &destination );
		}
		else //image found so show it
		{
			if ( !server[s_id].ship[s_type].ship_img[s_kind][i] )
				shop.rotate_sell = 0; //Set back if you hit a premature end

			SDL_BlitSurface ( server[s_id].ship[s_type].ship_img[s_kind][i], NULL, screen, &destination );
		}

		i++;

		if ( i==16 )
			i=0;

#ifdef _M_IX86 //if linux or windows
		Sleep ( 75 );
#else
		usleep ( 50000 );
#endif

		sdl_flip_mutex();
	}


	stat_viewer.thread = NULL; //let everyone know it has exited
	return 0;
}

void button_stat_viewer ( int x, int y )
{
	int i;
	const int button_max = 16;
	char rtn_str[500];

	for ( i=0;i<button_max;i++ )
		if ( x >= stat_viewer.button[i].x && x <= stat_viewer.button[i].x + stat_viewer.button[i].w && y >= stat_viewer.button[i].y && y <= stat_viewer.button[i].y + stat_viewer.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0: //leave
			stat_viewer.visible = 0;

			//dont do a thing if the repair screen is still f'ing around
			while ( stat_viewer.thread )
				SDL_Delay ( 40 );

			if ( game.home_id == game.server_id && game.home_planet == planet.last_planet )
			{
				switch ( server[game.server_id].planet[planet.last_planet].home_class )
				{
					default:
					case 1:
						load_gen_home();
						break;
					case 2:
						load_velic_home();
						break;
				}
			}
			else //not home planet
			{
				load_cb_info();
			}
			break;
		case 1:
		case 2:
		case 3:
			//request list info
			sprintf ( rtn_str,"25,%d",i );
			send_con_hub ( rtn_str );
			break;
		default:
			//list selects
			sprintf ( rtn_str, "24,%s", stat_viewer.list_name_label[i-4].message );
			send_con_hub ( rtn_str );
			break;
		case 14: //left deco
			stat_viewer_left_deco();
			break;
		case 15: //right deco
			stat_viewer_right_deco();
			break;
	}
}

void key_stat_viewer ( int c )
{
	int i;
	char rtn_str[500];

	if ( c == 13 )
	{
		sprintf ( rtn_str, "24,%s", stat_viewer.text_box.message );
		send_con_hub ( rtn_str );
	}
	else
	{
		add_text_box ( screen, &stat_viewer.text_box, c );
		sdl_flip_mutex();
	}
}

void load_splash_screen()
{
	int i;
	SDL_Surface *blackout;
	SDL_Rect clear_space;
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	clear_space.x = 0;
	clear_space.y = 0;
	clear_space.h = 600;
	clear_space.w = 800;

	blackout = SDL_CreateRGBSurface ( SDL_HWSURFACE, 800, 600, 32, rmask, gmask, bmask, amask );
	SDL_FillRect ( blackout,&clear_space,SDL_MapRGBA ( blackout->format,0,0,0,10 ) );


	if ( !splash_screen.backround )
	{
		splash_screen.backround = IMG_Load ( "graphics/splashback.jpg" );
	}


	set_forms_invisible();
	splash_screen.visible = 1;
	SDL_BlitSurface ( splash_screen.backround, NULL, screen, NULL );
	sdl_flip_mutex();

	SDL_Delay ( 1000 );

	load_sounds();

	for ( i=0;i + 10<=150;i+=3 )
	{
		SDL_SetAlpha ( blackout, SDL_RLEACCEL | SDL_SRCALPHA, 10 + i );
		SDL_BlitSurface ( blackout, NULL, screen, NULL );
		sdl_flip_mutex();
		SDL_Delay ( 5 );
	}

}

void load_login()
{
	SDL_Surface *create_button;
	SDL_Surface *exit_button;
	SDL_Rect destination;
	int i;

	if ( !login.backround )
	{
		login.backround = IMG_Load ( "graphics/logback.jpg" );
		create_button = IMG_Load ( "graphics/newchar.jpg" );
		exit_button = IMG_Load ( "graphics/leave.jpg" );

		destination.x = 660;
		destination.y = 520;
		SDL_BlitSurface ( create_button, NULL, login.backround, &login.button[0] );
		SDL_BlitSurface ( exit_button, NULL, login.backround, &destination );
	}

	set_forms_invisible();
	login.visible = 1;
	SDL_BlitSurface ( login.backround, NULL, screen, NULL );

	for ( i=0;i<2;i++ )
		if ( login.text_box[i].selected )
			select_text_box ( screen, &login.text_box[i] );
		else
			unselect_text_box ( screen, &login.text_box[i] );

	draw_raw_label ( screen, login.label.message, login.label.location, 255, 255, 255 );

	sdl_flip_mutex();
}

void button_login ( int x, int y )
{
	int i, success = 0, button;

	for ( i=0;i<3;i++ )
		if ( x >= login.button[i].x && x <= login.button[i].x + login.button[i].w && y >= login.button[i].y && y <= login.button[i].y + login.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<2;i++ )
			if ( x >= login.text_box[i].location.x && x <= login.text_box[i].location.x + login.text_box[i].location.w && y >= login.text_box[i].location.y && y <= login.text_box[i].location.y + login.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
		switch ( i )
		{
			case 0: //create char
				load_create_char();
				break;
			case 1: //exit game
				load_leave_game();
				break;
			case 2: //switch server
				load_switch_server();
				break;
		}
	else
		switch ( i )
		{
			case 0: //top text_box
				if ( login.text_box[i].selected ) return;
				select_text_box ( screen, &login.text_box[i] );
				unselect_text_box ( screen, &login.text_box[1] );
				sdl_flip_mutex();
				break;
			case 1: //2nd text..
				if ( login.text_box[i].selected ) return;
				select_text_box ( screen, &login.text_box[i] );
				unselect_text_box ( screen, &login.text_box[0] );
				sdl_flip_mutex();
				break;
		}
}

void key_login ( int c )
{
	int i;
	char rtn_str[50] = "3,";

	if ( c == 9 ) //tab
	{
		if ( login.text_box[0].selected )
		{
			unselect_text_box ( screen, &login.text_box[0] );
			select_text_box ( screen, &login.text_box[1] );
			sdl_flip_mutex();
		}
		else
		{
			unselect_text_box ( screen, &login.text_box[1] );
			select_text_box ( screen, &login.text_box[0] );
			sdl_flip_mutex();
		}
	}
	else if ( c == 13 ) //enter
	{
		strcat ( rtn_str,login.text_box[0].message );
		strcat ( rtn_str,"," );
		strcat ( rtn_str,login.text_box[1].message );
		strcpy ( game.password,login.text_box[1].message );
		send_con_hub ( rtn_str );
	}
	else
	{
		for ( i=0;i<2;i++ )
			if ( login.text_box[i].selected )
			{
				add_text_box ( screen, &login.text_box[i], c );
				sdl_flip_mutex();
			}
	}

}

void load_create_char()
{
	SDL_Surface *submit_button;
	int i;


	if ( !create_char.backround )
	{
		create_char.backround = IMG_Load ( "graphics/newback.jpg" );
		submit_button = IMG_Load ( "graphics/submit.jpg" );

		SDL_BlitSurface ( submit_button, NULL, create_char.backround, &create_char.button[0] );
	}

	set_forms_invisible();
	create_char.visible = 1;
	SDL_BlitSurface ( create_char.backround, NULL, screen, NULL );

	for ( i=0;i<6;i++ )
		if ( create_char.text_box[i].selected )
			select_text_box ( screen, &create_char.text_box[i] );
		else
			unselect_text_box ( screen, &create_char.text_box[i] );

	if ( create_char.label.message[0] != '\0' ) draw_raw_label ( screen, create_char.label.message, create_char.label.location, 255, 255, 255 );

	sdl_flip_mutex();
}

void button_create_char ( int x, int y )
{
	int i, k, success = 0, button;

	for ( i=0;i<2;i++ )
		if ( x >= create_char.button[i].x && x <= create_char.button[i].x + create_char.button[i].w && y >= create_char.button[i].y && y <= create_char.button[i].y + create_char.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<6;i++ )
			if ( x >= create_char.text_box[i].location.x && x <= create_char.text_box[i].location.x + create_char.text_box[i].location.w && y >= create_char.text_box[i].location.y && y <= create_char.text_box[i].location.y + create_char.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
		switch ( i )
		{
			case 0: //submit shit
				send_create_char();
				break;
			case 1: //cancel
				load_login();
				break;
		}
	else
	{
		if ( create_char.text_box[i].selected ) return;
		select_text_box ( screen, &create_char.text_box[i] );
		for ( k=0;k<6;k++ )
			if ( k != i ) unselect_text_box ( screen, &create_char.text_box[k] );
		sdl_flip_mutex();
	}
}

void key_create_char ( int c )
{
	int i;

	if ( c == 9 ) //tab
	{
		for ( i=0;i<6;i++ )
			if ( create_char.text_box[i].selected ) break;

		unselect_text_box ( screen, &create_char.text_box[i] );

		if ( i==5 )
			{i=0;}
		else
			{i++;}

		select_text_box ( screen, &create_char.text_box[i] );
		sdl_flip_mutex();
	}
	else if ( c == 13 )
	{
		send_create_char();
	}
	else
	{
		for ( i=0;i<6;i++ )
			if ( create_char.text_box[i].selected )
			{
				add_text_box ( screen, &create_char.text_box[i], c );
				sdl_flip_mutex();
			}
	}

}

void load_leave_game()
{
	if ( !leave_game.backround )
		leave_game.backround = IMG_Load ( "graphics/exitback.jpg" );

	set_forms_invisible();
	leave_game.visible = 1;
	SDL_BlitSurface ( leave_game.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void button_leave_game ( int x, int y )
{
	exit ( 1 );
}

void load_server_select()
{
	int i;
	SDL_Rect destination;

	if ( !server_select.backround )
	{
		server_select.backround = IMG_Load ( "graphics/serverback.jpg" );
		server_select.circle = IMG_Load ( "graphics/e1.gif" );
		server_select.planetbut = IMG_Load ( "graphics/serverbut.jpg" );
	}

	if ( server_select.newlist )
	{
		/*SDL_BlitSurface(server_select.backround, NULL, server_select.temp_backround, NULL);
		for(i=0;i<15 && server_select.server_x[i] > 0 && server_select.server_y[i] > 0;i++)
		{
			destination.x = server_select.server_x[i];
			destination.y = server_select.server_y[i];
			SDL_SetColorKey(server_select.circle, SDL_SRCCOLORKEY, SDL_MapRGB( server_select.circle->format, 255, 255, 255));
			SDL_BlitSurface(server_select.circle, NULL, server_select.backround, &destination);

			destination.x = server_select.server_x[i] + 20;
			destination.y = server_select.server_y[i] + 20;
			SDL_BlitSurface(server_select.planetbut, NULL, server_select.backround, &destination);
			destination.x = server_select.server_x[i] + 30;
			destination.y = server_select.server_y[i] + 20;
			draw_raw_label(server_select.backround, server_select.servername[i], destination, 255, 255, 255);
		}
		*/
		;
	}

	set_forms_invisible();
	server_select.visible = 1;
	SDL_BlitSurface ( server_select.backround, NULL, screen, NULL );

	if ( server_select.label.message[0] != '\0' ) draw_raw_label ( screen, server_select.label.message, server_select.label.location, 255, 255, 255 );
	for ( i=0;i<15 && server_select.server_x[i] > 0 && server_select.server_y[i] > 0;i++ )
	{
		destination.x = server_select.server_x[i];
		destination.y = server_select.server_y[i];
		SDL_BlitSurface ( server_select.circle, NULL, screen, &destination );

		destination.x = server_select.server_x[i] + 20;
		destination.y = server_select.server_y[i] + 20;
		SDL_BlitSurface ( server_select.planetbut, NULL, screen, &destination );
		destination.x = server_select.server_x[i] + 30;
		destination.y = server_select.server_y[i] + 20;
		draw_raw_label ( screen, server_select.servername[i], destination, 255, 255, 255 );
	}

	sdl_flip_mutex();
}

void button_server_select ( int x, int y )
{
	int i, success = 0, button;
	char temp_message[80];

	if ( x >= server_select.button.x && x <= server_select.button.x + server_select.button.w && y >= server_select.button.y && y <= server_select.button.y + server_select.button.h )
		{success = 1; button = 1;}

	if ( !success ) for ( i=0;i<15 && server_select.server_x[i] > 0 && server_select.server_y[i] > 0;i++ )
			if ( ( x >= server_select.server_x[i] && x <= server_select.server_x[i] + 20 && y >= server_select.server_y[i] && y <= server_select.server_y[i] + 20 ) || ( x >= server_select.server_x[i] + 20 && x <= server_select.server_x[i] + 160 && y >= server_select.server_y[i] + 20 && y <= server_select.server_y[i] + 40 ) )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
	{
		if ( planet.last_planet == -1 || !server_select.connected )
			load_leave_game();
		else
			load_planet ( planet.last_planet );
		return;
	}
	else
	{
		if ( server_select.connecting ) return;
		server_select.connecting = 1;
		game.server_id = server_select.server_id[i];

		//connect_to_server ( server_select.server_ip[i] );
		strcpy(game.server_address, server_select.server_ip[i]);

		sprintf ( temp_message,"Warping to %s...", server_select.servername[i] );
		draw_label ( screen, temp_message, &server_select.label, 255, 255, 255 );
		sdl_flip_mutex();
	}
}

void load_planet_sat()
{
	SDL_Rect destination;
	char temp_str[200];

	if ( !planet_sat.backround )
	{
		planet_sat.backround = IMG_Load ( "graphics/satback.jpg" );
		planet_sat.planetbut = planet.planetbut;
	}

	set_forms_invisible();
	planet_sat.visible = 1;
	SDL_BlitSurface ( planet_sat.backround, NULL, screen, NULL );

	//screen only fool

	//label
	destination.x = 86;
	destination.y = 61;
	if ( !planet_sat.sat.guild_name[0] )
		sprintf ( temp_str, "This planet is currently unowned" );
	else
		sprintf ( temp_str, "This planet is currently under the control of the %s guild", planet_sat.sat.guild_name );
	draw_raw_label ( screen, temp_str, destination, 255, 255, 255 );

	//buttons
	//we own this sat? or under no control?
	if ( !planet_sat.sat.guild_name[0] )
	{
		SDL_BlitSurface ( planet_sat.planetbut, NULL, screen, &planet_sat.button[1] );
		draw_raw_center_label ( screen, "Satellite Maintenance", planet_sat.button[1], 255, 255, 255 );

		planet_sat.show_sat_editor = 1;
		planet_sat.show_planet_editor = 0;
	}
	else if ( str_match ( game.guild.name, planet_sat.sat.guild_name ) )
	{
		SDL_BlitSurface ( planet_sat.planetbut, NULL, screen, &planet_sat.button[1] );
		draw_raw_center_label ( screen, "Satellite Maintenance", planet_sat.button[1], 255, 255, 255 );

		SDL_BlitSurface ( planet_sat.planetbut, NULL, screen, &planet_sat.button[2] );
		draw_raw_center_label ( screen, "Planet Maintenance", planet_sat.button[2], 255, 255, 255 );

		planet_sat.show_sat_editor = 1;
		planet_sat.show_planet_editor = 1;
	}
	else
	{
		planet_sat.show_sat_editor = 0;
		planet_sat.show_planet_editor = 0;
	}


	sdl_flip_mutex();
}

void load_planet ( int planet_number )
{
	if ( !planet.backround )
	{
		planet.planetbut = IMG_Load ( "graphics/planetbut.jpg" );
		planet.exit = IMG_Load ( "graphics/exit.jpg" );
	}

	if ( planet_number != planet.last_planet )
	{
		int i, k, z;
		char temp_str[200];
		SDL_Rect destination;

		sprintf ( temp_str, "server/%s/backs/planet/planet%d.jpg", server[game.server_id].servername, planet_number );

		//spare us the memory leak
		if ( planet.backround && game.nif_image.no_planet_img != planet.backround )
			SDL_FreeSurface ( planet.backround );

		planet.backround = IMG_Load ( temp_str );

		if ( !planet.backround )
		{
			//load nif
			if ( !game.nif_image.no_planet_img )
				game.nif_image.no_planet_img = IMG_Load ( "graphics/nif/nif_planet.jpg" );

			planet.backround = IMG_Load ( "graphics/nif/nif_planet.jpg" );
		}

		destination.x = 86;
		destination.y = 61;
		sprintf ( temp_str, "Welcome to %s", server[game.server_id].planet[planet_number].name );
		draw_raw_label ( planet.backround, temp_str, destination, 255, 255, 255 );

		//clear the button stuff
		for ( i=0;i<PLANET_MAX_BUTTON;i++ )
		{
			planet.button_type[i] = -1;
			planet.button_zone[i] = -1;
		}

		for ( i=0;i<4;i++ ) //only 4 buttons are guarenteed to be written
			SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[i] );

		if ( planet_number == game.home_planet && game.home_id == game.server_id )
		{
			sprintf ( temp_str, "%s's Home", game.username );
			draw_raw_center_label ( planet.backround, temp_str, planet.button[0], 255, 255, 255 );
		}
		else
		{
			draw_raw_center_label ( planet.backround, "CB Information Banks", planet.button[0], 255, 255, 255 );
		}

		draw_raw_center_label ( planet.backround, server[game.server_id].planet[planet_number].bar_name, planet.button[1], 255, 255, 255 );
		draw_raw_center_label ( planet.backround, "Repair", planet.button[2], 255, 255, 255 );

		if ( server[game.server_id].planet[planet_number].is_gate ) //if is a normal gate
		{
			//no trade, sat or guild
			z = 0;
			if ( server[game.server_id].planet[planet_number].is_server_gate )
			{
				draw_raw_center_label ( planet.backround, "Warp to Server", planet.button[3+z], 255, 255, 255 );
				planet.button_type[3+z] = 5;
				planet.button_zone[3+z] = -2;
				z++;
			}

			for ( i=0;i<ZONE_MAX;i++ )
				if ( server[game.server_id].zone[i].name[0] != '\0' )
					for ( k=0;k<SECTOR_MAX;k++ )
						if ( server[game.server_id].zone[i].sector[k].planet == planet_number )
						{
							sprintf ( temp_str, "Warp to %s Zone", server[game.server_id].zone[i].name );
							if ( z > 0 ) SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[3 + z] ); //write new button if needed
							draw_raw_center_label ( planet.backround, temp_str, planet.button[3 + z], 255, 255, 255 );
							planet.button_type[3+z] = 4;
							planet.button_zone[3+z] = i;
							z++;
						}

		}
		else //normal planet
		{
			z = 0;
			draw_raw_center_label ( planet.backround, "Trade Center", planet.button[3], 255, 255, 255 );
			planet.button_type[3] = 0;

			if ( planet_number != game.home_planet || game.home_id != game.server_id )
			{
				SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4] );
				draw_raw_center_label ( planet.backround, "Hanger", planet.button[4], 255, 255, 255 );
				planet.button_type[4] = 6;
				z = 1;
			}

			if ( server[game.server_id].planet[planet_number].has_guild_halls )
			{
				SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4 + z] );
				draw_raw_center_label ( planet.backround, "Guild Halls", planet.button[4 + z], 255, 255, 255 );
				planet.button_type[4 + z] = 1;
				z++;
			}

			if ( server[game.server_id].planet[planet_number].guild[0] != '\0' )
			{
				SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4 + z] );
				sprintf ( temp_str, "%s Guild", server[game.server_id].planet[planet_number].guild );
				draw_raw_center_label ( planet.backround, temp_str, planet.button[4 + z], 255, 255, 255 );
				planet.button_type[4 + z] = 2;
				z++;
			}

			printf("server[game.server_id].planet[planet_number].ship_plan:%d\n", server[game.server_id].planet[planet_number].ship_plan);
			if ( server[game.server_id].planet[planet_number].ship_plan)
			{
				int the_plan;
				
				the_plan = server[game.server_id].planet[planet_number].ship_plan;
				
				if(the_plan > -1 && the_plan < SHIP_MAX)
				{
					SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4 + z] );
					sprintf ( temp_str, "%s Plans", server[game.server_id].ship[the_plan].name);
					draw_raw_center_label ( planet.backround, temp_str, planet.button[4 + z], 255, 255, 255 );
					planet.button_type[4 + z] = 7;
					z++;
				}
			}
			
			//sat monkies
			SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4 + z] );
			draw_raw_center_label ( planet.backround, "Satellite", planet.button[4 + z], 255, 255, 255 );
			planet.button_type[4 + z] = 3;
			z++;

			if ( server[game.server_id].planet[planet_number].is_server_gate )
			{
				SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4 + z] );
				draw_raw_center_label ( planet.backround, "Warp to Server", planet.button[4+z], 255, 255, 255 );
				planet.button_type[4+z] = 5;
				planet.button_zone[4+z] = -2;
				z++;
			}

			SDL_BlitSurface ( planet.planetbut, NULL, planet.backround, &planet.button[4 + z] );
			draw_raw_center_label ( planet.backround, "Launch", planet.button[4 + z], 255, 255, 255 );
			planet.button_type[4 + z] = 4;
			planet.button_zone[4 + z] = -1;
		}

		SDL_BlitSurface ( planet.exit, NULL, planet.backround, &planet.button[PLANET_MAX_BUTTON] );

		planet.last_planet = planet_number;
	}

	//special little bit for the ship lost screen
	if ( !death.visible )
	{
		set_forms_invisible();
		planet.visible = 1;
		SDL_BlitSurface ( planet.backround, NULL, screen, NULL );
		sdl_flip_mutex();
	}
	else
	{
		death.okay = 1;
	}

	//good time to reset this
	space.show_ship = 1;
}

void button_sat_editor ( int x, int y )
{
	const int button_max = 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 7, text_box_max = 1;
	int i, success = 0, button;

	for ( i=0;i<button_max;i++ )
		if ( x >= sat_editor.button[i].x && x <= sat_editor.button[i].x + sat_editor.button[i].w && y >= sat_editor.button[i].y && y <= sat_editor.button[i].y + sat_editor.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<text_box_max;i++ )
			if ( x >= sat_editor.text_box[i].location.x && x <= sat_editor.text_box[i].location.x + sat_editor.text_box[i].location.w && y >= sat_editor.text_box[i].location.y && y <= sat_editor.text_box[i].location.y + sat_editor.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
		switch ( i )
		{
			case 0://exit
				load_planet_sat();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 0://upgrade/purchase
				sat_send_upgrade_purchase();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 1://viewing wep but
				sat_toggle_weapons_viewing();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 2://left
				sat_choose_left();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 3://right
				sat_choose_right();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 4://buy/sell
				sat_send_buy_sell();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 5://reload/repair
				sat_send_reload_repair();
				break;
			case 1 + ( SAT_LEVEL_MAX * SAT_GUN_INC ) + 6://reload/repair
				sat_send_bug_sat();
				break;
			default:
				sat_make_weapon_select ( i-1 );
				break;
		}
	else
	{
		;
	}
}

void button_planet_editor ( int x, int y )
{
	int i;
	const int button_max = 1;

	for ( i=0;i<button_max;i++ )
		if ( x >= planet_editor.button[i].x && x <= planet_editor.button[i].x + planet_editor.button[i].w && y >= planet_editor.button[i].y && y <= planet_editor.button[i].y + planet_editor.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0://exit
			load_planet_sat();
			break;
	}
}

void button_planet_sat ( int x, int y )
{
	int i;
	const int button_max = 3;

	for ( i=0;i<button_max;i++ )
		if ( x >= planet_sat.button[i].x && x <= planet_sat.button[i].x + planet_sat.button[i].w && y >= planet_sat.button[i].y && y <= planet_sat.button[i].y + planet_sat.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0://exit
			load_planet ( planet.last_planet );
			break;
		case 1:// sat editor
			if ( !planet_sat.show_sat_editor ) break;
			load_sat_editor();
			break;
		case 2:// planet editor
			if ( !planet_sat.show_planet_editor ) break;
			load_planet_editor();
			break;
	}
}

void button_planet ( int x, int y )
{
	int i;
	char launch_msg[8];
	const int button_max = 1+PLANET_MAX_BUTTON;

	for ( i=0;i<button_max;i++ )
		if ( x >= planet.button[i].x && x <= planet.button[i].x + planet.button[i].w && y >= planet.button[i].y && y <= planet.button[i].y + planet.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0: //but1 CB or home
			if ( game.home_id == game.server_id && game.home_planet == planet.last_planet )
			{
				switch ( server[game.server_id].planet[planet.last_planet].home_class )
				{
					default:
					case 1:
						load_gen_home();
						break;
					case 2:
						load_velic_home();
						break;
				}
			}
			else
			{
				load_cb_info();//load CB form
			}
			break;
		case 1: //bar
			load_bar();
			break;
		case 2: //repair
			load_repair();
			break;
		case PLANET_MAX_BUTTON: //exit
			load_leave_game();
			break;
		default:
			switch ( planet.button_type[i] )
			{
				case 0: //Trade
					load_trade();
					break;
				case 1: //Guild Halls
					load_guild_halls();
					break;
				case 2: //Guild Join
					load_planet_guild();
					break;
				case 3: //Sat
					load_planet_sat();
					break;
				case 4: //Launch
					if ( game.ship[game.ship_sel].server_id == -1 ) break;
					load_launching();
					sprintf ( launch_msg, "6,%d", planet.button_zone[i] );
					send_con_server ( launch_msg );
					break;
				case 5: //Server Select
					load_server_select();
					break;
				case 6: //hanger
					load_hanger();
					break;
				case 7: //ship plans
					load_purchase_plans();
					break;
			}
			break;
	}
}

void load_gen_home()
{
	if ( !gen_home.backround )
		gen_home.backround = IMG_Load ( "graphics/genback.jpg" );

	set_forms_invisible();
	gen_home.visible = 1;
	SDL_BlitSurface ( gen_home.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void button_gen_home ( int x, int y )
{
	int i;

	for ( i=0;i<6;i++ )
		if ( x >= gen_home.button[i].x && x <= gen_home.button[i].x + gen_home.button[i].w && y >= gen_home.button[i].y && y <= gen_home.button[i].y + gen_home.button[i].h )
			break;

	if ( i == 6 ) return;

	switch ( i )
	{
		case 0: //hanger
			load_hanger();
			break;
		case 1: //new pass
			break;
		case 2: //message
			break;
		case 3: //stats
			load_stat_viewer();
			break;
		case 4: //bounty
			load_bounty();
			break;
		case 5: //leave
			load_planet ( planet.last_planet );
			break;
	}
}

void load_velic_home()
{
	if ( !velic_home.backround )
		velic_home.backround = IMG_Load ( "graphics/velicback.jpg" );

	set_forms_invisible();
	velic_home.visible = 1;
	SDL_BlitSurface ( velic_home.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void button_velic_home ( int x, int y )
{
	int i;

	for ( i=0;i<6;i++ )
		if ( x >= velic_home.button[i].x && x <= velic_home.button[i].x + velic_home.button[i].w && y >= velic_home.button[i].y && y <= velic_home.button[i].y + velic_home.button[i].h )
			break;

	if ( i == 6 ) return;

	switch ( i )
	{
		case 0: //hanger
			load_hanger();
			break;
		case 1: //new pass
			break;
		case 2: //message
			break;
		case 3: //stats
			load_stat_viewer();
			break;
		case 4: //bounty
			load_bounty();
			break;
		case 5: //leave
			load_planet ( planet.last_planet );
			break;
	}
}

void load_hanger()
{
	if ( !hanger.backround )
	{
		hanger.backround = IMG_Load ( "graphics/hangerback.jpg" );
		hanger.selection = IMG_Load ( "graphics/selection.gif" );
		hanger.buynew = IMG_Load ( "graphics/buynew.jpg" );
		hanger.takeselected = IMG_Load ( "graphics/takeselected.jpg" );
		hanger.redraw = 1;
	}

	if ( hanger.redraw ) redraw_hanger();

	set_forms_invisible();
	hanger.visible = 1;
	SDL_BlitSurface ( hanger.temp_backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void redraw_hanger()
{
	int i, empty_slot = 0;
	char temp_filename[200], temp_str[21], temp_line[200];
	SDL_Rect img_location, text_location[3];

	//bring back original to our temp
	SDL_BlitSurface ( hanger.backround, NULL, hanger.temp_backround, NULL );

	for ( i=0;i<10;i++ )
	{
		//check if have ship
		if ( hanger.ship[i].server_id < 0 || hanger.ship[i].type < 0 || hanger.ship[i].kind < 0 )
		{
			//used below
			empty_slot = 1;
			continue;
		}

		//load and draw ship image
		if ( i<5 )
		{
			img_location.x = 104;
			img_location.y = 120 + ( i * 77 );
		}
		else
		{
			img_location.x = 414;
			img_location.y = 120 + ( ( i-5 ) * 77 );
		}

		if ( load_fship ( hanger.ship[i].server_id, hanger.ship[i].type, hanger.ship[i].kind, 0, server[hanger.ship[i].server_id].ship[hanger.ship[i].type].name ) )
			SDL_BlitSurface ( server[hanger.ship[i].server_id].ship[hanger.ship[i].type].fship_img[hanger.ship[i].kind][0], NULL, hanger.temp_backround, &img_location );
		else
			SDL_BlitSurface ( hanger.no_fship[hanger.ship[i].kind][0], NULL, hanger.temp_backround, &img_location );

		//write text
		if ( i<5 )
		{
			text_location[0].x = text_location[1].x = text_location[2].x = 175;
			text_location[0].y = 118 + ( i * 77 );
			text_location[1].y = 135 + ( i * 77 );
			text_location[2].y = 152 + ( i * 77 );
		}
		else
		{
			text_location[0].x = text_location[1].x = text_location[2].x = 485;
			text_location[0].y = 118 + ( ( i-5 ) * 77 );
			text_location[1].y = 135 + ( ( i-5 ) * 77 );
			text_location[2].y = 152 + ( ( i-5 ) * 77 );
		}

		sprintf ( temp_line,"%s %s", server[hanger.ship[i].server_id].ship[hanger.ship[i].type].name, ship_name ( hanger.ship[i].kind, temp_str ) );
		draw_raw_label ( hanger.temp_backround, temp_line, text_location[0], 255, 255, 255 );
		sprintf ( temp_line,"Hull:%d/%d Cargo:%d/%d", hanger.ship[i].hull, hanger.ship[i].hull_max, hanger.ship[i].cargo, hanger.ship[i].cargo_max );
		draw_raw_label ( hanger.temp_backround, temp_line, text_location[1], 255, 255, 255 );
		if ( hanger.ship[i].has_weapons )
			sprintf ( temp_line,"Rng:%d Spd:%d Shld:%d Armed", hanger.ship[i].range, hanger.ship[i].speed, hanger.ship[i].shield );
		else
			sprintf ( temp_line,"Rng:%d Spd:%d Shld:%d Empty", hanger.ship[i].range, hanger.ship[i].speed, hanger.ship[i].shield );
		draw_raw_label ( hanger.temp_backround, temp_line, text_location[2], 255, 255, 255 );
	}

	//draw the selection box
	if ( game.ship_sel<5 )
	{
		img_location.x = 87;
		img_location.y = 103 + ( game.ship_sel * 78 );
	}
	else
	{
		img_location.x = 397;
		img_location.y = 103 + ( ( game.ship_sel-5 ) * 78 );
	}
	SDL_BlitSurface ( hanger.selection, NULL, hanger.temp_backround, &img_location );


	//draw buy new ship if there is an empty slot
	if ( empty_slot )
	{
		img_location.x = 204;
		img_location.y = 519;
		SDL_BlitSurface ( hanger.buynew, NULL, hanger.temp_backround, &img_location );
	}

	//draw take selected... if there is a ship selected
	if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 )
	{
		img_location.x = 343;
		img_location.y = 519;
		SDL_BlitSurface ( hanger.takeselected, NULL, hanger.temp_backround, &img_location );
	}

	hanger.redraw = 0;
}

void button_hanger ( int x, int y )
{
	int i, j;

	for ( i=0;i<13;i++ )
		if ( x >= hanger.button[i].x && x <= hanger.button[i].x + hanger.button[i].w && y >= hanger.button[i].y && y <= hanger.button[i].y + hanger.button[i].h )
			break;

	if ( i == 13 ) return;

	switch ( i )
	{
		case 10: //buy new
			//send server command to switch to empty hanger spot
			for ( j=0;j<10;j++ ) //find empty slot
				if ( hanger.ship[j].server_id < 0 || hanger.ship[j].type < 0 || hanger.ship[j].kind < 0 )
					break;

			if ( j==10 ) //if no empty slot
				break;

			load_shop();

			if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 )
				select_ship ( j ); //if we already have a ship selected, then choose the first empty slot
			break;
		case 11: //edit selected
			if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 )
				load_shop();
			break;
		case 12: //leave
			if ( game.home_id == game.server_id && game.home_planet == planet.last_planet )
			{
				switch ( server[game.server_id].planet[planet.last_planet].home_class )
				{
					default:
					case 1:
						load_gen_home();
						break;
					case 2:
						load_velic_home();
						break;
				}
			}
			else //not home planet
			{
				load_planet ( planet.last_planet );
			}
			break;
		default:
			select_ship ( i ); //because all others are ship cells
			break;
	}
}

void load_shop()
{
	char temp_str[32];
	int i, j, k;
	SDL_Rect destination;
	void *nothing;

	if ( !shop.backround )
	{
		shop.backround = IMG_Load ( "graphics/shopback.jpg" );
		shop.buy_img = IMG_Load ( "graphics/buy.jpg" );
		shop.sell_img = IMG_Load ( "graphics/sell.jpg" );
		shop.empty_pod = IMG_Load ( "graphics/empty_pod.jpg" );
		shop.sel_right = IMG_Load ( "graphics/selup.jpg" );
		shop.sel_left = IMG_Load ( "graphics/seldown.jpg" );
		shop.no_misc_pod = IMG_Load ( "graphics/nif/nif_miscpod_pod.jpg" );
		shop.misc_pod[0] = IMG_Load ( "graphics/shop/equipment/icons/Hull Booster.jpg" ); //0-Hull,1-Shield,2-Cargo,3-Jump
		shop.misc_pod[1] = IMG_Load ( "graphics/shop/equipment/icons/Shielding.jpg" );
		shop.misc_pod[2] = IMG_Load ( "graphics/shop/equipment/icons/Cargo Pod.jpg" );
		shop.misc_pod[3] = IMG_Load ( "graphics/shop/equipment/icons/Jump Drive.jpg" );
		shop.sel_blank = SDL_CreateRGBSurface ( SDL_HWSURFACE, 20, 20, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
		shop.ship_blank = SDL_CreateRGBSurface ( SDL_HWSURFACE, 280, 190, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
		shop.buy_blank = SDL_CreateRGBSurface ( SDL_HWSURFACE, 34, 15, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
		shop.sell_blank = SDL_CreateRGBSurface ( SDL_HWSURFACE, 41, 14, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
		shop.blank_pod = SDL_CreateRGBSurface ( SDL_HWSURFACE, 30, 20, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
		SDL_FillRect ( shop.sel_blank, NULL, SDL_MapRGB ( shop.sel_blank->format, 0, 0, 0 ) );
		SDL_FillRect ( shop.ship_blank, NULL, SDL_MapRGB ( shop.ship_blank->format, 0, 0, 0 ) );
		SDL_FillRect ( shop.buy_blank, NULL, SDL_MapRGB ( shop.buy_blank->format, 0, 0, 0 ) );
		SDL_FillRect ( shop.sell_blank, NULL, SDL_MapRGB ( shop.sell_blank->format, 0, 0, 0 ) );
		SDL_FillRect ( shop.blank_pod, NULL, SDL_MapRGB ( shop.blank_pod->format, 0, 0, 0 ) );

	}

	if ( shop.last_planet != planet.last_planet )
	{
		SDL_Surface *shop_img;
		char temp_str[200];

		SDL_BlitSurface ( shop.backround, NULL, shop.temp_backround, NULL );

		sprintf ( temp_str, "server/%s/backs/shop/shop%d.jpg", server[game.server_id].servername, planet.last_planet );
		shop_img = IMG_Load ( temp_str );
		destination.x = 307;
		destination.y = 109;

		if ( !shop_img )
		{
			if ( !game.nif_image.no_shop_img )
				game.nif_image.no_shop_img = IMG_Load ( "graphics/nif/nif_shop.jpg" );

			shop_img = game.nif_image.no_shop_img;
		}

		SDL_BlitSurface ( shop_img, NULL, shop.temp_backround, &destination );

		if ( shop_img != game.nif_image.no_shop_img )
			SDL_FreeSurface ( shop_img );

		shop_set_initial_selections();

		shop.shop_sel 		= -1;
	}

	//set all sell labels to nothing if we dont have a ship...
	if ( game.ship[game.ship_sel].server_id < 0 || game.ship[game.ship_sel].type < 0 || game.ship[game.ship_sel].kind < 0 )
	{
		for ( i=0;i<10;i++ )
		{
			draw_label ( shop.temp_backround, "", &shop.info_right_label[i], 255, 255, 255 );
			draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );
		}
		draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 ); //clear sell ammount
		draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 ); //Set Selection Names
		draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );
		SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] ); //clear sell button

		for ( i=0;i<16;i++ ) //clean up the damn mess...
			SDL_BlitSurface ( shop.blank_pod, NULL, shop.temp_backround, &shop.button[i + 11] );

		//handle special case...
		if ( shop.shop_sel == 0 && game.money >= server[game.server_id].ship[shop.ship_type_sel].cost[shop.ship_kind_sel] )
			SDL_BlitSurface ( shop.buy_img, NULL, shop.temp_backround, &shop.button[1] );
		else
			SDL_BlitSurface ( shop.buy_blank, NULL, shop.temp_backround, &shop.button[1] );
	}

	set_forms_invisible();
	shop.visible = 1;
	shop.last_planet = planet.last_planet;
	sprintf ( temp_str,"%d", game.money );
	draw_right_label ( shop.temp_backround, temp_str, &shop.user_money_label, 255, 255, 255 );

	change_shop ( 0 );
	if ( !shop.thread )
		shop.thread = SDL_CreateThread ( rotate_shop_buy_pic, nothing );
}

void change_shop ( int new_shop )
{
	char temp_str[32];
	int i, show_buy_img = 1;
	int previous_shop;

	//special case inwhich we may have bought plans at the homeplanet
	if(shop.shop_sel == new_shop && !new_shop && at_home_planet())
	{
		shop_show_type_left_right();
		shop_show_kind_left_right();
	}
	
	if ( shop.shop_sel != new_shop )
	{
		shop_blank_current_item_sell();

		previous_shop = shop.shop_sel;
		shop.shop_sel = new_shop;
		if(shop_can_be_set_to(new_shop))
		switch ( new_shop )
		{
			case 0: //chassis
				shop_show_type_left_right();
				shop_show_kind_left_right();

				shop_display_buy_ship_stat_names();

				shop_display_buy_ship_stats();

				shop.attempt_load_buy = 0;
				shop.rotate = 0; //reset index to start

				shop_show_buy_button();
				
				break;
			case 1: //guns
				shop_show_kind_left_right();

				if ( previous_shop == 0 ) //if the last shop was the chassis one, black out these unneeded buttons and label
				{
					SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[3] );
					SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[4] );
					draw_center_label ( shop.temp_backround, "", &shop.sel_left_label[0], 255, 255, 255 );
				}

				shop_display_buy_gun_stat_names();

				shop_display_buy_gun_stats();

				shop.attempt_load_buy = 0;
				shop.rotate = 0; //reset index to start

				shop_show_buy_button();

				//shop.shop_sel = 1;
				break;
			case 2: //missiles
				shop_show_kind_left_right();

				if ( previous_shop == 0 ) //if the last shop was the chassis one, black out these unneeded buttons and label
				{
					SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[3] );
					SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[4] );
					draw_center_label ( shop.temp_backround, "", &shop.sel_left_label[0], 255, 255, 255 );
				}

				shop_display_buy_missile_stat_names();

				shop_display_buy_missile_stats();

				shop.attempt_load_buy = 0;
				shop.rotate = 0; //reset index to start

				shop_show_buy_button();

				break;
			case 3: //misc
				shop_show_kind_left_right();

				if ( previous_shop == 0 ) //if the last shop was the chassis one, black out these unneeded buttons and label
				{
					SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[3] );
					SDL_BlitSurface ( shop.sel_blank, NULL, shop.temp_backround, &shop.button[4] );
					draw_center_label ( shop.temp_backround, "", &shop.sel_left_label[0], 255, 255, 255 );
				}

				shop_display_buy_misc_stat_names();

				shop_display_buy_misc();

				shop.attempt_load_buy = 0;
				shop.rotate = 0; //reset index to start
				
				shop_show_buy_button();

				break;
		}

	}

	if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 )
		view_sell_shop();
	else
		SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );

	SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
	sdl_flip_mutex();

}

void clear_sell_shop()
{
	int i, show_buy_img = 1;

	for ( i=0;i<10;i++ )
	{
		draw_label ( shop.temp_backround, "", &shop.info_right_label[i], 255, 255, 255 );
		draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );
	}

	draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );

	draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 ); //Set Selection Names
	draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );

	SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] ); //remove sell button

	switch ( shop.shop_sel )
	{
		case 0: //chassis
			shop_show_buy_button();
			break;
		case 1: //gun
			shop_show_buy_button();
			break;
		case 2: //missile
			shop_show_buy_button();
			break;
		case 3: //misc
			shop_show_buy_button();
			break;
	}
}

void view_sell_shop()
{
	char temp_str[32], temp_filename[200];
	int i, temp_misc;

	switch ( shop.shop_sel )
	{
		case 0://chassis

			shop_display_sell_ship_stat_names();

			shop_display_sell_ship();

			shop.attempt_load_sell = 0;
			shop.rotate_sell = 0; //reset index to start

			for ( i=0;i<16;i++ ) //clean up the damn mess...
				SDL_BlitSurface ( shop.blank_pod, NULL, shop.temp_backround, &shop.button[i + 11] );

			SDL_BlitSurface ( shop.buy_blank, NULL, shop.temp_backround, &shop.button[1] ); //remove buy button
			SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] ); //add sell button
			break;
		case 1://gun

			shop_display_sell_gun_stat_names();

			if ( game.ship[game.ship_sel].gun[shop.gun_pod_sel] > -1 && game.ship[game.ship_sel].gun[shop.gun_pod_sel] < GUN_MAX && game.ship[game.ship_sel].gun_id[shop.gun_pod_sel] > -1 && game.ship[game.ship_sel].gun_id[shop.gun_pod_sel] < MAX_SERVER ) //if this selection isn't empty
			{
				shop_display_sell_gun();

				SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
			}
			else
			{
				draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );
				for ( i=0;i<10;i++ ) //clear the rest
					draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );

				draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 );
				draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );

				SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
			}

			shop_display_sell_guns();

			break;
		case 2://missile

			shop_display_sell_missile_stat_names();

			if ( game.ship[game.ship_sel].missile[shop.missile_pod_sel] > -1 && game.ship[game.ship_sel].missile[shop.missile_pod_sel] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[shop.missile_pod_sel] > -1 && game.ship[game.ship_sel].missile_id[shop.missile_pod_sel] < MAX_SERVER )
			{
				shop_display_sell_missile();

				SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
			}
			else
			{
				draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );
				for ( i=0;i<10;i++ ) //clear the rest
					draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );

				draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 );
				draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );

				SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
			}

			for ( i=0;i<game.ship[game.ship_sel].missile_ammount;i++ ) //show those missile pods
				if ( game.ship[game.ship_sel].missile[i] > -1 && game.ship[game.ship_sel].missile[i] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[i] > -1 && game.ship[game.ship_sel].missile_id[i] < MAX_SERVER )
				{
					if ( !load_missile_pod_img ( i ) ) //still not loaded... wah
						SDL_BlitSurface ( game.nif_image.no_missile_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
					else //or is it?
						SDL_BlitSurface ( server[game.ship[game.ship_sel].missile_id[i]].missile[game.ship[game.ship_sel].missile[i]].missile_pod_img, NULL, shop.temp_backround, &shop.button[i + 11] );

				}
				else
				{
					SDL_BlitSurface ( shop.empty_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
				}

			for ( ;i<16;i++ ) //blank out remaining spots
				SDL_BlitSurface ( shop.blank_pod, NULL, shop.temp_backround, &shop.button[i + 11] );

			break;
		case 3://misc

			shop_display_sell_misc_stat_names();

			if ( game.ship[game.ship_sel].misc[shop.misc_pod_sel] > -1 )
			{
				shop_display_sell_misc();

				SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
			}
			else
			{
				draw_label ( shop.temp_backround, "Attribute Increase ::", &shop.info_right_label[1], 255, 255, 255 );

				draw_right_label ( shop.temp_backround, "", &shop.sell_money_label, 255, 255, 255 );
				for ( i=0;i<10;i++ ) //clear the rest
					draw_right_label ( shop.temp_backround, "", &shop.stat_right_label[i], 255, 255, 255 );

				draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[0], 255, 255, 255 );
				draw_center_label ( shop.temp_backround, "", &shop.sel_right_label[1], 255, 255, 255 );

				SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
			}

			for ( i=0;i<game.ship[game.ship_sel].misc_ammount;i++ ) //show those misc pods
				if ( game.ship[game.ship_sel].misc[i] > -1 )
				{
					temp_misc = game.ship[game.ship_sel].misc[i];
					while ( temp_misc > 3 ) //reduce to prime element
						temp_misc -= 4;

					if ( !shop.misc_pod[temp_misc] ) //odd, its not loaded...
						SDL_BlitSurface ( shop.no_misc_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
					else //or is it?
						SDL_BlitSurface ( shop.misc_pod[temp_misc], NULL, shop.temp_backround, &shop.button[i + 11] );

				}
				else
				{
					SDL_BlitSurface ( shop.empty_pod, NULL, shop.temp_backround, &shop.button[i + 11] );
				}

			for ( ;i<16;i++ ) //blank out remaining spots
				SDL_BlitSurface ( shop.blank_pod, NULL, shop.temp_backround, &shop.button[i + 11] );

			break;
	}
}

void update_shop_sell ( int mod_type, int slot_number )
{
	char temp_str[32], temp_filename[200];
	int i, temp_misc;

	if ( !shop.visible ) return; //ya uh spare us if were not there...

	switch ( mod_type )
	{
		case 0://gun
			if ( shop.shop_sel != 1 ) break; //don't bother if we aren't even viewing this shop...

			if ( game.ship[game.ship_sel].gun[slot_number] > -1 && game.ship[game.ship_sel].gun[slot_number] < GUN_MAX && game.ship[game.ship_sel].gun_id[slot_number] > -1 && game.ship[game.ship_sel].gun_id[slot_number] < MAX_SERVER ) //if this selection isn't empty
			{
				if ( slot_number == shop.gun_pod_sel ) //only dick around with stuff if we are editing the selected item
				{
					shop_display_sell_gun();

					SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
				}

				//load the image if needed
				if ( load_gun_pod_img ( slot_number ) )
					SDL_BlitSurface ( server[game.ship[game.ship_sel].gun_id[slot_number]].gun[game.ship[game.ship_sel].gun[slot_number]].gun_pod_img, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
				else
					SDL_BlitSurface ( game.nif_image.no_gun_pod, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
			}
			else
			{
				if ( slot_number == shop.gun_pod_sel ) //only dick around with stuff if we are editing the selected item
				{
					shop_display_sell_gun();

					shop_select_next_sell_item();

					if ( slot_number != shop.gun_pod_sel )
					{
						int g;

						shop_display_sell_gun();

						g = shop.gun_pod_sel;

						if ( game.ship[game.ship_sel].gun[g] > -1 && game.ship[game.ship_sel].gun[g] < GUN_MAX && game.ship[game.ship_sel].gun_id[g] > -1 && game.ship[game.ship_sel].gun_id[g] < MAX_SERVER )
							SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
					}
				}

				SDL_BlitSurface ( shop.empty_pod, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
			}

			//check if we used up all the slots and the buy img should not be removed
			for ( i=0;i<server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].gun_ammount[game.ship[game.ship_sel].kind];i++ )
				if ( game.ship[game.ship_sel].gun[i] < 0 || game.ship[game.ship_sel].gun_id[i] < 0 ) //check for emptyness
					break;

			if ( i == server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].gun_ammount[game.ship[game.ship_sel].kind] )
				SDL_BlitSurface ( shop.buy_blank, NULL, shop.temp_backround, &shop.button[1] );
			else
				SDL_BlitSurface ( shop.buy_img, NULL, shop.temp_backround, &shop.button[1] );

			SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
			break;
		case 1://missile
			if ( shop.shop_sel != 2 ) break;

			if ( game.ship[game.ship_sel].missile[slot_number] > -1 && game.ship[game.ship_sel].missile[slot_number] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[slot_number] > -1 && game.ship[game.ship_sel].missile_id[slot_number] < MAX_SERVER )
			{
				if ( slot_number == shop.missile_pod_sel ) //only dick around with stuff if we are editing the selected item
				{
					shop_display_sell_missile();

					SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
				}

				if ( load_missile_pod_img ( slot_number ) )
					SDL_BlitSurface ( server[game.ship[game.ship_sel].missile_id[slot_number]].missile[game.ship[game.ship_sel].missile[slot_number]].missile_pod_img, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
				else
					SDL_BlitSurface ( game.nif_image.no_missile_pod, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
			}
			else
			{
				if ( slot_number == shop.missile_pod_sel ) //only dick around with stuff if we are editing the selected item
				{
					shop_display_sell_missile();

					shop_select_next_sell_item();

					if ( slot_number != shop.missile_pod_sel )
					{
						int w;

						shop_display_sell_missile();

						w = shop.missile_pod_sel;

						if ( game.ship[game.ship_sel].missile[w] > -1 && game.ship[game.ship_sel].missile[w] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[w] > -1 && game.ship[game.ship_sel].missile_id[w] < MAX_SERVER )
							SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
					}
				}

				SDL_BlitSurface ( shop.empty_pod, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
			}

			//check if we used up all the slots and the buy img should not be removed
			for ( i=0;i<server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].missile_ammount[game.ship[game.ship_sel].kind];i++ )
				if ( game.ship[game.ship_sel].missile[i] < 0 || game.ship[game.ship_sel].missile_id[i] < 0 ) //check for emptyness
					break;

			if ( i == server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].missile_ammount[game.ship[game.ship_sel].kind] )
				SDL_BlitSurface ( shop.buy_blank, NULL, shop.temp_backround, &shop.button[1] );
			else
				SDL_BlitSurface ( shop.buy_img, NULL, shop.temp_backround, &shop.button[1] );

			SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
			break;
		case 2://misc
			if ( shop.shop_sel != 3 ) break;

			if ( game.ship[game.ship_sel].misc[slot_number] > -1 )
			{
				if ( shop.misc_pod_sel == slot_number )
				{
					shop_display_sell_misc();

					SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
				}

				temp_misc = game.ship[game.ship_sel].misc[slot_number];
				while ( temp_misc > 3 ) //reduce to prime element
					temp_misc -= 4;

				if ( !shop.misc_pod[temp_misc] ) //odd, its not loaded...
					SDL_BlitSurface ( shop.no_misc_pod, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
				else //or is it?
					SDL_BlitSurface ( shop.misc_pod[temp_misc], NULL, shop.temp_backround, &shop.button[slot_number + 11] );
			}
			else
			{
				if ( shop.misc_pod_sel == slot_number )
				{
					shop_display_sell_misc();

					shop_select_next_sell_item();

					if ( slot_number != shop.misc_pod_sel )
					{
						int w;

						shop_display_sell_misc();

						w = shop.misc_pod_sel;

						if ( game.ship[game.ship_sel].misc[w] > -1 )
							SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
					}
				}

				SDL_BlitSurface ( shop.empty_pod, NULL, shop.temp_backround, &shop.button[slot_number + 11] );
			}

			//check if we used up all the slots and the buy img should not be removed
			for ( i=0;i<server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].misc_ammount[game.ship[game.ship_sel].kind];i++ )
				if ( game.ship[game.ship_sel].misc[i] < 0 ) //check for emptyness
					break;

			if ( i == server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].misc_ammount[game.ship[game.ship_sel].kind] )
				SDL_BlitSurface ( shop.buy_blank, NULL, shop.temp_backround, &shop.button[1] );
			else
				SDL_BlitSurface ( shop.buy_img, NULL, shop.temp_backround, &shop.button[1] );

			SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
			break;
	}
}

void select_shop_sell_pod ( int pod_number )
{
	char temp_str[32];
	int i;

	switch ( shop.shop_sel )
	{
		case 1://gun
			if ( pod_number > game.ship[game.ship_sel].gun_ammount - 1 ) break;

			//bleh bleh bleh
			shop_draw_sell_item_unselected ( shop.gun_pod_sel );

			shop.gun_pod_sel = pod_number;

			shop_display_sell_gun();

			if ( game.ship[game.ship_sel].gun[shop.gun_pod_sel] > -1 && game.ship[game.ship_sel].gun[shop.gun_pod_sel] < GUN_MAX && game.ship[game.ship_sel].gun_id[shop.gun_pod_sel] > -1 && game.ship[game.ship_sel].gun_id[shop.gun_pod_sel] < MAX_SERVER ) //if this selection isn't empty
				SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
			else //no pod found...
				SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );

			SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
			break;
		case 2://missile
			if ( pod_number > game.ship[game.ship_sel].missile_ammount - 1 ) break;

			//bleh bleh bleh
			shop_draw_sell_item_unselected ( shop.missile_pod_sel );

			shop.missile_pod_sel = pod_number;

			shop_display_sell_missile();

			if ( game.ship[game.ship_sel].missile[shop.missile_pod_sel] > -1 && game.ship[game.ship_sel].missile[shop.missile_pod_sel] < MISSILE_MAX && game.ship[game.ship_sel].missile_id[shop.missile_pod_sel] > -1 && game.ship[game.ship_sel].missile_id[shop.missile_pod_sel] < MAX_SERVER )
				SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
			else //no pod found...
				SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );

			SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
			break;
		case 3://misc
			if ( pod_number > game.ship[game.ship_sel].misc_ammount - 1 ) break;

			//bleh bleh bleh
			shop_draw_sell_item_unselected ( shop.misc_pod_sel );

			shop.misc_pod_sel = pod_number;

			shop_display_sell_misc();

			if ( game.ship[game.ship_sel].misc[shop.misc_pod_sel] > -1 )
			{
				SDL_BlitSurface ( shop.sell_img, NULL, shop.temp_backround, &shop.button[2] );
			}
			else //no pod found...
			{
				draw_label ( shop.temp_backround, "Attribute Increase ::", &shop.info_right_label[1], 255, 255, 255 );

				SDL_BlitSurface ( shop.sell_blank, NULL, shop.temp_backround, &shop.button[2] );
			}

			SDL_BlitSurface ( shop.temp_backround, NULL, screen, NULL );
			break;
	}
}

void button_shop ( int x, int y )
{
	int i;

	for ( i=0;i<27;i++ )
		if ( x >= shop.button[i].x && x <= shop.button[i].x + shop.button[i].w && y >= shop.button[i].y && y <= shop.button[i].y + shop.button[i].h )
			break;

	if ( i == 27 ) return;

	switch ( i )
	{
		case 0: //exit
			shop.visible = 0;

			//dont do a thing if the repair screen is still f'ing around
			while ( shop.thread )
			{
#ifdef WIN32
				Sleep ( 1 );
#else
				usleep ( 1000 );
#endif
			}

			load_hanger();

			break;
		case 1: //buy
			switch ( shop.shop_sel )
			{
				case 0: //chassis
					if ( game.money >= server[game.server_id].ship[shop.ship_type_sel].cost[shop.ship_kind_sel] )
						purchase_ship ( shop.ship_type_sel, shop.ship_kind_sel );
					break;
				case 1: //gun
					if ( game.money >= server[game.server_id].gun[shop.gun_sel].cost ) //will need to check for if you have a ship selected too...
						purchase_ship_mod ( 1, shop.gun_sel );
					break;
				case 2: //missile
					if ( game.money >= server[game.server_id].missile[shop.missile_sel].cost )
						purchase_ship_mod ( 2, shop.missile_sel );
					break;
				case 3: //misc
					if ( game.money >= misc_pod_cost ( shop.misc_sel ) )
						purchase_ship_mod ( 3, shop.misc_sel );
					break;
			}
			break;
		case 2: //sell
			switch ( shop.shop_sel )
			{
				case 0: //chassis
					sell_ship ( game.ship_sel );
					break;
				case 1: //gun
					sell_ship_mod ( shop.shop_sel, shop.gun_pod_sel );
					break;
				case 2: //gun
					sell_ship_mod ( shop.shop_sel, shop.missile_pod_sel );
					break;
				case 3: //gun
					sell_ship_mod ( shop.shop_sel, shop.misc_pod_sel );
					break;
			}
			break;
		case 3: //left_top
			set_shop_type_down();
			break;
		case 4: //right_top
			set_shop_type_up();
			break;
		case 5: //left_bottom
			set_shop_kind_down();
			break;
		case 6: //right_bottom
			set_shop_kind_up();
			break;
		case 7: //chassis shop
			change_shop ( 0 );
			break;
		case 8: //guns shop
			change_shop ( 1 );
			break;
		case 9: //missiles shop
			change_shop ( 2 );
			break;
		case 10: //misc shop
			change_shop ( 3 );
			break;
		default: //all remaining cases are pod selects, so...
			if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 )
				select_shop_sell_pod ( i - 11 );
			break;
	}


}

int rotate_shop_buy_pic ( void *nothing )
{
	int temp_misc;
	SDL_Rect buy_pic_location, sell_pic_location;
	char temp_filename[200], temp_str[21];
	int attempt_load_buy_safety, attempt_load_sell_safety;
	int show_ship_buy, show_gun_buy, show_missile_buy;
	
	show_ship_buy = shop_can_be_set_to(0);
	show_gun_buy = shop_can_be_set_to(0);
	show_missile_buy = shop_can_be_set_to(0);

	buy_pic_location.x = 100;
	buy_pic_location.y = 345;
	sell_pic_location.x = 420;
	sell_pic_location.y = 345;

	while ( shop.visible )
	{
		if ( shop.rotate == 16 ) shop.rotate = 0; //loop back to start..
		if ( shop.rotate_sell == 16 ) shop.rotate_sell = 0;

		switch ( shop.shop_sel )
		{
			case 0: //chassis
				//Begin sell ship rotate code...

				if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 ) //if we got a ship...
				{
					if ( shop.attempt_load_sell > -1 ) //load img if requested
					{
						shop.rotate_sell = attempt_load_sell_safety = shop.attempt_load_sell;//make sure they are insync, and set safety

						if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][shop.attempt_load_sell] )
						{
							sprintf ( temp_filename, "server/%s/shop/ships/%s/%s%d.JPG", server[game.ship[game.ship_sel].server_id].servername, ship_name ( game.ship[game.ship_sel].kind, temp_str ), server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name, attempt_load_sell_safety + 1 );
							server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][attempt_load_sell_safety] = IMG_Load ( temp_filename );
						}

						if ( shop.attempt_load_sell == 15 || !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][attempt_load_sell_safety] )
							shop.attempt_load_sell = -2;

						if ( attempt_load_sell_safety == shop.attempt_load_sell )
							shop.attempt_load_sell++;
					}

					if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][0] ) //if no starting index then display the "no ship" img
					{
						if ( !shop.no_ship[game.ship[game.ship_sel].kind] ) //load the image if not already.
						{
							sprintf ( temp_filename, "graphics/nif/nif_%s.jpg", ship_name ( game.ship[game.ship_sel].kind, temp_str ) );
							shop.no_ship[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
						}
						SDL_BlitSurface ( shop.no_ship[game.ship[game.ship_sel].kind], NULL, screen, &sell_pic_location );
					}
					else
					{
						if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][shop.rotate_sell] )
							shop.rotate_sell = 0; //Set back if you hit a premature end

						SDL_BlitSurface ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][shop.rotate_sell], NULL, screen, &sell_pic_location );
					}

				}

				//Begin buy ship rotate code...
				if(!show_ship_buy) break;

				if ( shop.attempt_load_buy > -1 ) //load img if requested
				{
					shop.rotate = attempt_load_buy_safety = shop.attempt_load_buy;//make sure they are insync, and set safety

					if ( !server[game.server_id].ship[shop.ship_type_sel].ship_img[shop.ship_kind_sel][shop.attempt_load_buy] )
					{
						sprintf ( temp_filename, "server/%s/shop/ships/%s/%s%d.JPG", server[game.server_id].servername, ship_name ( shop.ship_kind_sel, temp_str ), server[game.server_id].ship[shop.ship_type_sel].name, attempt_load_buy_safety + 1 );
						server[game.server_id].ship[shop.ship_type_sel].ship_img[shop.ship_kind_sel][attempt_load_buy_safety] = IMG_Load ( temp_filename );
					}

					if ( shop.attempt_load_buy == 15 || !server[game.server_id].ship[shop.ship_type_sel].ship_img[shop.ship_kind_sel][attempt_load_buy_safety] )
						shop.attempt_load_buy = -2;

					if ( attempt_load_buy_safety == shop.attempt_load_buy )
						shop.attempt_load_buy++;
				}

				if ( !server[game.server_id].ship[shop.ship_type_sel].ship_img[shop.ship_kind_sel][0] ) //if no starting index then display the "no ship" img
				{
					if ( !shop.no_ship[shop.ship_kind_sel] ) //load the image if not already.
					{
						sprintf ( temp_filename, "graphics/nif/nif_%s.jpg", ship_name ( shop.ship_kind_sel, temp_str ) );
						shop.no_ship[shop.ship_kind_sel] = IMG_Load ( temp_filename );
					}
					SDL_BlitSurface ( shop.no_ship[shop.ship_kind_sel], NULL, screen, &buy_pic_location );
					sdl_flip_mutex();
					break;
				}

				if ( !server[game.server_id].ship[shop.ship_type_sel].ship_img[shop.ship_kind_sel][shop.rotate] )
					shop.rotate = 0; //Set back if you hit a premature end

				SDL_BlitSurface ( server[game.server_id].ship[shop.ship_type_sel].ship_img[shop.ship_kind_sel][shop.rotate], NULL, screen, &buy_pic_location );
				sdl_flip_mutex();
				break;
			case 1: //guns
				if(!show_gun_buy) break;

				if ( shop.attempt_load_buy > -1 ) //load img if requested
				{
					shop.rotate = attempt_load_buy_safety = shop.attempt_load_buy;//make sure they are insync, and set safety

					if ( !server[game.server_id].gun[shop.gun_sel].gun_img[shop.attempt_load_buy] )
					{
						sprintf ( temp_filename, "server/%s/shop/guns/pics/%s%d.jpg", server[game.server_id].servername, server[game.server_id].gun[shop.gun_sel].name, attempt_load_buy_safety + 1 );
						server[game.server_id].gun[shop.gun_sel].gun_img[attempt_load_buy_safety] = IMG_Load ( temp_filename );
					}

					if ( shop.attempt_load_buy == 15 || !server[game.server_id].gun[shop.gun_sel].gun_img[attempt_load_buy_safety] )
						shop.attempt_load_buy = -2;

					if ( attempt_load_buy_safety == shop.attempt_load_buy )
						shop.attempt_load_buy++;
				}

				if ( !server[game.server_id].gun[shop.gun_sel].gun_img[0] ) //if no starting index then display the "no ship" img
				{
					if ( !shop.no_gun ) //load the image if not already.
					{
						strcpy ( temp_filename, "graphics/nif/nif_Gun.jpg" );
						shop.no_gun = IMG_Load ( temp_filename );
					}
					SDL_BlitSurface ( shop.no_gun, NULL, screen, &buy_pic_location );
					sdl_flip_mutex();
					break;
				}

				if ( !server[game.server_id].gun[shop.gun_sel].gun_img[shop.rotate] )
					shop.rotate = 0; //Set back if you hit a premature end

				SDL_BlitSurface ( server[game.server_id].gun[shop.gun_sel].gun_img[shop.rotate], NULL, screen, &buy_pic_location );
				sdl_flip_mutex();
				break;
			case 2: //missiles
				if(!show_missile_buy) break;
				
				if ( shop.attempt_load_buy > -1 ) //load img if requested
				{
					shop.rotate = attempt_load_buy_safety = shop.attempt_load_buy;//make sure they are insync, and set safety

					if ( !server[game.server_id].missile[shop.missile_sel].missile_img[shop.attempt_load_buy] )
					{
						sprintf ( temp_filename, "server/%s/shop/missiles/pics/%s%d.jpg", server[game.server_id].servername, server[game.server_id].missile[shop.missile_sel].name, attempt_load_buy_safety + 1 );
						server[game.server_id].missile[shop.missile_sel].missile_img[attempt_load_buy_safety] = IMG_Load ( temp_filename );
					}

					if ( shop.attempt_load_buy == 15 || !server[game.server_id].missile[shop.missile_sel].missile_img[attempt_load_buy_safety] )
						shop.attempt_load_buy = -2;

					if ( attempt_load_buy_safety == shop.attempt_load_buy )
						shop.attempt_load_buy++;
				}

				if ( !server[game.server_id].missile[shop.missile_sel].missile_img[0] ) //if no starting index then display the "no ship" img
				{
					if ( !shop.no_missile ) //load the image if not already.
					{
						strcpy ( temp_filename, "graphics/nif/nif_Missile.jpg" );
						shop.no_missile = IMG_Load ( temp_filename );
					}
					SDL_BlitSurface ( shop.no_missile, NULL, screen, &buy_pic_location );
					sdl_flip_mutex();
					break;
				}

				if ( !server[game.server_id].missile[shop.missile_sel].missile_img[shop.rotate] )
					shop.rotate = 0; //Set back if you hit a premature end

				SDL_BlitSurface ( server[game.server_id].missile[shop.missile_sel].missile_img[shop.rotate], NULL, screen, &buy_pic_location );
				sdl_flip_mutex();
				break;
			case 3: //misc
				temp_misc = shop.misc_sel;

				while ( temp_misc>3 ) //reduce shop.misc_sel to 0-3 for use
					temp_misc -= 4;

				if ( shop.attempt_load_buy > -1 ) //load img if requested
				{
					shop.rotate = attempt_load_buy_safety = shop.attempt_load_buy;//make sure they are insync, and set safety

					if ( !shop.misc_img[temp_misc][shop.attempt_load_buy] )
					{
						switch ( temp_misc )
						{
							case 0:
								sprintf ( temp_filename, "graphics/shop/equipment/pics/Hull Booster%d.JPG", attempt_load_buy_safety + 1 );
								break;
							case 1:
								sprintf ( temp_filename, "graphics/shop/equipment/pics/Shielding%d.JPG", attempt_load_buy_safety + 1 );
								break;
							case 2:
								sprintf ( temp_filename, "graphics/shop/equipment/pics/Cargo Pod%d.JPG", attempt_load_buy_safety + 1 );
								break;
							case 3:
								sprintf ( temp_filename, "graphics/shop/equipment/pics/Jump Drive%d.JPG", attempt_load_buy_safety + 1 );
								break;
						}
						shop.misc_img[temp_misc][attempt_load_buy_safety] = IMG_Load ( temp_filename );
					}

					if ( shop.attempt_load_buy == 15 || !shop.misc_img[temp_misc][attempt_load_buy_safety] )
						shop.attempt_load_buy = -2;

					if ( attempt_load_buy_safety == shop.attempt_load_buy )
						shop.attempt_load_buy++;
				}

				if ( !shop.misc_img[temp_misc][0] ) //if no starting index then display the "no ship" img
				{
					if ( !shop.no_misc ) //load the image if not already.
					{
						strcpy ( temp_filename, "graphics/nif/nif_Miscpod.jpg" );
						shop.no_misc = IMG_Load ( temp_filename );
					}
					SDL_BlitSurface ( shop.no_misc, NULL, screen, &buy_pic_location );
					sdl_flip_mutex();
					break;
				}

				if ( !shop.misc_img[temp_misc][shop.rotate] )
					shop.rotate = 0; //Set back if you hit a premature end

				SDL_BlitSurface ( shop.misc_img[temp_misc][shop.rotate], NULL, screen, &buy_pic_location );
				sdl_flip_mutex();
				break;
		}

#ifdef _M_IX86 //if linux or windows
		Sleep ( 75 );
#else
		usleep ( 50000 );
#endif

		shop.rotate++;
		shop.rotate_sell++;
	}

	shop.thread = NULL;
	return 0; //if not in the shop, then scado
}

void load_bar()
{
	SDL_Rect destination;
	char rtn_str[3] = "2";
	int i;

	if ( !bar.backround )
	{
		bar.backround = IMG_Load ( "graphics/barback.jpg" );
		bar.nif_bar = IMG_Load ( "graphics/nif/nif_bar.jpg" );
		bar.but1_clear = IMG_Load ( "graphics/bar_but1.jpg" );
		bar.but1_transfer = IMG_Load ( "graphics/bar_but1_transfer.jpg" );
		bar.but1_enlist = IMG_Load ( "graphics/bar_but1_enlist.jpg" );
		bar.but1_promote = IMG_Load ( "graphics/bar_but1_promote.jpg" );
		bar.but2_clear = IMG_Load ( "graphics/bar_but2.jpg" );
		bar.but2_remove = IMG_Load ( "graphics/bar_but2_remove.jpg" );
		bar.but2_demote = IMG_Load ( "graphics/bar_but2_demote.jpg" );
		bar.but2_guild = IMG_Load ( "graphics/bar_but2_guild.jpg" );
		bar.but3_clear = IMG_Load ( "graphics/bar_but3.jpg" );
		bar.but3_cancel = IMG_Load ( "graphics/bar_but3_cancel.jpg" );
		bar.but3_chat = IMG_Load ( "graphics/bar_but3_chat.jpg" );
	}

	if ( bar.last_planet != planet.last_planet )
	{
		SDL_Surface *bar_img;
		char temp_str[200];

		sprintf ( temp_str, "server/%s/backs/bar/bar%d.jpg", server[game.server_id].servername, planet.last_planet );
		bar_img = IMG_Load ( temp_str );
		destination.x = 94;
		destination.y = 310;

		if ( bar_img )
			SDL_BlitSurface ( bar_img, NULL, bar.backround, &destination );
		else
			SDL_BlitSurface ( bar.nif_bar, NULL, bar.backround, &destination );
	}

	set_forms_invisible();
	bar.visible = 1;
	bar.last_planet = planet.last_planet;
	SDL_BlitSurface ( bar.backround, NULL, screen, NULL );

	//make "no selection"
	bar.selected_i = -1;

	//now begin writing screen only stuff like text/display boxes
	for ( i=0;i<2;i++ )
	{
		if ( bar.text_box[i].selected )
			select_text_box ( screen, &bar.text_box[i] );
		else
			unselect_text_box ( screen, &bar.text_box[i] );
	}

	//clear and setup begining message for displaybox0
	clear_display_box ( NULL,&bar.display_box[0] );

	draw_display_box ( screen,&bar.display_box[0] );
	draw_display_box ( screen,&bar.display_box[1] );

	sdl_flip_mutex();

	send_con_server ( rtn_str ); //ask for bar info
}

void read_bar_info ( char *bar_info )
{
	int i, point = 0, temp_len;
	char temp_str[1025];

	temp_len = strlen ( bar_info ); //gotta know this

	for ( i=0;i<24;i++ ) //clear all
	{
		bar.playername[i][0] = '\0';
		bar.playerguild[i][0] = '\0';
		bar.playerguild_rank[i][0] = '\0';
		bar.playerguild_rank_id[i] = 0;
	}

	for ( i=0;i<24 && point < temp_len; i++ ) //read in info
	{
		split ( temp_str,bar_info,',',&point ); //read in name
		temp_str[20] = '\0'; //cap for security
		strcpy ( bar.playername[i],temp_str ); //move to right location

		split ( temp_str,bar_info,',',&point ); //same for guild
		temp_str[20] = '\0';
		strcpy ( bar.playerguild[i],temp_str );

		split ( temp_str,bar_info,',',&point ); //same for guild_rank
		temp_str[20] = '\0';
		strcpy ( bar.playerguild_rank[i],temp_str );

		split ( temp_str,bar_info,',',&point ); //same for guild_rank_id
		bar.playerguild_rank_id[i] = atoi ( temp_str );
	}



	if ( bar.visible )
	{
		for ( i=0;i<24;i++ )
			draw_label ( screen, bar.playername[i], &bar.player_label[i], 255, 255, 255 );
		sdl_flip_mutex();
	}
}

void add_bar_entry ( char *username, char *userguild, char *user_guild_rank, int user_guild_rank_id )
{
	int i;

	for ( i=0;i<24;i++ ) //look for copy
		if ( str_match ( bar.playername[i], username ) )
		{
			userguild[20] = '\0'; //safety
			user_guild_rank[20] = 0;
			strcpy ( bar.playerguild[i], userguild );
			strcpy ( bar.playerguild_rank[i], user_guild_rank );
			bar.playerguild_rank_id[i] = user_guild_rank_id;

			//now make changes to title if this player is selected.
			if ( bar.selected_i == i )
			{
				bar_draw_name_tag();
				if ( bar.selection_enlist || bar.selection_demote || bar.selection_remove ) //make sure we have the right guild type thing selected
				{
					bar_clear_selection_type();
					bar_correct_guild_select();
					bar_draw_buttons();
				}


				sdl_flip_mutex();
			}

			return;
		}

	for ( i=0;i<24;i++ ) //no copy found so look for empty slot
		if ( !bar.playername[i][0] )
		{
			userguild[20] = '\0'; //safety
			username[20] = '\0'; //safety
			user_guild_rank[20] = 0;
			strcpy ( bar.playername[i], username );
			strcpy ( bar.playerguild[i], userguild );
			strcpy ( bar.playerguild_rank[i], user_guild_rank );
			bar.playerguild_rank_id[i] = user_guild_rank_id;

			//add player to list
			if ( bar.visible )
			{
				draw_label ( screen, bar.playername[i], &bar.player_label[i], 255, 255, 255 );
				sdl_flip_mutex();
			}
			return;
		}
}

void remove_bar_entry ( char *username )
{
	int i;

	for ( i=0;i<24;i++ ) //look for copy
		if ( str_match ( bar.playername[i], username ) )
		{
			bar.playername[i][0] = 0;
			bar.playerguild[i][0] = 0;
			bar.playerguild_rank[i][0] = 0;
			bar.playerguild_rank_id[i] = 0;

			//now make changes to title if this player is selected.
			if ( bar.visible )
			{
				if ( i==bar.selected_i )
					bar_clear_selection();
				draw_label ( screen, "", &bar.player_label[i], 255, 255, 255 );
				sdl_flip_mutex();
			}

			return;
		}
}

void button_bar ( int x, int y )
{
	int i, success = 0, button;
	char rtn_str[3] = "3";
	const int button_max = 4 + MAX_BAR_ENTRY, text_box_max = 3;

	for ( i=0;i<button_max;i++ )
		if ( x >= bar.button[i].x && x <= bar.button[i].x + bar.button[i].w && y >= bar.button[i].y && y <= bar.button[i].y + bar.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<text_box_max;i++ )
			if ( x >= bar.text_box[i].location.x && x <= bar.text_box[i].location.x + bar.text_box[i].location.w && y >= bar.text_box[i].location.y && y <= bar.text_box[i].location.y + bar.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
	{
		switch ( i )
		{
			case 0: //
			case 1: //
			case 2: //
				bar_button_choice ( i );
				break;
			case 3: //exit
				load_planet ( planet.last_planet );
				send_con_server ( rtn_str ); //notify the server you left the bar
				break;
			default: //player select
				bar_select_player ( i - 4 );
				break;
		}
	}
	else
	{
		int k;

		//already selected ?
		if ( bar.text_box[i].selected ) return;

		//transfer box?
		if ( i==2 && !bar_transfer_box_visible() ) return;

		//unselect those who are
		for ( k=0;k<2;k++ )
			if ( bar.text_box[k].selected )
				unselect_text_box ( screen, &bar.text_box[k] );

		//special
		if ( bar_transfer_box_visible() && bar.text_box[2].selected )
			unselect_text_box ( screen, &bar.text_box[2] );

		//select the lover
		select_text_box ( screen, &bar.text_box[i] );

		//flip the mother
		sdl_flip_mutex();
	}
}

void key_bar ( int c )
{
	int i;
	const int text_box_max = 3;

	//find the selected one
	for ( i=0;i<text_box_max;i++ )
		if ( bar.text_box[i].selected ) break;

	if ( c == 9 ) //tab
	{
		//unselect it
		if ( i<text_box_max )
		{
			if ( i==2 )
			{
				if ( bar_transfer_box_visible() )
					unselect_text_box ( screen, &bar.text_box[i] );
			}
			else
			{
				unselect_text_box ( screen, &bar.text_box[i] );
			}
		}

		//do the addition
		i++;

		//this mother visible?
		if ( i==2 && !bar_transfer_box_visible() ) i++;

		//over the monkey?
		if ( i >= text_box_max ) i = 0;

		//select the shitter
		select_text_box ( screen, &bar.text_box[i] );

		//flip the shitter
		sdl_flip_mutex();
	}
	else if ( c == 13 )
	{
		//f?
		if ( i>=text_box_max ) return;

		switch ( i )
		{
			case 0: // reg chat
				send_public_chat ( bar.text_box[0].message );
				add_text_box_entry ( &bar.text_box[0], bar.text_box[0].message );
				clear_text_box ( screen, &bar.text_box[0] );
				sdl_flip_mutex();
				break;
			case 1: //private chat
				private_chat_send ( bar.text_box[1].message );
				add_text_box_entry ( &bar.text_box[1], bar.text_box[1].message );
				clear_text_box ( screen, &bar.text_box[1] );
				sdl_flip_mutex();
				break;
			case 2: //transfer
				if ( !bar_transfer_box_visible() ) return;

				i = bar.selected_i;

				if ( i==-1 ) return;

				transfer_money_to ( bar.playername[i],atoi ( bar.text_box[2].message ) );

				break;
		}
	}
	else if ( c == 280 || c == 281 )
	{
		if ( i>1 ) return; //not for the higher folk

		if ( c == 280 )
			scroll_up_display_box ( screen, bar.text_box[i].display_box_link );

		if ( c == 281 )
			scroll_down_display_box ( screen, bar.text_box[i].display_box_link );

		sdl_flip_mutex();
	}
	else
	{
		//f?
		if ( i>=text_box_max ) return;

		//bastard?
		if ( i==2 && !bar_transfer_box_visible() ) return;
		if ( i==2 && ( ! ( c >= '0' && c <= '9' ) && ! ( c == 8 || c == 127 || c == 273 || c == 274 || c == 275 || c == 276 ) ) ) return;

		add_text_box ( screen, &bar.text_box[i], c );
		sdl_flip_mutex();
	}
}

void load_trade()
{
	SDL_Rect destination;
	int i;
	char temp_str[32];

	if ( !trade.backround )
	{
		trade.backround = IMG_Load ( "graphics/tradeback.jpg" );
		trade.nif_trade = IMG_Load ( "graphics/nif/nif_trade.jpg" );
		trade.large_good[0] = IMG_Load ( "graphics/gpic1.jpg" );
		trade.large_good[1] = IMG_Load ( "graphics/gpic2.jpg" );
		trade.large_good[2] = IMG_Load ( "graphics/gpic3.jpg" );
		trade.large_good[3] = IMG_Load ( "graphics/gpic4.jpg" );
		trade.large_good[4] = IMG_Load ( "graphics/gpic5.jpg" );
		trade.large_good[5] = IMG_Load ( "graphics/gpic6.jpg" );
		trade.large_good[6] = IMG_Load ( "graphics/gpic7.jpg" );
		trade.large_good[7] = IMG_Load ( "graphics/gpic8.jpg" );
		trade.large_good[8] = IMG_Load ( "graphics/gpic9.jpg" );
		trade.large_good[9] = IMG_Load ( "graphics/gpic10.jpg" );
		trade.goto_trade_but = IMG_Load ( "graphics/exit_trade_news_but.jpg" );
		trade.goto_news_but = IMG_Load ( "graphics/goto_trade_news_but.jpg" );
		trade.sell_all_but = IMG_Load ( "graphics/trade_sell_all_but.jpg" );

		load_good_images();
	}

	if ( trade.last_planet != planet.last_planet )
	{
		SDL_Surface *trade_img;
		char temp_str[200];

		sprintf ( temp_str, "server/%s/backs/trade/bar%d.jpg", server[game.server_id].servername, planet.last_planet );
		trade_img = IMG_Load ( temp_str );
		destination.x = 115;
		destination.y = 148;

		if ( trade_img )
			SDL_BlitSurface ( trade_img, NULL, trade.backround, &destination );
		else
			SDL_BlitSurface ( trade.nif_trade, NULL, trade.backround, &destination );
	}

	set_forms_invisible();
	trade.visible = 1;
	trade.last_planet = planet.last_planet;
	SDL_BlitSurface ( trade.backround, NULL, screen, NULL );

	//Just alittle curious
	sprintf ( temp_str,"%d", game.money );
	draw_right_label ( screen, temp_str, &trade.money_label, 107, 216, 210 );

	sprintf ( temp_str,"%d", game.ship[game.ship_sel].cargo - ship_total_good_ammount ( game.ship_sel ) );
	draw_right_label ( screen, temp_str, &trade.cargo_label, 107, 216, 210 );

	//draw in the shop..
	trade_draw_news_or_shop();

	sdl_flip_mutex();
}

void button_trade ( int x, int y )
{
	int i, success = 0, button;

	for ( i=0;i<13;i++ )
		if ( x >= trade.button[i].x && x <= trade.button[i].x + trade.button[i].w && y >= trade.button[i].y && y <= trade.button[i].y + trade.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<2;i++ )
			if ( x >= trade.text_box[i].location.x && x <= trade.text_box[i].location.x + trade.text_box[i].location.w && y >= trade.text_box[i].location.y && y <= trade.text_box[i].location.y + trade.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
	{
		switch ( i )
		{
			default:
				if ( trade.viewing_news ) break;
				trade_draw_selection ( i );
				sdl_flip_mutex();
				break;
			case 10: //exit
				load_planet ( planet.last_planet );
				break;
			case 11: //news/trade
				trade_toggle_news_and_shop();
				break;
			case 12: //sell all
				if ( !trade.viewing_news )
					trade_sell_all_goods();
				break;
		}
	}
	else
	{
		switch ( i )
		{
			case 0: //top text_box
				if ( trade.text_box[0].selected ) return;
				select_text_box ( screen, &trade.text_box[0] );
				unselect_text_box ( screen, &trade.text_box[1] );
				sdl_flip_mutex();
				break;
			case 1: //2nd text..
				if ( trade.text_box[1].selected ) return;
				select_text_box ( screen, &trade.text_box[1] );
				unselect_text_box ( screen, &trade.text_box[0] );
				sdl_flip_mutex();
				break;
		}
	}
}

void key_trade ( int c )
{
	int i;
	char rtn_str[32];

	if ( c == 9 ) //tab
	{
		if ( trade.text_box[0].selected )
		{
			unselect_text_box ( screen, &trade.text_box[0] );
			select_text_box ( screen, &trade.text_box[1] );
			sdl_flip_mutex();
		}
		else
		{
			unselect_text_box ( screen, &trade.text_box[1] );
			select_text_box ( screen, &trade.text_box[0] );
			sdl_flip_mutex();
		}
	}
	else if ( c == 13 )
	{
		if ( trade.text_box[0].selected )
		{
			sprintf ( rtn_str,"17,%d,%d", trade.good_selected, atoi ( trade.text_box[0].message ) );
			send_con_hub ( rtn_str );
			clear_text_box ( screen, &trade.text_box[0] );
			sdl_flip_mutex();
		}
		else
		{
			sprintf ( rtn_str,"18,%d,%d", trade.good_selected, atoi ( trade.text_box[1].message ) );
			send_con_hub ( rtn_str );
			clear_text_box ( screen, &trade.text_box[1] );
			sdl_flip_mutex();
		}
	}
	else if ( ( c >= '0' && c <= '9' ) || c == 8 || c == 127 || c == 275 || c == 276 )
	{
		for ( i=0;i<2;i++ )
			if ( trade.text_box[i].selected )
			{
				add_text_box ( screen, &trade.text_box[i], c );
				sdl_flip_mutex();
			}
	}
}

void load_repair()
{
	SDL_Rect destination, ohull_box, ihull_box;
	int i;
	char temp_str[30];

	if ( !repair.backround )
	{
		repair.backround = IMG_Load ( "graphics/repairback.jpg" );
		repair.reload_wep_img = IMG_Load ( "graphics/reload_wep.jpg" );
		repair.repair_wep_img = IMG_Load ( "graphics/repair_wep.jpg" );
		repair.repair_ship_img = IMG_Load ( "graphics/repair_ship.jpg" );
	}

	if ( repair.last_planet != planet.last_planet )
	{
		SDL_Surface *repair_img;
		char temp_str[200];

		sprintf ( temp_str, "server/%s/backs/repair/rep%d.jpg", server[game.server_id].servername, planet.last_planet );
		repair_img = IMG_Load ( temp_str );
		destination.x = 91;
		destination.y = 124;

		if ( !repair_img )
		{
			if ( !game.nif_image.no_repair_img )
				game.nif_image.no_repair_img = IMG_Load ( "graphics/nif/nif_repair.jpg" );

			repair_img = game.nif_image.no_repair_img;
		}

		SDL_BlitSurface ( repair_img, NULL, repair.backround, &destination );

		if ( repair_img != game.nif_image.no_repair_img )
			SDL_FreeSurface ( repair_img );
	}

	//draw weapons if requested.
	if ( repair.redraw_weapons )
		repair_draw_weapons();

	set_forms_invisible();
	repair.visible = 1;
	repair.last_planet = planet.last_planet;
	SDL_BlitSurface ( repair.backround, NULL, screen, NULL );

	//----------------------------------------------------
	//screen only stuff

	//show money
	sprintf ( temp_str,"%d", game.money );
	draw_right_label ( screen, temp_str, &repair.money_label, 82, 177, 188 );

	//draw hull box
	ohull_box.x = 470;
	ohull_box.y = 82;
	ohull_box.w = 180;
	ohull_box.h = 16;
	ihull_box.x = 472;
	ihull_box.y = 84;
	ihull_box.w = ( int ) ( 176.0 * game.ship[game.ship_sel].hull / game.ship[game.ship_sel].hull_max );
	ihull_box.h = 12;
	SDL_FillRect ( screen, &ohull_box, SDL_MapRGB ( screen->format, 167, 169, 23 ) );
	if ( game.ship[game.ship_sel].server_id > -1 )
		SDL_FillRect ( screen, &ihull_box, SDL_MapRGB ( screen->format, 246, 246, 62 ) );

	if ( game.ship[game.ship_sel].server_id > -1 && game.ship[game.ship_sel].type > -1 && game.ship[game.ship_sel].kind > -1 )
	{
		//draw repair button if needed
		if ( game.ship[game.ship_sel].hull_max - game.ship[game.ship_sel].hull > 0 )
			SDL_BlitSurface ( repair.repair_ship_img, NULL, screen, &repair.button[1] );

		//draw damage cost
		sprintf ( temp_str,"%d", ( game.ship[game.ship_sel].hull_max - game.ship[game.ship_sel].hull ) * 10 );
		draw_right_label ( screen, temp_str, &repair.cost_label, 82, 177, 188 );
	}

	//draw all gun/missile rep buttons
	if ( game.ship[game.ship_sel].server_id >= 0 && game.ship[game.ship_sel].type >= 0 && game.ship[game.ship_sel].kind >= 0 )
	{
		for ( i=0;i<game.ship[game.ship_sel].gun_ammount;i++ )
			if ( game.ship[game.ship_sel].gun_destroyed[i] && game.ship[game.ship_sel].gun[i] > -1 )
				SDL_BlitSurface ( repair.repair_wep_img, NULL, screen, &repair.button[2 + i] );

		for ( i=0;i<game.ship[game.ship_sel].missile_ammount;i++ )
			if ( game.ship[game.ship_sel].missile_left[i] < 4 && game.ship[game.ship_sel].missile[i] > -1 )
				SDL_BlitSurface ( repair.reload_wep_img, NULL, screen, &repair.button[18 + i] );
	}

	sdl_flip_mutex();

	//start rotation thread
	if ( !repair.thread )
		repair.thread = SDL_CreateThread ( repair_thread, NULL );
}

int repair_thread ( void *nothing )
{
	SDL_Rect destination;
	char temp_str[21], temp_filename[512];
	int i = 0;

	//make sure it is set first
	while ( !repair.thread ) SDL_Delay ( 40 );

	if ( game.ship[game.ship_sel].server_id < 0 ) 	{repair.thread = NULL; return 0;}
	if ( game.ship[game.ship_sel].type < 0 ) 		{repair.thread = NULL; return 0;}
	if ( game.ship[game.ship_sel].kind < 0 ) 		{repair.thread = NULL; return 0;}

	destination.x = 85;
	destination.y = 335;

	while ( repair.visible )
	{
		//check if we need to load
		if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][i] )
		{
			sprintf ( temp_filename, "server/%s/shop/ships/%s/%s%d.JPG", server[game.ship[game.ship_sel].server_id].servername, ship_name ( game.ship[game.ship_sel].kind, temp_str ), server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name, i + 1 );
			server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][i] = IMG_Load ( temp_filename );
		}

		//if no starting index then display the "no ship" img
		if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][0] )
		{
			if ( !shop.no_ship[game.ship[game.ship_sel].kind] ) //load the image if not already.
			{
				sprintf ( temp_filename, "graphics/nif/nif_%s.jpg", ship_name ( game.ship[game.ship_sel].kind, temp_str ) );
				shop.no_ship[game.ship[game.ship_sel].kind] = IMG_Load ( temp_filename );
			}
			SDL_BlitSurface ( shop.no_ship[game.ship[game.ship_sel].kind], NULL, screen, &destination );

			sdl_flip_mutex();//if the purchase half wont flip the screen then do it here..

			//and exit this thread, its job here is done.
			repair.thread = NULL;
			return 0;
		}
		else //image found so show it
		{
			if ( !server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][i] )
				shop.rotate_sell = 0; //Set back if you hit a premature end

			SDL_BlitSurface ( server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].ship_img[game.ship[game.ship_sel].kind][i], NULL, screen, &destination );
		}

		i++;

		if ( i==16 ) i=0;

#ifdef _WIN32 //if linux or windows
		Sleep ( 75 );
#else
		usleep ( 50000 );
#endif

		sdl_flip_mutex();
	}


	repair.thread = NULL; //let everyone know it has exited
	return 0;
}

void load_launching()
{
	if ( !launching.backround )
	{
		launching.backround = IMG_Load ( "graphics/launchingback.jpg" );
	}

	set_forms_invisible();
	launching.visible = 1;
	SDL_BlitSurface ( launching.backround, NULL, screen, NULL );
	sdl_flip_mutex();

	//one thing that belongs here good sir
	check_to_load_server_sun_damages();
}

void load_landing()
{

	stop_sound_repeat ( sound_engine );

	if ( !landing.backround )
	{
		landing.backround = IMG_Load ( "graphics/landingback.jpg" );
	}

	set_forms_invisible();
	landing.visible = 1;
	SDL_BlitSurface ( landing.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void load_map()
{
	int i, k;
	int range_max;
	char temp_filename[200], temp_str[21];
	double offset_x, offset_y, hypo, temp_dbl;
	SDL_Rect destination;

	if ( !map.backround )
	{
		map.backround 			= IMG_Load ( "graphics/jumpback.jpg" );
		map.backround_cutout 		= IMG_Load ( "graphics/jumpback_cut.bmp" );
		map.center_button			= IMG_Load ( "graphics/r1.gif" );
		map.scan_large			= IMG_Load ( "graphics/scan_large.gif" );
		map.scan_small			= IMG_Load ( "graphics/scan_small.gif" );
		map.scan_tiny			= IMG_Load ( "graphics/scan_tiny.gif" );
		map.scan_up				= IMG_Load ( "graphics/scan_up.gif" );
		map.scan_down			= IMG_Load ( "graphics/scan_down.gif" );
		map.scan_left			= IMG_Load ( "graphics/scan_left.gif" );
		map.scan_right			= IMG_Load ( "graphics/scan_right.gif" );
		map.scan_up_left			= IMG_Load ( "graphics/scan_up_left.gif" );
		map.scan_up_right			= IMG_Load ( "graphics/scan_up_right.gif" );
		map.scan_down_left		= IMG_Load ( "graphics/scan_down_left.gif" );
		map.scan_down_right		= IMG_Load ( "graphics/scan_down_right.gif" );
		map.sun_img				= IMG_Load ( "graphics/sun_hyper.gif" );
		map.nebula_img			= IMG_Load ( "graphics/nebula_hyper.gif" );
		map.e_mark				= IMG_Load ( "graphics/exclamation_mark.png" );
		map.g_shield			= IMG_Load ( "graphics/gshield.png" );
		game.game_image.hyper_s_grey	= IMG_Load ( "graphics/hyper_sec_grey.gif" );

		SDL_SetAlpha ( map.nebula_img, SDL_SRCALPHA | SDL_RLEACCEL, 170 );
		SDL_SetColorKey ( map.backround_cutout, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB ( map.backround_cutout->format, 255,255,255 ) );
	}

	//make sure sector icons are loaded
	if ( !server[game.server_id].zone[game.zone].hyper_sel )
	{
		sprintf ( temp_filename, "server/%s/buttons/hyper/sel_%d.gif", server[game.server_id].servername, game.zone );
		server[game.server_id].zone[game.zone].hyper_sel = IMG_Load ( temp_filename );

		if ( !server[game.server_id].zone[game.zone].hyper_sel )
			server[game.server_id].zone[game.zone].hyper_sel = IMG_Load ( "graphics/nif/nif_hyper_sel.gif" );
	}

	if ( !server[game.server_id].zone[game.zone].hyper_sec )
	{
		sprintf ( temp_filename, "server/%s/buttons/hyper/sec_%d.gif", server[game.server_id].servername, game.zone );
		server[game.server_id].zone[game.zone].hyper_sec = IMG_Load ( temp_filename );

		if ( !server[game.server_id].zone[game.zone].hyper_sec )
			server[game.server_id].zone[game.zone].hyper_sec = IMG_Load ( "graphics/nif/nif_hyper_sec.gif" );
	}

	if ( !server[game.server_id].zone[game.zone].hyper_safe )
	{
		sprintf ( temp_filename, "server/%s/buttons/hyper/safe_%d.gif", server[game.server_id].servername, game.zone );
		server[game.server_id].zone[game.zone].hyper_safe = IMG_Load ( temp_filename );

		if ( !server[game.server_id].zone[game.zone].hyper_safe )
			server[game.server_id].zone[game.zone].hyper_safe = IMG_Load ( "graphics/nif/nif_hyper_safe.gif" );
	}

	//temporarily stop the space thread
	//SDL_LockMutex ( space_mutex );
	
	set_forms_invisible();
	map.visible = 1;
	
	//allow it to go again
	//SDL_UnlockMutex ( space_mutex );
	

	//now do screen only stuff
	map_redraw();
	

	sdl_flip_mutex();
}

void button_up_map ( int x, int y )
{
	map_set_shift ( x,y,0 );
}

void button_map ( int x, int y )
{
	int i;
	char rtn_str[10];
	const int button_max = 8;

	for ( i=0;i<button_max;i++ )
		if ( x >= map.button[i].x && x <= map.button[i].x + map.button[i].w && y >= map.button[i].y && y <= map.button[i].y + map.button[i].h )
			break;

	if ( i == button_max ) {map_set_shift ( x,y,1 ); return;}

	switch ( i )
	{
		default: //jump sector
			if ( map.sector_link[i]<0 ) break; //dont send if it isn't valid
			sprintf ( rtn_str, "8,%d", map.sector_link[i] );
			send_con_server ( rtn_str );
			break;
		case 5: //cancel
			load_space();
			break;
		case 6: //plus
			map_zoom_in();
			break;
		case 7: //minus
			map_zoom_out();
			break;
	}
}

void key_map ( int c )
{
	int i;
	char rtn_str[500];

	if ( c == 13 )
	{
		sprintf ( rtn_str, "13,%s", map.text_box.message );
		add_text_box_entry ( &map.text_box, map.text_box.message );
		send_con_server ( rtn_str );
	}
	else
	{
		add_text_box ( screen, &map.text_box, c );
		sdl_flip_mutex();
	}
}

void load_space()
{
	int i;
	char temp_str[50], temp_filename[100];
	SDL_Rect destination, hull_fill;

	//ehh sound
	if ( launching.visible )
		play_sound_repeat ( sound_engine );

	if ( !space.backround )
	{
		space.backround = IMG_Load ( "graphics/mainback.jpg" );
		space.viewing_ship = IMG_Load ( "graphics/view_ship.jpg" );
		space.viewing_sector = IMG_Load ( "graphics/view_sec.jpg" );
		space.viewing_guns = IMG_Load ( "graphics/view_guns.jpg" );
		space.viewing_missiles = IMG_Load ( "graphics/view_missiles.jpg" );
		space.jump_img = IMG_Load ( "graphics/jump.jpg" );
		space.shield_off[0] = IMG_Load ( "graphics/sh1off.jpg" );
		space.shield_off[1] = IMG_Load ( "graphics/sh2off.jpg" );
		space.shield_off[2] = IMG_Load ( "graphics/sh3off.jpg" );
		space.shield_off[3] = IMG_Load ( "graphics/sh4off.jpg" );
		space.shield_off[4] = IMG_Load ( "graphics/sh5off.jpg" );
		space.shield_on[0] = IMG_Load ( "graphics/sh1on.jpg" );
		space.shield_on[1] = IMG_Load ( "graphics/sh2on.jpg" );
		space.shield_on[2] = IMG_Load ( "graphics/sh3on.jpg" );
		space.shield_on[3] = IMG_Load ( "graphics/sh4on.jpg" );
		space.shield_on[4] = IMG_Load ( "graphics/sh5on.jpg" );
		space.trade_goods_img = IMG_Load ( "graphics/tradegoods.jpg" );
		space.contraband_img = IMG_Load ( "graphics/contraband.jpg" );
		space.trade_goods_grey_img = IMG_Load ( "graphics/tradegoods_grey.jpg" );
		space.contraband_grey_img = IMG_Load ( "graphics/contraband_grey.jpg" );
		space.trade_img = IMG_Load ( "graphics/trade.jpg" );
		space.cancel_img = IMG_Load ( "graphics/cancel.jpg" );
		space.engage_img = IMG_Load ( "graphics/engage.jpg" );
		space.disengage_img = IMG_Load ( "graphics/disengage.jpg" );
		space.chat_img = IMG_Load ( "graphics/chat.jpg" );
		space.viewing_goods_space = IMG_Load ( "graphics/view_good_sector.jpg" );
		space.viewing_goods_ship = IMG_Load ( "graphics/view_good_ship.jpg" );
		space.viewing_contras_space = IMG_Load ( "graphics/view_contra_sector.jpg" );
		space.viewing_contras_ship = IMG_Load ( "graphics/view_contra_ship.jpg" );

		space.loc_sun = IMG_Load ( "graphics/loc_sun.gif" );
		//SDL_SetAlpha(space.loc_sun, SDL_SRCALPHA | SDL_RLEACCEL, 170);

		for ( i=0;i<52;i++ )
		{
			sprintf ( temp_filename,"graphics/explosion/%d.GIF",i+1 );
			space.destroy_img[i] = IMG_Load ( temp_filename );
		}

		//draw shield off images onto backround
		for ( i=0;i<5;i++ )
			SDL_BlitSurface ( space.shield_off[i], NULL, space.backround, &space.button[40 + i] );
	}

	if ( space.redraw_loc )
		redraw_space_loc();

	//clear last motion user displayed
	space.motion_sel = -1;

	destination.x = 97;
	destination.y = 65;

	SDL_BlitSurface ( space.temp_loc, NULL, space.backround, &destination );

	set_forms_invisible();
	space.visible = 1;
	SDL_BlitSurface ( space.backround, NULL, screen, NULL );

	//now begin writing screen only stuff like text/display boxes
	for ( i=0;i<2;i++ )
	{
		if ( space.text_box[i].selected )
			select_text_box ( screen, &space.text_box[i] );
		else
			unselect_text_box ( screen, &space.text_box[i] );

		draw_display_box ( screen,&space.display_box[i] );
	}

	//good buttons
	draw_space_good_buttons();

	//draw text boxes
	draw_display_box ( screen,&space.display_box[0] );
	draw_display_box ( screen,&space.display_box[1] );

	//redraw nesscary hud images
	if ( space.view_sector_goods )
		SDL_BlitSurface ( space.viewing_sector, NULL, screen, &space.button[48] );
	else
		SDL_BlitSurface ( space.viewing_ship, NULL, screen, &space.button[48] );

	//shield
	SDL_BlitSurface ( space.shield_on[space.shield_level], NULL, screen, &space.button[40 + space.shield_level] );
	sprintf ( temp_str,"%d", game.ship[game.ship_sel].shield * ( space.shield_level / 4 ) );
	draw_center_label ( screen, temp_str, &space.shield_label, 0, 240, 240 );

	//hull
	sprintf ( temp_str,"%d", game.ship[game.ship_sel].hull );
	draw_center_label ( screen, temp_str, &space.hull_label, 0, 240, 240 );
	hull_fill.x = 139;
	hull_fill.w = 24;
	hull_fill.h = ( short unsigned int ) ( 60 * ( 1.0 * game.ship[game.ship_sel].hull / game.ship[game.ship_sel].hull_max ) );
	hull_fill.y = 376 - hull_fill.h;
	SDL_FillRect ( screen, &hull_fill, SDL_MapRGB ( screen->format, 185, 185, 13 ) );

	//name and ship
	if ( space.show_ship )
		space_do_show_ship();

	//as it says
	space_draw_good_box_if_suposed_to();

	if ( space.jump_ok ) //draw jump button
	{
		SDL_Rect jump_fill;

		jump_fill.x = 97;
		jump_fill.w = 24;
		jump_fill.h = 60;
		jump_fill.y = 316;
		SDL_FillRect ( screen, &jump_fill, SDL_MapRGB ( screen->format, 190, 26, 7 ) );
		SDL_BlitSurface ( space.jump_img, NULL, screen, &space.button[51] );
	}

	//reselect selected if nesscary
	if ( space.user_sel > 0 )
	{
		//this is done becuase the function isn't
		//designed for reselecting an already selected user
		i = space.user_sel;
		space.user_sel = -1;
		space_select_user ( i );
	}

	//show guns if attacking
	if ( space.engaging > -1 )
		space_draw_weapons();

	//good time to set that it loaded
	space.form_load_time = current_time();

	sdl_flip_mutex();

	//start thread if already isnt
// 	if ( !space.thread )
// 		space.thread = SDL_CreateThread ( space_thread, NULL );
}

void button_right_space ( int x, int y )
{
	int i, k, button;

	for ( i=0;i<79;i++ )
		if ( x >= space.button[i].x && x <= space.button[i].x + space.button[i].w && y >= space.button[i].y && y <= space.button[i].y + space.button[i].h )
		{
			if ( space.viewing_cargo && i >= 53 && i <= 68 ) continue;
			if ( space.engaging == -1 && i == 52 ) continue;

			break;
		}

	switch ( i )
	{
		case 0: //posible land
			if ( !space.at_planet )
				break;
		default: //1-39, the launched users
			space_select_user_to_scan ( i );
			break;
		case 40://shield change
		case 41:
		case 42:
		case 43:
		case 44:
			break;
		case 45: //engage / transfer
		case 46: //chat
			break;
		case 47: //trade
			break;
		case 48: //ship/sector
			break;
		case 49: //trade goods
			break;
		case 50: //contraband
			break;
		case 51: //jump
			break;
		case 52: //change weapons viewied
			break;
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
		case 58:
		case 59:
		case 60:
		case 61:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 68:
			break;
		case 69:
		case 70:
		case 71:
		case 72:
		case 73:
		case 74:
		case 75:
		case 76:
		case 77:
		case 78:
			space_select_good ( i - 69 );
			space_auto_do_good ( i - 69 );
			break;
	}
}

void button_space ( int x, int y )
{
	int i, k, success = 0, button;
	char land_msg[4] = "7,";

	for ( i=0;i<79;i++ )
		if ( x >= space.button[i].x && x <= space.button[i].x + space.button[i].w && y >= space.button[i].y && y <= space.button[i].y + space.button[i].h )
		{
			if ( space.viewing_cargo && i >= 53 && i <= 68 ) continue;
			if ( space.engaging == -1 && i == 52 ) continue;

			success = 1; button = 1; break;
		}

	if ( !success ) for ( i=0;i<4;i++ )
			if ( x >= space.text_box[i].location.x && x <= space.text_box[i].location.x + space.text_box[i].location.w && y >= space.text_box[i].location.y && y <= space.text_box[i].location.y + space.text_box[i].location.h )
			{


				success = 1; button = 0; break;
			}

	if ( !success ) return;

	if ( button )
	{
		switch ( i )
		{
			case 0: //posible land
				if ( space.at_planet )
				{
					load_landing();
					send_con_server ( land_msg );
				}
				else
				{
					space_select_user ( 0 );
				}
				break;
			default: //1-39, the launched users
				space_select_user ( i );
				break;
			case 40://shield change
			case 41:
			case 42:
			case 43:
			case 44:
				set_shield ( i - 40 );
				break;
			case 45: //engage / transfer
				if ( space.user_sel != -1 ) //if someone selected (and your not transfering)
					if ( !space.transferring ) //engage
						space_engage_user();
					else
						transfer_money_to ( space.user[space.user_sel].username,0 );
				break;
			case 46: //chat
				space_chat_button();
				break;
			case 47: //trade
				break;
			case 48: //ship/sector
				space_toggle_ship_sector();
				break;
			case 49: //trade goods
				if ( space.engaging != -1 ) break;

				space_draw_good_box ( 1,space.view_cargo_sector );
				break;
			case 50: //contraband
				if ( space.engaging != -1 ) break;

				space_draw_good_box ( 0,space.view_cargo_sector );
				break;
			case 51: //jump
				if ( !space.jump_ok ) break;
				load_map();
				break;
			case 52: //change weapons viewied


				if ( space.weapon_viewing )
					space.weapon_viewing = 0;
				else
					space.weapon_viewing = 1;

				space_draw_weapons();
				sdl_flip_mutex();
				break;
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 58:
			case 59:
			case 60:
			case 61:
			case 62:
			case 63:
			case 64:
			case 65:
			case 66:
			case 67:
			case 68:
				if ( space.weapon_viewing == 1 ) //missiles
					space_fire_missile ( i - 53 );
				break;
			case 69:
			case 70:
			case 71:
			case 72:
			case 73:
			case 74:
			case 75:
			case 76:
			case 77:
			case 78:
				space_select_good ( i - 69 );
				break;
		}
	}
	else
	{

		//checks....
		if ( i==2 )
			if ( !space.viewing_cargo || space.engaging != -1 )
				return;
		if ( i==3 )
			return;


		//do the stuff...
		for ( k=0;k<4;k++ )
		{
			if ( k==2 )
				if ( !space.viewing_cargo || space.engaging != -1 )
					continue;
			if ( k==3 )
				continue;

			space.text_box[k].selected = 0;

			if ( k != i )
				unselect_text_box ( screen, &space.text_box[k] );
			else
				select_text_box ( screen, &space.text_box[k] );
		}

		sdl_flip_mutex();
	}
}

void motion_space ( int x, int y )
{
	int i;
	char hover_str[500], temp_str[21];

	for ( i=0;i<40;i++ )
		if ( x >= space.button[i].x && x <= space.button[i].x + space.button[i].w && y >= space.button[i].y && y <= space.button[i].y + space.button[i].h )
			break;

	if ( i == 40 ) return;

	//check if anyone is there
	if ( space.user[i].ship_id != -1 )
	{
		if ( space.info_label_mode == 0 && space.motion_sel == i ) return;
		if ( space.info_label_mode == 1 && space.motion_sel == space.user_sel ) return;
		if ( space.info_label_mode == -1 && space.user_sel == i ) return;

		space.motion_sel = i;
		space.info_label_mode = 0;
		sprintf ( hover_str, "%s : %s %s :: %s", space.user[i].username, space.user[i].ship_name, ship_name ( space.user[i].ship_kind, temp_str ), space.user[i].guild );
		draw_label ( screen, hover_str, &space.info_label, 0, 230, 0 );
		sdl_flip_mutex();
	}
	else if ( space.user[i].ship_id == -1 && space.user[i].ship_kind > -1 && space.user[i].ship_kind < SPECIAL_FSHIP_MAX )
	{
		//sat
		if ( space.info_label_mode == 0 && space.motion_sel == i ) return;
		if ( space.info_label_mode == 1 && space.motion_sel == space.user_sel ) return;
		if ( space.info_label_mode == -1 && space.user_sel == i ) return;

		space.motion_sel = i;
		space.info_label_mode = 0;
		sprintf ( hover_str, "%s : Defense System :: %s", space.user[i].username, space.user[i].guild );
		draw_label ( screen, hover_str, &space.info_label, 0, 230, 0 );
		sdl_flip_mutex();

	}
	else if ( space.user_sel != -1 ) //over a userless sector
	{
		if ( space.info_label_mode != 0 ) return; //leave if not coming from motion

		//check what we should be drawing

		space.motion_sel = -1;
		space.info_label_mode = 1;
		if ( space.user[space.user_sel].ship_id == -1 && space.user[space.user_sel].ship_kind > -1 && space.user[space.user_sel].ship_kind < 2 )
			sprintf ( hover_str, "%s : Defense System :: %s", space.user[space.user_sel].username, space.user[space.user_sel].guild );
		else
			sprintf ( hover_str, "%s : %s %s :: %s", space.user[space.user_sel].username, space.user[space.user_sel].ship_name, ship_name ( space.user[space.user_sel].ship_kind, temp_str ), space.user[space.user_sel].guild );
		draw_label ( screen, hover_str, &space.info_label, 0, 230, 0 );
		sdl_flip_mutex();
	}
	else //over a userless sector when no one is selected
	{
		if ( space.info_label_mode == -1 ) return;

		space.motion_sel = -1;
		space.info_label_mode = -1;
		draw_label ( screen, "", &space.info_label, 0, 230, 0 );
		sdl_flip_mutex();
	}
}

void key_space ( int c )
{
	int i, sel;
	const int text_max = 4;

	for ( i=0;i<text_max;i++ )
		if ( space.text_box[i].selected )
			break;

	//wtf?
	if ( i==text_max ) i=0;

	sel = i;

	if ( c == 9 ) //tab
	{
		sel++;

		if ( sel==text_max ) sel = 0;

		//okay now for the pisser tests
		if ( sel==2 )
			if ( !space.viewing_cargo || space.engaging != -1 )
			{
				sel++;
				if ( sel==text_max ) sel = 0;

			}
		if ( sel==3 ) //just completely skip this one for now
		{
			sel++;
			if ( sel==text_max ) sel = 0;
		}

		//do the stuff...
		for ( i=0;i<text_max;i++ )
		{
			//checks....
			if ( i==2 )
				if ( !space.viewing_cargo || space.engaging != -1 )
				{
					space.text_box[i].selected = 0;
					continue;
				}
			if ( i==3 )
			{
				space.text_box[i].selected = 0;
				continue;
			}

			if ( i != sel )
				unselect_text_box ( screen, &space.text_box[i] );
			else
				select_text_box ( screen, &space.text_box[i] );
		}

		sdl_flip_mutex();
	}
	else if ( c == 13 ) //enter
	{
		switch ( sel )
		{
			case 0: //public chat
				send_public_chat ( space.text_box[0].message );
				add_text_box_entry ( &space.text_box[0], space.text_box[0].message );
				clear_text_box ( screen, &space.text_box[0] );
				sdl_flip_mutex();
				break;
			case 1: //private chat
				private_chat_send ( space.text_box[1].message );
				add_text_box_entry ( &space.text_box[1], space.text_box[1].message );
				clear_text_box ( screen, &space.text_box[1] );
				sdl_flip_mutex();
				break;
			case 2: //good drop/pickup
				space_do_drop_good ( atoi ( space.text_box[2].message ) );
				break;
			case 3: //transfer money
				break;
		}
	}
	else if ( c == 280 || c == 281 )
	{
		if ( i>1 ) return; //not for the higher folk

		if ( c == 280 )
			scroll_up_display_box ( screen, space.text_box[i].display_box_link );

		if ( c == 281 )
			scroll_down_display_box ( screen, space.text_box[i].display_box_link );

		sdl_flip_mutex();
	}
	else
	{
		//now for special cases
		if ( sel >= 2 && ( ! ( c >= '0' && c <= '9' ) && ! ( c == 8 || c == 127 || c == 275 || c == 276 ) ) ) return;

		add_text_box ( screen, &space.text_box[sel], c );
		sdl_flip_mutex();
	}
}

int space_thread ( void *nothing )
{
	int jump_set = 0, redraw, reset_backround = 0, gun_x, gun_y, i, k;
	int hit_on, hit_x[16], hit_y[16];
	double the_time, interval_time, gun_time = 0, temp_time, temp_ratio, hit_time;
	SDL_Rect jump_fill, user_spot, destination, odamage_box, idamage_box;
	const double hit_time_max = 0.5;

	destination.x = 97;
	destination.y = 65;

	jump_fill.x = 97;
	jump_fill.w = 24;

	odamage_box.w = 60;
	odamage_box.h = 4;

	idamage_box.h = 2;

	the_time = current_time();

	for ( ; space.visible || map.visible;redraw = 0 )
	{
		//sync with server requests
		//SDL_LockMutex ( space_mutex );
		//SDL_UnlockMutex(space_mutex);

		//save old time
		interval_time = current_time() - the_time;
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

		//SDL_UnlockMutex ( space_mutex );

#ifdef _WIN32 //if linux or windows
		Sleep ( 100 );
#else
		usleep ( 100000 );
#endif
	}

	space.thread = NULL; //let everyone know it has exited
	return 0;
}

void redraw_space_loc()
{
	int i;
	char temp_filename[200], temp_str[21];
	SDL_Rect img_location;

	//reset temp loc
	SDL_BlitSurface ( space.loc, NULL, space.temp_loc, NULL );

	//add players to loc
	for ( i=0;i<40;i++ )
		draw_space_user_ship ( i );

	space.redraw_loc = 0;
}

void button_repair ( int x, int y )
{
	int i;
	char rtn_str[20];

	for ( i=0;i<34;i++ )
		if ( x >= repair.button[i].x && x <= repair.button[i].x + repair.button[i].w && y >= repair.button[i].y && y <= repair.button[i].y + repair.button[i].h )
			break;

	if ( i == 34 ) return;

	switch ( i )
	{
		case 0: //exit
			repair.visible = 0;

			//dont do a thing if the repair screen is still f'ing around
			while ( repair.thread )
				SDL_Delay ( 40 );

			load_planet ( planet.last_planet );
			break;
		case 1: //repair ship
			if ( game.ship[game.ship_sel].hull_max - game.ship[game.ship_sel].hull < 0 ) break;

			strcpy ( rtn_str,"13," );
			send_con_hub ( rtn_str );
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17: //gun repair
			if ( !game.ship[game.ship_sel].gun_destroyed[i-2] ) break;
			sprintf ( rtn_str,"15,%d", i-2 );
			send_con_hub ( rtn_str );
			break;
		default: //missile restock
			if ( game.ship[game.ship_sel].missile_left[i-18] >= 4 ) break;
			sprintf ( rtn_str,"16,%d", i-18 );
			send_con_hub ( rtn_str );
			break;
	}
}

void load_death()
{
	//sound...
	stop_sound_repeat ( sound_engine );

	if ( !death.backround )
		death.backround = IMG_Load ( "graphics/shiplost.jpg" );

	set_forms_invisible();
	death.visible = 1;
	SDL_BlitSurface ( death.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void button_death ( int x, int y )
{
	if ( death.okay )
	{
		//crude planet screen loader
		death.okay = 0;

		set_forms_invisible();
		planet.visible = 1;
		SDL_BlitSurface ( planet.backround, NULL, screen, NULL );
		sdl_flip_mutex();
	}
}

void load_guild_halls()
{
	if ( !guild_halls.backround )
	{
		guild_halls.backround = IMG_Load ( "graphics/guildhalls.jpg" );
		game.game_image.guild_toggle_off = IMG_Load ( "graphics/guild_off.jpg" );
		game.game_image.guild_toggle_on = IMG_Load ( "graphics/guild_on.jpg" );
	}

	set_forms_invisible();
	guild_halls.visible = 1;
	SDL_BlitSurface ( guild_halls.backround, NULL, screen, NULL );
	sdl_flip_mutex();
}

void button_guild_halls ( int x, int y )
{
	int i;
	char rtn_str[20];

	for ( i=0;i<5;i++ )
		if ( x >= guild_halls.button[i].x && x <= guild_halls.button[i].x + guild_halls.button[i].w && y >= guild_halls.button[i].y && y <= guild_halls.button[i].y + guild_halls.button[i].h )
			break;

	if ( i == 5 ) return;

	switch ( i )
	{
		case 0: //exit
			load_planet ( planet.last_planet );
			break;
		case 1: //bank
			load_guild_bank();
			break;
		case 2: //listings
			load_guild_listing();
			break;
		case 3: //registry
			load_guild_register();
			break;
		case 4: //editor
			load_guild_editor();
			break;
	}
}

void load_sat_editor()
{
	SDL_Rect destination;

	if ( !sat_editor.backround )
	{
		sat_editor.backround = IMG_Load ( "graphics/sateditor.jpg" );
		sat_editor.empty_pod = IMG_Load ( "graphics/empty_pod.jpg" );
		sat_editor.sat_upgrade_but = IMG_Load ( "graphics/sat_upgrade_but.jpg" );
		sat_editor.sat_bug_but = IMG_Load ( "graphics/sat_bug_but.jpg" );
		sat_editor.sat_purchase_but = IMG_Load ( "graphics/sat_purchase_but.jpg" );
		sat_editor.wep_left_but = IMG_Load ( "graphics/seldown.jpg" );
		sat_editor.wep_right_but = IMG_Load ( "graphics/selup.jpg" );
		sat_editor.wep_buy_but = IMG_Load ( "graphics/sat_wep_buy_but.jpg" );
		sat_editor.wep_sell_but = IMG_Load ( "graphics/sat_wep_sell_but.jpg" );
		sat_editor.wep_repair_but = IMG_Load ( "graphics/sat_wep_repair_but.jpg" );
		sat_editor.wep_reload_but = IMG_Load ( "graphics/sat_wep_reload_but.jpg" );
		sat_editor.viewing_guns_img = IMG_Load ( "graphics/view_guns.jpg" );
		sat_editor.viewing_missiles_img = IMG_Load ( "graphics/view_missiles.jpg" );
	}

	destination.x = 400 - ( 480 / 2 );
	destination.y = 300 - ( 360 / 2 );

	set_forms_invisible();
	sat_editor.visible = 1;
	SDL_BlitSurface ( sat_editor.backround, NULL, screen, &destination );

	//screen only shit
	sat_editor_redraw_entire_screen();

	sdl_flip_mutex();
}

void load_planet_editor()
{
	SDL_Rect destination;

	if ( !planet_editor.backround )
	{
		planet_editor.backround = IMG_Load ( "graphics/planeteditor.jpg" );
	}

	destination.x = 400 - ( 480 / 2 );
	destination.y = 300 - ( 360 / 2 );

	set_forms_invisible();
	planet_editor.visible = 1;
	SDL_BlitSurface ( planet_editor.backround, NULL, screen, &destination );

	//screen only shit
	//guild_editor_reset_variables();
	//guild_editor_reset_text_boxes();
	//guild_editor_reset_toggles();

	sdl_flip_mutex();
}

void load_guild_editor()
{
	SDL_Rect destination;

	if ( !guild_editor.backround )
	{
		guild_editor.backround = IMG_Load ( "graphics/guildeditor.jpg" );

		if ( !game.game_image.guild_toggle_off ) game.game_image.guild_toggle_off = IMG_Load ( "graphics/guild_off.jpg" );
		if ( !game.game_image.guild_toggle_on ) game.game_image.guild_toggle_on = IMG_Load ( "graphics/guild_on.jpg" );
	}

	destination.x = 400 - ( 480 / 2 );
	destination.y = 300 - ( 360 / 2 );

	set_forms_invisible();
	guild_editor.visible = 1;
	SDL_BlitSurface ( guild_editor.backround, NULL, screen, &destination );

	//screen only shit
	guild_editor_reset_variables();
	guild_editor_reset_text_boxes();
	guild_editor_reset_toggles();

	sdl_flip_mutex();
}

void load_guild_editor_more()
{
	SDL_Rect destination;

	if ( !guild_editor_more.backround )
	{
		guild_editor_more.backround = IMG_Load ( "graphics/guildeditor_more.jpg" );
		guild_editor_more.on[0] = IMG_Load ( "graphics/guild_off.jpg" );
		guild_editor_more.on[1] = IMG_Load ( "graphics/guild_on.jpg" );
		guild_editor_more.sel_left = IMG_Load ( "graphics/seldown.jpg" );
		guild_editor_more.sel_right = IMG_Load ( "graphics/selup.jpg" );
		guild_editor_more.save_settings = IMG_Load ( "graphics/save_settings.jpg" );
	}

	destination.x = 400 - ( 480 / 2 );
	destination.y = 300 - ( 360 / 2 );

	set_forms_invisible();
	guild_editor_more.visible = 1;
	SDL_BlitSurface ( guild_editor_more.backround, NULL, screen, &destination );

	//screen only shit
	guild_editor_more_reset_text_boxes();
	guild_editor_more_draw_deco ( 0 );

	sdl_flip_mutex();
}

void load_guild_listing()
{
	if ( !guild_listing.backround )
	{
		guild_listing.backround = IMG_Load ( "graphics/guildlisting.jpg" );
		guild_listing.next_on = IMG_Load ( "graphics/guild_listing_next.jpg" );
		guild_listing.next_off = IMG_Load ( "graphics/guild_listing_next_grey.jpg" );
		guild_listing.prev_on = IMG_Load ( "graphics/guild_listing_previous.jpg" );
		guild_listing.prev_off = IMG_Load ( "graphics/guild_listing_previous_grey.jpg" );

		if ( !game.game_image.guild_toggle_off ) game.game_image.guild_toggle_off = IMG_Load ( "graphics/guild_off.jpg" );
		if ( !game.game_image.guild_toggle_on ) game.game_image.guild_toggle_on = IMG_Load ( "graphics/guild_on.jpg" );
	}

	set_forms_invisible();
	guild_listing.visible = 1;
	SDL_BlitSurface ( guild_listing.backround, NULL, screen, NULL );

	//screen only

	//clear some stuff
	guild_listing_reset_variables();

	//textbox
	select_text_box ( screen, &guild_listing.text_box );

	sdl_flip_mutex();

	//ask for the list
	guild_listing_request_list();
}

void load_guild_bank()
{
	SDL_Rect destination;
	char money_str[50];

	if ( !guild_bank.backround )
		guild_bank.backround = IMG_Load ( "graphics/guildbank.jpg" );

	destination.x = 400 - 150;
	destination.y = 300 - ( 225 / 2 );

	set_forms_invisible();
	guild_bank.visible = 1;
	SDL_BlitSurface ( guild_bank.backround, NULL, screen, &destination );

	select_text_box ( screen, &guild_bank.text_box );
	guild_bank.money_label.message[0] = '\0';
	guild_bank.balance_label.message[0] = '\0';
	sprintf ( money_str,"%d",game.money );
	draw_center_label ( screen, money_str, &guild_bank.money_label, 15, 116, 102 );
	sprintf ( money_str,"%d",game.guild_balance );
	draw_label ( screen, money_str, &guild_bank.balance_label, 15, 116, 102 );

	sdl_flip_mutex();
}

void load_guild_register()
{
	SDL_Rect destination;

	if ( !guild_register.backround )
		guild_register.backround = IMG_Load ( "graphics/guildregister.jpg" );

	destination.x = 400 - 150;
	destination.y = 300 - ( 225 / 2 );

	set_forms_invisible();
	guild_register.visible = 1;
	SDL_BlitSurface ( guild_register.backround, NULL, screen, &destination );
	//now for screen only stuff

	select_text_box ( screen, &guild_register.text_box );
	guild_register.info_label.message[0] = '\0';
	draw_center_label ( screen, "Enter new guild and hit enter.", &guild_register.info_label, 15, 116, 102 );


	sdl_flip_mutex();
}

void key_sat_editor ( int c )
{
	int i;
	const int text_box_max = 1;

	if ( c==13 )
	{
		sat_send_deposit ( atoi ( sat_editor.text_box[0].message ) );//send

		//clear
		clear_text_box ( screen, &sat_editor.text_box[0] );
		sdl_flip_mutex();
	}
	else
	{
		if ( ! ( c >= '0' && c <= '9' ) && ! ( c == 8 || c == 127 || c == 275 || c == 276 ) ) return;

		add_text_box ( screen, &sat_editor.text_box[0], c );
		sdl_flip_mutex();
	}

}

void key_guild_editor ( int c )
{
	int i;

	if ( c == 9 ) //tab
	{
		for ( i=0;i<8;i++ )
			if ( guild_editor.text_box[i].selected ) break;

		unselect_text_box ( screen, &guild_editor.text_box[i] );

		if ( i>=7 )
			{i=0;}
		else
			{i++;}


		select_text_box ( screen, &guild_editor.text_box[i] );
		sdl_flip_mutex();
	}
	else
	{
		for ( i=0;i<8;i++ )
			if ( guild_editor.text_box[i].selected )
			{
				add_text_box ( screen, &guild_editor.text_box[i], c );
				sdl_flip_mutex();
			}
	}
}

void button_guild_editor ( int x, int y )
{
	const int button_max = 2 + 30 + 1, text_box_max = 2 + 6;
	int i, success = 0, button;

	for ( i=0;i<button_max;i++ )
		if ( x >= guild_editor.button[i].x && x <= guild_editor.button[i].x + guild_editor.button[i].w && y >= guild_editor.button[i].y && y <= guild_editor.button[i].y + guild_editor.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<text_box_max;i++ )
			if ( x >= guild_editor.text_box[i].location.x && x <= guild_editor.text_box[i].location.x + guild_editor.text_box[i].location.w && y >= guild_editor.text_box[i].location.y && y <= guild_editor.text_box[i].location.y + guild_editor.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
		switch ( i )
		{
			case 0: //exit
				load_guild_halls();
				break;
			case 1: //save
				guild_editor_send_save();
				break;
			case 32: //more options
				load_guild_editor_more();
				break;
			default:
				guild_editor_toggle_button ( i );
				sdl_flip_mutex();
				break;
		}
	else
	{
		int k;

		for ( k=0;k<text_box_max;k++ )
			if ( guild_editor.text_box[k].selected )
				unselect_text_box ( screen, &guild_editor.text_box[k] );

		select_text_box ( screen, &guild_editor.text_box[i] );

		sdl_flip_mutex();
	}
}

void key_guild_editor_more ( int c )
{
	int i;
	const int text_box_max = 4;

	if ( c == 9 ) //tab
	{
		for ( i=0;i<text_box_max;i++ )
			if ( guild_editor_more.text_box[i].selected ) break;

		unselect_text_box ( screen, &guild_editor_more.text_box[i] );

		if ( i>=text_box_max-1 )
			{i=0;}
		else
			{i++;}


		select_text_box ( screen, &guild_editor_more.text_box[i] );
		sdl_flip_mutex();
	}
	else
	{
		for ( i=0;i<text_box_max;i++ )
			if ( guild_editor_more.text_box[i].selected )
			{
				add_text_box ( screen, &guild_editor_more.text_box[i], c );

				if ( i==3 )
					guild_editor_more_draw_save_changes();

				sdl_flip_mutex();
			}
	}
}

void button_guild_editor_more ( int x, int y )
{
	const int button_max = 6, text_box_max = 4;
	int i, success = 0, button;

	for ( i=0;i<button_max;i++ )
		if ( x >= guild_editor_more.button[i].x && x <= guild_editor_more.button[i].x + guild_editor_more.button[i].w && y >= guild_editor_more.button[i].y && y <= guild_editor_more.button[i].y + guild_editor_more.button[i].h )
			{success = 1; button = 1; break;}

	if ( !success ) for ( i=0;i<text_box_max;i++ )
			if ( x >= guild_editor_more.text_box[i].location.x && x <= guild_editor_more.text_box[i].location.x + guild_editor_more.text_box[i].location.w && y >= guild_editor_more.text_box[i].location.y && y <= guild_editor_more.text_box[i].location.y + guild_editor_more.text_box[i].location.h )
				{success = 1; button = 0; break;}

	if ( !success ) return;

	if ( button )
		switch ( i )
		{
			case 0: //exit
				load_guild_editor();
				break;
			case 1: //save
				guild_editor_more_send_save();
				break;
			case 2: // left deco
				guild_editor_more_previous_deco();
				break;
			case 3: // right deco
				guild_editor_more_next_deco();
				break;
			case 4: // toggle deco
				guild_editor_more_toggle_deco();
				break;
			case 5: //save changes
				if ( !guild_editor_more_deco_changes() ) break;
				guild_editor_more_send_deco_changes();
				break;
		}
	else
	{
		int k;

		for ( k=0;k<text_box_max;k++ )
			if ( guild_editor_more.text_box[k].selected )
				unselect_text_box ( screen, &guild_editor_more.text_box[k] );

		select_text_box ( screen, &guild_editor_more.text_box[i] );

		sdl_flip_mutex();
	}
}

void button_guild_listing ( int x, int y )
{
	int i;
	const int button_max = 6 + MAX_GUILD_LISTING;

	for ( i=0;i<button_max;i++ )
		if ( x >= guild_listing.button[i].x && x <= guild_listing.button[i].x + guild_listing.button[i].w && y >= guild_listing.button[i].y && y <= guild_listing.button[i].y + guild_listing.button[i].h )
			break;

	if ( i == button_max ) return;

	switch ( i )
	{
		case 0: //exit
			load_guild_halls();
			break;
		case 1: //sort name
			guild_listing_sort_name();
			break;
		case 2: //sort worth
			guild_listing_sort_worth();
			break;
		case 3: //sort players
			guild_listing_sort_players();
			break;
		case 4: //previous
			guild_listing_previous();
			break;
		case 5: //next
			guild_listing_next();
			break;
		default:
			guild_listing_select_guild ( i-6 );
			break;
	}
}

void button_guild_bank ( int x, int y )
{
	int i;
	char rtn_str[500];

	for ( i=0;i<3;i++ )
		if ( x >= guild_bank.button[i].x && x <= guild_bank.button[i].x + guild_bank.button[i].w && y >= guild_bank.button[i].y && y <= guild_bank.button[i].y + guild_bank.button[i].h )
			break;

	if ( i == 3 ) return;

	switch ( i )
	{
		case 0: //exit
			load_guild_halls();
			break;
		case 1: //withdraw
			sprintf ( rtn_str,"22,%s",guild_bank.text_box.message );
			send_con_hub ( rtn_str );
			break;
		case 2: //deposit
			sprintf ( rtn_str,"23,%s",guild_bank.text_box.message );
			send_con_hub ( rtn_str );
			break;
	}
}

void button_guild_register ( int x, int y )
{
	int i;

	for ( i=0;i<1;i++ )
		if ( x >= guild_register.button[i].x && x <= guild_register.button[i].x + guild_register.button[i].w && y >= guild_register.button[i].y && y <= guild_register.button[i].y + guild_register.button[i].h )
			break;

	if ( i == 1 ) return;

	switch ( i )
	{
		case 0: //exit
			load_guild_halls();
			break;
	}
}

void key_guild_register ( int c )
{
	int i;
	char rtn_str[500];

	if ( c == 13 )
	{
		sprintf ( rtn_str, "19,%s", guild_register.text_box.message );
		send_con_hub ( rtn_str );
	}
	else
	{
		add_text_box ( screen, &guild_register.text_box, c );
		sdl_flip_mutex();
	}
}

void key_guild_listing ( int c )
{
	int i;
	char rtn_str[500];

	if ( c == 13 )
	{
		sprintf ( rtn_str, "28,%s", guild_listing.text_box.message );
		send_con_hub ( rtn_str );
	}
	else
	{
		add_text_box ( screen, &guild_listing.text_box, c );
		sdl_flip_mutex();
	}
}

void key_guild_bank ( int c )
{
	int i;

	if ( ( c >= '0' && c <= '9' ) || c == 8 || c == 127 || c == 275 || c == 276 )
	{
		add_text_box ( screen, &guild_bank.text_box, c );
		sdl_flip_mutex();
	}
}

void handle_button_right ( int x, int y )
{
	if ( space.visible ) 		{button_right_space ( x,y ); return;}
}

void handle_button_up ( int x, int y )
{
	if ( map.visible ) 		{button_up_map ( x,y ); return;}
}

void handle_button ( int x, int y )
{
	if ( login.visible ) 		{button_login ( x,y ); return;}
	if ( create_char.visible ) 	{button_create_char ( x,y ); return;}
	if ( leave_game.visible ) 	{button_leave_game ( x,y ); return;}
	if ( server_select.visible ) 	{button_server_select ( x,y ); return;}
	if ( planet.visible ) 		{button_planet ( x,y ); return;}
	if ( planet_sat.visible ) 	{button_planet_sat ( x,y ); return;}
	if ( gen_home.visible ) 		{button_gen_home ( x,y ); return;}
	if ( velic_home.visible ) 	{button_velic_home ( x,y ); return;}
	if ( hanger.visible ) 		{button_hanger ( x,y ); return;}
	if ( shop.visible ) 		{button_shop ( x,y ); return;}
	if ( bar.visible ) 		{button_bar ( x,y ); return;}
	if ( repair.visible ) 		{button_repair ( x,y ); return;}
	if ( space.visible ) 		{button_space ( x,y ); return;}
	if ( map.visible ) 		{button_map ( x,y ); return;}
	if ( death.visible ) 		{button_death ( x,y ); return;}
	if ( trade.visible ) 		{button_trade ( x,y ); return;}
	if ( guild_halls.visible ) 	{button_guild_halls ( x,y ); return;}
	if ( guild_editor.visible ) 	{button_guild_editor ( x,y ); return;}
	if ( guild_editor_more.visible ) 	{button_guild_editor_more ( x,y ); return;}
	if ( guild_listing.visible ) 	{button_guild_listing ( x,y ); return;}
	if ( guild_bank.visible ) 	{button_guild_bank ( x,y ); return;}
	if ( guild_register.visible ) 	{button_guild_register ( x,y ); return;}
	if ( stat_viewer.visible ) 	{button_stat_viewer ( x,y ); return;}
	if ( cb_info.visible ) 		{button_cb_info ( x,y ); return;}
	if ( bounty.visible ) 		{button_bounty ( x,y ); return;}
	if ( sat_editor.visible ) 		{button_sat_editor ( x,y ); return;}
	if ( planet_editor.visible ) 		{button_planet_editor ( x,y ); return;}
	if ( planet_guild.visible ) 		{button_planet_guild ( x,y ); return;}
	if ( switch_server.visible ) 	{button_switch_server ( x,y ); return;}
	if ( purchase_plans.visible ) 	{button_purchase_plans ( x,y ); return;}
}

void handle_motion ( int x, int y )
{
	if ( space.visible ) 		{motion_space ( x,y ); return;}
}

void handle_key ( int c )
{
	if ( login.visible ) 		{key_login ( c ); return;}
	if ( create_char.visible ) 	{key_create_char ( c ); return;}
	if ( bar.visible ) 		{key_bar ( c ); return;}
	if ( space.visible ) 		{key_space ( c ); return;}
	if ( map.visible ) 		{key_map ( c ); return;}
	if ( trade.visible ) 		{key_trade ( c ); return;}
	if ( guild_register.visible ) 	{key_guild_register ( c ); return;}
	if ( guild_bank.visible ) 	{key_guild_bank ( c ); return;}
	if ( stat_viewer.visible ) 	{key_stat_viewer ( c ); return;}
	if ( bounty.visible ) 		{key_bounty ( c ); return;}
	if ( guild_editor.visible ) 		{key_guild_editor ( c ); return;}
	if ( guild_editor_more.visible ) 		{key_guild_editor_more ( c ); return;}
	if ( guild_listing.visible ) 		{key_guild_listing ( c ); return;}
	if ( sat_editor.visible ) 		{key_sat_editor ( c ); return;}
}

void handle_ctrl_key ( int c )
{
	switch ( c )
	{
		case 108:
			ctrl_l();
			break;
		case 117:
			ctrl_u();
			break;
		case 103:
			ctrl_g();
			break;
		case 99:
			ctrl_c();
			break;
		case 'v':
			ctrl_v();
            break;
        case 'w':
			ctrl_w();
            break;
	}
}

void redraw_visible()
{
	if ( login.visible )
	{
		SDL_BlitSurface ( login.backround, NULL, screen, NULL );
		sdl_flip_mutex(); return;
	}
	if ( create_char.visible )
	{
		SDL_BlitSurface ( create_char.backround, NULL, screen, NULL );
		sdl_flip_mutex(); return;
	}
	if ( leave_game.visible )
	{
		SDL_BlitSurface ( leave_game.backround, NULL, screen, NULL );
		sdl_flip_mutex(); return;
	}
}

void check_draw_login()
{
	if ( login.visible )
	{
		SDL_BlitSurface ( login.backround, NULL, screen, NULL );
		sdl_flip_mutex();
	}
}

void init_form_memory()
{
	int i, j, k;

	login.backround = NULL;
	login.visible = 0;
	login.button[0].x = 90;
	login.button[0].y = 150;
	login.button[0].w = 245;
	login.button[0].h = 30;
	login.button[1].x = 655;
	login.button[1].y = 515;
	login.button[1].w = 60;
	login.button[1].h = 25;
	login.button[2].x = 90;
	login.button[2].y = 206;
	login.button[2].w = 175;
	login.button[2].h = 30;
	login.label.location.x = 352;
	login.label.location.y = 77;
	login.label.location.w = 242;
	login.label.location.h = 22;
	login.label.message[0] = '\0';
	clear_text_box ( NULL, &login.text_box[0] );
	clear_text_box ( NULL, &login.text_box[1] );
	setup_text_box ( &login.text_box[0] );
	setup_text_box ( &login.text_box[1] );
	login.text_box[0].max_char = 20;
	login.text_box[0].pass_char = '\0';
	login.text_box[1].max_char = 20;
	login.text_box[1].pass_char = '*';
	login.text_box[0].location.x = 203;
	login.text_box[0].location.y = 75;
	login.text_box[0].location.w = 130;
	login.text_box[0].location.h = 20;
	login.text_box[1].location.x = 203;
	login.text_box[1].location.y = 95;
	login.text_box[1].location.w = 130;
	login.text_box[1].location.h = 20;
	login.text_box[0].selected = 1;

	create_char.backround = NULL;
	create_char.visible = 0;
	create_char.button[0].x = 270;
	create_char.button[0].y = 257;
	create_char.button[0].w = 50;
	create_char.button[0].h = 10;
	create_char.button[1].x = 615;
	create_char.button[1].y = 500;
	create_char.button[1].w = 100;
	create_char.button[1].h = 30;
	create_char.label.location.x = 86;
	create_char.label.location.y = 61;
	create_char.label.location.w = 329;
	create_char.label.location.h = 17;
	create_char.label.message[0] = '\0';
	for ( i=0;i<6;i++ )
	{
		clear_text_box ( NULL, &create_char.text_box[i] );
		setup_text_box ( &create_char.text_box[i] );
		create_char.text_box[i].location.x = 196;
		create_char.text_box[i].location.w = 123;
		create_char.text_box[i].location.h = 17;
		create_char.text_box[i].max_char = 20;
		create_char.text_box[i].pass_char = '\0';
	}
	create_char.text_box[0].location.y = 103;
	create_char.text_box[1].location.y = 129;
	create_char.text_box[2].location.y = 155;
	create_char.text_box[3].location.y = 182;
	create_char.text_box[4].location.y = 207;
	create_char.text_box[5].location.y = 234;
	create_char.text_box[0].selected = 1;
	create_char.text_box[1].pass_char = '*';
	create_char.text_box[4].max_char = 50;

	leave_game.backround = NULL;
	leave_game.visible = 0;

	server_select.backround = NULL;
	server_select.visible = 0;
	server_select.label.location.x = 86;
	server_select.label.location.y = 61;
	server_select.label.location.w = 329;
	server_select.label.location.h = 17;
	server_select.button.x = 613;
	server_select.button.y = 502;
	server_select.button.w = 102;
	server_select.button.h = 28;
	server_select.label.message[0] = '\0';
	server_select.connecting = 0;
	server_select.connected = 0;
	server_select.newlist = 0;

	planet.backround = NULL;
	planet.planetbut = NULL;
	planet.last_planet = -1;
	planet.visible = 0;
	for ( i=0;i<PLANET_MAX_BUTTON;i++ )
	{
		planet.button[i].x = 576;
		planet.button[i].y = 99 + i * 20;
		planet.button[i].w = 140;
		planet.button[i].h = 20;
	}
	planet.button[PLANET_MAX_BUTTON].x = 612;
	planet.button[PLANET_MAX_BUTTON].y = 498;
	planet.button[PLANET_MAX_BUTTON].w = 130;
	planet.button[PLANET_MAX_BUTTON].h = 30;

	gen_home.backround = NULL;
	gen_home.visible = 0;
	gen_home.button[0].x = 301;
	gen_home.button[0].y = 152;
	gen_home.button[0].w = 172;
	gen_home.button[0].h = 111;
	gen_home.button[1].x = 149;
	gen_home.button[1].y = 226;
	gen_home.button[1].w = 117;
	gen_home.button[1].h = 105;
	gen_home.button[2].x = 391;
	gen_home.button[2].y = 272;
	gen_home.button[2].w = 80;
	gen_home.button[2].h = 51;
	gen_home.button[3].x = 481;
	gen_home.button[3].y = 268;
	gen_home.button[3].w = 80;
	gen_home.button[3].h = 57;
	gen_home.button[4].x = 572;
	gen_home.button[4].y = 276;
	gen_home.button[4].w = 93;
	gen_home.button[4].h = 75;
	gen_home.button[5].x = 655;
	gen_home.button[5].y = 515;
	gen_home.button[5].w = 60;
	gen_home.button[5].h = 25;

	velic_home.backround = NULL;
	velic_home.visible = 0;
	velic_home.button[0].x = 260;
	velic_home.button[0].y = 128;
	velic_home.button[0].w = 250;
	velic_home.button[0].h = 136;
	velic_home.button[1].x = 530;
	velic_home.button[1].y = 210;
	velic_home.button[1].w = 110;
	velic_home.button[1].h = 140;
	velic_home.button[2].x = 260;
	velic_home.button[2].y = 280;
	velic_home.button[2].w = 90;
	velic_home.button[2].h = 55;
	velic_home.button[3].x = 380;
	velic_home.button[3].y = 275;
	velic_home.button[3].w = 80;
	velic_home.button[3].h = 55;
	velic_home.button[4].x = 160;
	velic_home.button[4].y = 310;
	velic_home.button[4].w = 90;
	velic_home.button[4].h = 60;
	velic_home.button[5].x = 655;
	velic_home.button[5].y = 515;
	velic_home.button[5].w = 60;
	velic_home.button[5].h = 25;

	hanger.backround = NULL;
	hanger.temp_backround = SDL_CreateRGBSurface ( SDL_HWSURFACE, 800, 600, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
	hanger.visible = 0;
	for ( i=0;i<5;i++ )
	{
		hanger.button[i].x = 92;
		hanger.button[i].y = 109 + ( i * 77 );
		hanger.button[i].w = 305;
		hanger.button[i].h = 73;
		hanger.button[i + 5].x = 402;
		hanger.button[i + 5].y = 109 + ( i * 77 );
		hanger.button[i + 5].w = 305;
		hanger.button[i + 5].h = 73;
	}
	hanger.button[10].x = 201;
	hanger.button[10].y = 515;
	hanger.button[10].w = 125;
	hanger.button[10].h = 37;
	hanger.button[11].x = 342;
	hanger.button[11].y = 515;
	hanger.button[11].w = 300;
	hanger.button[11].h = 37;
	hanger.button[12].x = 655;
	hanger.button[12].y = 515;
	hanger.button[12].w = 60;
	hanger.button[12].h = 25;
	for ( i=0;i<10;i++ )
	{
		hanger.ship[i].name[0] = '\0';
		hanger.ship[i].server_id = -1;
		hanger.ship[i].type = -1;
		hanger.ship[i].kind = -1;
	}

	shop.backround = NULL;
	shop.no_missile = NULL;
	shop.no_gun = NULL;
	shop.no_misc = NULL;
	for ( i=0;i<8;i++ )
		shop.no_ship[i] = NULL;
	shop.temp_backround = SDL_CreateRGBSurface ( SDL_HWSURFACE, 800, 600, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
	shop.visible = 0;
	for ( i=0;i<13;i++ )
	{
		shop.button[i].x = 0;
		shop.button[i].y = 0;
		shop.button[i].w = 0;
		shop.button[i].h = 0;
	}
	shop.button[0].x = 456;
	shop.button[0].y = 62;
	shop.button[0].w = 62;
	shop.button[0].h = 14;
	shop.button[1].x = 91;
	shop.button[1].y = 93;
	shop.button[1].w = 34;
	shop.button[1].h = 15;
	shop.button[2].x = 674;
	shop.button[2].y = 93;
	shop.button[2].w = 41;
	shop.button[2].h = 14;
	shop.button[3].x = 85;
	shop.button[3].y = 127;
	shop.button[3].w = 20;
	shop.button[3].h = 20;
	shop.button[4].x = 247;
	shop.button[4].y = 127;
	shop.button[4].w = 20;
	shop.button[4].h = 20;
	shop.button[5].x = 85;
	shop.button[5].y = 150;
	shop.button[5].w = 20;
	shop.button[5].h = 20;
	shop.button[6].x = 247;
	shop.button[6].y = 150;
	shop.button[6].w = 20;
	shop.button[6].h = 20;
	shop.button[7].x = 295;
	shop.button[7].y = 305;
	shop.button[7].w = 45;
	shop.button[7].h = 14;
	shop.button[8].x = 345;
	shop.button[8].y = 305;
	shop.button[8].w = 53;
	shop.button[8].h = 14;
	shop.button[9].x = 403;
	shop.button[9].y = 305;
	shop.button[9].w = 51;
	shop.button[9].h = 14;
	shop.button[10].x = 460;
	shop.button[10].y = 305;
	shop.button[10].w = 45;
	shop.button[10].h = 14;
	for ( i=11;i<19;i++ ) //first row of pods
	{
		shop.button[i].x = 429 + ( 36 * ( i - 11 ) );
		shop.button[i].y = 360;
		shop.button[i].w = 30;
		shop.button[i].h = 20;
	}
	for ( ;i<27;i++ ) //second row of pods
	{
		shop.button[i].x = 429 + ( 36 * ( i - 19 ) );
		shop.button[i].y = 390;
		shop.button[i].w = 30;
		shop.button[i].h = 20;
	}
	shop.user_money_label.location.x = 352;
	shop.user_money_label.location.y = 60;
	shop.user_money_label.location.w = 98;
	shop.user_money_label.location.h = 11;
	shop.buy_money_label.location.x = 136;
	shop.buy_money_label.location.y = 91;
	shop.buy_money_label.location.w = 80;
	shop.buy_money_label.location.h = 20;
	shop.sell_money_label.location.x = 587;
	shop.sell_money_label.location.y = 91;
	shop.sell_money_label.location.w = 80;
	shop.sell_money_label.location.h = 20;
	shop.sel_left_label[0].location.x = 105;
	shop.sel_left_label[0].location.y = 127;
	shop.sel_left_label[0].location.w = 142;
	shop.sel_left_label[0].location.h = 20;
	shop.sel_left_label[1].location.x = 105;
	shop.sel_left_label[1].location.y = 150;
	shop.sel_left_label[1].location.w = 142;
	shop.sel_left_label[1].location.h = 20;
	shop.sel_right_label[0].location.x = 550;
	shop.sel_right_label[0].location.y = 127;
	shop.sel_right_label[0].location.w = 142;
	shop.sel_right_label[0].location.h = 20;
	shop.sel_right_label[1].location.x = 550;
	shop.sel_right_label[1].location.y = 150;
	shop.sel_right_label[1].location.w = 142;
	shop.sel_right_label[1].location.h = 20;
	shop.user_money_label.message[0] 	= '\0';
	shop.buy_money_label.message[0] 	= '\0';
	shop.sell_money_label.message[0] 	= '\0';
	shop.sel_left_label[0].message[0]	= '\0';
	shop.sel_left_label[1].message[0]	= '\0';
	for ( i=0;i<10;i++ )
	{
		shop.info_left_label[i].location.x = 85;
		shop.info_left_label[i].location.y = 177 + i * 17;
		shop.info_left_label[i].location.w = 110;
		shop.info_left_label[i].location.h = 17;
		shop.stat_left_label[i].location.x = 195;
		shop.stat_left_label[i].location.y = 177 + i * 17;
		shop.stat_left_label[i].location.w = 72;
		shop.stat_left_label[i].location.h = 17;
		shop.info_right_label[i].location.x = 530;
		shop.info_right_label[i].location.y = 177 + i * 17;
		shop.info_right_label[i].location.w = 110;
		shop.info_right_label[i].location.h = 17;
		shop.stat_right_label[i].location.x = 640;
		shop.stat_right_label[i].location.y = 177 + i * 17;
		shop.stat_right_label[i].location.w = 72;
		shop.stat_right_label[i].location.h = 17;
		shop.stat_left_label[i].message[0] = '\0';
		shop.stat_right_label[i].message[0] = '\0';
	}
	shop.info_left_label[0].location.y = 170;
	shop.stat_left_label[0].location.y = 170;
	shop.info_right_label[0].location.y = 170;
	shop.stat_right_label[0].location.y = 170;
	for ( i=0;i<2;i++ )
	{
		shop.sel_left_label[i].message[0] = '\0';
		shop.sel_right_label[i].message[0] = '\0';
	}
	shop.left_display_box.lines = 3;
	shop.right_display_box.lines = 3;
	shop.left_display_box.x = shop.info_left_label[7].location.x;
	shop.left_display_box.y = shop.info_left_label[7].location.y;
	shop.left_display_box.w = 267 - shop.left_display_box.x;
	shop.right_display_box.x = shop.info_right_label[7].location.x;
	shop.right_display_box.y = shop.info_right_label[7].location.y;
	shop.right_display_box.w = shop.left_display_box.w;
	setup_display_box ( & ( shop.left_display_box ) );
	setup_display_box ( & ( shop.right_display_box ) );
	shop.last_planet 	= -1;
	shop.ship_kind_sel 	= -1;
	shop.ship_type_sel 	= -1;
	shop.gun_sel 		= -1;
	shop.missile_sel 	= -1;
	shop.misc_sel 		= -1;
	shop.gun_pod_sel 	= 0;
	shop.missile_pod_sel 	= 0;
	shop.misc_pod_sel	= 0;

	bar.backround = NULL;
	bar.visible = 0;
	for ( i=0;i<12;i++ )
	{
		bar.player_label[i].location.x = 90;
		bar.player_label[i].location.y = 70 + 17 * i;
		bar.player_label[i].location.w = 135;
		bar.player_label[i].location.h = 17;
		bar.player_label[i].message[0] = '\0';
		bar.player_label[i+12].location.x = 225;
		bar.player_label[i+12].location.y = 70 + 17 * i;
		bar.player_label[i+12].location.w = 135;
		bar.player_label[i+12].location.h = 17;
		bar.player_label[i+12].message[0] = '\0';
		bar.button[4 + i] = bar.player_label[i].location;
		bar.button[4 + i + 12] = bar.player_label[i + 12].location;
		bar.playername[i][0] = '\0';
		bar.playername[i+12][0] = '\0';
		bar.playerguild[i][0] = '\0';
		bar.playerguild[i+12][0] = '\0';
	}
	bar.button[0].x = 389;
	bar.button[0].y = 81;
	bar.button[0].w = 106;
	bar.button[0].h = 16;
	bar.button[1].x = 513;
	bar.button[1].y = 81;
	bar.button[1].w = 84;
	bar.button[1].h = 16;
	bar.button[2].x = 614;
	bar.button[2].y = 81;
	bar.button[2].w = 84;
	bar.button[2].h = 16;
	bar.button[3].x = 595;
	bar.button[3].y = 494;
	bar.button[3].w = 690 - bar.button[3].x;
	bar.button[3].h = 28;
	bar.label.location.x = 387;
	bar.label.location.y = 60;
	bar.label.location.w = 717 - bar.label.location.x;
	bar.label.location.h = 20;
	bar.text_box[0].location.x = 464;
	bar.text_box[0].location.y = 281;
	bar.text_box[0].location.w = 230;
	bar.text_box[0].location.h = 14;
	bar.text_box[1].location.x = 464;
	bar.text_box[1].location.y = 116;
	bar.text_box[1].location.w = 230;
	bar.text_box[1].location.h = 14;
	bar.text_box[2].location = bar.button[1];
	bar.text_box[2].location.x += 3;
	bar.text_box[2].location.y -= 2;
	bar.text_box[2].location.w -= 3;
	for ( i=0;i<3;i++ )
	{
		clear_text_box ( NULL, &bar.text_box[i] );
		setup_text_box ( &bar.text_box[i] );
	}
	set_text_box_displaybox ( &bar.text_box[0],&bar.display_box[0] );
	set_text_box_displaybox ( &bar.text_box[1],&bar.display_box[1] );
	bar.text_box[0].selected = 1;
	bar.text_box[0].max_char = 200;
	bar.text_box[1].max_char = 200;
	bar.text_box[2].max_char = 20;
	bar.text_box[0].pass_char = '\0';
	bar.text_box[1].pass_char = '\0';
	bar.text_box[2].pass_char = '\0';
	bar.display_box[0].x = 400;
	bar.display_box[0].y = 305;
	bar.display_box[0].w = 315;
	bar.display_box[1].x = 400;
	bar.display_box[1].y = 140;
	bar.display_box[1].w = 315;
	bar.display_box[0].lines = 10;
	bar.display_box[1].lines = 9;
	setup_display_box ( &bar.display_box[0] );
	setup_display_box ( &bar.display_box[1] );
	bar.last_planet = -1;

	repair.backround = NULL;
	repair.visible = 0;
	repair.money_label.location.x = 147;
	repair.money_label.location.y = 103;
	repair.money_label.location.w = 151;
	repair.money_label.location.h = 15;
	repair.money_label.message[0] = '\0';
	repair.cost_label.location.x = 623;
	repair.cost_label.location.y = 64;
	repair.cost_label.location.w = 88;
	repair.cost_label.location.h = 15;
	repair.cost_label.message[0] = '\0';
	for ( i=0;i<16;i++ )
	{
		repair.button[i+2].x = 450 + ( 120 * ( i>7 ) );
		repair.button[i+2].y = ( 132 + ( i * 22 ) ) - ( 176 * ( i>7 ) );
		repair.button[i+2].w = 55;
		repair.button[i+2].h = 16;
		repair.button[i+18].x = repair.button[i+2].x;
		repair.button[i+18].y = repair.button[i+2].y + 211;
		repair.button[i+18].w = 65;
		repair.button[i+18].h = 16;
	}
	repair.button[0].x = 322;
	repair.button[0].y = 101;
	repair.button[0].w = 151;
	repair.button[0].h = 16;
	repair.button[1].x = 655;
	repair.button[1].y = 82;
	repair.button[1].w = 55;
	repair.button[1].h = 16;
	repair.thread = NULL;
	repair.redraw_weapons = 1;
	repair.last_planet = -1;

	launching.backround = NULL;
	launching.visible = 0;

	landing.backround = NULL;
	landing.visible = 0;

	space.backround = NULL;
	space.loc = SDL_CreateRGBSurface ( SDL_HWSURFACE, 600, 200, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );
	space.temp_loc = SDL_DisplayFormat ( space.loc );
	space.last_loc_number = -1;
	space.last_loc_planet = -1;
	space.last_loc_safe = -1;
	space.last_loc_zone = -1;
	for ( i=0;i<10;i++ )
	{
		space.button[i].x = 97 + ( 60 * i );
		space.button[i + 10].x = 97 + ( 60 * i );
		space.button[i + 20].x = 97 + ( 60 * i );
		space.button[i + 30].x = 97 + ( 60 * i );
		space.button[i].y = 65;
		space.button[i + 10].y = 115;
		space.button[i + 20].y = 165;
		space.button[i + 30].y = 215;
		space.button[i].w = 60;
		space.button[i + 10].w = 60;
		space.button[i + 20].w = 60;
		space.button[i + 30].w = 60;
		space.button[i].h = 50;
		space.button[i + 10].h = 50;
		space.button[i + 20].h = 50;
		space.button[i + 30].h = 50;
	}
	for ( i=40;i<45;i++ )
	{
		space.button[i].x = 176;
		space.button[i].y = 365 - ( 12 * ( i - 40 ) );
		space.button[i].w = 33;
		space.button[i].h = 10;
	}
	for ( i=53;i<69;i++ )
	{

		space.button[i].x = 218 + ( ( ( i-53 ) / 4 ) * 34 );
		space.button[i].y = 288 + ( ( ( i-53 ) % 4 ) * 24 );
		space.button[i].w = 34;
		space.button[i].h = 24;
	}
	//top row buttons
	space.button[45].x = 422;
	space.button[45].y = 293;
	space.button[45].w = 84;
	space.button[45].h = 15;
	space.button[46].x = 519;
	space.button[46].y = 293;
	space.button[46].w = 84;
	space.button[46].h = 15;
	space.button[47].x = 614;
	space.button[47].y = 293;
	space.button[47].w = 84;
	space.button[47].h = 15;
	//bottom row buttons
	space.button[48].x = 422;
	space.button[48].y = 316;
	space.button[48].w = 84;
	space.button[48].h = 15;
	space.button[49].x = 519;
	space.button[49].y = 316;
	space.button[49].w = 84;
	space.button[49].h = 15;
	space.button[50].x = 614;
	space.button[50].y = 316;
	space.button[50].w = 84;
	space.button[50].h = 15;
	space.button[51].x = 91;
	space.button[51].y = 278;
	space.button[51].w = 33;
	space.button[51].h = 33;
	space.button[52].x = 215;
	space.button[52].y = 277;
	space.button[52].w = 165;
	space.button[52].h = 10;
	space.text_box[0].location.x = 458;
	space.text_box[0].location.y = 338;
	space.text_box[0].location.w = 242;
	space.text_box[0].location.h = 16;
	space.text_box[1].location.x = 150;
	space.text_box[1].location.y = 390;
	space.text_box[1].location.w = 222;
	space.text_box[1].location.h = 16;
	space.text_box[2].location.x = 332;
	space.text_box[2].location.y = 278;
	space.text_box[2].location.w = 42;
	space.text_box[2].location.h = 16;
	space.text_box[3].location.x = 615;
	space.text_box[3].location.y = 292;
	space.text_box[3].location.w = 85;
	space.text_box[3].location.h = 16;
	for ( i=0;i<4;i++ )
	{
		clear_text_box ( NULL, &space.text_box[i] );
		setup_text_box ( &space.text_box[i] );
	}
	set_text_box_displaybox ( &space.text_box[0],&space.display_box[0] );
	set_text_box_displaybox ( &space.text_box[1],&space.display_box[1] );
	space.text_box[0].selected = 1;
	space.text_box[1].selected = 0;
	space.text_box[2].selected = 0;
	space.text_box[3].selected = 0;
	space.text_box[0].max_char = 200;
	space.text_box[1].max_char = 200;
	space.text_box[2].max_char = 10;
	space.text_box[3].max_char = 10;
	space.text_box[0].pass_char = '\0';
	space.text_box[1].pass_char = '\0';
	space.text_box[2].pass_char = '\0';
	space.text_box[3].pass_char = '\0';
	space.display_box[0].x = 393;
	space.display_box[0].y = 359;
	space.display_box[0].w = 307;
	space.display_box[1].x = 88;
	space.display_box[1].y = 415;
	space.display_box[1].w = 285;
	space.display_box[0].lines = 12;
	space.display_box[1].lines = 8;
	setup_display_box ( &space.display_box[0] );
	setup_display_box ( &space.display_box[1] );
	space.user_label.location.x = 218;
	space.user_label.location.y = 356;
	space.user_label.location.w = 159;
	space.user_label.location.h = 15;
	space.user_label.message[0] = '\0';
	space.guild_label.location.x = 218;
	space.guild_label.location.y = 368;
	space.guild_label.location.w = 159;
	space.guild_label.location.h = 15;
	space.guild_label.message[0] = '\0';
	space.hull_label.location.x = 134;
	space.hull_label.location.y = 288;
	space.hull_label.location.w = 35;
	space.hull_label.location.h = 15;
	space.hull_label.message[0] = '\0';
	space.shield_label.location.x = 176;
	space.shield_label.location.y = 288;
	space.shield_label.location.w = 35;
	space.shield_label.location.h = 15;
	space.shield_label.message[0] = '\0';
	space.info_label.location.x = 425;
	space.info_label.location.y = 274;
	space.info_label.location.w = 275;
	space.info_label.location.h = 13;
	space.info_label.message[0] = '\0';
	space.info_label_mode = -1; //0-motion,1-user,2-transfer
	for ( i=0;i<5;i++ )
	{
		//destination.x = 216;
		//destination.y = 279;
		//left row
		space.good_label[i].location.x = 240;
		space.good_label[i].location.y = 297 + ( i * 18 );
		space.good_label[i].location.w = 55;
		space.good_label[i].location.h = 13;
		space.good_label[i].message[0] = '\0';

		//right row
		space.good_label[i+5].location.x = 320;
		space.good_label[i+5].location.y = 297 + ( i * 18 );
		space.good_label[i+5].location.w = 55;
		space.good_label[i+5].location.h = 13;
		space.good_label[i+5].message[0] = '\0';

		//now for those buttons
		space.button[i+69].x = 218;
		space.button[i+69].y = 296 + ( i * 18 );
		space.button[i+69].w = 20;
		space.button[i+69].h = 17;

		space.button[i+74].x = 300;
		space.button[i+74].y = 296 + ( i * 18 );
		space.button[i+74].w = 20;
		space.button[i+74].h = 17;
	}
	space.user_sel = -1;
	space.motion_sel = -1;
	space.view_cargo_goods = 1;
	space.view_cargo_sector = 0;
	space.viewing_cargo = 0;
	space.good_viewing = 0;
	space.show_ship = 1;
	space.jump_ok = 0;
	space.shield_level = 0;
	space.transferring = 0;
	space.engaging = -1;
	space.hit_enemy = 0;
	space.weapon_viewing = 0;
	space.was_sun = 0;
	space.thread = NULL;
	space.shield_disable_time = current_time() - SHIELD_DISABLE_TIME;
	space.visible = 0;
	setup_private_comms ( & ( space.display_box[1] ) ); //setup initial displaybox

	map.backround = NULL;
	map.visible = 0;
	for ( i=0;i<5;i++ )
	{
		map.button[i].w = 20;
		map.button[i].h = 20;
	}
	map.button[5].x = 618;
	map.button[5].y = 513;
	map.button[5].w = 100;
	map.button[5].h = 25;
	map.button[6].x = 615;
	map.button[6].y = 108;
	map.button[6].w = 20;
	map.button[6].h = 20;
	map.button[7].x = 695;
	map.button[7].y = 108;
	map.button[7].w = 20;
	map.button[7].h = 20;
	map.scan_label.location.x = 85;
	map.scan_label.location.y = 108;
	map.scan_label.location.w = 360;
	map.scan_label.location.h = 17;
	map.scan_label.message[0] = '\0';
	map.text_box.location.x = 248;
	map.text_box.location.y = 69;
	map.text_box.location.w = 195;
	map.text_box.location.h = 16;
	clear_text_box ( NULL, &map.text_box );
	setup_text_box ( &map.text_box );
	map.text_box.selected = 1;
	map.text_box.max_char = 20;
	map.text_box.pass_char = '\0';
	map.scan_last = SDL_CreateRGBSurface ( SDL_HWSURFACE, 38, 38, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff );

	death.backround = NULL;
	death.visible = 0;
	death.okay = 0;

	trade.backround = NULL;
	trade.visible = 0;
	trade.last_planet = -1;
	for ( i=0;i<10;i++ )
	{
		trade.button[i].x = 367 + i * 35;
		trade.button[i].y = 100;
		trade.button[i].w = 20;
		trade.button[i].h = 20;
	}
	trade.button[10].x = 83;
	trade.button[10].y = 105;
	trade.button[10].w = 50;
	trade.button[10].h = 13;
	trade.button[11].x = 162;
	trade.button[11].y = 104;
	trade.button[11].w = 128;
	trade.button[11].h = 14;
	trade.button[12].x = 655;
	trade.button[12].y = 516;
	trade.button[12].w = 55;
	trade.button[12].h = 16;
	for ( i=0;i<3;i++ )
	{
		trade.trade_label[i].message[0] = '\0';
		trade.trade_display[i].x = 363;
		trade.trade_display[i].w = 308;
	}
	trade.trade_label[0].location.x = 439;
	trade.trade_label[0].location.y = 162;
	trade.trade_label[0].location.w = 100;
	trade.trade_label[0].location.h = 20;
	trade.trade_label[1].location.x = 425;
	trade.trade_label[1].location.y = 300;
	trade.trade_label[1].location.w = 100;
	trade.trade_label[1].location.h = 20;
	trade.trade_label[2].location.x = 425;
	trade.trade_label[2].location.y = 415;
	trade.trade_label[2].location.w = 100;
	trade.trade_label[2].location.h = 20;
	trade.money_label.location.x = 168;
	trade.money_label.location.y = 397;
	trade.money_label.location.w = 149;
	trade.money_label.location.h = 20;
	trade.cargo_label.location.x = 228;
	trade.cargo_label.location.y = 432;
	trade.cargo_label.location.w = 91;
	trade.cargo_label.location.h = 20;
	trade.money_label.message[0] = '\0';
	trade.cargo_label.message[0] = '\0';
	trade.text_box[0].location.x = 438;
	trade.text_box[0].location.y = 377;
	trade.text_box[0].location.w = 92;
	trade.text_box[0].location.h = 20;
	trade.text_box[1].location.x = 438;
	trade.text_box[1].location.y = 495;
	trade.text_box[1].location.w = 92;
	trade.text_box[1].location.h = 20;
	trade.text_box[0].max_char = 10;
	trade.text_box[0].pass_char = '\0';
	trade.text_box[1].max_char = 10;
	trade.text_box[1].pass_char = '\0';
	for ( i=0;i<2;i++ )
	{
		clear_text_box ( NULL, &trade.text_box[i] );
		setup_text_box ( &trade.text_box[i] );
	}
	trade.text_box[0].selected = 1;
	trade.trade_display[0].y = 205;
	trade.trade_display[1].y = 335;
	trade.trade_display[2].y = 455;
	trade.trade_display[0].lines = 6;
	trade.trade_display[1].lines = 3;
	trade.trade_display[2].lines = 3;
	setup_display_box ( &trade.trade_display[0] );
	setup_display_box ( &trade.trade_display[1] );
	setup_display_box ( &trade.trade_display[2] );
	trade.news_disaster_display.lines = 17;
	trade.news_disaster_display.x = 363;
	trade.news_disaster_display.y = 90;
	trade.news_disaster_display.w = 330;
	setup_display_box ( &trade.news_disaster_display );
	struct label best_trade_planet_name_label[PLANET_TRADE_MAX];
	struct label best_trade_good_difference_label[PLANET_TRADE_MAX * 10];
	struct label best_trade_suggested_label[PLANET_TRADE_MAX];
	for ( i=0;i<PLANET_TRADE_MAX;i++ )
	{
		trade.best_trade_planet_name_label[i].location.x = 363;
		trade.best_trade_planet_name_label[i].location.y = 350 + ( i * 34 );
		trade.best_trade_planet_name_label[i].location.w = 330;
		trade.best_trade_planet_name_label[i].location.h = 15;

		trade.best_trade_suggested_label[i].location.x = 363;
		trade.best_trade_suggested_label[i].location.y = 365 + ( i * 34 );
		trade.best_trade_suggested_label[i].location.w = 330;
		trade.best_trade_suggested_label[i].location.h = 15;
	}
	trade.viewing_news = 1;

	guild_halls.backround = NULL;
	guild_halls.visible = 0;
	for ( i=1;i<5;i++ )
	{
		guild_halls.button[i].y = 344;
		guild_halls.button[i].h = 66;
	}
	guild_halls.button[0].x = 655;
	guild_halls.button[0].y = 515;
	guild_halls.button[0].w = 60;
	guild_halls.button[0].h = 25;
	guild_halls.button[1].x = 118;
	guild_halls.button[1].w = 56;
	guild_halls.button[2].x = 188;
	guild_halls.button[2].w = 100;
	guild_halls.button[3].x = 295;
	guild_halls.button[3].w = 100;
	guild_halls.button[4].x = 396;
	guild_halls.button[4].w = 72;

	guild_editor.backround = NULL;
	guild_editor.visible = 0;
	guild_editor.button[0].x = 530;
	guild_editor.button[0].y = 415;
	guild_editor.button[0].w = 63;
	guild_editor.button[0].h = 20;
	guild_editor.button[1].x = 217;
	guild_editor.button[1].y = 418;
	guild_editor.button[1].w = 326 - 217;
	guild_editor.button[1].h = 436 - 418;
	guild_editor.button[32].x = ( 400 - ( 480 / 2 ) ) + 180;
	guild_editor.button[32].y = ( 300 - ( 360 / 2 ) ) + 300;
	guild_editor.button[32].w = 285 - 180;
	guild_editor.button[32].h = 315 - 285;
	for ( i=0;i<2+6;i++ )
	{
		clear_text_box ( NULL, &guild_editor.text_box[i] );
		setup_text_box ( &guild_editor.text_box[i] );
		guild_editor.text_box[i].max_char = 21;
		guild_editor.text_box[i].pass_char = '\0';

		guild_editor.text_box[i].location.x = 225;
		guild_editor.text_box[i].location.y = 277 + ( ( i-2 ) * 23 );
		guild_editor.text_box[i].location.w = 318 - 225;
		guild_editor.text_box[i].location.h = 20;
	}
	for ( i=0;i<30;i++ )
	{
		int h, v;

		k = i + 2;

		h = i % 6;
		v = i / 6;

		guild_editor.button[k].x = 340 + ( v * 53 );
		guild_editor.button[k].y = 277 + ( ( 5-h ) * 23 );
		guild_editor.button[k].w = 20;
		guild_editor.button[k].h = 20;
	}
	guild_editor.text_box[0].location.x = 315;
	guild_editor.text_box[0].location.y = 210;
	guild_editor.text_box[0].location.w = 582 - 315;
	guild_editor.text_box[0].location.h = 20;
	guild_editor.text_box[0].max_char = 200;
	guild_editor.text_box[1].location.x = 353;
	guild_editor.text_box[1].location.y = 225;
	guild_editor.text_box[1].location.w = 582 - 353;
	guild_editor.text_box[1].location.h = 20;
	guild_editor.website[0] = 0;
	guild_editor.bank_money = 0;
	guild_editor.bank_lend = 0;
	for ( i=0;i<6;i++ )
	{
		guild_editor.rank_name[i][0] = 0;
		for ( k=0;k<5;k++ )
			guild_editor.rank_allow[i][k] = 0;
	}

	guild_editor_more.backround = NULL;
	guild_editor_more.visible = 0;
	guild_editor_more.button[0].x = 530;
	guild_editor_more.button[0].y = 415;
	guild_editor_more.button[0].w = 63;
	guild_editor_more.button[0].h = 20;
	guild_editor_more.button[1].x = 217;
	guild_editor_more.button[1].y = 418;
	guild_editor_more.button[1].w = 326 - 217;
	guild_editor_more.button[1].h = 436 - 418;
	guild_editor_more.button[2].x = 220;
	guild_editor_more.button[2].y = 296;
	guild_editor_more.button[2].w = 20;
	guild_editor_more.button[2].h = 20;
	guild_editor_more.button[3].x = 355;
	guild_editor_more.button[3].y = 296;
	guild_editor_more.button[3].w = 20;
	guild_editor_more.button[3].h = 20;
	guild_editor_more.button[4].x = 380;
	guild_editor_more.button[4].y = 296;
	guild_editor_more.button[4].w = 20;
	guild_editor_more.button[4].h = 20;
	guild_editor_more.button[5].x = 445;
	guild_editor_more.button[5].y = 295;
	guild_editor_more.button[5].w = 100;
	guild_editor_more.button[5].h = 20;
	for ( i=0;i<3;i++ )
	{
		clear_text_box ( NULL, &guild_editor_more.text_box[i] );
		setup_text_box ( &guild_editor_more.text_box[i] );
		guild_editor_more.text_box[i].max_char = 21;
		guild_editor_more.text_box[i].pass_char = '\0';

		guild_editor_more.text_box[i].location.x = ( 400 - ( 480 / 2 ) ) + 233;
		guild_editor_more.text_box[i].location.y = ( 300 - ( 360 / 2 ) ) + 90 + ( i*14 );
		guild_editor_more.text_box[i].location.w = 420 - 233;
		guild_editor_more.text_box[i].location.h = 20;
	}
	clear_text_box ( NULL, &guild_editor_more.text_box[3] );
	setup_text_box ( &guild_editor_more.text_box[3] );
	guild_editor_more.text_box[3].max_char = 31;
	guild_editor_more.text_box[3].pass_char = '\0';
	guild_editor_more.text_box[3].location.x = 357;
	guild_editor_more.text_box[3].location.y = 267;
	guild_editor_more.text_box[3].location.w = 582 - 357;
	guild_editor_more.text_box[3].location.h = 20;


	guild_bank.backround = NULL;
	guild_bank.visible = 0;
	guild_bank.button[0].x = 450;
	guild_bank.button[0].y = 367;
	guild_bank.button[0].w = 63;
	guild_bank.button[0].h = 20;
	guild_bank.button[1].x = 287;
	guild_bank.button[1].y = 367;
	guild_bank.button[1].w = 78;
	guild_bank.button[1].h = 20;
	guild_bank.button[2].x = 365;
	guild_bank.button[2].y = 367;
	guild_bank.button[2].w = 78;
	guild_bank.button[2].h = 20;
	guild_bank.money_label.location.x = 386;
	guild_bank.money_label.location.y = 295;
	guild_bank.money_label.location.w = 122;
	guild_bank.money_label.location.h = 20;
	guild_bank.money_label.message[0] = '\0';
	guild_bank.balance_label.location.x = 373;
	guild_bank.balance_label.location.y = 324;
	guild_bank.balance_label.location.w = 139;
	guild_bank.balance_label.location.h = 20;
	guild_bank.balance_label.message[0] = '\0';
	guild_bank.text_box.location.x = 373;
	guild_bank.text_box.location.y = 342;
	guild_bank.text_box.location.w = 139;
	guild_bank.text_box.location.h = 20;
	guild_bank.text_box.max_char = 20;
	guild_bank.text_box.pass_char = '\0';
	guild_bank.text_box.selected = 1;
	clear_text_box ( NULL, &guild_bank.text_box );
	setup_text_box ( &guild_bank.text_box );

	guild_register.backround = NULL;
	guild_register.visible = 0;
	guild_register.button[0].x = 450;
	guild_register.button[0].y = 367;
	guild_register.button[0].w = 63;
	guild_register.button[0].h = 20;
	guild_register.info_label.location.x = 288;
	guild_register.info_label.location.y = 340;
	guild_register.info_label.location.w = 226;
	guild_register.info_label.location.h = 20;
	guild_register.info_label.message[0] = '\0';
	guild_register.text_box.location.x = 296;
	guild_register.text_box.location.y = 365;
	guild_register.text_box.location.w = 156;
	guild_register.text_box.location.h = 20;
	guild_register.text_box.max_char = 20;
	guild_register.text_box.pass_char = '\0';
	guild_register.text_box.selected = 1;
	clear_text_box ( NULL, &guild_register.text_box );
	setup_text_box ( &guild_register.text_box );

	guild_listing.backround = NULL;
	guild_listing.visible = 0;
	guild_listing.button[0].x = 593;
	guild_listing.button[0].y = 494;
	guild_listing.button[0].w = 99;
	guild_listing.button[0].h = 28;
	guild_listing.button[1].x = 193;
	guild_listing.button[1].y = 446;
	guild_listing.button[1].w = 236 - 193;
	guild_listing.button[1].h = 462 - 446;
	guild_listing.button[2].x = 237;
	guild_listing.button[2].y = 446;
	guild_listing.button[2].w = 310 - 237;
	guild_listing.button[2].h = 462 - 446;
	guild_listing.button[3].x = 310;
	guild_listing.button[3].y = 446;
	guild_listing.button[3].w = 363 - 310;
	guild_listing.button[3].h = 462 - 446;
	guild_listing.button[4].x = 360;
	guild_listing.button[4].y = 64;
	guild_listing.button[4].w = 16;
	guild_listing.button[4].h = 125;
	guild_listing.button[5].x = 360;
	guild_listing.button[5].y = 64 + 125;
	guild_listing.button[5].w = 16;
	guild_listing.button[5].h = 79;
	guild_listing.guild_name_label.location.x = 510;
	guild_listing.guild_name_label.location.y = 119;
	guild_listing.guild_name_label.location.w = 717 - 510;
	guild_listing.guild_name_label.location.h = 20;
	guild_listing.guild_owner_label.location.x = 501;
	guild_listing.guild_owner_label.location.y = 137;
	guild_listing.guild_owner_label.location.w = 717 - 501;
	guild_listing.guild_owner_label.location.h = 20;
	guild_listing.guild_website_label.location.x = 512;
	guild_listing.guild_website_label.location.y = 155;
	guild_listing.guild_website_label.location.w = 717 - 512;
	guild_listing.guild_website_label.location.h = 20;
	guild_listing.guild_money_label.location.x = 536;
	guild_listing.guild_money_label.location.y = 173;
	guild_listing.guild_money_label.location.w = 717 - 536;
	guild_listing.guild_money_label.location.h = 20;
	guild_listing.guild_lend_label.location.x = 547;
	guild_listing.guild_lend_label.location.y = 191;
	guild_listing.guild_lend_label.location.w = 717 - 547;
	guild_listing.guild_lend_label.location.h = 20;
	guild_listing.guild_players_label.location.x = 543;
	guild_listing.guild_players_label.location.y = 209;
	guild_listing.guild_players_label.location.w = 717 - 543;
	guild_listing.guild_players_label.location.h = 20;
	guild_listing.guild_enemy_label[2].location.x = 506;
	guild_listing.guild_enemy_label[2].location.y = 229;
	guild_listing.guild_enemy_label[2].location.w = 717 - 506;
	guild_listing.guild_enemy_label[2].location.h = 20;
	guild_listing.guild_enemy_label[1].location.x = 506;
	guild_listing.guild_enemy_label[1].location.y = 247;
	guild_listing.guild_enemy_label[1].location.w = 717 - 506;
	guild_listing.guild_enemy_label[1].location.h = 20;
	guild_listing.guild_enemy_label[0].location.x = 506;
	guild_listing.guild_enemy_label[0].location.y = 265;
	guild_listing.guild_enemy_label[0].location.w = 717 - 506;
	guild_listing.guild_enemy_label[0].location.h = 20;
	guild_listing.guild_rank_label[0].location.x = 405;
	guild_listing.guild_rank_label[0].location.y = 316 + ( 23 * 5 );
	guild_listing.guild_rank_label[0].location.w = 480 - 405;
	guild_listing.guild_rank_label[0].location.h = 20;
	guild_listing.guild_rank_label[1].location.x = 405;
	guild_listing.guild_rank_label[1].location.y = 316 + ( 23 * 4 );
	guild_listing.guild_rank_label[1].location.w = 480 - 405;
	guild_listing.guild_rank_label[1].location.h = 20;
	guild_listing.guild_rank_label[2].location.x = 405;
	guild_listing.guild_rank_label[2].location.y = 316 + ( 23 * 3 );
	guild_listing.guild_rank_label[2].location.w = 480 - 405;
	guild_listing.guild_rank_label[2].location.h = 20;
	guild_listing.guild_rank_label[3].location.x = 405;
	guild_listing.guild_rank_label[3].location.y = 316 + ( 23 * 2 );
	guild_listing.guild_rank_label[3].location.w = 480 - 405;
	guild_listing.guild_rank_label[3].location.h = 20;
	guild_listing.guild_rank_label[4].location.x = 405;
	guild_listing.guild_rank_label[4].location.y = 316 + ( 23 * 1 );
	guild_listing.guild_rank_label[4].location.w = 480 - 405;
	guild_listing.guild_rank_label[4].location.h = 20;
	guild_listing.guild_rank_label[5].location.x = 405;
	guild_listing.guild_rank_label[5].location.y = 316 + ( 23 * 0 );
	guild_listing.guild_rank_label[5].location.w = 480 - 405;
	guild_listing.guild_rank_label[5].location.h = 20;
	guild_listing.text_box.location.x = 80;
	guild_listing.text_box.location.y = 480;
	guild_listing.text_box.location.w = 360 - 80;
	guild_listing.text_box.location.h = 20;
	guild_listing.text_box.max_char = 20;
	guild_listing.text_box.pass_char = '\0';
	guild_listing.text_box.selected = 1;
	clear_text_box ( NULL, &guild_listing.text_box );
	setup_text_box ( &guild_listing.text_box );
	guild_listing.max_entry = 0;
	for ( i=0;i<MAX_GUILD_LISTING;i++ )
	{
		const int y_int = 16;

		if ( i<HALF_MAX_GUILD_LISTING )
		{
			guild_listing.guild_listing_label[i].location.x = 95;
			guild_listing.guild_listing_label[i].location.y = 75 + ( y_int * i );
			guild_listing.guild_listing_label[i].location.w = ( 360 - 95 ) / 2;
			guild_listing.guild_listing_label[i].location.h = 15;
		}
		else
		{
			guild_listing.guild_listing_label[i].location.x = 95 + ( ( 360 - 95 ) / 2 );
			guild_listing.guild_listing_label[i].location.y = guild_listing.guild_listing_label[i-HALF_MAX_GUILD_LISTING].location.y;
			guild_listing.guild_listing_label[i].location.w = ( 360 - 95 ) / 2;
			guild_listing.guild_listing_label[i].location.h = 15;
		}

		//now for corresponding buttons
		guild_listing.button[6 + i] = guild_listing.guild_listing_label[i].location;
	}

	sat_editor.backround = NULL;
	sat_editor.visible = 0;
	sat_editor.button[0].x = 530;
	sat_editor.button[0].y = 415;
	sat_editor.button[0].w = 63;
	sat_editor.button[0].h = 20;
	for ( i=1;i<1 + ( SAT_LEVEL_MAX * SAT_GUN_INC );i++ )
	{
		sat_editor.button[i].x = 398 + ( ( ( i-1 ) %6 ) * 31 );
		sat_editor.button[i].y = 281 + ( ( ( i-1 ) /6 ) * 21 );
		sat_editor.button[i].w = 30;
		sat_editor.button[i].h = 20;
	}
	sat_editor.button[i+0].x = 301; //upgrade/purchase
	sat_editor.button[i+0].y = 269;
	sat_editor.button[i+0].w = 55;
	sat_editor.button[i+0].h = 16;
	sat_editor.button[i+1].x = 408; //view gun/missile but
	sat_editor.button[i+1].y = 266;
	sat_editor.button[i+1].w = 165;
	sat_editor.button[i+1].h = 9;
	sat_editor.button[i+2].x = 220;//left
	sat_editor.button[i+2].y = 291;
	sat_editor.button[i+2].w = 20;
	sat_editor.button[i+2].h = 20;
	sat_editor.button[i+3].x = 371;//right
	sat_editor.button[i+3].y = 291;
	sat_editor.button[i+3].w = 20;
	sat_editor.button[i+3].h = 20;
	sat_editor.button[i+4].x = 218;//buy/sell
	sat_editor.button[i+4].y = 378;
	sat_editor.button[i+4].w = 55;
	sat_editor.button[i+4].h = 16;
	sat_editor.button[i+5].x = 280;//repair/reload
	sat_editor.button[i+5].y = 378;
	sat_editor.button[i+5].w = 55;
	sat_editor.button[i+5].h = 16;
	sat_editor.button[i+6].x = 301; //bug
	sat_editor.button[i+6].y = 288;
	sat_editor.button[i+6].w = 55;
	sat_editor.button[i+6].h = 16;
	sat_editor.money_label.location.x = 400;
	sat_editor.money_label.location.y = 238;
	sat_editor.money_label.location.w = 460 - sat_editor.money_label.location.x;
	sat_editor.money_label.location.h = 20;
	sat_editor.current_level_label.location.x = 340;
	sat_editor.current_level_label.location.y = 393;
	sat_editor.current_level_label.location.w = 393 - sat_editor.current_level_label.location.x;
	sat_editor.current_level_label.location.h = 20;
	sat_editor.wep_name_label.location.x = 240;
	sat_editor.wep_name_label.location.y = 293;
	sat_editor.wep_name_label.location.w = 368 - sat_editor.wep_name_label.location.x;
	sat_editor.wep_name_label.location.h = 20;
	sat_editor.wep_accuracy_label.location.x = 292;
	sat_editor.wep_accuracy_label.location.y = 313;
	sat_editor.wep_accuracy_label.location.w = 393 - sat_editor.wep_accuracy_label.location.x;
	sat_editor.wep_accuracy_label.location.h = 20;
	sat_editor.wep_speed_label.location = sat_editor.wep_accuracy_label.location;
	sat_editor.wep_damage_label.location = sat_editor.wep_accuracy_label.location;
	sat_editor.wep_speed_label.location.y += 14;
	sat_editor.wep_damage_label.location.y += 28;
	sat_editor.annual_tax_label.location = sat_editor.current_level_label.location;
	sat_editor.taxes_collected_label.location = sat_editor.current_level_label.location;
	sat_editor.annual_tax_label.location.y += 14;
	sat_editor.taxes_collected_label.location.y += 28;
	sat_editor.text_box[0].location.x = 530;
	sat_editor.text_box[0].location.y = sat_editor.money_label.location.y;
	sat_editor.text_box[0].location.w = 581 - sat_editor.text_box[0].location.x;
	sat_editor.text_box[0].location.h = 20;
	clear_text_box ( NULL, &sat_editor.text_box[0] );
	setup_text_box ( &sat_editor.text_box[0] );
	sat_editor.text_box[0].max_char = 21;
	sat_editor.text_box[0].pass_char = '\0';

	planet_editor.backround = NULL;
	planet_editor.visible = 0;
	planet_editor.button[0].x = 530;
	planet_editor.button[0].y = 415;
	planet_editor.button[0].w = 63;
	planet_editor.button[0].h = 20;

	splash_screen.backround = NULL;
	splash_screen.visible = 0;

	cb_info.backround = NULL;
	cb_info.visible = 0;
	cb_info.button[0].x = 655;
	cb_info.button[0].y = 515;
	cb_info.button[0].w = 60;
	cb_info.button[0].h = 25;
	cb_info.button[1].x = 400;
	cb_info.button[1].y = 280;
	cb_info.button[1].w = 75;
	cb_info.button[1].h = 70;
	cb_info.button[2].x = 482;
	cb_info.button[2].y = 270;
	cb_info.button[2].w = 70;
	cb_info.button[2].h = 70;

	stat_viewer.backround = NULL;
	stat_viewer.visible = 0;
	stat_viewer.button[0].x = 322;
	stat_viewer.button[0].y = 101;
	stat_viewer.button[0].w = 151;
	stat_viewer.button[0].h = 16;
	stat_viewer.button[1].x = 84;
	stat_viewer.button[1].y = 148;
	stat_viewer.button[1].w = 80;
	stat_viewer.button[1].h = 13;
	stat_viewer.button[2].x = 171;
	stat_viewer.button[2].y = 148;
	stat_viewer.button[2].w = 135;
	stat_viewer.button[2].h = 13;
	stat_viewer.button[3].x = 312;
	stat_viewer.button[3].y = 148;
	stat_viewer.button[3].w = 66;
	stat_viewer.button[3].h = 13;
	stat_viewer.button[14].x = 405;
	stat_viewer.button[14].y = 420;
	stat_viewer.button[14].w = 20;
	stat_viewer.button[14].h = 20;
	stat_viewer.button[15].x = 540;
	stat_viewer.button[15].y = 420;
	stat_viewer.button[15].w = 20;
	stat_viewer.button[15].h = 20;
	stat_viewer.text_box.location.x = 180;
	stat_viewer.text_box.location.y = 104;
	stat_viewer.text_box.location.w = 120;
	stat_viewer.text_box.location.h = 20;
	stat_viewer.text_box.max_char = 20;
	stat_viewer.text_box.pass_char = '\0';
	stat_viewer.text_box.selected = 1;
	for ( i=0;i<10;i++ )
	{
		stat_viewer.list_name_label[i].location.x = 135;
		stat_viewer.list_name_label[i].location.y = 162 + ( i * 15 );
		stat_viewer.list_name_label[i].location.w = 100;
		stat_viewer.list_name_label[i].location.h = 20;
		stat_viewer.list_name_label[i].message[0] = '\0';

		stat_viewer.list_attr_label[i].location.x = 250;
		stat_viewer.list_attr_label[i].location.y = stat_viewer.list_name_label[i].location.y;
		stat_viewer.list_attr_label[i].location.w = 100;
		stat_viewer.list_attr_label[i].location.h = 20;
		stat_viewer.list_attr_label[i].message[0] = '\0';

		stat_viewer.list_class[i] = -1;

		//buttoonnnsss
		stat_viewer.button[i+4].x = 90;
		stat_viewer.button[i+4].y = stat_viewer.list_name_label[i].location.y;
		stat_viewer.button[i+4].w = 377 - 90;
		stat_viewer.button[i+4].h = 15;
	}
	stat_viewer.player_name_label.location.x = 560;
	stat_viewer.player_name_label.location.y = 167;
	stat_viewer.player_name_label.location.w = 90;
	stat_viewer.player_name_label.location.h = 20;
	stat_viewer.player_name_label.message[0] = '\0';
	stat_viewer.guild_name_label.location.x = 472;
	stat_viewer.guild_name_label.location.y = 182;
	stat_viewer.guild_name_label.location.w = 90;
	stat_viewer.guild_name_label.location.h = 20;
	stat_viewer.guild_name_label.message[0] = '\0';
	stat_viewer.guild_rank_label.location.x = 531;
	stat_viewer.guild_rank_label.location.y = 197;
	stat_viewer.guild_rank_label.location.w = 90;
	stat_viewer.guild_rank_label.location.h = 20;
	stat_viewer.guild_rank_label.message[0] = '\0';
	stat_viewer.player_worth_label.location.x = 571;
	stat_viewer.player_worth_label.location.y = 227;
	stat_viewer.player_worth_label.location.w = 90;
	stat_viewer.player_worth_label.location.h = 20;
	stat_viewer.player_worth_label.message[0] = '\0';
	stat_viewer.player_exp_label.location.x = 547;
	stat_viewer.player_exp_label.location.y = 242;
	stat_viewer.player_exp_label.location.w = 90;
	stat_viewer.player_exp_label.location.h = 20;
	stat_viewer.player_exp_label.message[0] = '\0';
	stat_viewer.player_kills_label.location.x = 560;
	stat_viewer.player_kills_label.location.y = 257;
	stat_viewer.player_kills_label.location.w = 90;
	stat_viewer.player_kills_label.location.h = 20;
	stat_viewer.player_kills_label.message[0] = '\0';
	stat_viewer.player_home_planet_label.location.x = 557;
	stat_viewer.player_home_planet_label.location.y = 287;
	stat_viewer.player_home_planet_label.location.w = 90;
	stat_viewer.player_home_planet_label.location.h = 20;
	stat_viewer.player_home_planet_label.message[0] = '\0';
	stat_viewer.player_home_server_label.location.x = 562;
	stat_viewer.player_home_server_label.location.y = 302;
	stat_viewer.player_home_server_label.location.w = 90;
	stat_viewer.player_home_server_label.location.h = 20;
	stat_viewer.player_home_server_label.message[0] = '\0';
	stat_viewer.player_last_server_label.location.x = 556;
	stat_viewer.player_last_server_label.location.y = 317;
	stat_viewer.player_last_server_label.location.w = 90;
	stat_viewer.player_last_server_label.location.h = 20;
	stat_viewer.player_last_server_label.message[0] = '\0';
	stat_viewer.player_online_label.location.x = 572;
	stat_viewer.player_online_label.location.y = 347;
	stat_viewer.player_online_label.location.w = 90;
	stat_viewer.player_online_label.location.h = 20;
	stat_viewer.player_online_label.message[0] = '\0';
	stat_viewer.player_actvated_label.location.x = 618;
	stat_viewer.player_actvated_label.location.y = 377;
	stat_viewer.player_actvated_label.location.w = 90;
	stat_viewer.player_actvated_label.location.h = 20;
	stat_viewer.player_actvated_label.message[0] = '\0';
	stat_viewer.deco_label.location.x = 555;
	stat_viewer.deco_label.location.y = 392;
	stat_viewer.deco_label.location.w = 710 - stat_viewer.deco_label.location.x;
	stat_viewer.deco_label.location.h = 20;
	stat_viewer.deco_label.message[0] = '\0';
	clear_text_box ( NULL, &stat_viewer.text_box );
	setup_text_box ( &stat_viewer.text_box );
	stat_viewer.thread = NULL;
	stat_viewer.s_id = -1;
	stat_viewer.s_type = -1;
	stat_viewer.s_kind = -1;
	stat_viewer.load_ship = 0;
	stat_viewer.player_class_points = 0;

	bounty.backround = NULL;
	bounty.visible = 0;
	bounty.button[0].x = 666;
	bounty.button[0].y = 500;
	bounty.button[0].w = 50;
	bounty.button[0].h = 20;
	bounty.button[1].x = 299;
	bounty.button[1].y = 500;
	bounty.button[1].w = 50;
	bounty.button[1].h = 20;
	for ( i=0;i<38;i++ )
	{
		const int left_x = 85, right_x = 418, interval_x1 = 180 - left_x, interval_x2 = 277 - left_x;
		const int start_y = 90, interval_y = 20;

		clear_text_box ( NULL, &bounty.text_box[i] );
		setup_text_box ( &bounty.text_box[i] );
		bounty.text_box[i].max_char = 21;
		bounty.text_box[i].pass_char = '\0';

		bounty.name_list_label[i].message[0] = 0;
		bounty.price_list_label[i].message[0] = 0;

		if ( i<19 )
		{
			bounty.name_list_label[i].location.x = left_x + 3;
			bounty.name_list_label[i].location.y = start_y + ( i * interval_y );
			bounty.name_list_label[i].location.w = interval_x1;
			bounty.name_list_label[i].location.h = interval_y;

			bounty.price_list_label[i].location.x = left_x + interval_x1;
			bounty.price_list_label[i].location.y = start_y + ( i * interval_y );
			bounty.price_list_label[i].location.w = interval_x1 - 6;
			bounty.price_list_label[i].location.h = interval_y;

			bounty.text_box[i].location.x = left_x + interval_x2 + 5;
			bounty.text_box[i].location.y = start_y + ( i * interval_y );
			bounty.text_box[i].location.w = interval_x1;
			bounty.text_box[i].location.h = interval_y;
		}
		else
		{
			bounty.name_list_label[i].location.x = right_x + 3;
			bounty.name_list_label[i].location.y = start_y + ( ( i-19 ) * interval_y );
			bounty.name_list_label[i].location.w = interval_x1;
			bounty.name_list_label[i].location.h = interval_y;

			bounty.price_list_label[i].location.x = right_x + interval_x1;
			bounty.price_list_label[i].location.y = start_y + ( ( i-19 ) * interval_y );
			bounty.price_list_label[i].location.w = interval_x1 - 6;
			bounty.price_list_label[i].location.h = interval_y;

			bounty.text_box[i].location.x = right_x + interval_x2 + 5;
			bounty.text_box[i].location.y = start_y + ( ( i-19 ) * interval_y );
			bounty.text_box[i].location.w = interval_x1;
			bounty.text_box[i].location.h = interval_y;
		}

	}
	for ( ;i<40;i++ )
	{
		clear_text_box ( NULL, &bounty.text_box[i] );
		setup_text_box ( &bounty.text_box[i] );
		bounty.text_box[i].location.x = 122;
		bounty.text_box[i].location.w = 155;
		bounty.text_box[i].location.h = 20;
		bounty.text_box[i].max_char = 21;
		bounty.text_box[i].pass_char = '\0';
	}
	bounty.text_box[38].selected = 1;
	bounty.text_box[38].location.y = 495;
	bounty.text_box[39].location.y = 525;
	bounty.money_label.location.x = 567;
	bounty.money_label.location.y = 496;
	bounty.money_label.location.w = 78;
	bounty.money_label.location.h = 20;
	bounty.money_label.message[0] = '\0';
	bounty.status_label.location.x = 375;
	bounty.status_label.location.y = 498;
	bounty.status_label.location.w = 107;
	bounty.status_label.location.h = 20;
	bounty.status_label.message[0] = '\0';
	bounty.list_max = 0;

	planet_sat.backround = NULL;
	planet_sat.planetbut = NULL;
	planet_sat.visible = 0;
	planet_sat.button[0].x = 612;
	planet_sat.button[0].y = 498;
	planet_sat.button[0].w = 130;
	planet_sat.button[0].h = 30;
	for ( i=1;i<3;i++ )
	{
		planet_sat.button[i].x = 576;
		planet_sat.button[i].y = 99 + ( i-1 ) * 20;
		planet_sat.button[i].w = 140;
		planet_sat.button[i].h = 20;
	}

	planet_guild.backround = NULL;
	planet_guild.visible = 0;
	planet_guild.button[0].x = 450;
	planet_guild.button[0].y = 367;
	planet_guild.button[0].w = 63;
	planet_guild.button[0].h = 20;
	planet_guild.button[1].x = 290;
	planet_guild.button[1].y = 368;
	planet_guild.button[1].w = 386 - 290;
	planet_guild.button[1].h = 383 - 368;
	planet_guild.guild_name_label.location.x = 400;
	planet_guild.guild_name_label.location.y = 258;
	planet_guild.guild_name_label.location.w = 511 - planet_guild.guild_name_label.location.x;
	planet_guild.guild_name_label.location.h = 20;
	planet_guild.entry_cost_label.location = planet_guild.guild_name_label.location;
	planet_guild.money_label.location = planet_guild.guild_name_label.location;
	planet_guild.your_guild_name_label.location = planet_guild.guild_name_label.location;
	planet_guild.entry_cost_label.location.y += 17;
	planet_guild.money_label.location.y += 34;
	planet_guild.your_guild_name_label.location.y += 51;

	switch_server.backround = NULL;
	switch_server.visible = 0;
	switch_server.button[0].x = 530; //exit
	switch_server.button[0].y = 415;
	switch_server.button[0].w = 63;
	switch_server.button[0].h = 20;
	switch_server.button[1].x = 428; //left
	switch_server.button[1].y = 419;
	switch_server.button[1].w = 20;
	switch_server.button[1].h = 20;
	switch_server.button[2].x = 493; //right
	switch_server.button[2].y = 419;
	switch_server.button[2].w = 20;
	switch_server.button[2].h = 20;
	switch_server.info_label.location.x = 220;
	switch_server.info_label.location.y = 215;
	switch_server.info_label.location.w = 580 - switch_server.info_label.location.x;
	switch_server.info_label.location.h = 20;
	switch_server.server_max = 0;
	for ( i=0;i<SWITCH_SERVER_PAGE_DISPLAY;i++ )
	{
		switch_server.server_name_label[i].location.x = 220;
		switch_server.server_name_label[i].location.y = 270 + ( i*20 );
		switch_server.server_name_label[i].location.w = 340 - switch_server.server_name_label[i].location.x;
		switch_server.server_name_label[i].location.h = 20;

		switch_server.server_ip_label[i].location.x = 345;
		switch_server.server_ip_label[i].location.y = 270 + ( i*20 );
		switch_server.server_ip_label[i].location.w = 465 - switch_server.server_ip_label[i].location.x;
		switch_server.server_ip_label[i].location.h = 20;

		switch_server.server_players_label[i].location.x = 470;
		switch_server.server_players_label[i].location.y = 270 + ( i*20 );
		switch_server.server_players_label[i].location.w = 545 - switch_server.server_players_label[i].location.x;
		switch_server.server_players_label[i].location.h = 20;

		switch_server.server_ping_label[i].location.x = 540;
		switch_server.server_ping_label[i].location.y = 270 + ( i*20 );
		switch_server.server_ping_label[i].location.w = 580 - switch_server.server_ping_label[i].location.x;
		switch_server.server_ping_label[i].location.h = 20;

		switch_server.button[3 + i].x = 220; //right
		switch_server.button[3 + i].y = 270 + ( i*20 );
		switch_server.button[3 + i].w = 580 - switch_server.button[3 + i].x;
		switch_server.button[3 + i].h = 20;
	}
	
	purchase_plans.backround = NULL;
	purchase_plans.visible = 0;
	purchase_plans.button[0].x = 530; //exit
	purchase_plans.button[0].y = 415;
	purchase_plans.button[0].w = 63;
	purchase_plans.button[0].h = 20;
	purchase_plans.button[1].x = 219; //buy
	purchase_plans.button[1].y = 292;
	purchase_plans.button[1].w = 45;
	purchase_plans.button[1].h = 20;
	purchase_plans.cost_label.location.x = 293;
	purchase_plans.cost_label.location.y = 237;
	purchase_plans.cost_label.location.w = 373 - 293;
	purchase_plans.cost_label.location.h = 20;
	purchase_plans.exp_label.location = purchase_plans.cost_label.location;
	purchase_plans.kills_label.location = purchase_plans.cost_label.location;
	purchase_plans.exp_label.location.y += 17;
	purchase_plans.kills_label.location.y += 17 * 2;
}
