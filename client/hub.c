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

void connect_hub_process()
{
	char rtn_str[10];
	sprintf(rtn_str,"1,%d",VERSION);
	send_con_hub(rtn_str);
}

void disconnect_hub_process()
{
	if(!login.visible && !switch_server.visible)
	{
		char message[200] = "... Warning : Disconnected from the main hub server, please log off and reconnect ..";
		
		add_private_chat(1,message);
	}
}

void write_login_message(char *message)
{
		if (login.visible)
		{ 
			draw_label(screen, message, &login.label, 255, 255, 255);
			//draw_label(login.backround, message, &login.label, 255, 255, 255);
			sdl_flip_mutex();
		}
		if (create_char.visible)
		{
			draw_label(screen, message, &create_char.label, 255, 255, 255);
			//draw_label(create_char.backround, message, &create_char.label, 255, 255, 255);
			sdl_flip_mutex();
		}
		if (guild_register.visible)
		{
			draw_center_label(screen, message, &guild_register.info_label, 15, 116, 102);
			sdl_flip_mutex();
		}
		if(bounty.visible)
		{
             draw_center_label(screen, message, &bounty.status_label, 107, 216, 210);
             sdl_flip_mutex();
        }
}

void load_server_list(char *list)
{
	int i, point, length;
	char temp_str[3][2000];
	
	length = strlen(list);

	for(i=point=0;point < length && i < 15;i++)
	{
		split(temp_str[0],list,',',&point);
		split(server_select.servername[i],list,',',&point);
		split(server_select.server_ip[i],list,',',&point);
		split(temp_str[1],list,',',&point);
		split(temp_str[2],list,',',&point);
		server_select.server_id[i]   = atoi(temp_str[0]);
		server_select.server_x[i] = atoi(temp_str[1]) + 95; //set it into the screen some
		server_select.server_y[i] = atoi(temp_str[2]) + 110;
		server_select.newlist = 1;
	}
	
	for(;i<15;i++)
	{
		server_select.server_x[i] = 0;
		server_select.server_y[i] = 0;
	}

	load_server_select();

}

void read_full_server_list(char *list)
{
	int i, point, length;

	length = strlen(list);
	
	for(i=point=0;point < length && i < MAX_SERVER;i++)
		split(server[i].servername, list, ',', &point);
}

void set_ship_info(char *message)
{
	char temp_str[1025];
	int i, point = 0;
	
	//ship_selected,ship_id,ship_brand,ship_kind,exp,hull,hull_base,shields,range,speed,evasion,cargo,max_guns,max_missiles,max_misc,gun0_id,gun0_num,missile0_id,missile0_num,misc0_number
	
	split(temp_str,message,',',&point);
	game.ship_sel = atoi(temp_str);
	
	if (game.ship_sel < 0) game.ship_sel = 0; //check if hub is fucking around
	if (game.ship_sel > 9) game.ship_sel = 9;
	
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].server_id = atoi(temp_str);
	
	if (game.ship[game.ship_sel].server_id < -1) 			game.ship[game.ship_sel].server_id = -1; //check if hub is fucking around
	if (game.ship[game.ship_sel].server_id > MAX_SERVER - 1) 	game.ship[game.ship_sel].server_id = -1;
	
	if (game.ship[game.ship_sel].server_id == -1) //welp you have no ship
	{
		if (hanger.visible) // but that is no reason to not draw the selection box.
		{
			redraw_hanger();
			SDL_BlitSurface(hanger.temp_backround, NULL, screen, NULL);
			sdl_flip_mutex();
		}
		else
		{
			hanger.redraw = 1;
		}
		
		if (shop.visible)
		{
			clear_sell_shop();
			SDL_BlitSurface(shop.temp_backround, NULL, screen, NULL);
			sdl_flip_mutex();
		}
		
		clear_ship_info(game.ship_sel);
		
		return;
	}
	
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].type = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].kind = atoi(temp_str);
	split(temp_str,message,',',&point);
	temp_str[20] = 0;
	strcpy(server[game.ship[game.ship_sel].server_id].ship[game.ship[game.ship_sel].type].name, temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].cost = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].exp = atoi(temp_str);
	split(temp_str,message,',',&point);
	set_hull(atoi(temp_str));
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].hull_base = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].shield = atoi(temp_str);
	game.ship[game.ship_sel].shield_base = game.ship[game.ship_sel].shield;
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].range = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].speed = atoi(temp_str);
	game.ship[game.ship_sel].speed_base = game.ship[game.ship_sel].speed;
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].evasion = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].cargo = atoi(temp_str);
	game.ship[game.ship_sel].cargo_base = game.ship[game.ship_sel].cargo;
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].gun_ammount = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].missile_ammount = atoi(temp_str);
	split(temp_str,message,',',&point);
	game.ship[game.ship_sel].misc_ammount = atoi(temp_str);
	
	if (game.ship[game.ship_sel].type < -1) 		game.ship[game.ship_sel].type = -1; //check if hub is fucking around
	if (game.ship[game.ship_sel].type > SHIP_MAX - 1) 	game.ship[game.ship_sel].type = -1;
	if (game.ship[game.ship_sel].kind < -1) 		game.ship[game.ship_sel].kind = -1;
	if (game.ship[game.ship_sel].kind > 7) 			game.ship[game.ship_sel].kind = -1;
	
	if (game.ship[game.ship_sel].gun_ammount > 16) 		game.ship[game.ship_sel].gun_ammount = 16;
	if (game.ship[game.ship_sel].missile_ammount > 16) 	game.ship[game.ship_sel].missile_ammount = 16;
	if (game.ship[game.ship_sel].misc_ammount > 16) 	game.ship[game.ship_sel].misc_ammount = 16;
	
	
	for(i=0;i<game.ship[game.ship_sel].gun_ammount;i++)
	{
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].gun_id[i] = atoi(temp_str);
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].gun[i] = atoi(temp_str);
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].gun_destroyed[i] = atoi(temp_str);
		
		if (game.ship[game.ship_sel].gun_id[i] < -1) 			game.ship[game.ship_sel].gun_id[i] = -1; //check if hub is fucking around
		if (game.ship[game.ship_sel].gun_id[i] > MAX_SERVER - 1) 	game.ship[game.ship_sel].gun_id[i] = -1;
		
		if (game.ship[game.ship_sel].gun[i] < -1) 		game.ship[game.ship_sel].gun[i] = -1; //check if hub is fucking around
		if (game.ship[game.ship_sel].gun[i] > GUN_MAX - 1) 	game.ship[game.ship_sel].gun[i] = -1;
	}
	for(;i<16;i++) //clear the rest
	{
        game.ship[game.ship_sel].gun_id[i] = -1;
        game.ship[game.ship_sel].gun[i] = -1;          
    }
	
	for(i=0;i<game.ship[game.ship_sel].missile_ammount;i++)
	{
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].missile_id[i] = atoi(temp_str);
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].missile[i] = atoi(temp_str);
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].missile_left[i] = atoi(temp_str);
		
		if (game.ship[game.ship_sel].missile_id[i] < -1) 		game.ship[game.ship_sel].missile_id[i] = -1; //check if hub is fucking around
		if (game.ship[game.ship_sel].missile_id[i] > MAX_SERVER - 1) 	game.ship[game.ship_sel].missile_id[i] = -1;
		
		if (game.ship[game.ship_sel].missile[i] < -1) 		game.ship[game.ship_sel].missile[i] = -1; //check if hub is fucking around
		if (game.ship[game.ship_sel].missile[i] > MISSILE_MAX - 1) 	game.ship[game.ship_sel].missile[i] = -1;
	}
	for(;i<16;i++) //clear the rest
	{
        game.ship[game.ship_sel].missile_id[i] = -1;
        game.ship[game.ship_sel].missile[i] = -1;          
    }
	
	for(i=0;i<game.ship[game.ship_sel].misc_ammount;i++)
	{
		split(temp_str,message,',',&point);
		game.ship[game.ship_sel].misc[i] = atoi(temp_str);
		
		if (game.ship[game.ship_sel].misc[i] < -1) 		game.ship[game.ship_sel].misc[i] = -1; //check if hub is fucking around printf("blah\n");
	}
	for(;i<16;i++) //clear the rest
	{
        game.ship[game.ship_sel].misc[i] = -1;          
    }
	
	set_user_misc(); //make changes to hull/shield/etc based on misc pods
	set_average_gun_speed(); //in case you plan on useing them guns
	set_missile_speed_times(); //in case you plan on useing them missiles
	repair.redraw_weapons = 1; //see em in the repair shop
	space.shield_level = 0; //reset shield every ship change
	
	if(shop.visible) //well... they gotta now they got it right?
	{
		view_sell_shop();
		SDL_BlitSurface(shop.temp_backround, NULL, screen, NULL);
	}
	
	if (hanger.visible)
	{
		redraw_hanger();
		SDL_BlitSurface(hanger.temp_backround, NULL, screen, NULL);
		sdl_flip_mutex();
	}
	else
	{
		hanger.redraw = 1;
	}
	
	//shittt might want to know
    space_set_hit_actual_max();
}

void set_cargo_info(int sh, char *message)
{
	int i, point = 0;;
	char temp_str[1025];
	
	hanger.ship[sh].cargo = 0;
	
	for(i=0;i<10;i++)
	{
		split(temp_str,message,',',&point);
		game.ship[sh].good_ammount[i] = atoi(temp_str);
		
		hanger.ship[sh].cargo += game.ship[sh].good_ammount[i];
	}
}

void set_ship_mod(char *message)
{
	int mod_type, slot_number, misc_id, misc_value, misc_left, misc_destroyed, point = 0;
	char temp_str[1025];
	
	//mod_type,slot_number,gun/miss/misc_id,gun/miss/misc_value
	
	split(temp_str,message,',',&point);
	mod_type = atoi(temp_str);
	split(temp_str,message,',',&point);
	slot_number = atoi(temp_str);
	
	if (slot_number < 0 || slot_number > 15) return; //wtf garbaled number
	
	switch (mod_type)
	{
	case 0://gun
		split(temp_str,message,',',&point);
		misc_id = atoi(temp_str);
		split(temp_str,message,',',&point);
		misc_value = atoi(temp_str);
		split(temp_str,message,',',&point);
		misc_destroyed = atoi(temp_str);
		
		if ((misc_id < 0 || misc_id > MAX_SERVER -1) || (misc_value < 0 || misc_value > GUN_MAX -1)) //check for fuck arounds...
		    {misc_id = -1; misc_value = -1; misc_destroyed = 0;}
		
		game.ship[game.ship_sel].gun_id[slot_number] = misc_id;
		game.ship[game.ship_sel].gun[slot_number] = misc_value;
		game.ship[game.ship_sel].gun_destroyed[slot_number] = misc_destroyed;
		
		set_average_gun_speed(); //in case you plan on useing them guns
		repair.redraw_weapons = 1; //see em in the repair shop
		update_shop_sell(0, slot_number);
		
		//shittt might want to know
        space_set_hit_actual_max();
		break;
	case 1://missile
		split(temp_str,message,',',&point);
		misc_id = atoi(temp_str);
		split(temp_str,message,',',&point);
		misc_value = atoi(temp_str);
		split(temp_str,message,',',&point);
		misc_left = atoi(temp_str);
		
		if ((misc_id < 0 || misc_id > MAX_SERVER -1) || (misc_value < 0 || misc_value > MISSILE_MAX -1))
		    {misc_id = -1; misc_value = -1; misc_left = 0;}
		
		game.ship[game.ship_sel].missile_id[slot_number] = misc_id;
		game.ship[game.ship_sel].missile[slot_number] = misc_value;
		game.ship[game.ship_sel].missile_left[slot_number] = misc_left;
		
		repair.redraw_weapons = 1; //see em in the repair shop
		
		//set the speed for this missile for use in space
		if (misc_id != -1 && misc_value != -1)
		{
			space.missile_speed[slot_number] = 100 - server[misc_id].missile[misc_value].speed;
			space.missile_speed[slot_number] *= .1;
		}
		
		update_shop_sell(1, slot_number);
		break;
	case 2://misc
		split(temp_str,message,',',&point);
		misc_value = atoi(temp_str);

		if (misc_value < -1 || misc_value > 7) 			misc_value = -1; //check for fuck arounds...
		
		game.ship[game.ship_sel].misc[slot_number] = misc_value;
		
		set_user_misc(); //make changes to hull/shield/etc based on misc pods
		
		update_shop_sell(2, slot_number);
		break;
	}
;
}

void read_hanger_info(char *message)
{
	int i, point = 0;
	char temp_str[1025];
	
	//cycle through ships and input data
	for(i=0;i<10;i++)
	{
		//ship_id,ship_brand,ship_kind,ship_name,max_hull,hull,max_cargo,cargo,max_speed,range,max_shields,has_weapons0-1
		split(temp_str,message,',',&point);
		hanger.ship[i].server_id = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].type = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].kind = atoi(temp_str);
		
		split(temp_str,message,',',&point);
		temp_str[20] = '\0'; //cap that sucker
		strcpy(hanger.ship[i].name,temp_str);
		
		split(temp_str,message,',',&point);
		hanger.ship[i].hull_max = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].hull = atoi(temp_str);
		
		split(temp_str,message,',',&point);
		hanger.ship[i].cargo_max = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].cargo = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].speed = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].range = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].shield = atoi(temp_str);
		split(temp_str,message,',',&point);
		hanger.ship[i].has_weapons = atoi(temp_str);
		
		//check for indecencies
		if (hanger.ship[i].server_id<0 || hanger.ship[i].server_id>MAX_SERVER - 1) 	hanger.ship[i].server_id = -1;
		if (hanger.ship[i].type<0 || hanger.ship[i].type>SHIP_MAX - 1) 			hanger.ship[i].type = -1;
		if (hanger.ship[i].kind<0 || hanger.ship[i].kind>7) 				hanger.ship[i].kind = -1;
		
		if (hanger.ship[i].server_id<0 || hanger.ship[i].server_id>MAX_SERVER - 1) 	continue;
		if (hanger.ship[i].type<0 || hanger.ship[i].type>SHIP_MAX - 1) 			continue;
		if (hanger.ship[i].kind<0 || hanger.ship[i].kind>7) 				continue;
		
		//well then if it is good, don't mind if we do
		strcpy(server[hanger.ship[i].server_id].ship[hanger.ship[i].type].name, hanger.ship[i].name);
	}
	
	if (hanger.visible)
	{
		redraw_hanger();
		sdl_flip_mutex();
	}
	else
	{
		hanger.redraw = 1;
	}
	
	
}

void update_hanger_info(char *message)
{
	int i, point = 0;
	char temp_str[1025];
	
	//cycle through ships and input data
		//ship_id,ship_brand,ship_kind,ship_name,max_hull,hull,max_cargo,cargo,max_speed,range,max_shields,has_weapons0-1
	split(temp_str,message,',',&point);
	i = atoi(temp_str);
	
	if (i<0 || i>9) return; //invalid entry?
	
	split(temp_str,message,',',&point);
	hanger.ship[i].server_id = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].type = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].kind = atoi(temp_str);
	split(temp_str,message,',',&point);
	temp_str[20] = '\0'; //cap that sucker
	strcpy(hanger.ship[i].name,temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].hull_max = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].hull = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].cargo_max = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].cargo = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].speed = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].range = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].shield = atoi(temp_str);
	split(temp_str,message,',',&point);
	hanger.ship[i].has_weapons = atoi(temp_str);
	
	//check for indecencies
	if (hanger.ship[i].server_id<0 || hanger.ship[i].server_id>MAX_SERVER - 1) 	hanger.ship[i].server_id = -1;
	if (hanger.ship[i].type<0 || hanger.ship[i].type>SHIP_MAX - 1) 			hanger.ship[i].type = -1;
	if (hanger.ship[i].kind<0 || hanger.ship[i].kind>7) 				hanger.ship[i].kind = -1;

	
	if (hanger.visible)
	{
		redraw_hanger();
		sdl_flip_mutex();
	}
	else
	{
		hanger.redraw = 1;
	}
}

void read_user_info(char *message)
{
	int point = 0;
	char temp_str[1025];

    //username
	split(temp_str, message, ',', &point);
    temp_str[20] = 0;
    strcpy(game.username, temp_str);
    
    //guild name
    split(temp_str, message, ',', &point);
    temp_str[20] = 0;
    strcpy(game.guild.name, temp_str);

    //guild rank number
	split(temp_str, message, ',', &point);
	game.guild_rank_id = atoi(temp_str);
	
	//oooo
	if(planet_guild.visible)
	{
         //set
         draw_right_label(screen, game.guild.name, &planet_guild.your_guild_name_label, 107, 216, 210);                   
    }
    
    jabber_connect("hestia.nighsoft.net",to_jabber_name(temp_str,game.username),game.password);
}

void set_user_money(int money)
{
	char temp_str[32];
	game.money = money;
	
	if (shop.visible)
	{
		sprintf(temp_str,"%d",game.money);
		draw_right_label(shop.temp_backround, temp_str, &shop.user_money_label, 255, 255, 255);
		SDL_BlitSurface(shop.temp_backround, NULL, screen, NULL);
		//sdl_flip_mutex(); <- this creates bad lag and isn't needed
	}
	
	if(repair.visible)
	{
		sprintf(temp_str,"%d", game.money);
		draw_right_label(screen, temp_str, &repair.money_label, 82, 177, 188);
		//sdl_flip_mutex(); <- this creates bad lag and isn't needed
	}
	
	if(trade.visible)
	{
		sprintf(temp_str,"%d", game.money);
		draw_right_label(screen, temp_str, &trade.money_label, 107, 216, 210);
		sdl_flip_mutex();
	}
	
	if(guild_bank.visible)
	{
		sprintf(temp_str,"%d",game.money);
		draw_center_label(screen, temp_str, &guild_bank.money_label, 15, 116, 102);
		sdl_flip_mutex();
	}

    if(bounty.visible)
    {
		sprintf(temp_str,"%d",game.money);
        draw_right_label(screen, temp_str, &bounty.money_label, 107, 216, 210);                  
		sdl_flip_mutex();
    }
    
    if(planet_guild.visible)
    {
		sprintf(temp_str,"%d",game.money);
        draw_right_label(screen, temp_str, &planet_guild.money_label, 107, 216, 210);                  
		sdl_flip_mutex();
    }
}

void purchase_ship(int ship_type, int ship_kind)
{
	char rtn_str[20];
	
	sprintf(rtn_str, "4,0,%d,%d", ship_type, ship_kind);
	send_con_hub(rtn_str);
}

void purchase_ship_mod(int mod_type, int mod_number)
{
	char rtn_str[20];
	
	sprintf(rtn_str, "4,%d,%d", mod_type, mod_number);
	send_con_hub(rtn_str);
}

void sell_ship(int ship_selected)
{
	char rtn_str[20];
	
	sprintf(rtn_str, "12,0,%d", ship_selected);
	send_con_hub(rtn_str);
}

void sell_ship_mod(int mod_type, int mod_selected)
{
	char rtn_str[20];
	
	sprintf(rtn_str, "12,%d,%d", mod_type, mod_selected);
	send_con_hub(rtn_str);
}

void select_ship(int new_selection)
{
	char rtn_str[20];
	
	sprintf(rtn_str, "5,%d", new_selection);
	send_con_hub(rtn_str);
}

void fix_gun(int gun_number)
{
	if (gun_number < 0) gun_number = 0;
	if (gun_number >= 16) gun_number = 15;
	
	game.ship[game.ship_sel].gun_destroyed[gun_number] = 0;
	
	if(repair.visible)
		SDL_FillRect(screen, &repair.button[gun_number + 2], SDL_MapRGB(screen->format, 0, 0, 0));
		
	//shit might want to know
	space_set_hit_actual_max();
	
	set_average_gun_speed(); //in case you plan on useing them guns
}

void restock_missile(int missile_number, int payload)
{
	if (missile_number < 0) missile_number = 0;
	if (missile_number >= 16) missile_number = 15;
	
	if (payload < 0) payload = 0;
	if (payload > 4) payload = 4;
	
	game.ship[game.ship_sel].missile_left[missile_number] = payload;
	
	if(repair.visible && payload == 4)
		SDL_FillRect(screen, &repair.button[missile_number + 18], SDL_MapRGB(screen->format, 0, 0, 0));
}

void update_ship_good_ammount(int sh, int g, int ammount)
{
	game.ship[sh].good_ammount[g] = ammount;
	
	hanger.ship[sh].cargo = ship_total_good_ammount(sh);
	
	hanger.redraw = 1;
	
	if(sh == game.ship_sel)
	{
		if(trade.visible && !trade.info_visible)
			trade_update_player_good_ammount();
		
		if(space_redraw_good_number_if_required(g,0))
			sdl_flip_mutex();
	}
}

void update_ship_contra_ammount(int sh, int g, int ammount)
{
	game.ship[sh].contra_ammount[g] = ammount;
	
	hanger.ship[sh].cargo = ship_total_good_ammount(sh);
	
	hanger.redraw = 1;
	
	if(sh == game.ship_sel)
	{
		if(space_redraw_good_number_if_required(g,0))
			sdl_flip_mutex();
	}
}

void update_guild_info(char *message)
{
	const int input_amount = 20 + (MAX_DECO * 2);
	char temp_str[input_amount][1030];
	int i, k, j, point;
	struct guild_db current_guild;
	
	i = strlen(message);
	
	for(k=0;k<input_amount;k++)
		temp_str[k][0] = '\0';
	for(point=k=0;point<=i && k<input_amount;k++)
		split(temp_str[k],message,',',&point);
				
	strcpy(current_guild.name,temp_str[0]);
	strcpy(current_guild.owner,temp_str[1]);
	strcpy(current_guild.website,temp_str[2]);
	current_guild.bank_money = atoi(temp_str[3]);
	current_guild.bank_lend = atoi(temp_str[4]);
	strcpy(current_guild.rank_name[0],temp_str[5]);
	strcpy(current_guild.rank_name[1],temp_str[6]);
	strcpy(current_guild.rank_name[2],temp_str[7]);
	strcpy(current_guild.rank_name[3],temp_str[8]);
	strcpy(current_guild.rank_name[4],temp_str[9]);
	strcpy(current_guild.rank_name[5],temp_str[10]);
	strcpy(current_guild.enemy[0],temp_str[11]);
	strcpy(current_guild.enemy[1],temp_str[12]);
	strcpy(current_guild.enemy[2],temp_str[13]);
	for(k=0;k<5;k++) current_guild.rank_allow[0][k] = temp_str[14][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[1][k] = temp_str[15][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[2][k] = temp_str[16][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[3][k] = temp_str[17][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[4][k] = temp_str[18][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[5][k] = temp_str[19][k] - '0';
	
	for(k=0,j=19;k<MAX_DECO;k++)
	{
		j++;
		temp_str[j][30] = 0;
		strcpy(current_guild.deco_name[k], temp_str[j]);
		j++;
		current_guild.deco_activated[k] = !(!atoi(temp_str[j]));
	}

	//k...
    strcpy(game.guild.name, current_guild.name);
	strcpy(game.guild.owner, current_guild.owner);
	strcpy(game.guild.website, current_guild.website);
	game.guild.bank_money = current_guild.bank_money;
	game.guild.bank_lend = current_guild.bank_lend;
	strcpy(game.guild.enemy[0], current_guild.enemy[0]);
	strcpy(game.guild.enemy[1], current_guild.enemy[1]);
	strcpy(game.guild.enemy[2], current_guild.enemy[2]);
	for(i=0;i<6;i++)
	{
	    strcpy(game.guild.rank_name[i], current_guild.rank_name[i]);
	    
	    for(k=0;k<5;k++)
	        game.guild.rank_allow[i][k] = current_guild.rank_allow[i][k];
	}
	for(k=0;k<MAX_DECO;k++)
	{
		strcpy(game.guild.deco_name[k], current_guild.deco_name[k]);
		game.guild.deco_activated[k] = current_guild.deco_activated[k];
	}
}

void display_guild_info(char *message)
{
	char temp_str[21][1030];
	int i, k, point;
	struct guild_db current_guild;
	
	i = strlen(message);
	
	for(k=0;k<21;k++)
		temp_str[k][0] = '\0';
	for(point=k=0;point<=i && k<21;k++)
		split(temp_str[k],message,',',&point);
				
	strcpy(current_guild.name,temp_str[0]);
	strcpy(current_guild.owner,temp_str[1]);
	strcpy(current_guild.website,temp_str[2]);
	current_guild.bank_money = atoi(temp_str[3]);
	current_guild.bank_lend = atoi(temp_str[4]);
	strcpy(current_guild.rank_name[0],temp_str[5]);
	strcpy(current_guild.rank_name[1],temp_str[6]);
	strcpy(current_guild.rank_name[2],temp_str[7]);
	strcpy(current_guild.rank_name[3],temp_str[8]);
	strcpy(current_guild.rank_name[4],temp_str[9]);
	strcpy(current_guild.rank_name[5],temp_str[10]);
	strcpy(current_guild.enemy[0],temp_str[11]);
	strcpy(current_guild.enemy[1],temp_str[12]);
	strcpy(current_guild.enemy[2],temp_str[13]);
	for(k=0;k<5;k++) current_guild.rank_allow[0][k] = temp_str[14][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[1][k] = temp_str[15][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[2][k] = temp_str[16][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[3][k] = temp_str[17][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[4][k] = temp_str[18][k] - '0';
	for(k=0;k<5;k++) current_guild.rank_allow[5][k] = temp_str[19][k] - '0';
	
	//do this color stuff
    guild_listing_select_new_guild(current_guild.name);
	
	//oookkkk
	strcpy(guild_listing.selected_guild_name, current_guild.name);
	strcpy(guild_listing.owner_name, current_guild.owner);
	strcpy(guild_listing.website, current_guild.website);
	guild_listing.bank_money = current_guild.bank_money;
    guild_listing.bank_lend =	current_guild.bank_lend;
	strcpy(guild_listing.rank_name[0], current_guild.rank_name[0]);
	strcpy(guild_listing.rank_name[1], current_guild.rank_name[1]);
	strcpy(guild_listing.rank_name[2], current_guild.rank_name[2]);
	strcpy(guild_listing.rank_name[3], current_guild.rank_name[3]);
	strcpy(guild_listing.rank_name[4], current_guild.rank_name[4]);
	strcpy(guild_listing.rank_name[5], current_guild.rank_name[5]);
	strcpy(guild_listing.guild_enemy[0], current_guild.enemy[0]);
	strcpy(guild_listing.guild_enemy[1], current_guild.enemy[1]);
	strcpy(guild_listing.guild_enemy[2], current_guild.enemy[2]);
	guild_listing.players = atoi(temp_str[20]);
	
	for(i=0;i<6;i++)
	    for(k=0;k<5;k++)
	        guild_listing.rank_allow[i][k] = current_guild.rank_allow[i][k];
	
	//draw it now
	guild_listing_draw_guild();
	
	//flippy
	sdl_flip_mutex();
	
}

void set_guild_balance(int ammount)
{
	char money_str[500];
	
	game.guild_balance = ammount;
	
	if(guild_bank.visible)
	{
		sprintf(money_str,"%d",game.guild_balance);
		draw_label(screen, money_str, &guild_bank.balance_label, 15, 116, 102);
		sdl_flip_mutex();
	}
}

void set_guild_money(int ammount)
{
	game.guild.bank_money = ammount;
}

void add_guild_listing(int i, char *entry_name, int guild_worth, int guild_players)
{
	//make sure all this businuess is right
	if(i<0) return;
	if(i>=MAX_GUILD) return;
	
	//cap the monkey
	entry_name[20] = 0;
	
	strcpy(guild_listing.guild_name[i],entry_name);
	guild_listing.guild_worth[i] = guild_worth;
	guild_listing.guild_players[i] = guild_players;
	
	if(i >= guild_listing.max_entry) guild_listing.max_entry = i+1;
}

void update_user_exp(int exp)
{
	//update the exp
	game.exp = exp;
}

void hub_set_is_activated(int it_is)
{
	game.activated = it_is;
}

void hub_set_guild_deco(int d_i, char *deco_name, int deco_activated)
{
	//checkin
	if(d_i < 0) return;
	if(d_i >= MAX_DECO) return;
	deco_name[30] = 0;
	if(deco_activated) deco_activated = 1;
	
	//settin
	strcpy(game.guild.deco_name[d_i], deco_name);
	game.guild.deco_activated[d_i] = deco_activated;
	
	//viewin
	if(guild_editor_more.visible && d_i == guild_editor_more.deco_choosen)
	{
		guild_editor_more_draw_deco(guild_editor_more.deco_choosen);
		
		sdl_flip_mutex();
	}
}

void hub_send_ask_pricechange()
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"53,");
	send_con_hub(rtn_str);
}

void hub_send_utoggle()
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"55,");
	send_con_hub(rtn_str);
}

void hub_send_dsamount()
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"56,");
	send_con_hub(rtn_str);
}

void hub_send_transferds(char *playername, int amount)
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"57,%s,%d", playername, amount);
	send_con_hub(rtn_str);
}

void hub_send_buyactivation(char *playername)
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"58,%s", playername);
	send_con_hub(rtn_str);
}

void hub_send_buyexp(char *playername, int amount)
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"59,%s,%d", playername, amount);
	send_con_hub(rtn_str);
}

void hub_send_buydc(char *playername, int amount)
{
	char rtn_str[2000];
	
	sprintf(rtn_str,"60,%s,%d", playername, amount);
	send_con_hub(rtn_str);
}

void hub_ping_request()
{
	char send_str[20] = "61,";
	//61
	
	send_con_hub(send_str);
}

void hub_process_plan_list(char *the_list)
{
	int point = 0;
	char temp_str[2][1024];
	int len;
	
	//clear first
	clear_plan_list();
	
	len = strlen(the_list);
	
	while(point < len)
	{
		int s, pl;
		
		split(temp_str[0],the_list,',',&point);
		split(temp_str[1],the_list,',',&point);
		
		s = atoi(temp_str[0]);
		pl = atoi(temp_str[1]);
		
		game.has_plan[s][pl] = 1;
	}
	
	//now to do a draw?
	if(purchase_plans.visible)
	{
		int s, p, the_plan;
		
		s = game.server_id;
		p = planet.last_planet;
		the_plan = server[s].planet[p].ship_plan;
		
		if(game.has_plan[s][the_plan])
		{
			SDL_FillRect(screen, &purchase_plans.button[1], SDL_MapRGB(screen->format, 0, 0, 0));
			sdl_flip_mutex();
		}
		
	}
}

void hub_send_lease_login(char *lease_login, char *lease_password)
{
	char send_str[2000];
	//63
	
	sprintf(send_str, "63,%s,%s", lease_login, lease_password);
	
	send_con_hub(send_str);
}
