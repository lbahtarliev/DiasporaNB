//standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>

#ifdef _WIN32 //if windows
	#include <windows.h>		//win for sleep(1000)
	//#include <winsock2.h>		//win
#else
	#include <sys/types.h>		//lin
	#include <sys/socket.h>		//lin
	#include <netinet/in.h>		//lin
	#include <netdb.h>		//lin
	#include <sys/param.h>		//lin
	#include <sys/ioctl.h>		//lin
	#include <arpa/inet.h>
	#include <poll.h>
#endif

//defines
#define VERSION 1

#define MAX_USER_CON 1000

#define SUN_RADIUS 100
#define SUN_DAMAGE 5
#define FIELD_REDUCTION 0.2
#define SAFE_DISCONNECT_TIME 10
#define JUMP_DOCK_TIME 0.2

//deactivated user stuff
#define DEACTIVATED_RANGE_LIMIT 5
#define DEACTIVATED_EXP_LIMIT 800
#define DEACTIVATED_PRICE_LIMIT 8000000

//ping
#define PING_USER_DELAY_SECONDS 60
#define PING_HUB_DELAY_SECONDS (60 * 3)

//other timers
#define CLEAR_BOT_GOODS_DELAY_SECONDS (60 * 7)

//to let people know who else is on when the server has low ammount of players
#define LOW_PLAYER_COUNT 5

#include "database_common.h"

//structs
/*
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
    
    
    //non saveable
    int hull;
    char guild_name[21];
    
    //server use only
    int gun_time[SAT_LEVEL_MAX * SAT_GUN_INC];
    int missile_time[SAT_LEVEL_MAX * SAT_MISSILE_INC];
    int gun_can_attack[SAT_LEVEL_MAX * SAT_GUN_INC];
    int missile_can_attack[SAT_LEVEL_MAX * SAT_MISSILE_INC];
    int user_attacking[MAX_USER_CON];
    int bot_attacking[BOT_TOTAL_MAX];
    int bot_type_attacking[BOT_TOTAL_MAX];
    int user_attacking_max;
    int bot_attacking_max;
    int zone, sector;
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
	struct planet_ship ship[SHIP_MAX];
	int gun[GUN_MAX];
	int missile[MISSLE_MAX];
	struct server_satellite_db sat;
};

struct gun_db
{
	char name[21];
	int cost;
	int damage;
	int speed;
	int accuracy;
};

struct missile_db
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
	int is_sun;
	int is_nebula;
	int contra_amount[10];
	int good_amount[10];
	
	int sun_damage;
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
	int missile_ammount[8];
	int misc_ammount[8];
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
};

struct bot_type_db
{
	char name[21];
	char guild[21];
	int bot_class;
	int aggression; //0= no fight, 1=militia, 2=klyn, 3=Rin, 4=Pirate
	int intelligence;
	int zone;
	int field_type;
	char sector[SECTOR_MAX];
	struct bot_db bot[BOT_MAX];
	
	//non-db standard stuff
	int bot_top;
	int g_sector[SECTOR_MAX];
	int g_sector_max;
	char on_list[MAX_USER_CON];
	int ally[BOT_TYPE_MAX];
};
*/

//game structs
struct gun_info
{
	int damage;
	int speed;
	int accuracy;
	int type;
	int destroyed;
};

struct missile_info
{
	int damage;
	int speed;
	int accuracy;
	int type;
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
	double shield_disable_time;
	int evasion;
	int speed;
	int range;
	int cargo;
	int gun_ammount;
	int missile_ammount;
	struct gun_info gun[16];
	struct missile_info missile[16];
	
	int destroy_code;
};

struct con_user_info
{
	int user_id;
	char username[21];
	char guild[21];
	char guild_rank_name[21];
	int guild_rank_id;
	int user_attacking;
	int bot_type_attacking;
	int attacking_bot;
	int attacking_sat;
	int attacking_sat_zone;
	int attacking_sat_sector;
	double attack_time;
	double attack_speed;
	double last_attacked_time;
	double last_missile_time;
	int last_attacker;
	int received_enemy_jump_status_as_full;
	int update_hull;
	int update_missiles;
	int update_guns;
	int sector;
	int zone;
	int last_planet;
	int landed;
	int inbar;
	int destroy_on_recv;
	double jump_time;
	double last_jump_time;
	double last_jump_dock_time;
	int has_galaxy_info;
	int has_goods;
	int activated;
	int has_rune;
	struct user_ship ship;
	int has_bot_guild;
};

//prototypes
//database
extern void read_all_db();
extern void read_server_config();
extern void read_planet_db();
extern void read_galaxy_db();
extern void read_ship_db();
extern void read_bot_db();
extern void read_gun_db();
extern void read_missile_db();
extern void write_all_db();
extern void write_server_config();
extern void write_planet_db();
extern void write_galaxy_db();
extern void write_ship_db();
extern void write_bot_db();
extern void write_gun_db();
extern void write_missile_db();
extern void clear_all_sat_info();
extern void set_sat_locations();
extern void set_sun_damages(struct server_zone_db *the_zone);

//databse_auto
extern void create_planet_db();
extern void create_galaxy_db();
extern void create_ship_db();
extern void create_gun_db();
extern void create_bot_db();
extern void create_missile_db();
extern void set_server_sector_info(int s, int zone, int sector, int planet, int shield, int x, int y, int sec1, int sec2, int sec3, int sec4, int sec5, int range, int is_sun, int is_nebula);
extern void set_server_planet_info(int s, int planet_num, int is_gate, int is_home, char *planet_name, char *guild_name, int home_class, char *bar_name, int has_guild_halls, int is_server_gate);
extern void set_server_gun_info(int s, int weapon_num, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type);
extern void set_server_missile_info(int s, int weapon_num, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type);
extern void set_server_ship_info(int s, int k, int z, int cost, int exp, int hull, int shield, int evasion, int speed, int range, int cargo, int guns, int missiles, int miscs);
extern void set_server_bot_type_info(int s, int i, int dum, char *name, char *guild, int the_class, int aggression, int intelligence, int zone, int field_type);
extern void set_server_bot_info(int s, int i, int k, char *name, int ship_class, int ship_kind, int gun, float def_multi, int exp);

//xinsock
extern void do_sleep();
extern void connect_to_hub();
extern void start_server();
extern void send_con_hub(char *message);
extern void send_con_user(int con, char *message);
extern void raw_send_con_user(int con, char* message);
extern void get_con_user_ip(int con, char *dest);
extern int handle_socket_receive_client(struct client_info *cur_socket, int c_num, void (*process_func)(int, char*));
extern int handle_socket_receive_hub(struct client_info *cur_socket, int c_num, void (*process_func)(int, char*));
extern void close_user_socket(int con);
extern unsigned long get_user_s_addr(int con);

//common
extern void split(char dest[], char message[], char char_spliter, int *initial);
extern int str_match(char *str1, char *str2);
extern int str_match_lcase(char *str1, char *str2);
extern void lcase(char *dest, char *src);
extern void right(char *dest, char *src, int point);
extern void left(char *dest, char *src, int point);
extern void uni_pause(int m_sec);
extern void printd(char *message);
extern int distance(int x1, int y1, int x2, int y2);
extern char *good_name(int good_number, char *dest);
extern int ship_speed_difference(int s_k);
extern double ship_total_seconds(int speed, int s_k);
extern double current_time();
extern double ship_damage_difference(int s_k, double damage);
extern double ship_damage_defense(int s_k, double damage);
extern int pthread_create_always(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
extern void *handle_pthread_create_always(void *start_routine);
extern double random_chance();

//protocol
extern void process_packet_hub(int dummy, char *message);
extern void process_packet_user(int con, char *message);

//hub
extern void connect_hub_process();
extern void handle_user_login(int con, int user_id, char *user_info);
extern void set_user_ship(char *user_name, char *message);
extern void set_user_ship_mod(char *user_name, char *message);
extern void send_user_to_planet(char *user_name, int planet);
extern void set_user_hull(char *user_name, int new_hull);
extern void reload_user_missile(char *user_name, int wep_num, int wep_amt);
extern void set_good_ammounts(int planet_number, char *message);
extern void set_good_prices(int planet_number, char *message);
extern void update_good_ammount(int planet_num, int good_num, int ammount);
extern void update_user_guild(char *user_name, char *new_guild, char *new_rank, int new_rank_id);
extern void update_sector_good(int s, int z, int good_num, int good_amount);
extern void update_sector_contra(int s, int z, int good_num, int good_amount);
extern void send_hub_all_sector_info();
extern void send_hub_sector_goods_info(int s, int z);
extern void send_hub_sector_contras_info(int s, int z);
extern void hub_update_sector_goods(int s, int z, char *message);
extern void hub_update_sector_contras(int s, int z, char *message);
extern int sector_has_contras(int s, int z);
extern int sector_has_goods(int s, int z);
extern void hub_request_sector_info();
extern void hub_add_sector_good(int s, int z, int g, int amount);
extern void hub_add_sector_contra(int s, int z, int g, int amount);
extern void send_hub_user_kill_bot_exp(int user_id, int exp, int class_amt, char *the_guild);
extern void hub_force_user_offline(char *username);
extern void hub_relay_user_exp(int userid, int addition, int exp);
extern void hub_set_sat_info(int p, char *message);
extern void repair_sat(int p, int new_money);
extern void send_user_con_to_planet(int con, int p);
extern void send_hub_sat_hull(int p);
extern void add_sat_to_sector(int p);
extern void hub_set_sat_money_collected(int p, int amount);
extern void hub_set_sat_hull(int p, int amount);
extern void hub_set_sat_money(int p, int amount);
extern void hub_set_sat_missile_left(int p, int w);
extern void hub_set_sat_gun_destroyed(int p, int w);
extern void hub_repair_user_gun(char *user_name, int g);
extern void hub_set_player_has_goods(int userid, int has_goods);
extern void hub_clear_out_planet_trade_disasters();
extern void hub_add_planet_trade_disaster(int p, int g, int is_low, int i);
extern void hub_set_user_activated(int userid, int activated);
extern void send_hub_user_zone_sector(int u);
extern void send_hub_user_kill_bot_random_drop(int user_id, struct bot_drop_db drop_type);
extern void hub_ping_request();
extern void ping_hub();
extern void hub_ping_response();
extern void hub_send_clear_sector_contraband(int z, int s);
extern void hub_send_clear_sector_goods(int z, int s);
extern void clear_sector_contraband_if_needed(int z, int s);
extern void hub_annouce_sat_deposit(int p, char *username, int deposit_amount);

//user
extern void con_user_connect(int con);
extern void user_logoff(int con);
extern void send_user_galaxy_info(int con);
extern void user_enter_bar(int con);
extern void user_leave_bar(int con);
extern void user_chat(int con, char *message);
extern void user_special_chat(int con, int message_type, char *message);
extern void user_relay_ldm(int con, char *to_user, char *message);
extern void move_to_sector(int con, int new_sector);
extern void set_average_gun_speed(int con);
extern void set_missile_time(int con, int missile_number);
extern void start_gun_thread();
extern void attack_user(int con, char *victom);
extern void attack_sat(int con);
extern void disengage_user(int con);
extern int userid_to_con(int userid);
extern int user_to_con(char *username);
extern int user_to_con_miscase(char *username);
extern int planet_to_id(char *planetname);
extern void fire_missile(int con, int missile_number);
extern void scan_user(int con, char *user_name);
extern void set_user_zone_sector(int con);
extern void set_user_shield(int con, int level);
extern void send_good_price_info(int con, int planet_number);
extern void send_good_ammount_info(int con, int planet_number);
extern void send_all_good_price_info(int con);
extern void user_update_good_ammount(int con, int planet_num, int good_num);
extern void send_user_sector_user_war_info(int con);
extern void relay_destroy_user_msg(int s, int z, char *victom_name);
extern void send_user_was_hit(int con, char *enemy_name);
extern void relay_update_good_amount_sector(int s, int z);
extern void user_update_good_amount_sector(int con);
extern void user_update_single_good_amount_sector(int con, int g);
extern void user_update_single_contra_amount_sector(int con, int g);
extern void relay_single_good_amount_sector(int s, int z, int g);
extern void relay_single_contra_amount_sector(int s, int z, int g);
extern void user_pick_up_sector_good(int con, int g, int amount);
extern void user_pick_up_sector_contra(int con, int g, int amount);
extern void user_drop_sector_good(int con, int g, int amount);
extern void user_drop_sector_contra(int con, int g, int amount);
extern void relay_player_blow_up(int s, int z, char *user_name);
extern void relay_user_leave_sector(int con, int s, int z);
extern void user_send_public_chat(int con, int color, char *message);
extern int calculate_gun_accuracy(int con, int accuracy, int type);
extern int calculate_gun_damage(int con, int damage, int type);
extern int calculate_missile_accuracy(int con, int accuracy, int type);
extern int calculate_missile_damage(int con, int damage, int type);
extern void launch_user(int con, int the_zone);
extern void land_user(int con);
extern void relay_public_entry(int con);
extern int users_online();
extern void user_send_sat_info(int con, int p);
extern void relay_planet_sat_info(int p);
extern void relay_user_enter_sector(int z, int s, int sh_id, int sh_t, int sh_k, char *sh_name, char *username, char *userguild, int except_i);
extern void relay_planet_sat_hull(int p);
extern void relay_planet_sat_money(int p);
extern void relay_user_attack_user(int z, int s, char *attacker_name, char *victom_name, int except_i_1, int except_i_2);
extern void sat_attack_user(int p, int u);
extern void sat_attack_bot(int p, int b_t, int b_u);
extern int sat_should_attack_user(int p, int i);
extern int sat_should_attack_bot(int p, int i);
extern void sat_disengage_user(int p, int i);
extern void sat_disengage_bot(int p, int i);
extern void destroy_user(int con, int k_con);
extern void send_user_getting_attacked_from(int con, char *attacker_name);
extern void send_user_hull(int con);
extern void relay_user_disengage(int s, int z, char *username, int except_i);
extern double destroy_gun_chance(double percent_hull_left, int wep_accuracy, int is_missile);
extern void attempt_destroy_gun(int *gun_destroyed_loc, double percent_hull_left, int wep_accuracy, int is_missile);
extern void attempt_destroy_random_sat_gun(int p, int wep_accuracy, int is_missile);
extern void attempt_destroy_random_user_gun(int u, int wep_accuracy, int is_missile);
extern void relay_user_destroy_gun(int u, int g);
extern void send_all_trade_disasters(int con);
extern double sector_damage_reduction(int z, int s);
extern void destroy_user_disconnecting(int con);
extern void user_notify_attackee_of_shield(int u);
extern int calc_gun_acc(int evasion, int shield, int accuracy, int type);
extern int calc_gun_dam(int evasion, int shield, int damage, int type);
extern int calc_missile_acc(int evasion, int shield, int accuracy, int type);
extern int calc_missile_dam(int evasion, int shield, int damage, int type);
extern void notify_attacker_of_victom_shield(int u);
extern void user_dock_jump(int con);
extern void gun_type_effect_dam(int *eva, int *shield, int type);
extern void missile_type_effect_dam(int *eva, int *shield, int type);
extern void gun_type_effect_acc(int *eva, int *shield, int type);
extern void missile_type_effect_acc(int *eva, int *shield, int type);
extern int user_try_disable_shield(int con);
extern int user_shield_is_disabled(int con);
extern void user_notify_shield_disabled(int con);
extern void con_clear_stat(int con);
extern void user_request_goodprices(int con, char *planetname);
extern void set_user_jump_time(int con);
//extern void user_do_shield_time_dock(int con);
extern void user_do_shield_time_dock(int con, double int_time);
extern void gun_thread_user_attack_sat(int i);
extern void sat_try_disable_shield(int p);
extern void set_sat_shield(int p, int level);
extern void sat_notify_attackers_of_shield(int p);
extern int sat_shield_is_disabled(int p);
extern void sat_init_shields();
extern void fix_sat_shields();
extern void notify_attacker_of_enemy_jump(int con);
extern int user_jump_is_charged(int con);
extern void relay_fire_missile(int z, int s, char *attacker_name, int skip_i);
extern void do_pings();
extern void ping_all_users();
extern void ping_user_con(int con);
extern void user_ping_response(int con);
extern void user_set_for_bot_death(int u);
extern void user_do_destroy_on_recv(int u);
extern void user_send_hub_hull(int con);
extern int sector_has_users(int z, int s);
extern void recalculate_sector_free_scan(int z, int s, int entering);
extern void user_send_free_scan_list(int con);
extern int is_bot_guild(char *the_guild);
extern int sector_has_guild_member(int z, int s, char *the_guild, int exclude_con);
extern void annouce_guild_member_enter_sector(int z, int s, char *the_guild, int exclude_con);
extern void annouce_guild_member_leave_sector(int z, int s, char *the_guild, int exclude_con);
extern void user_clear_has_member_sectors(int con);
extern void user_resend_has_member_sectors(int con);

//bot
extern void start_bot_thread();
extern void add_bots_from_sector(char *message, int zone, int sector);
extern void bot_name_to_reference(int *bot_type_num, int *bot_num, char *bot_name);
extern void send_user_sector_bot_war_info(int con);
extern void bot_drop_all_goods(int i, int k);
extern void bot_destroy(int i, int k);
extern void bot_collect_sector_goods(int i, int k);
extern void bot_set_initial_goods(int i, int k);
extern void bot_add_good(int i, int k, int g, int amount);
extern void bot_add_contra(int i, int k, int g, int amount);
extern void bot_clear_all_bot_goods();
extern void clear_bot_lists();
extern void user_add_to_bot_list(int u, int b_t);
extern void bot_user_add_to_any_list(int u, int z, int s);
extern void user_remove_from_all_lists(int u);
extern int bot_should_chase(int i, int k, int t, int u);
extern int bot_has_goods(int i, int k);
extern void start_sun_thread();
extern void bot_check_for_sun_damage(int b_t, int b_u);
extern void set_bot_allies(int b_t);
extern void bot_attack_with_leader(int b_t, int b_u);
extern void bot_set_follow(int b_t, int b_u, int b_u_f);
extern void bot_gather_chasers(int b_t, int b_u);
extern void bot_find_follow(int b_t, int b_u);
extern void bot_stop_following(int b_t, int b_u);
extern void bot_clear_chasing(int b_t, int b_u);
extern void bot_set_random_chasing_max(int b_t, int b_u);
extern void bot_provoke_all_chasers_attack(int b_t, int b_u);
extern void bot_provoke_all_chasers_follow(int b_t, int b_u);
extern void bot_set_shield_level(int b_t, int b_u, int shield_level);
extern void bot_notify_attackers_of_shield(int b_t, int b_u);
extern void bot_dock_jump(int b_t, int b_u);
extern int bot_shield_is_disabled(int b_t, int b_u);
extern void bot_try_disable_shield(int b_t, int b_u);
extern void destroy_sat(int p);
extern struct bot_drop_db bot_drop_item(int b_t, int b_u);
extern int bot_jump_is_charged(int b_t, int b_u);
extern void set_all_smuggler_prices();
extern int get_smuggler_sector_contra_worth(int z, int s, int g);

//memory
extern void clear_sector_goods();
extern void set_default_sat_shields();
extern void reset_all_runes();

//globals
extern struct server_db server_info;
extern struct server_planet_db planet[PLANET_MAX];
extern struct server_zone_db zone[ZONE_MAX];
extern struct server_ship_db ship[SHIP_MAX];
extern struct server_bot_type_db bot_type[BOT_TYPE_MAX];
extern struct server_gun_db gun[GUN_MAX];
extern struct server_missile_db missile[MISSILE_MAX];

extern struct con_user_info user_con[MAX_USER_CON];
extern int user_con_top;

extern double sat_default_gun_evasion;
extern double sat_default_gun_reduction;
extern double sat_default_missile_evasion;
extern double sat_default_missile_reduction;


