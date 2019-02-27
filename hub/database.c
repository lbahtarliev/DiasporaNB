#include "main.h"

void clear_user_db();
void reset_user_db();
void s_addr_user_db();

void write_server_planet_db ( int i );
void write_server_galaxy_db ( int i );
void write_server_ship_db ( int i );
void write_server_gun_db ( int i );
void write_server_missile_db ( int i );
void write_server_bot_db ( int s );
void write_server_satellite_db ( int s );

void create_server_folder();

extern void read_all_db()
{
	read_hub_db();
	read_user_db();
	read_server_db();
	read_guild_db();
	read_ban_db();
	read_admin_db();
	read_bounty_db();
}

extern void write_all_db()
{
	write_user_db ( 0 );
	write_server_db();
	write_guild_db ( 0 );
	write_ban_db();
	write_admin_db();
	write_bounty_db();
}

void read_hub_db()
{
	char *file_name = "hub.txt";
	FILE *fp;
	char temp_str[21];
	char *str_toked;

	fp = fopen ( file_name, "r" );

	if ( !fp )
	{
		strcpy ( game.hub_name, "pDiaspora" );
		write_hub_db();

		printf ( "Hub name written to %s\n", file_name );
		printf ( "Hub name set to::%s\n", game.hub_name );
		return;
	}

	fgets ( temp_str, 20, fp );

	dejunk_bytes ( temp_str, 20 );

	if ( temp_str[0] && ( str_toked = strtok ( temp_str, "\n\r\t:" ) ) )
	{
		strcpy ( game.hub_name, str_toked );
		printf ( "Hub name set to::%s\n", game.hub_name );
	}
	else
	{
		strcpy ( game.hub_name, "pDiaspora" );
		printf ( "Invalid hub name read from %s\n", file_name );
		printf ( "Hub name set to::%s\n", game.hub_name );
	}

	fclose ( fp );
}

void write_hub_db()
{
	char *file_name = "hub.txt";
	FILE *fp;

	fp = fopen ( file_name, "w" );

	if ( !fp ) return;

	fprintf ( fp, "%s\n", game.hub_name );

	fclose ( fp );
}

void clear_user_db()
{
	int u, i, k, j;

	for ( u=0;u<MAX_USER;u++ )
	{
		user[u].login[0] = 0;
		user[u].password[0] = 0;
		user[u].username[0] = 0;
		user[u].email[0] = 0;
		user[u].user_class = 0;
		user[u].money = 0;
		user[u].standards = 0;
		user[u].user_id = 0;
		user[u].exp = 0;
		user[u].kills = 0;
		user[u].user_class_points = 0;
		user[u].user_ip[0] = 0;
		user[u].con_id = -1;
		user[u].server_id = -1;
		user[u].guild_id = 0;
		user[u].guild_rank = 0;
		user[u].guild_balance = 0;
		user[u].guild_balance_update = 0;
		user[u].guild_request = -1;
		user[u].banned = 0;
		user[u].ship_selected = 0;
		user[u].debug = 0;
		user[u].version = 0;
		user[u].last_planet = -1;
		user[u].home_planet = -1;
		user[u].home_id = -1;
		user[u].landed = 1;
		user[u].activated = 0;

		for ( i=0;i<10;i++ )
		{
			user[u].ship[i].ship_id = -1;
			user[u].ship[i].brand = -1;
			user[u].ship[i].type = -1;
			user[u].ship[i].hull = 0;
			user[u].ship[i].hull_max = 0;
			user[u].ship[i].speed_max = 0;

			for ( j=0;j<10;j++ )
			{
				user[u].ship[i].cargo[j] = 0;
				user[u].ship[i].contra[j] = 0;
			}

			for ( j=0;j<16;j++ )
			{
				user[u].ship[i].gun[j] = -1;
				user[u].ship[i].gun_id[j] = -1;
				user[u].ship[i].gun_destroyed[j] = -1;

				user[u].ship[i].missile[j] = -1;
				user[u].ship[i].missile_id[j] = -1;
				user[u].ship[i].missile_left[j] = -1;

				user[u].ship[i].misc[j] = -1;
			}

			user[u].ship[i].sent_info = 0;

			user[u].ship[i].destroy_code = 0;
		}

		for ( i=0;i<MAX_DECO;i++ )
			user[u].has_deco[i] = 0;
		
		for(i=0;i<MAX_SERVER;i++)
			for(j=0;j<SHIP_MAX;j++)
				user[u].has_plan[i][j] = 0;

	}
}

void fix_user_db_semi_colons()
{
	int u, i;
	int len;
	
	for ( u=0;u<=user_max;u++ )
	{
		char *message;
		
		//login
		message = user[u].login;
		len = strlen(message);
		for(i=0;i<len;i++)
			if(message[i] == ';')
				message[i] = '-';
		
		//password
		message = user[u].password;
		len = strlen(message);
		for(i=0;i<len;i++)
			if(message[i] == ';')
				message[i] = '-';
		
		//name
		message = user[u].username;
		len = strlen(message);
		for(i=0;i<len;i++)
			if(message[i] == ';')
				message[i] = '-';
		
		//email
		message = user[u].email;
		len = strlen(message);
		for(i=0;i<len;i++)
			if(message[i] == ';')
				message[i] = '-';
	}
	
	//now check if any clones were made
	find_user_db_clones();
}

void find_user_db_clones()
{
	int u,u2;
	int len;
	char login[2][51];
	char username[2][51];
	
	for ( u=0;u<=user_max;u++ )
	{
		lcase ( login[0],user[u].login );
		lcase ( username[0],user[u].username );
		
		for(u2=u+1;u2<=user_max;u2++)
		{
			lcase ( login[1],user[u2].login );
			lcase ( username[1],user[u2].username );
			
			if(!strcmp(login[0], login[1]))
				printf("clone found for login:%s...%d:%d\n", user[u].login);
			
			if(!strcmp(username[0], username[1]))
				printf("clone found for username:%s...%d:%d\n", user[u].username);
		}
	}
}

void reset_user_db()
{
	int u, i, k, j;

	for ( u=0;u<MAX_USER;u++ )
	{
		//user[u].login[0] = 0;
		//user[u].password[0] = 0;
		//user[u].username[0] = 0;
		//user[u].email[0] = 0;
		user[u].user_class = 0;
		user[u].money = 2500000; //MIN_MONEY;
		user[u].standards = 0;
		user[u].user_id = 0;
		user[u].exp = 200;
		user[u].kills = 0;
		user[u].user_class_points = 0;
		//user[u].user_ip[0] = 0;
		user[u].con_id = -1;
		user[u].server_id = -1;
		user[u].guild_id = 0;
		user[u].guild_rank = 0;
		user[u].guild_balance = 0;
		user[u].guild_balance_update = 0;
		user[u].guild_request = -1;
		user[u].banned = 0;
		user[u].ship_selected = 0;
		user[u].debug = 0;
		user[u].version = 0;
		user[u].last_planet = -1;
		user[u].home_planet = -1;
		user[u].home_id = -1;
		user[u].landed = 1;
		//user[u].activated = 0;

		for ( i=0;i<10;i++ )
		{
			user[u].ship[i].ship_id = -1;
			user[u].ship[i].brand = -1;
			user[u].ship[i].type = -1;
			user[u].ship[i].hull = 0;
			user[u].ship[i].hull_max = 0;
			user[u].ship[i].speed_max = 0;

			for ( j=0;j<10;j++ )
			{
				user[u].ship[i].cargo[j] = 0;
				user[u].ship[i].contra[j] = 0;
			}

			for ( j=0;j<16;j++ )
			{
				user[u].ship[i].gun[j] = -1;
				user[u].ship[i].gun_id[j] = -1;
				user[u].ship[i].gun_destroyed[j] = -1;

				user[u].ship[i].missile[j] = -1;
				user[u].ship[i].missile_id[j] = -1;
				user[u].ship[i].missile_left[j] = -1;

				user[u].ship[i].misc[j] = -1;
			}

			user[u].ship[i].sent_info = 0;

			user[u].ship[i].destroy_code = 0;
		}

		for ( i=0;i<MAX_DECO;i++ )
			user[u].has_deco[i] = 0;

		for(i=0;i<MAX_SERVER;i++)
			for(j=0;j<SHIP_MAX;j++)
				user[u].has_plan[i][j] = 0;
	}
}

void read_user_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	const int input_amount = 1176 + MAX_DECO;
	char cur_line[6024] = "", c, temp_str[input_amount][51], line_section[2][6024];
	int i = 0, k, j, point, cur_line_num = -1; //use this as the length of the line red

	ifp = fopen ( "user.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		write_user_db ( 0 );
		ifp = fopen ( "user.db","r" );
	}

	clear_user_db();

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			//empty line?
			if(!i) continue;
			
			cur_line[i] = '\0';
			cur_line_num++;
			
			//the user db has two sections per line seperated by ';'
			point=0;
			split(line_section[0], cur_line, ';', &point);
			split(line_section[1], cur_line, ';', &point);
			
			//process section 1
			{
				for ( k=0;k<input_amount;k++ )
					temp_str[k][0] = '\0';
				for ( point=k=0;point<=i && k<input_amount;k++ )
					split ( temp_str[k],line_section[0],',',&point );
					//split ( temp_str[k],cur_line,',',&point );
	
				j=0;
				strcpy ( user[cur_line_num].login,temp_str[j++] );
				strcpy ( user[cur_line_num].password,temp_str[j++] );
				strcpy ( user[cur_line_num].lease_login,temp_str[j++] );
				strcpy ( user[cur_line_num].lease_password,temp_str[j++] );
				strcpy ( user[cur_line_num].username,temp_str[j++] );
				strcpy ( user[cur_line_num].email,temp_str[j++] );
				user[cur_line_num].money 		= atoi ( temp_str[j++] );
				user[cur_line_num].standards 		= atoi ( temp_str[j++] );
				user[cur_line_num].exp 			= atoi ( temp_str[j++] );
				
				strcpy ( user[cur_line_num].user_ip,temp_str[j++] );
				user[cur_line_num].the_s_addr 			= atol ( temp_str[j++] );
				strcpy ( user[cur_line_num].user_ip_creator,temp_str[j++] );
				user[cur_line_num].the_s_addr_creator		= atol ( temp_str[j++] );
				strcpy ( user[cur_line_num].user_ip_lease,temp_str[j++] );
				user[cur_line_num].the_s_addr_lease			= atol ( temp_str[j++] );
				
				user[cur_line_num].time_created 			= atoi ( temp_str[j++] );
				user[cur_line_num].time_logged 			= atoi ( temp_str[j++] );
				user[cur_line_num].time_lease_logged 		= atoi ( temp_str[j++] );
	
				user[cur_line_num].server_id 		= atoi ( temp_str[j++] );
				user[cur_line_num].guild_id 		= atoi ( temp_str[j++] );
				user[cur_line_num].guild_rank 	= atoi ( temp_str[j++] );
				user[cur_line_num].guild_balance 	= atoi ( temp_str[j++] );
				user[cur_line_num].user_class 	= atoi ( temp_str[j++] );
				user[cur_line_num].banned 		= atoi ( temp_str[j++] );
				user[cur_line_num].ship_selected 	= atoi ( temp_str[j++] );
				user[cur_line_num].last_planet 	= atoi ( temp_str[j++] );
				user[cur_line_num].home_planet 	= atoi ( temp_str[j++] );
				user[cur_line_num].home_id 		= atoi ( temp_str[j++] );
				user[cur_line_num].kills 		= atoi ( temp_str[j++] );
				user[cur_line_num].user_class_points 	= atoi ( temp_str[j++] );
				user[cur_line_num].activated 			= atoi ( temp_str[j++] );
				for ( k=0;k<10;k++ )
				{
					user[cur_line_num].ship[k].ship_id 				= atoi ( temp_str[j] );
					j++;
					user[cur_line_num].ship[k].brand 				= atoi ( temp_str[j] );
					j++;
					user[cur_line_num].ship[k].type 				= atoi ( temp_str[j] );
					j++;
					for ( i=0;i<16;i++,j++ ) user[cur_line_num].ship[k].gun_id[i] 		       = atoi ( temp_str[j] );
					for ( i=0;i<16;i++,j++ ) user[cur_line_num].ship[k].gun[i] 		       = atoi ( temp_str[j] );
					for ( i=0;i<16;i++,j++ ) user[cur_line_num].ship[k].gun_destroyed[i] 	   = atoi ( temp_str[j] );
					for ( i=0;i<16;i++,j++ )  user[cur_line_num].ship[k].missile_id[i] 	   = atoi ( temp_str[j] );
					for ( i=0;i<16;i++,j++ )  user[cur_line_num].ship[k].missile[i] 	       = atoi ( temp_str[j] );
					for ( i=0;i<16;i++,j++ )  user[cur_line_num].ship[k].missile_left[i] 	   = atoi ( temp_str[j] );
					for ( i=0;i<16;i++,j++ ) user[cur_line_num].ship[k].misc[i] 		       = atoi ( temp_str[j] );
	
					//set all the missiles to full
					//for(i=0;i<16;i++)  user[cur_line_num].ship[k].missile_left[i] 	= 4;
				}
	
				for ( k=0;k<MAX_DECO;k++,j++ )
					user[cur_line_num].has_deco[k] = atoi ( temp_str[j] );
			}
			
			//process section 2
			//if(0)
			{
				int len;
				int s, pl;
				
				len = strlen(line_section[1]);
		
				point = 0;
				while(point < len)
				{
					split(temp_str[0],line_section[1],',',&point);
					split(temp_str[1],line_section[1],',',&point);
			
					s = atoi(temp_str[0]);
					pl = atoi(temp_str[1]);
					
					if(s<0) continue;
					if(s>=MAX_SERVER) continue;
					if(pl<0) continue;
					if(pl>=SHIP_MAX) continue;
					
					user[cur_line_num].has_plan[s][pl] = 1;
				}
			}
			
			
			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	user_max = cur_line_num;
	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:user\n",cur_line_num + 1 );

	//sleeper
	assign_all_user_ship_destroy_codes();

	//for conversion
	//s_addr_user_db();
	//fix_user_db_semi_colons();
	//write_user_db(0);
}

extern void read_server_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[6][50];
	int i =0, k, point, cur_line_num = -1; //use this as the length of the line read

	ifp = fopen ( "server.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_server_db();
		write_server_db();
		ifp = fopen ( "server.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			//empty line?
			if(!i) continue;
			
			cur_line[i] = '\0';
			cur_line_num++;
			for ( k=0;k<6;k++ )
				temp_str[k][0] = '\0';
			for ( point=k=0;point<=i && k<6;k++ )
				split ( temp_str[k],cur_line,',',&point );

			temp_str[0][20] = '\0';
			temp_str[1][20] = '\0';
			temp_str[2][20] = '\0';
			temp_str[3][49] = '\0';

			strcpy ( server[cur_line_num].login,temp_str[0] ); //NOTE!!! a faster way you could read in the db file is to use split and "read" directly into user[cur_line_num].whatever for all string variables
			strcpy ( server[cur_line_num].password,temp_str[1] );
			strcpy ( server[cur_line_num].servername,temp_str[2] );
			strcpy ( server[cur_line_num].real_address,temp_str[3] );
			server[cur_line_num].x = atoi ( temp_str[4] );
			server[cur_line_num].y = atoi ( temp_str[5] );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	server_max = cur_line_num;
	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:server\n",cur_line_num + 1 );
}

extern void read_guild_db()
{
	const int input_amount = 20 + ( MAX_DECO * 2 );
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[input_amount][201];
	int i =0, k, j, point, cur_line_num = -1; //use this as the length of the line read


	ifp = fopen ( "guild.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_guild_db();
		write_guild_db ( 0 );
		ifp = fopen ( "guild.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			//empty line?
			if(!i) continue;
			
			cur_line[i] = '\0';
			cur_line_num++;
			for ( k=0;k<input_amount;k++ )
				temp_str[k][0] = '\0';
			for ( point=k=0;point<=i && k<input_amount;k++ )
				split ( temp_str[k],cur_line,',',&point );

			//cap
			temp_str[0][20] = 0;
			temp_str[1][20] = 0;
			temp_str[2][200] = 0;

			temp_str[5][20] = 0;
			temp_str[6][20] = 0;
			temp_str[7][20] = 0;
			temp_str[8][20] = 0;
			temp_str[9][20] = 0;
			temp_str[10][20] = 0;
			temp_str[11][20] = 0;
			temp_str[12][20] = 0;
			temp_str[13][20] = 0;

			strcpy ( guild[cur_line_num].name,temp_str[0] );
			strcpy ( guild[cur_line_num].owner,temp_str[1] );
			strcpy ( guild[cur_line_num].website,temp_str[2] );
			guild[cur_line_num].bank_money = atoi ( temp_str[3] );
			guild[cur_line_num].bank_lend = atoi ( temp_str[4] );
			strcpy ( guild[cur_line_num].rank_name[0],temp_str[5] );
			strcpy ( guild[cur_line_num].rank_name[1],temp_str[6] );
			strcpy ( guild[cur_line_num].rank_name[2],temp_str[7] );
			strcpy ( guild[cur_line_num].rank_name[3],temp_str[8] );
			strcpy ( guild[cur_line_num].rank_name[4],temp_str[9] );
			strcpy ( guild[cur_line_num].rank_name[5],temp_str[10] );
			strcpy ( guild[cur_line_num].enemy[0],temp_str[11] );
			strcpy ( guild[cur_line_num].enemy[1],temp_str[12] );
			strcpy ( guild[cur_line_num].enemy[2],temp_str[13] );
			for ( k=0;k<5;k++ ) guild[cur_line_num].rank_allow[0][k] = temp_str[14][k] - '0';
			for ( k=0;k<5;k++ ) guild[cur_line_num].rank_allow[1][k] = temp_str[15][k] - '0';
			for ( k=0;k<5;k++ ) guild[cur_line_num].rank_allow[2][k] = temp_str[16][k] - '0';
			for ( k=0;k<5;k++ ) guild[cur_line_num].rank_allow[3][k] = temp_str[17][k] - '0';
			for ( k=0;k<5;k++ ) guild[cur_line_num].rank_allow[4][k] = temp_str[18][k] - '0';
			for ( k=0;k<5;k++ ) guild[cur_line_num].rank_allow[5][k] = temp_str[19][k] - '0';

			//read in decos
			for ( j=0,k=0;j<MAX_DECO;j++ )
			{
				k++;
				temp_str[19 + k][30] = 0;
				strcpy ( guild[cur_line_num].deco_name[j], temp_str[19 + k] );
				k++;
				guild[cur_line_num].deco_activated[j] = atoi ( temp_str[19 + k] );
			}

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	//clear them!
	clear_all_guild_payouts();

	guild_max = cur_line_num;
	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:guild\n",cur_line_num + 1 );
}

extern void read_ban_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[2][16];
	int i =0, k, point, cur_line_num = -1; //use this as the length of the line read

	ifp = fopen ( "ban.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		write_ban_db();
		ifp = fopen ( "ban.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			//empty line?
			if(!i) continue;
			
			cur_line[i] = '\0';
			cur_line_num++;
			for ( k=0;k<2;k++ )
				temp_str[k][0] = '\0';
			for ( point=k=0;point<=i && k<2;k++ )
				split ( temp_str[k],cur_line,',',&point );

			strcpy ( ban[cur_line_num].ip,temp_str[0] ); //NOTE!!! a faster way you could read in the db file is to use split and "read" directly into user[cur_line_num].whatever for all string variables
			ban[cur_line_num].ban_free = atoi ( temp_str[1] );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	ban_max = cur_line_num;
	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:ban\n",cur_line_num + 1 );
}

extern void read_admin_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[3][21];
	int i =0, k, point, cur_line_num = -1; //use this as the length of the line read

	ifp = fopen ( "admin.db","r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		create_admin_db();
		write_admin_db();
		ifp = fopen ( "admin.db","r" );
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			//empty line?
			if(!i) continue;
			
			cur_line[i] = '\0';
			cur_line_num++;
			for ( k=0;k<3;k++ )
				temp_str[k][0] = '\0';
			for ( point=k=0;point<=i && k<3;k++ )
				split ( temp_str[k],cur_line,',',&point );

			strcpy ( admin[cur_line_num].login,temp_str[0] ); //NOTE!!! a faster way you could read in the db file is to use split and "read" directly into user[cur_line_num].whatever for all string variables
			strcpy ( admin[cur_line_num].password,temp_str[1] );
			admin[cur_line_num].level = atoi ( temp_str[2] );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	admin_max = cur_line_num;
	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:admin\n",cur_line_num + 1 );
}

extern void read_bounty_db()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[3][21];
	int i =0, k, point, cur_line_num = -1; //use this as the length of the line read

	ifp = fopen ( "bounty.db","r" );  //setup this declared file to open user.db to read from

	//clear the db
	for ( i=0;i<MAX_BOUNTY;i++ )
	{
		bounty[i].user_id = -1;
		bounty[i].price = -1;
		bounty[i].lifetime = -1;
	}

	if ( !ifp ) //if the file doesnt exist, then just use the cleared db we already have
	{
		printf ( "database loaded- entries:0 purpose:bounty\n" );
		return;
	}

	i = 0;
	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			//empty line?
			if(!i) continue;
			
			cur_line[i] = '\0';
			cur_line_num++;

			if ( cur_line_num >= MAX_BOUNTY ) break;

			for ( k=0;k<3;k++ )
				temp_str[k][0] = '\0';

			for ( point=k=0;point<=i && k<3;k++ )
				split ( temp_str[k],cur_line,',',&point );

			i = 0;
			
			//the entries good?
			if(!temp_str[0][0]) continue;
			if(!temp_str[1][0]) continue;
			if(!temp_str[2][0]) continue;
			
			bounty[cur_line_num].user_id = atoi ( temp_str[0] );
			bounty[cur_line_num].price = atoi ( temp_str[1] );
			bounty[cur_line_num].lifetime = atoi ( temp_str[2] );
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	printf ( "database loaded- entries:%d purpose:bounty\n",cur_line_num + 1 );
}

void write_user_emails()
{
	FILE *ofp;
	int i, k, j;
	char filename[200] = "user_emails.txt";

	ofp = fopen ( filename,"w" );

	for ( i=0;i<=user_max;i++ )
		fprintf ( ofp,"%s\n", user[i].email );

	fclose ( ofp );
}

void write_user_db ( int is_backup )
{
	FILE *ofp;
	int i, k, j;
	char filename[200] = "user.db";

	if ( is_backup )
		file_date ( filename );

	ofp = fopen ( filename,"w" );

	for ( i=0;i<=user_max;i++ )
	{
		fprintf ( ofp, "%s,%s,%s,%s,%s,%s,%d,%d,%d,%s,%lu,%s,%lu,%s,%lu,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		          user[i].login,
		          user[i].password,
		          user[i].lease_login,
		          user[i].lease_password,
		          user[i].username,
		          user[i].email,
		          user[i].money,
		          user[i].standards,
		          user[i].exp,
		          user[i].user_ip,
		          user[i].the_s_addr,
		          user[i].user_ip_creator,
		          user[i].the_s_addr_creator,
		          user[i].user_ip_lease,
		          user[i].the_s_addr_lease,
		          user[i].time_created,
		          user[i].time_logged,
		          user[i].time_lease_logged,
		          user[i].server_id,
		          user[i].guild_id,
		          user[i].guild_rank,
		          user[i].guild_balance,
		          user[i].user_class,
		          user[i].banned,
		          user[i].ship_selected,
		          user[i].last_planet,
		          user[i].home_planet,
		          user[i].home_id,
		          user[i].kills,
		          user[i].user_class_points,
		          user[i].activated );

		for ( k=0;k<10;k++ )
		{
			fprintf ( ofp, ",%d,%d,%d", user[i].ship[k].ship_id, user[i].ship[k].brand, user[i].ship[k].type );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].gun_id[j] );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].gun[j] );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].gun_destroyed[j] );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].missile_id[j] );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].missile[j] );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].missile_left[j] );
			for ( j=0;j<16;j++ ) fprintf ( ofp, ",%d", user[i].ship[k].misc[j] );
		}

		for ( k=0;k<MAX_DECO;k++ )
			fprintf ( ofp, ",%d",user[i].has_deco[k] );

		//now for the second layer
		fprintf ( ofp, "%c", ';' );
		
		for(k=0;k<MAX_SERVER;k++)
			for(j=0;j<SHIP_MAX;j++)
				if(user[i].has_plan[k][j])
					fprintf(ofp, "%d,%d,", k, j);
		
		fprintf ( ofp, "%c", 13 );
	}

	fclose ( ofp );
}

void write_guild_db ( int is_backup )
{
	FILE *ofp;
	int i, k;
	char filename[200] = "guild.db";

	if ( is_backup )
		file_date ( filename );

	ofp = fopen ( filename,"w" );

	for ( i=0;i<=guild_max;i++ )
	{
		fprintf ( ofp, "%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d,%d%d%d%d%d",
		          guild[i].name,
		          guild[i].owner,
		          guild[i].website,
		          guild[i].bank_money,
		          guild[i].bank_lend,
		          guild[i].rank_name[0],
		          guild[i].rank_name[1],
		          guild[i].rank_name[2],
		          guild[i].rank_name[3],
		          guild[i].rank_name[4],
		          guild[i].rank_name[5],
		          guild[i].enemy[0],
		          guild[i].enemy[1],
		          guild[i].enemy[2],
		          guild[i].rank_allow[0][0],
		          guild[i].rank_allow[0][1],
		          guild[i].rank_allow[0][2],
		          guild[i].rank_allow[0][3],
		          guild[i].rank_allow[0][4],
		          guild[i].rank_allow[1][0],
		          guild[i].rank_allow[1][1],
		          guild[i].rank_allow[1][2],
		          guild[i].rank_allow[1][3],
		          guild[i].rank_allow[1][4],
		          guild[i].rank_allow[2][0],
		          guild[i].rank_allow[2][1],
		          guild[i].rank_allow[2][2],
		          guild[i].rank_allow[2][3],
		          guild[i].rank_allow[2][4],
		          guild[i].rank_allow[3][0],
		          guild[i].rank_allow[3][1],
		          guild[i].rank_allow[3][2],
		          guild[i].rank_allow[3][3],
		          guild[i].rank_allow[3][4],
		          guild[i].rank_allow[4][0],
		          guild[i].rank_allow[4][1],
		          guild[i].rank_allow[4][2],
		          guild[i].rank_allow[4][3],
		          guild[i].rank_allow[4][4],
		          guild[i].rank_allow[5][0],
		          guild[i].rank_allow[5][1],
		          guild[i].rank_allow[5][2],
		          guild[i].rank_allow[5][3],
		          guild[i].rank_allow[5][4] );

		//save the decos
		for ( k=0;k<MAX_DECO;k++ )
			fprintf ( ofp, ",%s,%d", guild[i].deco_name[k], guild[i].deco_activated[k] );

		//cap a g
		fprintf ( ofp, "%c", 13 );
	}

	fclose ( ofp );
}

extern void write_server_db()
{
	FILE *ofp;
	int i;

	ofp = fopen ( "server.db","w" );

	for ( i=0;i<=server_max;i++ )
		fprintf ( ofp, "%s,%s,%s,%s,%d,%d%c", server[i].login, server[i].password, server[i].servername, server[i].real_address, server[i].x, server[i].y, 13 );

	fclose ( ofp );
}

extern void write_ban_db()
{
	FILE *ofp;
	int i;

	ofp = fopen ( "ban.db","w" );

	for ( i=0;i<=ban_max;i++ )
		fprintf ( ofp, "%s,%d%c", ban[i].ip, ban[i].ban_free, 13 );

	fclose ( ofp );
}

extern void write_admin_db()
{
	FILE *ofp;
	int i;

	ofp = fopen ( "admin.db","w" );

	for ( i=0;i<=admin_max;i++ )
		fprintf ( ofp, "%s,%s,%d%c", admin[i].login, admin[i].password, admin[i].level, 13 );

	fclose ( ofp );
}

extern void write_bounty_db()
{
	FILE *ofp;
	int i;

	ofp = fopen ( "bounty.db","w" );

	for ( i=0;i<MAX_BOUNTY;i++ )
		if(bounty[i].user_id > -1 && bounty[i].price > 0 && bounty[i].lifetime > 0)
			fprintf ( ofp, "%d,%d,%d%c", bounty[i].user_id, bounty[i].price, bounty[i].lifetime, 13 );

	fclose ( ofp );
}

//server read in databases

extern void read_all_server_db()
{
	int i, loaded;

	for ( i=0;i<MAX_SERVER;i++ )
		if ( server[i].servername[0] != '\0' )
		{
			loaded = read_all_servers_db ( i );

			if ( loaded == 5 ) //all databases loaded
			{
				printf ( "server databases loaded- server:%s loaded:%d\n", server[i].servername, loaded );
				server[i].db_loaded = 1;
			}
			else
			{
				printf ( "server databases missing- server:%s loaded:%d\n", server[i].servername, loaded );
				server[i].db_loaded = 0;
			}
		}
}

int read_all_servers_db ( int s )
{
	int loaded = 0;

	loaded += server_read_planet_db ( s );
	loaded += server_read_galaxy_db ( s );
	loaded += server_read_ship_db ( s );
	loaded += server_read_gun_db ( s );
	loaded += server_read_missile_db ( s );
	server_read_bot_db ( s );
	server_read_satellite_db ( s );

	if ( loaded == 5 )
		set_galaxy_ranges ( server[s].zone, server[s].planet, server[s].ship, server[s].gun, server[s].missile );

	return loaded;
}

extern int server_read_planet_db ( int server_number )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[83][21];
	int i, k, z, point, cur_line_num = -1, temp_planet, temp_is_gate, temp_ship, temp_ship_max, temp_wep; //use this as the length of the line read
	char file_name[200];

	clear_planet_db ( server[server_number].planet, & ( server[server_number].home_class ) );

	sprintf ( file_name, "server/%s_planet.db",server[server_number].servername );
	ifp = fopen ( file_name, "r" );  //setup this declared file to open blank.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		if ( !strcmp ( "pDiaspora",server[server_number].servername ) )
		{
			create_server_planet_db ( server_number );
			write_server_planet_db ( server_number );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	i=0;
	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone

			cur_line_num++;

			read_planet_db_entry ( server[server_number].planet, & ( server[server_number].home_class ), cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

extern int server_read_galaxy_db ( int server_number )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[15][21];
	int i =0, k, z, point, cur_line_num = -1, temp_zone, temp_sector; //use this as the length of the line read
	char file_name[200];

	clear_galaxy_db ( server[server_number].zone );

	sprintf ( file_name, "server/%s_galaxy.db",server[server_number].servername );
	ifp = fopen ( file_name, "r" );  //setup this declared file to open blank.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		if ( !strcmp ( "pDiaspora",server[server_number].servername ) )
		{
			create_server_galaxy_db ( server_number );
			write_server_galaxy_db ( server_number );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
			cur_line_num++;

			read_galaxy_db_entry ( server[server_number].zone, cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

extern int server_read_ship_db ( int server_number )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[13][21];
	int i =0, k, z, point, cur_line_num = -1, temp_class, temp_kind; //use this as the length of the line read
	char file_name[200];

	clear_ship_db ( server[server_number].ship );

	sprintf ( file_name, "server/%s_ship.db",server[server_number].servername );
	ifp = fopen ( file_name, "r" );  //setup this declared file to open blank.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		if ( !strcmp ( "pDiaspora",server[server_number].servername ) )
		{
			create_server_ship_db ( server_number );
			write_server_ship_db ( server_number );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //class,kind,cost,exp,hull,shield,evasion,speed,range,cargo,gun,missile,misc or //class,-1,class_name
			cur_line_num++;

			read_ship_db_entry ( server[server_number].ship, cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

extern int server_read_gun_db ( int server_number )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[6][21];
	int i =0, k, z, point, cur_line_num = -1, temp_gun; //use this as the length of the line read
	char file_name[200];

	clear_gun_db ( server[server_number].gun );

	sprintf ( file_name, "server/%s_gun.db",server[server_number].servername );
	ifp = fopen ( file_name, "r" );  //setup this declared file to open blank.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		if ( !strcmp ( "pDiaspora",server[server_number].servername ) )
		{
			create_server_gun_db ( server_number );
			write_server_gun_db ( server_number );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0';
			cur_line_num++;

			read_gun_db_entry ( server[server_number].gun, cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

extern int server_read_missile_db ( int server_number )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[5024] = "", c, temp_str[6][21];
	int i =0, k, z, point, cur_line_num = -1, temp_gun; //use this as the length of the line read
	char file_name[200];

	clear_missile_db ( server[server_number].missile );

	sprintf ( file_name, "server/%s_missile.db",server[server_number].servername );
	ifp = fopen ( file_name, "r" );  //setup this declared file to open blank.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		if ( !strcmp ( "pDiaspora",server[server_number].servername ) )
		{
			create_server_missile_db ( server_number );
			write_server_missile_db ( server_number );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //class,kind,cost,exp,hull,shield,evasion,speed,range,cargo,gun,missile,misc or //class,-1,class_name
			cur_line_num++;

			read_missile_db_entry ( server[server_number].missile, cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

int server_read_bot_db ( int s )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char file_name[1000], cur_line[5024] = "", c, temp_str[9 + SECTOR_MAX][21];
	int i =0, k, z, point, cur_line_num = -1, temp_bot_type, temp_bot, temp; //use this as the length of the line read

	clear_bot_db ( server[s].bot_type );

	sprintf ( file_name, "server/%s_bot.db",server[s].servername );
	ifp = fopen ( file_name,"r" );  //setup this declared file to open user.db to read from

	if ( !ifp ) //if the file doesnt exist, then who gives a shit
	{
		if ( !strcmp ( "pDiaspora",server[s].servername ) )
		{
			create_server_bot_db ( s );
			write_server_bot_db ( s );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //zone,sector,planet,shield,x,y,sec1,2,3,4,5,connectingzone
			cur_line_num++;

			read_bot_db_entry ( server[s].bot_type, cur_line );

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

int server_read_satellite_db ( int s )
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	const int temp_str_max = 6 + ( 2 * SAT_LEVEL_MAX * ( SAT_GUN_INC + SAT_MISSILE_INC ) );
	char cur_line[5024] = "", c, temp_str[temp_str_max][21];
	int i =0, k, j, z, point, cur_line_num = -1; //use this as the length of the line read
	char file_name[200];
	int cur_time;

	cur_time = time ( 0 );

	for ( k=0;k<PLANET_MAX;k++ ) //clear out previous sat db
	{
		server[s].planet[k].sat.level = 0;
		server[s].planet[k].sat.guild_id = -1;
		server[s].planet[k].sat.money = 0;
		server[s].planet[k].sat.money_collected = 0;
		server[s].planet[k].sat.hull = 0;
		server[s].planet[k].sat.last_warning_time = cur_time;
		server[s].planet[k].sat.bug_guild_id = -1;

		for ( z=0;z<SAT_LEVEL_MAX * SAT_GUN_INC;z++ )
		{
			server[s].planet[k].sat.gun[z] = -1;
			server[s].planet[k].sat.gun_destroyed[z] = 0;
		}

		for ( z=0;z<SAT_LEVEL_MAX * SAT_MISSILE_INC;z++ )
		{
			server[s].planet[k].sat.missile[z] = -1;
			server[s].planet[k].sat.missile_left[z] = 0;
		}
	}

	sprintf ( file_name, "server/%s_satellite.db",server[s].servername );
	ifp = fopen ( file_name, "r" );  //setup this declared file to open blank.db to read from

	if ( !ifp ) //if the file doesnt exist, then create it.
	{
		if ( !strcmp ( "pDiaspora",server[s].servername ) )
		{
			create_server_satellite_db ( s );
			write_server_satellite_db ( s );
			return 1;
		}
		else
		{
			return 0;
		}
	}

	while ( ( c = fgetc ( ifp ) ) != EOF ) //read in one char at a time until we hit the end of the file
	{
		if ( c == '\n' || c == '\r' ) //if char = newcharline, then process string collected into a db entry and clean it to start reading in the next
		{
			cur_line[i] = '\0'; //class,kind,cost,exp,hull,shield,evasion,speed,range,cargo,gun,missile,misc or //class,-1,class_name
			cur_line_num++;
			for ( point=k=0;point<=i && k<temp_str_max;k++ )
				split ( temp_str[k],cur_line,',',&point );

			k = atoi ( temp_str[0] );

			//test
			if ( k < 0 || k >= PLANET_MAX )
			{
				i = 0;
				continue;
			}

			//go on my friend
			server[s].planet[k].sat.level = atoi ( temp_str[1] );
			server[s].planet[k].sat.guild_id = atoi ( temp_str[2] );
			server[s].planet[k].sat.money = atoi ( temp_str[3] );
			server[s].planet[k].sat.money_collected = atoi ( temp_str[4] );
			server[s].planet[k].sat.bug_guild_id = atoi ( temp_str[5] );
			server[s].planet[k].sat.hull = server[s].planet[k].sat.level * SAT_HULL_INC;

			for ( z=0,j=6;z<SAT_LEVEL_MAX * SAT_GUN_INC;z++ )
			{
				server[s].planet[k].sat.gun[z] = atoi ( temp_str[j++] );
				server[s].planet[k].sat.gun_destroyed[z] = atoi ( temp_str[j++] );
			}

			for ( z=0;z<SAT_LEVEL_MAX * SAT_MISSILE_INC;z++ )
			{
				server[s].planet[k].sat.missile[z] = atoi ( temp_str[j++] );
				server[s].planet[k].sat.missile_left[z] = atoi ( temp_str[j++] );
			}

			i = 0;
		}
		else
		{
			cur_line[i] = c;
			i++;
		}
	}

	fclose ( ifp );
	return 1;
}

void create_server_folder()
{
#ifdef WIN32 //if windows
	mkdir ( "server" );
#else
	mkdir ( "server",-1 );
#endif
}

void create_backup_folder()
{
#ifdef WIN32 //if windows
	mkdir ( "backup" );
#else
	mkdir ( "backup",-1 );
#endif
}

void write_server_satellite_db ( int s )
{
	int i, k;
	FILE *ofp;
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_satellite.db", server[s].servername );
	ofp = fopen ( filename,"w" );

	for ( i=0;i<PLANET_MAX;i++ )
		if ( server[s].planet[i].name[0] != '\0' )
		{
			//static junk
			fprintf ( ofp, "%d,%d,%d,%d,%d,%d",
			          i,
			          server[s].planet[i].sat.level,
			          server[s].planet[i].sat.guild_id,
			          server[s].planet[i].sat.money,
			          server[s].planet[i].sat.money_collected,
			          server[s].planet[i].sat.bug_guild_id );

			//gun junk
			for ( k=0;k<SAT_LEVEL_MAX * SAT_GUN_INC;k++ )
				fprintf ( ofp, ",%d,%d", server[s].planet[i].sat.gun[k], server[s].planet[i].sat.gun_destroyed[k] );

			//missile junk
			for ( k=0;k<SAT_LEVEL_MAX * SAT_MISSILE_INC;k++ )
				fprintf ( ofp, ",%d,%d", server[s].planet[i].sat.missile[k], server[s].planet[i].sat.missile_left[k] );

			//cap
			fprintf ( ofp,"%c", 13 );
		}

	fclose ( ofp );
}

void write_server_planet_db ( int z )
{
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_planet.db",server[z].servername );

	write_planet_db_universal ( filename, server[z].planet, & ( server[z].home_class ) );
}

void write_server_galaxy_db ( int z )
{
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_galaxy.db",server[z].servername );

	write_galaxy_db_universal ( filename, server[z].zone );
}

void write_server_ship_db ( int z )
{
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_ship.db",server[z].servername );

	write_ship_db_universal ( filename, server[z].ship );
}

void write_server_bot_db ( int s )
{
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_bot.db",server[s].servername );

	write_bot_db_universal ( filename, server[s].bot_type );
}

void write_server_gun_db ( int z )
{
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_gun.db",server[z].servername );

	write_gun_db_universal ( filename, server[z].gun );
}

void write_server_missile_db ( int z )
{
	char filename[150];

	create_server_folder();

	sprintf ( filename,"server/%s_missile.db",server[z].servername );

	write_missile_db_universal ( filename, server[z].missile );
}

void write_required_server_sats()
{
	int s;

	for ( s=0;s<MAX_SERVER;s++ )
		if ( server[s].servername[0] && server[s].rewrite_sat_db )
			write_server_satellite_db ( s );
}

void check_databases_for_save()
{
	if ( game.rewrite_user_db )
	{
		write_user_db ( 0 );
		game.rewrite_user_db = 0;
	}
	if ( game.rewrite_guild_db )
	{
		write_guild_db ( 0 );
		game.rewrite_guild_db = 0;
	}
	if ( game.rewrite_bounty_db )
	{
		write_bounty_db();
		game.rewrite_bounty_db = 0;
	}
	write_required_server_sats();
}

int planet_zone ( struct server_zone_db *the_zone, int p )
{
	int z, s;

	for ( z=0;z<ZONE_MAX;z++ )
		for ( s=0;s<SECTOR_MAX;s++ )
			if ( the_zone[z].sector[s].planet == p )
				return z;

	return -1;
}

int ip_is_banned ( char *ip_address )
{
	//printf("bann ? %d %s\n", ip_address, ip_address);

	if ( !strcmp ( "69.64.50.167", ip_address ) )
		return 1;

	if ( !strcmp ( "210.84.51.8", ip_address ) )
		return 1;
	//124.168.61.95

	if ( !strcmp ( "124.168.61.95", ip_address ) )
		return 1;
	return 0;
}

void print_out_all_ship_stats()
{
	int s, sh, k;

	for ( s=0;s<MAX_SERVER;s++ )
		if ( server[s].servername[0] )
			for ( sh=0;sh<SHIP_MAX;sh++ )
				if ( server[s].ship[sh].name[0] )
					for ( k=0;k<8;k++ )
					{
						printf ( "server:%s ship:%s-%d-%d:\n cost:%d\n exp:%d\n hull:%d\n shield:%d\n eva:%d\n spd:%d\n range:%d\n cargo:%d\n guns:%d\n missiles:%d\n misc:%d\n",
						         server[s].servername,
						         server[s].ship[sh].name,
						         sh,
						         k,
						         server[s].ship[sh].cost[k],
						         server[s].ship[sh].exp[k],
						         server[s].ship[sh].hull[k],
						         server[s].ship[sh].shield[k],
						         server[s].ship[sh].evasion[k],
						         server[s].ship[sh].speed[k],
						         server[s].ship[sh].range[k],
						         server[s].ship[sh].cargo[k],
						         server[s].ship[sh].gun_ammount[k],
						         server[s].ship[sh].missile_ammount[k],
						         server[s].ship[sh].misc_ammount[k] );
					}
}

void s_addr_user_db()
{
	int u;

	for ( u=0;u<=user_max;u++ )
	{
		if ( !user[u].the_s_addr )
			user[u].the_s_addr = inet_addr ( user[u].user_ip );

		if ( !user[u].user_ip_creator[0] )
		{
			strcpy ( user[u].user_ip_creator, user[u].user_ip );
		}

		if ( !user[u].the_s_addr_creator )
			user[u].the_s_addr_creator = inet_addr ( user[u].user_ip_creator );
	}
}

int user_creation_limit ( unsigned long the_addr )
{
	int u;
	int max_time = 0;
	int time_difference;

	//find whoever was the most recent guy to be created with this ip
	for ( u=0;u<=user_max;u++ )
		if ( user[u].the_s_addr_creator == the_addr && user[u].time_created > max_time )
			max_time = user[u].time_created;

	//find how long its been since the most recent user was created
	time_difference = time ( 0 ) - max_time;

	if ( time_difference < 0 ) //time create after current time?
	{
		printf ( "warning!! user created at a time that exists in the future!\n" );
		return 0;
	}
	else if ( time_difference < CREATION_LIMIT_SECONDS )
	{
		return CREATION_LIMIT_SECONDS - time_difference;
	}
	else
	{
		return 0;
	}
}

void set_galaxy_ranges ( struct server_zone_db *the_zone, struct server_planet_db *the_planet, struct server_ship_db *the_ship, struct server_gun_db *the_gun, struct server_missile_db *the_missile )
{
	int i,j,k;
	int highest_range;

	//find the highest range
	highest_range = 0;
	for ( i=0;i<ZONE_MAX;i++ )
		if ( the_zone[i].name[0] )
			for ( j=0;j<SECTOR_MAX;j++ )
				if ( the_zone[i].sector[j].x )
					if ( highest_range < the_zone[i].sector[j].range ) highest_range = the_zone[i].sector[j].range;

	//err
	the_zone[0].highest_range = highest_range;

	//clear out all ranges
	for ( i=0;i<PLANET_MAX;i++ ) the_planet[i].range = highest_range;
	for ( i=0;i<GUN_MAX;i++ ) the_gun[i].range = highest_range;
	for ( i=0;i<MISSILE_MAX;i++ ) the_missile[i].range = highest_range;
	for ( i=0;i<SHIP_MAX;i++ ) for ( j=0;j<8;j++ ) the_ship[i].buy_range[j] = highest_range;

	//set planet ranges
	for ( i=0;i<ZONE_MAX;i++ )
		if ( the_zone[i].name[0] )
			for ( j=0;j<SECTOR_MAX;j++ )
				if ( the_zone[i].sector[j].x && the_zone[i].sector[j].planet > -1 )
					if ( the_planet[the_zone[i].sector[j].planet].range > the_zone[i].sector[j].range ) the_planet[the_zone[i].sector[j].planet].range = the_zone[i].sector[j].range;

	for ( i=0;i<PLANET_MAX;i++ )
		if ( the_planet[i].name[0] )
		{
			//set ship ranges
			for ( j=0;j<SHIP_MAX;j++ )
				if ( the_planet[i].ship[j].have && the_ship[j].name[0] )
					for ( k=0;k<=the_planet[i].ship[j].max;k++ )
						if ( !the_planet[i].ship[j].exclude[k] )
							if ( the_ship[j].buy_range[k] > the_planet[i].range ) the_ship[j].buy_range[k] = the_planet[i].range;

			//set gun ranges
			for ( j=0;j<GUN_MAX;j++ )
				if ( the_planet[i].gun[j] && the_gun[j].name[0] )
					if ( the_gun[j].range > the_planet[i].range ) the_gun[j].range = the_planet[i].range;

			//set missile ranges
			for ( j=0;j<MISSILE_MAX;j++ )
				if ( the_planet[i].missile[j] && the_missile[j].name[0] )
					if ( the_missile[j].range > the_planet[i].range ) the_missile[j].range = the_planet[i].range;
		}
}
