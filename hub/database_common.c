#include "main.h"

void clear_planet_db ( struct server_planet_db *the_db, int *home_class )
{
	int i, k, z;

	*home_class = 0;

	for ( k=0;k<PLANET_MAX;k++ ) //clear out previous galaxy db
	{
		the_db[k].name[0] = '\0';
		the_db[k].guild[0] = '\0';
		the_db[k].bar_name[0] = '\0';
		the_db[k].is_gate = 0;
		the_db[k].is_server_gate = 0;
		the_db[k].has_guild_halls = 0;
		the_db[k].ship_plan = -1;
		the_db[k].ship_plan_cost = 0;
		the_db[k].ship_plan_exp = 0;
		the_db[k].ship_plan_kills = 0;

		for ( z=0;z<SHIP_MAX;z++ )
		{
			the_db[k].ship[z].have = 0;

			for ( i=0;i<8;i++ )
				the_db[k].ship[z].exclude[i] = 0;
		}
		for ( z=0;z<GUN_MAX;z++ )
			the_db[k].gun[z] = 0;
		for ( z=0;z<MISSILE_MAX;z++ )
			the_db[k].missile[z] = 0;

		for ( z=0;z<PLANET_TRADE_MAX;z++ )
			the_db[k].trade_planet[z] = -1;
	}
}

void read_planet_db_entry ( struct server_planet_db *the_db, int *home_class, char *input_line )
{
	const int inputs = 82;
	char cur_line[1024] = "", c, temp_str[inputs][21];
	int len, k, z, point, temp_planet, temp_is_gate, temp_ship, temp_ship_max, temp_wep;

	len = strlen ( input_line );

	if ( !len ) return;

	for ( k=0;k<inputs;k++ )
		temp_str[k][0] = 0;
	for ( point=k=0;point<=len && k<inputs;k++ )
		split ( temp_str[k],input_line,',',&point );

	temp_planet = atoi ( temp_str[0] );
	temp_is_gate = atoi ( temp_str[1] );

	if ( temp_is_gate == -1 ) //is planet ship info
	{
		if ( temp_planet >=0 && temp_planet < PLANET_MAX )
		{
			temp_ship = atoi ( temp_str[2] );
			temp_ship_max = atoi ( temp_str[3] );

			if ( temp_ship >=0 && temp_ship < SHIP_MAX )
			{
				the_db[temp_planet].ship[temp_ship].have = 1;
				if ( temp_ship_max < 0 ) temp_ship_max = 0;
				if ( temp_ship_max > 7 ) temp_ship_max = 7;
				the_db[temp_planet].ship[temp_ship].max = temp_ship_max;

				for ( z=0;z<8;z++ )
					the_db[temp_planet].ship[temp_ship].exclude[z] = atoi ( temp_str[z+4] );
			}
		}
	}
	else
		if ( temp_is_gate == -2 ) //is planet gun info
		{
			if ( temp_planet >=0 && temp_planet < PLANET_MAX )
			{
				temp_wep = atoi ( temp_str[2] );
				if ( temp_wep >=0 && temp_wep < GUN_MAX )
					the_db[temp_planet].gun[temp_wep] = 1;
			}
		}
		else
			if ( temp_is_gate == -3 ) //is planet missile info
			{
				if ( temp_planet >=0 && temp_planet < PLANET_MAX )
				{
					temp_wep = atoi ( temp_str[2] );
					if ( temp_wep >=0 && temp_wep < MISSILE_MAX )
						the_db[temp_planet].missile[temp_wep] = 1;
				}
			}
			else
				if ( temp_is_gate == -4 ) //planet good info
				{
					if ( temp_planet >=0 && temp_planet < PLANET_MAX )
						for ( k=0;k<10;k++ )
						{
							the_db[temp_planet].good_price_high[k]		= atoi ( temp_str[k + 2] );
							the_db[temp_planet].good_price_low[k]		= atoi ( temp_str[k + 12] );
							the_db[temp_planet].contra_price_high[k]	= atoi ( temp_str[k + 22] );
							the_db[temp_planet].contra_price_low[k]		= atoi ( temp_str[k + 32] );
							the_db[temp_planet].good_ammount_high[k]	= atoi ( temp_str[k + 42] );
							the_db[temp_planet].good_ammount_low[k]		= atoi ( temp_str[k + 52] );
							the_db[temp_planet].contra_ammount_high[k]	= atoi ( temp_str[k + 62] );
							the_db[temp_planet].contra_ammount_low[k]	= atoi ( temp_str[k + 72] );
						}
				}
				else
					if ( temp_is_gate == -5 ) //home class
					{
						*home_class = atoi ( temp_str[2] );
					}
					else if ( temp_is_gate == -6 ) //home class
					{
						for ( k=0;k<PLANET_TRADE_MAX;k++ )
							if ( temp_str[k + 2][0] )
								the_db[temp_planet].trade_planet[k] = atoi ( temp_str[k + 2] );
					}
					else //planet#,is_gate,is_home,name,guild,class,bar_name,ship_plan
					{
						if ( temp_planet >=0 && temp_planet < PLANET_MAX )
						{
							//cappin
							temp_str[3][20] = 0;
							temp_str[4][20] = 0;
							temp_str[6][20] = 0;

							the_db[temp_planet].is_gate = temp_is_gate;
							the_db[temp_planet].is_home = atoi ( temp_str[2] );
							strcpy ( the_db[temp_planet].name,temp_str[3] );
							strcpy ( the_db[temp_planet].guild,temp_str[4] );
							the_db[temp_planet].home_class = atoi ( temp_str[5] );
							strcpy ( the_db[temp_planet].bar_name,temp_str[6] );
							the_db[temp_planet].has_guild_halls = atoi ( temp_str[7] );
							the_db[temp_planet].is_server_gate = atoi ( temp_str[8] );
							the_db[temp_planet].ship_plan = atoi ( temp_str[9] );
							the_db[temp_planet].ship_plan_cost = atoi ( temp_str[10] );
							the_db[temp_planet].ship_plan_exp = atoi ( temp_str[11] );
							the_db[temp_planet].ship_plan_kills = atoi ( temp_str[12] );
							
						}
					}
}

void clear_galaxy_db ( struct server_zone_db *the_db )
{
	int k, z;

	for ( k=0;k<ZONE_MAX;k++ ) //clear out previous galaxy db
	{
		the_db[k].name[0] = '\0';
		the_db[k].home_class = 0;
		the_db[k].free_scan = 0;
		for ( z=0;z<SECTOR_MAX;z++ )
		{
			the_db[k].sector[z].planet = -1;
			the_db[k].sector[z].shield = 0;
			the_db[k].sector[z].x = 0;
			the_db[k].sector[z].y = 0;
			the_db[k].sector[z].sector[0] = -1;
			the_db[k].sector[z].sector[1] = -1;
			the_db[k].sector[z].sector[2] = -1;
			the_db[k].sector[z].sector[3] = -1;
			the_db[k].sector[z].sector[4] = -1;
			the_db[k].sector[z].range = 0;
			the_db[k].sector[z].is_sun = 0;
			the_db[k].sector[z].is_nebula = 0;
			the_db[k].sector[z].free_scan = 0;
			the_db[k].sector[z].has_player = 0;
			the_db[k].sector[z].has_guild_member = 0;
		}
	}
}

void read_galaxy_db_entry ( struct server_zone_db *the_db, char *input_line )
{
	const int inputs = 15;
	int len;
	char cur_line[1024] = "", c, temp_str[inputs][21];
	int i =0, k, z, point, cur_line_num = -1, temp_zone, temp_sector;

	len = strlen ( input_line );

	if ( !len ) return;

	for ( k=0;k<inputs;k++ )
		temp_str[k][0] = 0;
	for ( point=k=0;point<=len && k<inputs;k++ )
		split ( temp_str[k],input_line,',',&point );

	temp_zone = atoi ( temp_str[0] );
	temp_sector = atoi ( temp_str[1] );

	if ( temp_sector == -1 ) //is zone name or is sector info
	{
		if ( temp_zone >=0 && temp_zone < ZONE_MAX )
		{
			temp_str[2][20] = '\0';
			strcpy ( the_db[temp_zone].name,temp_str[2] );
			the_db[temp_zone].home_class = atoi ( temp_str[3] );
			the_db[temp_zone].free_scan = atoi ( temp_str[4] );
		}
	}
	else
	{
		if ( ( temp_zone >=0 && temp_zone < ZONE_MAX ) && ( temp_sector >=0 && temp_sector < SECTOR_MAX ) )
		{
			the_db[temp_zone].sector[temp_sector].planet	= atoi ( temp_str[2] );
			the_db[temp_zone].sector[temp_sector].shield	= atoi ( temp_str[3] );
			the_db[temp_zone].sector[temp_sector].x		= atoi ( temp_str[4] );
			the_db[temp_zone].sector[temp_sector].y		= atoi ( temp_str[5] );
			the_db[temp_zone].sector[temp_sector].sector[0]	= atoi ( temp_str[6] );
			the_db[temp_zone].sector[temp_sector].sector[1]	= atoi ( temp_str[7] );
			the_db[temp_zone].sector[temp_sector].sector[2]	= atoi ( temp_str[8] );
			the_db[temp_zone].sector[temp_sector].sector[3]	= atoi ( temp_str[9] );
			the_db[temp_zone].sector[temp_sector].sector[4]	= atoi ( temp_str[10] );
			the_db[temp_zone].sector[temp_sector].range	= atoi ( temp_str[11] );
			the_db[temp_zone].sector[temp_sector].is_sun	= atoi ( temp_str[12] );
			the_db[temp_zone].sector[temp_sector].is_nebula	= atoi ( temp_str[13] );
			the_db[temp_zone].sector[temp_sector].free_scan	= atoi ( temp_str[14] );
		}
	}
}

void clear_ship_db ( struct server_ship_db *the_db )
{
	int k, z;

	for ( k=0;k<SHIP_MAX;k++ ) //clear out previous galaxy db
	{
		the_db[k].name[0] = '\0';
		for ( z=0;z<8;z++ )
		{
			the_db[k].cost[z]			= 0;
			the_db[k].exp[z]			= 0;
			the_db[k].hull[z]			= 0;
			the_db[k].shield[z]		= 0;
			the_db[k].evasion[z]		= 0;
			the_db[k].speed[z]		= 0;
			the_db[k].range[z]		= 0;
			the_db[k].cargo[z]		= 0;
			the_db[k].gun_ammount[z]		= 0;
			the_db[k].missile_ammount[z]	= 0;
			the_db[k].misc_ammount[z]		= 0;
		}
	}
}

void read_ship_db_entry ( struct server_ship_db *the_db, char *input_line )
{
	const int inputs = 13;
	int len;
	char cur_line[1024] = "", c, temp_str[inputs][21];
	int i =0, k, z, point, cur_line_num = -1, temp_class, temp_kind;

	len = strlen ( input_line );

	if ( !len ) return;

	for ( k=0;k<inputs;k++ )
		temp_str[k][0] = 0;
	for ( point=k=0;point<=len && k<inputs;k++ )
		split ( temp_str[k],input_line,',',&point );

	temp_class = atoi ( temp_str[0] );
	temp_kind = atoi ( temp_str[1] );

	if ( temp_kind == -1 ) //is ship name or is ship info
	{
		if ( temp_class >=0 && temp_class < SHIP_MAX )
		{
			temp_str[2][20] = '\0';
			strcpy ( the_db[temp_class].name,temp_str[2] );
		}
	}
	else
	{
		if ( ( temp_class >=0 && temp_class < SHIP_MAX ) && ( temp_kind >=0 && temp_kind < 8 ) )
		{
			the_db[temp_class].cost[temp_kind]		= atoi ( temp_str[2] );
			the_db[temp_class].exp[temp_kind]			= atoi ( temp_str[3] );
			the_db[temp_class].hull[temp_kind]		= atoi ( temp_str[4] );
			the_db[temp_class].shield[temp_kind]		= atoi ( temp_str[5] );
			the_db[temp_class].evasion[temp_kind]		= atoi ( temp_str[6] );
			the_db[temp_class].speed[temp_kind]		= atoi ( temp_str[7] );
			the_db[temp_class].range[temp_kind]		= atoi ( temp_str[8] );
			the_db[temp_class].cargo[temp_kind]		= atoi ( temp_str[9] );
			the_db[temp_class].gun_ammount[temp_kind]		= atoi ( temp_str[10] );
			the_db[temp_class].missile_ammount[temp_kind]	= atoi ( temp_str[11] );
			the_db[temp_class].misc_ammount[temp_kind]	= atoi ( temp_str[12] );
		}
	}
}

void clear_bot_db ( struct server_bot_type_db *the_db )
{
	int k, j, z;

	for ( k=0;k<BOT_TYPE_MAX;k++ ) //clear out previous galaxy db
	{
		the_db[k].name[0] 	= '\0';
		the_db[k].guild[0] 	= '\0';
		the_db[k].bot_class 	= 0;
		the_db[k].aggression 	= 0;
		the_db[k].intelligence = 0;
		the_db[k].zone 	= -1;
		the_db[k].field_type 	= 0;
		the_db[k].drops_contraband 	= 0;
		the_db[k].is_smuggler 	= 0;

		for ( j=0;j<10;j++ )
		{
			the_db[k].contra_price[j] = 0;
			the_db[k].contra_price_min[j] = 0;
			the_db[k].contra_price_max[j] = 0;
		}

		for ( j=0;j<MAX_BOT_TYPE_DROP;j++ )
		{
			the_db[k].random_drop[j].drop_type = no_type;
			the_db[k].random_drop[j].ship_class = -1;
			the_db[k].random_drop[j].ship_kind = -1;
			the_db[k].random_drop[j].weapon = -1;
			the_db[k].random_drop[j].chance = 0.0;
		}


		for ( z=0;z<SECTOR_MAX;z++ )
			the_db[k].sector[z] = 0;

		for ( z=0;z<BOT_MAX;z++ )
		{
			the_db[k].bot[z].name[0] 	= '\0';
			the_db[k].bot[z].ship_class 	= -1;
			the_db[k].bot[z].ship_kind 	= -1;
			the_db[k].bot[z].gun 		= 0;
			the_db[k].bot[z].def_multi 	= 1;

			for ( j=0;j<MAX_BOT_DROP;j++ )
			{
				the_db[k].bot[z].random_drop[j].drop_type = no_type;
				the_db[k].bot[z].random_drop[j].ship_class = -1;
				the_db[k].bot[z].random_drop[j].ship_kind = -1;
				the_db[k].bot[z].random_drop[j].weapon = -1;
				the_db[k].bot[z].random_drop[j].chance = 0.0;
			}
		}


	}
}

void read_bot_db_entry ( struct server_bot_type_db *the_db, char *input_line )
{
	const int inputs = 8 + SECTOR_MAX + ( sizeof ( struct bot_drop_db ) * MAX_BOT_TYPE_DROP );
	int len;
	char cur_line[1024] = "", c, temp_str[inputs][21];
	int i, j, k, z, point, cur_line_num = -1, temp_bot_type, temp_bot, temp;

	len = strlen ( input_line );

	if ( !len ) return;

	for ( k=0;k<inputs;k++ )
		temp_str[k][0] = 0;
	for ( point=k=0;point<=len && k<inputs;k++ )
		split ( temp_str[k],input_line,',',&point );

	i = 0;
	temp_bot_type = atoi ( temp_str[i++] );
	temp_bot = atoi ( temp_str[i++] );

	if ( temp_bot == -1 ) //is zone name or is sector info
	{
		if ( temp_bot_type >= 0 && temp_bot_type < BOT_TYPE_MAX )
		{
			temp_str[2][20] = '\0';
			temp_str[3][20] = '\0';
			strcpy ( the_db[temp_bot_type].name,temp_str[i++] );
			strcpy ( the_db[temp_bot_type].guild,temp_str[i++] );
			the_db[temp_bot_type].bot_class 	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].aggression 	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].intelligence 	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].zone 		= atoi ( temp_str[i++] );
			the_db[temp_bot_type].field_type	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].drops_contraband	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].is_smuggler	= atoi ( temp_str[i++] );

			for ( j=0;j<10;j++ )
			{
				the_db[temp_bot_type].contra_price_min[j] = atoi ( temp_str[i++] );
				the_db[temp_bot_type].contra_price_max[j] = atoi ( temp_str[i++] );
			}

			for ( z=j=0;z<MAX_BOT_TYPE_DROP;z++ )
			{
				struct bot_drop_db temp_item;

				temp_item.ship_class = atoi ( temp_str[i++] );
				temp_item.ship_kind = atoi ( temp_str[i++] );
				temp_item.weapon = atoi ( temp_str[i++] );
				temp_item.chance = atof ( temp_str[i++] );
				temp_item.drop_type = ( enum BOT_DROP_TYPE ) atoi ( temp_str[i++] );

				if ( temp_item.drop_type != no_type )
					the_db[temp_bot_type].random_drop[j++] = temp_item;
			}

			while ( i<k )
			{
				int temp_i = i++;

				if ( !temp_str[temp_i][0] ) continue;

				temp = atoi ( temp_str[temp_i] );
				if ( temp >= 0 && temp < SECTOR_MAX )
					the_db[temp_bot_type].sector[temp] = 1;
			}
		}
	}
	else
	{
		if ( ( temp_bot_type >= 0 && temp_bot_type < BOT_TYPE_MAX ) && ( temp_bot >=0 && temp_bot < BOT_MAX ) )
		{
			temp_str[2][20] = '\0';
			strcpy ( the_db[temp_bot_type].bot[temp_bot].name, temp_str[i++] );
			the_db[temp_bot_type].bot[temp_bot].ship_class	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].bot[temp_bot].ship_kind	= atoi ( temp_str[i++] );
			the_db[temp_bot_type].bot[temp_bot].gun		= atoi ( temp_str[i++] );
			the_db[temp_bot_type].bot[temp_bot].def_multi	= atof ( temp_str[i++] );
			the_db[temp_bot_type].bot[temp_bot].exp		= atoi ( temp_str[i++] );


			for ( z=j=0;z<MAX_BOT_DROP;z++ )
			{
				struct bot_drop_db temp_item;

				temp_item.ship_class = atoi ( temp_str[i++] );
				temp_item.ship_kind = atoi ( temp_str[i++] );
				temp_item.weapon = atoi ( temp_str[i++] );
				temp_item.chance = atof ( temp_str[i++] );
				temp_item.drop_type = ( enum BOT_DROP_TYPE ) atoi ( temp_str[i++] );

				if ( temp_item.drop_type != no_type )
					the_db[temp_bot_type].bot[temp_bot].random_drop[j++] = temp_item;
			}

			//make valid
			if ( the_db[temp_bot_type].bot[temp_bot].ship_class < 0 )
				the_db[temp_bot_type].bot[temp_bot].name[0] = '\0';
			if ( the_db[temp_bot_type].bot[temp_bot].ship_class >= SHIP_MAX )
				the_db[temp_bot_type].bot[temp_bot].name[0] = '\0';
			if ( the_db[temp_bot_type].bot[temp_bot].ship_kind < 0 )
				the_db[temp_bot_type].bot[temp_bot].name[0] = '\0';
			if ( the_db[temp_bot_type].bot[temp_bot].ship_kind >= 8 )
				the_db[temp_bot_type].bot[temp_bot].name[0] = '\0';
			if ( the_db[temp_bot_type].bot[temp_bot].gun < 0 )
				the_db[temp_bot_type].bot[temp_bot].gun = 0;
			if ( the_db[temp_bot_type].bot[temp_bot].def_multi < 0.1 )
				the_db[temp_bot_type].bot[temp_bot].def_multi = 1;
		}
	}
}

void clear_gun_db ( struct server_gun_db *the_db )
{
	int k;

	for ( k=0;k<GUN_MAX;k++ ) //clear out previous galaxy db
	{
		the_db[k].name[0] 		= '\0';
		the_db[k].cost 		= 0;
		the_db[k].damage 		= 0;
		the_db[k].speed 		= 0;
		the_db[k].accuracy 	= 0;
		the_db[k].exp_static 	= 0;
		the_db[k].exp_added 	= 0;
		the_db[k].tech_level 	= 0;
		the_db[k].type 	= 0;
	}
}

void read_gun_db_entry ( struct server_gun_db *the_db, char *input_line )
{
	const int inputs = 10;
	int len;
	char cur_line[1024] = "", c, temp_str[inputs][21];
	int i =0, k, z, point, cur_line_num = -1, temp_gun;

	len = strlen ( input_line );

	if ( !len ) return;

	for ( k=0;k<inputs;k++ )
		temp_str[k][0] = 0;
	for ( point=k=0;point<=len && k<inputs;k++ )
		split ( temp_str[k],input_line,',',&point );

	temp_gun = atoi ( temp_str[0] );

	if ( temp_gun >=0 && temp_gun < GUN_MAX )
	{
		temp_str[1][20] = '\0';
		strcpy ( the_db[temp_gun].name,temp_str[1] );
		the_db[temp_gun].cost 	= atoi ( temp_str[2] );
		the_db[temp_gun].damage	= atoi ( temp_str[3] );
		the_db[temp_gun].speed	= atoi ( temp_str[4] );
		the_db[temp_gun].accuracy	= atoi ( temp_str[5] );
		the_db[temp_gun].exp_static	= atoi ( temp_str[6] );
		the_db[temp_gun].exp_added	= atoi ( temp_str[7] );
		the_db[temp_gun].tech_level	= atoi ( temp_str[8] );
		the_db[temp_gun].type	= atoi ( temp_str[9] );
	}
}

void clear_missile_db ( struct server_missile_db *the_db )
{
	int k;

	for ( k=0;k<MISSILE_MAX;k++ ) //clear out previous galaxy db
	{
		the_db[k].name[0] 	= '\0';
		the_db[k].cost 		= 0;
		the_db[k].damage 	= 0;
		the_db[k].speed 	= 0;
		the_db[k].accuracy 	= 0;
		the_db[k].exp_static 	= 0;
		the_db[k].exp_added 	= 0;
		the_db[k].tech_level 	= 0;
		the_db[k].type 	= 0;
	}
}

void read_missile_db_entry ( struct server_missile_db *the_db, char *input_line )
{
	const int inputs = 10;
	int len;
	char cur_line[1024] = "", c, temp_str[inputs][21];
	int i =0, k, z, point, cur_line_num = -1, temp_gun;

	len = strlen ( input_line );

	if ( !len ) return;

	for ( k=0;k<inputs;k++ )
		temp_str[k][0] = 0;
	for ( point=k=0;point<=len && k<inputs;k++ )
		split ( temp_str[k],input_line,',',&point );

	temp_gun = atoi ( temp_str[0] );

	if ( temp_gun >=0 && temp_gun < MISSILE_MAX )
	{
		temp_str[1][20] = '\0';
		strcpy ( the_db[temp_gun].name,temp_str[1] );
		the_db[temp_gun].cost 		= atoi ( temp_str[2] );
		the_db[temp_gun].damage		= atoi ( temp_str[3] );
		the_db[temp_gun].speed		= atoi ( temp_str[4] );
		the_db[temp_gun].accuracy	= atoi ( temp_str[5] );
		the_db[temp_gun].exp_static	= atoi ( temp_str[6] );
		the_db[temp_gun].exp_added	= atoi ( temp_str[7] );
		the_db[temp_gun].tech_level	= atoi ( temp_str[8] );
		the_db[temp_gun].type	= atoi ( temp_str[9] );
	}
}

//Now for writing functions --------------------------------------------------------

void write_planet_db_universal ( char *file_name, struct server_planet_db *the_db, int *home_class )
{
	int i, k, z;
	FILE *ofp;

	ofp = fopen ( file_name,"w" );

	fprintf ( ofp,"0,-5,%d%c", *home_class, 13 );

	for ( i=0;i<PLANET_MAX;i++ )
		if ( the_db[i].name[0] != '\0' )
		{
			fprintf ( ofp, "%d,%d,%d,%s,%s,%d,%s,%d,%d,%d,%d,%d,%d%c", 
					i, 
					the_db[i].is_gate, 
					the_db[i].is_home, 
					the_db[i].name, 
					the_db[i].guild, 
					the_db[i].home_class, 
					the_db[i].bar_name, 
					the_db[i].has_guild_halls, 
					the_db[i].is_server_gate, 
					the_db[i].ship_plan,
					the_db[i].ship_plan_cost,
					the_db[i].ship_plan_exp,
					the_db[i].ship_plan_kills,
					13 );
			for ( k=0;k<SHIP_MAX;k++ )
				if ( the_db[i].ship[k].have )
				{
					fprintf ( ofp, "%d,-1,%d,%d", i, k, the_db[i].ship[k].max );

					for ( z=0;z<8;z++ )
						fprintf ( ofp, ",%d", the_db[i].ship[k].exclude[z] );

					fprintf ( ofp, "%c", 13 );
				}
			for ( k=0;k<GUN_MAX;k++ )
				if ( the_db[i].gun[k] )
					fprintf ( ofp, "%d,-2,%d%c", i, k, 13 );
			for ( k=0;k<MISSILE_MAX;k++ )
				if ( the_db[i].missile[k] )
					fprintf ( ofp, "%d,-3,%d%c", i, k, 13 );

			fprintf ( ofp, "%d,-4,", i );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].good_price_high[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].good_price_low[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].contra_price_high[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].contra_price_low[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].good_ammount_high[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].good_ammount_low[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].contra_ammount_high[k] );
			for ( k=0;k<10;k++ )
				fprintf ( ofp, "%d,", the_db[i].contra_ammount_low[k] );

			fprintf ( ofp, "%c", 13 );

			//trade planets
			fprintf ( ofp, "%d,-6,", i );
			for ( k=0;k<PLANET_TRADE_MAX;k++ )
				fprintf ( ofp, "%d,", the_db[i].trade_planet[k] );

			fprintf ( ofp, "%c", 13 );

		}

	fclose ( ofp );
}

void write_galaxy_db_universal ( char *file_name, struct server_zone_db *the_db )
{
	int i, k;
	FILE *ofp;

	ofp = fopen ( file_name,"w" );

	for ( i=0;i<ZONE_MAX;i++ )
		if ( the_db[i].name[0] != '\0' ) //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
		{
			fprintf ( ofp, "%d,-1,%s,%d,%d%c", i, the_db[i].name, the_db[i].home_class, the_db[i].free_scan, 13 );
			for ( k=0;k<SECTOR_MAX;k++ )
				if ( ( the_db[i].sector[k].sector[0] >= 0 || the_db[i].sector[k].sector[1] >= 0 || the_db[i].sector[k].sector[2] >= 0 || the_db[i].sector[k].sector[3] >= 0 || the_db[i].sector[k].sector[4] >= 0 ) || the_db[i].sector[k].is_sun )
				{
					fprintf ( ofp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%c",
					          i,
					          k,
					          the_db[i].sector[k].planet,
					          the_db[i].sector[k].shield,
					          the_db[i].sector[k].x,
					          the_db[i].sector[k].y,
					          the_db[i].sector[k].sector[0],
					          the_db[i].sector[k].sector[1],
					          the_db[i].sector[k].sector[2],
					          the_db[i].sector[k].sector[3],
					          the_db[i].sector[k].sector[4],
					          the_db[i].sector[k].range,
					          the_db[i].sector[k].is_sun,
					          the_db[i].sector[k].is_nebula,
					          the_db[i].sector[k].free_scan,
					          13 );
				}
		}

	fclose ( ofp );
}

void write_ship_db_universal ( char *file_name, struct server_ship_db *the_db )
{
	int i, k;
	FILE *ofp;

	ofp = fopen ( file_name,"w" );

	for ( i=0;i<SHIP_MAX;i++ )
		if ( the_db[i].name[0] != '\0' ) //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
		{
			fprintf ( ofp, "%d,-1,%s%c", i, the_db[i].name, 13 );
			for ( k=0;k<8;k++ )
				fprintf ( ofp, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d%c",
				          i,
				          k,
				          the_db[i].cost[k],
				          the_db[i].exp[k],
				          the_db[i].hull[k],
				          the_db[i].shield[k],
				          the_db[i].evasion[k],
				          the_db[i].speed[k],
				          the_db[i].range[k],
				          the_db[i].cargo[k],
				          the_db[i].gun_ammount[k],
				          the_db[i].missile_ammount[k],
				          the_db[i].misc_ammount[k], 13 );
		}

	fclose ( ofp );
}

void write_bot_db_universal ( char *file_name, struct server_bot_type_db *the_db )
{
	FILE *ofp;
	int k, i, z;

	ofp = fopen ( file_name,"w" );

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( the_db[i].name[0] ) //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
		{
			fprintf ( ofp, "%d,-1,%s,%s,%d,%d,%d,%d,%d,%d,%d",
			          i,
			          the_db[i].name,
			          the_db[i].guild,
			          the_db[i].bot_class,
			          the_db[i].aggression,
			          the_db[i].intelligence,
			          the_db[i].zone,
			          the_db[i].field_type,
			          the_db[i].drops_contraband,
			          the_db[i].is_smuggler );

			for ( z=0;z<10;z++ )
			{
				fprintf ( ofp, ",%d", the_db[i].contra_price_min[z] );
				fprintf ( ofp, ",%d", the_db[i].contra_price_max[z] );
			}

			for ( z=0;z<MAX_BOT_TYPE_DROP;z++ )
			{
				fprintf ( ofp, ",%d", the_db[i].random_drop[z].ship_class );
				fprintf ( ofp, ",%d", the_db[i].random_drop[z].ship_kind );
				fprintf ( ofp, ",%d", the_db[i].random_drop[z].weapon );
				fprintf ( ofp, ",%f", the_db[i].random_drop[z].chance );
				fprintf ( ofp, ",%d", the_db[i].random_drop[z].drop_type );
			}

			for ( k=0;k<SECTOR_MAX;k++ )
				if ( the_db[i].sector[k] )
					fprintf ( ofp, ",%d", k );
			fprintf ( ofp, "%c", 13 );

			for ( k=0;k<BOT_MAX;k++ )
				if ( the_db[i].bot[k].name[0] )
				{
					fprintf ( ofp, "%d,%d,%s,%d,%d,%d,%f,%d",
					          i,
					          k,
					          the_db[i].bot[k].name,
					          the_db[i].bot[k].ship_class,
					          the_db[i].bot[k].ship_kind,
					          the_db[i].bot[k].gun,
					          the_db[i].bot[k].def_multi,
					          the_db[i].bot[k].exp );

					for ( z=0;z<MAX_BOT_DROP;z++ )
					{
						fprintf ( ofp, ",%d", the_db[i].bot[k].random_drop[z].ship_class );
						fprintf ( ofp, ",%d", the_db[i].bot[k].random_drop[z].ship_kind );
						fprintf ( ofp, ",%d", the_db[i].bot[k].random_drop[z].weapon );
						fprintf ( ofp, ",%f", the_db[i].bot[k].random_drop[z].chance );
						fprintf ( ofp, ",%d", the_db[i].bot[k].random_drop[z].drop_type );
					}

					fprintf ( ofp, "%c", 13 );
				}
		}

	fclose ( ofp );
}

void write_gun_db_universal ( char *file_name, struct server_gun_db *the_db )
{
	int i, k;
	FILE *ofp;

	ofp = fopen ( file_name,"w" );

	for ( i=0;i<GUN_MAX;i++ )
		if ( the_db[i].name[0] != '\0' ) //gun#,name,cost,damage,speed,accuracy
			fprintf ( ofp, "%d,%s,%d,%d,%d,%d,%d,%d,%d,%d%c",
			          i,
			          the_db[i].name,
			          the_db[i].cost,
			          the_db[i].damage,
			          the_db[i].speed,
			          the_db[i].accuracy,
			          the_db[i].exp_static,
			          the_db[i].exp_added,
			          the_db[i].tech_level,
			          the_db[i].type, 13 );

	fclose ( ofp );
}

void write_missile_db_universal ( char *file_name, struct server_missile_db *the_db )
{
	int i, k;
	FILE *ofp;

	ofp = fopen ( file_name,"w" );

	for ( i=0;i<MISSILE_MAX;i++ )
		if ( the_db[i].name[0] != '\0' ) //gun#,name,cost,damage,speed,accuracy
			fprintf ( ofp, "%d,%s,%d,%d,%d,%d,%d,%d,%d,%d%c",
			          i,
			          the_db[i].name,
			          the_db[i].cost,
			          the_db[i].damage,
			          the_db[i].speed,
			          the_db[i].accuracy,
			          the_db[i].exp_static,
			          the_db[i].exp_added,
			          the_db[i].tech_level,
			          the_db[i].type, 13 );

	fclose ( ofp );
}

//Other common functions -------------------------------------------------------
int gun_exp_requirement ( struct server_gun_db *the_db, int g, int ship_exp )
{
	int added_exp;

	added_exp = ship_exp + the_db[g].exp_added;

	if ( added_exp > the_db[g].exp_static )
		return added_exp;
	else
		return the_db[g].exp_static;
}

int missile_exp_requirement ( struct server_missile_db *the_db, int g, int ship_exp )
{
	int added_exp;

	added_exp = ship_exp + the_db[g].exp_added;

	if ( added_exp > the_db[g].exp_static )
		return added_exp;
	else
		return the_db[g].exp_static;
}

char* gun_type_name ( char *dest, int type )
{
	switch ( type )
	{
		case 0:
			strcpy ( dest,"Laser" );
			break;
		case 1:
			strcpy ( dest,"Jump Disruptor" );
			break;
		case 2:
			strcpy ( dest,"EMP Device" );
			break;
		case 3:
			strcpy ( dest,"Cannon" );
			break;
		case 4:
			strcpy ( dest,"Rapid Pulse" );
			break;
		case 5:
			strcpy ( dest,"Diamond Lazer" );
			break;
	}

	return dest;
}

char* missile_type_name ( char *dest, int type )
{
	switch ( type )
	{
		case 0:
			strcpy ( dest,"Rocket" );
			break;
		case 1:
			strcpy ( dest,"Jump Disruptor" );
			break;
		case 2:
			strcpy ( dest,"EMP Device" );
			break;
		case 3:
			strcpy ( dest,"Homing" );
			break;
		case 4:
			strcpy ( dest,"Heavy Blast Radius" );
			break;
		case 5:
			strcpy ( dest,"Heavy Projectile" );
			break;
	}

	return dest;
}

char* gun_type_description ( char *dest, int type )
{
	switch ( type )
	{
		case 0:
			strcpy ( dest,"Standard issue gun. Acts normally in response to shielding and evasion." );
			break;
		case 1:
			strcpy ( dest,"Emits high tech magnetic waves to disrupt engine charging. Knocks off a second to the victim's jump timer." );
			break;
		case 2:
			strcpy ( dest,"Equips small explosive charges which cause system failure in shield systems. 5% chance of temporally deactivating victim's shielding entirely." );
			break;
		case 3:
			strcpy ( dest,"Made of heavy solid material. Heavily effected by shielding." );
			break;
		case 4:
			strcpy ( dest,"Designed to fire weak, quick, accurate pulses instead of fully charged blasts. 4x speed, unaffected by evasion, 1/8th damage." );
			break;
		case 5:
			strcpy ( dest,"Pinpoint slicing for better mining. 4x effect against field type bots (asteroids), 1/2 effect on anything else." );
			break;
	}

	return dest;
}

char* missile_type_description ( char *dest, int type )
{
	switch ( type )
	{
		case 0:
			strcpy ( dest,"Standard issue missile. Acts normally in response to shielding and evasion." );
			break;
		case 1:
			strcpy ( dest,"Explosion emits high tech magnetic waves to disrupt engine charging. Knocks off a second to the victim's jump timer." );
			break;
		case 2:
			strcpy ( dest,"Built in explosive charge which causes system failure in shield systems. 10% chance of temporally deactivating victim's shielding entirely." );
			break;
		case 3:
			strcpy ( dest,"Slow moving smart missile. Chance of hit unaffected by shields and evasion." );
			break;
		case 4:
			strcpy ( dest,"Equips new technologies allowing for directionally focused blast emission. Higher chance of gun destruction." );
			break;
		case 5:
			strcpy ( dest,"Super heavy casing for increased momentum. 2x damage, highly effected by evasion." );
			break;
	}

	return dest;
}


