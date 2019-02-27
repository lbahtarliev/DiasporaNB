/*
Author: Mike Bok
Email: freaknigh@hotmail.com
Website: www.nighsoft.net
License and Rights: Full licensing and Rights available at www.nighsoft.net.
If licensing and rights not available at www.nighsoft.net, then all the following apply as bulleted:
-all rights reserved.
-you can not modify source code in this file.
-you can compile and use this source code.
*/


#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define VERSION 1

//hub
#define MAX_SERVER 100
#define MAX_GUILD 500

//server
#define SHIP_MAX 20
#define GUN_MAX 50
#define MISSLE_MAX 50
#define MISSILE_MAX MISSLE_MAX
#define PLANET_MAX 100
#define PLANET_TRADE_MAX 5
#define PLANET_TRADE_DISASTER_MAX 2
#define MAX_TRADE_DISASTER 10
#define SECTOR_MAX 300
#define ZONE_MAX 10


//sound
#define MAX_SOUND 30

//client
#define LOC_MAX 6
#define FIELD_TYPE_MAX 10
#define SPECIAL_FSHIP_MAX 2
#define SUN_RADIUS 100
#define SUN_DAMAGE 5
#define ZOOM_IN_AMT 1.2
#define ZOOM_OUT_AMT 1.0 / ZOOM_IN_AMT
#define ZOOM_MAX 3

//sound struct
struct sound_sample 
{
	SDL_AudioSpec sound_info;
	Uint8 *data;
	Uint32 dpos;
	Uint32 dlen;
	int repeat;
};

//server structs
struct planet_ship
{
	int have;
	int max;
	int exclude[8];
};

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
};

struct planet_trade_disaster
{
    int g;
    int is_low;
    int i;
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
	struct planet_ship ship[SHIP_MAX + FIELD_TYPE_MAX];
	int gun[GUN_MAX];
	int missle[MISSLE_MAX];
	SDL_Surface *loc_img;
	SDL_Surface *hyper_img;
};

struct server_gun_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
	SDL_Surface *gun_img[16];
	SDL_Surface *gun_pod_img;
};

struct server_missle_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
	SDL_Surface *missile_img[16];
	SDL_Surface *missile_pod_img;
};

struct sector_db
{
	int planet;
	int shield;
	int range;
	double x;
	double y;
	int sector[5];
	int is_sun;
	int is_nebula;
	int good_amount[10];
	int contra_amount[10];
	
	int sun_damage;
};

struct server_zone_db
{
	char name[21];
	int home_class;
	struct sector_db sector[SECTOR_MAX];
	SDL_Surface *loc[LOC_MAX];
	SDL_Surface *loc_safe[LOC_MAX];
	SDL_Surface *hyper_sec;
	SDL_Surface *hyper_sel;
	SDL_Surface *hyper_safe;
	int loc_max;
	int loc_safe_max;
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
	int missle_ammount[8];
	int misc_ammount[8];
	SDL_Surface *ship_img[8][16];
	SDL_Surface *fship_img[8][5];
	SDL_Surface *space_img[8];
	SDL_Surface *rad_img[8];
};

struct server_db
{
	char servername[21];
	int home_class;
	int db_loaded;
	struct server_planet_db planet[PLANET_MAX];
	struct server_zone_db zone[ZONE_MAX];
	struct server_ship_db ship[SHIP_MAX + FIELD_TYPE_MAX];
	struct server_gun_db gun[GUN_MAX];
	struct server_missle_db missle[MISSLE_MAX];
	
	int set_zone_sun_damages;
};

struct user_ship_db
{
	char name[21];
	int server_id;
	int type; //ie zeyhper
	int kind; //ie seeker
	int cost;
	int exp;
	int hull;
	int hull_base;
	int hull_max;
	int shield;
	int shield_base;
	int evasion;
	int speed;
	int speed_base;
	int range;
	int cargo;
	int cargo_base;
	int gun[16];
	int gun_id[16];
	int gun_destroyed[16];
	int gun_ammount;
	double gun_speed;
	int missile[16];
	int missile_id[16];
	int missile_left[16];
	int missile_ammount;
	int misc[16];
	int misc_id[16];
	int misc_ammount;
	int good_ammount[10];
	int contra_ammount[10];
};

struct nif_images
{
	SDL_Surface *no_gun_pod;
	SDL_Surface *no_missile_pod;
	SDL_Surface *no_planet_img;
	SDL_Surface *no_repair_img;
	SDL_Surface *no_shop_img;
	SDL_Surface *no_loc_img;
	SDL_Surface *no_loc_safe_img;
	SDL_Surface *no_loc_p_img;
	SDL_Surface *no_hyper_p_img;
};

struct game_images
{
	SDL_Surface *mini_good[10];
	SDL_Surface *mini_small_good[10];
	SDL_Surface *mini_contra[10];
	SDL_Surface *guild_toggle_off;
	SDL_Surface *guild_toggle_on;
	SDL_Surface *fship_sat;
	SDL_Surface *fship_sat_destroyed;
};

struct game_memory
{
	char username[21];
	char hub_address[51];
	int money;
	int exp;
	int guild_balance;
	int guild_rank_id;
	int user_id;
	int server_id;
	int home_id;
	int home_planet;
	int zone;
	int sector;
	int ship_sel;
	int sound_on;
	struct guild_db guild;
	struct user_ship_db ship[10];
	struct nif_images nif_image;
	struct game_images game_image;
	//int last_planet is refered to as planet.last_planet
};

//sdl
extern SDL_Surface *screen;
extern TTF_Font *ttf_font;
extern struct sound_sample game_sound[MAX_SOUND];
extern int sound_max;

//game
extern struct game_memory game;
extern struct server_db server[MAX_SERVER];

//sounds
extern int sound_explosion;
extern int sound_engine;
extern int sound_jump;
extern int sound_warning;
extern int sound_gun;
extern int sound_enemy_gun[2];
extern int sound_missile;
extern int sound_shield;
extern int sound_impact[5];
extern int sound_button;

//HELL!
extern void set_sun_damages(struct server_zone_db *the_zone);

