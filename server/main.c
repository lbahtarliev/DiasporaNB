#include "main.h"

void init_memory();
void main_loop();

int main()
{
	init_memory();
	read_all_db();
	connect_to_hub();
	start_server();
	start_gun_thread();
	start_bot_thread();
	start_sun_thread();
	main_loop();
	return 0;
}

void init_memory()
{
	srand(time(0));
	
	user_con_top = -1;
	
	clear_sector_goods();
	
	clear_all_sat_info();
	
	set_default_sat_shields();
}

void main_loop()
{
	int last_user_ping_time;
	int last_hub_ping_time;
	int last_bot_clear_goods_time;
	int the_time;
	
	last_user_ping_time = time(0);
	last_hub_ping_time = last_user_ping_time;
	last_bot_clear_goods_time = last_user_ping_time;
	
	while(1)
	{
		the_time = time(0);
		
		if(the_time - last_user_ping_time >= PING_USER_DELAY_SECONDS)
		{
			ping_all_users();
			last_user_ping_time = the_time;
		}
		
		if(the_time - last_hub_ping_time >= PING_HUB_DELAY_SECONDS)
		{
			ping_hub();
			last_hub_ping_time = the_time;
		}
		
		if(the_time - last_bot_clear_goods_time >= CLEAR_BOT_GOODS_DELAY_SECONDS)
		{
			bot_clear_all_bot_goods();
			last_bot_clear_goods_time = the_time;
		}

#ifdef _WIN32 //if windows
		Sleep(1000);	//windows
#else
		sleep(1);	//linux
#endif
	}
}
