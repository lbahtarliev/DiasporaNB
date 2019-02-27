//consts
#define IS_HUB 1
//#define USE_MYSQL 1 //enables code specifically designed for the nighsoft.net host

#define MYSQL_USER "user"
#define MYSQL_PASSWORD "pass"

//standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <errno.h>
#ifdef USE_MYSQL
#include <mysql/mysql.h>
#endif

#ifdef _WIN32 //if windows
	#include <windows.h>		//win for Sleep(1000)
	#include <direct.h>		//win
#else
	#include <sys/types.h>		//lin
	#include <sys/socket.h>		//lin
	#include <netinet/in.h>		//lin
	#include <netdb.h>		//lin
	#include <sys/stat.h>		//lin
	#include <sys/ioctl.h>		//lin
	#include <arpa/inet.h>
    #include <poll.h>
#endif

//game
#define MIN_USER_VERSION 1
#define MIN_SERVER_VERSION 1
#define GUILD_COST 1500000
#define MIN_MONEY 50000
#define MAX_BOUNTY_LIFETIME 200
#define MAX_BOUNTY_LIFETIME_INC 24
#define MIN_BOUNTY_LIFETIME_INC_PRICE 30000
#define MIN_BOUNTY_WORTH 3000000
#define MAX_PRIVATE_CHAT_USERS 20
#define KILL_REWARD 0.05
#define MFC_EXP_MAX 2000
#define CREATION_LIMIT_HOUR 4
#define CREATION_LIMIT_MINUTE 30
#define CREATION_LIMIT_SECONDS (CREATION_LIMIT_MINUTE * 60) + (CREATION_LIMIT_HOUR * 3600)
#define FLOOD_LIMIT 1.0


//ping
#define PING_USER_DELAY_SECONDS 60
#define PING_SERVER_DELAY_SECONDS (60 * 3)

//deactivated user stuff
#define DEACTIVATED_RANGE_LIMIT 5
#define DEACTIVATED_EXP_LIMIT 800
#define DEACTIVATED_PRICE_LIMIT 8000000

//DS Stuff
#define DS_FOR_ACTIVATION 5
#define DC_FOR_DS 10000000
#define EXP_FOR_DS 50

//sock stuff
#define MAX_DATA_STORED 40000
#define MAX_BUF_SIZE 20000
#define MAX_CONNECTIONS_PER_IP 7

/*
//database
#define MAX_USER 1000
#define MAX_GUILD 500
#define MAX_SERVER 100
#define MAX_ADMIN 20
#define MAX_BAN 200
#define MAX_BOUNTY 38
*/
//sock
#define MAX_ADMIN_CON 100
#define MAX_USER_CON 1000
#define MAX_SERVER_CON 100
#define ADMIN_PORT 4800
#define USER_PORT 4200
#define SERVER_PORT 4000

#include "database_common.h"

/*
//server
#define SHIP_MAX 20
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

//sat
#define SAT_LEVEL_MAX 9
#define SAT_HULL_INC 20000
#define SAT_GUN_INC 4
#define SAT_MISSILE_INC 2
#define SAT_TAX_INC 5000
#define SAT_COST_INC 750000
*/

//Structs --------------------------------------------------
struct user_ship
{
    int ship_id; //server_id that the ship is derived from
    int brand; //Zeyhper etc
    int type; //Seeker etc
    int hull;
    int hull_max;
    int speed_max;
    int cargo_max;
    int shield_max;
    unsigned int cargo[10];
    unsigned int contra[10];
    int gun_id[16]; //server_id that the gun is derived from
    int gun[16];
    int gun_destroyed[16];
    int missile_id[16]; //server_id that the missle is derived from
    int missile[16];
    int missile_left[16];
    int misc[16];
    int sent_info;
    
    int destroy_code;
};

struct user_db
{
	char login[21];
	char password[21];
	char lease_login[21];
	char lease_password[21];
	char username[21];
	char email[51];
	int user_class;
	int money;
	int standards;
	int user_id;
	int exp;
	int kills;
	int user_class_points;
	char user_ip[16];
	char user_ip_creator[16];
	char user_ip_lease[16];
	unsigned long the_s_addr;
	unsigned long the_s_addr_creator;
	unsigned long the_s_addr_lease;
	double last_message_time;
	int time_created;
	int time_logged;
	int time_lease_logged;
	int con_id; //if the user is connected, then to what #?
	int server_id;
	int guild_id;
	int guild_rank;
	int guild_balance;
	int guild_balance_update;
	int guild_request;
	int banned; //-1 = banfree, 1 = banned
	int ship_selected;
	int debug;
	int version;
	int last_planet;
	int home_planet;
	int home_id;
	int landed;
	int activated;
	int lease_logged_in;
	struct user_ship ship[10];
	int has_deco[MAX_DECO];
	int has_plan[MAX_SERVER][SHIP_MAX];
	
	//temp stuff
	int has_goods;
	int zone, sector;
	int uni_chat_on;
	int private_chat_user[MAX_PRIVATE_CHAT_USERS];
	int private_chat_status[MAX_PRIVATE_CHAT_USERS]; //0-none, 1-we_requesting, 2-accepted
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

struct bounty_db
{
       int user_id;
       int price;
       int lifetime;
};

struct game_info
{
	char rewrite_user_db;
	char rewrite_guild_db;
	char rewrite_bounty_db;
	double last_pricechange;
	char hub_name[21];
};

struct client_info
{
	int c_id;
	struct sockaddr_in c_in;
	char recv_data[MAX_DATA_STORED];
	int recv_amt;
	int connected;
	int sending_mutex;
};

//Prototypes --------------------------------------------------

//main
extern void main_loop();

//Xinsock Prototypes
extern void start_server();
extern void send_con_admin(int con, char* message);
extern void send_con_user(int con, char* message);
extern void send_con_server(int con, char* message);
extern void raw_send_con_user(int con, char* message);
extern void raw_send_con_server(int con, char* message);
extern void close_con_admin(int con);
extern void close_con_user(int con);
extern void close_con_server(int con);
extern void get_con_admin_ip(int con, char *dest);
extern void get_con_user_ip(int con, char *dest);
extern void get_con_server_ip(int con, char *dest);
extern void dejunk_bytes(char *buf, int amt);
extern char *ip_from_s_addr(unsigned long s_addrw, char *dest);


//Protocal Prototypes
extern void process_packet_admin(int con, char* message);
extern void process_packet_user(int con, char* message);
extern void process_packet_server(int con, char* message);
extern void con_user_connect(int con);

//Database Prototypes
extern void read_all_db();
extern void read_all_server_db();
extern int read_all_servers_db(int s);
extern void read_user_db();
extern void read_guild_db();
extern void read_server_db();
extern void read_ban_db();
extern void read_admin_db();
extern void read_bounty_db();
extern void read_hub_db();
extern void write_all_db();
extern void write_user_emails();
extern void write_user_db(int is_backup);
extern void write_guild_db(int is_backup);
extern void write_server_db();
extern void write_ban_db();
extern void write_admin_db();
extern void write_bounty_db();
extern void write_required_server_sats();
extern void write_hub_db();
extern int server_read_planet_db(int server_number);
extern int server_read_galaxy_db(int server_number);
extern int server_read_ship_db(int server_number);
extern int server_read_gun_db(int server_number);
extern int server_read_missile_db(int server_number);
extern int server_read_bot_db(int s);
extern int server_read_satellite_db(int s);
extern void check_databases_for_save();
extern void set_galaxy_ranges(struct server_zone_db *the_zone, struct server_planet_db *the_planet, struct server_ship_db *the_ship, struct server_gun_db *the_gun, struct server_missile_db *the_missile);
extern int planet_zone(struct server_zone_db *the_zone, int p);
extern int ip_is_banned(char *ip_address);
extern void print_out_all_ship_stats();
extern int user_creation_limit(unsigned long the_addr);
extern void fix_user_db_semi_colons();
extern void find_user_db_clones();


//Common Prototypes
extern void split(char dest[], char message[], char char_spliter, int *initial);
extern int str_match(char *str1, char *str2);
extern int str_match_lcase(char *str1, char *str2);
extern void lcase(char *dest, char *src);
extern void right(char *dest, char *src, int point);
extern int misc_pod_value(int misc_pod_number);
extern int misc_pod_exp(int misc_pod_number);
extern int misc_pod_cost(int misc_pod_number);
extern void uni_pause(int m_sec);
extern void printd(char *message);
extern void printd_admin(char *message);
extern void printd_ds(char *message);
extern double current_time();
extern char *to_jabber_name(char *dest, char *username);
extern char *file_date(char *filename);
extern char *ship_name(int ship_number, char *dest);
extern int add_will_overflow(int a, int b);

//Admin Prototypes
extern int online_admins();
extern void admin_logoff_duplicates(int a);
extern void admin_login(int con, char *username, char *password);
extern void admin_logoff(int con);
extern void admin_shell(int con,char *message);

//User Prototypes
extern void user_do_lease_time_message(int u);
extern int user_check_lease_loggedin(int u);
extern int find_just_any_login(char *login);
extern int find_login(char *login, char *password);
extern int find_lease_login(char *login, char *password);
extern int online_users();
extern void user_login(int con, char *login, char *password);
extern void user_logoff(int con);
extern int user_to_id(char *username);
extern int user_login_to_id(char *username);
extern int userid_to_id(int user_id);
extern int user_to_bounty(int u);
extern int guild_name_to_id(char *guildname);
extern void guild_clear_decorations(int g);
extern void send_user_debug(int con, char *message);
extern void create_user(int con, char *login, char *password, char *username, char *email);
extern void purchase_ship(int con, int ship_brand, int ship_kind);
extern void purchase_ship_mod(int con, int mod_type, int mod_number);
extern void send_user_ship_info_to_user(int user_id);
extern void send_user_ship_info_to_server(int user_id);
extern void send_user_hanger_info(int user_id);
extern void send_user_hanger_ship(int user_id, int ship_number);
extern void set_user_ship_misc(int user_id, int ship_number);
extern void set_user_ship_selected(int con, int new_selection);
extern void sell_user_ship_mod(int con, int mod_type, int mod_number);
extern void configure_user_ships();
extern void sell_user_ship_chassis(int user_id);
extern void repair_user(int con);
extern void reload_user_missile(int con, int wep_num);
extern void repair_user_gun(int con, int wep_num);
extern void send_user_gun_info(int con, int gun_id, int gun);
extern void send_user_missile_info(int con, int missile_id, int missile);
extern void user_purchase_goods(int con, int good_num, int ammount);
extern void user_sell_goods(int con, int good_num, int ammount);
extern int user_good_ammount(int con);
extern int user_id_good_amount(int u);
extern int user_cargo_amount(int u);
extern void user_send_money(int con);
extern void user_update_good_ammount(int con, int good_num);
extern void user_update_contra_ammount(int con, int g);
extern void user_send_cargo_info(int con);
extern void user_register_guild(int con, char *new_guild);
extern int user_leave_guild(int u);
extern void send_user_name_guild(int u);
extern void user_relay_guild_listings(int con);
extern void user_request_guild_info(int con, char *guild_name);
extern void user_send_specific_guild_info(int con, int g);
extern void user_send_guild_info(int con);
extern void user_edit_guild(int con, char *message);
extern void user_edit_guild_more(int con, char *message);
extern void user_withdraw_bank(int con, int ammount);
extern void user_deposit_bank(int con, int ammount);
extern void notify_guild_money(int g);
extern void user_send_guild_balance(int con);
extern int user_exp_algo(int user_exp, int enemy_exp);
extern void user_send_exp(int u);
extern void user_logoff_all_server(int u);
extern void user_logoff_duplicates(int u);
extern void user_retrive_stats(int con, char *username);
extern void user_retrive_stats_list(int con, int list_type);
extern int user_worth(int u);
extern int user_ship_worth(int u, int s);
extern int user_ship_worth_lost(int u, int s);
extern void user_add_kill(int u, int the_class, int addition);
extern void user_update_home_info(int u);
extern void send_bounty_list(int con);
extern void user_add_bounty(int con, char *victom_name, int price);
extern void remove_bounty(int i);
extern void reduce_bounty_lifetime();
extern int create_bounty(int u, int price);
extern void append_bounty(int i, int price);
extern int free_bounty();
extern void reward_bounty(int b_u, int u, int max_amount);
extern void reward_kill_reward(int u, int v_u);
extern void send_user_public_chat(int con, int color, char *message);
extern void send_user_private_chat(int con, int color, char *message);
extern int guild_worth(int i);
extern int guild_players(int i);
extern int user_deserve_guild_info(int u, int g, int r);
extern void user_transfer_credits(int con, char *to_name, int amount);
extern void guild_enlist_user(int con, char *victom_name);
extern void guild_remove_user(int con, char *victom_name);
extern void guild_promote_user(int con, char *victom_name);
extern void guild_demote_user(int con, char *victom_name);
extern void guild_user_accept(int con, char *guildname);
extern void user_guild_chat(int con, char *message);
extern void user_return_guild_list(int con);
extern void user_return_online_guild_list(int con);
extern int user_award_guild_reward(int u, char *the_guild);
extern void user_purchase_upgrade_sat(int con);
extern void user_deposit_sat_money(int con, int amount);
extern void user_buy_sell_sat_weapon(int con, int is_missile, int i, int w);
extern void user_repair_reload_sat_weapon(int con, int is_missile, int i);
extern int user_can_edit_sat(int u, int g, int s, int p);
extern int is_game_reserved_name(char *name);
extern void relay_user_gun_repaired(int u, int g);
extern void user_enter_planet_guild(int con, int check_p);
extern void sell_all_user_goods(int u);
extern void sell_all_user_ship_goods(int u, int s);
extern int user_has_goods(int u);
extern void clear_all_user_ship_goods(int u, int sh);
extern void user_sell_all_current_ship_goods(int con);
extern void setup_user(int u);
extern void user_send_hull(int u);
extern void user_assign_ship_destroy_code(int u, int sh);
extern void assign_all_user_ship_destroy_codes();
extern void user_sell_ship(int u, int sh);
extern void user_send_home(int u);
extern int user_check_reward_min_money(int u);
extern int is_game_bot_name(char *name);
extern void user_check_ship_validity(int u);
extern void user_send_guilds_money(int u);
extern void user_send_activated_user(int u);
extern void user_send_activated_server(int u);
extern void user_set_zone_sector(int u, int z, int s);
extern void user_drop_good(int u, int g, int amount);
extern void user_drop_contra(int u, int g, int amount, int g_price);
extern void user_drop_all_goods(int u);
extern void relay_sat_under_attack_warning(int s, int p);
extern void user_add_chat_user(int con, char *username);
extern void user_remove_chat_user(int con, char *username);
extern void user_list_chat_user(int con);
extern void user_list_online_chat_user(int con);
extern void user_relay_private_chat(int con, char *message);
extern void clear_all_user_private_chats();
extern void send_user_private_chat_list(int u);
extern void send_user_private_chat_entry(int u, int i);
extern void send_user_private_chat_remove(int u, char *username);
extern void user_clear_private_chats_raw(int u);
extern void user_change_password(int con, char *old_password, char *new_password);
extern void user_retrive_short_user_stats(int con, char *search_user);
extern void user_retrive_short_guild_reward_list(int con, char *search_guild);
extern void user_edit_guild_deco(int con, int d_i, char *deco_name, int deco_activated);
extern void guild_anouce_deco_change(int g, int d_i);
extern void guild_award_deco(int con, char *player_name, char *deco_name);
extern void guild_remove_deco(int con, char *player_name, char *deco_name);
extern void guild_list_deco(int con, char *player_name);
extern void guild_list_decog(int con, char *guild_name);
extern void guild_list_bugs(int con);
extern int decoration_to_id(int g, char *deco_name);
extern void user_clear_decorations(int u);
extern void user_bug_sat(int con);
extern void user_request_pc_time(int con);
extern void clear_all_user_uni_chat();
extern void user_toggle_uni_chat(int con);
extern void user_uni_chat(int con, char *message);
extern void insert_all_users_jabber();
extern int ship_owned_amount(int s, int sh, int k);
extern void user_award_mfc(char *user_name, int exp_added, int money_added);
extern void user_get_dsamount(int con);
extern void user_transferds(int con, char *playername, int amount);
extern void user_buyactivation(int con, char *playername);
extern void user_buyexp(int con, char *playername, int amount);
extern void user_buydc(int con, char *playername, int amount);
extern int activate_user(int u);
extern void award_user_ds(char *playername, int amount);
extern int flood_check(int u);
extern void user_award_missile(int u, int sh, int s, int wep, int k);
extern void user_award_gun(int u, int sh, int s, int wep, int k);
extern void user_award_ship(int u, int sh, int s, int s_c, int s_k);
extern int user_likely_barcode(char *name);
extern int guild_rejoin_owner(int u, char *guildname);
extern int user_bad_spacing(char *name);
extern void ping_user_con(int con);
extern void ping_all_users();
extern void user_ping_response(int con);
extern void user_do_noob_message(int u);
extern void user_purchase_plans(int con, int plan_check);
extern void user_send_plan_list(int u);
extern int user_at_home_planet(int u);
extern int has_bad_user_characters(char *message);
extern void user_set_lease_login(int con, char *new_lease_login, char *new_lease_password);

//Server Prototypes
extern int online_servers();
extern void server_login(int con, char *login, char *password);
extern void server_logoff(int con);
extern void server_logoff_duplicates(int s);
extern void send_server_debug(int con, char *message);
extern void retrieve_user_info(int con, char *rtn_con, int user_id, unsigned long s_addrw);
extern void set_user_launch(int user_id, int is_landed, int new_planet);
extern void destroy_user(int con, int user_id, int killer_id);
extern void destroy_user_disconnected(int con, char *username, int destroy_code, int z, int s, int killer_id);
extern void award_killer(int k_i, int i, int the_worth_lost);
extern void set_user_hull(int user_id, int new_hull);
extern void set_user_payloads(int user_id, char *message);
extern void do_price_change();
extern void server_send_good_contra_info(int i);
extern void server_update_good_ammount(int i, int planet_num, int good_num);
extern void server_clear_sector_amounts(int con);
extern void server_set_good_amounts(int con, int s, int z, char *message);
extern void server_set_contra_amounts(int con, int s, int z, char *message);
extern void server_set_good_amount(int con, int s, int z, int good_num, int good_amount);
extern void server_set_contra_amount(int con, int s, int z, int good_num, int good_amount);
extern void server_add_good_amount(int con, int s, int z, int good_num, int good_amount);
extern void server_add_contra_amount(int con, int s, int z, int good_num, int good_amount);
extern void server_send_sector_good_amount(int con, int s, int z, int g);
extern void server_send_sector_contra_amount(int con, int s, int z, int g);
extern void server_bot_pickup_good(int con, int t, int u, int s, int z, int g, int amount);
extern void server_bot_pickup_contra(int con, int t, int u, int s, int z, int g, int amount);
extern void server_user_pickup_good(int con, int user_id, int s, int z, int g, int amount);
extern void server_user_pickup_contra(int con, int user_id, int s, int z, int g, int amount);
extern void server_user_drop_good(int con, int user_id, int s, int z, int g, int amount);
extern void server_user_drop_contra(int con, int user_id, int s, int z, int g, int amount, int g_price);
extern void server_user_give_bot_exp(int con, int user_id, int exp, int class_amt, char *the_guild);
extern int server_random_home_planet(int s_id, int the_class);
extern void server_update_user_guild(int s, int u);
extern void server_send_all_sat_info(int s);
extern void server_send_sat_info(int s, int p);
extern void server_send_sat_deposit_info(int s, int p, char *username, int deposit_amount);
extern void server_relay_sat_repair(int s, int p);
extern int server_get_server_gate(int s);
extern int server_gate_random_planet(int s);
extern int server_get_least_ranged_planet(int s);
extern void server_set_sat_hull(int con, int p, int new_hull);
extern void destroy_sat(int s, int p);
extern void award_all_sat_taxes();
extern void award_guild_money(int g, int amount);
extern void check_and_repair_all_sats();
extern void send_server_sat_money_collected(int s, int p);
extern void server_relay_sat_hull(int s, int p);
extern void server_relay_sat_money(int s, int p);
extern void server_set_sat_missile_left(int con, int p, int w, int amount);
extern void server_set_sat_gun_destroyed(int con, int p, int w, int is_destroyed);
extern void set_user_gun_destroyed(int user_id, int g);
extern void set_and_send_server_user_has_goods(int u, int has_goods);
extern void check_and_create_bot_guilds();
extern void create_bot_guild(int s, int b_t);
extern int server_name_to_id(char *servername);
extern void set_user_zone_sector(int con, int user_id, int zone, int sector);
extern void send_server_user_gun_info(int s, int g, int u, int ship_g);
extern void send_server_user_missile_info(int s, int g, int u, int ship_g);
extern void pay_all_guilds();
extern void clear_all_guild_payouts();
extern void pay_guild(int g, int amount);
extern void pay_planet_guild(int s, int p, int amount);
extern void pay_planet_and_user_guild(int g, int s, int p, int amount);
extern void guild_pay_ranks(int g);
extern int guild_unaccessable_bank_amount(int g);
extern void guild_anouce_new_balances();
extern void regulate_all_server_sat_bank_maxs();
extern void server_regulate_sat_bank_maxs(int s);
extern void create_backup_folder();
extern void server_user_award_random_drop(int con, int user_id, int s_c, int s_k, int wep, enum BOT_DROP_TYPE drop_type);
extern int server_to_id(char *servername);
extern int planet_to_id(int s, char *planetname);
extern void ping_server_con(int con);
extern void ping_all_servers();
extern void server_ping_response(int con);
extern void server_ping_request(int con);
extern void server_clear_sector_goods(int con, int z, int s);
extern void server_clear_sector_contraband(int con, int z, int s);
extern void regulate_sat_bank_max(int s, int p);

//memory
extern void set_trade_disaster_texts();
extern int get_trade_disaster_max(int g, int lowers_price);
extern void set_random_trade_disaster(struct planet_trade_disaster *dest);

//database_mysql publics
extern void mysql_init_vars();
extern int mysql_make_connection();
extern int mysql_make_disconnection();
extern int mysql_create_sat_table();
extern void mysql_update_sat_entry(int s, int p);
extern void mysql_update_ship_class_entry(int s, int sh);
extern void mysql_update_ship_entry(int s, int sh, int k);
extern void mysql_update_server_entry(int s);
extern void mysql_write_all_tables();
extern int mysql_write_ship_classes_table();
extern int mysql_create_sat_table();
extern int mysql_write_sat_table();
extern int mysql_write_server_table();
extern int mysql_write_ships_table();
extern void mysql_make_jabber_insert(char *username, char *password);
extern void mysql_do_mfc_awards();
extern void mysql_do_ds_awards();

//database_auto
extern void create_server_db();
extern void create_guild_db();
extern void create_admin_db();
extern void create_server_planet_db(int s);
extern void create_server_galaxy_db(int s);
extern void create_server_ship_db(int s);
extern void create_server_gun_db(int s);
extern void create_server_missile_db(int s);
extern void create_server_satellite_db(int s);
extern void create_server_bot_db(int s);
extern void set_server_satellite_default(int s, int p, int level, int g, int gun, int missile);
extern void set_server_sector_info(int s, int zone, int sector, int planet, int shield, int x, int y, int sec1, int sec2, int sec3, int sec4, int sec5, int range, int is_sun, int is_nebula);
extern void set_server_planet_info(int s, int planet_num, int is_gate, int is_home, char *planet_name, char *guild_name, int home_class, char *bar_name, int has_guild_halls, int is_server_gate);
extern void set_server_gun_info(int s, int weapon_num, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type);
extern void set_server_missile_info(int s, int weapon_num, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type);
extern void set_server_ship_info(int s, int k, int z, int cost, int exp, int hull, int shield, int evasion, int speed, int range, int cargo, int guns, int missiles, int miscs);
extern void set_server_bot_type_info(int s, int i, int dum, char *name, char *guild, int the_class, int aggression, int intelligence, int zone, int field_type);
extern void set_server_bot_info(int s, int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp);

//database_mysql hiddens
#ifdef USE_MYSQL
extern int mysql_do_create_sat_table();
extern int mysql_do_create_ship_classes_table();
extern int mysql_do_create_server_table();
extern int mysql_do_create_ships_table();
#endif

//global --------------------------------------------------

//database
extern struct user_db user[MAX_USER];
extern struct guild_db guild[MAX_GUILD];
extern struct server_db server[MAX_SERVER];
extern struct admin_db admin[MAX_ADMIN];
extern struct ban_db ban[MAX_BAN];
extern struct bounty_db bounty[MAX_BOUNTY];
extern int user_max; //# of users loaded \/\/\/\/
extern int guild_max;
extern int server_max;
extern int ban_max;
extern int admin_max;


//sock
extern int admin_con_id[MAX_ADMIN_CON]; //the array id of the user connected on this con#
extern int user_con_id[MAX_USER_CON];
extern int server_con_id[MAX_SERVER_CON];
extern int user_con_version[MAX_USER_CON]; //temporary placements
extern int server_con_version[MAX_SERVER_CON];
extern int admin_con_top; //these are the value of the highest con# connected, used for faster searching
extern int user_con_top;
extern int server_con_top;
extern struct client_info client_admin[MAX_ADMIN_CON];
extern struct client_info client_user[MAX_USER_CON];
extern struct client_info client_server[MAX_SERVER_CON];

//database mysql
#ifdef USE_MYSQL
extern MYSQL *my_sql_conn;
#endif

//game
extern struct game_info game;
