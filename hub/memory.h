//game
#define MIN_USER_VERSION 1
#define MIN_SERVER_VERSION 1
#define GUILD_COST 1500000

//database
#define MAX_USER 1000
#define MAX_GUILD 500
#define MAX_SERVER 100
#define MAX_ADMIN 20
#define MAX_BAN 200

//sock
#define MAX_ADMIN_CON 1000
#define MAX_USER_CON 1000
#define MAX_SERVER_CON 1000

//server
#define SHIP_MAX 20
#define GUN_MAX 50
#define MISSLE_MAX 50
#define PLANET_MAX 30
#define SECTOR_MAX 300
#define ZONE_MAX 10
#define BOT_MAX 600

//server structs
struct planet_ship
{
	int have;
	int max;
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

struct server_gun_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
};

struct server_missle_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
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
	int contra_amount[10];
	int good_amount[10];
};

struct server_zone_db
{
	char name[21];
	int home_class;
	struct server_sector_db sector[SECTOR_MAX];
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
};

struct server_bot_db
{
	char name[21];
};

//struct declarations
struct user_ship
{
        int ship_id; //server_id that the ship is derived from
        int brand;
        int type;
        int hull;
	int hull_max;
	int speed_max;
	int cargo_max;
	int shield_max;
        unsigned int cargo[10];
        unsigned int contra[10];
        int gun_id[16]; //server_id that the gun is derived from
        int gun[16];
        int missle_id[16]; //server_id that the missle is derived from
        int missle[16];
	int missile_left[16];
        int misc[16];
	int sent_info;
};

struct user_db
{
        char login[21];
        char password[21];
        char username[21];
        char email[51];
	int user_class;
        int money;
        int user_id;
	int exp;
	int kills;
	int user_class_points;
        char user_ip[16];
        int con_id; //if the user is connected, then to what #?
        int server_id;
        int guild_id;
        int guild_rank;
        int guild_balance;
        int banned; //-1 = banfree, 1 = banned
	int ship_selected;
	int debug;
	int version;
	int last_planet;
	int home_planet;
	int home_id;
	int landed;
	int activated;
	struct user_ship ship[10];
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
};

struct server_db
{
        char login[21];
        char password[21];
	char servername[21];
	char real_address[50];
	int con_id;
	char server_ip[50];
	int x;
	int y;
	int debug;
	int db_loaded;
	int home_class;
	struct server_planet_db planet[PLANET_MAX];
	struct server_zone_db zone[ZONE_MAX];
	struct server_ship_db ship[SHIP_MAX];
	struct server_bot_db bot[BOT_MAX];
	struct server_gun_db gun[GUN_MAX];
	struct server_missle_db missle[MISSLE_MAX];
};

struct ban_db
{
        char ip[16];
        int ban_free;
};

struct admin_db
{
	char login[21];
	char password[21];
	int level;
	int con_id;
	int user_debug;
	int server_debug;
};

struct game_info
{
	char rewrite_user_db;
	char rewrite_guild_db;
};

//database
struct user_db user[MAX_USER];
struct guild_db guild[MAX_GUILD];
struct server_db server[MAX_SERVER];
struct admin_db admin[MAX_ADMIN];
struct ban_db ban[MAX_BAN];
int user_max; //# of users loaded \/\/\/\/
int guild_max;
int server_max;
int ban_max;
int admin_max;
int bounty_max;

//sock
int admin_con_id[MAX_ADMIN_CON]; //the array id of the user connected on this con#
int user_con_id[MAX_USER_CON];
int server_con_id[MAX_SERVER_CON];
int user_con_version[MAX_USER_CON]; //temporary placements
int server_con_version[MAX_SERVER_CON];
int admin_con_top; //these are the value of the highest con# connected, used for faster searching
int user_con_top;
int server_con_top;

//game
struct game_info game;

