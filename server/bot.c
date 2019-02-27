#include "main.h"

#define BOT_MISSILE_SPEED 2.0
#define BOT_MISSILE_SPEED_ADJUST 1.0
#define BOT_MISSILE_ACCURACY 70
#define BOT_MISSILE_MIN_DAMAGE 5
#define BOT_MISSILE_DAMAGE_PERCENT_GUN 0.20

void *bot_thread ( void *nothing );
void *sun_thread ( void *nothing );
void do_sat_engine ( double the_time );
void set_bot_variables();
void set_bot_g_sectors();
void respawn_bots();
void respawn_bot ( int bot_type_num, int bot_num );
void bot_move_to_sector ( int bot_type_num, int bot_num, int new_sector );
void bot_do_attack ( int i, int k );
void bot_find_attack ( int i, int k );
int bot_random_choose_user ( int i, int k );
void bot_random_choose_bot ( int i, int k, int *type_return, int *num_return );
void bot_annouce_attack ( int i, int k );
void bot_disengage ( int i,int k );
int bot_player_on_list ( int i, int k, int con );
int bot_chase_and_kill ( int i,int k, int t, int u );
int bot_move_if_allowed ( int i, int k, int s, int z );
void bot_reload_missiles ( int b_t, int b );
void bot_fire_missiles ( int i, int k, double the_time );
void bot_drop_death_body_parts ( int i, int k );
void bot_set_last_minute_contra(int i, int k);

void start_bot_thread()
{
	pthread_t temp_thread;

	//set bot initial junk
	set_bot_variables();
	set_bot_g_sectors();
	clear_bot_lists();
	set_all_smuggler_prices();
	respawn_bots();

	pthread_create_always ( &temp_thread, NULL, bot_thread, NULL );
}

void *bot_thread ( void *nothing )
{
	int i, k, z, temp_sector[5], temp_sector_max;
	int attack_ok;
	double the_time;

	//until the end of time
	while ( 1 )
	{
		the_time = current_time();

		attack_ok = ! ( rand() % 3 );

		//all the sat attacking etc
		do_sat_engine ( the_time );

		for ( i=0;i<BOT_TYPE_MAX;i++ )
			if ( bot_type[i].name[0] )
				for ( k=0;k<=bot_type[i].bot_top;k++ )
					if ( bot_type[i].bot[k].name[0] )
					{
						//if dead
						if ( bot_type[i].bot[k].hull <= 0 )
						{
							if ( the_time - bot_type[i].bot[k].death_time > 30 && ! ( rand() % 20 ) )
								respawn_bot ( i,k );
							else
								continue;
						}

						//pick up some dang goods?
						if ( ! ( rand() % 20 ) )
							bot_collect_sector_goods ( i,k );

						//attack
						if ( bot_type[i].bot[k].user_attacking > -1 )
						{
							if ( attack_ok )
							{
								bot_do_attack ( i,k );
								bot_fire_missiles ( i,k,the_time );
							}
						}
						else
						{
							bot_find_attack ( i,k );
						}

						//move
						if ( the_time - bot_type[i].bot[k].jump_time > bot_type[i].bot[k].jump_speed )
						{
							//set their shield up now
							bot_set_shield_level ( i,k,3 );

							//other checks
							if ( ! ( rand() % 20 ) )
							{
								if ( bot_type[i].bot[k].user_attacking > -1 )
								{
									if ( ! ( rand() % 10 ) )
									{
										//do jump
										for ( z=temp_sector_max=0;z<5;z++ )
											if ( zone[bot_type[i].zone].sector[bot_type[i].bot[k].sector].sector[z] > -1 && bot_type[i].sector[zone[bot_type[i].zone].sector[bot_type[i].bot[k].sector].sector[z]] )
											{
												temp_sector[temp_sector_max] = zone[bot_type[i].zone].sector[bot_type[i].bot[k].sector].sector[z];
												temp_sector_max++;
											}

										if ( temp_sector_max && ! ( rand() % 3 ) )
											bot_move_to_sector ( i, k, temp_sector[rand() % temp_sector_max] );
									}
								}
								else
								{
									//do jump
									for ( z=temp_sector_max=0;z<5;z++ )
										if ( zone[bot_type[i].zone].sector[bot_type[i].bot[k].sector].sector[z] > -1 && bot_type[i].sector[zone[bot_type[i].zone].sector[bot_type[i].bot[k].sector].sector[z]] )
										{
											temp_sector[temp_sector_max] = zone[bot_type[i].zone].sector[bot_type[i].bot[k].sector].sector[z];
											temp_sector_max++;
										}

									if ( temp_sector_max && ! ( rand() % 3 ) )
										bot_move_to_sector ( i, k, temp_sector[rand() % temp_sector_max] );
								}
							}
						}

						//other stuff
						bot_check_for_sun_damage ( i,k );
					}


#ifdef _WIN32 //if windows
		Sleep ( 1000 );	//windows
#else
		sleep ( 1 );	//linux
#endif
	}
}

void respawn_bots()
{
	int i, k;

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' )
			for ( k=0;k<=bot_type[i].bot_top;k++ )
				if ( bot_type[i].bot[k].name[0] != '\0' )
					respawn_bot ( i,k );

}

void respawn_bot ( int bot_type_num, int bot_num )
{
	int new_sector;

	//redo their health
	bot_type[bot_type_num].bot[bot_num].hull = bot_type[bot_type_num].bot[bot_num].hull_max;

	//give them their starting good amounts
	bot_set_initial_goods ( bot_type_num, bot_num );

	//work the chasing AI
	bot_set_random_chasing_max ( bot_type_num, bot_num );

	//undisable their shields
	bot_type[bot_type_num].bot[bot_num].shield_disable_time = current_time() - SHIELD_DISABLE_TIME;

	//reload their missiles
	bot_reload_missiles ( bot_type_num, bot_num );

	//appear in sector
	if ( bot_type[bot_type_num].g_sector_max > 0 )
	{
		new_sector = rand() % bot_type[bot_type_num].g_sector_max;
		bot_move_to_sector ( bot_type_num, bot_num, bot_type[bot_type_num].g_sector[new_sector] );
	}
	else
	{
		printf ( "error-bot has odd sector amount .. bot_type:%d bot:%d bot_sectors:%d\n", bot_type_num, bot_num, bot_type[bot_type_num].g_sector_max );
	}
}

void set_bot_g_sectors()
{
	int i, k;

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' )
		{
			if ( !zone[bot_type[i].zone].name[0] || bot_type[i].zone < 0 || bot_type[i].zone >= ZONE_MAX )
			{
				printf ( "error-bot_type %d has invalid zone\n", i );
				continue;
			}

			for ( k=bot_type[i].g_sector_max=0;k<SECTOR_MAX;k++ )
				if ( bot_type[i].sector[k] )
				{
					//check if this sector is on the map...
					if ( !zone[bot_type[i].zone].sector[k].x )
					{
						bot_type[i].sector[k] = 0;
						continue;
					}
					bot_type[i].g_sector[bot_type[i].g_sector_max] = k;
					bot_type[i].g_sector_max++;
				}
		}
}

void set_bot_variables()
{
	int i,k,j;

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] )
		{
			bot_type[i].bot_top = -1;

			set_bot_allies ( i );

			for ( k=0;k<BOT_MAX;k++ )
				if ( bot_type[i].bot[k].name[0] )
				{
					int ship_class, ship_kind;
					int reg_speed;

					ship_class = bot_type[i].bot[k].ship_class;
					ship_kind = bot_type[i].bot[k].ship_kind;

					bot_type[i].bot_top = k;

					reg_speed = ship[ship_class].speed[ship_kind] + 2 * ship[ship_class].misc_ammount[ship_kind];
					if ( reg_speed > 26 ) reg_speed = 26;
					if ( reg_speed < 0 ) reg_speed = 0;

					bot_type[i].bot[k].hull_max = ship[ship_class].hull[ship_kind];
					bot_type[i].bot[k].jump_speed = ship_total_seconds ( reg_speed, ship_kind );
					bot_type[i].bot[k].cargo_max = ship[ship_class].cargo[ship_kind];
					bot_type[i].bot[k].shield_max = ship[ship_class].shield[ship_kind];
					bot_type[i].bot[k].evasion = ship[ship_class].evasion[ship_kind];

					//zero out some shield
					bot_type[i].bot[k].shield = 0;
					bot_type[i].bot[k].shield_level = 0;

					//...
					if ( bot_type[i].bot[k].def_multi > 0 ) bot_type[i].bot[k].hull_max = ( int ) ( bot_type[i].bot[k].hull_max *  bot_type[i].bot[k].def_multi );

					bot_type[i].bot[k].sector = -1;

					//no attacking!
					bot_type[i].bot[k].user_attacking = -1;
					bot_type[i].bot[k].bot_type_attacking = -1;

					//fix stuff
					if ( bot_type[i].bot[k].jump_speed < 4 )
						bot_type[i].bot[k].jump_speed = 4;

					//chasing stuff
					for ( j=0;j<MAX_BOT_FOLLOW;j++ )
						bot_type[i].bot[k].bot_chasing[j] = -1;
					bot_type[i].bot[k].bot_following = -1;
					bot_type[i].bot[k].bot_chasing_amt = 0;
					bot_set_random_chasing_max ( i,k );

					//missile stuff
					bot_type[i].bot[k].missile_amount = ship[ship_class].missile_ammount[ship_kind];
					bot_type[i].bot[k].missile_damage = ( int ) ( bot_type[i].bot[k].gun * BOT_MISSILE_DAMAGE_PERCENT_GUN );
					if ( bot_type[i].bot[k].missile_damage < BOT_MISSILE_MIN_DAMAGE )
						bot_type[i].bot[k].missile_damage = BOT_MISSILE_MIN_DAMAGE;
					for ( j=0;j<16;j++ )
					{
						bot_type[i].bot[k].missile_speed[j] = BOT_MISSILE_SPEED + ( BOT_MISSILE_SPEED_ADJUST * random_chance() );
						bot_type[i].bot[k].missile_next_time[j] = 0;
						bot_type[i].bot[k].missile_left[j] = 4;
					}
				}
		}
}

void bot_reload_missiles ( int i, int k )
{
	int j;
	int &m_max = bot_type[i].bot[k].missile_amount;

	for ( j=0;j<m_max;j++ )
	{
		bot_type[i].bot[k].missile_next_time[j] = 0;
		bot_type[i].bot[k].missile_left[j] = 4;
	}
}

void set_bot_allies ( int b_t )
{
	int i,k;

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' && !strcmp ( bot_type[i].guild, bot_type[b_t].guild ) )
		{
			bot_type[b_t].ally[i] = 1;
		}
		else
		{
			bot_type[b_t].ally[i] = 0;
		}
}

void bot_fire_missiles ( int i, int k, double the_time )
{
	int j;
	char die_str[24], damage_str[16], destroy_you[5] = {'1', '8', ',', 13, '\0'};
	double damage_delt;
	int v_s_k, v_s_t, v_s_s;
	int temp_accuracy;

	int u = bot_type[i].bot[k].user_attacking;
	int t = bot_type[i].bot[k].bot_type_attacking;
	int s = bot_type[i].bot[k].sector;
	int z = bot_type[i].zone;

	int s_s = 0;
	int s_t = bot_type[i].bot[k].ship_class;
	int s_k = bot_type[i].bot[k].ship_kind;

	int m_max = bot_type[i].bot[k].missile_amount;
	int m_damage = bot_type[i].bot[k].missile_damage;

	//simple checks...
	if ( u < 0 ) return;
	if ( bot_type[i].bot[k].hull <= 0 ) {bot_disengage ( i,k ); return;};

	//fire missiles
	if ( bot_type[i].bot[k].fighting_bot )
	{
		int bot_evasion = bot_type[t].bot[u].evasion;
		int bot_shield = bot_type[t].bot[u].shield;

		//check if we should still be attacking
		if ( t < 0 ) return;
		if ( s != bot_type[t].bot[u].sector ) {bot_disengage ( i,k ); return;};
		if ( z != bot_type[t].zone ) {bot_disengage ( i,k ); return;};
		if ( bot_type[t].bot[u].hull <= 0 ) {bot_disengage ( i,k ); return;};

		//we dont waste missiles
		temp_accuracy = calc_missile_acc ( bot_evasion, bot_shield, BOT_MISSILE_ACCURACY, 0 );
		if ( temp_accuracy < 20 ) return;

		//set some stuff
		v_s_s = 0;
		v_s_t = bot_type[t].bot[u].ship_class;
		v_s_k = bot_type[t].bot[u].ship_kind;

		//now do some attacking
		for ( j=0;j<m_max;j++ )
			if ( bot_type[i].bot[k].missile_left[j] > 0 && the_time >= bot_type[i].bot[k].missile_next_time[j] )
			{
				//this missile is loaded bitch

				//slight random chance we don't fire anyways
				if ( rand() %100 > 80 ) continue;

				//anouce fire
				relay_fire_missile ( z, s, bot_type[i].bot[k].name, -1 );

				//set new time and reduce one from the payload
				bot_type[i].bot[k].missile_next_time[j] = the_time + bot_type[i].bot[k].missile_speed[j];
				bot_type[i].bot[k].missile_left[j]--;

				//it hit?
				if ( rand() %100 + 1 < temp_accuracy ) continue;

				//do damage
				damage_delt = calc_missile_dam ( bot_evasion, bot_shield, m_damage, 0 ) * sector_damage_reduction ( z, s );
				damage_delt = ship_damage_difference ( s_k, damage_delt );
				damage_delt = ship_damage_defense ( v_s_k, damage_delt );
				bot_type[t].bot[u].hull -= ( int ) damage_delt;

				//kill that fucker
				if ( bot_type[t].bot[u].hull <= 0 )
				{
					//ah peanuts, he's dead.

					//now for the chat stuff
					relay_destroy_user_msg ( bot_type[t].bot[u].sector, bot_type[t].zone, bot_type[t].bot[u].name );

					//tell everyone else
					relay_player_blow_up ( s,z,bot_type[t].bot[u].name );

					//let dead dogs lie
					bot_disengage ( i,k );

					//Get them out of here?
					bot_destroy ( t,u );

					//send us back to good
					return;
				}
			}
	}
	else
	{
		//attacking a player

		//check if we should still be attacking
		if ( s != user_con[u].sector ) {bot_disengage ( i,k ); return;};
		if ( z != user_con[u].zone ) {bot_disengage ( i,k ); return;};
		if ( user_con[u].ship.hull <= 0 ) {bot_disengage ( i,k ); return;};
		if ( user_con[u].landed ) {bot_disengage ( i,k ); return;};

		//we dont waste missiles
		temp_accuracy = calculate_missile_accuracy ( u, BOT_MISSILE_ACCURACY, 0 );
		if ( temp_accuracy < 20 ) return;

		v_s_s = user_con[u].ship.server_id;
		v_s_t = user_con[u].ship.type;
		v_s_k = user_con[u].ship.kind;

		for ( j=0;j<m_max;j++ )
			if ( bot_type[i].bot[k].missile_left[j] > 0 && the_time >= bot_type[i].bot[k].missile_next_time[j] )
			{
				//this missile is loaded bitch

				//slight random chance we don't fire anyways
				if ( rand() %100 > 80 ) continue;

				//anouce fire
				relay_fire_missile ( z, s, bot_type[i].bot[k].name, -1 );

				//set new time and reduce one from the payload
				bot_type[i].bot[k].missile_next_time[j] = the_time + bot_type[i].bot[k].missile_speed[j];
				bot_type[i].bot[k].missile_left[j]--;

				//it hit?
				if ( rand() %100 + 1 < temp_accuracy ) continue;

				//do damage
				damage_delt = calculate_missile_damage ( u, m_damage, 0 ) * sector_damage_reduction ( z, s );
				damage_delt = ship_damage_difference ( s_k, damage_delt );
				damage_delt = ship_damage_defense ( v_s_k, damage_delt );
				user_con[u].ship.hull -= ( int ) damage_delt;

				//set the update flag
				user_con[u].update_hull = 1;

				//check if we need to bring happyness to our kind friend
				if ( user_con[u].ship.hull <= 0 )
				{
					//ah peanuts, he's dead.
					destroy_user ( u,-1 );

					//let dead dogs lie
					bot_disengage ( i,k );

					//send us back to good
					return;
				}

				//try some shit
				attempt_destroy_random_user_gun ( u, BOT_MISSILE_ACCURACY, 0 );

				//and him
				sprintf ( damage_str,"17,%d", user_con[u].ship.hull );
				send_con_user ( u,damage_str );

				//and this...
				send_user_was_hit ( u,bot_type[i].bot[k].name );
			}
	}
}

void bot_do_attack ( int i, int k )
{
	int j;
	char die_str[24], damage_str[16], destroy_you[5] = {'1', '8', ',', 13, '\0'};
	double damage_delt;
	int v_s_k, v_s_t, v_s_s;

	int u = bot_type[i].bot[k].user_attacking;
	int t = bot_type[i].bot[k].bot_type_attacking;
	int s = bot_type[i].bot[k].sector;
	int z = bot_type[i].zone;

	int s_s = 0;
	int s_t = bot_type[i].bot[k].ship_class;
	int s_k = bot_type[i].bot[k].ship_kind;

	//simple checks...
	if ( u < 0 ) return;
	if ( bot_type[i].bot[k].hull <= 0 ) {bot_disengage ( i,k ); return;};

	//fire guns
	if ( bot_type[i].bot[k].fighting_bot )
	{
		int bot_evasion, bot_shield;

		bot_evasion = bot_type[t].bot[u].evasion;
		bot_shield = bot_type[t].bot[u].shield;

		//check if we should still be attacking
		if ( t < 0 ) return;
		if ( s != bot_type[t].bot[u].sector ) {bot_disengage ( i,k ); return;};
		if ( z != bot_type[t].zone ) {bot_disengage ( i,k ); return;};
		if ( bot_type[t].bot[u].hull <= 0 ) {bot_disengage ( i,k ); return;};

		//now attack with that gun
		if ( rand() %100 + 1 < calc_gun_acc ( bot_evasion, bot_shield, 50, 0 ) ) return;

		//set this
		v_s_s = 0;
		v_s_t = bot_type[t].bot[u].ship_class;
		v_s_k = bot_type[t].bot[u].ship_kind;

		//do damage
		damage_delt = calc_gun_dam ( bot_evasion, bot_shield, bot_type[i].bot[k].gun, 0 ) * sector_damage_reduction ( z, s );
		damage_delt = ship_damage_difference ( s_k, damage_delt );
		damage_delt = ship_damage_defense ( v_s_k, damage_delt );
		bot_type[t].bot[u].hull -= ( int ) ( damage_delt );

		//kill that fucker
		if ( bot_type[t].bot[u].hull <= 0 )
		{
			//ah peanuts, he's dead.

			//now for the chat stuff
			relay_destroy_user_msg ( bot_type[t].bot[u].sector, bot_type[t].zone, bot_type[t].bot[u].name );

			//better tell the folks
			//tell hub if it deserve a reward

			//tell everyone else
			relay_player_blow_up ( s,z,bot_type[t].bot[u].name );


			//let dead dogs lie
			bot_disengage ( i,k );

			//Get them out of here?
			bot_destroy ( t,u );

			//send us back to good
			return;
		}
	}
	else
	{
		//check if we should still be attacking
		if ( s != user_con[u].sector ) {bot_disengage ( i,k ); return;};
		if ( z != user_con[u].zone ) {bot_disengage ( i,k ); return;};
		if ( user_con[u].ship.hull <= 0 ) {bot_disengage ( i,k ); return;};
		if ( user_con[u].landed ) {bot_disengage ( i,k ); return;};
		if(user_con[u].destroy_on_recv) {bot_disengage(i,k); return;};

		//we hit?
		//check chance
		if ( rand() %100 + 1 < calculate_gun_accuracy ( u, 50, 0 ) ) return;

		//set this
		v_s_s = user_con[u].ship.server_id;
		v_s_t = user_con[u].ship.type;
		v_s_k = user_con[u].ship.kind;

		//do damage
		damage_delt = calculate_gun_damage ( u, bot_type[i].bot[k].gun, 0 ) * sector_damage_reduction ( z, s );
		damage_delt = ship_damage_difference ( s_k, damage_delt );
		damage_delt = ship_damage_defense ( v_s_k, damage_delt );
		user_con[u].ship.hull -= ( int ) damage_delt;

		//set the update flag
		user_con[u].update_hull = 1;

		//check if we need to bring happyness to our kind friend
		if ( user_con[u].ship.hull <= 0 )
		{
			//ah peanuts, he's dead.
			//destroy_user ( u,-1 );
			
			//set them to die the next time we recv a packet from them
			user_set_for_bot_death(u);

			//let dead dogs lie
			bot_disengage ( i,k );

			//send us back to good
			return;
		}

		//try some shit
		attempt_destroy_random_user_gun ( u, 50, 0 );

		//and him
		sprintf ( damage_str,"17,%d", user_con[u].ship.hull );
		send_con_user ( u,damage_str );

		//and this...
		send_user_was_hit ( u,bot_type[i].bot[k].name );
	}
}

void bot_disengage ( int i,int k )
{
	int j, u, t;
	char disengage_str[25];

	u = bot_type[i].bot[k].user_attacking;
	t = bot_type[i].bot[k].bot_type_attacking;

	bot_type[i].bot[k].user_attacking = -1;
	bot_type[i].bot[k].bot_type_attacking = -1;


	//tell everyone!
	sprintf ( disengage_str,"21,%s%c", bot_type[i].bot[k].name, 13 );
	for ( j=0;j<=user_con_top;j++ )
	{
		if ( user_con[j].username[0] == '\0' ) continue;
		if ( !user_con[j].user_id ) continue;
		if ( bot_type[i].bot[k].sector != user_con[j].sector || bot_type[i].zone != user_con[j].zone )
			continue;
		if ( user_con[j].landed ) continue;

		raw_send_con_user ( j, disengage_str );
	}

	if ( bot_should_chase ( i,k,t,u ) )
		bot_chase_and_kill ( i,k,t,u );
}

int bot_should_chase ( int i, int k, int t, int u )
{
	if ( bot_type[i].aggression == 3 ) return 1;

	if ( bot_type[i].aggression == 4 )
	{
		if ( bot_type[i].bot[k].fighting_bot && bot_has_goods ( t,u ) ) return 1;
		if ( !bot_type[i].bot[k].fighting_bot && user_con[u].has_goods ) return 1;
	}

	//chase but not same ship class
	if ( bot_type[i].aggression == 5 ) return 1;

	return 0;
}

int bot_has_goods ( int i, int k )
{
	if ( bot_type[i].bot[k].cargo ) return 1;

	return 0;
}

int bot_chase_and_kill ( int i,int k, int t, int u )
{
	int s, z;

	//check if we should be doing this shit......
	if ( u < 0 ) return 0;
	if ( bot_type[i].bot[k].hull <= 0 ) return 0;

	//now bot or human?
	if ( bot_type[i].bot[k].fighting_bot )
	{
		//check if they are dead...
		if ( bot_type[t].bot[u].hull <= 0 ) return 0;

		s = bot_type[t].bot[u].sector;
		z = bot_type[t].zone;

		//if we could follow, then reengage
		if ( bot_move_if_allowed ( i,k,s,z ) )
		{
			bot_type[i].bot[k].user_attacking = u;
			bot_type[i].bot[k].bot_type_attacking = t;
			bot_type[i].bot[k].fighting_bot = 1;
		}
	}
	else
	{
		//check if they are dead or not available
		if ( user_con[u].ship.hull <= 0 ) return 0;
		if ( user_con[u].landed ) return 0;

		s = user_con[u].sector;
		z = user_con[u].zone;

		//if we could follow, then reengage
		if ( bot_move_if_allowed ( i,k,s,z ) )
		{
			bot_type[i].bot[k].user_attacking = u;
			bot_type[i].bot[k].bot_type_attacking = -1;
			bot_type[i].bot[k].fighting_bot = 0;
		}
	}

	if ( bot_type[i].bot[k].user_attacking > -1 )
	{
		bot_annouce_attack ( i,k );
		return 1;
	}

	return 0;
}

int bot_move_if_allowed ( int i, int k, int s, int z )
{
	int j;

	if ( current_time() - bot_type[i].bot[k].jump_time < bot_type[i].bot[k].jump_speed ) return 0;
	if ( z != bot_type[i].zone ) return 0;
	if ( !bot_type[i].sector[s] ) return 0;

	bot_move_to_sector ( i, k, s );

	return 1;
}

void bot_find_attack ( int i, int k )
{
	int choosen_user, choosen_bot_u, choosen_bot_t;

	//check argression type.....
	if ( !bot_type[i].aggression ) return;

	//make some choices
	choosen_user = bot_random_choose_user ( i,k );
	bot_random_choose_bot ( i,k,&choosen_bot_t,&choosen_bot_u );

	if ( rand() % 2 )
	{
		bot_type[i].bot[k].user_attacking = choosen_user;
		bot_type[i].bot[k].bot_type_attacking = -1;
		bot_type[i].bot[k].fighting_bot = 0;
	}
	else
	{
		bot_type[i].bot[k].user_attacking = choosen_bot_u;
		bot_type[i].bot[k].bot_type_attacking = choosen_bot_t;
		bot_type[i].bot[k].fighting_bot = 1;
	}

	if ( bot_type[i].bot[k].user_attacking > -1 )
		bot_annouce_attack ( i,k );
}

int bot_player_on_list ( int i, int k, int con )
{
	return 0;
}

void bot_annouce_attack ( int i, int k )
{
	char attack_face[80], attacking_you[25] = "19,";
	int victom_con, victom_type;
	char *attacker_name, *victom_name;
	int j;
	int z, s;

	s = bot_type[i].bot[k].sector;
	z = bot_type[i].zone;

	attacker_name = bot_type[i].bot[k].name;
	victom_con = bot_type[i].bot[k].user_attacking;
	victom_type = bot_type[i].bot[k].bot_type_attacking;
	if ( bot_type[i].bot[k].fighting_bot )
		victom_name = bot_type[victom_type].bot[victom_con].name;
	else
		victom_name = user_con[victom_con].username;

	//tell everyone!
	sprintf ( attack_face,"20,%s,%s%c", attacker_name, victom_name, 13 );
	for ( j=0;j<=user_con_top;j++ )
	{
		if ( user_con[j].username[0] == '\0' ) continue;
		if ( s != user_con[j].sector || z != user_con[j].zone ) continue;
		if ( user_con[j].landed ) continue;
		if ( j == victom_con ) continue;

		raw_send_con_user ( j, attack_face );
	}

	//tell the victom
	if ( !bot_type[i].bot[k].fighting_bot )
	{
		strcat ( attacking_you, attacker_name );
		send_con_user ( victom_con, attacking_you );
	}

	//well now who else?
	bot_provoke_all_chasers_attack ( i,k );
}

void bot_random_choose_bot ( int i, int k, int *type_return, int *num_return )
{
	int b_t, b_u, b_max, b_t_choosen[BOT_TOTAL_MAX], b_u_choosen[BOT_TOTAL_MAX];
	int s, z;
	int random_choosen;

	s = bot_type[i].bot[k].sector;
	z = bot_type[i].zone;
	*type_return = -1;
	*num_return = -1;

	//double check this stuff..
	if ( s < 0 ) return;
	if ( z < 0 ) return;
	if ( s >= SECTOR_MAX ) return;
	if ( z >= ZONE_MAX ) return;

	//collect the posibilties
	for ( b_t=b_max=0;b_t<BOT_TYPE_MAX;b_t++ )
		if ( bot_type[b_t].name[0] != '\0' )
	{
		 //i dont think we need to attack our own type
		if ( i == b_t ) continue;
		
		//dont shoot allies
		if ( bot_type[b_t].ally[i] ) continue;
		
		//dont shoot smugglers
		if ( bot_type[b_t].is_smuggler) continue;
		
		
			for ( b_u=0;b_u<BOT_MAX;b_u++ )
				if ( bot_type[b_t].bot[b_u].name[0] != '\0' )
					if ( bot_type[b_t].bot[b_u].sector == s && bot_type[b_t].zone == z )
					{
						//do other checks
						if ( bot_type[b_t].bot[b_u].hull <= 0 ) continue;
						
						//if ( i == b_t && k == b_u ) continue;
						

						//now for special cases
						if ( bot_type[b_t].aggression == 1 && bot_type[i].aggression == 1 ) continue;

						b_t_choosen[b_max] = b_t;
						b_u_choosen[b_max] = b_u;
						b_max++;
					}
	}

	//none found?
	if ( !b_max ) return;

	random_choosen = rand() % b_max;

	*type_return = b_t_choosen[random_choosen];
	*num_return = b_u_choosen[random_choosen];
}

int bot_random_choose_user ( int i, int k )
{
	int u, u_max, u_choosen[MAX_USER_CON];
	int s, z;

	s = bot_type[i].bot[k].sector;
	z = bot_type[i].zone;

	//double check this stuff..
	if ( s < 0 ) return -1;
	if ( z < 0 ) return -1;
	if ( s >= SECTOR_MAX ) return -1;
	if ( z >= ZONE_MAX ) return -1;

	//collect the posibilties
	for ( u=u_max=0;u<=user_con_top;u++ )
		if ( user_con[u].username[0] && user_con[u].sector == s && user_con[u].zone == z )
		{
			//now do those other checks
			if ( user_con[u].landed ) continue;
			if ( user_con[u].ship.hull <= 0 ) continue;
			if(user_con[u].destroy_on_recv) continue;
			if ( !bot_type[i].on_list[u] )
			{
				if ( bot_type[i].aggression == 1 ) continue;
				if ( bot_type[i].aggression == 4 && !user_con[u].has_goods ) continue;
				if ( !strcmp ( bot_type[i].guild, user_con[u].guild ) ) continue;
				if ( bot_type[i].aggression == 5 && user_con[u].ship.type == bot_type[i].bot[k].ship_class ) continue;
			}


			u_choosen[u_max] = u;
			u_max++;
		}

	//none found?
	if ( !u_max ) return -1;

	return u_choosen[rand() % u_max];
}

void bot_move_to_sector ( int bot_type_num, int bot_num, int new_sector )
{
	char rtn_str[128];
	int i, old_sector;

	//set the shit
	old_sector = bot_type[bot_type_num].bot[bot_num].sector;
	bot_type[bot_type_num].bot[bot_num].sector = new_sector;

	//shields down...
	bot_set_shield_level ( bot_type_num, bot_num, 0 );

	if ( old_sector != -1 )
	{
		//tell everyone you left
		sprintf ( rtn_str, "14,%s%c", bot_type[bot_type_num].bot[bot_num].name, 13 );
		for ( i=0;i<=user_con_top;i++ )
		{
			if ( !user_con[i].username[0] || user_con[i].landed ) continue; //if connected and launched
			if ( old_sector != user_con[i].sector ) continue; //same sector
			if ( bot_type[bot_type_num].zone != user_con[i].zone ) continue; //same sector

			raw_send_con_user ( i, rtn_str );
		}
	}

	//tell everyone you entered
	//ship id,type,kind,class_name,user_name,user_guild
	if ( !bot_type[bot_type_num].field_type )
		sprintf ( rtn_str, "13,%d,%d,%d,%s,%s,%s%c", server_info.reference_number, bot_type[bot_type_num].bot[bot_num].ship_class, bot_type[bot_type_num].bot[bot_num].ship_kind, ship[bot_type[bot_type_num].bot[bot_num].ship_class].name, bot_type[bot_type_num].bot[bot_num].name, bot_type[bot_type_num].guild, 13 );
	else
		sprintf ( rtn_str, "13,%d,-2,%d,%s,%s,%s%c", server_info.reference_number, bot_type[bot_type_num].field_type - 1, bot_type[bot_type_num].guild, bot_type[bot_type_num].bot[bot_num].name, bot_type[bot_type_num].guild, 13 );


	for ( i=0;i<=user_con_top;i++ )
	{
		if ( !user_con[i].username[0] || user_con[i].landed ) continue; //if connected and launched
		if ( new_sector != user_con[i].sector || bot_type[bot_type_num].zone != user_con[i].zone ) continue; //same sector

		raw_send_con_user ( i, rtn_str );
	}

	//set jump time
	bot_type[bot_type_num].bot[bot_num].jump_time = current_time();

	//bring in the posse
	bot_provoke_all_chasers_follow ( bot_type_num, bot_num );

	//ok now that we are here... chase some shit?
	if ( bot_type[bot_type_num].intelligence == 1 )
	{
		//we chasin or being chased?
		if ( bot_type[bot_type_num].bot[bot_num].bot_following > -1 ) return;

		if ( !bot_type[bot_type_num].bot[bot_num].bot_chasing_amt )
		{
			bot_find_follow ( bot_type_num, bot_num );
			return;
		}
		else if ( bot_type[bot_type_num].bot[bot_num].bot_chasing_amt < bot_type[bot_type_num].bot[bot_num].bot_chasing_max )
		{
			bot_gather_chasers ( bot_type_num, bot_num );
			return;
		}
	}
	else if ( bot_type[bot_type_num].intelligence == 2 ) //mothership follow class
	{

	}


}

extern void add_bots_from_sector ( char *message, int zone, int sector )
{
	char temp_str[128];
	int i, k, approx_len;

	approx_len = strlen ( message );

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' && bot_type[i].zone == zone )
			for ( k=0;k<=bot_type[i].bot_top;k++ )
				if ( bot_type[i].bot[k].name[0] != '\0' && bot_type[i].bot[k].sector == sector && bot_type[i].bot[k].hull )
				{
					if ( !bot_type[i].field_type )
						approx_len += sprintf ( temp_str, ",%d,%d,%d,%s,%s,%s", server_info.reference_number, bot_type[i].bot[k].ship_class, bot_type[i].bot[k].ship_kind, ship[bot_type[i].bot[k].ship_class].name, bot_type[i].bot[k].name, bot_type[i].guild );
					else
						approx_len += sprintf ( temp_str, ",%d,-2,%d,%s,%s,%s", server_info.reference_number, bot_type[i].field_type - 1, bot_type[i].guild, bot_type[i].bot[k].name, bot_type[i].guild );

					if ( approx_len < 3050 )
						strcat ( message, temp_str );
				}
}

extern void bot_name_to_reference ( int *bot_type_num, int *bot_num, char *bot_name )
{
	char temp_name[21], temp_bot_name[21];
	int i,k;

	lcase ( temp_bot_name, bot_name );

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' )
			for ( k=0;k<=bot_type[i].bot_top;k++ )
				if ( bot_type[i].bot[k].name[0] != '\0' )
				{
					lcase ( temp_name,bot_type[i].bot[k].name );

					if ( str_match ( temp_name,temp_bot_name ) )
					{
						*bot_type_num = i;
						*bot_num = k;
						return;
					}
				}

	*bot_type_num = -1;
	*bot_num = -1;
}

void send_user_sector_bot_war_info ( int con )
{
	char attack_face[80];
	int u,t, s, z;
	int i,k;
	char *attacker_name, *victom_name;

	//well
	s = user_con[con].sector;
	z = user_con[con].zone;

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' )
			for ( k=0;k<=bot_type[i].bot_top;k++ )
				if ( bot_type[i].bot[k].name[0] != '\0' )
					if ( bot_type[i].bot[k].user_attacking > -1 )
					{
						//other checks
						if ( s != bot_type[i].bot[k].sector ) continue;
						if ( z != bot_type[i].zone ) continue;

						//simplify
						attacker_name = bot_type[i].bot[k].name;
						u = bot_type[i].bot[k].user_attacking;
						t = bot_type[i].bot[k].bot_type_attacking;

						if ( bot_type[i].bot[k].fighting_bot )
							victom_name = bot_type[t].bot[u].name;
						else
							victom_name = user_con[u].username;

						sprintf ( attack_face,"20,%s,%s%c", attacker_name, victom_name, 13 );

						raw_send_con_user ( con, attack_face );
					}
}

void bot_destroy ( int i, int k )
{
	//why i do this stuff so late...

	//hull and death time stuff
	bot_type[i].bot[k].hull = 0;
	bot_type[i].bot[k].death_time = current_time();

	//get away ehhhh
	if ( bot_type[i].intelligence == 1 || bot_type[i].intelligence == 2 )
	{
		if ( bot_type[i].bot[k].bot_following )
			bot_stop_following ( i,k );

		if ( bot_type[i].bot[k].bot_chasing_amt );
		bot_clear_chasing ( i,k );

	}

	//they drop the goods on the skin...
	bot_set_last_minute_contra(i,k);
	bot_drop_all_goods ( i,k );
	bot_drop_death_body_parts ( i,k );
}

void bot_collect_sector_goods ( int i, int k )
{
	int s,z, g, left_cargo, take_amount;
	char rtn_str[1000];

	//this a smuggler?
	if ( bot_type[i].is_smuggler ) 
	{
		s = bot_type[i].bot[k].sector;
		z = bot_type[i].zone;
		
		clear_sector_contraband_if_needed(z,s);
		return;
	}
	
	//field types dont pick up goods
	if ( bot_type[i].field_type ) return;
	
	
	//already full?
	if ( bot_type[i].bot[k].cargo >= bot_type[i].bot[k].cargo_max ) return;

	//set some stuff
	left_cargo = bot_type[i].bot[k].cargo_max - bot_type[i].bot[k].cargo;
	s = bot_type[i].bot[k].sector;
	z = bot_type[i].zone;


	//well lets find a good...
	for ( g=0;g<10;g++ )
	{
		//does it exist?
		if ( zone[z].sector[s].good_amount[g] )
		{
			if ( zone[z].sector[s].good_amount[g] > left_cargo )
				take_amount = left_cargo;
			else
				take_amount = zone[z].sector[s].good_amount[g];

			//make the purchase
			//void server_bot_pickup_good(int con, int t, int u, int s, int z, int g, int amount)
			sprintf ( rtn_str,"18,%d,%d,%d,%d,%d,%d",i,k,s,z,g,take_amount );
			send_con_hub ( rtn_str );

			//remove from our temporarly left_cargo amount
			left_cargo -= take_amount;

			//get the hell out?
			if ( left_cargo <= 0 ) break;
		}

		//does it exist?
		if ( zone[z].sector[s].contra_amount[g] )
		{
			if ( zone[z].sector[s].contra_amount[g] > left_cargo )
				take_amount = left_cargo;
			else
				take_amount = zone[z].sector[s].contra_amount[g];

			//make the purchase
			//void server_bot_pickup_good(int con, int t, int u, int s, int z, int g, int amount)
			sprintf ( rtn_str,"19,%d,%d,%d,%d,%d,%d",i,k,s,z,g,take_amount );
			send_con_hub ( rtn_str );

			//remove from our temporarly left_cargo amount
			left_cargo -= take_amount;

			//get the hell out?
			if ( left_cargo <= 0 ) break;
		}
	}

}

void bot_set_last_minute_contra(int i, int k)
{
	int g, j;
	int max_amount, amount_left;
	
	if(!bot_type[i].drops_contraband) return;
	
	max_amount = ( int ) ( bot_type[i].bot[k].cargo_max * 0.25);
	amount_left = bot_type[i].bot[k].cargo_max - bot_type[i].bot[k].cargo;
	
	//erm ?
	if(amount_left <= 0) return;
	
	//is the amount we are going to add more then we have left?
	if(max_amount > amount_left)
		max_amount = amount_left;
	
	//add the contraband
	for ( j=0;j<max_amount;j++ )
		bot_type[i].bot[k].contra_amount[rand() % 10]++;

	//set the new cargo amount
	bot_type[i].bot[k].cargo += max_amount;
}

void bot_set_initial_goods ( int i, int k )
{
	int g, j;

	//clear the cargo..
	bot_type[i].bot[k].cargo = 0;
	for ( g=0;g<10;g++ )
	{
		bot_type[i].bot[k].good_amount[g] = 0;
		bot_type[i].bot[k].contra_amount[g] = 0;
	}

	if ( bot_type[i].field_type )
	{
		int max_amount;

		//figure out the new cargo amount
		max_amount = ( int ) ( bot_type[i].bot[k].cargo_max * 0.50 );

		//add the goods
		for ( j=0;j<max_amount;j++ )
			bot_type[i].bot[k].good_amount[rand() % 10]++;

		//set the new cargo amount
		bot_type[i].bot[k].cargo = max_amount;
	}
}

void bot_add_good ( int i, int k, int g, int amount )
{
	//because of the damn hub...
	if ( i<0 ) return;
	if ( k<0 ) return;
	if ( g<0 ) return;
	if ( i>=BOT_TYPE_MAX ) return;
	if ( k>=BOT_MAX ) return;
	if ( g>=10 ) return;

	//checks...
	if ( bot_type[i].bot[k].cargo + amount > bot_type[i].bot[k].cargo_max ) return;
	if ( bot_type[i].bot[k].cargo + amount < 0 ) return;

	bot_type[i].bot[k].good_amount[g] += amount;
	bot_type[i].bot[k].cargo += amount;
}

void bot_add_contra ( int i, int k, int g, int amount )
{
	//because of the damn hub...
	if ( i<0 ) return;
	if ( k<0 ) return;
	if ( g<0 ) return;
	if ( i>=BOT_TYPE_MAX ) return;
	if ( k>=BOT_MAX ) return;
	if ( g>=10 ) return;

	//checks...
	if ( bot_type[i].bot[k].cargo + amount > bot_type[i].bot[k].cargo_max ) return;
	if ( bot_type[i].bot[k].cargo + amount < 0 ) return;

	bot_type[i].bot[k].contra_amount[g] += amount;
	bot_type[i].bot[k].cargo += amount;
}

void bot_drop_death_body_parts ( int i, int k )
{
	int s,z;
	const int max_body_parts_dropped = 5;

	//field types, nien
	if ( bot_type[i].field_type ) return;

	s = bot_type[i].bot[k].sector;
	z = bot_type[i].zone;

	hub_add_sector_contra ( s,z, 9, ( rand() % max_body_parts_dropped ) + 1 );
}

void bot_drop_all_goods ( int i, int k )
{
	int s,z,g;

	s = bot_type[i].bot[k].sector;
	z = bot_type[i].zone;

	for ( g=0;g<10;g++ )
	{
		//got the good?
		if ( bot_type[i].bot[k].good_amount[g] > 0 )
		{
			//send away
			hub_add_sector_good ( s,z,g,bot_type[i].bot[k].good_amount[g] );
			//and remove
			bot_add_good ( i,k,g,-bot_type[i].bot[k].good_amount[g] );
		}

		//got the contra?
		if ( bot_type[i].bot[k].contra_amount[g] > 0 )
		{
			//send away
			hub_add_sector_contra ( s,z,g,bot_type[i].bot[k].contra_amount[g] );
			//and remove
			bot_add_contra ( i,k,g,-bot_type[i].bot[k].contra_amount[g] );
		}
	}
}

void bot_clear_all_bot_goods()
{
	int i,k,g;

	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] != '\0' && !bot_type[i].field_type )
			for ( k=0;k<BOT_MAX;k++ )
				if ( bot_type[i].bot[k].name[0] != '\0' )
				{
					for ( g=0;g<10;g++ )
					{
						//got the good?
						if ( bot_type[i].bot[k].good_amount[g] )
						{
							//and remove
							bot_add_good ( i,k,g,-bot_type[i].bot[k].good_amount[g] );
						}

						//got the contra?
						if ( bot_type[i].bot[k].contra_amount[g] )
						{
							//and remove
							bot_add_contra ( i,k,g,-bot_type[i].bot[k].contra_amount[g] );
						}
					}
				}
}

void do_sat_engine ( double the_time )
{
	int p;
	int i, j, k;
	int done_damage, missile_fired;
	int max_guns, max_missiles;
	int w;

	for ( p=0;p<PLANET_MAX;p++ )
		if ( planet[p].name[0] )
		{
			//it exist?
			if ( !planet[p].sat.guild_name[0] ) continue;
			if ( planet[p].sat.level <= 0 ) continue;
			if ( planet[p].sat.hull <= 0 ) continue;

			//fix its shields
			if ( !planet[p].sat.shield_level && !sat_shield_is_disabled ( p ) )
				set_sat_shield ( p, 4 );

			//set this
			missile_fired = 0;

			//check all the weapons for "can attack"
			max_guns = planet[p].sat.level * SAT_GUN_INC;
			for ( j=0;j<max_guns;j++ )
			{
				w = planet[p].sat.gun[j];

				if ( w<0 ) continue;

				if ( the_time - planet[p].sat.gun_time[j] > ( 100 - gun[w].speed ) * 0.1 )
					planet[p].sat.gun_can_attack[j] = 1;
			}

			max_missiles = planet[p].sat.level * SAT_MISSILE_INC;
			for ( j=0;j<max_missiles;j++ )
			{
				w = planet[p].sat.missile[j];

				if ( w<0 ) continue;

				if ( the_time - planet[p].sat.missile_time[j] > ( 100 - missile[w].speed ) * 0.1 )
					planet[p].sat.missile_can_attack[j] = 1;
			}

			//attack some shit
			for ( i=0;i<planet[p].sat.bot_attacking_max;i++ )
			{
				;
			}

			for ( i=0;i<planet[p].sat.user_attacking_max;i++ ) // it is attacking some users...
			{
				int u;

				u = planet[p].sat.user_attacking[i];

				//this player exist
				if ( u<0 ) continue;
				if ( u>=MAX_USER_CON ) continue;

				//should we still be attacking the user?
				if ( !sat_should_attack_user ( p,i ) )
					{sat_disengage_user ( p,i ); continue;};

				//set this
				done_damage = 0;

				//fire the guns!
				for ( j=0;j<max_guns;j++ )
				{
					int w;

					w = planet[p].sat.gun[j];

					//have the gun and it not destroyed?
					if ( w < 0 ) continue;
					if ( planet[p].sat.gun_destroyed[j] ) continue;

					//now for a time thing
					if ( !planet[p].sat.gun_can_attack[j] ) continue;

					//this one hit?
					if ( rand() %100 + 1 < calculate_gun_accuracy ( u, gun[w].accuracy, gun[w].type ) ) continue;

					//well do the f'ing damage
					user_con[u].ship.hull -= calculate_gun_damage ( u, gun[w].damage, gun[w].type );

					//bitch!
					if ( gun[w].type == 1 )
						user_dock_jump ( u );

					//he dead?
					if ( user_con[u].ship.hull <= 0 ) continue;

					//ho ho guns going down bitch!
					attempt_destroy_random_user_gun ( u, gun[w].accuracy, 0 );

					//set this
					done_damage = 1;
				}

				//he dead?
				if ( user_con[u].ship.hull <= 0 )
					{destroy_user ( u,-1 ); sat_disengage_user ( p,i ); continue;}

				//fire the missiles!
				for ( j=0;j<max_missiles;j++ )
				{
					int w;
					int temp_accuracy;

					w = planet[p].sat.missile[j];

					//have the gun and it not destroyed?
					if ( w < 0 ) continue;
					if ( !planet[p].sat.missile_left[j] ) continue;

					//now for a time thing
					if ( !planet[p].sat.missile_can_attack[j] ) continue;

					//we dont waste missiles
					temp_accuracy = calculate_missile_accuracy ( u, missile[w].accuracy, missile[w].type );
					if ( temp_accuracy < 20 ) continue;

					//slight random chance we don't fire anyways
					if ( rand() %100 > 80 ) continue;

					//ok it is fired so do some things before seeing if it hit

					//remove a missile and check
					planet[p].sat.missile_left[j]--;
					if ( planet[p].sat.missile_left[j]<0 ) planet[p].sat.missile_left[j] = 0;

					//anouce missile fire
					relay_fire_missile ( user_con[u].zone, user_con[u].sector, "Satellite", -1 );

					//tbut did it hit?
					if ( rand() %100 + 1 < temp_accuracy ) continue;

					//well do the f'ing damage
					user_con[u].ship.hull -= calculate_missile_damage ( u, missile[w].damage, missile[w].type );

					//bitch!
					if ( missile[w].type == 1 )
						user_dock_jump ( u );

					//set this
					missile_fired = 1;

					//notify the boss
					hub_set_sat_missile_left ( p,j );

					//he dead?
					if ( user_con[u].ship.hull <= 0 ) continue;

					//ho ho guns going down bitch!
					attempt_destroy_random_user_gun ( u, missile[w].accuracy, 1 );
					if ( missile[w].type == 4 )
					{
						attempt_destroy_random_user_gun ( u, missile[w].accuracy, 1 );
						attempt_destroy_random_user_gun ( u, missile[w].accuracy, 1 );
					}

					//set this
					done_damage = 1;
				}

				//did this happen?
				if ( missile_fired )
					relay_planet_sat_info ( p );

				//he dead?
				if ( user_con[u].ship.hull <= 0 )
					{destroy_user ( u,-1 ); sat_disengage_user ( p,i ); continue;}

				//tell them stuff if need be
				if ( done_damage )
				{
					//set the update flag
					user_con[u].update_hull = 1;

					//and him
					send_user_hull ( u );

					//and this...
					send_user_was_hit ( u,"Satellite" );
				}
			}

			//uncheck all the weapons for "can attack"
			max_guns = planet[p].sat.level * SAT_GUN_INC;
			for ( j=0;j<max_guns;j++ )
				if ( planet[p].sat.gun_can_attack[j] )
				{
					planet[p].sat.gun_can_attack[j] = 0;
					planet[p].sat.gun_time[j] = the_time;
				}

			max_missiles = planet[p].sat.level * SAT_MISSILE_INC;
			for ( j=0;j<max_missiles;j++ )
				if ( planet[p].sat.missile_can_attack[j] )
				{
					planet[p].sat.missile_can_attack[j] = 0;
					planet[p].sat.missile_time[j] = the_time;
				}
		}

}

void clear_bot_lists()
{
	int b_t, u;

	for ( b_t=0;b_t<BOT_TYPE_MAX;b_t++ )
		for ( u=0;u<MAX_USER_CON;u++ )
			bot_type[b_t].on_list[u] = 0;
}

void bot_user_add_to_any_list ( int u, int z, int s )
{
	int b_t, b_u;

	//find if there are any bots that use lists in this sector
	for ( b_t=0;b_t<BOT_TYPE_MAX;b_t++ )
		if ( bot_type[b_t].aggression == 1 || bot_type[b_t].aggression == 4 )
			for ( b_u=0;b_u<BOT_MAX;b_u++ )
				if ( bot_type[b_t].bot[b_u].name[0] )
				{
					//checks...
					if ( bot_type[b_t].zone != z ) continue;
					if ( bot_type[b_t].bot[b_u].sector != s ) continue;

					//add to the list
					bot_type[b_t].on_list[u] = 1;
				}
}

void user_add_to_bot_list ( int u, int b_t )
{
	bot_type[b_t].on_list[u] = 1;
}

void user_remove_from_all_lists ( int u )
{
	int b_t, b_u;

	for ( b_t=0;b_t<BOT_TYPE_MAX;b_t++ )
		bot_type[b_t].on_list[u] = 0;
}

void start_sun_thread()
{
	pthread_t temp_thread;

	pthread_create_always ( &temp_thread, NULL, sun_thread, NULL );
}

void *sun_thread ( void *nothing )
{
	int u;
	char damage_str[200];

	while ( 1 )
	{
		//check all online
		for ( u=0;u<=user_con_top;u++ )
			if ( user_con[u].username[0] && !user_con[u].landed )
			{
				int s, z;

				s = user_con[u].sector;
				z = user_con[u].zone;

				if ( s<0 ) continue;
				if ( z<0 ) continue;

				if ( zone[z].sector[s].sun_damage )
				{
					user_con[u].ship.hull -= zone[z].sector[s].sun_damage;

					//set the update flag
					user_con[u].update_hull = 1;

					//check if we need to bring happyness to our kind friend
					if ( user_con[u].ship.hull <= 0 )
					{
						//ah peanuts, he's dead.
						destroy_user ( u,-1 );

						//send us back to good
						continue;
					}

					//and him
					sprintf ( damage_str,"17,%d", user_con[u].ship.hull );
					send_con_user ( u,damage_str );
				}
			}

#ifdef _WIN32 //if windows
		Sleep ( 1000 );	//windows
#else
		sleep ( 1 );	//linux
#endif
	}
}

void bot_check_for_sun_damage ( int b_t, int b_u )
{
	int s, z;

	s = bot_type[b_t].bot[b_u].sector;
	z = bot_type[b_t].zone;

	if ( s<0 ) return;
	if ( z<0 ) return;

	if ( !zone[z].sector[s].sun_damage ) return;

	bot_type[b_t].bot[b_u].hull -= zone[z].sector[s].sun_damage;

	//kill that fucker
	if ( bot_type[b_t].bot[b_u].hull <= 0 )
	{
		//ah peanuts, he's dead.

		//now for the chat stuff
		relay_destroy_user_msg ( s, z, bot_type[b_t].bot[b_u].name );

		//better tell the folks

		//tell everyone else
		relay_player_blow_up ( s,z,bot_type[b_t].bot[b_u].name );

		//Get them out of here?
		bot_destroy ( b_t,b_u );

		//send us back to good
		return;
	}
}

void bot_clear_chasing ( int b_t, int b_u )
{
	int i;

	if ( bot_type[b_t].intelligence != 1 && bot_type[b_t].intelligence != 2 ) return;

	for ( i=0;i<MAX_BOT_FOLLOW;i++ )
		if ( bot_type[b_t].bot[b_u].bot_chasing[i] > -1 )
		{
			int b_u_c;

			b_u_c = bot_type[b_t].bot[b_u].bot_chasing[i];

			if ( bot_type[b_t].bot[b_u_c].bot_following == b_u )
				bot_stop_following ( b_t, b_u_c );

			bot_find_follow ( b_t, b_u_c );
		}
}

void bot_stop_following ( int b_t, int b_u )
{
	int b_u_f;
	int i;

	if ( bot_type[b_t].intelligence != 1 && bot_type[b_t].intelligence != 2 ) return;

	b_u_f = bot_type[b_t].bot[b_u].bot_following;

	if ( b_u_f > -1 )
	{
		//clear us
		bot_type[b_t].bot[b_u].bot_following = -1;

		//clear them
		for ( i=0;i<MAX_BOT_FOLLOW;i++ )
			if ( bot_type[b_t].bot[b_u_f].bot_chasing[i] > b_u )
			{
				bot_type[b_t].bot[b_u_f].bot_chasing[i] = -1;
				bot_type[b_t].bot[b_u_f].bot_chasing_amt--;
			}

		//check them
		if ( bot_type[b_t].bot[b_u_f].bot_chasing_amt < 0 )
			bot_type[b_t].bot[b_u_f].bot_chasing_amt = 0;
	}
}

void bot_find_follow ( int b_t, int b_u )
{
	int i,j,k;
	int s;
	int temp_found[BOT_MAX];
	int temp_found_max;

	//already following?
	if ( bot_type[b_t].bot[b_u].bot_following > -1 ) return;

	if ( bot_type[b_t].intelligence == 1 ) //random follow class
	{
		s = bot_type[b_t].bot[b_u].sector;

		//collect all bots in the sector capable of being followed
		temp_found_max = 0;
		for ( i=0;i<=bot_type[b_t].bot_top;i++ )
		{
			if ( bot_type[b_t].bot[i].sector != s ) return; //same sector?
			if ( bot_type[b_t].bot[i].hull <= 0 ) return; //is dead?
			if ( bot_type[b_t].bot[i].bot_chasing_amt == bot_type[b_t].bot[i].bot_chasing_max ) return; //too many children?
			if ( bot_type[b_t].bot[i].bot_following > -1 ) return; //chasing a fool themselves?

			//add to the list
			temp_found[temp_found_max] = i;
			temp_found_max++;
		}

		//ok we find any?
		if ( !temp_found_max ) return;

		//now set!
		bot_set_follow ( b_t, b_u, temp_found[rand() % temp_found_max] );
	}
	else if ( bot_type[b_t].intelligence == 2 ) //mothership follow class
	{

	}
}

void bot_gather_chasers ( int b_t, int b_u )
{
	int i,j,k;
	int s;

	//are we actually chasing someone ourselves?
	if ( bot_type[b_t].bot[b_u].bot_following > -1 ) return;

	s = bot_type[b_t].bot[b_u].sector;

	//are we of that bot_type intelly?
	if ( bot_type[b_t].intelligence == 1 )
	{
		//find all who need us
		for ( i=0;i<=bot_type[b_t].bot_top && bot_type[b_t].bot[b_u].bot_chasing_amt < bot_type[b_t].bot[b_u].bot_chasing_max;i++ )
		{
			if ( bot_type[b_t].bot[i].sector != s ) return; //same sector?
			if ( bot_type[b_t].bot[i].hull <= 0 ) return; //is dead?
			if ( bot_type[b_t].bot[i].bot_following > -1 ) return; //is available?
			if ( bot_type[b_t].bot[i].bot_chasing_amt > 0 ) return; //is chasing themselves?

			//well ok
			bot_set_follow ( b_t, i, b_u );
		}
	}
	else if ( bot_type[b_t].intelligence == 2 ) //mothership follow class
	{
		//are we a mothership?
		if ( !bot_type[b_t].bot[b_u].is_mothership ) return;
	}
}

void bot_set_follow ( int b_t, int b_u, int b_u_f )
{
	int i, j, k;

	//put us on their list
	for ( i=0;i<bot_type[b_t].bot[b_u_f].bot_chasing_max;i++ )
		if ( bot_type[b_t].bot[b_u_f].bot_chasing[i] < 0 ) return;

	//son of a no space?
	if ( i==bot_type[b_t].bot[b_u_f].bot_chasing_max ) return;

	//put us in their hearts
	bot_type[b_t].bot[b_u_f].bot_chasing[i] = b_u;
	bot_type[b_t].bot[b_u_f].bot_chasing_amt++;

	//well now choose one to follow
	bot_type[b_t].bot[b_u].bot_following = b_u_f;

	//now we need to attack who the leader is, if the leader is
	bot_attack_with_leader ( b_t, b_u );
}

void bot_attack_with_leader ( int b_t, int b_u )
{
	int b_u_f;

	if ( bot_type[b_t].intelligence != 1 && bot_type[b_t].intelligence != 2 ) return;

	b_u_f = bot_type[b_t].bot[b_u].bot_following;

	if ( b_u_f < 0 ) return; //dont have a leader..
	if ( bot_type[b_t].bot[b_u_f].user_attacking < 0 ) return; //leader not attacking anyone

	//okay do we need to disengage?
	if ( bot_type[b_t].bot[b_u].user_attacking > -1 )
		bot_disengage ( b_t, b_u );

	bot_type[b_t].bot[b_u].user_attacking = bot_type[b_t].bot[b_u_f].user_attacking;
	bot_type[b_t].bot[b_u].bot_type_attacking = bot_type[b_t].bot[b_u_f].bot_type_attacking;
	bot_type[b_t].bot[b_u].fighting_bot = bot_type[b_t].bot[b_u_f].fighting_bot;

	bot_annouce_attack ( b_t,b_u );
}

void bot_provoke_all_chasers_attack ( int b_t, int b_u )
{
	int i,j,k;
	int z, s;

	if ( bot_type[b_t].intelligence != 1 && bot_type[b_t].intelligence != 2 ) return;

	if ( !bot_type[b_t].bot[b_u].bot_chasing_amt ) return; //got no brains!

	s = bot_type[b_t].bot[b_u].sector;

	for ( i=0;i<bot_type[b_t].bot[b_u].bot_chasing_max;i++ )
		if ( bot_type[b_t].bot[b_u].bot_chasing[i] > -1 )
		{
			int b_u_c;

			b_u_c = bot_type[b_t].bot[b_u].bot_chasing[i];

			//well attack...
			bot_attack_with_leader ( b_t, b_u_c );
		}
}

void bot_provoke_all_chasers_follow ( int b_t, int b_u )
{
	int i,j,k;
	int z, s;

	if ( bot_type[b_t].intelligence != 1 && bot_type[b_t].intelligence != 2 ) return;

	if ( !bot_type[b_t].bot[b_u].bot_chasing_amt ) return; //got no brains!

	z = bot_type[b_t].zone;
	s = bot_type[b_t].bot[b_u].sector;

	for ( i=0;i<bot_type[b_t].bot[b_u].bot_chasing_max;i++ )
		if ( bot_type[b_t].bot[b_u].bot_chasing[i] > -1 )
		{
			int b_u_c;

			b_u_c = bot_type[b_t].bot[b_u].bot_chasing[i];

			if ( s == bot_type[b_t].bot[b_u_c].sector ) continue; //follow already here

			//so we got a valid move here?
			for ( j=0;j<5;j++ )
				if ( s == zone[z].sector[s].sector[j] )
					break;

			if ( j==5 ) continue; //not found!

			//so.. make the move
			bot_move_to_sector ( b_t, b_u_c, s );
		}
}

void bot_set_random_chasing_max ( int b_t, int b_u )
{
	if ( bot_type[b_t].intelligence == 1 )
	{
		bot_type[b_t].bot[b_u].bot_chasing_max = ( rand() % MAX_BOT_FOLLOW ) + 1;
	}
	else if ( bot_type[b_t].intelligence == 2 )
	{
		bot_type[b_t].bot[b_u].bot_chasing_max = ( rand() % MAX_BOT_FOLLOW ) + 1;

		//set is mothership or not
	}
	else // not a following class
	{
		bot_type[b_t].bot[b_u].bot_chasing_max = 0;
	}
}

void bot_set_shield_level ( int b_t, int b_u, int shield_level )
{
	//a check
	if ( shield_level == bot_type[b_t].bot[b_u].shield_level ) return;
	if ( bot_type[b_t].field_type ) return;

	//disable?
	if ( bot_shield_is_disabled ( b_t, b_u ) ) return;

	//set the level
	bot_type[b_t].bot[b_u].shield_level = shield_level;

	//set the shield
	bot_type[b_t].bot[b_u].shield = bot_type[b_t].bot[b_u].shield_max * shield_level / 4;

	//tell anyone who might be attacking this fool of the change
	bot_notify_attackers_of_shield ( b_t, b_u );
}

void bot_notify_attackers_of_shield ( int b_t, int b_u )
{
	char rtn_str[1000];
	int z, s, i;

	//should we be doing this?
	if ( bot_type[b_t].bot[b_u].hull <= 0 ) return;

	//make shit
	z = bot_type[b_t].zone;
	s = bot_type[b_t].bot[b_u].sector;
	sprintf ( rtn_str,"37,%d%c", bot_type[b_t].bot[b_u].shield_level, 13 );

	for ( i=0;i<=user_con_top;i++ )
		if ( user_con[i].username[0] )
		{
			//this person suposedly attacking us?
			if ( !user_con[i].attacking_bot ) continue;
			if ( user_con[i].bot_type_attacking != b_t ) continue;
			if ( user_con[i].user_attacking != b_u ) continue;

			//should they be?
			if ( user_con[i].landed ) continue;
			if ( user_con[i].zone != z ) continue;
			if ( user_con[i].sector != s ) continue;

			//well tell them
			raw_send_con_user ( i,rtn_str );
		}
}

void bot_dock_jump ( int b_t, int b_u )
{
	double the_time;

	the_time = current_time();

	//cant dock a full charged fool
	if ( the_time - bot_type[b_t].bot[b_u].jump_time > bot_type[b_t].bot[b_u].jump_speed ) return;

	//dock!!!!!!!!!!!!!!!!!!
	bot_type[b_t].bot[b_u].jump_time += JUMP_DOCK_TIME;

	//check......!
	if ( bot_type[b_t].bot[b_u].jump_time > the_time )
		bot_type[b_t].bot[b_u].jump_time = the_time;
}

void bot_try_disable_shield ( int b_t, int b_u )
{
	if ( rand() % 100 > SHIELD_DISABLE_CHANCE ) return;

	//and set
	bot_set_shield_level ( b_t, b_u, 0 );

	bot_type[b_t].bot[b_u].shield_disable_time = current_time();
}

int bot_shield_is_disabled ( int b_t, int b_u )
{
	return ( current_time() < bot_type[b_t].bot[b_u].shield_disable_time + SHIELD_DISABLE_TIME );
}

void destroy_sat ( int p )
{
	planet[p].sat.user_attacking_max = 0;
	planet[p].sat.bot_attacking_max = 0;
}

struct bot_drop_db bot_drop_item ( int b_t, int b_u )
{
	int i;
	int list[MAX_BOT_TYPE_DROP];
	int list_max;
	struct bot_drop_db blank;

	blank.drop_type = no_type;
	blank.ship_class = -1;
	blank.ship_kind = -1;
	blank.weapon = -1;
	blank.chance = 0.0;


	//try bot special items first
	for ( i=list_max=0;i<MAX_BOT_DROP && bot_type[b_t].bot[b_u].random_drop[i].drop_type != no_type;i++ )
		if ( ( rand() % 10000 ) / 10000.0 < bot_type[b_t].bot[b_u].random_drop[i].chance )
			list[list_max++] = i;

	if ( list_max ) //an award shall be made
		return bot_type[b_t].bot[b_u].random_drop[list[rand() % list_max]];

	//try bot type items
	for ( i=list_max=0;i<MAX_BOT_TYPE_DROP && bot_type[b_t].random_drop[i].drop_type != no_type;i++ )
		if ( ( rand() % 10000 ) / 10000.0 < bot_type[b_t].random_drop[i].chance )
			list[list_max++] = i;

	if ( list_max ) //an award shall be made
		return bot_type[b_t].random_drop[list[rand() % list_max]];

	//none
	return blank;
}

int bot_jump_is_charged ( int b_t, int b_u )
{
	return ( current_time() - bot_type[b_t].bot[b_u].jump_time > bot_type[b_t].bot[b_u].jump_speed );
}

void set_all_smuggler_prices()
{
	int i, g;
	
	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] )
	{
		if(!bot_type[i].is_smuggler) continue;
		
		for(g=0;g<10;g++)
		{
			int the_diff;
			
			bot_type[i].contra_price[g] = 0;
			
			the_diff = bot_type[i].contra_price_max[g] - bot_type[i].contra_price_min[g];
			
			if(the_diff <= 0) continue;
			if(bot_type[i].contra_price_min[g] < 0) continue;
			
			//set it
			bot_type[i].contra_price[g] = (rand() % the_diff) + 1;
		}
	}
}

int get_smuggler_sector_contra_worth(int z, int s, int g)
{
	int i, k;
	
	//find if there is a smuggler in this sector
	for ( i=0;i<BOT_TYPE_MAX;i++ )
		if ( bot_type[i].name[0] && bot_type[i].is_smuggler )
		{
			//this contra got a price?
			if(bot_type[i].contra_price[g] <= 0) return 0;
			
			//zone the same?
			if(bot_type[i].zone != z) return 0;
			
			for ( k=0;k<BOT_MAX;k++ )
				if ( bot_type[i].bot[k].name[0] )
				{
					//it alive?
					if ( bot_type[i].bot[k].hull <= 0 ) continue;
					//same sector?
					if(bot_type[i].bot[k].sector != s) continue;
					
					//guess this bot is good, return their price for the contraband
					return bot_type[i].contra_price[g];
				}
		}
		
		return 0;
}
