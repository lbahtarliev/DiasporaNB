#ifndef _DATABASE_COMMON_H_
#define _DATABASE_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//from "common.h"
extern void split(char*, char*, char, int*);

//database
#define MAX_USER 8000
#define MAX_GUILD 1000
#define MAX_SERVER 20
#define MAX_ADMIN 20
#define MAX_BAN 200
#define MAX_BOUNTY 38

//server
#define SHIP_MAX 40
#define GUN_MAX 50
#define MISSILE_MAX 50
#define PLANET_MAX 100
#define PLANET_TRADE_MAX 5
#define PLANET_TRADE_DISASTER_MAX 2
#define MAX_TRADE_DISASTER 10
#define SECTOR_MAX 300
#define ZONE_MAX 10
#define BOT_MAX 200
#define BOT_TYPE_MAX 50
#define BOT_TOTAL_MAX BOT_TYPE_MAX * BOT_MAX

//sat stuff
#define SAT_LEVEL_MAX 9
#define SAT_HULL_INC 20000
#define SAT_GUN_INC 4
#define SAT_MISSILE_INC 2
#define SAT_TAX_INC 5000
#define SAT_COST_INC 250000
#define SAT_DEFAULT_SHIELD 60
#define SAT_DEFAULT_EVASION 10
#define SAT_WARNING_TIME 5 * 60
#define SAT_MAX_BANK_INC 200000

#ifndef MAX_USER_CON
#define MAX_USER_CON 1000
#endif

//other
#define MAX_BOT_FOLLOW 5
#define SHIELD_DISABLE_TIME 4
#define SHIELD_DISABLE_CHANCE 5
#define MAX_DECO 30
#define TRADE_POINT_LOSS 20

//make sure MAX_BOT_TYPE_DROP is always larger or equal to MAX_BOT_DROP
#define MAX_BOT_DROP 10
#define MAX_BOT_TYPE_DROP 25

struct guild_db
{
	char name[21];
	char owner[21];
	char website[201];
	int bank_money;
	int bank_lend;
	char rank_name[6][21];
	int rank_allow[6][5];
	char enemy[3][21];
	char deco_name[MAX_DECO][31];
	int deco_activated[MAX_DECO];

	//non-saved
	int pay_out;
};

struct planet_ship
{
	int have;
	int max;
	int exclude[8];
};

struct planet_trade_disaster
{
	int g;
	int is_low;
	int i;
};

struct server_satellite_db
{
	//saveable information
	int level;
	int guild_id;
	int gun[SAT_LEVEL_MAX * SAT_GUN_INC];
	int missile[SAT_LEVEL_MAX * SAT_MISSILE_INC];
	int gun_destroyed[SAT_LEVEL_MAX * SAT_GUN_INC];
	int missile_left[SAT_LEVEL_MAX * SAT_MISSILE_INC];
	int money;
	int money_collected;
	int bug_guild_id;

	//non saveable
	int hull;
	char guild_name[21];

	//server use only
	double gun_time[SAT_LEVEL_MAX * SAT_GUN_INC];
	double missile_time[SAT_LEVEL_MAX * SAT_MISSILE_INC];
	int gun_can_attack[SAT_LEVEL_MAX * SAT_GUN_INC];
	int missile_can_attack[SAT_LEVEL_MAX * SAT_MISSILE_INC];
	int user_attacking[MAX_USER_CON];
	int bot_attacking[BOT_TOTAL_MAX];
	int bot_type_attacking[BOT_TOTAL_MAX];
	int user_attacking_max;
	int bot_attacking_max;
	int zone, sector;
	int shield;
	int shield_level;
	double shield_disable_time;

	//hub use only
	int last_warning_time;
};

struct server_planet_db
{
	char name[21];
	char guild[21];
	char bar_name[21];
	char owner[21];
	int home_class;
	int sat_level;
	int sat_hull;
	int is_gate;
	int is_server_gate;
	int is_home;
	int has_guild_halls;
	int ship_plan;
	int ship_plan_cost;
	int ship_plan_exp;
	int ship_plan_kills;
	int trade_planet[PLANET_TRADE_MAX];
	int good_price[10];
	int good_price_high[10];
	int good_price_low[10];
	int good_ammount[10];
	int good_ammount_high[10];
	int good_ammount_low[10];
	int contra_price[10];
	int contra_price_high[10];
	int contra_price_low[10];
	int contra_ammount[10];
	int contra_ammount_high[10];
	int contra_ammount_low[10];
	struct planet_trade_disaster trade_disaster[PLANET_TRADE_DISASTER_MAX];
	int trade_disaster_max;
	struct planet_ship ship[SHIP_MAX];
	int gun[GUN_MAX];
	int missile[MISSILE_MAX];
	struct server_satellite_db sat;

	//for other use
	int range;
#ifdef IS_CLIENT
	SDL_Surface *loc_img;
	SDL_Surface *hyper_img;
#endif
};

struct server_gun_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
	int exp_static;
	int exp_added;
	int tech_level;
	int type;

	//for other use
	double rank;
	int range;
#ifdef IS_CLIENT
	SDL_Surface *gun_img[16];
	SDL_Surface *gun_pod_img;
#endif
};

struct server_missile_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
	int exp_static;
	int exp_added;
	int tech_level;
	int type;

	//for other use
	double rank;
	int range;
#ifdef IS_CLIENT
	SDL_Surface *missile_img[16];
	SDL_Surface *missile_pod_img;
#endif
};

struct server_sector_db
{
	int planet;
	int shield;
	int range;
	int x;
	int y;
	int sector[5];
	int zone;
	int is_sun;
	int is_nebula;
	int free_scan;
	int contra_amount[10];
	int good_amount[10];

	//for other
	int sun_damage;
	int has_player;
	int has_guild_member;
	char has_rune[4];
};

struct server_zone_db
{
	char name[21];
	int home_class;
	int free_scan;
	struct server_sector_db sector[SECTOR_MAX];

	//for other use
	int highest_range;
#ifdef IS_CLIENT
	SDL_Surface *loc[LOC_MAX];
	SDL_Surface *loc_safe[LOC_MAX];
	SDL_Surface *hyper_sec;
	SDL_Surface *hyper_sel;
	SDL_Surface *hyper_safe;
	int loc_max;
	int loc_safe_max;
#endif
};


struct server_ship_db
{
	char name[21];
	int cost[8];
	int exp[8];
	int hull[8];
	int shield[8];
	int evasion[8];
	int speed[8];
	int range[8];
	int cargo[8];
	int gun_ammount[8];
	int missile_ammount[8];
	int misc_ammount[8];

	//for other use
	double rank[8];
	int buy_range[8];
#ifdef IS_CLIENT
	SDL_Surface *ship_img[8][16];
	SDL_Surface *fship_img[8][5];
	SDL_Surface *fship_sos_img[8][5];
	SDL_Surface *space_img[8];
	SDL_Surface *rad_img[8];
	SDL_Surface *plan_img;
#endif
};

enum BOT_DROP_TYPE
{
	no_type = 0,
	is_gun = 1,
	is_missile = 2,
	is_ship = 3
};

struct bot_drop_db
{
	int ship_class;
	int ship_kind;
	int weapon;
	float chance;
	enum BOT_DROP_TYPE drop_type;
};

struct server_bot_db
{
	char name[21];
	int ship_class;
	int ship_kind;
	int gun;
	float def_multi;
	int exp;
	struct bot_drop_db random_drop[MAX_BOT_DROP];

	//non-db standard stuff
	int sector;
	int hull;
	int hull_max;
	int cargo;
	int cargo_max;
	int evasion;
	int shield;
	int shield_max;
	int shield_level;
	double shield_disable_time;
	int fighting_bot;
	int user_attacking;
	int bot_type_attacking;
	double jump_speed;
	double jump_time;
	double gun_time;
	double death_time;
	int contra_amount[10];
	int good_amount[10];
	int is_mothership;
	int bot_following;
	int bot_chasing[MAX_BOT_FOLLOW];
	int bot_chasing_amt;
	int bot_chasing_max;

	int missile_amount;
	int missile_damage;
	int missile_left[16];
	double missile_next_time[16]; //time in which aloud to shoot next
	double missile_speed[16];
};

struct server_bot_type_db
{
	char name[21];
	char guild[21];
	int bot_class;
	int aggression; //0= no fight, 1=militia, 2=klyn, 3=Rin, 4=Pirate
	int intelligence;
	int zone;
	int field_type;
	int drops_contraband;
	int is_smuggler;
	int contra_price_min[10];
	int contra_price_max[10];
	int contra_price[10];
	char sector[SECTOR_MAX];
	struct server_bot_db bot[BOT_MAX];
	struct bot_drop_db random_drop[MAX_BOT_TYPE_DROP];

	//non-db standard stuff
	int bot_top;
	int g_sector[SECTOR_MAX];
	int g_sector_max;
	char on_list[MAX_USER_CON];
	int ally[BOT_TYPE_MAX];
};

struct server_db
{
	char login[21];
	char password[21];
	char hub_address[51];
	int home_class;
	int reference_number;

	//from the hub
	char servername[21];
	char real_address[50];
	int con_id;
	char server_ip[50];
	int x;
	int y;
	int debug;
	int db_loaded;
#ifdef IS_HUB
	struct server_planet_db planet[PLANET_MAX];
	struct server_zone_db zone[ZONE_MAX];
	struct server_ship_db ship[SHIP_MAX];
	struct server_bot_type_db bot_type[BOT_TYPE_MAX];
	struct server_gun_db gun[GUN_MAX];
	struct server_missile_db missile[MISSILE_MAX];
#endif
#ifdef IS_CLIENT
	struct server_planet_db planet[PLANET_MAX];
	struct server_zone_db zone[ZONE_MAX];
	struct server_ship_db ship[SHIP_MAX];
	struct server_bot_type_db bot_type[BOT_TYPE_MAX];
	struct server_gun_db gun[GUN_MAX];
	struct server_missile_db missile[MISSILE_MAX];
#endif

	int set_zone_sun_damages;

	//for the good people
	char rewrite_sat_db;
};

//clearing
extern void clear_planet_db ( struct server_planet_db *the_db, int *home_class );
extern void clear_galaxy_db ( struct server_zone_db *the_db );
extern void clear_ship_db ( struct server_ship_db *the_db );
extern void clear_bot_db ( struct server_bot_type_db *the_db );
extern void clear_gun_db ( struct server_gun_db *the_db );
extern void clear_missile_db ( struct server_missile_db *the_db );

//reading
extern void read_planet_db_entry ( struct server_planet_db *the_db, int *home_class, char *input_line );
extern void read_galaxy_db_entry ( struct server_zone_db *the_db, char *input_line );
extern void read_ship_db_entry ( struct server_ship_db *the_db, char *input_line );
extern void read_bot_db_entry ( struct server_bot_type_db *the_db, char *input_line );
extern void read_gun_db_entry ( struct server_gun_db *the_db, char *input_line );
extern void read_missile_db_entry ( struct server_missile_db *the_db, char *input_line );

//writing
extern void write_planet_db_universal ( char *file_name, struct server_planet_db *the_db, int *home_class );
extern void write_galaxy_db_universal ( char *file_name, struct server_zone_db *the_db );
extern void write_ship_db_universal ( char *file_name, struct server_ship_db *the_db );
extern void write_bot_db_universal ( char *file_name, struct server_bot_type_db *the_db );
extern void write_gun_db_universal ( char *file_name, struct server_gun_db *the_db );
extern void write_missile_db_universal ( char *file_name, struct server_missile_db *the_db );

//other common functions
extern int gun_exp_requirement ( struct server_gun_db *the_db, int g, int ship_exp );
extern int missile_exp_requirement ( struct server_missile_db *the_db, int g, int ship_exp );
extern char* gun_type_name ( char *dest, int type );
extern char* missile_type_name ( char *dest, int type );
extern char* gun_type_description ( char *dest, int type );
extern char* missile_type_description ( char *dest, int type );

#endif

