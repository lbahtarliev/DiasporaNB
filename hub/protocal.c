#include "main.h"

void process_packet_admin ( int con,char* message )
{
	char temp_str[3][1025];
	int point = 0, idt;

	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)

	split ( temp_str[0],message,',',&point );
	idt = atoi ( temp_str[0] );

	printf ( "adm-rcv-%d:%s\n",con,message );

	switch ( idt )
	{
		case 1: //login
			split ( temp_str[0],message,',',&point );
			split ( temp_str[1],message,',',&point );
			admin_login ( con,temp_str[0],temp_str[1] );
			break;
		case 2: //shell command
			if ( admin_con_id[con] == -1 ) break; //if no one is logged on...
			right ( temp_str[0],message,point );
			admin_shell ( con,temp_str[0] );
			break;
	}
}

void process_packet_user ( int con,char* message )
{
	char rtn_str[1060], temp_str[4][1025];
	int point = 0, idt, temp_int;

	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)

	printf ( "usr-rcv-%d:%s\n",con,message );

	split ( temp_str[0],message,',',&point );
	idt = atoi ( temp_str[0] );

	if ( user_con_id[con] != -1 && user[user_con_id[con]].debug == 1 ) //if the user is being debuged by someone..
	{
		sprintf ( rtn_str, "2,user-rcv:%s:%s", user[user_con_id[con]].username, message );
		send_user_debug ( con,rtn_str );
	}

	if ( user_con_id[con] == -1 )
	{
		switch ( idt ) //commands that only work if a user is not logged on.
		{
			case 1: //version
				right ( temp_str[0], message, point );
				user_con_version[con] = atoi ( temp_str[0] );
				break;
			case 2: //create user
				if ( user_con_version[con] < MIN_USER_VERSION ) break;//tsktsk user didnt give us version
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				split ( temp_str[2],message,',',&point );
				split ( temp_str[3],message,',',&point );
				create_user ( con, temp_str[0], temp_str[1], temp_str[2], temp_str[3] );
				break;
			case 3: //login
				if ( user_con_version[con] < MIN_USER_VERSION ) break;//tsktsk user didnt give us version
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_login ( con, temp_str[0], temp_str[1] );
				break;
		}
	}
	else
	{
		switch ( idt ) //commands that only work if a user is logged on.
		{
			case 4: //purchase ship or mod it
				split ( temp_str[0],message,',',&point );
				temp_int = atoi ( temp_str[0] );

				if ( !temp_int )
				{
					split ( temp_str[1],message,',',&point );
					split ( temp_str[2],message,',',&point );
					purchase_ship ( con, atoi ( temp_str[1] ), atoi ( temp_str[2] ) );
				}
				else
				{
					split ( temp_str[1],message,',',&point );
					purchase_ship_mod ( con, temp_int, atoi ( temp_str[1] ) );
				}
				break;
			case 5: //select ship
				split ( temp_str[0],message,',',&point );
				set_user_ship_selected ( con, atoi ( temp_str[0] ) );
				break;
			case 6: //sell all goods
				user_sell_all_current_ship_goods ( con );
				break;
			case 7: //change password
				split ( temp_str[1],message,',',&point );
				split ( temp_str[2],message,',',&point );
				user_change_password ( con, temp_str[1], temp_str[2] );
				break;
			case 8: //ask for online guild list
				user_return_online_guild_list ( con );
				break;
			case 9: //return online private chat list
				user_list_online_chat_user ( con );
				break;
			case 10: //retrive short user stats
				split ( temp_str[0],message,',',&point );
				user_retrive_short_user_stats ( con, temp_str[0] );
				break;
			case 11: //retrive guild reward list
				split ( temp_str[0],message,',',&point );
				user_retrive_short_guild_reward_list ( con, temp_str[0] );
				break;
			case 12: //Sell ship/mod
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				sell_user_ship_mod ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 13: //repair ship
				split ( temp_str[0],message,',',&point );
				repair_user ( con );
				break;
			case 14: //money transfer
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_transfer_credits ( con, temp_str[0], atoi ( temp_str[1] ) );
				break;
			case 15: //Repair gun
				split ( temp_str[0],message,',',&point );
				repair_user_gun ( con, atoi ( temp_str[0] ) );
				break;
			case 16: //restock missile
				split ( temp_str[0],message,',',&point );
				reload_user_missile ( con, atoi ( temp_str[0] ) );
				break;
			case 17: //purchase goods
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_purchase_goods ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 18: //sell goods
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_sell_goods ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 19: //register guild
				split ( temp_str[0],message,',',&point );
				user_register_guild ( con,temp_str[0] );
				break;
			case 20: //ask for guild listings
				user_relay_guild_listings ( con );
				break;
			case 21: //edit guild
				right ( temp_str[0], message, point );
				user_edit_guild ( con,temp_str[0] );
				break;
			case 22: //withdraw from bank
				split ( temp_str[0],message,',',&point );
				user_withdraw_bank ( con,atoi ( temp_str[0] ) );
				break;
			case 23: //deposit to bank
				split ( temp_str[0],message,',',&point );
				user_deposit_bank ( con,atoi ( temp_str[0] ) );
				break;
			case 24: //retrive user stats
				split ( temp_str[0],message,',',&point );
				user_retrive_stats ( con,temp_str[0] );
				break;
			case 25: //retrive stats list
				split ( temp_str[0],message,',',&point );
				user_retrive_stats_list ( con,atoi ( temp_str[0] ) );
				break;
			case 26: //retrive bounty list
				send_bounty_list ( con );
				break;
			case 27: //add user to bounty list
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_add_bounty ( con,temp_str[0],atoi ( temp_str[1] ) );
				break;
			case 28: //ask for guild info
				split ( temp_str[0],message,',',&point );
				user_request_guild_info ( con,temp_str[0] );
				break;
			case 29: //guild enlist
				split ( temp_str[0],message,',',&point );
				guild_enlist_user ( con, temp_str[0] );
				break;
			case 30: //guild remove
				split ( temp_str[0],message,',',&point );
				guild_remove_user ( con, temp_str[0] );
				break;
			case 31: //guild promote
				split ( temp_str[0],message,',',&point );
				guild_promote_user ( con, temp_str[0] );
				break;
			case 32: //guild demote
				split ( temp_str[0],message,',',&point );
				guild_demote_user ( con, temp_str[0] );
				break;
			case 33: //guild accept
				split ( temp_str[0],message,',',&point );
				guild_user_accept ( con, temp_str[0] );
				break;
			case 34: //guild list
				user_return_guild_list ( con );
				break;
			case 35: // guild chat
				right ( temp_str[0], message, point );
				user_guild_chat ( con, temp_str[0] );
				break;
			case 36: //guild edit more
				right ( temp_str[0], message, point );
				user_edit_guild_more ( con,temp_str[0] );
				break;
			case 37: // sat purchase / upgrade
				user_purchase_upgrade_sat ( con );
				break;
			case 38: // sat deposit money
				split ( temp_str[0],message,',',&point );
				user_deposit_sat_money ( con, atoi ( temp_str[0] ) );
				break;
			case 39: // sat purchase / sell weapon weapon
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				split ( temp_str[2],message,',',&point );
				user_buy_sell_sat_weapon ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ) );
				break;
			case 40: // sat repair / reload weapon
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_repair_reload_sat_weapon ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 41: //enter planet guild
				split ( temp_str[0],message,',',&point );
				user_enter_planet_guild ( con, atoi ( temp_str[0] ) );
				break;
			case 42: //add private chat
				split ( temp_str[0], message, ',', &point );
				user_add_chat_user ( con, temp_str[0] );
				break;
			case 43: //remove private chat
				split ( temp_str[0], message, ',', &point );
				user_remove_chat_user ( con, temp_str[0] );
				break;
			case 44: //list private chat
				user_list_chat_user ( con );
				break;
			case 45: //use private chat
				right ( temp_str[0], message, point );
				user_relay_private_chat ( con, temp_str[0] );
				break;
			case 46: //save guild deco
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				split ( temp_str[2],message,',',&point );
				user_edit_guild_deco ( con, atoi ( temp_str[0] ), temp_str[1], atoi ( temp_str[2] ) );
				break;
			case 47: //award deco
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				guild_award_deco ( con, temp_str[0], temp_str[1] );
				break;
			case 48: //remove deco
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				guild_remove_deco ( con, temp_str[0], temp_str[1] );
				break;
			case 49: //list deco
				split ( temp_str[0],message,',',&point );
				guild_list_deco ( con, temp_str[0] );
				break;
			case 50: //list deco of guild
				split ( temp_str[0],message,',',&point );
				guild_list_decog ( con, temp_str[0] );
				break;
			case 51: //bug a sat
				user_bug_sat ( con );
				break;
			case 52: //ask for bug list
				guild_list_bugs ( con );
				break;
			case 53: //ask for pricechange time
				user_request_pc_time ( con );
				break;
			case 54: //uni chat relay
				right ( temp_str[0], message, point );
				user_uni_chat ( con, temp_str[0] );
				break;
			case 55: //uni toggle
				user_toggle_uni_chat ( con );
				break;
			case 56: //dsamount
				user_get_dsamount ( con );
				break;
			case 57: //transferds
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_transferds ( con,temp_str[0],atoi ( temp_str[1] ) );
				break;
			case 58: //buyactivation
				split ( temp_str[0],message,',',&point );
				user_buyactivation ( con,temp_str[0] );
				break;
			case 59: //buyexp
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_buyexp ( con,temp_str[0],atoi ( temp_str[1] ) );
				break;
			case 60: //buydc
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_buydc ( con,temp_str[0],atoi ( temp_str[1] ) );
				break;
			case 61: //user ping response
				user_ping_response ( con );
				break;
			case 62: //purchase plans
				split ( temp_str[0],message,',',&point );
				user_purchase_plans(con, atoi ( temp_str[0] ));
				break;
			case 63: //set lease login
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				user_set_lease_login ( con, temp_str[0], temp_str[1]);
				break;
		}

	}
}

void process_packet_server ( int con,char* message )
{
	char rtn_str[1060], temp_str[7][1025];
	int point = 0, idt;

	message[1024] = '\0'; //no recieved packet should be greater then this. (done for security reasons)

	//printf ( "svr-rcv-%d:%s\n",con,message );

	split ( temp_str[0],message,',',&point );
	idt = atoi ( temp_str[0] );

	if ( server_con_id[con] != -1 && user[server_con_id[con]].debug == 1 ) //if the user is being debuged by someone..
	{
		sprintf ( rtn_str, "3,server-rcv:%s:%s", server[server_con_id[con]].servername, message );
		send_server_debug ( con,rtn_str );
	}

	if ( server_con_id[con] == -1 )
	{
		switch ( idt ) //commands that only work if a server is not logged on.
		{
			case 1: //version
				right ( temp_str[0], message, point );
				server_con_version[con] = atoi ( temp_str[0] );
				break;
			case 2: //login
				if ( server_con_version[con] < MIN_SERVER_VERSION ) break;//tsktsk user didnt give us version
				split ( temp_str[0],message,',',&point );
				split ( temp_str[1],message,',',&point );
				server_login ( con, temp_str[0], temp_str[1] );
				break;
		}
	}
	else
	{
		switch ( idt ) //commands that only work if a server is logged on.
		{
			case 3: //login user
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				retrieve_user_info ( con, temp_str[0], atoi ( temp_str[1] ), ( unsigned long ) atol ( temp_str[2] ) );
				break;
			case 4: //LMD user
				break;
			case 5: //update sat owners
				break;
			case 6: //land/launch user...
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				set_user_launch ( atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ) );
				break;
			case 7: //destroy user
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				destroy_user ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 8: //user hull update
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				set_user_hull ( atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 9: //user wep destroyed
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				set_user_gun_destroyed ( atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 10: //missile payload ammounts
				split ( temp_str[0], message, ',', &point );
				right ( temp_str[1], message, point );
				set_user_payloads ( atoi ( temp_str[0] ), temp_str[1] );
				break;
			case 11: //clear out all sector good info
				server_clear_sector_amounts ( con );
				break;
			case 12: //sector goods info
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				right ( temp_str[2], message, point );
				server_set_good_amounts ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ), temp_str[2] );
				break;
			case 13: //sector contras info
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				right ( temp_str[2], message, point );
				server_set_contra_amounts ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ), temp_str[2] );
				break;
			case 14: //sector good info
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				server_set_good_amount ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ),atoi ( temp_str[2] ), atoi ( temp_str[3] ) );
				break;
			case 15: //sector contra info
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				server_set_contra_amount ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ),atoi ( temp_str[2] ), atoi ( temp_str[3] ) );
				break;
			case 16: //add sector good info
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				server_add_good_amount ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ),atoi ( temp_str[2] ), atoi ( temp_str[3] ) );
				break;
			case 17: //add sector contra info
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				server_add_contra_amount ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ),atoi ( temp_str[2] ), atoi ( temp_str[3] ) );
				break;
			case 18: //bot pick up goods
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				split ( temp_str[5], message, ',', &point );
				server_bot_pickup_good ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ), atoi ( temp_str[5] ) );
				break;
			case 19: //bot pick up goods
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				split ( temp_str[5], message, ',', &point );
				server_bot_pickup_contra ( con,atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ), atoi ( temp_str[5] ) );
				break;
			case 20: //user pick up good
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				server_user_pickup_good ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ) );
				break;
			case 21: //user pick up contra
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				server_user_pickup_contra ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ) );
				break;
			case 22: //user drop good
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				server_user_drop_good ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ) );
				break;
			case 23: //user drop contra
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				split ( temp_str[5], message, ',', &point );
				server_user_drop_contra ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ), atoi ( temp_str[5] ) );
				break;
			case 24: //give user exp from a bot
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				server_user_give_bot_exp ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), temp_str[3] );
				break;
			case 25: //update planet sat hull
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				server_set_sat_hull ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ) );
				break;
			case 26: //update planet sat missile_left
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				server_set_sat_missile_left ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ) );
				break;
			case 27: //update planet sat missile_left
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				server_set_sat_gun_destroyed ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ) );
				break;
			case 28: //disconnect destroy
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				destroy_user_disconnected ( con, temp_str[0], atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), atoi ( temp_str[4] ) );
				break;
			case 29: //set user zone / sector
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				set_user_zone_sector ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ) );
				break;
			case 30: //random drop award
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				split ( temp_str[2], message, ',', &point );
				split ( temp_str[3], message, ',', &point );
				split ( temp_str[4], message, ',', &point );
				server_user_award_random_drop ( con, atoi ( temp_str[0] ), atoi ( temp_str[1] ), atoi ( temp_str[2] ), atoi ( temp_str[3] ), ( enum BOT_DROP_TYPE ) atoi ( temp_str[4] ) );
				break;
			case 31: //server ping response
				server_ping_response ( con );
				break;
			case 32: //server ping request
				server_ping_request ( con );
				break;
			case 33: //clear sector contraband
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				server_clear_sector_contraband(con, atoi(temp_str[0]), atoi(temp_str[1]));
				break;
			case 34: //clear sector goods
				split ( temp_str[0], message, ',', &point );
				split ( temp_str[1], message, ',', &point );
				server_clear_sector_goods(con, atoi(temp_str[0]), atoi(temp_str[1]));
				break;
		}
	}
}
