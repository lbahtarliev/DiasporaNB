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

void process_packet_hub(char *message)
{
        char temp_str[4][1025];
	int point = 0, idt;
	
	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)
		
	printf("rcv-h-%s\n",message);
	
	split(temp_str[0],message,',',&point);
	idt = atoi(temp_str[0]);
	
	switch(idt)
	{
	case 1: //encryption	
		break;
	case 2: //text
		right(temp_str[0],message,point);
		write_login_message(temp_str[0]);
		break;
	case 3: //user_id
		right(temp_str[0],message,point);
		game.user_id = atoi(temp_str[0]);
		break;
	case 4: //connect straight to server
		break;
	case 5: //load server list
		right(temp_str[0],message,point);
		load_server_list(temp_str[0]);
		break;
	case 6: //store full server list
		right(temp_str[0],message,point);
		read_full_server_list(temp_str[0]);
		break;
	case 7: //home planet info
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		game.home_id = atoi(temp_str[0]);
		game.home_planet = atoi(temp_str[1]);
		break;
	case 8: //full ship info
		right(temp_str[0],message,point);
		set_ship_info(temp_str[0]);
		break;
	case 9: //cargo info
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		set_cargo_info(atoi(temp_str[0]),temp_str[1]);
		break;
	case 10: //ship mod
		right(temp_str[0],message,point);
		set_ship_mod(temp_str[0]);
		break;
	case 11: //hanger info
		right(temp_str[0],message,point);
		read_hanger_info(temp_str[0]);
		break;
	case 12: //user info
		right(temp_str[0],message,point);
		read_user_info(temp_str[0]);
		break;
	case 13: //user money
		right(temp_str[0],message,point);
		set_user_money(atoi(temp_str[0]));
		break;
	case 14: //hanger ship update
		right(temp_str[0],message,point);
		update_hanger_info(temp_str[0]);
		break;
	case 15: //update hull
		right(temp_str[0],message,point);
		set_hull(atoi(temp_str[0]));
		break;
	case 16: //fix gun
		split(temp_str[0],message,',',&point);
		fix_gun(atoi(temp_str[0]));
		break;
	case 17: //restock missiles
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		restock_missile(atoi(temp_str[0]),atoi(temp_str[1]));
		break;
	case 18: //gun info
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		read_gun_entry(atoi(temp_str[0]), temp_str[1]);
		break;
	case 19: //missile info
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		read_missile_entry(atoi(temp_str[0]), temp_str[1]);
		break;
	case 20: //update good info
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		update_ship_good_ammount(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]));
		break;
	case 21: //full guild info
		right(temp_str[0],message,point);
		update_guild_info(temp_str[0]);
		break;
	case 22: //guild list entry
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		split(temp_str[3],message,',',&point);
		add_guild_listing(atoi(temp_str[0]),temp_str[1],atoi(temp_str[2]),atoi(temp_str[3]));
		break;
	case 23: //other's guild info
		right(temp_str[0],message,point);
		display_guild_info(temp_str[0]);
		break;
	case 24: //guild money info
		split(temp_str[0],message,',',&point);
		set_guild_money(atoi(temp_str[0]));
		break;
	case 25: //guild balance
		split(temp_str[0],message,',',&point);
		set_guild_balance(atoi(temp_str[0]));
		break;
	case 26: //update contra info
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		update_ship_contra_ammount(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]));
		break;
	case 27: //update exp amount
		split(temp_str[0],message,',',&point);
		update_user_exp(atoi(temp_str[0]));
		break;
	case 28: //retrive user stat info
		right(temp_str[0],message,point);
		hub_retrive_user_stats(temp_str[0]);
		break;
	case 29: //retrive user stat list
		right(temp_str[0],message,point);
		hub_retrive_stats_list(temp_str[0]);
		break;
	case 30: //bounty list
		right(temp_str[0],message,point);
		bounty_load_list(temp_str[0]);
		break;
 	case 31: //standard chat
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		add_chat(atoi(temp_str[0]), temp_str[1]);
		break;
	case 32: //private chat
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		add_private_chat(atoi(temp_str[0]), temp_str[1]);
		break;
	case 33: //redraw guild listings
		guild_listing_draw_list();
		sdl_flip_mutex();
		break;
	case 34: //private_chat_add_edit_entry
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		private_chat_add_edit_entry(temp_str[0], atoi(temp_str[1]));
		break;
	case 35: //private chat remove
		split(temp_str[0],message,',',&point);
		private_chat_remove_entry(temp_str[0]);
		break;
	case 36: //whole private chat list
		right(temp_str[0],message,point);
		private_chat_load_entry_list(temp_str[0]);
        	break;
	case 37: //is_activated
		split(temp_str[0],message,',',&point);
		hub_set_is_activated(atoi(temp_str[0]));
		break;
	case 38: //update guild deco
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		hub_set_guild_deco(atoi(temp_str[0]), temp_str[1], atoi(temp_str[2]));
		break;
	case 39: //receive hub ping request
		hub_ping_request();
		break;
	case 40: //plan info
		right(temp_str[0],message,point);
		hub_process_plan_list(temp_str[0]);
		break;
	}
}

void process_packet_server(char *message)
{
	char temp_str[4][1025];
	int point = 0, idt;
	
	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)
		
	printf("rcv-s-%s\n",message);
	
	split(temp_str[0],message,',',&point);
	idt = atoi(temp_str[0]);
	
	switch(idt)
	{
	case 1: //planet info
		right(temp_str[0],message,point);
		read_planet_entry(temp_str[0]);
		break;
	case 2: //galaxy info
		right(temp_str[0],message,point);
		read_galaxy_entry(temp_str[0]);
		break;
	case 3: //gun info
		right(temp_str[0],message,point);
		read_gun_entry(game.server_id, temp_str[0]);
		break;
	case 4: //missile info
		right(temp_str[0],message,point);
		read_missile_entry(game.server_id, temp_str[0]);
		break;
	case 5: //ship info
		right(temp_str[0],message,point);
		read_ship_entry(temp_str[0]);
		break;
	case 6: //load planet
		split(temp_str[0],message,',',&point);
		clear_display_box(NULL,&space.display_box[0]); //best placed here...
		setup_public_comms(&space.display_box[0]);
		load_planet(atoi(temp_str[0]));
		break;
	case 7: //full bar info
		right(temp_str[0],message,point);
		read_bar_info(temp_str[0]);
		break;
	case 8: //add user to bar
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		split(temp_str[3],message,',',&point);
		add_bar_entry(temp_str[0], temp_str[1], temp_str[2], atoi(temp_str[3]));
		break;
	case 9: //remove user from bar
		split(temp_str[0],message,',',&point);
		remove_bar_entry(temp_str[0]);
		break;
	case 10: //standard chat
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		add_chat(atoi(temp_str[0]), temp_str[1]);
		break;
	case 11: //private chat
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		add_private_chat(atoi(temp_str[0]), temp_str[1]);
		break;
	case 12: //sector info
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		right(temp_str[2],message,point);
		load_sector(atoi(temp_str[0]), atoi(temp_str[1]), temp_str[2]);
		break;
	case 13: //add user to sector
		right(temp_str[0],message,point);
		add_to_sector(temp_str[0]);
		break;
	case 14: //remove user from sector
		split(temp_str[0],message,',',&point);
		remove_from_sector(temp_str[0]);
		break;
	case 15: //destroy user
		right(temp_str[0],message,point);
		destroy_user(temp_str[0]);
		break;
	case 16: //hull % of attackie
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		set_enemy_hull(atoi(temp_str[0]),atoi(temp_str[1]));
		break;
	case 17: //New Hull
		right(temp_str[0],message,point);
		set_hull(atoi(temp_str[0]));
		break;
	case 18: //death window
		load_death();
		break;
	case 19: //attacking us
		split(temp_str[0],message,',',&point);
		set_user_engage_us(temp_str[0]);
		break;
	case 20: //attacking other
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		set_user_engage(temp_str[0],temp_str[1]);
		break;
	case 21: //disengaging
		split(temp_str[0],message,',',&point);
		set_user_disengage(temp_str[0]);
		break;
	case 22: //scan
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		map_set_scan(atoi(temp_str[0]), temp_str[1]);
		sdl_flip_mutex();
	case 23: //set good prices
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		set_good_prices(game.server_id, atoi(temp_str[0]), temp_str[1]);
	case 24: //set good ammounts
		split(temp_str[0],message,',',&point);
		right(temp_str[1],message,point);
		set_good_ammounts(game.server_id, atoi(temp_str[0]), temp_str[1]);
	case 25: //update good ammount
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		update_planet_good_ammount(game.server_id,atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]));
		break;
	case 26: //recieve hit from...
		split(temp_str[0],message,',',&point);
		space_user_hit_us(temp_str[0]);
	case 27: //set sector good amounts
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		right(temp_str[2],message,point);
		set_sector_goods(atoi(temp_str[0]),atoi(temp_str[1]),temp_str[2]);
		break;
	case 28: //set sector contra amounts
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		right(temp_str[2],message,point);
		set_sector_contras(atoi(temp_str[0]),atoi(temp_str[1]),temp_str[2]);
		break;
	case 29: //set sector single good amount
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		split(temp_str[3],message,',',&point);
		set_sector_good(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]),atoi(temp_str[3]));
		break;
	case 30: //set sector single contra amount
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		split(temp_str[3],message,',',&point);
		set_sector_contra(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]),atoi(temp_str[3]));
		break;
	case 31: //retrieve sat info
		right(temp_str[0],message,point);
		server_set_sat_info(temp_str[0]);
		break;
	case 32: //set sat hull
		split(temp_str[0],message,',',&point);
		server_set_sat_hull(atoi(temp_str[0]));
		break;
	case 33: //set sat money
		split(temp_str[0],message,',',&point);
		server_set_sat_money(atoi(temp_str[0]));
		break;
	case 34: //destroy gun
		split(temp_str[0],message,',',&point);
		server_set_gun_destroyed(atoi(temp_str[0]));
		break;
	case 35: //clear planet trade disasters
		server_clear_out_planet_trade_disasters();
		break;
	case 36: //add planet trade disaster
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		split(temp_str[3],message,',',&point);
		server_add_planet_trade_disaster(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]),atoi(temp_str[3]));
		break;
	case 37: //set enemy shield level
		split(temp_str[0],message,',',&point);
		set_victom_shield_level(atoi(temp_str[0]));
		break;
	case 38: //dock jump bar
		dock_jump_bar();
		break;
	case 39: //disable shields
		disable_shields();
		break;
	case 40: //set enemy jump status
		split(temp_str[0],message,',',&point);
		set_victom_jump_status(atoi(temp_str[0]));
		break;
	case 41: //show enemy missile fire
		right(temp_str[0],message,point);
		show_enemy_missile(temp_str[0]);
		break;
	case 42: //server ping request
		server_ping_request();
		break;
	case 43: //set sector has players
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		server_set_sector_has_player(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]));
		break;
	case 44: //set sector has guild members
		split(temp_str[0],message,',',&point);
		split(temp_str[1],message,',',&point);
		split(temp_str[2],message,',',&point);
		server_set_sector_has_guild_member(atoi(temp_str[0]),atoi(temp_str[1]),atoi(temp_str[2]));
		break;
	case 45: //clear all sectors has guild members
		server_clear_has_guild_member_sectors();
		break;
	}
}

