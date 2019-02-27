#include "main.h"

void initialize_memory();
void do_sleep();

int main()
{
	initialize_memory();
	read_all_db();
	//write_user_emails();
	read_all_server_db();
	regulate_all_server_sat_bank_maxs();
	mysql_write_all_tables();
	check_and_create_bot_guilds();
	configure_user_ships();
	clear_all_user_private_chats();
	do_price_change();
	atexit(check_databases_for_save);
	start_server();
	//do_sleep();
	return 0;
}

//------------------------------------------------------------------------------------------------

void initialize_memory()
{
	int i, k;
	
	srand(time(0));
	
	set_trade_disaster_texts();
	
	clear_all_user_uni_chat();
	
	mysql_init_vars();
	
	
	for(i=0;i<MAX_ADMIN;i++)	//can't have these users starting with a "connection"
	{
		admin[i].con_id = -1;
		admin[i].user_debug = -1;
		admin[i].server_debug = -1;
	}
		
	for(i=0;i<MAX_USER;i++)
	{
		user[i].con_id = -1;
		user[i].debug = -1;
	}
		
	for(i=0;i<MAX_SERVER;i++)
	{
		server[i].con_id = -1;
		server[i].debug = -1;
		server[i].rewrite_sat_db = 0;
	}
		
	admin_con_top = -1;
	user_con_top = -1;
	server_con_top = -1;
	
	game.rewrite_user_db = 0;
	game.rewrite_guild_db = 0;
	game.rewrite_bounty_db = 0;
}


void do_sleep()
{
	int ticker;
	
	for(ticker=0;1;ticker++)
	{
		switch(ticker)
		{
			case 15:
                try{check_databases_for_save();} catch(int e) {printd("crash: check_database");}
				break;
			case 30:
                try{check_databases_for_save();} catch(int e) {printd("crash: check_database");}
				break;
			case 45:
                try{check_databases_for_save();} catch(int e) {printd("crash: check_database");}
				break;
			case 60:
                try{check_databases_for_save();} catch(int e) {printd("crash: check_database");}
				
				try{do_price_change();} catch(int e) {printd("crash: price_change");}
				try{reduce_bounty_lifetime();} catch(int e) {printd("crash: reduce_bounty_time");}
				try{award_all_sat_taxes();} catch(int e) {printd("crash: award_sat_taxes");}
				try{pay_all_guilds();} catch(int e) {printd("crash: reduce_bounty_time");}
				try{mysql_do_mfc_awards();} catch(int e) {printd("crash: mysql_do_mfc_awards");}
				write_user_db(1);
				write_guild_db(1);
				ticker = 0;
				break;
		}
		
		if(ticker % 5 == 0)
		{
			try{mysql_do_ds_awards();} catch(int e) {printd("crash: mysql_do_ds_awards");}
		}
		
		//happens every minute
		try{check_and_repair_all_sats();} catch(int e) {printd("crash: repair_sata");}
		
#ifdef _WIN32 //if windows
		Sleep(60000);	//win version
#else
		sleep(60);	//lin version
#endif
	}
}

void main_loop()
{
	static int last_minute_time = 0;
	static int ticker = 0;
	static int last_user_ping_time;
	static int last_user_server_time;
	int the_time;
	
	the_time = time(0);
	
	//init
	if(!last_minute_time)
	{
		last_minute_time = the_time;
		last_user_ping_time = last_minute_time;
		last_user_server_time = last_minute_time;
	}
	
	//time to ping the users?
	if(the_time - last_user_ping_time >= PING_USER_DELAY_SECONDS)
	{
		ping_all_users();
		last_user_ping_time = the_time;
	}
	
	//time to ping the servers?
	if(the_time - last_user_server_time >= PING_SERVER_DELAY_SECONDS)
	{
		ping_all_servers();
		last_user_server_time = the_time;
	}
	
	if(the_time - last_minute_time >= 60)
	{
		ticker++;
		last_minute_time = the_time;
		
		switch(ticker)
		{
			case 15:
				check_databases_for_save();
				break;
			case 30:
				check_databases_for_save();
				break;
			case 45:
				check_databases_for_save();
				break;
			case 60:
				check_databases_for_save();
				
				do_price_change();
				reduce_bounty_lifetime();
				award_all_sat_taxes();
				pay_all_guilds();
				mysql_do_mfc_awards();
				write_user_db(1);
				write_guild_db(1);
				ticker = 0;
				break;
		}
		
		if(ticker % 5 == 0)
			mysql_do_ds_awards();
		
		//happens every minute
		check_and_repair_all_sats();
	}
}
