#define VERSION 1
#define SHIP_MAX 20
#define GUN_MAX 50
#define MISSLE_MAX 50
#define MISC_MAX 30
#define PLANET_MAX 30
#define SECTOR_MAX 300
#define ZONE_MAX 10
#define BOT_MAX 200
#define BOT_TYPE_MAX 50
#define BOT_TOTAL_MAX BOT_TYPE_MAX * BOT_MAX
#define MAX_USER_CON 1000
#define MAX_BOT_DROP 5
#define MAX_BOT_TYPE_DROP 15

//for security checking reasons
#define MAX_SERVER 100

//datebase structs
struct server_db
{
	char login[21];
	char password[21];
	char hub_address[51];
	int home_class;
	int reference_number;
};

struct planet_ship
{
	int have;
	int max;
};

struct planet_db
{
	char name[21];
	char guild[21];
	char bar_name[21];
	char owner[21];
	int home_class;
	int sat_level;
	int sat_hull;
	int is_gate;
	int is_home;
	int has_guild_halls;
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
	struct planet_ship ship[SHIP_MAX];
	int gun[GUN_MAX];
	int missle[MISSLE_MAX];
};

struct gun_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
};

struct missle_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
};

struct sector_db
{
	int planet;
	int shield;
	int range;
	int x;
	int y;
	int sector[5];
	int contra_amount[10];
	int good_amount[10];
};

struct zone_db
{
	char name[21];
	int home_class;
	struct sector_db sector[SECTOR_MAX];
};


struct ship_db
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
};

enum BOT_DROP_TYPE
{
	no_type = 0,
	gun = 1,
	missile = 2,
	ship = 3
};

struct bot_drop_db
{
	int ship_class;
	int ship_kind;
	int weapon;
	enum BOT_DROP_TYPE drop_type;
};

struct bot_db
{
	char name[21];
	int ship_class;
	int ship_kind;
	int gun;
	int def_multi;
	int exp;
	
	//non-db standard stuff
	int sector;
	int hull;
	int hull_max;
	int cargo;
	int cargo_max;
	double gun_evasion;
	double gun_reduction;
	double missile_evasion;
	double missile_reduction;
	int fighting_bot;
	int user_attacking;
	int bot_type_attacking;
	int jump_speed;
	int jump_time;
	int gun_time;
	int death_time;
	int contra_amount[10];
	int good_amount[10];
	
	struct bot_drop_db random_drop[MAX_BOT_DROP];
};

struct bot_type_db
{
	char name[21];
	char guild[21];
	int bot_class;
	int aggression;
	int intelligence;
	int zone;
	int field_type;
	char sector[SECTOR_MAX];
	struct bot_db bot[BOT_MAX];
	struct bot_drop_db random_drop[MAX_BOT_TYPE_DROP];
	
	//non-db standard stuff
	int bot_top;
	int g_sector[SECTOR_MAX];
	int g_sector_max;
};

//game structs

struct gun_info
{
	int damage;
	int speed;
	int accuracy;
};

struct missile_info
{
	int damage;
	int speed;
	int accuracy;
	int left;
	double time;
	double previous_time;
};

struct user_ship
{
	char class_name[21];
	int server_id;
	int type; //ie zeyhper
	int kind; //ie seeker
	int cost;
	int exp;
	int hull;
	int hull_max;
	int shield;
	int shield_max;
	int shield_level;
	int evasion;
	int speed;
	int range;
	int cargo;
	int gun_ammount;
	int missile_ammount;
	struct gun_info gun[16];
	struct missile_info missile[16];
	
};

struct con_user_info
{
	int user_id;
	char username[21];
	char guild[21];
	int user_attacking;
	int bot_type_attacking;
	int attacking_bot;
	double attack_time;
	double attack_speed;
	int update_hull;
	int update_missiles;
	int update_guns;
	int sector;
	int zone;
	int last_planet;
	int landed;
	int inbar;
	int jump_time;
	int has_galaxy_info;
	struct user_ship ship;
};


extern struct server_db server_info;
extern struct planet_db planet[PLANET_MAX];
extern struct zone_db zone[ZONE_MAX];
extern struct ship_db ship[SHIP_MAX];
extern struct bot_type_db bot_type[BOT_TYPE_MAX];
extern struct gun_db gun[GUN_MAX];
extern struct missle_db missle[MISSLE_MAX];

extern struct con_user_info user_con[MAX_USER_CON];
extern int user_con_top;


