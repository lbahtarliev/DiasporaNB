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

void init_memory();
void main_loop();

int main(int argc, char *argv[])
{
	if (!init_sdl()) return 0;
	init_memory();
	init_audio();
	load_splash_screen();
	load_login();
	read_host_info();
	read_config_info();
	load_ignore();
	load_sos();
	//connect_to_hub();
	init_sockets();
	main_loop();
	return 0;
}

void main_loop()
{
	int do_exit = 0;
	
	while(!do_exit)
	{
		handle_sockets();
		do_exit += handle_sdl_events();
		space_threadless();
		
#ifdef WIN32
		Sleep(1);
#else
		usleep(1000);
#endif
	}
}

void init_memory()
{
	int i, j, k, s, sh;
	
	init_form_memory();
	set_trade_disaster_texts();
	clear_private_chat_entries();
	clear_plan_list();
	
	game.zone = 0;
	game.sector = 0;
	
	game.last_ldm[0] = 0;
	
	//start randomizer
	srand(time(0));
	
	for(i=0;i<10;i++)
	{
		game.ship[i].server_id = -1;
		game.ship[i].type = -1;
		game.ship[i].kind = -1;
		
		for(k=0;k<16;k++)
		{
			game.ship[i].gun[k] = -1;
			game.ship[i].gun_id[k] = -1;
			game.ship[i].missile[k] = -1;
			game.ship[i].missile_id[k] = -1;
			game.ship[i].misc[k] = -1;
			game.ship[i].misc_id[k] = -1;
		}
	}
	
	//nif images
	game.nif_image.no_gun_pod = NULL;
	game.nif_image.no_missile_pod = NULL;
	game.nif_image.no_planet_img = NULL;
	game.nif_image.no_repair_img = NULL;
	game.nif_image.no_shop_img = NULL;
	game.nif_image.no_loc_img = NULL;
	game.nif_image.no_loc_safe_img = NULL;
	game.nif_image.no_loc_p_img = NULL;
	game.nif_image.no_hyper_p_img = NULL;
	
	//game images
	for(i=0;i<10;i++)
	{
		game.game_image.mini_good[i] = NULL;
		game.game_image.mini_contra[i] = NULL;
	}
	
	for(i=0;i<MAX_DECO;i++)
		game.game_image.deco_img[i] = NULL;
	
	for(i=0;i<EXP_RANK_IMG_MAX;i++)
		game.game_image.exp_rank[i] = NULL;
	
	game.game_image.guild_toggle_off = NULL;
	game.game_image.guild_toggle_on = NULL;
	game.game_image.fship_sat = NULL;
	game.game_image.fship_sat_destroyed = NULL;
	
	//ship images
	//server[server_id].ship[ship_type].fship_img[ship_kind][ship_face]
	for(s=0;s<MAX_SERVER;s++)
	{
		for(sh=0;sh<SHIP_MAX;sh++)
		{
			/*
			SDL_Surface *ship_img[8][16];
			SDL_Surface *fship_img[8][5];
			SDL_Surface *fship_sos_img[8][5];
			SDL_Surface *space_img[8];
			SDL_Surface *rad_img[8];
			SDL_Surface *plan_img;
			*/
			
			server[s].ship[sh].plan_img = NULL;
			
			for(i=0;i<8;i++)
				for(j=0;j<16;j++)
					server[s].ship[sh].ship_img[i][j] = NULL;
			
			for(i=0;i<8;i++)
				for(j=0;j<5;j++)
			{
				server[s].ship[sh].fship_img[i][j] = NULL;
				server[s].ship[sh].fship_sos_img[i][j] = NULL;
			}
			
			for(i=0;i<8;i++)
			{
				server[s].ship[sh].space_img[i] = NULL;
				server[s].ship[sh].rad_img[i] = NULL;
			}
		}
	}
}
