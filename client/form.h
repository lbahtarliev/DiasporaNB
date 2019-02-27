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

struct hanger_ship
{
        char name[21]; //server_id that the ship is derived from
	int server_id;
	int type; //ie zeyhper
	int kind; //ie seeker
        int hull;
	int hull_max;
	int cargo;
	int cargo_max;
	int shield;
	int range;
	int speed;
	int has_weapons;
};

struct label
{
	char message[501];
	SDL_Rect location;
};

#define DISPLAYBOX_ENTRY_MAX 50
struct displaybox
{
	struct label label[20];
	char *entry[DISPLAYBOX_ENTRY_MAX];
	char entry_mem[DISPLAYBOX_ENTRY_MAX][501]; //done ultimately for faster entry adding
	int color[DISPLAYBOX_ENTRY_MAX];
	int scroll;
	int entry_max;
	int lines;
	int x;
	int y;
	int w;
};

#define TEXTBOX_SUBMISSION_MAX 10
#define TEXTBOX_TEXT_MAX 400
struct textbox
{
	char message[TEXTBOX_TEXT_MAX];
	char prev_message[TEXTBOX_TEXT_MAX + 5];
	char *past_entry[TEXTBOX_SUBMISSION_MAX];
	char past_message[TEXTBOX_SUBMISSION_MAX][TEXTBOX_TEXT_MAX];
	int entry_max;
	int entry_cursor;
	int cursor;
	int cursor_loc;
	int last_shift;
	int max_char;
	int char_amt;
	char pass_char;
	struct displaybox *display_box_link;
	int selected;
	SDL_Rect location;
};

struct login
{
	SDL_Surface *backround;
	SDL_Rect button[3];
	struct label label;
	struct textbox text_box[2];
	int visible;
};

struct create_char
{
	SDL_Surface *backround;
	SDL_Rect button[2];
	struct label label;
	struct textbox text_box[6];
	int visible;
};

struct leave_game
{
	SDL_Surface *backround;
	int visible;
};

#define SWITCH_SERVER_MAX 200
#define SWITCH_SERVER_PAGE_DISPLAY 7

struct switch_server
{
	SDL_Surface *backround;
	SDL_Rect button[3 + SWITCH_SERVER_PAGE_DISPLAY];
	struct label info_label;
	struct label server_name_label[SWITCH_SERVER_PAGE_DISPLAY];
	struct label server_ip_label[SWITCH_SERVER_PAGE_DISPLAY];
	struct label server_players_label[SWITCH_SERVER_PAGE_DISPLAY];
	struct label server_ping_label[SWITCH_SERVER_PAGE_DISPLAY];
	char server_name[SWITCH_SERVER_MAX][50];
	char server_ip[SWITCH_SERVER_MAX][50];
	int server_players[SWITCH_SERVER_MAX];
	int server_ping[SWITCH_SERVER_MAX];
	int server_selected;
	int server_max;
	int server_page;
	int visible;
};

#define PLANET_MAX_BUTTON 11

struct planet
{
	SDL_Surface *backround;
	SDL_Surface *planetbut;
	SDL_Surface *exit;
	SDL_Rect button[1+PLANET_MAX_BUTTON];
	int button_type[PLANET_MAX_BUTTON]; //Trade 0, Guild Halls 1, Guild Join 2, Sat 3, Launch 4, Server Select 5, Hanger 6, Ship_Plan 7
	int button_zone[PLANET_MAX_BUTTON];
	int last_planet;
	int visible;
};

struct planet_sat
{
	SDL_Surface *backround;
	SDL_Surface *planetbut;
	SDL_Rect button[3];
	struct server_satellite_db sat;
	int show_sat_editor;
	int show_planet_editor;
	int visible;
};

struct sat_editor
{
	SDL_Surface *backround;
	SDL_Surface *empty_pod;
	SDL_Surface *sat_upgrade_but;
	SDL_Surface *sat_bug_but;
	SDL_Surface *sat_purchase_but;
	SDL_Surface *wep_left_but;
	SDL_Surface *wep_right_but;
	SDL_Surface *wep_buy_but;
	SDL_Surface *wep_sell_but;
	SDL_Surface *wep_repair_but;
	SDL_Surface *wep_reload_but;
	SDL_Surface *viewing_guns_img;
	SDL_Surface *viewing_missiles_img;
	SDL_Rect button[1 + (SAT_LEVEL_MAX * SAT_GUN_INC) + 7];
	struct textbox text_box[1];
	struct label money_label;
	struct label current_level_label;
	struct label annual_tax_label;
	struct label taxes_collected_label;
	struct label wep_name_label;
	struct label wep_accuracy_label;
	struct label wep_speed_label;
	struct label wep_damage_label;
	int viewing_guns;
	int viewing_missiles;
	int upgrade_sat;
	int purchase_sat;
	int sel_right;
	int sel_left;
	int sell_wep;
	int buy_wep;
	int reload_wep;
	int repair_wep;
	int weapon_selected;
	int gun_selected;
	int missile_selected;
	int min_gun, max_gun;
	int min_missile, max_missile;
	int visible;
};

struct planet_editor
{
	SDL_Surface *backround;
	SDL_Rect button[1];
	int visible;
};

struct select_server
{
	SDL_Surface *backround;
	SDL_Surface *temp_backround;
	SDL_Surface *circle;
	SDL_Surface *planetbut;
	SDL_Rect button;
	struct label label;
	char servername[15][21];
	char server_ip[15][128];
	int server_id[15];
	int server_x[15];
	int server_y[15];
	int connecting;
	int connected;
	int newlist;
	int visible;
};

struct gen_home
{
	SDL_Surface *backround;
	SDL_Rect button[6];
	int visible;
};

struct velic_home
{
	SDL_Surface *backround;
	SDL_Rect button[6];
	int visible;
};

struct hanger
{
	SDL_Surface *backround;
	SDL_Surface *temp_backround;
	SDL_Surface *selection;
	SDL_Surface *buynew;
	SDL_Surface *takeselected;
	SDL_Surface *no_fship[8][5];
	SDL_Rect button[13];
	struct hanger_ship ship[10];
	int redraw;
	int visible;
};

struct shop
{
	SDL_Surface *backround;
	SDL_Surface *temp_backround;
	SDL_Surface *buy_img;
	SDL_Surface *sell_img;
	SDL_Surface *sel_right;
	SDL_Surface *sel_left;
	SDL_Surface *buy_blank;
	SDL_Surface *sell_blank;
	SDL_Surface *sel_blank;
	SDL_Surface *ship_blank;
	SDL_Surface *empty_pod;
	SDL_Surface *blank_pod;
	SDL_Surface *no_ship[8];
	SDL_Surface *no_gun;
	SDL_Surface *no_missile;
	SDL_Surface *no_misc;
	SDL_Surface *no_misc_pod;
	SDL_Surface *misc_pod[4];
	SDL_Surface *misc_img[4][16]; //0-Hull,1-Shield,2-Cargo,3-Jump
	SDL_Thread *thread;
	SDL_Rect button[27]; //0-exit 1-buy 2-sell 3,4,5,6-left/right 7,8,9,10-shop_sel 11-26-pod
	struct label user_money_label;
	struct label buy_money_label;
	struct label sell_money_label;
	struct label info_left_label[10]; //info, ie "hull"
	struct label stat_left_label[10]; //stat, ie "200"
	struct label stat_right_label[10];
	struct label info_right_label[10];
	struct label sel_left_label[2];
	struct label sel_right_label[2];
	struct displaybox left_display_box;
	struct displaybox right_display_box;
	int shop_sel;
	int ship_kind_sel; //ie seeker
	int ship_type_sel; //ie zeyhper
	int gun_sel;
	int gun_pod_sel;
	int missile_sel;
	int missile_pod_sel;
	int misc_sel;
	int misc_pod_sel;

	int rotate; //purchase image rotate index
	int rotate_sell;
	//int rotate_draw_buy;
	int attempt_load_buy;
	int attempt_load_sell;
	int last_planet;
	int visible;
};

#define MAX_BAR_ENTRY 24

struct bar
{
	SDL_Surface *backround;
	SDL_Surface *nif_bar;
	SDL_Surface *but1_clear;
	SDL_Surface *but1_transfer;
	SDL_Surface *but1_enlist;
	SDL_Surface *but1_promote;
	SDL_Surface *but2_clear;
	SDL_Surface *but2_remove;
	SDL_Surface *but2_demote;
	SDL_Surface *but2_guild;
	SDL_Surface *but3_clear;
	SDL_Surface *but3_cancel;
	SDL_Surface *but3_chat;
	SDL_Rect button[4 + MAX_BAR_ENTRY];
	struct label label;
	struct label player_label[MAX_BAR_ENTRY];
	struct textbox text_box[3];
	struct displaybox display_box[2];
	char playername[MAX_BAR_ENTRY][21];
	char playerguild[MAX_BAR_ENTRY][21];
	char playerguild_rank[MAX_BAR_ENTRY][21];
	int playerguild_rank_id[MAX_BAR_ENTRY];
	int selected_i;
	int selection_fresh;
	int selection_remove;
	int selection_demote;
	int selection_enlist;
	int selection_transfer;
	int last_planet;
	int visible;
};

struct repair
{
	SDL_Surface *backround;
	SDL_Surface *repair_ship_img;
	SDL_Surface *repair_wep_img;
	SDL_Surface *reload_wep_img;
	SDL_Thread *thread;
	SDL_Rect button[34]; //0= leave, 1= ship rep, 2-17= gun rep, 18-33= missiles restock
	struct label money_label;
	struct label cost_label;
	int redraw_weapons;
	int last_planet;
	int visible;
};

struct launching
{
	SDL_Surface *backround;
	int visible;
};

struct landing
{
	SDL_Surface *backround;
	int visible;
};

struct space_user
{
	char username[21];
	char guild[21];
	char ship_name[21];
	int ship_id;
	int ship_kind; //zeyhper
	int ship_type; //seeker
	int destroy;
	double destroy_time;
	int attacking;
	double attack_time;
	int attack_side;
	int attack_position;
	int attack_x;
	int attack_y;
	int hull_percent;
	int shield_level;
	int refresh_shield;
	int jump_status;
	int missile_firing[16];
	double missile_time[16];
	double missile_speed[16];
	int missile_x[16];
	int missile_y[16];
	int sos;
	int image;
};

struct space
{
	SDL_Surface *backround;
	SDL_Surface *loc;
	SDL_Surface *loc_sun;
	SDL_Surface *temp_loc;
	SDL_Surface *viewing_ship;
	SDL_Surface *viewing_sector;
	SDL_Surface *viewing_guns;
	SDL_Surface *viewing_missiles;
	SDL_Surface *viewing_goods_space;
	SDL_Surface *viewing_goods_ship;
	SDL_Surface *viewing_contras_space;
	SDL_Surface *viewing_contras_ship;
	SDL_Surface *jump_img;
	SDL_Surface *shield_off[5];
	SDL_Surface *shield_on[5];
	SDL_Surface *trade_goods_img;
	SDL_Surface *trade_goods_grey_img;
	SDL_Surface *contraband_img;
	SDL_Surface *contraband_grey_img;
	SDL_Surface *trade_img;
	SDL_Surface *cancel_img;
	SDL_Surface *engage_img;
	SDL_Surface *disengage_img;
	SDL_Surface *chat_img;
	SDL_Surface *ship_nif_img[8];
	SDL_Surface *destroy_img[52];
	SDL_Thread *thread;
	SDL_Rect button[79]; //0-39=users; 40=44=shield; 45-47=top row; 48-50=bottom row; 51=jump; 52-Weapon Viewed; 53-68-gun/missile; 69-78 Good Select
	struct textbox text_box[4];
	struct displaybox display_box[2];
	struct label guild_label;
	struct label user_label;
	struct label info_label;
	struct label shield_label;
	struct label hull_label;
	struct label good_label[10];
	struct space_user user[40];
	double timer;
	double thread_start_time;
	int info_label_mode;
	int motion_sel;
	int user_sel;
	int user_ammount;
	int last_loc_zone;
	int last_loc_safe;
	int last_loc_planet;
	int last_loc_number;
	int jump_ok;
	double jump_time;
	double jump_time_start;
	double shield_disable_time;
	double form_load_time;
	int clear_jump_bar;
	int selection_box_i;
	int view_sector_goods;
	int show_ship;
	int shield_level;
	int at_planet;
	int transferring;
	int engaging;
	int redraw_loc;
	int hit_enemy;
	int hit_max;
	int hit_actual_max;
	int was_sun;
	int weapon_viewing; //0=guns; 1=missiles
	int view_cargo_goods;
	int view_cargo_sector;
	int viewing_cargo;
	int good_viewing;
	int missile_firing[16];
	int missile_x[16];
	int missile_y[16];
	int draw_goods_grey;
	int draw_contra_grey;
	double missile_time[16];
	double missile_speed[16];
	int visible;
};

struct map
{
	SDL_Surface *backround;
	SDL_Surface *backround_cutout;
	SDL_Surface *center_button;
	SDL_Surface *sun_img;
	SDL_Surface *nebula_img;
	SDL_Surface *scan_large;
	SDL_Surface *scan_small;
	SDL_Surface *scan_tiny;
	SDL_Surface *scan_up;
	SDL_Surface *scan_down;
	SDL_Surface *scan_left;
	SDL_Surface *scan_right;
	SDL_Surface *scan_up_left;
	SDL_Surface *scan_up_right;
	SDL_Surface *scan_down_left;
	SDL_Surface *scan_down_right;
	SDL_Surface *e_mark;
	SDL_Surface *g_shield;
	SDL_Rect button[8]; //0-4 = jump locations, 5 = cancel, 6=+,7=-
	SDL_Rect 	scan_location;
	SDL_Surface *	scan_last;
	int 		scan_sector;
	struct label 	scan_label;
	struct textbox text_box;
	int sector_link[5];
	int sector_link_x[5];
	int sector_link_y[5];
	int sector_x;
	int sector_y;
	int shift_x;
	int shift_y;
	int do_shift;
	int zoom_level;
	int visible;
};

struct death
{
	SDL_Surface *backround;
	int okay;
	int visible;
};

struct planet_guild
{
	SDL_Surface *backround;
	SDL_Rect button[2];
	struct label guild_name_label;
	struct label entry_cost_label;
	struct label money_label;
	struct label your_guild_name_label;
	int visible;
};

struct trade
{
	SDL_Surface *backround;
	SDL_Surface *nif_trade;
	SDL_Surface *large_good[10];
	SDL_Surface *goto_news_but;
	SDL_Surface *goto_trade_but;
	SDL_Surface *sell_all_but;
	SDL_Rect button[13];
	struct textbox text_box[2];
	struct label money_label;
	struct label cargo_label;
	struct label trade_label[3];
	struct label best_trade_planet_name_label[PLANET_TRADE_MAX];
	struct label best_trade_suggested_label[PLANET_TRADE_MAX];
	struct displaybox trade_display[3];
	struct displaybox news_disaster_display;
	int good_selected;
	int info_visible;
	int viewing_news;
	int last_planet;
	int visible;
};

struct guild_halls
{
	SDL_Surface *backround;
	SDL_Rect button[5];
	int visible;
};

struct guild_editor
{
	SDL_Surface *backround;
	SDL_Rect button[2 + 30 + 1];
	struct textbox text_box[2 + 6];
    char website[201];
	int bank_money;
	int bank_lend;
	char rank_name[6][21];
	int rank_allow[6][5];
    int visible;
};

struct guild_editor_more
{
	SDL_Surface *backround;
	SDL_Rect button[6];
	struct textbox text_box[4];
	char guild_enemy[3][21];
	SDL_Surface *on[2];
	SDL_Surface *sel_left, *sel_right;
	SDL_Surface *save_settings;
	int deco_choosen;
	int deco_activated;
	int visible;
};

#define MAX_GUILD_LISTING 46
#define HALF_MAX_GUILD_LISTING MAX_GUILD_LISTING / 2

struct guild_listing
{
	SDL_Surface *backround;
	SDL_Surface *next_on, *next_off;
	SDL_Surface *prev_on, *prev_off;
	SDL_Rect button[6 + MAX_GUILD_LISTING]; // exit, sort, prev/next,
	char guild_name[MAX_GUILD][21];
	int guild_worth[MAX_GUILD];
	int guild_players[MAX_GUILD];
	int max_entry;
	int sort_method;
	int sort_list[3][MAX_GUILD];
	int list_start;
	struct textbox text_box;
	struct label guild_name_label;
	struct label guild_owner_label;
	struct label guild_website_label;
	struct label guild_money_label;
	struct label guild_lend_label;
	struct label guild_players_label;
	struct label guild_enemy_label[3];
	struct label guild_rank_label[6];
	struct label guild_listing_label[MAX_GUILD_LISTING];
	char selected_guild_name[21];
	char owner_name[21];
	char website[201];
	int bank_money;
	int bank_lend;
	char rank_name[6][21];
	int rank_allow[6][5];
	char guild_enemy[3][21];
	int players;
	int visible;
};

struct guild_bank
{
	SDL_Surface *backround;
	SDL_Rect button[3];
	struct label money_label;
	struct label balance_label;
	struct textbox text_box;
	int visible;
};

struct guild_register
{
	SDL_Surface *backround;
	SDL_Rect button[1];
	struct label info_label;
	struct textbox text_box;
	int visible;
};

struct splash_screen
{
	SDL_Surface *backround;
	int visible;
};

struct stat_viewer
{
	SDL_Surface *backround;
	SDL_Surface *class_img[2];
	SDL_Surface *pointer_img;
	SDL_Surface *sel_left, *sel_right;
	SDL_Thread *thread;
	SDL_Rect button[16]; //0-3 stuff, 4-13 list selects, 14<-deco->15,
	struct label list_name_label[10];
	struct label list_attr_label[10];
	struct label player_name_label;
	struct label guild_name_label;
	struct label guild_rank_label;
	struct label player_worth_label;
	struct label player_exp_label;
	struct label player_kills_label;
	struct label player_home_planet_label;
	struct label player_home_server_label;
	struct label player_last_server_label;
	struct label player_online_label;
	struct label player_actvated_label;
	struct label deco_label;
	struct textbox text_box;
	int player_worth, player_exp, player_kills;
	int player_online, player_activated;
	int player_ship_id, player_ship_type, player_ship_kind;
	int player_class_points;
	char deco_name[MAX_DECO][31];
	int deco_i[MAX_DECO];
	int deco_max;
	int load_ship, s_id, s_type, s_kind;
	int deco_chosen;
	char s_name[21];
	int list_class[10];
	int visible;
};

struct bounty
{
	SDL_Surface *backround;
	SDL_Surface *bounty_text_img;
	SDL_Rect button[2];
	struct label name_list_label[38];
	struct label price_list_label[38];
	struct label money_label;
	struct label status_label;
	struct textbox text_box[40]; //38=name 39=price
	int list_max;
	int visible;
};

struct cb_info
{
	SDL_Surface *backround;
	SDL_Rect button[3];
	int visible;
};

struct purchase_plans
{
	SDL_Surface *backround;
	struct label cost_label;
	struct label exp_label;
	struct label kills_label;
	SDL_Rect button[2];
	int visible;
};

//Memory
extern struct login login;
extern struct create_char create_char;
extern struct leave_game leave_game;
extern struct select_server server_select;
extern struct planet planet;
extern struct gen_home gen_home;
extern struct velic_home velic_home;
extern struct hanger hanger;
extern struct shop shop;
extern struct bar bar;
extern struct repair repair;
extern struct launching launching;
extern struct landing landing;
extern struct space space;
extern struct map map;
extern struct death death;
extern struct trade trade;
extern struct guild_halls guild_halls;
extern struct guild_editor guild_editor;
extern struct guild_editor_more guild_editor_more;
extern struct guild_listing guild_listing;
extern struct guild_bank guild_bank;
extern struct guild_register guild_register;
extern struct splash_screen splash_screen;
extern struct stat_viewer stat_viewer;
extern struct cb_info cb_info;
extern struct bounty bounty;
extern struct planet_sat planet_sat;
extern struct sat_editor sat_editor;
extern struct planet_editor planet_editor;
extern struct planet_guild planet_guild;
extern struct switch_server switch_server;
extern struct purchase_plans purchase_plans;


