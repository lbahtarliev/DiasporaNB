#include "main.h"

#ifdef USE_MYSQL
MYSQL *my_sql_conn;
#endif

int mysql_mutex;

void mysql_init_vars()
{
#ifdef USE_MYSQL
	my_sql_conn = 0;
	mysql_mutex = 0;
#endif
}

void mysql_write_all_tables()
{
	mysql_write_server_table();
	mysql_write_sat_table();
	mysql_write_ship_classes_table();
	mysql_write_ships_table();
}

int mysql_make_connection()
{
	char *server = "localhost";
	char *user = MYSQL_USER;
	char *password = MYSQL_PASSWORD;
	char *database = "nighsoft_pdias_db";
	
#ifdef USE_MYSQL
	while(mysql_mutex)
		uni_pause(100);
	mysql_mutex = 1;
	
	my_sql_conn = mysql_init(NULL);
	
	if(!my_sql_conn)
	{
		printf("mysql: cannot init mysql\n");
		mysql_mutex = 0;
		return 0;
	}
	
	if(mysql_real_connect(my_sql_conn, server, user, password, database, 0, NULL, CLIENT_FOUND_ROWS))
	{
		printf("mysql: connection established\n");
		return 1;
	}
	else
	{
		printf("mysql: cannot connect: %s\n", mysql_error(my_sql_conn));
		mysql_mutex = 0;
		return 0;
	}
#else
	return 0;
#endif
}

int mysql_make_disconnection()
{
#ifdef USE_MYSQL
	mysql_close(my_sql_conn);
	
	mysql_mutex = 0;
	
	printf("mysql: connection closed\n");
	
	return 1;
#else
	return 0;
#endif
}

void mysql_make_jabber_insert(char *username, char *password)
{
#ifdef USE_MYSQL
	char *server = "localhost";
	char *db_user = MYSQL_USER;
	char *db_password = MYSQL_PASSWORD;
	char *database = "jabberd2";
	char sql_cmd[5000];
	char our_damn_name[500];
	char *our_realm = "hestia.nighsoft.net";
	MYSQL *my_sql_jab_conn;
	
	while(mysql_mutex)
		uni_pause(100);
	mysql_mutex = 1;
	
	my_sql_jab_conn = mysql_init(NULL);
	
	if(!my_sql_jab_conn)
	{
		printf("mysql: cannot init mysql for jabber_insert\n");
		mysql_mutex = 0;
		return;
	}
	
	if(mysql_real_connect(my_sql_jab_conn, server, db_user, db_password, database, 0, NULL, CLIENT_FOUND_ROWS))
	{
		printf("mysql: connection established for jabber_insert\n");
	}
	else
	{
		printf("mysql: cannot connect jabber_insert: %s\n", mysql_error(my_sql_jab_conn));
		mysql_mutex = 0;
		return;
	}
	
	//make the name right
	to_jabber_name(our_damn_name,username);
	
	//attempt to create the table cause were cool
	sprintf(sql_cmd,"\
	CREATE TABLE authreg (\
	username varchar(256),\
	realm varchar(256),\
	password varchar(256),\
	token varchar(10),\
	sequence integer,\
	hash varchar(40) )\
			");
	
	if(!mysql_query(my_sql_jab_conn, sql_cmd))
		printf("mysql: jabberd2.authreg table created\n");
	else
		printf("mysql: jabberd2.authreg table not created: %s\n", mysql_error(my_sql_jab_conn));
	
	//now send some shit!!!!
	sprintf(sql_cmd,"UPDATE authreg SET username='%s', realm='%s', password='%s' WHERE username='%s'",
		our_damn_name,
		our_realm,
		password,
		our_damn_name);

	if(!mysql_query(my_sql_jab_conn, sql_cmd))
	{
		if(mysql_affected_rows(my_sql_jab_conn) > 0)
		{
			printf("mysql: jabber dude already in - %d:%s\n", mysql_affected_rows(my_sql_jab_conn), username);
			mysql_close(my_sql_jab_conn);
			mysql_mutex = 0;
			return;
		}
	}
	
	sprintf(sql_cmd,"INSERT INTO authreg ( username, realm, password ) VALUES ('%s', '%s', '%s')",
		our_damn_name,
		our_realm,
		password);

	if(!mysql_query(my_sql_jab_conn, sql_cmd))
	{
		printf("mysql: pdias_site_sat insert successful for %s\n", username);
		mysql_close(my_sql_jab_conn);
		mysql_mutex = 0;
		return;
	}
	else
	{
		printf("mysql: jabberd2.authreg insert unsuccessful: %s\n", mysql_error(my_sql_jab_conn));
		mysql_close(my_sql_jab_conn);
		mysql_mutex = 0;
		return;
	}
	
	mysql_close(my_sql_jab_conn);
	
	mysql_mutex = 0;
#endif
}

int mysql_create_sat_table()
{
#ifdef USE_MYSQL
	return mysql_do_create_sat_table();
#else
	return 0;
#endif
}

void mysql_do_ds_awards()
{
#ifdef USE_MYSQL
	char sql_cmd[5000];
	MYSQL_RES *res;
	MYSQL_ROW row;
	unsigned int num_fields;
	unsigned int i;
	char *server = "localhost";
	char *db_user = MYSQL_USER;
	char *db_password = MYSQL_PASSWORD;
	char *database = "nighsoft_pdias_pay_db";
	MYSQL *my_sql_here_conn;
	
	//make connection
	while(mysql_mutex)
		uni_pause(100);
	mysql_mutex = 1;
	
	my_sql_here_conn = mysql_init(NULL);
	
	if(!my_sql_here_conn)
	{
		printf("mysql: cannot init mysql for ds awards\n");
		mysql_mutex = 0;
		return;
	}
	
	if(mysql_real_connect(my_sql_here_conn, server, db_user, db_password, database, 0, NULL, CLIENT_FOUND_ROWS))
	{
		printf("mysql: connection established for ds awards\n");
	}
	else
	{
		printf("mysql: cannot connect ds awards: %s\n", mysql_error(my_sql_here_conn));
		mysql_close(my_sql_here_conn);
		mysql_mutex = 0;
		return;
	}
	
	sprintf(sql_cmd, "SELECT * FROM payout_entry WHERE was_paid = 2");
	if(mysql_query(my_sql_here_conn, sql_cmd))
	{
		printf("mysql: selection error: %s\n", mysql_error(my_sql_here_conn));
		mysql_close(my_sql_here_conn);
		mysql_mutex = 0;
		return;
	}
	

	res=mysql_store_result(my_sql_here_conn); /* Download result from server */
	num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res)))
	{
		unsigned long *lengths;
		int i;
		lengths = mysql_fetch_lengths(res);
		
		char username[2][500];
		int ds_tobe[2], tabID;
		
		tabID = atoi(row[0]);
		strcpy(username[0], row[1]);
		ds_tobe[0] = atoi(row[2]);
		strcpy(username[1], row[3]);
		ds_tobe[1] = atoi(row[4]);
		
		award_user_ds(username[0], ds_tobe[0]);
		award_user_ds(username[1], ds_tobe[1]);
		
		//mysql_query("UPDATE payout_entry SET was_paid='1' WHERE tabID='" . $the_id . "'");
		sprintf(sql_cmd, "UPDATE payout_entry SET was_paid='1' WHERE tabID='%d'", tabID);
		if(mysql_query(my_sql_here_conn, sql_cmd))
		{
			printf("mysql: pay marking error: %s\n", mysql_error(my_sql_here_conn));
			continue;
		}
	}
	
	printf("mysql: connection closed for ds awards\n");
	
	mysql_free_result(res); /* Release memory used to store results. */
	mysql_close(my_sql_here_conn);
	mysql_mutex = 0;
#endif
}

void mysql_do_mfc_awards()
{
#ifdef USE_MYSQL
	char sql_cmd[5000];
	MYSQL_RES *res;
	MYSQL_ROW row;
	unsigned int num_fields;
	unsigned int i;
	char *server = "localhost";
	char *db_user = MYSQL_USER;
	char *db_password = MYSQL_PASSWORD;
	char *database = "nighsoft_affiliate_db";
	MYSQL *my_sql_here_conn;
	
	//make connection
	while(mysql_mutex)
		uni_pause(100);
	mysql_mutex = 1;
	
	my_sql_here_conn = mysql_init(NULL);
	
	if(!my_sql_here_conn)
	{
		printf("mysql: cannot init mysql for mfc awards\n");
		mysql_mutex = 0;
		return;
	}
	
	if(mysql_real_connect(my_sql_here_conn, server, db_user, db_password, database, 0, NULL, CLIENT_FOUND_ROWS))
	{
		printf("mysql: connection established for mfc awards\n");
	}
	else
	{
		printf("mysql: cannot connect mfc awards: %s\n", mysql_error(my_sql_here_conn));
		mysql_close(my_sql_here_conn);
		mysql_mutex = 0;
		return;
	}
	
	sprintf(sql_cmd, "SELECT * FROM affiliate_user WHERE aff_exp > 0");
	if(mysql_query(my_sql_here_conn, sql_cmd))
	{
		printf("mysql: selection error: %s\n", mysql_error(my_sql_here_conn));
		mysql_close(my_sql_here_conn);
		mysql_mutex = 0;
		return;
	}
	

	res=mysql_store_result(my_sql_here_conn); /* Download result from server */
	num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res)))
	{
		unsigned long *lengths;
		int i;
		lengths = mysql_fetch_lengths(res);
		
		char their_username[500];
		int exp_tobe, money_tobe, tabID;
		
		tabID = atoi(row[0]);
		strcpy(their_username, row[4]);
		exp_tobe = atoi(row[5]);
		money_tobe = atoi(row[6]);
		
		user_award_mfc(their_username, exp_tobe, money_tobe);
		
		//mysql_query("UPDATE affiliate_user SET aff_exp='0', aff_money='0'  WHERE tabID='" . $the_id . "'");
		sprintf(sql_cmd, "UPDATE affiliate_user SET aff_exp='0', aff_money='0'  WHERE tabID='%d'", tabID);
		if(mysql_query(my_sql_here_conn, sql_cmd))
		{
			printf("mysql: aff clearing error: %s\n", mysql_error(my_sql_here_conn));
			continue;
		}
	}
	
	printf("mysql: connection closed for mfc awards\n");
	
	mysql_free_result(res); /* Release memory used to store results. */
	mysql_close(my_sql_here_conn);
	mysql_mutex = 0;
#endif
}

void mysql_update_sat_entry(int s, int p)
{
#ifdef USE_MYSQL
	int sat_is_damaged;
	char *the_guild, *the_zone;
	char *nothing = "";
	char sql_cmd[5000];
	int g, z;
	
	if(!mysql_make_connection()) return;
	
	if(server[s].planet[p].is_gate) 
	{
		mysql_make_disconnection();
		return;
	}
	
	g = server[s].planet[p].sat.guild_id;
					
	if(g > -1 && g <= guild_max)
		the_guild = guild[g].name;
	else
		the_guild = nothing;
			
	if(server[s].planet[p].sat.money)
		sat_is_damaged = 0;
	else
		sat_is_damaged = 1;
			
			//edit 
	sprintf(sql_cmd,"UPDATE pdias_site_sat SET guild_i='%d', sat_guild_name='%s', sat_level='%d', sat_is_damaged='%d' WHERE server_i='%d' AND planet_i='%d'",
		g,
		the_guild,
		server[s].planet[p].sat.level,
		sat_is_damaged,
		s + 1,
		p + 1);

//	printf("%s\n",sql_cmd);
			
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		if(mysql_affected_rows(my_sql_conn) > 0)
		{
			printf("mysql: pdias_site_sat update successful\n");
			mysql_make_disconnection();
			return;
		}
	}
			
	//gotta find this manually 
	z = planet_zone(server[s].zone, p);
	if(z>-1)
		the_zone = server[s].zone[z].name;
	else
		the_zone = nothing;
	
	
	//create
	sprintf(sql_cmd,"INSERT INTO pdias_site_sat ( server_i, planet_i, planet_range, guild_i, sat_server_name, sat_zone_name, sat_planet_name, sat_guild_name, sat_level, sat_is_damaged ) VALUES ('%d', '%d', '%d', '%d', '%s', '%s', '%s', '%s', '%d', '%d')",
		s + 1,
		p + 1,
		server[s].planet[p].range,
		g,
		server[s].servername,
		the_zone,
		server[s].planet[p].name,
		the_guild,
		server[s].planet[p].sat.level,
		sat_is_damaged);

	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_sat insert successful\n");
		mysql_make_disconnection();
		return;
	}
	else
	{
		printf("mysql: pdias_site_sat insert unsuccessful: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return;
	}
	
	mysql_make_disconnection();
#endif
}

/*
			server_i int, \
			class_i int, \
			kind_i int, \
			ship_server_name varchar(25), \
			ship_class_name varchar(25)), \
			ship_cost int, \
			ship_exp int, \
			ship_hull int, \
			ship_shield int, \
			ship evasion int, \
			ship_speed int, \
			ship_range int, \
			ship_cargo int, \
			ship_guns int, \
			ship_missiles int, \
			ship_miscs int, \
			ship_owned_amount int \

*/
void mysql_update_ship_entry(int s, int sh, int k)
{
#ifdef USE_MYSQL
	char sql_cmd[5000];
	int shippers;
	
	if(!mysql_make_connection()) return;
	
	shippers = ship_owned_amount(s,sh,k);
	
	sprintf(sql_cmd,"UPDATE pdias_site_ships SET ship_server_name='%s', ship_class_name='%s', ship_cost='%d', ship_exp='%d', ship_hull='%d', ship_shield='%d', ship_evasion='%d', ship_speed='%d', ship_range='%d', ship_cargo='%d', ship_guns='%d', ship_missiles='%d', ship_miscs='%d', ship_owned_amount='%d' WHERE server_i='%d' AND class_i='%d' AND kind_i='%d'",
		server[s].servername,
		server[s].ship[sh].name,
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
		server[s].ship[sh].misc_ammount[k],
		shippers,
		s + 1,
		sh + 1,
		k + 1);
			
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		if(mysql_affected_rows(my_sql_conn) > 0)
		{
			printf("mysql: pdias_site_ships update successful\n");
			mysql_make_disconnection();
			return;
		}
	}
	
	//create
	sprintf(sql_cmd,"INSERT INTO pdias_site_ships ( ship_server_name, ship_class_name, ship_cost, ship_exp, ship_hull, ship_shield, ship_evasion, ship_speed, ship_range, ship_cargo, ship_guns, ship_missiles, ship_miscs, ship_owned_amount, server_i, class_i, kind_i ) VALUES ('%s', '%s', '%d', '%d', '%d', '%d' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d' , '%d')",
		server[s].servername,
		server[s].ship[sh].name,
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
		server[s].ship[sh].misc_ammount[k],
		shippers,
		s + 1,
		sh + 1,
		k + 1);
	
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_ships insert successful\n");
		mysql_make_disconnection();
		return;
	}
	else
	{
		printf("mysql: pdias_site_ships insert unsuccessful: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return;
	}
	
	mysql_make_disconnection();
#endif
}

void mysql_update_ship_class_entry(int s, int sh)
{
#ifdef USE_MYSQL
	char sql_cmd[5000];
	
	if(!mysql_make_connection()) return;
	
	sprintf(sql_cmd,"UPDATE pdias_site_ship_classes SET ship_server_name='%s', ship_class_name='%s' WHERE server_i='%d' AND class_i='%d'",
		server[s].servername,
		server[s].ship[sh].name,
		s + 1,
		sh + 1);

//	printf("%s\n",sql_cmd);
			
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		if(mysql_affected_rows(my_sql_conn) > 0)
		{
			printf("mysql: pdias_site_ship_classes update successful\n");
			mysql_make_disconnection();
			return;
		}
	}
	
	//create
	sprintf(sql_cmd,"INSERT INTO pdias_site_ship_classes ( ship_server_name, ship_class_name, server_i, class_i ) VALUES ('%s', '%s', '%d', '%d')",
		server[s].servername,
		server[s].ship[sh].name,
		s + 1,
		sh + 1);

	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_ship_classes insert successful\n");
		mysql_make_disconnection();
		return;
	}
	else
	{
		printf("mysql: pdias_site_ship_classes insert unsuccessful: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return;
	}
	
	mysql_make_disconnection();
#endif
}

void mysql_update_server_entry(int s)
{
#ifdef USE_MYSQL
	char sql_cmd[5000];
	int is_online;
	
	if(!mysql_make_connection()) return;
	
	if(server[s].con_id > -1)
		is_online = 1;
	else
		is_online = 0;
	
	sprintf(sql_cmd,"UPDATE pdias_site_server SET server_name='%s', server_online='%d', server_loaded='%d' WHERE server_i='%d'",
		server[s].servername,
		is_online,
		server[s].db_loaded,
		s + 1);

//	printf("%s\n",sql_cmd);
			
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		if(mysql_affected_rows(my_sql_conn) > 0)
		{
			printf("mysql: pdias_site_server update successful\n");
			mysql_make_disconnection();
			return;
		}
	}
	
	//create
	sprintf(sql_cmd,"INSERT INTO pdias_site_server ( server_name, server_online, server_loaded, server_i ) VALUES ('%s', '%d', '%d', '%d')",
		server[s].servername,
		is_online,
		server[s].db_loaded,
		s + 1);

	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_server insert successful\n");
		mysql_make_disconnection();
		return;
	}
	else
	{
		printf("mysql: pdias_site_server insert unsuccessful: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return;
	}
	
	mysql_make_disconnection();
#endif
}

int mysql_write_sat_table()
{
#ifdef USE_MYSQL
	int s, p;
	
	mysql_create_sat_table();
	
	for(s=0;s<MAX_SERVER;s++)
		if(server[s].servername[0] && server[s].db_loaded)
			for(p=0;p<PLANET_MAX;p++)
				if(server[s].planet[p].name[0])
					mysql_update_sat_entry(s,p);
	
	return 1;
#else
	return 0;
#endif
}

int mysql_write_ship_classes_table()
{
#ifdef USE_MYSQL
	int s, sh;
	
	mysql_do_create_ship_classes_table();
	
	for(s=0;s<MAX_SERVER;s++)
		if(server[s].servername[0])
			for(sh=0;sh<SHIP_MAX;sh++)
				if(server[s].ship[sh].name[0])
					mysql_update_ship_class_entry(s,sh);
	
	return 1;
#else
	return 0;
#endif
}

int mysql_write_ships_table()
{
#ifdef USE_MYSQL
	int s, sh, k;
	
	mysql_do_create_ships_table();
	
	for(s=0;s<MAX_SERVER;s++)
		if(server[s].servername[0])
			for(sh=0;sh<SHIP_MAX;sh++)
				if(server[s].ship[sh].name[0])
					for(k=0;k<8;k++)
						mysql_update_ship_entry(s,sh,k);
	
	return 1;
#else
	return 0;
#endif
}

int mysql_write_server_table()
{
#ifdef USE_MYSQL
	int s;

	mysql_do_create_server_table();

	for(s=0;s<MAX_SERVER;s++)
		if(server[s].servername[0])
			mysql_update_server_entry(s);
	
	return 1;
#else
	return 0;
#endif
}

#ifdef USE_MYSQL
int mysql_do_create_sat_table()
{
	if(!mysql_make_connection()) return 0;
	
	char *sql_cmd = "CREATE TABLE pdias_site_sat ( tabID int NOT NULL AUTO_INCREMENT, PRIMARY KEY(tabID), \
			server_i int, \
			planet_i int, \
			planet_range int, \
			guild_i int, \
			sat_server_name varchar(25), \
			sat_zone_name varchar(25), \
			sat_planet_name varchar(25), \
			sat_guild_name varchar(25), \
			sat_level int, \
 			sat_is_damaged int)";
	
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_sat table created\n");
		mysql_make_disconnection();
		return 1;
	}
	else
	{
		printf("mysql: pdias_site_sat table not created: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return 1;
	}
	
	mysql_make_disconnection();
}

int mysql_do_create_ship_classes_table()
{
	if(!mysql_make_connection()) return 0;
	
	char *sql_cmd = "CREATE TABLE pdias_site_ship_classes ( tabID int NOT NULL AUTO_INCREMENT, PRIMARY KEY(tabID), \
			server_i int, \
			class_i int, \
			ship_server_name varchar(25), \
			ship_class_name varchar(25))";
	
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_ship_classes table created\n");
		mysql_make_disconnection();
		return 1;
	}
	else
	{
		printf("mysql: pdias_site_ship_classes table not created: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return 1;
	}
	
	mysql_make_disconnection();
}

int mysql_do_create_ships_table()
{
	if(!mysql_make_connection()) return 0;
	
	char *sql_cmd = "CREATE TABLE pdias_site_ships ( tabID int NOT NULL AUTO_INCREMENT, PRIMARY KEY(tabID), \
			server_i int, \
			class_i int, \
			kind_i int, \
			ship_server_name varchar(25), \
			ship_class_name varchar(25), \
			ship_cost int, \
			ship_exp int, \
			ship_hull int, \
			ship_shield int, \
			ship_evasion int, \
			ship_speed int, \
			ship_range int, \
			ship_cargo int, \
			ship_guns int, \
			ship_missiles int, \
			ship_miscs int, \
			ship_owned_amount int)";
	
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_ships table created\n");
		mysql_make_disconnection();
		return 1;
	}
	else
	{
		printf("mysql: pdias_site_ships table not created: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return 1;
	}
	
	mysql_make_disconnection();
}

int mysql_do_create_server_table()
{
	if(!mysql_make_connection()) return 0;
	
	char *sql_cmd = "CREATE TABLE pdias_site_server ( tabID int NOT NULL AUTO_INCREMENT, PRIMARY KEY(tabID), \
			server_i int, \
			server_online int, \
			server_loaded int, \
			server_name varchar(25))";
	
	if(!mysql_query(my_sql_conn, sql_cmd))
	{
		printf("mysql: pdias_site_server table created\n");
		mysql_make_disconnection();
		return 1;
	}
	else
	{
		printf("mysql: pdias_site_server table not created: %s\n", mysql_error(my_sql_conn));
		mysql_make_disconnection();
		return 1;
	}
	
	mysql_make_disconnection();
}

#endif
