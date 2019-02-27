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

//what kind
#define IS_CLIENT 1
//#define USE_JABBER 1 //enables code used specifically for the nighsoft.net servers
#define USE_LIBCURL 1

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
//#include <fcntl.h>
//#include <errno.h>

#ifdef _WIN32
	#include <windows.h>		//win for sleep(1000)
#else
	#include <sys/types.h>		//lin
	#include <sys/socket.h>		//lin
	#include <netinet/in.h>		//lin
	#include <netdb.h>		//lin
	#include <sys/param.h>		//lin
	#include <sys/ioctl.h>		//lin
#endif

#ifdef USE_JABBER

#define JABBER_MAX_USERS 500

#include <gloox/client.h>
#include <gloox/messagehandler.h>
#include <gloox/presencehandler.h>
#include <gloox/rostermanager.h>
#include <gloox/connectionlistener.h>
#include <gloox/rosterlistener.h>
#include <gloox/registration.h>
#include <gloox/registrationhandler.h>

using namespace gloox;

extern char jabber_name[JABBER_MAX_USERS][500];
extern int jabber_max;

#endif

#ifdef USE_LIBCURL
#include <curl/curl.h>
#endif

//defines
#define VERSION 1

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
#define MAX_PRIVATE_CHAT_USERS 20
#define JUMP_DOCK_TIME 0.2
#define MAX_IGNORE 100
#define MAX_SOS 100

//deactivated user stuff
#define DEACTIVATED_RANGE_LIMIT 5
#define DEACTIVATED_EXP_LIMIT 800
#define DEACTIVATED_PRICE_LIMIT 8000000

#include "database_common.h"
#include "form.h"

//structs
struct sound_sample 
{
	SDL_AudioSpec sound_info;
	Uint8 *data;
	Uint32 dpos;
	Uint32 dlen;
	int repeat;
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

#define EXP_RANK_IMG_MAX 14
struct game_images
{
	SDL_Surface *mini_good[10];
	SDL_Surface *mini_small_good[10];
	SDL_Surface *mini_contra[10];
	SDL_Surface *exp_rank[EXP_RANK_IMG_MAX];
	SDL_Surface *guild_toggle_off;
	SDL_Surface *guild_toggle_on;
	SDL_Surface *fship_sat;
	SDL_Surface *fship_sat_destroyed;
	SDL_Surface *hyper_s_grey;
	SDL_Surface *deco_img[MAX_DECO];
};

struct private_chat
{
    char username[21];
    char status;     
};

struct game_memory
{
	char username[21];
	char hub_address[51];
	char server_address[51];
	char password[21];
	char last_ldm[21];
	int money;
	int exp;
	int kills;
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
	int engine_on;
	int activated;
	int has_plan[MAX_SERVER][SHIP_MAX];
	struct guild_db guild;
	struct user_ship_db ship[10];
	struct nif_images nif_image;
	struct game_images game_image;
	struct private_chat private_chat_entry[MAX_PRIVATE_CHAT_USERS];
	//int last_planet is refered to as planet.last_planet
};

//jabber
extern void jabber_connect(char *server, char *username, char *password);
extern void jabber_send(char *username, char *message);
extern char *to_jabber_name(char *dest, char *message);


//sdl
extern int init_sdl();
extern void toggle_windowed();
//extern void handle_events();
extern int handle_sdl_events();
extern void sdl_flip_mutex();
extern void set_pixel(SDL_Surface *screan, int x, int y, Uint8 r, Uint8 g, Uint8 b);
extern void draw_label(SDL_Surface *screan, char *message, struct label *label, int r, int g, int b);
extern void draw_right_label(SDL_Surface *screan, char *message, struct label *label, int r, int g, int b);
extern void draw_center_label(SDL_Surface *screan, char *message, struct label *label, int r, int g, int b);
extern void draw_raw_label(SDL_Surface *screan, char *message, SDL_Rect location, int r, int g, int b);
extern void draw_raw_right_label(SDL_Surface *screan, char *message, SDL_Rect location, int r, int g, int b);
extern void draw_raw_center_label(SDL_Surface *screan, char *message, SDL_Rect location, int r, int g, int b);
extern void draw_text_box(SDL_Surface *screan, char *message, struct textbox *text_box, int r, int g, int b);
extern void clear_text_box(SDL_Surface *screan, struct textbox *text_box);
extern void add_text_box(SDL_Surface *screan, struct textbox *text_box, int c);
extern void select_text_box(SDL_Surface *screan, struct textbox *text_box);
extern void set_text_box_text(SDL_Surface *screan, struct textbox *text_box, char *message);
extern void unselect_text_box(SDL_Surface *screan, struct textbox *text_box);
extern int use_previous_text_box_entry(struct textbox *text_box);
extern int use_next_text_box_entry(struct textbox *text_box);
extern void add_text_box_entry(struct textbox *text_box, char *message);
extern void setup_text_box(struct textbox *text_box);
extern void set_text_box_displaybox(struct textbox *text_box, struct displaybox *display_box);
extern void clear_display_box(SDL_Surface *screan, struct displaybox *display_box);
extern void add_display_box(SDL_Surface *screan, struct displaybox *display_box, int text_color, char *message);
extern void draw_display_box(SDL_Surface *screan, struct displaybox *display_box);
extern void scroll_down_display_box(SDL_Surface *screan, struct displaybox *display_box);
extern void scroll_up_display_box(SDL_Surface *screan, struct displaybox *display_box);
extern void setup_display_box(struct displaybox *display_box);
extern int init_audio();
extern int make_sound(char *filename);
extern void play_sound(int sound_num);
extern void mixaudio(void *unused, Uint8 *stream, int len);
extern void play_sound_repeat(int sound_num);
extern void stop_sound_repeat(int sound_num);
extern SDL_Surface *add_red_glow(SDL_Surface *the_img);
extern SDL_Color get_pixel(SDL_Surface *the_img, int x, int y);

//form
extern void load_splash_screen();
extern void load_login();
extern void load_server_select();
extern void load_planet(int planet_number);
extern void load_gen_home();
extern void load_velic_home();
extern void load_space();
extern void load_death();
extern void handle_button(int x, int y);
extern void handle_button_up(int x, int y);
extern void handle_button_right(int x, int y);
extern void handle_motion(int x, int y);
extern void handle_key(int c);
extern void handle_ctrl_key(int c);
extern void init_form_memory();
extern void check_draw_login();
extern void redraw_visible();
extern void change_shop(int new_shop);
extern void clear_sell_shop();
extern void view_sell_shop();
extern void update_shop_sell(int mod_type, int slot_number);
extern void redraw_hanger();
extern void read_bar_info(char *bar_info);
extern void add_bar_entry(char *username, char *userguild, char *user_guild_rank, int user_guild_rank_id);
extern void remove_bar_entry(char *username);
extern void redraw_space_loc();

//Xinsock
extern void init_sockets();
extern void handle_sockets();
extern void threadless_hub_connect(char *address);
extern void threadless_server_connect(char *address);
//extern void connect_to_hub();
//extern void connect_to_server(char *host_address);
extern void send_con_hub(char *message);
extern void send_con_server(char *message);
extern void close_con_server();
extern void close_con_hub();

//Common
extern void split(char dest[], char message[], char char_spliter, int *initial);
extern int str_match(char *str1, char *str2);
extern void lcase(char *dest, char *src);
extern void right(char *dest, char *src, int point);
extern void left(char *dest, char *src, int point);
extern char *ship_name(int ship_number, char *dest);
extern char *misc_pod_name(int misc_pod_number, char *dest);
extern int misc_pod_value(int misc_pod_number);
extern int misc_pod_exp(int misc_pod_number);
extern int misc_pod_cost(int misc_pod_number);
extern int convert_key_sym(int key, int caps);
extern void printd(char *message);
extern int distance(int x1, int y1, int x2, int y2);
extern int ship_speed_difference(int s_k);
extern double ship_total_seconds(int speed, int s_k);
extern double current_time();
extern double random_chance();
extern void clean_newline(char *message, int size);

//Protocol
extern void process_packet_hub(char *message);
extern void process_packet_server(char *message);

//Hub
extern void connect_hub_process();
extern void disconnect_hub_process();
extern void write_login_message(char *message);
extern void load_server_list(char *list);
extern void read_full_server_list(char *list);
extern void set_ship_info(char *message);
extern void set_cargo_info(int sh, char *message);
extern void set_ship_mod(char *message);
extern void read_hanger_info(char *message);
extern void read_user_info(char *message);
extern void set_user_money(int money);
extern void purchase_ship(int ship_type, int ship_kind);
extern void purchase_ship_mod(int mod_type, int mod_number);
extern void sell_ship(int ship_selected);
extern void sell_ship_mod(int mod_type, int mod_selected);
extern void update_hanger_info(char *message);
extern void select_ship(int new_selection);
extern void fix_gun(int gun_number);
extern void restock_missile(int missile_number, int payload);
extern void update_ship_good_ammount(int sh, int g, int ammount);
extern void update_ship_contra_ammount(int sh, int g, int ammount);
extern void update_guild_info(char *message);
extern void set_guild_balance(int ammount);
extern void add_guild_listing(int i, char *entry_name, int guild_worth, int guild_players);
extern void display_guild_info(char *message);
extern void set_guild_money(int ammount);
extern void update_user_exp(int exp);
extern void hub_set_is_activated(int it_is);
extern void hub_set_guild_deco(int d_i, char *deco_name, int deco_activated);
extern void hub_send_ask_pricechange();
extern void hub_send_utoggle();
extern void hub_send_dsamount();
extern void hub_send_transferds(char *playername, int amount);
extern void hub_send_buyactivation(char *playername);
extern void hub_send_buyexp(char *playername, int amount);
extern void hub_send_buydc(char *playername, int amount);
extern void hub_ping_request();
extern void hub_process_plan_list(char *the_list);
extern void hub_send_lease_login(char *lease_login, char *lease_password);

//Server
extern void connect_server_process();
extern void disconnect_server_process();
extern void read_planet_entry(char *cur_line);
extern void read_galaxy_entry(char *cur_line);
extern void read_gun_entry(int server_num, char *cur_line);
extern void read_missile_entry(int server_num, char *cur_line);
extern void read_ship_entry(char *cur_line);
extern void set_good_ammounts(int server_number, int planet_number, char *message);
extern void set_good_prices(int server_number, int planet_number, char *message);
extern void update_planet_good_ammount(int s, int p, int i, int ammount);
extern void set_sector_goods(int s, int z, char *message);
extern void set_sector_contras(int s, int z, char *message);
extern void set_sector_good(int s, int z, int g, int amount);
extern void set_sector_contra(int s, int z, int g, int amount);
extern void server_set_sat_info(char *message);
extern void server_set_sat_hull(int amount);
extern void server_set_sat_money(int amount);
extern void server_set_gun_destroyed(int g);
extern void server_add_planet_trade_disaster(int p, int g, int is_low, int i);
extern void server_clear_out_planet_trade_disasters();
extern void set_sun_damages(struct server_zone_db *the_zone);
extern void set_victom_shield_level(int shield_level);
extern void dock_jump_bar();
extern void server_send_ask_goodprices(char *planet_name);
extern void set_victom_jump_status(int jump_status);
extern void server_ping_request();
extern void server_set_sector_has_player(int z, int s, int has_player);
extern void server_set_sector_has_guild_member(int z, int s, int has_guild_member);
extern void server_clear_has_guild_member_sectors();

//Game
extern void space_threadless();
extern void clear_user_misc(int i);
extern void set_user_misc();
extern void add_chat(int text_color, char *message);
extern void add_private_chat(int text_color, char *message);
extern void load_sector(int zone, int sector, char *user_list);
extern void add_to_sector(char *new_user);
extern void remove_from_sector(char *gone_user);
extern void setup_private_comms(struct displaybox *display_box);
extern void setup_public_comms(struct displaybox *display_box);
extern void set_shield(int new_level);
extern void space_select_user(int user_selected);
extern void space_engage_user();
extern void space_disengage_user();
extern void space_draw_weapons();
extern void space_draw_gun_lines(int x, int y);
extern void space_draw_line(int position, int side, int x, int y);
extern void space_draw_line_to_user(int user_number);
extern void space_fire_missile(int missile_number);
extern void space_draw_missile(int missile_number, double ratio);
extern void set_average_gun_speed();
extern void set_missile_speed_times();
extern void transfer_money_to(char *user_name, int amount);
extern void destroy_user(char *victom);
extern void set_hull(int new_hull);
extern void set_enemy_hull(int new_percent, int guns_hit);
extern void set_user_engage(char *username, char *victom);
extern void set_user_engage_us(char *username);
extern void set_user_disengage(char *username);
extern void clear_space_user_ship(int i);
extern void repair_draw_weapons();
extern int load_gun_pod_img(int i);
extern int load_missile_pod_img(int i);
extern int load_fship(int server_id, int ship_type, int ship_kind, int ship_face, char *the_ship_name);
extern int load_map_planet(int server_id, int planet_id);
extern void map_draw_scan();
extern void map_set_scan(int new_sector, char *message);
extern void send_public_chat(char *message);
extern void trade_draw_news();
extern void trade_draw_shop();
extern void trade_update_good_ammount();
extern void trade_update_player_good_ammount();
extern void trade_draw_selection(int new_selection);
extern void load_good_images();
extern char *contra_name(int contra_number, char *dest);
extern char *good_name(int good_number, char *dest);
extern char *good_mini_name(int good_number, char *dest);
extern int ship_total_good_ammount(int ship_number);
extern void guildlisting_display(char *guild_name, char *website, char *owner, int bank_money, int bank_lend, char **rank_name, int **rank_allow);
extern void guildlisting_add_entry(char *guild_name);
extern void guildlisting_clear_entries();
extern void load_sounds();
extern void space_user_hit_us(char *their_name);
extern void space_draw_good_box(int viewing_goods, int viewing_sector);
extern void space_draw_good_number(int viewing_goods, int viewing_sector, int g);
extern void space_select_good(int g);
extern void space_drop_good(int is_good, int g, int amount);
extern void space_pickup_good(int is_good, int g, int amount);
extern void space_clear_middle_box();
extern void space_toggle_ship_sector();
extern void space_draw_good_box_if_suposed_to();
extern int space_redraw_good_number_if_required(int g, int is_sector_change);
extern int space_good_select_animate();
extern void space_clear_good(int g);
extern void space_do_drop_good(int amount);
extern void space_do_show_ship();
extern void space_do_show_ship();
extern void space_draw_gun_destroyed(int g);
extern void space_set_hit_actual_max();
extern void stat_viewer_display_list();
extern void stat_viewer_clear_ship_area();
extern void hub_retrive_stats_list(char *message);
extern void hub_retrive_user_stats(char *message);
extern void bounty_draw_raw_lists();
extern void bounty_load_list(char *message);
extern void bounty_request();
extern void bounty_add(char *victom_name, int price);
extern void bounty_clear_all_text_boxes();
extern void guild_editor_more_reset_text_boxes();
extern void guild_editor_reset_text_boxes();
extern void guild_editor_reset_toggles();
extern void guild_editor_toggle_button(int i);
extern void guild_editor_reset_variables();
extern void guild_editor_send_save();
extern void guild_editor_more_send_save();
extern void guild_editor_more_draw_deco(int deco_num);
extern void guild_editor_more_next_deco();
extern void guild_editor_more_previous_deco();
extern void guild_editor_more_toggle_deco();
extern int guild_editor_more_deco_changes();
extern void guild_editor_more_send_deco_changes();
extern void guild_editor_more_draw_save_changes();
extern void guild_editor_draw_deco_onoff();
extern int load_deco(int deco_num);
extern void guild_listing_sort_name();
extern void guild_listing_sort_worth();
extern void guild_listing_sort_players();
extern void guild_listing_previous();
extern void guild_listing_next();
extern void guild_listing_draw_guild();
extern void guild_listing_draw_list();
extern void guild_listing_request_list();
extern void guild_listing_reset_variables();
extern void guild_listing_select_guild(int i);
extern void guild_listing_draw_previous_next();
extern void guild_listing_select_new_guild(char *new_name);
extern void bar_select_player(int i);
extern void bar_clear_selection_type();
extern void bar_draw_name_tag();
extern void bar_draw_buttons();
extern void bar_unselect();
extern void bar_select();
extern void bar_button_choice(int b);
extern void bar_correct_guild_select();
extern int bar_transfer_box_visible();
extern void bar_clear_selection();
extern void guild_enlist_user(char *username);
extern void guild_remove_user(char *username);
extern void guild_promote_user(char *username);
extern void guild_demote_user(char *username);
extern void guild_user_accept(char *guildname);
extern void guild_chat_send(char *message);
extern void guild_list_ask();
extern void guild_online_list_ask();
extern int check_for_command(char *message);
extern void process_command(char *command, char *message);
extern SDL_Surface *special_fship(int ship_num);
extern int space_user_is_special(int u_s);
extern void sat_editor_redraw_entire_screen();
extern void sat_reset_textbox();
extern void sat_draw_money_label();
extern void sat_draw_hull();
extern void sat_draw_weapon_icons();
extern void sat_draw_purchase_or_upgrade_button();
extern void sat_draw_weapon_selected();
extern void sat_undraw_weapon_selected();
extern void sat_set_default_variables();
extern int sat_is_ours();
extern int sat_is_owned();
extern void sat_draw_viewing_guns_or_missiles();
extern void sat_toggle_weapons_viewing();
extern void sat_make_weapon_select(int i);
extern void sat_send_upgrade_purchase();
extern void sat_choose_left();
extern void sat_choose_right();
extern void sat_send_buy_sell();
extern void sat_send_reload_repair();
extern void sat_draw_sell_buy_img();
extern void sat_draw_repair_reload_img();
extern void sat_draw_info_labels();
extern void sat_draw_weapon_stats();
extern void sat_set_weapon_variables();
extern int sat_show_right_but();
extern int sat_show_left_but();
extern void sat_draw_left_right_buttons();
extern void sat_send_deposit(int amount);
extern void sat_send_bug_sat();
extern int load_missile_pod_img_specific(int s, int w);
extern int load_gun_pod_img_specific(int s, int w);
extern void send_hub_enter_planet_guild();
extern void trade_draw_news_or_shop();
extern void trade_draw_news();
extern void trade_clear_space();
extern void trade_toggle_news_and_shop();
extern void trade_display_selected_good_amounts();
extern void check_to_load_server_sun_damages();
extern void clear_ship_info(int sh);
extern void map_draw_sector_images();
extern int map_check_sector_on_map(int x, int y);
extern void map_draw_local_sectors();
extern void map_redraw_side_hud();
extern void map_draw_nebula_and_sun();
extern void map_draw_planet_names();
extern void map_zoom_in();
extern void map_zoom_out();
extern void map_redraw();
extern void map_set_local_sector_info();
extern void map_set_shift(int x, int y, int button_down);
extern void space_select_user_to_scan(int u);
extern void space_auto_do_good(int g);
extern void trade_sell_all_goods();
extern void trade_draw_sell_all_button();
extern int ship_has_weapons(int sh);
extern double ship_equipment_worth(int sh);
extern void clear_space_user_struct(struct space_user *the_user);
extern void space_draw_enemy_shield();
extern void private_chat_send(char *message);
extern void private_chat_add(char *username);
extern void private_chat_remove(char *username);
extern void private_chat_list();
extern void private_chat_online_list();
extern void clear_private_chat_entries();
extern void private_chat_add_edit_entry(char *username, int status);
extern void private_chat_remove_entry(char *username);
extern void private_chat_load_entry_list(char *message);
extern void do_chat_button(char *username);
extern void set_shop_type_up();
extern void set_shop_type_down();
extern void set_shop_kind_up();
extern void set_shop_kind_down();
extern void shop_display_buy_ship_stats();
extern void shop_display_buy_gun_stats();
extern void shop_display_buy_missile_stats();
extern void shop_display_buy_misc();
extern void shop_display_sell_ship();
extern void shop_display_sell_gun();
extern void shop_display_sell_missile();
extern void shop_display_sell_misc();
extern void shop_display_sell_ship_stat_names();
extern void shop_display_sell_gun_stat_names();
extern void shop_display_sell_missile_stat_names();
extern void shop_display_sell_misc_stat_names();
extern void shop_display_buy_ship_stat_names();
extern void shop_display_buy_gun_stat_names();
extern void shop_display_buy_missile_stat_names();
extern void shop_display_buy_misc_stat_names();
extern void shop_display_sell_guns();
extern void shop_draw_sell_item_selected(int i);
extern void shop_draw_sell_item_unselected(int i);
extern void shop_blank_current_item_sell();
extern void shop_select_next_sell_item();
extern void disable_shields();
extern int shields_are_disabled();
extern void draw_our_shield(int old_level);
extern int exp_rank(int exp);
extern int load_exp_rank_img(int i);
extern void draw_exp_rank_img(int exp, int x, int y);
extern void space_chat_button();
extern int help_command(char *message);
extern void change_password(char *old_password, char *new_password);
extern void short_stat_user(char *username);
extern void guild_award_deco(char *player_name, char *deco_name);
extern void guild_remove_deco(char *player_name, char *deco_name);
extern void guild_list_deco(char *player_name);
extern void guild_list_decog(char *guild_name);
extern void stat_viewer_left_deco();
extern void stat_viewer_right_deco();
extern void stat_viewer_draw_deco(int deco_num);
extern int stat_viewer_is_left_deco();
extern int stat_viewer_is_right_deco();
extern void stat_viewer_draw_deco_img();
extern void guild_list_bugs();
extern void send_create_char();
extern void set_user_jump_time();
extern void remove_ldm(struct textbox *our_box);
extern void insert_ldm(struct textbox *our_box, char *message);
extern struct textbox *our_ldm_func();
extern void ctrl_l();
extern void ctrl_u();
extern void ctrl_c();
extern void ctrl_g();
extern void ctrl_v();
extern void ctrl_w();
extern void load_sector_loc(int s_id, int z, int s);
extern void load_sector_locs_safe(int s_id, int z);
extern void load_sector_locs(int s_id, int z);
extern void draw_sector_loc(int s_id, int z, int s, SDL_Surface *the_loc);
extern void switch_server_populate();
extern void switch_server_insert_array(char *array);
extern void switch_server_display_page();
extern void switch_server_left_button();
extern void switch_server_right_button();
extern void switch_server_select_server(int i);
extern void switch_server_leave_button();
extern void switch_server_ping();
extern int switch_server_ping_thread(void *nothing);
extern void add_ignore(char *name);
extern void remove_ignore(char *name);
extern void list_ignore();
extern int check_ignore_normal(int color, char *message);
extern int check_ignore_private(int color, char *message);
extern void save_ignore();
extern void load_ignore();
extern void clear_ilist();
extern int add_ignore_raw(char *name);
extern int check_ignore_raw(char *name);
extern void space_draw_enemy_jump_status();
extern int add_sos(char *name);
extern int remove_sos(char *name);
extern void list_sos();
extern void save_sos();
extern void load_sos();
extern void clear_soslist();
extern int add_sos_raw(char *name);
extern int check_sos_raw(char *name);
extern void update_space_user_sos(char *name, int is_sos);
extern void draw_space_user_ship(int i);
extern int load_fship_sos(int s, int s_t, int s_k, int i, char *the_ship_name);
extern void show_enemy_missile(char *name);
extern void space_draw_enemy_missile(int i, int missile_number, double ratio);
extern void draw_space_good_buttons();
extern int update_space_good_buttons();
extern void draw_purchase_plans();
extern void clear_plan_list();
extern int purchase_plans_should_show_buy_button(int s, int p, int pl);
extern void do_purchase_plan();
extern int next_shop_type_down();
extern int next_shop_type_up();
extern int next_shop_kind_down();
extern int next_shop_kind_up();
extern void shop_show_buy_button();
extern void shop_show_type_left_right();
extern void shop_show_kind_left_right();
extern int shop_ship_available_for_purchase(int sh_t, int sh_k);
extern void shop_choose_available_ship_kind();
extern int shop_can_be_set_to(int shop_type);
extern void shop_set_initial_selections();
extern int at_home_planet();

//Database
extern void read_host_info();
extern void write_host_info();
extern void read_config_info();
extern void write_config_info();

//memory
extern void set_trade_disaster_texts();
extern void add_trade_disaster_text(int g, int lowers_price, char *message);
extern char *get_trade_disaster_text(int g, int lowers_price, int i);

//web_access
extern int fetch_url(char *url, char *dest, int size);

//globals
//sdl
extern SDL_Surface *screen;
extern TTF_Font *ttf_font;
extern struct sound_sample game_sound[MAX_SOUND];
extern int sound_max;
//extern SDL_mutex* space_mutex;
extern SDL_mutex* flip_mutex;

//game
extern struct game_memory game;
extern struct server_db server[MAX_SERVER];

//ignore list
extern char *ignore_list[MAX_IGNORE];
extern int ignore_list_max;

//sos list
extern char *sos_list[MAX_IGNORE];
extern int sos_list_max;

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
