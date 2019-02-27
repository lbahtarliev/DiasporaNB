#include "main.h"

void admin_award_user_ship(int con, char *message, int point);
void admin_set_user_money(int con, char *message, int point);
void admin_award_user_money(int con, char *message, int point);
void admin_close_nicely(int con);
void admin_change_admin_password(int con, char *message, int point);
void admin_load_server_files(int con, char *message, int point);
void admin_activate_user(int con, char *message, int point);
void admin_deactivate_user(int con, char *message, int point);
void admin_create_server(int con, char *message, int point);
void admin_edit_server_password(int con, char *message, int point);
void admin_edit_server_cords(int con, char *message, int point);
void admin_edit_server_ip(int con, char *message, int point);
void admin_set_user_exp(int con, char *message, int point);
void admin_award_user_exp(int con, char *message, int point);
void admin_set_guild_money(int con, char *message, int point);
void admin_award_guild_money(int con, char *message, int point);
void admin_award_user_market_exp(int con, char *message, int point);
void admin_global_message(int con, char *message, int point);
void admin_clean_all_decos(int con);
void admin_do_guild_salery(int con);
void admin_change_guild_name(int con, char *message, int point);
void admin_change_guild_owner(int con, char *message, int point);
void admin_change_user_password(int con, char *message, int point);
void admin_change_user_login(int con, char *message, int point);
void admin_change_user_name(int con, char *message, int point);
void admin_insert_all_jabber(int con);
void admin_view_guild_bank_stats(int con, char *message, int point);
void admin_force_mfc_payout(int con);
void admin_list_all_standards(int con);
void admin_award_user_standards(int con, char *message, int point);
void admin_set_hub_name(int con, char *message, int point);
void admin_give_barcodes(int con, char *message, int point);
void admin_list_user_hanger(int con, char *message, int point);
void admin_award_user_gun(int con, char *message, int point);
void admin_award_user_missile(int con, char *message, int point);
void admin_award_user_ship_loaded(int con, char *message, int point);
void admin_set_user_guild_rank(int con, char *message, int point);
void admin_give_spacenames(int con, char *message, int point);
void admin_onlinenoobs(int con, char *message, int point);
void admin_list_noobs(int con, char *message, int point);
void admin_destroy_sat(int con, char *message, int point);

void admin_logoff(int con)
{
	int i;
	
	if (admin_con_id[con] != -1)
	{
		admin[admin_con_id[con]].user_debug = -1;
		admin[admin_con_id[con]].server_debug = -1;
	}
	
	admin_con_id[con] = -1;
	
	for(i=0;i<=admin_max;i++)
		if (admin[i].con_id == con)
	{
		admin[i].user_debug = -1;
		admin[i].server_debug = -1; 
		admin[i].con_id = -1;
	}
}

void admin_logoff_duplicates(int a)
{
	int i;
	
	for(i=0;i<=admin_con_top;i++)
		if (admin_con_id[i] == a)
			admin_logoff(i);
}

void admin_login(int con, char *username, char *password)
{
	int i;
	char temp_str[2][21], rtn_str[200];
	
	if (username[0] == '\0') return; //dont allow nameless user to login	
	
	username[21] = password[21] = '\0'; //make sure these arent larger then 19 chars
	
	lcase(temp_str[0],username); //put the lower case version of the requested username into temp_str
	
	for(i=0;i<=admin_max;i++)
	{
		lcase(temp_str[1],admin[i].login);
		
		if (str_match(temp_str[0],temp_str[1]) && str_match(password,admin[i].password))
		{
			//clean this con up
			admin_logoff(con);
			
			//no duplicates
			admin_logoff_duplicates(i);
			
			//setup this con
			admin[i].con_id = con;
			admin_con_id[con] = i;
			sprintf(rtn_str,"1,Welcome %s! There are currently %d users on %d servers online.",admin[i].login,online_users(),online_servers());
			send_con_admin(con,rtn_str);
			return;
		}
	}
	
	if (admin_max == -1) //if no admin users have been made, then create a temporary one
	{
		
	}
}

void admin_shell(int con,char *message)
{
	char command[20], temp_str[5][200], rtn_str[5000];
	int point =0, temp_int[5], i;
	
	printd_admin(message);
	
	split(command, message, ',', &point);
	lcase(command,command);
	
	if (str_match(command,"help"))
	{
		split(temp_str[0],message,',',&point);
		lcase(temp_str[0],temp_str[0]);
		
		if (str_match(temp_str[0],"command"))
		{
			strcpy(rtn_str,"1,help command - I didn't mean it literaly.");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"onlineusers"))
		{
			strcpy(rtn_str,"1,help onlineusers - gives list of online users");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage onlinusers - onlineusers");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"onlineservers"))
		{
			strcpy(rtn_str,"1,help onlineservers - gives list of online servers");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage onlineservers - onlineservers");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"debuguser"))
		{
			strcpy(rtn_str,"1,help debuguser - monitors traffic to and from a user");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage debuguser - debuguser,username");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"debugserver"))
		{
			strcpy(rtn_str,"1,help debugserver - monitors traffic to and from a server");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage debugserver - debugserver,servername");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"read_userdb"))
		{
			strcpy(rtn_str,"1,help read_userdb - reads in user.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage read_userdb - read_userdb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"read_serverdb"))
		{
			strcpy(rtn_str,"1,help read_serverdb - reads in server.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage read_serverdb - read_serverdb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"read_admindb"))
		{
			strcpy(rtn_str,"1,help read_admindb - reads in admin.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage read_admindb - read_admindb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"read_bandb"))
		{
			strcpy(rtn_str,"1,help read_bandb - reads in ban.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage read_bandb - read_bandb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"read_bountydb"))
		{
			strcpy(rtn_str,"1,help read_bountydb - reads in bounty.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage read_bountydb - read_bountydb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"write_userdb"))
		{
			strcpy(rtn_str,"1,help write_userdb - writes in user.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage write_userdb - write_userdb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"write_serverdb"))
		{
			strcpy(rtn_str,"1,help write_serverdb - writes in server.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage write_serverdb - write_serverdb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"write_admindb"))
		{
			strcpy(rtn_str,"1,help write_admindb - writes in admin.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage write_admindb - write_admindb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"write_bandb"))
		{
			strcpy(rtn_str,"1,help write_bandb - writes in ban.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage write_bandb - write_bandb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"write_bountydb"))
		{
			strcpy(rtn_str,"1,help write_bountydb - writes in bounty.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage write_bountydb - write_bountydb");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_ship"))
		{
			strcpy(rtn_str,"1,help award_user_ship - plainly gives a user a ship");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_ship - award_user_ship,playername,ship_servername,ship_classnumber,ship_kindnumber");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"delete_user_ship"))
		{
			strcpy(rtn_str,"1,help delete_user_ship - plainly delete a user's ship");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage delete_user_ship - delete_user_ship,playername,hanger_entry_number");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"view_user_hanger"))
		{
			strcpy(rtn_str,"1,help view_user_hanger - list all hanger and ship details of a user");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage view_user_hanger - view_user_hanger,playername");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"close_nicely"))
		{
			strcpy(rtn_str,"1,help close_nicely - shuts down the server the way its ment to");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage close_nicely - close_nicely");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"set_user_money"))
		{
			strcpy(rtn_str,"1,help set_user_money - set a users money to");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage set_user_money - set_user_money,user_name,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_money"))
		{
			strcpy(rtn_str,"1,help award_user_money - add amount to user account");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_money - award_user_money,user_name,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"set_guild_money"))
		{
			strcpy(rtn_str,"1,help set_guild_money - set a guilds money to");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage set_guild_money - set_guild_money,guild_name,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_guild_money"))
		{
			strcpy(rtn_str,"1,help award_guild_money - add amount to guilds bank");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_guild_money - award_guild_money,guild_name,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"set_user_exp"))
		{
			strcpy(rtn_str,"1,help set_user_exp - set a users exp to");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage set_user_exp - set_user_exp,user_name,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_exp"))
		{
			strcpy(rtn_str,"1,help award_user_exp - add exp amount to user account");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_exp - award_user_exp,user_name,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_admin_password"))
		{
			strcpy(rtn_str,"1,help change_admin_password - change current admin accounts password");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_admin_password - change_admin_password,new_password");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"load_server_files"))
		{
			strcpy(rtn_str,"1,help load_server_files - reload the files of a server");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage load_server_files - load_server_files,server_name");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"activate_user"))
		{
			strcpy(rtn_str,"1,help activate_user - activate a user account");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage activate_user - activate_user,user_name");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"deactivate_user"))
		{
			strcpy(rtn_str,"1,help deactivate_user - deactivate a user account");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage deactivate_user - deactivate_user,user_name");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"create_server"))
		{
			strcpy(rtn_str,"1,help create_server - create new server account");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage create_server - create_server,server_name,login,pass,x,y");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"edit_server_password"))
		{
			strcpy(rtn_str,"1,help edit_server_password - eh");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage edit_server_password - edit_server_password,server_name,new_password");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"edit_server_cords"))
		{
			strcpy(rtn_str,"1,help edit_server_cords - eh");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage edit_server_cords - edit_server_cords,server_name,x,y");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"edit_server_ip"))
		{
			strcpy(rtn_str,"1,help edit_server_ip - eh");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage edit_server_ip - edit_server_cords,server_name,forced_ip");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_market_exp"))
		{
			strcpy(rtn_str,"1,help award_user_market_exp - quicker way of awarding market exp and money, by setting in the amount of exp they should be awarded");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_market_exp - award_user_market_exp,playername,exp_owed");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"global_message"))
		{
			strcpy(rtn_str,"1,help global_message - send warning message to everyone online");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage global_message - global_message,message");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"clean_all_decos"))
		{
			strcpy(rtn_str,"1,help clean_all_decos - Wipes out all decos that guilds and users have");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage clean_all_decos - clean_all_decos");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"do_guild_salery"))
		{
			strcpy(rtn_str,"1,help do_guild_salery - Forces all guilds to pay out a round of saleries");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage do_guild_salery - do_guild_salery");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_guild_name"))
		{
			strcpy(rtn_str,"1,help change_guild_name - Change a guild's name");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_guild_name - change_guild_name,current_name,new_name");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_guild_owner"))
		{
			strcpy(rtn_str,"1,help change_guild_owner - Change a guild's owner");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_guild_owner - change_guild_name,guild_name,new_owner");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_guild_owner"))
		{
			strcpy(rtn_str,"1,help change_guild_owner - Change a guild's owner");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_guild_owner - change_guild_name,guild_name,new_owner");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_user_name"))
		{
			strcpy(rtn_str,"1,help change_user_name - Change a user's name");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_user_name - change_user_name,user_name,new_name");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_user_login"))
		{
			strcpy(rtn_str,"1,help change_user_login - Change a user's login");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_user_login - change_user_login,user_name,new_login");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"change_user_password"))
		{
			strcpy(rtn_str,"1,help change_user_password - Change a user's password");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage change_user_password - change_user_password,user_name,new_password");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"insert_all_jabber"))
		{
			strcpy(rtn_str,"1,help insert_all_jabber - Insert all user accounts into the jabber");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage insert_all_jabber - insert_all_jabber");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"view_guild_bank_stats"))
		{
			strcpy(rtn_str,"1,help view_guild_bank_stats - view some statistics related to a guilds bank");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage view_guild_bank_stats - view_guild_bank_stats,guildname");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"force_mfc_payout"))
		{
			strcpy(rtn_str,"1,help force_mfc_payout - force payout all mfc debts");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage force_mfc_payout - force_mfc_payout");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"list_all_standards"))
		{
			strcpy(rtn_str,"1,help list_all_standards - list everyone who has donation standards and how much");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage list_all_standards - list_all_standards");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_standards"))
		{
			strcpy(rtn_str,"1,help award_user_standards - Give a user x amount of donation standards");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_standards - award_user_standards,username,amount");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"set_hub_name"))
		{
			strcpy(rtn_str,"1,help set_hub_name - Set the hub name players see when they first connect");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage set_hub_name - set_hub_name,hubname");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"give_barcodes")) //give_spacenames
		{
			strcpy(rtn_str,"1,help give_barcodes - Relay all potential barcode usernames in the user.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage give_barcodes - give_barcodes");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"give_spacenames")) //give_spacenames
		{
			strcpy(rtn_str,"1,help give_spacenames - Relay all potential usernames with bad space usage in the user.db");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage give_spacenames - give_spacenames");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"list_user_hanger")) //admin_award_user_gun
		{
			strcpy(rtn_str,"1,help list_user_hanger - List the ships in a user's hanger and their array numbers");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage list_user_hanger - list_user_hanger,username");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_gun")) //admin_award_user_gun
		{
			strcpy(rtn_str,"1,help award_user_gun - Awards a gun to a user's ship");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_gun - award_user_gun,username,ship_hanger_number,servername,gun_number");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_missile")) //award_user_ship_loaded
		{
			strcpy(rtn_str,"1,help award_user_missile - Awards a missile to a user's ship");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_missile - award_user_missile,username,ship_hanger_number,servername,missile_number");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"award_user_ship_loaded")) //set_user_guild_rank
		{
			strcpy(rtn_str,"1,help award_user_ship_loaded - Awards a fully loaded ship to a user");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage award_user_ship_loaded - award_user_ship_loaded,username,servername,ship_class,ship_kind,gun_number,missile_number");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"set_user_guild_rank")) //set_user_guild_rank
		{
			strcpy(rtn_str,"1,help set_user_guild_rank - Sets a user's rank level (0-5)");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage set_user_guild_rank - user_name,rank_number");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"list_noobs")) //set_user_guild_rank
		{
			strcpy(rtn_str,"1,help list_noobs - List recent noobs as to date");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage list_noobs - list_noobs");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"onlinenoobs")) //set_user_guild_rank
		{
			strcpy(rtn_str,"1,help onlinenoobs - List online noobs");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage onlinenoobs - onlinenoobs");
			send_con_admin(con,rtn_str);
		}
		else if(str_match(temp_str[0],"destroy_sat")) //set_user_guild_rank
		{
			strcpy(rtn_str,"1,help destroy_sat - Destroy a planet's satellite");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,usage destroy_sat - destroy_sat,servername,planetname");
			send_con_admin(con,rtn_str);
		}
		else
		{
			strcpy(rtn_str,"1,Command list - type help command for more information");
			send_con_admin(con,rtn_str);
			strcpy(rtn_str,"1,list_noobs, destroy_sat, onlinenoobs, give_spacenames, set_user_guild_rank, award_user_ship_loaded, award_user_missile, award_user_gun, list_user_hanger, give_barcodes, set_hub_name, award_user_standards, list_all_standards, force_mfc_payout, view_guild_bank_stats, insert_all_jabber, change_user_password, change_user_login, change_user_name, change_guild_owner, change_guild_name, do_guild_salery, clean_all_decos, global_message, award_user_market_exp, create_server, edit_server_password, edit_server_cords, edit_server_ip, activate_user, deactivate_user, load_server_files, change_admin_password, award_guild_money, set_guild_money, award_user_money, set_user_money, award_user_exp, set_user_exp, close_nicely, view_user_hanger, delete_user_ship, award_user_ship, onlineusers, onlineservers, debuguser, debugserver, read_userdb, read_serverdb, read_admindb, read_bandb, read_bountydb, write_userdb, write_serverdb, write_admindb, write_bandb, write_bountydb");
			send_con_admin(con,rtn_str);
		}
	}
	
	//now for actually choosing one of those commands
	if (str_match(command,"onlineusers"))
	{
		sprintf(rtn_str,"1,Users online:%d list:",online_users());
		for(i=0;i<=user_con_top;i++)
			if (user_con_id[i] != -1)
			{
				sprintf(temp_str[0]," %s:%d,",user[user_con_id[i]].username, i);
				strcat(rtn_str,temp_str[0]);
			}
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"onlineservers"))
	{
		sprintf(rtn_str,"1,Servers online:%d list:",online_servers());
		for(i=0;i<=server_con_top;i++)
			if (server_con_id[i] != -1)
			{
				sprintf(temp_str[0]," %s,",server[server_con_id[i]].servername);
				strcat(rtn_str,temp_str[0]);
			}
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"debuguser"))
	{
		split(temp_str[0],message,',',&point);
		temp_int[0] = user_to_id(temp_str[0]);
		
		if (temp_int[0] == -1)
		{
			sprintf("1,User not found:%s",temp_str[0]);
			send_con_admin(con,rtn_str);
		}
		else if (user[temp_int[0]].con_id == -1)
		{
			sprintf("1,User not online:%s",user[temp_int[0]].username);
			send_con_admin(con,rtn_str);
		}
		else
		{
			sprintf(rtn_str,"1,Debug initiated on user:%s",user[temp_int[0]].username);
			send_con_admin(con,rtn_str);
			if (admin[admin_con_id[con]].user_debug != -1)//if your a fickle fuck!
			{
				for(i=0;i<=admin_con_top;i++)
					if (i != admin_con_id[con] && admin[admin_con_id[con]].user_debug == admin[admin_con_id[i]].user_debug) break;
					
				if ( i == admin_con_top && i != admin_con_id[con] && admin[admin_con_id[con]].user_debug == admin[admin_con_id[i]].user_debug)
					user[admin[admin_con_id[con]].user_debug].debug = -1; //if no one other then you is debuging this user, then let em free
			}
				
			admin[admin_con_id[con]].user_debug = temp_int[0];
			user[temp_int[0]].debug = 1;
		}
	}
	if (str_match(command,"debugserver"))
	{
	;
	}
	
	if (str_match(command,"read_userdb"))
	{
		read_user_db();
		sprintf(rtn_str,"1,user database loaded:%d entires found.",user_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"read_serverdb"))
	{
		read_server_db();
		sprintf(rtn_str,"1,server database loaded:%d entires found.",server_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"read_admindb"))
	{
		read_admin_db();
		sprintf(rtn_str,"1,admin database loaded:%d entires found.",admin_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"read_bandb"))
	{
		read_ban_db();
		sprintf(rtn_str,"1,ban database loaded:%d entires found.",ban_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"read_bountydb"))
	{
		read_bounty_db();
		sprintf(rtn_str,"1,bounty database loaded.");
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"write_userdb"))
	{
		write_user_db(0);
		sprintf(rtn_str,"1,user database written:%d entires found.",user_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"write_serverdb"))
	{
		write_server_db();
		sprintf(rtn_str,"1,server database written:%d entires found.",server_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"write_admindb"))
	{
		write_admin_db();
		sprintf(rtn_str,"1,admin database written:%d entires found.",admin_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"write_bandb"))
	{
		write_ban_db();
		sprintf(rtn_str,"1,ban database written:%d entires found.",ban_max + 1);
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"write_bountydb"))
	{
		write_bounty_db();
		sprintf(rtn_str,"1,bounty database written.");
		send_con_admin(con,rtn_str);
	}
	if (str_match(command,"set_user_money"))
		admin_set_user_money(con,message,point);
	if (str_match(command,"award_user_money"))
		admin_award_user_money(con,message,point);
	if (str_match(command,"close_nicely"))
		admin_close_nicely(con);
	if (str_match(command,"change_admin_password"))
		admin_change_admin_password(con,message,point);//
	if (str_match(command,"load_server_files"))
		admin_load_server_files(con,message,point);
	if (str_match(command,"activate_user"))
		admin_activate_user(con,message,point);
	if (str_match(command,"deactivate_user"))
		admin_deactivate_user(con,message,point);
	if (str_match(command,"create_server"))
		admin_create_server(con,message,point);
	if (str_match(command,"edit_server_password"))
		admin_edit_server_password(con,message,point);
	if (str_match(command,"edit_server_cords"))
		admin_edit_server_cords(con,message,point);
	if (str_match(command,"edit_server_ip"))
		admin_edit_server_ip(con,message,point);
	if (str_match(command,"set_user_exp"))
		admin_set_user_exp(con,message,point);
	if (str_match(command,"award_user_exp"))
		admin_award_user_exp(con,message,point);
	if (str_match(command,"set_guild_money"))
		admin_set_guild_money(con,message,point);
	if (str_match(command,"award_guild_money"))
		admin_award_guild_money(con,message,point);
	if(str_match(command,"award_user_market_exp"))
		admin_award_user_market_exp(con,message,point);
	if(str_match(command,"global_message"))
		admin_global_message(con,message,point);
	if(str_match(command,"clean_all_decos"))
		admin_clean_all_decos(con); // do_guild_salery
	if(str_match(command,"do_guild_salery"))
		admin_do_guild_salery(con); // change_guild_owner
	if(str_match(command,"change_guild_owner"))
		admin_change_guild_owner(con,message,point); // change_guild_name
	if(str_match(command,"change_guild_name"))
		admin_change_guild_name(con,message,point); // change_user_password
	if(str_match(command,"change_user_password"))
		admin_change_user_password(con,message,point); // change_user_passw
	if(str_match(command,"change_user_login"))
		admin_change_user_login(con,message,point); // change_user_passw
	if(str_match(command,"change_user_name"))
		admin_change_user_name(con,message,point); // change_user_passw
	if(str_match(command,"insert_all_jabber")) //view_guild_bank_stats
		admin_insert_all_jabber(con);
	if(str_match(command,"view_guild_bank_stats")) //view_guild_bank_stats
		admin_view_guild_bank_stats(con,message,point); //award_user_standards
	if(str_match(command,"force_mfc_payout")) //view_guild_bank_stats
		admin_force_mfc_payout(con); //list_all_standards
	if(str_match(command,"list_all_standards")) //view_guild_bank_stats
		admin_list_all_standards(con); //list_all_standards
	if(str_match(command,"award_user_standards")) //view_guild_bank_stats
		admin_award_user_standards(con,message,point); //admin_set_hub_name
	if(str_match(command,"set_hub_name")) //view_guild_bank_stats
		admin_set_hub_name(con,message,point); //give_barcodes
	if(str_match(command,"give_barcodes")) //view_guild_bank_stats
		admin_give_barcodes(con,message,point); //admin_award_user_ship
	if(str_match(command,"award_user_ship")) //view_guild_bank_stats
		admin_award_user_ship(con,message,point); //list_user_hanger
	if(str_match(command,"list_user_hanger")) //view_guild_bank_stats
		admin_list_user_hanger(con,message,point); //award_user_gun
	if(str_match(command,"award_user_gun")) //view_guild_bank_stats
		admin_award_user_gun(con,message,point); //award_user_gun
	if(str_match(command,"award_user_missile")) //view_guild_bank_stats
		admin_award_user_missile(con,message,point); //admin_award_user_ship_loaded
	if(str_match(command,"award_user_ship_loaded")) //view_guild_bank_stats
		admin_award_user_ship_loaded(con,message,point); //admin_set_user_guild_rank
	if(str_match(command,"set_user_guild_rank")) //view_guild_bank_stats
		admin_set_user_guild_rank(con,message,point); //admin_give_spacenames
	if(str_match(command,"give_spacenames")) //admin_list_online_noobs
		admin_give_spacenames(con,message,point); //admin_give_spacenames
	if(str_match(command,"onlinenoobs")) //admin_list_online_noobs
		admin_onlinenoobs(con,message,point); //admin_give_spacenames
	if(str_match(command,"list_noobs")) //admin_list_online_noobs
		admin_list_noobs(con,message,point); //admin_give_spacenames
	if(str_match(command,"destroy_sat")) //admin_list_online_noobs
		admin_destroy_sat(con,message,point); //admin_give_spacenames
}

extern int online_admins()
{
	int i, total = 0;
	
	for(i=0;i<=admin_con_top;i++)
		if (admin_con_id[i] != -1) total++;
		
	return total;
}

void admin_award_user_ship_loaded(int con, char *message, int point)
{
	//award_user_ship_loaded,username,servername,ship_class,ship_kind,gun_number,missile_number
	const int input_amount = 6;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	char ship_str[50];
	
	int i, k, z, u;
	int sh_s, sh_c, sh_k;
	int max;
	int g, m;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	sh_s = server_to_id(temp_str[1]);
	sh_c = atoi(temp_str[2]);
	sh_k = atoi(temp_str[3]);
	g = atoi(temp_str[4]);
	m = atoi(temp_str[5]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(sh_s==-1) 
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//no brainer checks..
	if(sh_c < 0) return;
	if(sh_k < 0) return;
	if(sh_c >= SHIP_MAX) return;
	if(sh_k >= 8) return;
	if(g < 0) return;
	if(g >= GUN_MAX) return;
	if(m < 0) return;
	if(m >= MISSILE_MAX) return;
	
	//few more checks..
	
	if(!server[sh_s].missile[m].name[0])
	{
		sprintf(rtn_str,"1,missile not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(!server[sh_s].gun[g].name[0])
	{
		sprintf(rtn_str,"1,gun not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//ship slot available?
	for(k=0;k<10;k++)
	{
		if(user[u].ship[k].ship_id == -1) break;
		if(user[u].ship[k].brand == -1) break;
		if(user[u].ship[k].type == -1) break;
	}
			
	//none found?
	if(k==10)
	{
		sprintf(rtn_str,"1,hanger not empty.");
		send_con_admin(con,rtn_str);
		return;
	}
			
	//give it!
	user_award_ship(u, k, sh_s, sh_c, sh_k);
	
	//now load the bitch!
	max = server[sh_s].ship[sh_c].missile_ammount[sh_k];
	for(z=0;z<max;z++)
		user_award_missile(u, k, sh_s, m, z);
	
	max = server[sh_s].ship[sh_c].gun_ammount[sh_k];
	for(z=0;z<max;z++)
		user_award_gun(u, k, sh_s, g, z);
	
	//notify
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Admin Awarded Item: The %s %s ship fully loaded..", server[sh_s].ship[sh_c].name, ship_name(sh_k, ship_str));
		send_user_private_chat(user[u].con_id, 1, rtn_str);
	}
	
	sprintf(rtn_str,"1,user awarded the %s %s ship fully loaded.", server[sh_s].ship[sh_c].name, ship_name(sh_k, ship_str));
	send_con_admin(con,rtn_str);
}

void admin_award_user_ship(int con, char *message, int point)
{
	const int input_amount = 4;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	char ship_str[50];
	
	int i, k, u;
	int sh_s, sh_c, sh_k;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	sh_s = server_to_id(temp_str[1]);
	sh_c = atoi(temp_str[2]);
	sh_k = atoi(temp_str[3]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(sh_s==-1) 
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//no brainer checks..
	if(sh_c < 0) return;
	if(sh_k < 0) return;
	if(sh_c >= SHIP_MAX) return;
	if(sh_k >= 8) return;
	
	//ship slot available?
	for(k=0;k<10;k++)
	{
		if(user[u].ship[k].ship_id == -1) break;
		if(user[u].ship[k].brand == -1) break;
		if(user[u].ship[k].type == -1) break;
	}
			
	//none found?
	if(k==10)
	{
		sprintf(rtn_str,"1,hanger not empty.");
		send_con_admin(con,rtn_str);
		return;
	}
			
	//give it!
	user_award_ship(u, k, sh_s, sh_c, sh_k);
			
	//notify
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Admin Awarded Item: The %s %s ship ..", server[sh_s].ship[sh_c].name, ship_name(sh_k, ship_str));
		send_user_private_chat(user[u].con_id, 1, rtn_str);
	}
	
	sprintf(rtn_str,"1,user awarded the %s %s ship.", server[sh_s].ship[sh_c].name, ship_name(sh_k, ship_str));
	send_con_admin(con,rtn_str);
}

void admin_change_admin_password(int con, char *message, int point)
{
     const int input_amount = 1;
     char temp_str[input_amount][1024];
     char rtn_str[2000];
     
     int i, a;
     
     a = admin_con_id[con];
     
     for(i=0;i<input_amount;i++)
     {
         split(temp_str[i],message,',',&point);
         lcase(temp_str[i],temp_str[i]);
         if(!temp_str[i][0]) return;
     }
     
     strcpy(admin[a].password,temp_str[0]);
     
    sprintf(rtn_str,"1,password set to %s", admin[a].password);
    send_con_admin(con,rtn_str);
     
     write_admin_db();
}

void admin_award_user_money(int con, char *message, int point)
{
     const int input_amount = 2;
     char temp_str[input_amount][1024];
     char rtn_str[2000];
     
     int i, u, amount;
     
     for(i=0;i<input_amount;i++)
     {
         split(temp_str[i],message,',',&point);
         lcase(temp_str[i],temp_str[i]);
         if(!temp_str[i][0]) return;
     }
         
     u = user_to_id(temp_str[0]);
     amount = atoi(temp_str[1]);
     
     if(u==-1) 
     {
        sprintf(rtn_str,"1,user not found.");
        send_con_admin(con,rtn_str);
        return;
     }
     
     //do the deed
     user[u].money += amount;
     
         		//now tell the character
     if(user[u].con_id > -1)
     {
	     sprintf(rtn_str,"... Admin awarded DC:%d ..", amount);
	     send_user_private_chat(user[u].con_id,1,rtn_str);
		
	     user_send_money(user[u].con_id);
     }
     
     //send the info
    sprintf(rtn_str,"1,user awarded %d, now has %d", amount, user[u].money);
    send_con_admin(con,rtn_str);
    
    //do this
    game.rewrite_user_db = 1;
}

void admin_set_user_money(int con, char *message, int point)
{
     const int input_amount = 2;
     char temp_str[input_amount][1024];
     char rtn_str[2000];
     
     int i, u, amount;
     
     for(i=0;i<input_amount;i++)
     {
         split(temp_str[i],message,',',&point);
         lcase(temp_str[i],temp_str[i]);
         if(!temp_str[i][0]) return;
     }
         
     u = user_to_id(temp_str[0]);
     amount = atoi(temp_str[1]);
     
     if(u==-1) 
     {
        sprintf(rtn_str,"1,user not found.");
        send_con_admin(con,rtn_str);
        return;
     }
     
     //do the deed
     user[u].money = amount;
     
     //send the info
    sprintf(rtn_str,"1,user money set to %d", user[u].money);
    send_con_admin(con,rtn_str);
    
    
    //do this
    game.rewrite_user_db = 1;
}

void admin_close_nicely(int con)
{
     char rtn_str[2000];
     check_databases_for_save();
     
     sprintf(rtn_str,"1,databases saved");
    send_con_admin(con,rtn_str);
}

void admin_load_server_files(int con, char *message, int point)
{
	const int input_amount = 1;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int loaded;
	
	int i, s;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	s = server_name_to_id(temp_str[0]);
	
	if(s == -1) return;
	
	loaded = read_all_servers_db(s);
	
	if(loaded == 5)
		server[s].db_loaded = 1;
	else
		server[s].db_loaded = 0;
		
	//who knows if anything changed but but...
	mysql_update_server_entry(s);
	
	sprintf(rtn_str,"1,databases read for %d:%s", loaded, server[s].servername);
	send_con_admin(con,rtn_str);
}

void admin_activate_user(int con, char *message, int point)
{
	const int input_amount = 1;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, u;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(activate_user(u))
	{
		sprintf(rtn_str,"1,user activated.");
		send_con_admin(con,rtn_str);
	}
	else
	{
		sprintf(rtn_str,"1,user already activated.");
		send_con_admin(con,rtn_str);
	}
}

void admin_deactivate_user(int con, char *message, int point)
{
	const int input_amount = 1;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, u;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	user[u].activated = 0;
	
	sprintf(rtn_str,"1,user deactivated.");
	send_con_admin(con,rtn_str);
	
	//tell who all that might care
	user_send_activated_user(u);
	user_send_activated_server(u);
		
	//do this
	game.rewrite_user_db = 1;
}

void admin_create_server(int con, char *message, int point)
{
	const int input_amount = 5;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, s;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(i > 0) //not the server name
			lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	s = server_name_to_id(temp_str[0]);
	
	if(s != -1)
	{
		sprintf(rtn_str,"1,server already made.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	server_max++;
	s = server_max;
	
	strcpy(server[s].servername, temp_str[0]);
	strcpy(server[s].login, temp_str[1]);
	strcpy(server[s].password, temp_str[2]);
	server[s].x = atoi(temp_str[3]);
	server[s].y = atoi(temp_str[4]);
	server[s].real_address[0] = 0;
	
	sprintf(rtn_str,"1,server db change made.");
	send_con_admin(con,rtn_str);
	
	write_server_db();
}

void admin_edit_server_password(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, s;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	s = server_name_to_id(temp_str[0]);
	
	if(s == -1)
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	strcpy(server[s].password, temp_str[1]);
	
	sprintf(rtn_str,"1,server db change made.");
	send_con_admin(con,rtn_str);
	
	write_server_db();
}

void admin_edit_server_cords(int con, char *message, int point)
{
	const int input_amount = 3;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, s;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	s = server_name_to_id(temp_str[0]);
	
	if(s == -1)
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	server[s].x = atoi(temp_str[1]);
	server[s].y = atoi(temp_str[2]);
	
	sprintf(rtn_str,"1,server db change made.");
	send_con_admin(con,rtn_str);
	
	write_server_db();
}

void admin_edit_server_ip(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, s;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
	
	s = server_name_to_id(temp_str[0]);
	
	if(s == -1)
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	strcpy(server[s].real_address, temp_str[1]);
	
	sprintf(rtn_str,"1,server db change made.");
	send_con_admin(con,rtn_str);
	
	write_server_db();
}

void admin_set_user_exp(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
     
	int i, u, amount;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
         
	u = user_to_id(temp_str[0]);
	amount = atoi(temp_str[1]);
     
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
     
     //do the deed
	user[u].exp = amount;
     
     //send the info
	sprintf(rtn_str,"1,user exp set to %d", user[u].exp);
	send_con_admin(con,rtn_str);
    
    //do this
	game.rewrite_user_db = 1;
}

void admin_award_user_exp(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
     
	int i, u, amount;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
         
	u = user_to_id(temp_str[0]);
	amount = atoi(temp_str[1]);
     
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
     
     //do the deed
	user[u].exp += amount;
     
     //send the info
	sprintf(rtn_str,"1,user awarded %d exp, now has %d exp", amount, user[u].exp);
	send_con_admin(con,rtn_str);
    
		//now tell the character
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Admin awarded Exp:%d ..", amount);
		send_user_private_chat(user[u].con_id,1,rtn_str);
		
		user_send_exp(u);
	}
	
    //do this
	game.rewrite_user_db = 1;
}

void admin_set_guild_money(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
     
	int i, g, amount;
	int old_amount;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
         
	g = guild_name_to_id(temp_str[0]);
	amount = atoi(temp_str[1]);
     
	if(g==-1) 
	{
		sprintf(rtn_str,"1,guild not found.");
		send_con_admin(con,rtn_str);
		return;
	}
     
     //do the deed
	old_amount = guild[g].bank_money;
	guild[g].bank_money = amount;
     
     //send the info
	sprintf(rtn_str,"1,guild money set to %d from %d", guild[g].bank_money, old_amount);
	send_con_admin(con,rtn_str);
    
    //do this
	game.rewrite_guild_db = 1;
}

void admin_award_guild_money(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
     
	int i, g, amount;
	int old_amount;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
         
	g = guild_name_to_id(temp_str[0]);
	amount = atoi(temp_str[1]);
     
	if(g==-1) 
	{
		sprintf(rtn_str,"1,guild not found.");
		send_con_admin(con,rtn_str);
		return;
	}
     
     //do the deed
	old_amount = guild[g].bank_money;
	guild[g].bank_money += amount;
     
     //send the info
	sprintf(rtn_str,"1,guild money set to %d from %d", guild[g].bank_money, old_amount);
	send_con_admin(con,rtn_str);
    
    //do this
	game.rewrite_guild_db = 1;
}

void admin_award_user_market_exp(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	const int exp_per_user = 2;
	const int dc_per_user = 50000;
     
	int i, u, amount, dc_amount;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
         
	u = user_to_id(temp_str[0]);
	amount = atoi(temp_str[1]);
     
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
     
	//set this now
	dc_amount = dc_per_user * amount / exp_per_user;
	
     //do the deed
	user[u].exp += amount;
	user[u].money += dc_amount;
     
     //send the info
	sprintf(rtn_str,"1,user awarded %d exp and %d dc, now has %d exp and %d dc", amount, dc_amount, user[u].exp, user[u].money);
	send_con_admin(con,rtn_str);
	
	//now tell the character
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Marketing script payout Exp:%d DC:%d ..", amount, dc_amount);
		send_user_private_chat(user[u].con_id,1,rtn_str);
		
		user_send_money(user[u].con_id);
		user_send_exp(u);
	}
    
    //do this
	game.rewrite_user_db = 1;
}

void admin_global_message(int con, char *message, int point)
{
	int i;
	char temp_str[1025];
	char rtn_str[1025];
	
	right(temp_str,message,point),
	
	//prepare message
	sprintf(rtn_str,"32,%d,... Admin Message : %s ..%c", 1, temp_str, 13);
	
	for(i=0;i<=user_con_top;i++)
		if(user_con_id[i] > -1)
			raw_send_con_user(i, rtn_str);
	
	sprintf(rtn_str,"1,message relayed: %s", message);
	send_con_admin(con,rtn_str);
}

void admin_clean_all_decos(int con)
{
	int i;
	char rtn_str[1025];
	
	//do the users
	for(i=0;i<=user_max;i++)
		user_clear_decorations(i);
	
	//do the guilds
	for(i=0;i<=guild_max;i++)
		guild_clear_decorations(i);
	
	game.rewrite_user_db = 1;
	game.rewrite_guild_db = 1;
	
	sprintf(rtn_str,"1,They cleaned me lord.");
	send_con_admin(con,rtn_str);
}//admin_do_guild_salery

void admin_do_guild_salery(int con)
{
	int g;
	char rtn_str[1025];
	
	for(g=0;g<=guild_max;g++)
		if(guild[g].name[0])
			guild_pay_ranks(g);
	
	game.rewrite_guild_db = 1;
	
	sprintf(rtn_str,"1,They paid me lord.");
	send_con_admin(con,rtn_str);
}

//admin_change_guild_name
void admin_change_guild_name(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i;
     
	int g;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	//guildname , newname
	
	g = guild_name_to_id(temp_str[0]);
	
	if(g == -1)
	{
		sprintf(rtn_str,"1,guild not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//does the new name exist?
	if(guild_name_to_id(temp_str[1]) != -1)
	{
		sprintf(rtn_str,"1,new guild name already exists");
		send_con_admin(con,rtn_str);
		return;
	}
	
	strcpy(guild[g].name, temp_str[1]);
	
	sprintf(rtn_str,"1,guild %d:%s changed to %s", g, temp_str[0], guild[g].name);
	send_con_admin(con,rtn_str);
	
	game.rewrite_guild_db = 1;
}

void admin_change_guild_owner(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i;
     
	int g, u;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	
	g = guild_name_to_id(temp_str[0]);
	
	if(g == -1)
	{
		sprintf(rtn_str,"1,guild not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	
	u = user_to_id(temp_str[1]);
	
	if(u == -1)
	{
		sprintf(rtn_str,"1,user not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//set
	strcpy(guild[g].owner, user[u].username);
	
	//know
	sprintf(rtn_str,"1,guild %d:%s changed to %d:%s", g, guild[g].name, u, user[u].username);
	send_con_admin(con,rtn_str);
	
	game.rewrite_guild_db = 1;
}

void admin_change_user_password(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i;
     
	int u;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	
	if(u == -1)
	{
		sprintf(rtn_str,"1,user not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//cap
	temp_str[1][20] = 0;
	
	//set
	strcpy(user[u].password, temp_str[1]);
	
	//know
	sprintf(rtn_str,"1,user %d:%s password to %s", u, user[u].username, temp_str[1]);
	send_con_admin(con,rtn_str);
	
	//this
	game.rewrite_user_db = 1;
}

void admin_change_user_login(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i;
     
	int u, v_u;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	
	if(u == -1)
	{
		sprintf(rtn_str,"1,user not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	v_u = user_login_to_id(temp_str[1]);
	
	if(v_u > -1)
	{
		sprintf(rtn_str,"1,login already used");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//cap
	temp_str[1][20] = 0;
	lcase(temp_str[1],temp_str[1]);
	
	//set
	strcpy(user[u].login, temp_str[1]);
	
	//know
	sprintf(rtn_str,"1,user %d:%s login to %s", u, user[u].username, temp_str[1]);
	send_con_admin(con,rtn_str);
	
	//this
	game.rewrite_user_db = 1;
}

void admin_change_user_name(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i;
     
	int u, v_u;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	
	if(u == -1)
	{
		sprintf(rtn_str,"1,user not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	v_u = user_to_id(temp_str[1]);
	
	if(v_u > -1)
	{
		sprintf(rtn_str,"1,new name already used");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//cap
	temp_str[1][20] = 0;
	
	//set
	strcpy(user[u].username, temp_str[1]);
	
	//know
	sprintf(rtn_str,"1,user %d:%s username to %s", u, user[u].username, temp_str[1]);
	send_con_admin(con,rtn_str);
	
	//this
	game.rewrite_user_db = 1;
}
void admin_insert_all_jabber(int con)
{
	char rtn_str[2000];
	
	//do
	insert_all_users_jabber();
	
	//know
	sprintf(rtn_str,"1,all put in...");
	send_con_admin(con,rtn_str);
}
//admin_view_guild_bank_stats
void admin_view_guild_bank_stats(int con, char *message, int point)
{
	const int input_amount = 1;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int debt, credit;
	int i;
     
	int g;
     
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	g = guild_name_to_id(temp_str[0]);
	
	if(g == -1)
	{
		sprintf(rtn_str,"1,guild not found");
		send_con_admin(con,rtn_str);
		return;
	}
	
	debt = 0;
	credit = 0;
	for(i=0;i<=user_max;i++)
		if(user[i].guild_id == g)
	{
		sprintf(rtn_str,"1,guild:%s user:%s balance:%d", guild[g].name, user[i].username, user[i].guild_balance);
		send_con_admin(con,rtn_str);
		
		if(user[i].guild_balance < 0)
			debt -= user[i].guild_balance;
		else
			credit += user[i].guild_balance;
	}
	
	sprintf(rtn_str,"1,guild:%s money:%d debt:%d credit:%d difference:%d", guild[g].name, guild[g].bank_money, debt, credit, guild[g].bank_money - credit + debt);
	send_con_admin(con,rtn_str);
}

void admin_force_mfc_payout(int con)
{
	char rtn_str[5000];
	
	//do it monkey asshole
	mysql_do_mfc_awards();
	
	sprintf(rtn_str,"1,The deed was done, mfc monkey.");
	send_con_admin(con,rtn_str);
}

void admin_list_all_standards(int con)
{
	char rtn_str[5000];
	int u;
	
	for(u=0;u<=user_max;u++)
		if(user[u].standards > 0)
	{
		sprintf(rtn_str,"1,%s :: %d DS", user[u].username, user[u].standards);
		send_con_admin(con,rtn_str);
	}
}

//admin_award_user_standards
void admin_award_user_standards(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	
	int i, u, amount;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		lcase(temp_str[i],temp_str[i]);
		if(!temp_str[i][0]) return;
	}
		
	u = user_to_id(temp_str[0]);
	amount = atoi(temp_str[1]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//do the deed
	user[u].standards += amount;
	
	//now tell the character
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Admin awarded DS:%d ..", amount);
		send_user_private_chat(user[u].con_id,1,rtn_str);
	}
	
	//send the info
	sprintf(rtn_str,"1,user awarded %d DS, now has %d DS", amount, user[u].standards);
	send_con_admin(con,rtn_str);
	
	//do this
	game.rewrite_user_db = 1;
}

void admin_set_hub_name(int con, char *message, int point)
{
	const int input_amount = 1;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	char *str_toked;
	int i;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	//cap
	temp_str[0][20] = 0;
	
	//junk
	dejunk_bytes(temp_str[0], 20);
	
	//tok
	
	if(str_toked = strtok(temp_str[0], "\n\r\t:"))
	{
		strcpy(game.hub_name, str_toked);
		write_hub_db();
		
		sprintf(rtn_str,"1,Hub name set to::%s", game.hub_name);
		send_con_admin(con,rtn_str);
	}
	else
	{
		sprintf(rtn_str,"1,Bad hub name");
		send_con_admin(con,rtn_str);
	}
}

void admin_give_barcodes(int con, char *message, int point)
{
	char rtn_str[2000];
	int u;
	
	for(u=0;u<=user_max;u++)
		if(user_likely_barcode(user[u].username))
	{
		sprintf(rtn_str,"1,Likely barcode name::%s::", user[u].username);
		send_con_admin(con,rtn_str);
	}
}

void admin_list_user_hanger(int con, char *message, int point) //award_user_gun
{
	const int input_amount = 1;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	char ship_str[50];
	char *str_toked;
	int i, k, u;
	
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	sprintf(rtn_str,"1,user's hanger...");
	send_con_admin(con,rtn_str);
	
	for(k=0;k<10;k++)
	{
		char full_ship_name[100];
		int s, s_c, s_k;
		
		s = user[u].ship[k].ship_id;
		s_c = user[u].ship[k].brand;
		s_k = user[u].ship[k].type;
		
		if(s == -1) continue;
		if(s_c == -1) continue;
		if(s_k == -1) continue;
		
		sprintf(full_ship_name, "%s %s", server[s].ship[s_c].name, ship_name(s_k, ship_str));
		sprintf(rtn_str,"1,hanger ship:%d: %-42s from %s", k, full_ship_name, server[s].servername);
		send_con_admin(con,rtn_str);
	}
}

void admin_award_user_gun(int con, char *message, int point)
{
	const int input_amount = 4;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int u, sh, s, w, max;
	int sh_s, sh_c, sh_k;
	int i, k;
	
	//award_user_gun,username,ship_hanger_number,servername,gun_number
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	sh = atoi(temp_str[1]);
	s = server_to_id(temp_str[2]);
	w = atoi(temp_str[3]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(s==-1) 
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//simple checks
	if(sh < 0) return;
	if(sh >= 10) return;
	if(w < 0) return;
	if(w >= GUN_MAX) return;
	
	//gun exist?
	if(!server[s].gun[w].name[0])
	{
		sprintf(rtn_str,"1,gun not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//ship there?
	sh_s = user[u].ship[sh].ship_id;
	sh_c = user[u].ship[sh].brand;
	sh_k = user[u].ship[sh].type;
	if(sh_s == -1 || sh_c == -1 || sh_k == -1)
	{
		sprintf(rtn_str,"1,ship not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//find empty spot
	max = server[sh_s].ship[sh_c].gun_ammount[sh_k];
	for(k=0;k<max;k++) //find first avaible gun slot...
		if (user[u].ship[sh].gun_id[k] == -1 || user[u].ship[sh].gun[k] == -1) 
			break;

	if (k==max) return; //if no available missile slots...
			
	//give it!
	user_award_gun(u, sh, s, w, k);
	
	//notify
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Admin Awarded Item: The %s gun ..", server[s].gun[w].name);
		send_user_private_chat(user[u].con_id, 1, rtn_str);
	}
	
	sprintf(rtn_str,"1,user awarded the %s gun.", server[s].gun[w].name);
	send_con_admin(con,rtn_str);
}

void admin_award_user_missile(int con, char *message, int point)
{
	const int input_amount = 4;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int u, sh, s, w, max;
	int sh_s, sh_c, sh_k;
	int i, k;
	
	//award_user_gun,username,ship_hanger_number,servername,gun_number
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	sh = atoi(temp_str[1]);
	s = server_to_id(temp_str[2]);
	w = atoi(temp_str[3]);
	
	if(u==-1) 
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(s==-1) 
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//simple checks
	if(sh < 0) return;
	if(sh >= 10) return;
	if(w < 0) return;
	if(w >= MISSILE_MAX) return;
	
	//missile exist?
	if(!server[s].missile[w].name[0])
	{
		sprintf(rtn_str,"1,missile not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//ship there?
	sh_s = user[u].ship[sh].ship_id;
	sh_c = user[u].ship[sh].brand;
	sh_k = user[u].ship[sh].type;
	if(sh_s == -1 || sh_c == -1 || sh_k == -1)
	{
		sprintf(rtn_str,"1,ship not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//find empty spot
	max = server[sh_s].ship[sh_c].missile_ammount[sh_k];
	for(k=0;k<max;k++) //find first avaible gun slot...
		if (user[u].ship[sh].missile_id[k] == -1 || user[u].ship[sh].missile[k] == -1) 
			break;

	if (k==max) return; //if no available missile slots...
			
	//give it!
	user_award_missile(u, sh, s, w, k);
	
	//notify
	if(user[u].con_id > -1)
	{
		sprintf(rtn_str,"... Admin Awarded Item: The %s missile ..", server[s].missile[w].name);
		send_user_private_chat(user[u].con_id, 1, rtn_str);
	}
	
	sprintf(rtn_str,"1,user awarded the %s missile.", server[s].missile[w].name);
	send_con_admin(con,rtn_str);
}

void admin_set_user_guild_rank(int con, char *message, int point)
{
	//user_name,rank_number
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i, u, rank, old_rank;
	
	//award_user_gun,username,ship_hanger_number,servername,gun_number
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	u = user_to_id(temp_str[0]);
	rank = atoi(temp_str[1]);
	
	if(rank<0) rank = 0;
	if(rank>5) rank = 5;
	
	if(u==-1)
	{
		sprintf(rtn_str,"1,user not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	//set it then
	old_rank = user[u].guild_rank;
	user[u].guild_rank = rank;
	
	//display
	sprintf(rtn_str,"1,user %s guild rank set to %d from %d.", user[u].username, user[u].guild_rank, old_rank);
	send_con_admin(con,rtn_str);
}

void admin_give_spacenames(int con, char *message, int point)
{
	char rtn_str[2000];
	int u;
	
	for(u=0;u<=user_max;u++)
		if(user_bad_spacing(user[u].username))
	{
		sprintf(rtn_str,"1,bad space name::%s::", user[u].username);
		send_con_admin(con,rtn_str);
	}
}

void admin_onlinenoobs(int con, char *message, int point)
{
	char rtn_str[2000];
	int u;
	int the_time = time(0);
	const int max_time = 60 * 60 * 24 * 3;
	const int max_exp = 200;
	
	for(u=user_max;u>=0;u--)
	{
		int hours_ago;
		int minutes_ago;
		int time_difference;
		
		if(the_time - user[u].time_created > max_time) break;
		if(user[u].exp > max_exp) continue;
		if(user[u].con_id == -1) continue;
		
		time_difference = the_time - user[u].time_created;
		hours_ago = time_difference / 3600;
		minutes_ago = (time_difference % 3600) / 60;
		
		sprintf(rtn_str,"1,online noob::%s exp::%d created_ago::%d:%d", user[u].username, user[u].exp, hours_ago, minutes_ago);
		send_con_admin(con,rtn_str);
	}
}

void admin_list_noobs(int con, char *message, int point)
{
	char rtn_str[2000];
	int u;
	int the_time = time(0);
	const int max_time = 60 * 60 * 24 * 1;
	const int max_exp = 500;
	
	for(u=user_max;u>=0;u--)
	{
		int hours_ago;
		int minutes_ago;
		int time_difference;
		
		if(the_time - user[u].time_created > max_time) break;
		if(user[u].exp > max_exp) continue;
		
		time_difference = the_time - user[u].time_logged;
		hours_ago = time_difference / 3600;
		minutes_ago = time_difference % 3600;
		
		sprintf(rtn_str,"1,noob::%s exp::%d logged_ago::%d:%d", user[u].username, user[u].exp, hours_ago, minutes_ago);
		send_con_admin(con,rtn_str);
	}
}

void admin_destroy_sat(int con, char *message, int point)
{
	const int input_amount = 2;
	char temp_str[input_amount][1024];
	char rtn_str[2000];
	int i;
	int s, p, g, lvl;
	
	//admin_destroy_sat,servername,planetname
	for(i=0;i<input_amount;i++)
	{
		split(temp_str[i],message,',',&point);
		if(!temp_str[i][0]) return;
	}
	
	s = server_to_id(temp_str[0]);
	
	if(s==-1) 
	{
		sprintf(rtn_str,"1,server not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	p = planet_to_id(s, temp_str[1]);
	
	if(p==-1) 
	{
		sprintf(rtn_str,"1,planet not found.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	if(!server[s].planet[p].sat.hull)
	{
		sprintf(rtn_str,"1,sat doesn't have any hull.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	g = server[s].planet[p].sat.guild_id;
	
	if(g == -1)
	{
		sprintf(rtn_str,"1,sat doesn't have a guild.");
		send_con_admin(con,rtn_str);
		return;
	}
	
	lvl = server[s].planet[p].sat.level;
	
	//destroy it
	destroy_sat(s,p);
	
	sprintf(rtn_str,"1,Satellite Destroyed:%s-%s  previously owned by:%s level:%d\n", server[s].servername, server[s].planet[p].name, guild[g].name, lvl);
	send_con_admin(con,rtn_str);
	return;
}
