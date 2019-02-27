#include "main.h"

extern void read_all_db()
{
	read_server_config();
	read_planet_db();
	read_galaxy_db();
	read_ship_db();
	read_bot_db();
	read_gun_db();
	read_missile_db();
}

extern void write_all_db()
{
	write_planet_db();
	write_galaxy_db();
	write_ship_db();
	write_bot_db();
	write_gun_db();
	write_missile_db();
}

extern void read_server_config()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[1024] = "", c, temp_str[3][21];
	int i = 0, k, point; //use this as the length of the line read

	ifp = fopen ( "server.txt","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it. and setup standard locations
	{
		strcpy ( server_info.login,"login_name" );
		strcpy ( server_info.password,"login_password" );
		strcpy ( server_info.hub_address,"127.0.0.1" );
		write_server_config();
		ifp = fopen ( "server.txt","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0';

			for ( point=k=0;point<=i && k<3;k++ )
				split ( temp_str[k],cur_line,',',&point );

			strcpy ( server_info.login,temp_str[0] ); //NOTE!!! a faster way you could read in the db file is to use split and "read" directly into user[cur_line_num].whatever for all string variables
			strcpy ( server_info.password,temp_str[1] );
			strcpy ( server_info.hub_address,temp_str[2] );

			break;
			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "server config loaded-\n" );
}

extern void write_server_config()
{
	FILE *ofp;

	ofp = fopen ( "server.txt","w" );

	fprintf ( ofp, "%s,%s,%s%c", server_info.login, server_info.password, server_info.hub_address, 13 );

	fclose ( ofp );
}

extern void read_planet_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[82][21];
	int i =0, k, z, point, cur_line_num = -1, temp_planet, temp_is_gate, temp_ship, temp_ship_max, temp_wep; //use this as the length of the line read

	clear_planet_db ( planet, & ( server_info.home_class ) );

	ifp = fopen ( "planet.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_planet_db();
		write_planet_db();
		ifp = fopen ( "planet.db","r" );
	}

	i=0;
	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
			cur_line_num++;

			read_planet_db_entry ( planet, & ( server_info.home_class ), cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:planet\n",cur_line_num + 1 );
}

extern void write_planet_db()
{
	write_planet_db_universal ( "planet.db", planet, & ( server_info.home_class ) );
}

extern void read_galaxy_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[14][21];
	int i =0, k, z, point, cur_line_num = -1, temp_zone, temp_sector; //use this as the length of the line read

	clear_galaxy_db ( zone );

	ifp = fopen ( "galaxy.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_galaxy_db();
		write_galaxy_db();
		ifp = fopen ( "galaxy.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
			cur_line_num++;

			read_galaxy_db_entry ( zone,cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	//best place to put this
	set_sat_locations();
	set_sun_damages ( zone );
	reset_all_runes();


	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:galaxy\n",cur_line_num + 1 );
}

extern void write_galaxy_db()
{
	write_galaxy_db_universal ( "galaxy.db", zone );
}

extern void read_ship_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[13][21];
	int i =0, k, z, point, cur_line_num = -1, temp_class, temp_kind; //use this as the length of the line read

	clear_ship_db ( ship );

	ifp = fopen ( "ship.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_ship_db();
		write_ship_db();
		ifp = fopen ( "ship.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //class,kind,cost,exp,hull,shield,evasion,speed,range,cargo,gun,missile,misc or //class,-1,class_name

			cur_line_num++;

			read_ship_db_entry ( ship,cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:ship\n",cur_line_num + 1 );
}

extern void write_ship_db()
{
	write_ship_db_universal ( "ship.db", ship );
}

extern void read_bot_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[9 + SECTOR_MAX][21];
	int i =0, k, z, point, cur_line_num = -1, temp_bot_type, temp_bot, temp; //use this as the length of the line read

	clear_bot_db ( bot_type );

	ifp = fopen ( "bot.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_bot_db();
		write_bot_db();
		ifp = fopen ( "bot.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
			cur_line_num++;

			read_bot_db_entry ( bot_type,cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:bot\n",cur_line_num + 1 );

	//write_bot_db_universal("bot_fd.db", bot_type);
}

extern void write_bot_db()
{
	write_bot_db_universal ( "bot.db", bot_type );
}

extern void read_gun_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[6][21];
	int i =0, k, z, point, cur_line_num = -1, temp_gun; //use this as the length of the line read

	clear_gun_db ( gun );

	ifp = fopen ( "gun.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_gun_db();
		write_gun_db();
		ifp = fopen ( "gun.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0';
			cur_line_num++;

			read_gun_db_entry ( gun,cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:gun\n",cur_line_num + 1 );
}

extern void write_gun_db()
{
	write_gun_db_universal ( "gun.db", gun );
}

extern void read_missile_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[6][21];
	int i =0, k, z, point, cur_line_num = -1, temp_gun; //use this as the length of the line read

	clear_missile_db ( missile );

	ifp = fopen ( "missile.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_missile_db();
		write_missile_db();
		ifp = fopen ( "missile.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //class,kind,cost,exp,hull,shield,evasion,speed,range,cargo,gun,missile,misc or //class,-1,class_name
			cur_line_num++;

			read_missile_db_entry ( missile,cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:missile\n",cur_line_num + 1 );
}

void write_missile_db()
{
	write_missile_db_universal ( "missile.db", missile );
}

void clear_all_sat_info()
{
	int p, i;
	int z, s;
	double the_time;

	the_time = current_time();

	for ( p=0;p<PLANET_MAX;p++ )
	{
		planet[p].sat.level = -1;
		planet[p].sat.guild_id = -1;
		planet[p].sat.money = 0;
		planet[p].sat.money_collected = 0;
		planet[p].sat.hull = 0;
		planet[p].sat.guild_name[0] = 0;

		for ( i=0;i<SAT_LEVEL_MAX * SAT_GUN_INC;i++ )
		{
			planet[p].sat.gun[i] = -1;
			planet[p].sat.gun_destroyed[i] = 0;
			planet[p].sat.gun_time[i] = the_time;
			planet[p].sat.gun_can_attack[i] = 0;
		}
		for ( i=0;i<SAT_LEVEL_MAX * SAT_MISSILE_INC;i++ )
		{
			planet[p].sat.missile[i] = -1;
			planet[p].sat.missile_left[i] = 0;
			planet[p].sat.missile_time[i] = the_time;
			planet[p].sat.missile_can_attack[i] = 0;
		}

		//attacking stuff
		planet[p].sat.user_attacking_max = 0;
		planet[p].sat.bot_attacking_max = 0;
	}
}

void set_sat_locations()
{
	int p, i;
	int z, s;

	for ( p=0;p<PLANET_MAX;p++ )
	{
		//set sector/zone
		planet[p].sat.zone = -1;
		planet[p].sat.sector = -1;
		for ( z=0;z<ZONE_MAX;z++ )
			if ( zone[z].name[0] )
				for ( s=0;s<SECTOR_MAX;s++ )
					if ( zone[z].sector[s].planet == p )
					{
						planet[p].sat.zone = z;
						planet[p].sat.sector = s;
					}
	}
}

void set_sun_damages ( struct server_zone_db *the_zone )
{
	int z, s, s2;

	//first clear the damages
	for ( z=0;z<ZONE_MAX;z++ )
		for ( s=0;s<SECTOR_MAX;s++ )
			the_zone[z].sector[s].sun_damage = 0;

	//now set the damages
	for ( z=0;z<ZONE_MAX;z++ )
		for ( s=0;s<SECTOR_MAX;s++ )
			if ( the_zone[z].sector[s].x && the_zone[z].sector[s].is_sun )
				for ( s2=0;s2<SECTOR_MAX;s2++ )
				{
					int dist;

					//checks
					if ( !the_zone[z].sector[s2].x ) continue;

					//oooO
					dist = distance ( the_zone[z].sector[s].x,the_zone[z].sector[s].y,the_zone[z].sector[s2].x,the_zone[z].sector[s2].y );

					if ( dist > SUN_RADIUS ) continue;

					the_zone[z].sector[s2].sun_damage += SUN_DAMAGE * ( SUN_RADIUS - dist ) / SUN_RADIUS;
				}
}
