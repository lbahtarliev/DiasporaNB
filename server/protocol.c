#include "main.h"

extern void process_packet_hub(int dummy, char *message)
{
	char temp_str[4][1025];
	int point = 0, idt;
	
	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)
		
	split(temp_str[0],message,',',&point);
	idt = atoi(temp_str[0]);
	
	if (idt != 2) printf("hub-rcv:%s\n",message);
	
	switch(idt)
	{
		case 1: //ask version	
			break;
		case 2: //raw text
			right(temp_str[0],message,point);
			printf("%s\n", temp_str[0]);
			break;
		case 3: //user info
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			right(temp_str[2],message,point);
			handle_user_login(atoi(temp_str[0]), atoi(temp_str[1]), temp_str[2]);
			break;
		case 4: //planet good prices aka pricechange
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			set_good_prices(atoi(temp_str[0]), temp_str[1]);
			bot_clear_all_bot_goods(); //ONLY HERE UNTIL BETTER SOLUTION!
			set_all_smuggler_prices();
			break;
		case 5: //planet good ammounts
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			set_good_ammounts(atoi(temp_str[0]), temp_str[1]);
			break;
		case 6: //user ship info
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			set_user_ship(temp_str[0],temp_str[1]);
			break;
		case 7: //ship mod update
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			set_user_ship_mod(temp_str[0],temp_str[1]);
			break;
		case 8: //send user to planet
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			send_user_to_planet(temp_str[0], atoi(temp_str[1]));
			break;
		case 9: //user hull
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			set_user_hull(temp_str[0], atoi(temp_str[1]));
			break;
		case 10: //reload user missile
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			reload_user_missile(temp_str[0], atoi(temp_str[1]), atoi(temp_str[2]));
			break;
		case 11: //set reference number
			split(temp_str[0],message,',',&point);
			server_info.reference_number = atoi(temp_str[0]);
			break;
		case 12: //update good ammount
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			update_good_ammount(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]));
			break;
		case 13: //update user guild
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			split(temp_str[3],message,',',&point);
			update_user_guild(temp_str[0],temp_str[1],temp_str[2],atoi(temp_str[3]));
			break;
		case 14: //update sector good amount 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			split(temp_str[3],message,',',&point);
			update_sector_good(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]), atoi(temp_str[3]));
			break;
		case 15: //update sector contra amount 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			split(temp_str[3],message,',',&point);
			update_sector_contra(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]), atoi(temp_str[3]));
			break;
		case 16: //update sector goods amount 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			right(temp_str[2],message,point);
			hub_update_sector_goods(atoi(temp_str[0]), atoi(temp_str[1]), temp_str[2]);
			break;
		case 17: //update sector contras amount 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			right(temp_str[2],message,point);
			hub_update_sector_contras(atoi(temp_str[0]), atoi(temp_str[1]), temp_str[2]);
			break;
		case 18: //request all sector info
			hub_request_sector_info();
			break;
		case 19: //give a bot some goods 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			split(temp_str[3],message,',',&point);
			bot_add_good(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]), atoi(temp_str[3]));
			break;
		case 20: //give a bot some goods 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			split(temp_str[3],message,',',&point);
			bot_add_contra(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]), atoi(temp_str[3]));
			break;
		case 21: //log user off 
			split(temp_str[0],message,',',&point);
			hub_force_user_offline(temp_str[0]);
			break;
		case 22: //relay user exp and exp addition 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			hub_relay_user_exp(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]));
			break;
		case 23: //planet sat info
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			hub_set_sat_info(atoi(temp_str[0]), temp_str[1]);
			break;
		case 24: //repair planet sat
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			repair_sat(atoi(temp_str[0]),atoi(temp_str[1]));
			break;
		case 25: //set sat money collected
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			hub_set_sat_money_collected(atoi(temp_str[0]), atoi(temp_str[1]));
			break;
		case 26: //set sat hull
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			hub_set_sat_hull(atoi(temp_str[0]), atoi(temp_str[1]));
			break;
		case 27: //set sat money
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			hub_set_sat_money(atoi(temp_str[0]), atoi(temp_str[1]));
			break;
		case 28: //repair user gun
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			hub_repair_user_gun(temp_str[0], atoi(temp_str[1]));
			break;
		case 29: //set user has goods
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			hub_set_player_has_goods(atoi(temp_str[0]), atoi(temp_str[1]));
			break;
		case 30: //clear out planet trade disasters
			hub_clear_out_planet_trade_disasters();
			break;
		case 31: //set planet trade disaster
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			split(temp_str[3],message,',',&point);
			hub_add_planet_trade_disaster(atoi(temp_str[0]), atoi(temp_str[1]), atoi(temp_str[2]), atoi(temp_str[3]));
			break;
		case 32: //set user activated
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			hub_set_user_activated(atoi(temp_str[0]), atoi(temp_str[1]));
			break;
		case 33: //hub ping request
			hub_ping_request();
			break;
		case 34: //hub ping response
			hub_ping_response();
			break;
		case 35: //announce sat money deposit
            split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			split(temp_str[2],message,',',&point);
			hub_annouce_sat_deposit(atoi(temp_str[0]), temp_str[1], atoi(temp_str[2]));
		  break;
	}
}

extern void process_packet_user(int con, char *message)
{
	char temp_str[3][1025], rtn_str[500];
	int point = 0, idt;
	
	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)
	
	split(temp_str[0],message,',',&point);
	idt = atoi(temp_str[0]);
	
	printf("usr-rcv-%d::%s\n", con, message);
	
	if(!user_con[con].username[0])
	{
		switch(idt)
		{
			case 1: //request login
				if (user_con[con].user_id) return;
				right(temp_str[0],message,point);
				user_con[con].user_id = atoi(temp_str[0]);
				sprintf(rtn_str,"3,%d,%d,%ld", con, user_con[con].user_id, get_user_s_addr(con));
				send_con_hub(rtn_str);
				break;
		}
	}
	else
	{
		//bot death
		if(user_con[con].destroy_on_recv)
			user_do_destroy_on_recv(con);
		
		switch(idt)
		{
		case 2: //Enter Bar
			user_enter_bar(con);
			break;
		case 3: //Leave Bar
			user_leave_bar(con);
			break;
		case 4: //Standard Chat
			right(temp_str[0],message,point);
			user_chat(con,temp_str[0]);
			break;
		case 5: //Special Chat
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			user_special_chat(con, atoi(temp_str[0]), temp_str[1]);
			break;
		case 6: //Launch
			split(temp_str[0],message,',',&point);
			launch_user(con, atoi(temp_str[0]));
			break;
		case 7: //Land
			land_user(con);
			break;
		case 8: //Goto Sector
			split(temp_str[0],message,',',&point);
			move_to_sector(con, atoi(temp_str[0]));
			break;
		case 9: //attack user
			right(temp_str[0],message,point);
			attack_user(con,temp_str[0]);
			break;
		case 10: //disengage
			disengage_user(con);
			break;
		case 11: //set shield
			split(temp_str[0],message,',',&point);
			set_user_shield(con,  atoi(temp_str[0]));
			break;
		case 12: //fire missile
			split(temp_str[0],message,',',&point);
			fire_missile(con, atoi(temp_str[0]));
			break;
		case 13: //scan user
			split(temp_str[0],message,',',&point);
			scan_user(con,temp_str[0]);
			break;
		case 14: //LDM
			split(temp_str[0],message,',',&point);
			right(temp_str[1],message,point);
			user_relay_ldm(con, temp_str[0], temp_str[1]);
			break;
		case 15: //pick up good 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			user_pick_up_sector_good(con,atoi(temp_str[0]),atoi(temp_str[1]));
			break;
		case 16: //pick up contra 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			user_pick_up_sector_contra(con,atoi(temp_str[0]),atoi(temp_str[1]));
			break;
		case 17: //drop up good 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			user_drop_sector_good(con,atoi(temp_str[0]),atoi(temp_str[1]));
			break;
		case 18: //drop up contra 
			split(temp_str[0],message,',',&point);
			split(temp_str[1],message,',',&point);
			user_drop_sector_contra(con,atoi(temp_str[0]),atoi(temp_str[1]));
			break;
		case 19: //ask planet good prices
			split(temp_str[0],message,',',&point);
			user_request_goodprices(con, temp_str[0]);
			break;
		case 20: //user ping response
			user_ping_response(con);
			break;
		}
	}
}
