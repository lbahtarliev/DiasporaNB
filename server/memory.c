#include "main.h"

struct server_db server_info;
struct server_planet_db planet[PLANET_MAX];
struct server_zone_db zone[ZONE_MAX];
struct server_ship_db ship[SHIP_MAX];
struct server_bot_type_db bot_type[BOT_TYPE_MAX];
struct server_gun_db gun[GUN_MAX];
struct server_missile_db missile[MISSILE_MAX];

struct con_user_info user_con[MAX_USER_CON];
int user_con_top;

//sat shit
double sat_default_gun_evasion;
double sat_default_gun_reduction;
double sat_default_missile_evasion;
double sat_default_missile_reduction;

void clear_sector_goods()
{
	int i,k, j;
	
	for(i=0;i<ZONE_MAX;i++)
		for(k=0;k<SECTOR_MAX;k++)
			for(j=0;j<10;j++)
	{
		zone[i].sector[k].contra_amount[j] = 0;
		zone[i].sector[k].good_amount[j] = 0;
	}
}

void set_default_sat_shields()
{
	sat_default_gun_evasion = (100 - SAT_DEFAULT_EVASION) * 0.01;
	sat_default_gun_reduction = ((100 - (SAT_DEFAULT_SHIELD / 2)) * 0.01) * 0.25;
	sat_default_missile_evasion = sat_default_gun_reduction * 3;
	sat_default_missile_reduction = sat_default_missile_evasion / 2;
	
	sat_init_shields();
}

void reset_all_runes()
{
	int s, z;
	int s_z[ZONE_MAX * SECTOR_MAX], s_s[ZONE_MAX * SECTOR_MAX];
	int s_max;
	int c, i;
	
	s_max = 0;
	
	for(z=0;z<ZONE_MAX;z++)
		if(zone[z].name[0])
			for(s=0;s<SECTOR_MAX;s++)
				if(zone[z].sector[s].x)
	{
		//set sector
		s_z[s_max] = z;
		s_s[s_max] = s;
		s_max++;
		
		//rape the sector
		zone[z].sector[s].has_rune[0] = 0;
		zone[z].sector[s].has_rune[1] = 0;
		zone[z].sector[s].has_rune[2] = 0;
		zone[z].sector[s].has_rune[3] = 0;
	}
	
	//now set some monkies
	if(!s_max) return;
	
	for(i=0;i<4;i++)
	{
		c = rand() % s_max;
		z = s_z[c];
		s = s_s[c];
		
		zone[z].sector[s].has_rune[i] = 1;
	}
}
