#include "main.h"

void create_planet_db()
{
	int k,z,i;
	
	server_info.home_class = 1;
	
	//clear this
	for(k=0;k<PLANET_MAX;k++)
		for(i=0;i<PLANET_TRADE_MAX;i++)
			planet[k].trade_planet[i] = -1;
	
	//int server_num, int planet_num, int is_gate, int is_home, char *planet_name, char *guild_name, int home_class, char *bar_name, int has_guild_halls, int is_server_gate);
	set_server_planet_info(i,0,0,1,"Gen Grayl","Gen Crusader",1,"The Dark Bar",1,0);
	set_server_planet_info(i,1,0,1,"Gen Irium","",1,"The Crystal Bar",1,0);
	set_server_planet_info(i,2,0,1,"Gen Pelein","",1,"The Guild Bar",1,0);
	//set_server_planet_info(i,3,0,0,"Azathio Zeta","Trader",0,"Conference Room",1,1);
	//set_server_planet_info(i,4,0,0,"Xerosis","Militia",0,"The Back Room",0,0);
	//set_server_planet_info(i,5,0,0,"Enktopis","Miner",0,"The Lounge",0,0);
	set_server_planet_info(i,6,1,0,"Gen Gate","",0,"The Gate Bar",0,0);
	set_server_planet_info(i,7,1,0,"Velic Gate","",0,"The Gate Bar",0,0);
	//set_server_planet_info(i,8,0,0,"Seira","",0,"The Loft",0,0);
	//set_server_planet_info(i,9,0,0,"Reynek","",0,"The Chop Shop",0,0);
	set_server_planet_info(i,10,0,0,"Jen Cando","Rin",0,"The Cutthroat",0,0);
	set_server_planet_info(i,11,1,0,"Gen Gate","",0,"The Gate Bar",0,0);
	//set_server_planet_info(i,12,0,0,"Lonis","",0,"The Railway",0,0);
	set_server_planet_info(i,13,0,0,"Laius Metis","",0,"The Chambers",0,1);
	set_server_planet_info(i,14,0,0,"Seejen","",0,"Friendly's",0,0);
	set_server_planet_info(i,15,0,0,"Tyros","",0,"The Smoke Shop",0,0);
	set_server_planet_info(i,16,0,0,"Jen Kayle","",0,"The Rin HQ",0,0);
	set_server_planet_info(i,17,0,0,"Crontas","Miner",0,"Resort Pub",0,0);
	set_server_planet_info(i,18,0,0,"Hy Syng","",0,"The Factory",0,0);
	set_server_planet_info(i,19,0,1,"Velic Carden","",2,"The Back Alley",1,0);
	set_server_planet_info(i,20,0,1,"Velic Grindow","",2,"Babydolls",1,0);
	set_server_planet_info(i,21,0,1,"Velic Vel","",2,"The Commune",1,0);
	set_server_planet_info(i,22,0,0,"Daynow","",0,"The Farm",0,0);
	set_server_planet_info(i,23,0,0,"Naykon","",0,"The Air Lock",0,0);
	//set_server_planet_info(i,24,0,0,"Endystation","",0,"The Deck",0,0);
	//set_server_planet_info(i,25,0,0,"Dysilica","",0,"The Hut",0,0);
	set_server_planet_info(i,26,0,0,"Kaydarin","",0,"Marble Palace",0,0);
	set_server_planet_info(i,27,1,0,"Neutral Gate","",0,"The Gate Bar",0,0);
	//set_server_planet_info(i,28,1,0,"Banished Gate","",0,"The Gate Bar",0,0);
	//set_server_planet_info(i,29,1,0,"Banished Gate","",0,"The Gate Bar",0,0);
	//set_server_planet_info(i,30,0,0,"Helliathin","",0,"Hanger Pub",0,0);
	//set_server_planet_info(i,31,0,0,"Dezba","",0,"War Room",1,0);
	set_server_planet_info(i,32,0,0,"Renos Brin","Kaydin",0,"Street Darbar",0,0);
	set_server_planet_info(i,33,1,0,"Velic Gate","",0,"The Gate Bar",0,0);
	//set_server_planet_info(i,34,0,0,"Adeona","",0,"The Show House",0,0);
	set_server_planet_info(i,35,1,0,"Gen Gate","",0,"The Gate Bar",0,0);
	set_server_planet_info(i,36,1,0,"Velic Gate","",0,"The Gate Bar",0,0);
    
    //trade planets
	k=0; //Gen Grayl
	planet[k].trade_planet[0] = 1;
	planet[k].trade_planet[1] = 2;
	k=1; //Gen Irium
	planet[k].trade_planet[0] = 0;
	planet[k].trade_planet[1] = 2;
	k=2; //Gen Pelein
	planet[k].trade_planet[0] = 0;
	planet[k].trade_planet[1] = 1;
	k=3; //Azathio Zeta
	planet[k].trade_planet[0] = 4;
	planet[k].trade_planet[1] = 5;
	k=4; //Xerosis
	planet[k].trade_planet[0] = 3;
	planet[k].trade_planet[1] = 10;
	k=5; //Enktopis
	planet[k].trade_planet[0] = 3;
	k=8; //Seira
	planet[k].trade_planet[0] = 9;
	k=9; //Reynek
	planet[k].trade_planet[0] = 8;
	k=10; //Jen Cando
	planet[k].trade_planet[0] = 14;
	planet[k].trade_planet[1] = 16;
	planet[k].trade_planet[2] = 22;
	k=13; //Laius Metis
	planet[k].trade_planet[0] = 14;
	planet[k].trade_planet[1] = 15;
	k=14; //Seejen
	planet[k].trade_planet[0] = 13;
	planet[k].trade_planet[1] = 15;
	planet[k].trade_planet[2] = 16;
	planet[k].trade_planet[3] = 10;
	k=15; //Tyros
	planet[k].trade_planet[0] = 13;
	planet[k].trade_planet[1] = 14;
	planet[k].trade_planet[2] = 17;
	planet[k].trade_planet[3] = 18;
	k=16; //Jen Kayle
	planet[k].trade_planet[0] = 14;
	planet[k].trade_planet[1] = 22;
	k=17; //Crontas
	planet[k].trade_planet[0] = 15;
	planet[k].trade_planet[1] = 18;
	planet[k].trade_planet[2] = 23;
	k=18; //Hy Syng
	planet[k].trade_planet[0] = 13;
	planet[k].trade_planet[1] = 17;
	planet[k].trade_planet[2] = 23;
	k=19; //Velic Carden
	planet[k].trade_planet[0] = 20;
	planet[k].trade_planet[1] = 21;
	k=20; //Velic Grindow
	planet[k].trade_planet[0] = 19;
	planet[k].trade_planet[1] = 21;
	k=21; //Velic Vel
	planet[k].trade_planet[0] = 19;
	planet[k].trade_planet[1] = 20;
	k=22; //Daynow
	planet[k].trade_planet[0] = 16;
	planet[k].trade_planet[1] = 10;
	planet[k].trade_planet[2] = 32;
	planet[k].trade_planet[3] = 26;
	k=23; //Naykon
	planet[k].trade_planet[0] = 17;
	planet[k].trade_planet[1] = 18;
	planet[k].trade_planet[2] = 26;
	planet[k].trade_planet[3] = 32;
	k=24; //Endystation
	planet[k].trade_planet[0] = 25;
	k=25; //Dysilica
	planet[k].trade_planet[0] = 24;
	k=26; //Kaydarin
	planet[k].trade_planet[0] = 32;
	planet[k].trade_planet[1] = 22;
	k=30; //Helliathin
	planet[k].trade_planet[0] = 31;
	k=31; //Dezba
	planet[k].trade_planet[0] = 30;
	planet[k].trade_planet[1] = 32;
	k=32; //Renos Brin
	planet[k].trade_planet[0] = 23;
	planet[k].trade_planet[1] = 26;
	planet[k].trade_planet[2] = 22;

	k=0;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=1;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=2;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=19;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=20;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=21;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=3;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[2].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].gun[6] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	planet[k].missile[2] = 1;
	
	k=4;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[2].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].gun[6] = 1;
	planet[k].gun[7] = 1;
	planet[k].gun[8] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	planet[k].missile[2] = 1;
	planet[k].missile[3] = 1;
	
	k=5;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[2].have = 1;
	planet[k].ship[3].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].gun[6] = 1;
	planet[k].gun[7] = 1;
	planet[k].gun[8] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	planet[k].missile[2] = 1;
	planet[k].missile[3] = 1;
	
	k=8;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=9;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	
	k=12;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[2].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	planet[k].missile[2] = 1;
	
	k=10;
	planet[k].ship[0].have = 1;
	planet[k].ship[1].have = 1;
	planet[k].ship[2].have = 1;
	planet[k].ship[3].have = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 4;
	planet[k].gun[0] = 1;
	planet[k].gun[1] = 1;
	planet[k].gun[2] = 1;
	planet[k].gun[3] = 1;
	planet[k].gun[4] = 1;
	planet[k].gun[5] = 1;
	planet[k].gun[6] = 1;
	planet[k].gun[7] = 1;
	planet[k].gun[8] = 1;
	planet[k].gun[9] = 1;
	planet[k].gun[10] = 1;
	planet[k].missile[0] = 1;
	planet[k].missile[1] = 1;
	planet[k].missile[2] = 1;
	planet[k].missile[3] = 1;
	planet[k].missile[4] = 1;
	
	k=13;
	for(z=0;z<=2;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=8;z++) planet[k].gun[z] = 1;
	for(z=0;z<=2;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	
	k=14;
	for(z=0;z<=2;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=8;z++) planet[k].gun[z] = 1;
	for(z=0;z<=2;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	
	k=15;
	for(z=0;z<=2;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=8;z++) planet[k].gun[z] = 1;
	for(z=0;z<=2;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	
	k=16;
	for(z=0;z<=3;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=12;z++) planet[k].gun[z] = 1;
	for(z=0;z<=3;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	
	k=17; //Crontas
	for(z=0;z<=4;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=12;z++) planet[k].gun[z] = 1;
	for(z=0;z<=3;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 4;
	planet[k].ship[4].max = 4;
	
	k=18; //Hy Syng
	for(z=0;z<=4;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=13;z++) planet[k].gun[z] = 1;
	for(z=0;z<=4;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	
	k=22; //Daynow
	for(z=0;z<=5;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=15;z++) planet[k].gun[z] = 1;
	for(z=0;z<=5;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 4;
	
	k=23; //Naykon
	for(z=0;z<=5;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=16;z++) planet[k].gun[z] = 1;
	for(z=0;z<=7;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 7;
	
	k=24; //Endystation
	for(z=0;z<=5;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=15;z++) planet[k].gun[z] = 1;
	for(z=0;z<=5;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 4;
	
	k=25; //Dysilica
	for(z=0;z<=5;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=15;z++) planet[k].gun[z] = 1;
	for(z=0;z<=6;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 7;
	
	k=26; //kaydarin
	for(z=0;z<=6;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=18;z++) planet[k].gun[z] = 1;
	for(z=0;z<=9;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 7;
	planet[k].ship[6].max = 7;
	
	k=30; // helliathin
	for(z=0;z<=6;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=17;z++) planet[k].gun[z] = 1;
	for(z=0;z<=8;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 7;
	planet[k].ship[6].max = 3;
	
	k=31;
	for(z=0;z<=6;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=18;z++) planet[k].gun[z] = 1;
	for(z=0;z<=9;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 7;
	planet[k].ship[6].max = 6;
	
	k=32; //renos brin
	for(z=0;z<=6;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=17;z++) planet[k].gun[z] = 1;
	for(z=0;z<=8;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;
	planet[k].ship[2].max = 7;
	planet[k].ship[3].max = 7;
	planet[k].ship[4].max = 7;
	planet[k].ship[5].max = 7;
	planet[k].ship[6].max = 6;
	
	k=34; //adeona
	for(z=0;z<=1;z++) planet[k].ship[z].have = 1;
	for(z=0;z<=5;z++) planet[k].gun[z] = 1;
	for(z=0;z<=2;z++) planet[k].missile[z] = 1;
	planet[k].ship[0].max = 7;
	planet[k].ship[1].max = 7;

	//gen zone
	for(k=0;k<3;k++)
		for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 120;
		planet[k].good_price_low[z] = 30;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//velic zone
	for(k=19;k<=21;k++)
		for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 120;
		planet[k].good_price_low[z] = 30;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//13 Lauis Metis
	k=13;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 160;
		planet[k].good_price_low[z] = 50;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Seejen
	k=14;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 160;
		planet[k].good_price_low[z] = 50;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Tyros
	k=15;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 160;
		planet[k].good_price_low[z] = 50;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Jen Cando
	k=10;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 180;
		planet[k].good_price_low[z] = 70;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//16 Jen Kayle
	k=16;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 180;
		planet[k].good_price_low[z] = 70;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//17 Crontas
	k=17;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 180;
		planet[k].good_price_low[z] = 70;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Hy Syng
	k=18;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 200;
		planet[k].good_price_low[z] = 90;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Daynow
	k=22;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 220;
		planet[k].good_price_low[z] = 120;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Naykon
	k=23;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 260;
		planet[k].good_price_low[z] = 140;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Renos Brin
	k=32;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 300;
		planet[k].good_price_low[z] = 160;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}
	
	//Kaydarin
	k=26;
	for(z=0;z<10;z++)
	{
		planet[k].good_price_high[z] = 400;
		planet[k].good_price_low[z] = 180;
		planet[k].contra_price_high[z] = 2200;
		planet[k].contra_price_low[z] = 2000;
		
		planet[k].good_ammount_high[z] = 5000;
		planet[k].good_ammount_low[z] = 500;
		planet[k].contra_ammount_high[z] = 300;
		planet[k].contra_ammount_low[z] = 100;
	}

	printf("database created- purpose:planet\n");
}

void create_galaxy_db()
{
	int i = 0;
	int k;
	
	//Gen zone
	k=0;
	strcpy(zone[k].name,"Gen");
	zone[k].home_class = 1;
	/* //one simple as hell gen zone
	set_server_sector_info(i,0,0,0,1,-136,9,2,1,-1,-1,-1,0);
	set_server_sector_info(i,0,1,-1,0,-94,59,0,3,6,-1,-1,0);
	set_server_sector_info(i,0,2,-1,0,-45,-25,0,3,4,-1,-1,0);
	set_server_sector_info(i,0,3,6,0,-38,30,2,5,1,1,-1,0);
	set_server_sector_info(i,0,4,2,1,49,-22,2,5,-1,-1,-1,0);
	set_server_sector_info(i,0,5,-1,0,34,43,3,6,4,-1,-1,0);
	set_server_sector_info(i,0,6,1,1,-16,114,1,5,-1,-1,-1,0);
	*/
	set_server_sector_info(i,0,0,1,1,-66,-19,5,7,25,-1,-1,0,0,0);
	set_server_sector_info(i,0,1,0,1,-247,123,6,13,32,-1,-1,0,0,0);
	set_server_sector_info(i,0,2,2,1,228,10,11,20,26,29,-1,0,0,0);
	set_server_sector_info(i,0,3,6,1,-32,127,14,22,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,4,-1,1,-153,49,5,6,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,5,-1,1,-107,12,0,4,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,6,-1,1,-201,87,1,4,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,7,-1,1,-14,-13,0,8,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,8,-1,1,36,-8,7,9,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,9,-1,1,86,-2,8,10,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,10,-1,1,138,2,9,11,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,11,-1,1,181,5,2,10,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,12,-1,1,-139,126,13,14,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,13,-1,1,-198,124,1,12,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,14,-1,1,-84,129,3,12,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,15,-1,0,54,255,21,16,57,58,-1,0,0,0);
	set_server_sector_info(i,0,16,-1,1,84,215,15,17,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,17,-1,1,113,172,16,18,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,18,-1,1,142,132,17,19,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,19,-1,1,170,93,18,20,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,20,-1,1,198,52,19,2,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,21,-1,1,21,212,15,22,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,22,-1,1,-7,168,3,21,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,23,-1,0,-119,-155,24,43,42,71,72,0,0,0);
	set_server_sector_info(i,0,24,-1,1,-103,-113,25,23,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,25,-1,1,-85,-62,0,24,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,26,-1,1,256,-29,2,27,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,27,-1,1,286,-71,28,26,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,28,-1,0,315,-112,27,38,65,64,-1,0,0,0);
	set_server_sector_info(i,0,29,-1,1,276,1,2,30,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,30,-1,1,328,-8,31,29,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,31,-1,0,379,-17,30,64,90,63,91,0,0,0);
	set_server_sector_info(i,0,32,-1,1,-261,72,1,33,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,33,-1,1,-272,33,32,34,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,34,-1,1,-285,-8,33,35,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,35,-1,1,-318,-38,34,36,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,36,-1,0,-353,-69,35,46,75,45,-1,0,0,0);
	set_server_sector_info(i,0,37,-1,0,244,208,60,84,59,-1,-1,0,0,0);
	set_server_sector_info(i,0,38,-1,0,266,-127,39,66,28,-1,-1,0,0,0);
	set_server_sector_info(i,0,39,-1,0,198,-120,40,67,38,-1,-1,0,0,0);
	set_server_sector_info(i,0,40,-1,0,136,-145,41,68,39,-1,-1,0,0,0);
	set_server_sector_info(i,0,41,-1,0,53,-163,42,69,40,-1,-1,0,0,0);
	set_server_sector_info(i,0,42,-1,0,-18,-159,23,70,41,-1,-1,0,0,0);
	set_server_sector_info(i,0,43,-1,0,-178,-129,44,23,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,44,-1,0,-237,-97,45,43,73,-1,-1,0,0,0);
	set_server_sector_info(i,0,45,-1,0,-301,-85,36,44,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,46,-1,0,-385,-30,47,76,36,-1,-1,0,0,0);
	set_server_sector_info(i,0,47,-1,0,-415,18,48,78,46,-1,-1,0,0,0);
	set_server_sector_info(i,0,48,-1,0,-439,74,49,79,47,-1,-1,0,0,0);
	set_server_sector_info(i,0,49,-1,0,-450,134,48,80,50,-1,-1,0,0,0);
	set_server_sector_info(i,0,50,-1,0,-414,182,51,49,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,51,-1,0,-372,226,52,82,50,81,-1,0,0,0);
	set_server_sector_info(i,0,52,-1,0,-296,228,53,51,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,53,-1,0,-239,250,54,52,82,-1,-1,0,0,0);
	set_server_sector_info(i,0,54,-1,0,-176,247,55,83,53,-1,-1,0,0,0);
	set_server_sector_info(i,0,55,-1,0,-124,236,54,56,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,56,-1,0,-86,283,57,55,83,-1,-1,0,0,0);
	set_server_sector_info(i,0,57,-1,0,-16,271,15,56,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,58,-1,0,122,242,59,15,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,59,-1,0,182,226,37,84,58,-1,-1,0,0,0);
	set_server_sector_info(i,0,60,-1,0,306,216,61,86,85,37,-1,0,0,0);
	set_server_sector_info(i,0,61,-1,0,364,179,62,87,60,-1,-1,0,0,0);
	set_server_sector_info(i,0,62,-1,0,356,118,63,88,61,-1,-1,0,0,0);
	set_server_sector_info(i,0,63,-1,0,363,53,31,89,62,-1,-1,0,0,0);
	set_server_sector_info(i,0,64,-1,0,380,-94,28,94,91,31,-1,0,0,0);
	set_server_sector_info(i,0,65,-1,0,359,-148,66,28,94,-1,-1,0,0,0);
	set_server_sector_info(i,0,66,-1,0,297,-165,67,38,65,-1,-1,0,0,0);
	set_server_sector_info(i,0,67,-1,0,229,-180,68,66,39,-1,-1,0,0,0);
	set_server_sector_info(i,0,68,-1,0,151,-187,69,40,67,-1,-1,0,0,0);
	set_server_sector_info(i,0,69,-1,0,74,-199,70,41,68,-1,-1,0,0,0);
	set_server_sector_info(i,0,70,-1,0,2,-207,71,42,69,-1,-1,0,0,0);
	set_server_sector_info(i,0,71,-1,0,-73,-196,23,72,70,-1,-1,0,0,0);
	set_server_sector_info(i,0,72,-1,0,-158,-197,73,23,71,-1,-1,0,0,0);
	set_server_sector_info(i,0,73,-1,0,-226,-160,44,74,72,-1,-1,0,0,0);
	set_server_sector_info(i,0,74,-1,0,-297,-154,75,73,146,-1,-1,0,0,0);
	set_server_sector_info(i,0,75,-1,0,-358,-119,76,36,77,74,-1,0,0,0);
	set_server_sector_info(i,0,76,-1,0,-410,-75,78,46,75,-1,-1,0,0,0);
	set_server_sector_info(i,0,77,-1,0,-405,-155,75,146,151,-1,-1,0,0,0);
	set_server_sector_info(i,0,78,-1,0,-450,-20,79,47,76,-1,-1,0,0,0);
	set_server_sector_info(i,0,79,-1,0,-473,26,48,80,78,-1,-1,0,0,0);
	set_server_sector_info(i,0,80,-1,0,-489,96,49,81,79,-1,-1,0,0,0);
	set_server_sector_info(i,0,81,-1,0,-466,186,51,80,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,82,-1,0,-297,282,83,53,51,-1,-1,0,0,0);
	set_server_sector_info(i,0,83,-1,0,-195,309,56,54,82,-1,-1,0,0,0);
	set_server_sector_info(i,0,84,-1,0,251,263,85,37,59,-1,-1,0,0,0);
	set_server_sector_info(i,0,85,-1,0,320,270,86,60,84,-1,-1,0,0,0);
	set_server_sector_info(i,0,86,-1,0,375,250,87,60,85,-1,-1,0,0,0);
	set_server_sector_info(i,0,87,-1,0,416,205,88,61,86,-1,-1,0,0,0);
	set_server_sector_info(i,0,88,-1,0,401,124,89,62,87,-1,-1,0,0,0);
	set_server_sector_info(i,0,89,-1,0,407,62,90,63,88,-1,-1,0,0,0);
	set_server_sector_info(i,0,90,-1,0,427,2,91,31,89,-1,-1,0,0,0);
	set_server_sector_info(i,0,91,-1,0,453,-51,64,93,90,31,-1,0,0,0);
	set_server_sector_info(i,0,92,-1,0,553,-121,100,139,97,93,-1,0,0,0);
	set_server_sector_info(i,0,93,-1,0,481,-95,94,92,91,-1,-1,0,0,0);
	set_server_sector_info(i,0,94,-1,0,436,-128,65,64,93,97,-1,0,0,0);
	set_server_sector_info(i,0,95,-1,0,914,-349,116,130,118,-1,-1,0,0,0);
	set_server_sector_info(i,0,96,8,1,709,-249,103,-1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,97,-1,0,507,-152,94,92,98,-1,-1,0,0,0);
	set_server_sector_info(i,0,98,-1,0,544,-191,97,100,99,113,-1,0,0,0);
	set_server_sector_info(i,0,99,-1,0,558,-250,98,113,107,-1,-1,0,0,0);
	set_server_sector_info(i,0,100,-1,0,597,-181,92,98,101,-1,-1,0,0,0);
	set_server_sector_info(i,0,101,-1,0,644,-192,100,139,102,-1,-1,0,0,0);
	set_server_sector_info(i,0,102,-1,0,687,-203,101,140,103,-1,-1,0,0,0);
	set_server_sector_info(i,0,103,-1,0,730,-213,102,96,104,112,-1,0,0,0);
	set_server_sector_info(i,0,104,-1,0,764,-249,103,105,112,-1,-1,0,0,0);
	set_server_sector_info(i,0,105,-1,0,793,-282,104,106,114,-1,-1,0,0,0);
	set_server_sector_info(i,0,106,-1,0,823,-312,117,115,105,-1,-1,0,0,0);
	set_server_sector_info(i,0,107,-1,0,579,-301,99,108,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,108,-1,0,623,-339,107,109,127,-1,-1,0,0,0);
	set_server_sector_info(i,0,109,-1,0,683,-355,108,128,110,-1,-1,0,0,0);
	set_server_sector_info(i,0,110,-1,0,761,-358,129,109,117,-1,-1,0,0,0);
	set_server_sector_info(i,0,111,9,1,889,-253,115,-1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,112,-1,0,793,-214,141,103,104,114,125,0,0,1);
	set_server_sector_info(i,0,113,-1,0,519,-256,99,98,126,-1,-1,0,0,0);
	set_server_sector_info(i,0,114,-1,0,826,-250,115,105,112,-1,-1,0,0,0);
	set_server_sector_info(i,0,115,-1,0,856,-284,111,116,106,114,-1,0,0,0);
	set_server_sector_info(i,0,116,-1,0,884,-317,117,115,95,-1,-1,0,0,0);
	set_server_sector_info(i,0,117,-1,0,840,-345,110,130,116,106,-1,0,0,0);
	set_server_sector_info(i,0,118,-1,0,976,-361,95,131,119,-1,-1,0,0,0);
	set_server_sector_info(i,0,119,-1,0,1026,-339,118,132,133,120,-1,0,1,0);
	set_server_sector_info(i,0,120,-1,0,1049,-283,119,134,121,-1,-1,0,0,0);
	set_server_sector_info(i,0,121,-1,0,1033,-228,120,135,122,-1,-1,0,0,0);
	set_server_sector_info(i,0,122,-1,0,1002,-186,121,136,123,-1,-1,0,0,0);
	set_server_sector_info(i,0,123,-1,0,944,-152,122,137,124,-1,-1,0,0,0);
	set_server_sector_info(i,0,124,-1,0,882,-156,123,138,125,-1,-1,0,0,0);
	set_server_sector_info(i,0,125,-1,0,834,-181,124,112,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,126,-1,0,541,-308,113,127,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,127,-1,0,590,-355,126,108,128,-1,-1,0,0,0);
	set_server_sector_info(i,0,128,-1,0,652,-376,127,129,109,-1,-1,0,0,0);
	set_server_sector_info(i,0,129,-1,0,725,-380,128,110,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,130,-1,0,884,-380,117,95,131,-1,-1,0,0,0);
	set_server_sector_info(i,0,131,-1,0,963,-401,130,118,132,-1,-1,0,0,0);
	set_server_sector_info(i,0,132,-1,0,1052,-392,131,119,133,-1,-1,0,0,0);
	set_server_sector_info(i,0,133,-1,0,1088,-336,119,132,134,-1,-1,0,0,0);
	set_server_sector_info(i,0,134,-1,0,1100,-278,133,120,135,-1,-1,0,0,0);
	set_server_sector_info(i,0,135,-1,0,1084,-208,134,121,136,-1,-1,0,0,0);
	set_server_sector_info(i,0,136,-1,0,1037,-137,135,122,137,-1,-1,0,0,0);
	set_server_sector_info(i,0,137,-1,0,970,-103,136,123,138,-1,-1,0,0,0);
	set_server_sector_info(i,0,138,-1,0,892,-106,137,124,142,-1,-1,0,0,0);
	set_server_sector_info(i,0,139,-1,0,623,-157,92,101,140,-1,-1,0,0,0);
	set_server_sector_info(i,0,140,-1,0,708,-170,139,102,141,-1,-1,0,0,0);
	set_server_sector_info(i,0,141,-1,0,779,-157,140,112,125,142,143,0,0,0);
	set_server_sector_info(i,0,142,-1,0,832,-117,141,138,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,143,-1,0,761,-117,141,144,-1,-1,-1,1,0,0);
	set_server_sector_info(i,0,144,-1,0,746,-82,143,145,-1,-1,-1,1,0,0);
	set_server_sector_info(i,0,145,11,1,732,-48,144,-1,-1,-1,-1,1,0,0);
	set_server_sector_info(i,0,146,-1,0,-353,-177,74,147,77,-1,-1,0,0,0);
	set_server_sector_info(i,0,147,-1,0,-399,-201,146,151,148,-1,-1,0,0,0);
	set_server_sector_info(i,0,148,-1,0,-426,-243,154,149,147,152,-1,0,0,0);
	set_server_sector_info(i,0,149,-1,0,-410,-310,157,148,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,150,-1,0,-460,-328,157,160,154,-1,-1,0,0,0);
	set_server_sector_info(i,0,151,-1,0,-438,-192,147,77,152,-1,-1,0,0,0);
	set_server_sector_info(i,0,152,-1,0,-464,-237,151,148,154,153,-1,0,0,0);
	set_server_sector_info(i,0,153,-1,0,-514,-244,152,155,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,154,-1,0,-468,-282,150,159,152,148,-1,0,0,0);
	set_server_sector_info(i,0,155,-1,0,-568,-312,153,159,156,-1,-1,0,0,0);
	set_server_sector_info(i,0,156,-1,0,-611,-302,155,166,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,157,-1,0,-411,-366,161,158,149,150,-1,0,0,0);
	set_server_sector_info(i,0,158,-1,0,-388,-418,162,157,192,-1,-1,0,0,0);
	set_server_sector_info(i,0,159,-1,0,-518,-323,160,154,155,-1,-1,0,0,0);
	set_server_sector_info(i,0,160,-1,0,-484,-363,161,150,159,-1,-1,0,0,0);
	set_server_sector_info(i,0,161,-1,0,-452,-404,162,157,160,-1,-1,0,0,0);
	set_server_sector_info(i,0,162,-1,0,-446,-454,170,163,161,158,-1,0,0,0);
	set_server_sector_info(i,0,163,-1,0,-439,-499,162,191,192,230,-1,0,0,0);
	set_server_sector_info(i,0,164,-1,0,-702,-294,166,165,193,-1,-1,0,0,0);
	set_server_sector_info(i,0,165,-1,0,-743,-295,164,173,193,-1,-1,0,0,0);
	set_server_sector_info(i,0,166,-1,0,-657,-291,167,156,164,193,-1,0,0,0);
	set_server_sector_info(i,0,167,-1,1,-654,-337,168,166,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,168,-1,1,-649,-390,169,167,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,169,12,1,-646,-437,168,172,190,-1,-1,0,0,0);
	set_server_sector_info(i,0,170,-1,1,-492,-449,171,162,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,171,-1,1,-539,-444,172,170,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,172,-1,1,-590,-439,169,171,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,173,-1,0,-783,-297,165,179,196,-1,-1,0,0,0);
	set_server_sector_info(i,0,174,-1,0,-800,-480,175,181,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,175,-1,0,-823,-454,176,174,204,-1,-1,0,0,0);
	set_server_sector_info(i,0,176,-1,0,-843,-430,177,175,203,-1,-1,0,0,0);
	set_server_sector_info(i,0,177,-1,0,-829,-396,178,176,200,-1,-1,0,0,0);
	set_server_sector_info(i,0,178,-1,0,-812,-359,179,177,196,200,-1,0,0,0);
	set_server_sector_info(i,0,179,-1,0,-797,-327,173,178,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,180,-1,0,-744,-529,181,182,210,-1,-1,0,0,0);
	set_server_sector_info(i,0,181,-1,0,-780,-505,189,174,180,209,-1,0,0,0);
	set_server_sector_info(i,0,182,-1,0,-710,-548,180,183,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,183,-1,0,-682,-564,182,184,216,-1,-1,0,0,0);
	set_server_sector_info(i,0,184,-1,0,-649,-584,183,185,218,-1,-1,0,0,0);
	set_server_sector_info(i,0,185,-1,0,-617,-600,184,186,224,-1,-1,0,0,0);
	set_server_sector_info(i,0,186,-1,0,-582,-578,185,187,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,187,-1,0,-552,-560,186,188,232,-1,-1,0,0,0);
	set_server_sector_info(i,0,188,-1,0,-517,-542,187,191,230,-1,-1,0,0,0);
	set_server_sector_info(i,0,189,-1,1,-739,-485,190,181,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,190,-1,1,-695,-463,169,189,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,191,-1,0,-480,-520,188,163,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,192,-1,0,-399,-471,163,158,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,193,-1,0,-714,-263,194,166,164,165,-1,0,0,0);
	set_server_sector_info(i,0,194,-1,0,-761,-261,197,193,195,-1,-1,0,0,0);
	set_server_sector_info(i,0,195,-1,0,-828,-249,194,197,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,196,-1,0,-828,-310,197,173,178,198,-1,0,0,0);
	set_server_sector_info(i,0,197,-1,0,-859,-289,199,196,194,195,-1,0,0,0);
	set_server_sector_info(i,0,198,-1,0,-877,-333,196,200,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,199,-1,0,-920,-334,202,197,201,-1,-1,0,0,0);
	set_server_sector_info(i,0,200,-1,0,-848,-372,198,178,177,201,-1,0,0,0);
	set_server_sector_info(i,0,201,-1,0,-898,-382,199,200,203,-1,-1,0,0,0);
	set_server_sector_info(i,0,202,-1,0,-955,-400,206,199,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,203,-1,0,-894,-441,206,176,201,207,204,0,0,0);
	set_server_sector_info(i,0,204,-1,0,-854,-503,203,208,209,175,207,0,0,0);
	set_server_sector_info(i,0,205,-1,0,-950,-489,212,206,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,206,-1,0,-946,-448,205,203,202,-1,-1,0,0,0);
	set_server_sector_info(i,0,207,-1,0,-906,-499,212,203,204,-1,-1,0,0,0);
	set_server_sector_info(i,0,208,-1,0,-862,-543,212,204,209,-1,-1,0,0,0);
	set_server_sector_info(i,0,209,-1,0,-807,-530,204,181,210,208,211,0,0,0);
	set_server_sector_info(i,0,210,-1,0,-777,-573,209,180,216,215,217,0,0,0);
	set_server_sector_info(i,0,211,-1,0,-827,-592,214,209,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,212,-1,0,-913,-549,213,205,207,208,-1,0,0,0);
	set_server_sector_info(i,0,213,-1,0,-887,-591,214,212,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,214,-1,0,-814,-631,221,213,211,215,-1,0,0,0);
	set_server_sector_info(i,0,215,-1,0,-781,-606,214,210,221,-1,-1,0,0,0);
	set_server_sector_info(i,0,216,-1,0,-730,-577,210,218,183,-1,-1,0,0,0);
	set_server_sector_info(i,0,217,-1,0,-731,-615,210,218,220,-1,-1,0,0,0);
	set_server_sector_info(i,0,218,-1,0,-680,-606,216,184,219,217,-1,0,0,0);
	set_server_sector_info(i,0,219,-1,0,-639,-642,225,218,220,-1,-1,0,0,0);
	set_server_sector_info(i,0,220,-1,0,-699,-655,222,217,219,223,221,0,0,0);
	set_server_sector_info(i,0,221,-1,0,-762,-651,222,214,220,215,-1,0,0,0);
	set_server_sector_info(i,0,222,-1,0,-721,-690,223,221,220,-1,-1,0,0,0);
	set_server_sector_info(i,0,223,-1,0,-658,-684,225,222,220,-1,-1,0,0,0);
	set_server_sector_info(i,0,224,-1,0,-585,-624,232,185,225,-1,-1,0,0,0);
	set_server_sector_info(i,0,225,-1,0,-598,-668,224,226,223,219,-1,0,0,0);
	set_server_sector_info(i,0,226,-1,0,-534,-676,232,227,225,-1,-1,0,0,0);
	set_server_sector_info(i,0,227,-1,0,-487,-685,226,228,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,228,-1,0,-445,-646,232,229,227,-1,-1,0,0,0);
	set_server_sector_info(i,0,229,-1,0,-456,-595,230,228,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,230,-1,0,-464,-547,163,188,229,231,-1,0,0,0);
	set_server_sector_info(i,0,231,-1,0,-504,-585,230,232,-1,-1,-1,0,0,0);
	set_server_sector_info(i,0,232,-1,0,-517,-628,231,187,224,226,228,0,0,0);

	
	
	//Diaspora zone
	k=1;
	strcpy(zone[k].name,"Diaspora");
	zone[k].home_class = 0;
	set_server_sector_info(i,1,0,6,0,-231,-69,1,-1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,1,-1,0,-190,-74,0,2,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,2,-1,0,-156,-54,3,1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,3,-1,0,-128,-15,2,10,4,29,19,0,0,0);
	set_server_sector_info(i,1,4,3,0,-87,13,3,5,29,18,10,0,0,0);
	set_server_sector_info(i,1,5,-1,0,-44,48,4,6,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,6,-1,0,-30,93,5,7,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,7,-1,0,-7,119,6,8,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,8,-1,0,36,127,9,7,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,9,7,0,70,110,8,-1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,10,-1,0,-81,-25,11,3,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,11,-1,0,-40,-33,12,17,18,10,-1,0,0,0);
	set_server_sector_info(i,1,12,-1,0,6,-56,13,16,17,11,-1,0,0,0);
	set_server_sector_info(i,1,13,-1,0,30,-93,14,15,16,12,-1,0,0,0);
	set_server_sector_info(i,1,14,4,0,54,-129,13,15,-1,-1,-1,0,0,0);
	set_server_sector_info(i,1,15,-1,0,82,-93,16,13,30,14,-1,0,0,0);
	set_server_sector_info(i,1,16,-1,0,58,-55,15,13,12,17,36,0,0,0);
	set_server_sector_info(i,1,17,-1,0,9,-11,16,12,11,18,-1,0,0,0);
	set_server_sector_info(i,1,18,-1,0,-45,9,17,11,4,-1,-1,0,0,0);
	set_server_sector_info(i,1,19,-1,0,-155,30,3,29,20,-1,-1,1,0,0);
	set_server_sector_info(i,1,20,-1,0,-163,77,19,28,21,-1,-1,1,0,0);
	set_server_sector_info(i,1,21,-1,0,-199,110,38,20,25,22,-1,1,0,0);
	set_server_sector_info(i,1,22,-1,0,-257,130,38,21,23,-1,-1,1,0,0);
	set_server_sector_info(i,1,23,-1,0,-241,178,38,22,24,-1,-1,1,0,0);
	set_server_sector_info(i,1,24,-1,0,-196,178,38,23,25,-1,-1,1,0,0);
	set_server_sector_info(i,1,25,-1,0,-148,156,21,24,27,26,-1,1,0,0);
	set_server_sector_info(i,1,26,-1,0,-113,176,25,27,-1,-1,-1,1,0,0);
	set_server_sector_info(i,1,27,-1,0,-109,123,25,26,28,-1,-1,1,0,0);
	set_server_sector_info(i,1,28,-1,0,-127,83,29,20,27,-1,-1,1,0,0);
	set_server_sector_info(i,1,29,-1,0,-119,44,4,3,19,28,-1,1,0,0);
	set_server_sector_info(i,1,30,-1,0,106,-66,15,36,31,37,-1,2,0,0);
	set_server_sector_info(i,1,31,-1,0,133,-31,35,30,34,32,-1,2,0,0);
	set_server_sector_info(i,1,32,-1,0,179,-18,33,31,37,39,-1,2,0,0);
	set_server_sector_info(i,1,33,10,0,200,31,34,32,43,42,39,2,0,0);
	set_server_sector_info(i,1,34,-1,0,153,27,33,35,31,-1,-1,2,0,0);
	set_server_sector_info(i,1,35,-1,0,100,8,36,31,34,-1,-1,2,0,0);
	set_server_sector_info(i,1,36,-1,0,80,-24,16,30,35,-1,-1,2,0,0);
	set_server_sector_info(i,1,37,-1,0,156,-73,30,32,39,-1,-1,2,0,0);
	set_server_sector_info(i,1,38,5,0,-210,145,21,22,23,24,-1,1,0,0);
	set_server_sector_info(i,1,39,-1,0,228,-35,37,32,40,33,-1,2,0,0);
	set_server_sector_info(i,1,40,-1,0,280,15,39,43,41,-1,-1,2,0,0);
	set_server_sector_info(i,1,41,-1,0,265,76,42,43,40,44,-1,2,0,0);
	set_server_sector_info(i,1,42,-1,0,213,100,33,44,41,43,-1,2,0,0);
	set_server_sector_info(i,1,43,-1,0,240,36,33,42,40,41,-1,2,0,0);
	set_server_sector_info(i,1,44,-1,0,249,145,42,41,45,-1,-1,2,0,0);
	set_server_sector_info(i,1,45,27,0,222,185,44,-1,-1,-1,-1,2,0,0);
	
	//Velic zone
	k=2;
	strcpy(zone[k].name,"Velic");
	zone[k].home_class = 2;
	set_server_sector_info(i,2,0,20,1,-256,-24,1,-1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,1,-1,1,-217,-45,0,2,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,2,-1,1,-172,-53,1,3,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,3,-1,0,-125,-41,2,4,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,4,-1,0,-88,-14,3,20,5,-1,-1,0,0,0);
	set_server_sector_info(i,2,5,-1,0,-53,18,4,20,6,25,-1,0,0,0);
	set_server_sector_info(i,2,6,-1,0,-9,47,25,5,7,-1,-1,0,0,0);
	set_server_sector_info(i,2,7,-1,0,45,61,6,8,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,8,-1,1,81,58,7,9,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,9,7,1,130,44,8,10,32,35,-1,0,0,0);
	set_server_sector_info(i,2,10,-1,1,172,26,9,11,63,-1,-1,0,0,0);
	set_server_sector_info(i,2,11,-1,0,218,9,10,12,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,12,-1,0,269,7,11,13,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,13,-1,0,319,16,12,14,67,68,-1,0,0,0);
	set_server_sector_info(i,2,14,-1,0,362,26,13,15,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,15,-1,0,411,26,14,16,68,-1,-1,0,0,0);
	set_server_sector_info(i,2,16,-1,0,453,2,15,17,70,-1,-1,0,0,0);
	set_server_sector_info(i,2,17,-1,0,478,-40,16,18,71,72,-1,1,0,0);
	set_server_sector_info(i,2,18,-1,0,497,-107,17,19,71,-1,-1,1,0,0);
	set_server_sector_info(i,2,19,33,0,501,-196,18,73,-1,-1,-1,1,0,0);
	set_server_sector_info(i,2,20,-1,0,-49,-17,4,5,21,-1,-1,0,0,0);
	set_server_sector_info(i,2,21,-1,0,-41,-69,20,22,59,-1,-1,0,0,0);
	set_server_sector_info(i,2,22,-1,1,-15,-117,21,23,60,-1,-1,0,0,0);
	set_server_sector_info(i,2,23,-1,1,21,-148,22,24,29,-1,-1,0,0,0);
	set_server_sector_info(i,2,24,19,1,47,-191,23,62,33,-1,-1,0,0,0);
	set_server_sector_info(i,2,25,-1,0,-51,68,5,26,6,-1,-1,0,0,0);
	set_server_sector_info(i,2,26,-1,1,-80,107,25,27,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,27,-1,1,-126,109,26,28,43,-1,-1,0,0,0);
	set_server_sector_info(i,2,28,21,1,-163,81,27,-1,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,29,-1,0,48,-124,23,30,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,30,-1,0,85,-94,33,29,31,34,-1,0,0,0);
	set_server_sector_info(i,2,31,-1,0,91,-41,30,32,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,32,-1,0,110,9,31,9,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,33,-1,0,86,-147,24,30,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,34,-1,0,124,-62,30,63,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,35,-1,0,119,96,9,36,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,36,-1,0,77,116,35,37,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,37,-1,0,49,169,36,38,39,-1,-1,0,0,0);
	set_server_sector_info(i,2,38,-1,0,1,207,37,39,40,-1,-1,0,0,0);
	set_server_sector_info(i,2,39,-1,0,-29,184,37,38,40,42,-1,0,0,0);
	set_server_sector_info(i,2,40,-1,0,-55,223,39,41,38,-1,-1,0,0,0);
	set_server_sector_info(i,2,41,-1,0,-121,212,43,42,40,-1,-1,0,0,0);
	set_server_sector_info(i,2,42,-1,0,-107,182,43,39,41,-1,-1,0,0,0);
	set_server_sector_info(i,2,43,-1,0,-153,149,27,44,42,41,-1,0,0,0);
	set_server_sector_info(i,2,44,-1,0,-218,110,43,48,45,-1,-1,0,0,0);
	set_server_sector_info(i,2,45,-1,0,-282,136,44,47,46,-1,-1,0,0,0);
	set_server_sector_info(i,2,46,-1,0,-362,132,45,47,50,-1,-1,0,0,0);
	set_server_sector_info(i,2,47,-1,0,-316,83,48,45,46,49,-1,0,0,0);
	set_server_sector_info(i,2,48,-1,0,-247,70,44,47,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,49,-1,0,-357,34,47,50,51,-1,-1,0,0,0);
	set_server_sector_info(i,2,50,-1,0,-414,34,46,49,52,-1,-1,0,0,0);
	set_server_sector_info(i,2,51,-1,0,-324,-24,49,52,54,-1,-1,0,0,0);
	set_server_sector_info(i,2,52,-1,0,-392,-45,51,50,53,-1,-1,0,0,0);
	set_server_sector_info(i,2,53,-1,0,-362,-118,52,55,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,54,-1,0,-312,-82,51,55,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,55,-1,0,-292,-133,54,53,56,-1,-1,0,0,0);
	set_server_sector_info(i,2,56,-1,0,-240,-169,55,57,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,57,-1,0,-185,-146,56,58,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,58,-1,0,-122,-132,57,59,60,-1,-1,0,0,0);
	set_server_sector_info(i,2,59,-1,0,-89,-86,58,21,60,-1,-1,0,0,0);
	set_server_sector_info(i,2,60,-1,0,-57,-110,59,58,22,61,-1,0,0,0);
	set_server_sector_info(i,2,61,-1,0,-45,-166,60,62,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,62,-1,0,-7,-183,61,24,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,63,-1,0,169,-11,34,64,10,-1,-1,0,0,0);
	set_server_sector_info(i,2,64,-1,0,207,-49,63,65,66,-1,-1,0,0,0);
	set_server_sector_info(i,2,65,-1,0,242,-71,64,67,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,66,-1,0,252,-34,64,67,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,67,-1,0,297,-22,13,66,65,-1,-1,0,0,0);
	set_server_sector_info(i,2,68,-1,0,371,73,13,15,69,-1,-1,0,0,0);
	set_server_sector_info(i,2,69,34,0,430,77,68,70,-1,-1,-1,0,0,0);
	set_server_sector_info(i,2,70,-1,0,471,42,16,69,71,-1,-1,0,0,0);
	set_server_sector_info(i,2,71,-1,0,501,-9,70,17,18,-1,-1,1,0,0);
	set_server_sector_info(i,2,72,-1,0,463,-102,17,73,-1,-1,-1,1,0,0);
	set_server_sector_info(i,2,73,-1,0,469,-153,72,19,-1,-1,-1,1,0,0);
	
	//Old Diaspora Zone
	k=3;
	strcpy(zone[k].name,"Perished");
	zone[k].home_class = 0;
	set_server_sector_info(i,3,0,13,0,-5,44,5,1,11,9,-1,1,0,0);
	set_server_sector_info(i,3,1,-1,0,54,44,0,2,6,12,-1,1,0,0);
	set_server_sector_info(i,3,2,-1,0,91,44,1,3,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,3,-1,0,129,44,2,4,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,4,33,0,179,44,3,-1,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,5,-1,0,-5,5,0,6,8,-1,-1,1,0,0);
	set_server_sector_info(i,3,6,-1,0,32,-13,7,5,1,-1,-1,1,0,0);
	set_server_sector_info(i,3,7,-1,0,88,-23,6,-1,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,8,-1,0,-43,-15,5,9,19,23,-1,1,0,0);
	set_server_sector_info(i,3,9,-1,0,-43,44,0,8,10,19,-1,1,0,0);
	set_server_sector_info(i,3,10,-1,0,-43,82,11,9,24,-1,-1,1,0,0);
	set_server_sector_info(i,3,11,-1,0,-5,82,0,12,10,-1,-1,1,0,0);
	set_server_sector_info(i,3,12,-1,0,35,82,13,1,11,-1,-1,1,0,0);
	set_server_sector_info(i,3,13,-1,0,56,106,14,18,12,-1,-1,1,0,0);
	set_server_sector_info(i,3,14,-1,0,88,138,13,18,15,-1,-1,1,0,0);
	set_server_sector_info(i,3,15,-1,0,127,138,16,14,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,16,-1,0,169,138,17,15,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,17,-1,0,223,138,16,-1,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,18,15,0,56,138,45,44,14,13,-1,1,0,0);
	set_server_sector_info(i,3,19,-1,0,-81,44,8,9,24,20,-1,1,0,0);
	set_server_sector_info(i,3,20,-1,0,-120,44,19,21,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,21,-1,0,-157,44,22,20,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,22,11,0,-200,44,21,-1,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,23,-1,0,-91,-24,8,-1,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,24,-1,0,-63,103,10,19,25,26,-1,1,0,0);
	set_server_sector_info(i,3,25,14,0,-63,141,24,31,32,26,-1,1,0,0);
	set_server_sector_info(i,3,26,-1,0,-90,141,24,25,27,-1,-1,1,0,0);
	set_server_sector_info(i,3,27,-1,0,-122,141,26,33,28,-1,-1,1,0,0);
	set_server_sector_info(i,3,28,-1,0,-162,141,27,29,34,-1,-1,1,0,0);
	set_server_sector_info(i,3,29,-1,0,-202,141,28,52,30,-1,-1,4,0,0);
	set_server_sector_info(i,3,30,-1,0,-254,141,29,107,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,31,-1,0,-44,180,25,41,40,-1,-1,1,0,0);
	set_server_sector_info(i,3,32,-1,0,-82,180,25,33,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,33,-1,0,-122,195,32,41,27,34,-1,1,0,0);
	set_server_sector_info(i,3,34,-1,0,-162,215,33,28,35,51,-1,1,0,0);
	set_server_sector_info(i,3,35,-1,0,-179,250,34,36,-1,-1,-1,2,0,0);
	set_server_sector_info(i,3,36,-1,0,-179,288,35,37,61,-1,-1,2,0,0);
	set_server_sector_info(i,3,37,-1,0,-143,272,38,36,-1,-1,-1,2,0,0);
	set_server_sector_info(i,3,38,-1,0,-103,274,37,39,57,-1,-1,2,0,0);
	set_server_sector_info(i,3,39,-1,0,-44,253,40,53,38,-1,-1,2,0,0);
	set_server_sector_info(i,3,40,-1,0,-44,216,42,31,39,-1,-1,1,0,0);
	set_server_sector_info(i,3,41,-1,0,-83,217,33,31,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,42,-1,0,12,231,40,48,47,43,-1,1,0,0);
	set_server_sector_info(i,3,43,-1,0,12,195,42,47,44,-1,-1,1,0,0);
	set_server_sector_info(i,3,44,-1,0,35,168,43,18,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,45,-1,0,71,170,18,46,-1,-1,-1,1,0,0);
	set_server_sector_info(i,3,46,-1,0,92,215,77,49,47,45,-1,2,0,0);
	set_server_sector_info(i,3,47,-1,0,50,243,42,76,46,48,43,2,0,0);
	set_server_sector_info(i,3,48,-1,0,12,271,74,42,47,-1,-1,2,0,0);
	set_server_sector_info(i,3,49,-1,0,126,239,77,50,46,-1,-1,2,0,0);
	set_server_sector_info(i,3,50,-1,0,166,249,84,49,-1,-1,-1,3,0,0);
	set_server_sector_info(i,3,51,-1,0,-215,215,34,52,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,52,-1,0,-273,215,51,29,107,116,-1,4,0,0);
	set_server_sector_info(i,3,53,-1,0,-44,289,39,54,-1,-1,-1,2,0,0);
	set_server_sector_info(i,3,54,16,0,-44,323,57,53,55,-1,-1,2,0,0);
	set_server_sector_info(i,3,55,-1,0,-44,365,54,56,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,56,-1,0,-44,403,55,66,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,57,-1,0,-83,307,38,54,58,-1,-1,2,0,0);
	set_server_sector_info(i,3,58,-1,0,-110,325,57,59,-1,-1,-1,3,0,0);
	set_server_sector_info(i,3,59,-1,0,-138,343,58,60,-1,-1,-1,3,0,0);
	set_server_sector_info(i,3,60,-1,0,-182,367,59,61,62,63,-1,3,0,0);
	set_server_sector_info(i,3,61,27,0,-198,330,36,60,-1,-1,-1,2,0,0);
	set_server_sector_info(i,3,62,-1,0,-139,391,60,63,64,-1,-1,4,0,0);
	set_server_sector_info(i,3,63,-1,0,-182,409,62,60,65,-1,-1,4,0,0);
	set_server_sector_info(i,3,64,-1,0,-121,430,62,67,65,-1,-1,4,0,0);
	set_server_sector_info(i,3,65,-1,0,-162,444,63,64,68,-1,-1,4,0,0);
	set_server_sector_info(i,3,66,-1,0,-76,445,67,56,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,67,-1,0,-98,483,64,66,68,69,-1,4,0,0);
	set_server_sector_info(i,3,68,-1,0,-134,483,65,67,70,-1,-1,4,0,0);
	set_server_sector_info(i,3,69,-1,0,-120,517,67,70,95,-1,-1,4,0,0);
	set_server_sector_info(i,3,70,-1,0,-176,503,68,69,71,97,-1,4,0,0);
	set_server_sector_info(i,3,71,-1,0,-201,461,70,72,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,72,-1,0,-236,464,71,73,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,73,-1,0,-277,482,72,98,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,74,-1,0,12,306,75,76,48,-1,-1,2,0,0);
	set_server_sector_info(i,3,75,17,0,12,348,78,74,-1,-1,-1,2,0,0);
	set_server_sector_info(i,3,76,-1,0,50,284,78,77,74,47,-1,2,0,0);
	set_server_sector_info(i,3,77,-1,0,92,261,79,76,49,46,-1,2,0,0);
	set_server_sector_info(i,3,78,-1,0,50,348,79,75,76,-1,-1,2,0,0);
	set_server_sector_info(i,3,79,-1,0,92,348,80,78,77,-1,-1,2,0,0);
	set_server_sector_info(i,3,80,-1,0,120,371,88,86,81,79,-1,3,0,0);
	set_server_sector_info(i,3,81,-1,0,157,352,80,82,-1,-1,-1,3,0,0);
	set_server_sector_info(i,3,82,-1,0,186,327,81,86,83,85,-1,3,0,0);
	set_server_sector_info(i,3,83,-1,0,206,310,82,86,84,-1,-1,3,0,0);
	set_server_sector_info(i,3,84,-1,0,185,291,83,85,50,-1,-1,3,0,0);
	set_server_sector_info(i,3,85,18,0,147,311,82,84,-1,-1,-1,3,0,0);
	set_server_sector_info(i,3,86,-1,0,206,369,87,80,82,83,-1,3,0,0);
	set_server_sector_info(i,3,87,-1,0,206,404,88,94,86,-1,-1,4,0,0);
	set_server_sector_info(i,3,88,-1,0,167,404,89,87,94,80,-1,4,0,0);
	set_server_sector_info(i,3,89,-1,0,127,404,90,88,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,90,-1,0,87,404,91,89,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,91,23,0,65,446,90,92,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,92,-1,0,103,446,91,93,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,93,-1,0,146,446,94,92,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,94,-1,0,187,446,87,88,93,113,-1,4,0,0);
	set_server_sector_info(i,3,95,-1,0,-139,554,69,96,100,-1,-1,4,0,0);
	set_server_sector_info(i,3,96,-1,0,-178,553,95,100,97,-1,-1,4,0,0);
	set_server_sector_info(i,3,97,-1,0,-199,537,96,70,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,98,-1,0,-311,516,73,99,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,99,-1,0,-313,575,98,101,-1,-1,-1,5,0,1);
	set_server_sector_info(i,3,100,22,0,-180,584,95,96,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,101,-1,0,-299,615,99,102,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,102,-1,0,-265,653,101,103,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,103,-1,0,-220,677,102,104,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,104,-1,0,-173,680,103,105,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,105,-1,0,-127,698,104,106,184,-1,-1,5,0,0);
	set_server_sector_info(i,3,106,-1,0,-82,711,105,185,190,-1,-1,5,0,0);
	set_server_sector_info(i,3,107,-1,0,-311,141,30,114,52,-1,-1,4,0,0);
	set_server_sector_info(i,3,108,-1,0,88,693,109,186,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,109,-1,0,127,653,110,108,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,110,-1,0,163,619,111,109,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,111,-1,0,199,578,112,110,-1,-1,-1,5,0,1);
	set_server_sector_info(i,3,112,-1,0,224,538,113,111,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,113,-1,0,223,481,94,112,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,114,-1,0,-372,141,107,116,115,118,-1,4,0,0);
	set_server_sector_info(i,3,115,-1,0,-430,156,119,114,120,-1,-1,4,0,0);
	set_server_sector_info(i,3,116,-1,0,-330,215,52,117,114,-1,-1,4,0,0);
	set_server_sector_info(i,3,117,-1,0,-371,255,116,118,-1,-1,-1,4,0,0);
	set_server_sector_info(i,3,118,-1,0,-423,272,117,119,114,-1,-1,4,0,0);
	set_server_sector_info(i,3,119,-1,0,-481,275,115,118,120,122,-1,4,0,0);
	set_server_sector_info(i,3,120,24,0,-504,198,115,119,121,-1,-1,4,0,0);
	set_server_sector_info(i,3,121,-1,0,-523,234,120,122,127,-1,-1,5,0,0);
	set_server_sector_info(i,3,122,-1,0,-523,294,119,121,123,-1,-1,5,0,0);
	set_server_sector_info(i,3,123,-1,0,-563,310,122,124,125,127,-1,5,0,0);
	set_server_sector_info(i,3,124,-1,0,-546,349,123,138,136,134,-1,5,0,0);
	set_server_sector_info(i,3,125,-1,0,-602,350,123,126,134,-1,-1,5,0,0);
	set_server_sector_info(i,3,126,-1,0,-594,234,127,125,133,128,-1,5,0,0);
	set_server_sector_info(i,3,127,-1,0,-563,219,123,121,126,-1,-1,5,0,0);
	set_server_sector_info(i,3,128,-1,0,-657,217,126,129,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,129,-1,0,-693,292,128,130,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,130,-1,0,-678,386,135,141,131,129,-1,5,0,0);
	set_server_sector_info(i,3,131,-1,0,-658,348,134,130,132,-1,-1,5,0,0);
	set_server_sector_info(i,3,132,-1,0,-639,310,131,133,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,133,-1,0,-615,271,132,126,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,134,-1,0,-637,387,124,135,131,125,-1,5,0,0);
	set_server_sector_info(i,3,135,-1,0,-658,423,142,136,130,134,-1,5,0,0);
	set_server_sector_info(i,3,136,-1,0,-597,407,124,137,135,143,-1,5,0,0);
	set_server_sector_info(i,3,137,-1,0,-560,407,140,138,136,-1,-1,5,0,0);
	set_server_sector_info(i,3,138,-1,0,-520,387,139,124,137,-1,-1,5,0,0);
	set_server_sector_info(i,3,139,-1,0,-487,408,145,140,138,-1,-1,6,0,0);
	set_server_sector_info(i,3,140,-1,0,-521,427,144,139,137,-1,-1,6,0,0);
	set_server_sector_info(i,3,141,25,0,-699,423,142,130,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,142,-1,0,-675,465,143,135,141,154,-1,5,0,0);
	set_server_sector_info(i,3,143,-1,0,-620,448,142,136,-1,-1,-1,5,0,0);
	set_server_sector_info(i,3,144,-1,0,-505,466,147,148,145,140,-1,6,0,0);
	set_server_sector_info(i,3,145,-1,0,-468,448,144,146,139,139,-1,6,0,0);
	set_server_sector_info(i,3,146,-1,0,-444,486,147,145,-1,-1,-1,6,0,1);
	set_server_sector_info(i,3,147,-1,0,-482,517,150,146,144,148,-1,6,0,0);
	set_server_sector_info(i,3,148,-1,0,-549,517,149,147,144,-1,-1,6,0,0);
	set_server_sector_info(i,3,149,-1,0,-581,558,151,148,-1,-1,-1,6,0,0);
	set_server_sector_info(i,3,150,28,0,-504,578,156,151,147,-1,-1,6,0,0);
	set_server_sector_info(i,3,151,-1,0,-619,595,157,150,149,152,-1,6,0,0);
	set_server_sector_info(i,3,152,-1,0,-675,577,155,151,153,-1,-1,6,0,0);
	set_server_sector_info(i,3,153,-1,0,-675,540,152,154,-1,-1,-1,6,0,0);
	set_server_sector_info(i,3,154,-1,0,-675,502,153,142,-1,-1,-1,6,0,0);
	set_server_sector_info(i,3,155,-1,0,-696,617,157,152,-1,-1,-1,6,0,1);
	set_server_sector_info(i,3,156,-1,0,-544,616,158,157,150,-1,-1,6,0,0);
	set_server_sector_info(i,3,157,-1,0,-659,634,159,156,155,151,-1,6,0,0);
	set_server_sector_info(i,3,158,-1,0,-580,658,160,159,156,-1,-1,6,0,0);
	set_server_sector_info(i,3,159,-1,0,-680,674,161,158,157,-1,-1,6,0,0);
	set_server_sector_info(i,3,160,-1,0,-605,693,162,161,158,-1,-1,6,0,0);
	set_server_sector_info(i,3,161,-1,0,-699,712,164,160,159,-1,-1,6,0,0);
	set_server_sector_info(i,3,162,-1,0,-605,731,163,164,160,-1,-1,6,0,0);
	set_server_sector_info(i,3,163,-1,0,-605,768,168,167,165,162,-1,6,0,0);
	set_server_sector_info(i,3,164,-1,0,-699,748,165,162,161,-1,-1,6,0,0);
	set_server_sector_info(i,3,165,-1,0,-698,785,166,163,164,-1,-1,6,0,0);
	set_server_sector_info(i,3,166,-1,0,-698,822,172,167,165,-1,-1,6,0,0);
	set_server_sector_info(i,3,167,-1,0,-605,804,171,166,163,-1,-1,6,0,0);
	set_server_sector_info(i,3,168,-1,0,-545,750,163,169,-1,-1,-1,6,0,0);
	set_server_sector_info(i,3,169,-1,0,-505,806,177,168,178,170,-1,6,0,0);
	set_server_sector_info(i,3,170,-1,0,-522,863,169,176,174,171,-1,6,0,0);
	set_server_sector_info(i,3,171,-1,0,-578,837,170,167,172,-1,-1,6,0,0);
	set_server_sector_info(i,3,172,-1,0,-661,869,173,171,166,-1,-1,6,0,0);
	set_server_sector_info(i,3,173,29,0,-621,903,175,174,172,-1,-1,6,0,0);
	set_server_sector_info(i,3,174,-1,0,-562,881,170,175,173,-1,-1,6,0,0);
	set_server_sector_info(i,3,175,-1,0,-562,922,176,174,173,-1,-1,6,0,0);
	set_server_sector_info(i,3,176,-1,0,-466,901,178,170,175,-1,-1,6,0,0);
	set_server_sector_info(i,3,177,-1,0,-445,808,183,179,169,-1,-1,6,0,0);
	set_server_sector_info(i,3,178,-1,0,-405,887,169,179,176,-1,-1,6,0,0);
	set_server_sector_info(i,3,179,-1,0,-353,887,180,177,178,-1,-1,6,0,0);
	set_server_sector_info(i,3,180,-1,0,-294,887,181,183,179,-1,-1,6,0,0);
	set_server_sector_info(i,3,181,-1,0,-224,887,192,182,180,-1,-1,6,0,0);
	set_server_sector_info(i,3,182,-1,0,-295,827,184,181,183,-1,-1,6,0,0);
	set_server_sector_info(i,3,183,-1,0,-372,827,182,180,177,-1,-1,6,0,0);
	set_server_sector_info(i,3,184,-1,0,-223,827,190,105,192,182,-1,5,0,0);
	set_server_sector_info(i,3,185,-1,0,-30,710,186,187,188,106,-1,5,0,0);
	set_server_sector_info(i,3,186,-1,0,30,726,108,185,187,-1,-1,5,0,0);
	set_server_sector_info(i,3,187,-1,0,-15,749,186,185,188,189,-1,5,0,0);
	set_server_sector_info(i,3,188,26,0,-63,778,185,190,187,189,-1,5,0,0);
	set_server_sector_info(i,3,189,-1,0,-39,820,187,188,191,-1,-1,5,0,0);
	set_server_sector_info(i,3,190,-1,0,-142,798,106,188,184,191,-1,5,0,0);
	set_server_sector_info(i,3,191,-1,0,-103,843,190,189,192,-1,-1,5,0,0);
	set_server_sector_info(i,3,192,-1,0,-166,865,191,184,181,-1,-1,5,0,0);
	set_server_sector_info(i,3,193,-1,0,-548,457,-1,-1,-1,-1,-1,0,1,0);
	
	//Banished Zone
	k=4;
	strcpy(zone[k].name,"Banished");
	zone[k].home_class = 0;
	set_server_sector_info(i,4,0,28,0,274,-172,1,-1,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,1,-1,0,257,-142,0,2,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,2,-1,0,228,-110,1,3,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,3,-1,0,190,-92,2,4,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,4,-1,0,148,-89,3,5,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,5,-1,0,109,-73,4,6,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,6,-1,0,88,-39,5,7,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,7,-1,0,85,4,6,8,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,8,-1,0,111,43,7,9,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,9,-1,0,101,80,8,18,10,11,-1,6,0,1);
	set_server_sector_info(i,4,10,-1,0,54,37,9,11,16,-1,-1,6,0,0);
	set_server_sector_info(i,4,11,-1,0,56,95,18,10,9,14,12,6,0,0);
	set_server_sector_info(i,4,12,-1,0,7,109,14,11,13,-1,-1,6,0,0);
	set_server_sector_info(i,4,13,-1,0,-46,99,14,12,25,24,15,6,0,0);
	set_server_sector_info(i,4,14,30,0,8,66,13,12,11,15,-1,6,0,0);
	set_server_sector_info(i,4,15,-1,0,-37,35,14,16,13,17,23,6,0,0);
	set_server_sector_info(i,4,16,-1,0,11,18,10,15,17,-1,-1,6,0,0);
	set_server_sector_info(i,4,17,-1,0,-54,-14,16,15,21,22,-1,6,0,0);
	set_server_sector_info(i,4,18,-1,0,115,124,9,11,19,-1,-1,6,0,0);
	set_server_sector_info(i,4,19,-1,0,150,159,18,20,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,20,29,0,201,184,19,-1,-1,-1,-1,6,0,0);
	set_server_sector_info(i,4,21,-1,0,-100,-60,26,22,17,-1,-1,7,0,0);
	set_server_sector_info(i,4,22,-1,0,-100,-10,27,23,21,17,-1,7,0,0);
	set_server_sector_info(i,4,23,-1,0,-100,40,22,24,15,-1,-1,7,0,0);
	set_server_sector_info(i,4,24,-1,0,-100,90,28,23,25,13,-1,7,0,0);
	set_server_sector_info(i,4,25,-1,0,-100,140,29,24,13,-1,-1,7,0,0);
	set_server_sector_info(i,4,26,-1,0,-140,-60,34,21,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,27,-1,0,-140,-10,33,22,-1,-1,-1,7,0,1);
	set_server_sector_info(i,4,28,-1,0,-140,90,31,24,-1,-1,-1,7,0,1);
	set_server_sector_info(i,4,29,-1,0,-140,140,30,25,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,30,-1,0,-180,140,36,29,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,31,-1,0,-180,90,32,36,28,-1,-1,7,0,0);
	set_server_sector_info(i,4,32,-1,0,-180,40,33,37,31,-1,-1,7,0,0);
	set_server_sector_info(i,4,33,-1,0,-180,-10,32,35,27,-1,-1,7,0,0);
	set_server_sector_info(i,4,34,-1,0,-180,-60,35,26,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,35,-1,0,-220,-35,39,-1,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,36,-1,0,-220,115,40,-1,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,37,-1,0,-220,40,38,32,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,38,-1,0,-260,40,-1,-1,37,-1,-1,7,1,0);
	set_server_sector_info(i,4,39,-1,0,-260,-35,41,35,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,40,-1,0,-260,115,43,36,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,41,-1,0,-300,-35,45,42,39,-1,-1,7,0,0);
	set_server_sector_info(i,4,42,-1,0,-300,15,46,41,38,-1,-1,7,0,0);
	set_server_sector_info(i,4,43,-1,0,-300,115,44,48,40,-1,-1,7,0,0);
	set_server_sector_info(i,4,44,-1,0,-300,65,47,43,38,-1,-1,7,0,0);
	set_server_sector_info(i,4,45,-1,0,-340,-35,41,46,100,99,85,7,0,0);
	set_server_sector_info(i,4,46,-1,0,-340,15,45,42,47,-1,-1,7,0,0);
	set_server_sector_info(i,4,47,-1,0,-340,65,46,44,48,98,-1,7,0,0);
	set_server_sector_info(i,4,48,-1,0,-340,115,47,43,98,-1,-1,7,0,0);
	set_server_sector_info(i,4,49,-1,0,152,-23,-1,-1,-1,-1,-1,0,1,0);
	set_server_sector_info(i,4,50,-1,0,-470,-190,51,55,78,-1,-1,8,0,0);
	set_server_sector_info(i,4,51,-1,0,-520,-190,50,56,52,78,-1,8,0,0);
	set_server_sector_info(i,4,52,-1,0,-570,-190,51,53,78,-1,-1,8,0,0);
	set_server_sector_info(i,4,53,-1,0,-620,-190,52,57,54,89,-1,8,0,0);
	set_server_sector_info(i,4,54,-1,0,-670,-190,53,58,89,-1,-1,8,0,0);
	set_server_sector_info(i,4,55,-1,0,-470,-230,50,59,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,56,-1,0,-520,-230,51,60,-1,-1,-1,8,0,1);
	set_server_sector_info(i,4,57,-1,0,-620,-230,62,53,-1,-1,-1,8,0,1);
	set_server_sector_info(i,4,58,-1,0,-670,-230,54,63,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,59,-1,0,-470,-270,64,55,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,60,-1,0,-520,-270,56,64,61,-1,-1,8,0,0);
	set_server_sector_info(i,4,61,-1,0,-570,-270,62,60,76,-1,-1,8,0,0);
	set_server_sector_info(i,4,62,-1,0,-620,-270,57,72,61,-1,-1,8,0,0);
	set_server_sector_info(i,4,63,-1,0,-670,-270,58,72,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,64,-1,0,-495,-310,-1,-1,65,-1,-1,8,0,0);
	set_server_sector_info(i,4,65,-1,0,-495,-350,64,66,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,66,-1,0,-495,-390,65,68,67,-1,-1,8,0,0);
	set_server_sector_info(i,4,67,-1,0,-495,-430,66,69,105,-1,-1,8,0,0);
	set_server_sector_info(i,4,68,-1,0,-545,-390,77,66,69,-1,-1,8,0,0);
	set_server_sector_info(i,4,69,-1,0,-545,-430,67,68,71,104,-1,8,0,0);
	set_server_sector_info(i,4,70,-1,0,-595,-390,77,74,71,-1,-1,8,0,0);
	set_server_sector_info(i,4,71,-1,0,-595,-430,69,70,75,103,-1,8,0,0);
	set_server_sector_info(i,4,72,-1,0,-645,-310,-1,-1,73,-1,-1,8,0,0);
	set_server_sector_info(i,4,73,-1,0,-645,-350,72,74,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,74,-1,0,-645,-390,70,73,75,-1,-1,8,0,0);
	set_server_sector_info(i,4,75,-1,0,-645,-430,71,74,102,-1,-1,8,0,0);
	set_server_sector_info(i,4,76,-1,0,-570,-310,61,77,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,77,-1,0,-570,-350,76,-1,-1,-1,-1,8,1,0);
	set_server_sector_info(i,4,78,-1,0,-528,-145,79,50,51,52,-1,8,0,0);
	set_server_sector_info(i,4,79,-1,0,-559,-117,80,78,89,-1,-1,8,0,0);
	set_server_sector_info(i,4,80,-1,0,-569,-69,90,81,79,-1,-1,8,0,0);
	set_server_sector_info(i,4,81,-1,0,-543,-17,82,93,80,-1,-1,8,0,0);
	set_server_sector_info(i,4,82,-1,0,-508,36,96,83,81,94,-1,7,0,0);
	set_server_sector_info(i,4,83,-1,0,-476,-2,84,97,82,-1,-1,7,0,0);
	set_server_sector_info(i,4,84,-1,0,-424,-46,99,83,85,-1,-1,7,0,0);
	set_server_sector_info(i,4,85,-1,0,-390,-2,45,84,97,-1,-1,7,0,0);
	set_server_sector_info(i,4,86,31,1,-699,125,87,-1,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,87,-1,1,-659,153,88,86,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,88,-1,1,-605,151,95,87,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,89,-1,0,-622,-131,90,54,53,79,-1,8,0,0);
	set_server_sector_info(i,4,90,-1,0,-627,-76,91,80,89,-1,-1,8,0,0);
	set_server_sector_info(i,4,91,-1,0,-659,-20,92,90,-1,-1,-1,8,0,1);
	set_server_sector_info(i,4,92,-1,0,-610,6,93,91,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,93,-1,0,-564,45,94,81,92,-1,-1,8,0,0);
	set_server_sector_info(i,4,94,-1,0,-544,93,82,96,95,93,-1,7,0,0);
	set_server_sector_info(i,4,95,-1,1,-561,136,94,88,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,96,-1,0,-473,80,97,82,94,-1,-1,7,0,1);
	set_server_sector_info(i,4,97,-1,0,-427,44,98,85,83,96,-1,7,0,0);
	set_server_sector_info(i,4,98,-1,0,-381,70,48,47,97,-1,-1,7,0,0);
	set_server_sector_info(i,4,99,-1,0,-373,-75,100,45,84,-1,-1,7,0,0);
	set_server_sector_info(i,4,100,-1,0,-310,-69,45,99,-1,-1,-1,7,0,0);
	set_server_sector_info(i,4,101,-1,0,-493,-73,-1,-1,-1,-1,-1,0,1,0);
	set_server_sector_info(i,4,102,-1,0,-685,-475,110,75,103,-1,-1,8,0,0);
	set_server_sector_info(i,4,103,-1,0,-625,-475,102,109,71,-1,-1,8,0,0);
	set_server_sector_info(i,4,104,-1,0,-515,-475,69,107,105,-1,-1,8,0,0);
	set_server_sector_info(i,4,105,-1,0,-455,-475,106,104,67,-1,-1,8,0,0);
	set_server_sector_info(i,4,106,-1,0,-455,-530,108,107,105,-1,-1,8,0,0);
	set_server_sector_info(i,4,107,-1,0,-515,-530,108,111,104,106,-1,8,0,0);
	set_server_sector_info(i,4,108,-1,0,-515,-595,112,113,107,109,106,8,0,0);
	set_server_sector_info(i,4,109,-1,0,-625,-530,108,111,110,103,-1,8,0,0);
	set_server_sector_info(i,4,110,-1,0,-685,-530,111,109,102,-1,-1,8,0,0);
	set_server_sector_info(i,4,111,-1,0,-625,-595,112,113,107,109,110,8,0,0);
	set_server_sector_info(i,4,112,-1,0,-625,-655,114,113,108,111,-1,8,0,0);
	set_server_sector_info(i,4,113,-1,0,-515,-655,112,115,108,111,-1,8,0,0);
	set_server_sector_info(i,4,114,-1,0,-680,-715,117,112,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,115,-1,0,-460,-715,116,113,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,116,-1,0,-460,-775,119,124,115,-1,-1,8,0,0);
	set_server_sector_info(i,4,117,-1,0,-680,-775,118,123,114,-1,-1,8,0,0);
	set_server_sector_info(i,4,118,-1,0,-680,-835,122,117,121,-1,-1,8,0,0);
	set_server_sector_info(i,4,119,-1,0,-460,-835,125,120,116,-1,-1,8,0,0);
	set_server_sector_info(i,4,120,-1,0,-515,-895,119,121,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,121,-1,0,-625,-895,118,120,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,122,-1,0,-620,-815,126,118,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,123,-1,0,-620,-735,126,117,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,124,-1,0,-520,-735,126,116,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,125,-1,0,-520,-815,126,119,-1,-1,-1,8,0,0);
	set_server_sector_info(i,4,126,32,0,-570,-775,123,124,125,122,-1,8,0,0);
	
	printf("database created- purpose:galaxy\n");
}

void create_ship_db()
{
	int i = 0;
	int k=0;
	
	strcpy(ship[k++].name,"Zephyr");
	set_server_ship_info(i,0,0,4000,0,120,40,45,18,0,5,1,1,2);
	set_server_ship_info(i,0,1,9000,0,175,45,40,16,0,7,2,1,3);
	set_server_ship_info(i,0,2,8000,0,230,45,35,10,0,70,1,1,3);
	set_server_ship_info(i,0,3,12000,0,300,50,30,14,0,10,2,2,4);
	set_server_ship_info(i,0,4,25000,0,380,55,20,8,0,140,3,2,5);
	set_server_ship_info(i,0,5,60000,10,480,60,5,12,1,20,4,3,5);
	set_server_ship_info(i,0,6,70000,15,600,65,10,10,1,30,5,4,6);
	set_server_ship_info(i,0,7,310000,50,750,70,5,8,2,45,7,6,7);
	//
	strcpy(ship[k++].name,"Arachne");
	set_server_ship_info(i,1,0,14000,0,100,40,50,20,0,7,2,1,2);
	set_server_ship_info(i,1,1,49000,0,150,45,45,18,0,10,3,1,3);
	set_server_ship_info(i,1,2,19000,0,200,45,40,12,0,100,2,1,3);
	set_server_ship_info(i,1,3,78000,5,260,50,35,16,1,15,3,2,4);
	set_server_ship_info(i,1,4,105000,10,320,50,25,12,1,200,4,2,5);
	set_server_ship_info(i,1,5,100000,10,400,55,20,16,1,20,5,3,5);
	set_server_ship_info(i,1,6,260000,25,500,60,15,14,2,35,6,4,6);
	set_server_ship_info(i,1,7,465000,50,600,65,10,12,2,45,8,5,7);
	//
	strcpy(ship[k++].name,"Nisus");
	set_server_ship_info(i,2,0,30000,0,200,50,50,16,1,10,2,2,2);
	set_server_ship_info(i,2,1,32000,0,320,55,45,12,1,15,2,3,3);
	set_server_ship_info(i,2,2,75000,5,450,55,40,12,1,120,3,4,3);
	set_server_ship_info(i,2,3,380000,35,750,60,35,12,2,20,4,4,4);
	set_server_ship_info(i,2,4,450000,45,980,70,25,10,2,240,5,5,5);
	set_server_ship_info(i,2,5,545000,55,1200,75,20,3,3,30,6,8,5);
	set_server_ship_info(i,2,6,880000,90,1400,80,15,3,3,40,8,9,6);
	set_server_ship_info(i,2,7,1500000,150,2000,85,10,3,3,50,10,12,7);
	//
	strcpy(ship[k++].name,"Talos");
	set_server_ship_info(i,3,0,68000,5,180,45,50,16,2,12,2,1,2);
	set_server_ship_info(i,3,1,85000,5,290,50,45,12,2,17,3,2,3);
	set_server_ship_info(i,3,2,175000,15,400,50,40,12,3,125,4,2,3);
	set_server_ship_info(i,3,3,280000,30,600,55,35,10,3,25,4,3,4);
	set_server_ship_info(i,3,4,572000,55,700,60,25,10,4,240,6,4,5);
	set_server_ship_info(i,3,5,1078000,100,900,60,20,10,4,30,7,5,6);
	set_server_ship_info(i,3,6,1800000,180,1200,65,15,8,5,35,10,6,7);
	set_server_ship_info(i,3,7,3250000,320,1600,70,10,6,5,40,12,8,8);
	//cost, exp, hull, shield, evasion, speed, range, cargo, guns, missiles, miscs
	strcpy(ship[k++].name,"Nereid");
	set_server_ship_info(i,4,0,290000,30,300,55,50,16,3,20,4,2,3);
	set_server_ship_info(i,4,1,230600,20,450,60,45,12,3,30,4,2,4);
	set_server_ship_info(i,4,2,400000,40,600,60,40,12,3,200,4,2,4);
	set_server_ship_info(i,4,3,944000,90,800,65,40,10,4,40,6,3,5);
	set_server_ship_info(i,4,4,1500000,150,1000,75,25,10,5,400,7,4,6);
	set_server_ship_info(i,4,5,2880000,280,1300,70,30,10,6,50,8,6,7);
	set_server_ship_info(i,4,6,4700000,470,1600,75,20,8,7,60,11,8,8);
	set_server_ship_info(i,4,7,7314000,730,2000,80,15,6,7,100,13,10,9);
	//
	strcpy(ship[k++].name,"Endymion");
	set_server_ship_info(i,5,0,230000,20,500,55,45,12,4,15,4,2,2);
	set_server_ship_info(i,5,1,620000,60,800,60,40,10,4,20,5,3,5);
	set_server_ship_info(i,5,2,720000,70,1000,60,35,8,4,150,5,3,5);
	set_server_ship_info(i,5,3,2030000,200,1500,70,30,8,5,30,7,5,6);
	set_server_ship_info(i,5,4,2605000,260,1800,75,25,6,5,320,8,5,7);
	set_server_ship_info(i,5,5,4950000,490,2000,75,20,6,6,40,10,8,8);
	set_server_ship_info(i,5,6,7650000,760,2400,80,15,4,7,50,12,10,9);
	set_server_ship_info(i,5,7,12880000,1300,3000,85,10,2,8,70,14,12,10);
	//cost, exp, hull, shield, evasion, speed, range, cargo, guns, missiles, miscs
	strcpy(ship[k++].name,"Helios");
	set_server_ship_info(i,6,0,1163000,115,700,40,40,16,6,18,6,3,5);
	set_server_ship_info(i,6,1,2200000,220,1000,45,35,12,6,25,7,3,6);
	set_server_ship_info(i,6,2,1122000,110,1200,80,25,8,6,180,7,3,6);
	set_server_ship_info(i,6,3,5400000,540,1800,50,30,7,10,35,8,5,7);
	set_server_ship_info(i,6,4,4600000,460,2200,85,15,8,10,380,8,5,5);
	set_server_ship_info(i,6,5,16550000,1650,2400,60,25,9,11,45,11,9,9);
	set_server_ship_info(i,6,6,19700000,2000,3200,70,15,9,11,65,14,12,10);
	set_server_ship_info(i,6,7,26000000,2600,3600,80,10,9,11,90,16,14,11);
	//cost, exp, hull, shield, evasion, speed, range, cargo, guns, missiles, miscs
	strcpy(ship[k++].name,"Erinyes");
	set_server_ship_info(i,7,0,500000,600,1200,70,35,18,8,200,6,4,4);
	set_server_ship_info(i,7,1,800000,800,1300,70,30,16,8,180,7,5,5);
	set_server_ship_info(i,7,2,700000,1400,1500,60,20,12,8,500,8,8,6);
	set_server_ship_info(i,7,3,1200000,1200,1800,80,30,14,9,240,9,7,8);
	set_server_ship_info(i,7,4,2400000,2000,2000,60,20,10,10,800,10,10,8);
	set_server_ship_info(i,7,5,14000000,1600,2400,85,20,14,11,300,14,8,8);
	set_server_ship_info(i,7,6,20000000,1800,3000,90,10,12,11,320,16,10,8);
	set_server_ship_info(i,7,7,26000000,2000,3800,90,5,10,11,400,16,12,8);
	//cost, exp, hull, shield, evasion, speed, range, cargo, guns, missiles, miscs
	strcpy(ship[k++].name,"Andros"); //8
	set_server_ship_info(i,8,0,400000,1200,200,50,60,24,11,20,10,2,5);
	set_server_ship_info(i,8,1,600000,1400,400,50,55,22,11,20,12,3,6);
	set_server_ship_info(i,8,2,5000000,2000,800,40,30,18,11,800,6,12,8);
	set_server_ship_info(i,8,3,1200000,1600,700,55,50,20,11,10,14,4,10);
	set_server_ship_info(i,8,4,8000000,2400,1200,40,30,16,11,1200,6,16,10);
	set_server_ship_info(i,8,5,8000000,1800,1200,60,40,9,11,30,16,6,12);
	set_server_ship_info(i,8,6,12000000,2000,1400,70,30,9,11,40,16,8,14);
	set_server_ship_info(i,8,7,16000000,2800,1800,80,20,9,11,120,16,10,16);
	
	printf("database created- purpose:ship\n");
}

void create_gun_db()
{
	int i = 0;
	//set_server_gun_info(int i, int k, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type)
	set_server_gun_info(i,0,"PLS Beam",700,5,80,50,0,0,0,0);
	set_server_gun_info(i,1,"RPD Beam",800,5,85,50,0,0,0,0);
	set_server_gun_info(i,2,"Ion Stream",1000,5,80,60,0,0,0,4);
	set_server_gun_info(i,3,"V1 Blaster",7000,10,70,70,0,0,0,5);
	set_server_gun_info(i,4,"K7 Cannon",3000,5,40,90,0,0,0,3);
	set_server_gun_info(i,5,"MK1 Fury",12000,10,80,70,5,0,0,0);
	set_server_gun_info(i,6,"K14 Cannon",17000,10,40,90,5,0,0,3);
	set_server_gun_info(i,7,"Drive Warper",100,5,60,60,0,200,0,1);
	set_server_gun_info(i,8,"Lance Ray",19000,20,80,60,20,0,0,4);
	set_server_gun_info(i,9,"V2 Blaster",19000,20,70,70,20,0,0,5);
	set_server_gun_info(i,10,"MK2 Fury",21000,15,80,70,50,0,0,0);
	set_server_gun_info(i,11,"MK1 Sunburst",28000,25,60,70,80,5,0,2);
	set_server_gun_info(i,12,"SilverStorm I",34000,40,70,50,100,10,0,3);
	set_server_gun_info(i,13,"Razor Ray",31000,25,85,60,120,20,0,4);
	set_server_gun_info(i,14,"MK3 Fury",39000,25,80,70,200,50,0,0);
	set_server_gun_info(i,15,"MK2 Sunburst",37000,35,60,70,300,80,0,2);
	set_server_gun_info(i,16,"Xi Silenus",43000,45,82,50,400,100,0,0);
	set_server_gun_info(i,17,"SilverStorm II",52000,55,65,50,600,150,0,3);
	set_server_gun_info(i,18,"Xen Silenus",68000,60,82,50,800,200,0,0);
	
	printf("database created- purpose:gun\n");
}

void create_bot_db()
{
	int i, j, k, s;
	
	//clear out all bot sectors...
	for(i=0;i<BOT_TYPE_MAX;i++)
		for(k=0;k<SECTOR_MAX;k++)
			bot_type[i].sector[k] = 0;
	
	i = 0;
	set_server_bot_type_info(s,0,-1,"Militia","Militia",1,1,0,1,0);
	for(k=0;k<31;k++)
		bot_type[i].sector[k] = 1;
	
	k=0;
	set_server_bot_info(s,0,k++,"Col Karter",5,6,120,2,400);
	set_server_bot_info(s,0,k++,"Col Darus",5,6,120,2,400);
	
	set_server_bot_info(s,0,k++,"Mjr Vine",5,5,100,2,300);
	set_server_bot_info(s,0,k++,"Mjr Plin",5,5,100,2,300);
	set_server_bot_info(s,0,k++,"Mjr Greeg",5,5,100,2,300);
	set_server_bot_info(s,0,k++,"Mjr Felson",5,5,100,2,300);
	set_server_bot_info(s,0,k++,"Mjr Drinn",5,5,100,2,300);
	
	set_server_bot_info(s,0,k++,"Lt Skeet",5,3,70,2,100);
	set_server_bot_info(s,0,k++,"Lt Price",5,3,70,2,100);
	set_server_bot_info(s,0,k++,"Lt Kray",5,3,70,2,100);
	set_server_bot_info(s,0,k++,"Lt Grin",5,3,70,2,100);
	set_server_bot_info(s,0,k++,"Lt Gido",5,3,70,2,100);
	set_server_bot_info(s,0,k++,"Lt Dan",5,3,70,2,100);
	
	set_server_bot_info(s,0,k++,"Pvt Vygen",5,1,50,2,50);
	set_server_bot_info(s,0,k++,"Pvt Klipe",5,1,50,2,50);
	set_server_bot_info(s,0,k++,"Pvt Inyan",5,1,50,2,50);
	set_server_bot_info(s,0,k++,"Pvt Groug",5,1,50,2,50);
	set_server_bot_info(s,0,k++,"Pvt Dunn",5,1,50,2,50);
	
	set_server_bot_type_info(s,1,-1,"Klyn","Klyn",2,2,0,1,0);
	
	i=1;
	for(k=0;k<=114;k++)
		bot_type[i].sector[k] = 1;
	
	k=0;
	set_server_bot_info(s,1,k++,"Crinlib",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Fargul",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Greeno",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Kin Sing",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Koodar",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Meegin",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Pootar",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Zinpo Brue",0,0,10,0.5,20);
	set_server_bot_info(s,1,k++,"Zwillin",0,0,10,0.5,20);
	
	set_server_bot_info(s,1,k++,"Brynch",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Dansus",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Fliggit",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Gillin",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Grin Chou",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Keelos",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Pintok",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Tevlar",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Quaze",1,1,20,0.5,50);
	set_server_bot_info(s,1,k++,"Wishgin",1,1,20,0.5,50);
	
	set_server_bot_info(s,1,k++,"Cowgin Jay",1,3,30,0.5,70);
	set_server_bot_info(s,1,k++,"Kraygol",1,3,30,0.5,70);
	set_server_bot_info(s,1,k++,"Vaycrant",1,3,30,0.5,70);
	set_server_bot_info(s,1,k++,"Visik",1,3,30,0.5,70);
	
	set_server_bot_info(s,1,k++,"Dinsow Ca",3,1,30,0.5,80);
	set_server_bot_info(s,1,k++,"Tweel",3,1,30,0.5,80);
	set_server_bot_info(s,1,k++,"Versi",3,1,30,0.5,80);
	set_server_bot_info(s,1,k++,"Weego",3,1,30,0.5,80);
	
	i=2;
	set_server_bot_type_info(s,2,-1,"Rin","Rin",2,3,1,1,0);
	bot_type[i].sector[32] = 1;
	bot_type[i].sector[31] = 1;
	bot_type[i].sector[33] = 1;
	bot_type[i].sector[34] = 1;
	bot_type[i].sector[35] = 1;
	bot_type[i].sector[36] = 1;
	bot_type[i].sector[37] = 1;
	bot_type[i].sector[38] = 1;
	bot_type[i].sector[39] = 1;
	bot_type[i].sector[40] = 1;
	bot_type[i].sector[53] = 1;
	bot_type[i].sector[54] = 1;
	bot_type[i].sector[57] = 1;
	
	k=0;
	set_server_bot_info(s,2,k++,"Ri Beedo",3,6,50,1,400);
	set_server_bot_info(s,2,k++,"Ri Bindelo",3,6,50,1,400);
	
	set_server_bot_info(s,2,k++,"Ri Caliban",3,5,35,1,350);
	set_server_bot_info(s,2,k++,"Ri Faucan",3,5,35,1,350);
	set_server_bot_info(s,2,k++,"Ri Feste",3,5,35,1,350);
	
	set_server_bot_info(s,2,k++,"Ri Foogo",3,3,20,1,250);
	set_server_bot_info(s,2,k++,"Ri Jay Ko",3,3,20,1,250);
	set_server_bot_info(s,2,k++,"Ri Gremio",3,3,20,1,250);
	set_server_bot_info(s,2,k++,"Ri Joka",3,3,20,1,250);
	
	set_server_bot_info(s,2,k++,"Ri Kado",3,1,15,1,200);
	set_server_bot_info(s,2,k++,"Ri Kudog",3,1,15,1,200);
	set_server_bot_info(s,2,k++,"Ri Ky Din",3,1,15,1,200);
	set_server_bot_info(s,2,k++,"Ri Ny Godar",3,1,15,1,200);

	set_server_bot_info(s,2,k++,"Ri Nyin",3,0,10,1,150);
	set_server_bot_info(s,2,k++,"Ri Nyphor",3,0,10,1,150);
	set_server_bot_info(s,2,k++,"Ri Siward",3,0,10,1,150);
	set_server_bot_info(s,2,k++,"Ri Trinculo",3,0,10,1,150);
	set_server_bot_info(s,2,k++,"Ri Tyvio",3,0,10,1,150);
	set_server_bot_info(s,2,k++,"Ri Veeg",3,0,10,1,150);
	
	
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	i=3;
	set_server_bot_type_info(s,i,-1,"Asteroid","Asteroid",0,0,1,1,1);
	bot_type[i].sector[43] = 1;
	bot_type[i].sector[42] = 1;
	bot_type[i].sector[48] = 1;
	bot_type[i].sector[74] = 1;
	bot_type[i].sector[75] = 1;
	bot_type[i].sector[47] = 1;
	bot_type[i].sector[76] = 1;
	bot_type[i].sector[78] = 1;
	bot_type[i].sector[79] = 1;
	bot_type[i].sector[77] = 1;
	bot_type[i].sector[46] = 1;
	bot_type[i].sector[49] = 1;
	bot_type[i].sector[50] = 1;
	bot_type[i].sector[84] = 1;
	bot_type[i].sector[85] = 1;
	bot_type[i].sector[82] = 1;
	bot_type[i].sector[83] = 1;
	bot_type[i].sector[81] = 1;
	bot_type[i].sector[80] = 1;
	bot_type[i].sector[86] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi)
	k=0;
	set_server_bot_info(s,i,k++,"Asteroid 1",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 2",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 3",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 4",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 5",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 6",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 7",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 8",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 9",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 10",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 11",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 12",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 13",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 14",4,1,0,1,50);
	set_server_bot_info(s,i,k++,"Asteroid 15",4,1,0,1,50);
	
	i=4;
	set_server_bot_type_info(s,i,-1,"Miner","Miner",1,4,1,1,0);
	bot_type[i].sector[43] = 1;
	bot_type[i].sector[42] = 1;
	bot_type[i].sector[48] = 1;
	bot_type[i].sector[74] = 1;
	bot_type[i].sector[75] = 1;
	bot_type[i].sector[47] = 1;
	bot_type[i].sector[76] = 1;
	bot_type[i].sector[78] = 1;
	bot_type[i].sector[79] = 1;
	bot_type[i].sector[77] = 1;
	bot_type[i].sector[46] = 1;
	bot_type[i].sector[49] = 1;
	bot_type[i].sector[50] = 1;
	bot_type[i].sector[84] = 1;
	bot_type[i].sector[85] = 1;
	bot_type[i].sector[82] = 1;
	bot_type[i].sector[83] = 1;
	bot_type[i].sector[81] = 1;
	bot_type[i].sector[80] = 1;
	bot_type[i].sector[86] = 1;
	
	k=0;
	set_server_bot_info(s,i,k++,"Ki Zar",4,6,60,0.2,300);
	set_server_bot_info(s,i,k++,"Ki Fet",4,6,60,0.2,300);
	
	set_server_bot_info(s,i,k++,"Ki Qune",4,5,50,0.2,280);
	set_server_bot_info(s,i,k++,"Ki Cado",4,5,50,0.2,280);
	set_server_bot_info(s,i,k++,"Ki Fic",4,5,50,0.2,280);
	
	set_server_bot_info(s,i,k++,"Ki Hoor",4,4,45,0.2,270);
	
	set_server_bot_info(s,i,k++,"Ki Nani",4,3,40,0.2,250);
	set_server_bot_info(s,i,k++,"Ki Aldrin",4,3,40,0.2,250);
	set_server_bot_info(s,i,k++,"Ki Jace",4,3,40,0.2,250);
	set_server_bot_info(s,i,k++,"Ki Goot",4,3,40,0.2,250);
	
	set_server_bot_info(s,i,k++,"Ki Tore",4,1,30,0.2,220);
	set_server_bot_info(s,i,k++,"Ki Jortus",4,1,30,0.2,220);
	set_server_bot_info(s,i,k++,"Ki Cru",4,1,30,0.2,220);
	set_server_bot_info(s,i,k++,"Ki Loon",4,1,30,0.2,220);
	set_server_bot_info(s,i,k++,"Ki Cooper",4,1,30,0.2,220);
	
	set_server_bot_info(s,i,k++,"Ki Konee",4,0,25,0.2,200);
	set_server_bot_info(s,i,k++,"Ki Heet",4,0,25,0.2,200);
	set_server_bot_info(s,i,k++,"Ki Isacc",4,0,25,0.2,200);
	set_server_bot_info(s,i,k++,"Ki Yeager",4,0,25,0.2,200);
	set_server_bot_info(s,i,k++,"Ki Lootus",4,0,25,0.2,200);
	set_server_bot_info(s,i,k++,"Ki Weet",4,0,25,0.2,200);
	
	i=5;
	set_server_bot_type_info(s,i,-1,"Gen Raider","Gen Raider",2,2,0,0,0);
	for(j=14;j<=42;j++)
		bot_type[i].sector[j] = 1;

	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"KinTok",0,6,25,0.1,70);
	set_server_bot_info(s,i,k++,"Zenik",0,6,25,0.1,70);
	set_server_bot_info(s,i,k++,"Hclon",0,6,25,0.1,70);
	set_server_bot_info(s,i,k++,"Trechil",0,6,25,0.1,70);
	
	set_server_bot_info(s,i,k++,"Yin Sok",0,5,20,0.1,60);
	set_server_bot_info(s,i,k++,"Veenik",0,5,20,0.1,60);
	set_server_bot_info(s,i,k++,"Guo Fin",0,5,20,0.1,60);
	set_server_bot_info(s,i,k++,"Vonnille",0,5,20,0.1,60);
	set_server_bot_info(s,i,k++,"Tkeloy",0,5,20,0.1,60);
	
	set_server_bot_info(s,i,k++,"Sinik",0,3,10,0.1,40);
	set_server_bot_info(s,i,k++,"Kez",0,3,10,0.1,40);
	set_server_bot_info(s,i,k++,"Kin Vine",0,3,10,0.1,40);
	set_server_bot_info(s,i,k++,"Fi Trol",0,3,10,0.1,40);
	set_server_bot_info(s,i,k++,"Pontis",0,3,10,0.1,40);
	set_server_bot_info(s,i,k++,"Ewntice",0,3,10,0.1,40);
	
	set_server_bot_info(s,i,k++,"Vync",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Pollic",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Xezic",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Jyke",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Seciz",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Tonis",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Twenol",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Chyr",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Fayr",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Dentroy",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Dar",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Heatch",0,1,10,0.1,30);
	set_server_bot_info(s,i,k++,"Peeh",0,1,10,0.1,30);
	
	set_server_bot_info(s,i,k++,"Wene Tey",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Trollic",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Potic",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Nish",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Lok Tyr",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Decit",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Harst",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Jenst",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Jarn",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Robet",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Shek",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Fendle",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Pohchi",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Stewndor",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Divtr",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Dhoone",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Birn",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Hyat",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Lerr",0,0,5,0.1,10);
	set_server_bot_info(s,i,k++,"Moul",0,0,5,0.1,10);
	
	i=6; //void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	set_server_bot_type_info(s,i,-1,"Gen Crusader","Gen Crusader",1,1,1,0,0);
	for(j=0;j<=16;j++)
		bot_type[i].sector[j] = 1;
	bot_type[i].sector[26] = 1;
	bot_type[i].sector[32] = 1;
	bot_type[i].sector[34] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Genix Thal",1,5,50,2,70);
	set_server_bot_info(s,i,k++,"Genix Blansek",1,5,50,2,70);
	set_server_bot_info(s,i,k++,"Genix Blansek",1,5,50,2,70);
	
	set_server_bot_info(s,i,k++,"Genix Thur",1,3,50,2,70);
	set_server_bot_info(s,i,k++,"Genix Sval",1,3,50,2,70);
	set_server_bot_info(s,i,k++,"Genix Vrek",1,3,50,2,70);
	set_server_bot_info(s,i,k++,"Genix Stonzot",1,3,50,2,70);
	set_server_bot_info(s,i,k++,"Genix Loming",1,3,50,2,70);
	
	set_server_bot_info(s,i,k++,"Genix Quin",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Pol",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Prin",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Olus",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Ply",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Kel",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Gris",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Lozet",1,1,30,2,40);
	set_server_bot_info(s,i,k++,"Genix Confo",1,1,30,2,40);
	
	set_server_bot_info(s,i,k++,"Genix Drek",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Axer",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Devon",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Flax",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Cal",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Cado",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Stenke",1,0,20,1,20);
	set_server_bot_info(s,i,k++,"Genix Tiro",1,0,20,1,20);
	
	i=7;
	set_server_bot_type_info(s,i,-1,"Pirate","Pirate",2,3,0,2,0);
	for(j=0;j<=28;j++)
		bot_type[i].sector[j] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Khresh",0,3,10,1,40);
	set_server_bot_info(s,i,k++,"Lark",0,3,10,1,40);
	
	set_server_bot_info(s,i,k++,"Vololo",0,1,10,1,30);
	set_server_bot_info(s,i,k++,"Sowmis",0,1,10,1,30);
	set_server_bot_info(s,i,k++,"Vohm",0,1,10,1,30);
	set_server_bot_info(s,i,k++,"Jitu",0,1,10,1,30);
	
	set_server_bot_info(s,i,k++,"Lich",0,0,5,1,10);
	set_server_bot_info(s,i,k++,"Vasz",0,0,5,1,10);
	set_server_bot_info(s,i,k++,"Jahon",0,0,5,1,10);
	set_server_bot_info(s,i,k++,"Jenst",0,0,5,1,10);
	set_server_bot_info(s,i,k++,"Dorne",0,0,5,1,10);
	set_server_bot_info(s,i,k++,"Loornace",0,0,5,1,10);
	/*
	i=8;
	set_server_bot_type_info(s,i,-1,"Rogue","Rogue",0,2,0,0,0);
	for(j=98;j<146;j++)
	bot_type[i].sector[j] = 1;
	    
	k=0;
	set_server_bot_info(s,i,k++,"Vhento",0,0,15,1,25);
	set_server_bot_info(s,i,k++,"Foele",0,0,15,1,25);
	set_server_bot_info(s,i,k++,"Smik",0,0,15,1,25);
	set_server_bot_info(s,i,k++,"Porto",0,0,15,1,25);
	set_server_bot_info(s,i,k++,"Thren",0,0,15,1,25);
	
	set_server_bot_info(s,i,k++,"Vynic",0,1,30,1,50);
	set_server_bot_info(s,i,k++,"Foon",0,1,30,1,50);
	set_server_bot_info(s,i,k++,"Donle",0,1,30,1,50);
	set_server_bot_info(s,i,k++,"Singe",0,1,30,1,50);
	
	set_server_bot_info(s,i,k++,"Voc",1,0,30,1,50);
	set_server_bot_info(s,i,k++,"Hwois",1,0,30,1,50);
	set_server_bot_info(s,i,k++,"Chinda",1,0,30,1,50);
	set_server_bot_info(s,i,k++,"Abhi",1,0,30,1,50);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	i=9;
	set_server_bot_type_info(s,i,-1,"Velic Asteroid","Asteroid",0,0,1,2,1);
	for(j=0;j<=28;j++)
		bot_type[i].sector[j] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi)
	k=0;
	set_server_bot_info(s,i,k++,"Velic Asteroid 1",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 2",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 3",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 4",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 5",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 6",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 7",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 8",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 9",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 10",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 11",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 12",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 13",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 14",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 15",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 16",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 17",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 18",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 19",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 20",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 21",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 22",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 23",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 24",0,1,0,0.1,10);
	set_server_bot_info(s,i,k++,"Velic Asteroid 25",0,1,0,0.1,10);
	/*
	i=10;
	set_server_bot_type_info(s,i,-1,"Li Sentry","Li Sentry",0,3,0,0,0);
	bot_type[i].sector[75] = 1;
	bot_type[i].sector[151] = 1;
	bot_type[i].sector[152] = 1;
	bot_type[i].sector[153] = 1;
	bot_type[i].sector[146] = 1;
	bot_type[i].sector[147] = 1;
	bot_type[i].sector[148] = 1;
	bot_type[i].sector[154] = 1;
	bot_type[i].sector[149] = 1;
	bot_type[i].sector[157] = 1;
	bot_type[i].sector[158] = 1;
	bot_type[i].sector[192] = 1;
	bot_type[i].sector[150] = 1;
	
	set_server_bot_info(s,i,k++,"Lex Edher",2,0,20,1,30);
	set_server_bot_info(s,i,k++,"Lex Pense",2,0,20,1,30);
	set_server_bot_info(s,i,k++,"Lex Goohte",2,0,20,1,30);
	set_server_bot_info(s,i,k++,"Lex Jish",2,0,20,1,30);
	set_server_bot_info(s,i,k++,"Lex Klenj",2,0,20,1,30);
	
	set_server_bot_info(s,i,k++,"Lex Vonz",2,1,20,1,40);
	set_server_bot_info(s,i,k++,"Lex Ehto",2,1,20,1,40);
	set_server_bot_info(s,i,k++,"Lex Brone",2,1,20,1,40);
	set_server_bot_info(s,i,k++,"Lex Bost",2,1,20,1,40);
	
	set_server_bot_info(s,i,k++,"Lex Bzanl",2,3,40,1,70);
	set_server_bot_info(s,i,k++,"Lex Jentor",2,3,40,1,70);
	set_server_bot_info(s,i,k++,"Lex Foonle",2,3,40,1,70);
	*/
	i=11;
	set_server_bot_type_info(s,i,-1,"Sentinel","Sentinel",1,1,0,0,0);
	//for(j=0;j<36;j++)
	    //bot_type[i].sector[j] = 1;
	bot_type[i].sector[26] = 1;
	bot_type[i].sector[32] = 1;
	bot_type[i].sector[31] = 1;
	bot_type[i].sector[30] = 1;
	bot_type[i].sector[29] = 1;
	bot_type[i].sector[41] = 1;
	bot_type[i].sector[42] = 1;
	
	bot_type[i].sector[33] = 1;
	bot_type[i].sector[37] = 1;
	bot_type[i].sector[38] = 1;
	bot_type[i].sector[39] = 1;
	bot_type[i].sector[40] = 1;
	
	bot_type[i].sector[10] = 1;
	bot_type[i].sector[14] = 1;
	bot_type[i].sector[15] = 1;
	bot_type[i].sector[0] = 1;
	bot_type[i].sector[16] = 1;
	bot_type[i].sector[4] = 1;
	
	bot_type[i].sector[17] = 1;
	bot_type[i].sector[18] = 1;
	bot_type[i].sector[19] = 1;
	bot_type[i].sector[20] = 1;
	
	set_server_bot_info(s,i,k++,"Sentinel 1",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 2",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 3",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 4",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 5",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 6",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 7",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 8",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 9",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 10",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 11",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 12",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 13",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 14",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 15",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 16",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 17",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 18",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 19",2,7,200,2,80);
	set_server_bot_info(s,i,k++,"Sentinel 20",2,7,200,2,80);
	
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=12;
	set_server_bot_type_info(s,i,-1,"Sentry Guard","Sentry Guard",2,2,0,3,0);
	for(j=31;j<42;j++)
	bot_type[i].sector[j] = 1;
	bot_type[i].sector[61] = 1;
	bot_type[i].sector[57] = 1;
	bot_type[i].sector[53] = 1;
	bot_type[i].sector[54] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Gen Kohl",2,7,200,1,500);
	
	set_server_bot_info(s,i,k++,"Col Vasto",2,6,160,1,400);
	set_server_bot_info(s,i,k++,"Col Glacius",2,6,160,1,400);
	
	set_server_bot_info(s,i,k++,"Mjr Talon",2,5,120,1,300);
	set_server_bot_info(s,i,k++,"Mjr Kale",2,5,120,1,300);
	set_server_bot_info(s,i,k++,"Mjr Pak",2,5,120,1,300);
	
	set_server_bot_info(s,i,k++,"Lt Aries",2,3,100,1,250);
	set_server_bot_info(s,i,k++,"Lt Lita",2,3,100,1,250);
	set_server_bot_info(s,i,k++,"Lt Vit",2,3,100,1,250);
	set_server_bot_info(s,i,k++,"Lt Baffon",2,3,100,1,250);
	
	set_server_bot_info(s,i,k++,"Pvt Lead",2,1,40,1,200);
	set_server_bot_info(s,i,k++,"Pvt Mamoth",2,1,40,1,200);
	set_server_bot_info(s,i,k++,"Pvt Stariff",2,1,40,1,200);
	set_server_bot_info(s,i,k++,"Pvt Kag",2,1,40,1,200);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	i=13;
	set_server_bot_type_info(s,i,-1,"High Council","High Council",1,4,0,1,0);
	for(j=62;j<=70;j++)
		bot_type[i].sector[j] = 1;
	bot_type[i].sector[55] = 1;
	bot_type[i].sector[56] = 1;
	bot_type[i].sector[60] = 1;
	bot_type[i].sector[95] = 1;
	bot_type[i].sector[96] = 1;
	bot_type[i].sector[97] = 1;
	bot_type[i].sector[100] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Gen Norman",5,7,180,0.8,1600);
	set_server_bot_info(s,i,k++,"Gen Mandable ",5,7,180,0.8,1600);
	
	set_server_bot_info(s,i,k++,"Col Artemis",5,6,150,0.8,1500);
	set_server_bot_info(s,i,k++,"Col Fudd",5,6,150,0.8,1500);
	set_server_bot_info(s,i,k++,"Col Meed",5,6,150,0.8,1500);
	
	set_server_bot_info(s,i,k++,"Mjr Kang",5,5,100,0.8,1250);
	set_server_bot_info(s,i,k++,"Mjr Kalun",5,5,100,0.8,1250);
	set_server_bot_info(s,i,k++,"Mjr Kaput",5,5,100,0.8,1250);
	set_server_bot_info(s,i,k++,"Mjr Kleetus",5,5,100,0.8,1250);
	
	set_server_bot_info(s,i,k++,"Lt Zeleno",5,3,40,0.8,1000);
	set_server_bot_info(s,i,k++,"Lt Kurcho",5,3,40,0.8,1000);
	set_server_bot_info(s,i,k++,"Lt Sedney",5,3,40,0.8,1000);
	set_server_bot_info(s,i,k++,"Lt Larf",5,3,40,0.8,1000);
	set_server_bot_info(s,i,k++,"Lt Tuppe",5,3,40,0.8,1000);
	set_server_bot_info(s,i,k++,"Lt Purt",5,3,40,0.8,1000);
	
	set_server_bot_info(s,i,k++,"Pvt Jung",5,3,20,0.8,800);
	set_server_bot_info(s,i,k++,"Pvt Bog",5,3,20,0.8,800);
	set_server_bot_info(s,i,k++,"Pvt Wirm",5,3,20,0.8,800);
	set_server_bot_info(s,i,k++,"Pvt Burt",5,3,20,0.8,800);
	set_server_bot_info(s,i,k++,"Pvt Kunos",5,3,20,0.8,800);
	set_server_bot_info(s,i,k++,"Pvt Marmar",5,3,20,0.8,800);
	set_server_bot_info(s,i,k++,"Pvt Pewt",5,3,20,0.8,800);
	
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=14;
	set_server_bot_type_info(s,i,-1,"Nuet Klyn","Klyn",2,3,0,3,0);
	for(j=0;j<193;j++)
	bot_type[i].sector[j] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Fiu",0,3,40,1,200);
	set_server_bot_info(s,i,k++,"Jatil",0,3,40,1,200);
	set_server_bot_info(s,i,k++,"Joi",0,3,40,1,200);
	set_server_bot_info(s,i,k++,"Vuise",0,3,40,1,200);
	set_server_bot_info(s,i,k++,"Nat",0,3,40,1,200);
	set_server_bot_info(s,i,k++,"Hios",0,3,40,1,200);
	
	set_server_bot_info(s,i,k++,"Jin",0,7,140,1,250);
	set_server_bot_info(s,i,k++,"Mef",0,7,140,1,250);
	set_server_bot_info(s,i,k++,"Triogn",0,7,140,1,250);
	set_server_bot_info(s,i,k++,"Sufi",0,7,140,1,250);
	
	set_server_bot_info(s,i,k++,"Mew",1,6,120,1,250);
	set_server_bot_info(s,i,k++,"Weiuwt",1,6,120,1,250);
	set_server_bot_info(s,i,k++,"Ios",1,6,120,1,250);
	set_server_bot_info(s,i,k++,"Ni",1,6,120,1,250);
	set_server_bot_info(s,i,k++,"Yio",1,6,120,1,250);
	
	set_server_bot_info(s,i,k++,"Fiah",1,7,160,1,300);
	set_server_bot_info(s,i,k++,"Fuv",1,7,160,1,300);
	set_server_bot_info(s,i,k++,"Seaf",1,7,160,1,300);
	set_server_bot_info(s,i,k++,"Saj",1,7,160,1,300);
	
	set_server_bot_info(s,i,k++,"Nef",3,5,140,1,300);
	set_server_bot_info(s,i,k++,"Poc",3,5,140,1,300);
	set_server_bot_info(s,i,k++,"Jaserl",3,5,140,1,300);
	set_server_bot_info(s,i,k++,"Dio",3,5,140,1,300);
	
	set_server_bot_info(s,i,k++,"Mej",3,7,240,1,600);
	set_server_bot_info(s,i,k++,"Nois",3,7,240,1,600);
	set_server_bot_info(s,i,k++,"Sed",3,7,240,1,600);
	set_server_bot_info(s,i,k++,"Venef",3,7,240,1,600);
	set_server_bot_info(s,i,k++,"Foid",3,7,240,1,600);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=15;
	set_server_bot_type_info(s,i,-1,"Old Asteroid","Asteroid",0,0,1,3,1);
	for(j=44;j<51;j++)
	bot_type[i].sector[j] = 1;
	for(j=77;j<87;j++)
	bot_type[i].sector[j] = 1;
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi)
	k=0;
	set_server_bot_info(s,i,k++,"Old Asteroid 1",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 2",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 3",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 4",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 5",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 6",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 7",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 8",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 9",0,3,0,1,10);
	set_server_bot_info(s,i,k++,"Old Asteroid 10",0,3,0,1,10);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=16;
	set_server_bot_type_info(s,i,-1,"Marauder","Shadow",2,3,0,3,0);
	for(j=137;j<193;j++)
	bot_type[i].sector[j] = 1;
	    
	bot_type[i].sector[141] = 0;
	bot_type[i].sector[186] = 0;
	bot_type[i].sector[187] = 0;
	bot_type[i].sector[188] = 0;
	bot_type[i].sector[185] = 0;
	
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Mar Devv",8,6,160,1,1000);
	
	set_server_bot_info(s,i,k++,"Mar Nien",8,5,160,1,900);
	set_server_bot_info(s,i,k++,"Mar Denons",8,5,160,1,900);
	
	set_server_bot_info(s,i,k++,"Mar Jo Kyn",8,3,140,1,800);
	
	set_server_bot_info(s,i,k++,"Mar Synsth",8,1,120,1,700);
	set_server_bot_info(s,i,k++,"Mar Knift",8,1,120,1,700);
	
	set_server_bot_info(s,i,k++,"Mar Snick",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Nivv",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Koff",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Junn",8,0,100,1,600);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=17;
	set_server_bot_type_info(s,i,-1,"Banished Marauder","Shadow",2,3,0,4,0);
	for(j=0;j<=20;j++)
	bot_type[i].sector[j] = 1;
	
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Mar Folt",8,3,140,1,800);
	set_server_bot_info(s,i,k++,"Mar Passt",8,3,140,1,800);
	set_server_bot_info(s,i,k++,"Mar Diam",8,3,140,1,800);
	set_server_bot_info(s,i,k++,"Mar Fut",8,3,140,1,800);
	
	set_server_bot_info(s,i,k++,"Mar Kynsth",8,1,120,1,700);
	set_server_bot_info(s,i,k++,"Mar Wesz",8,1,120,1,700);
	set_server_bot_info(s,i,k++,"Mar Kon",8,1,120,1,700);
	set_server_bot_info(s,i,k++,"Mar Ank",8,1,120,1,700);
	set_server_bot_info(s,i,k++,"Mar Jonk",8,1,120,1,700);
	
	set_server_bot_info(s,i,k++,"Mar Nos",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Kenn",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Loss",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Nar",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Hin",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Noff",8,0,100,1,600);
	set_server_bot_info(s,i,k++,"Mar Nin",8,0,100,1,600);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	i=18;
	set_server_bot_type_info(s,i,-1,"Kaydin","Kaydin",2,3,0,1,0);
	for(j=101;j<=114;j++)
		bot_type[i].sector[j] = 1;
	for(j=87;j<=94;j++)
		bot_type[i].sector[j] = 1;
	bot_type[i].sector[51] = 1;
	bot_type[i].sector[52] = 1;
	bot_type[i].sector[71] = 1;
	bot_type[i].sector[72] = 1;
	bot_type[i].sector[73] = 1;
	bot_type[i].sector[98] = 1;
	bot_type[i].sector[99] = 1;
	
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Kay Tar",6,7,200,1,2000);
	
	set_server_bot_info(s,i,k++,"Kay Liy",6,6,160,1,1900);
	set_server_bot_info(s,i,k++,"Kay Zeef",6,6,160,1,1900);
	
	set_server_bot_info(s,i,k++,"Kay Fio",6,5,120,1,1700);
	set_server_bot_info(s,i,k++,"Kay Godin",6,5,120,1,1700);
	set_server_bot_info(s,i,k++,"Kay Rit",6,5,120,1,1700);
	set_server_bot_info(s,i,k++,"Kay Zil",6,5,120,1,1700);
	
	set_server_bot_info(s,i,k++,"Kay Fli",6,3,80,1,1600);
	set_server_bot_info(s,i,k++,"Kay Sin",6,3,80,1,1600);
	set_server_bot_info(s,i,k++,"Kay Drio",6,3,80,1,1600);
	set_server_bot_info(s,i,k++,"Kay Glow",6,3,80,1,1600);
	set_server_bot_info(s,i,k++,"Kay Trew",6,3,80,1,1600);
	
	set_server_bot_info(s,i,k++,"Kay Tusc",6,1,40,1,1200);
	set_server_bot_info(s,i,k++,"Kay Trik",6,1,40,1,1200);
	set_server_bot_info(s,i,k++,"Kay Plin",6,1,40,1,1200);
	set_server_bot_info(s,i,k++,"Kay Gli",6,1,40,1,1200);
	set_server_bot_info(s,i,k++,"Kay Quin",6,1,40,1,1200);
	set_server_bot_info(s,i,k++,"Kay Wurt",6,1,40,1,1200);
	
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=19;
	set_server_bot_type_info(s,i,-1,"Gate Keeper","Gate Keeper",2,1,0,4,0);
	    
	bot_type[i].sector[22] = 1;
	bot_type[i].sector[23] = 1;
	bot_type[i].sector[24] = 1;
	bot_type[i].sector[27] = 1;
	bot_type[i].sector[28] = 1;
	bot_type[i].sector[33] = 1;
	bot_type[i].sector[32] = 1;
	bot_type[i].sector[31] = 1;
	
	bot_type[i].sector[60] = 1;
	bot_type[i].sector[56] = 1;
	bot_type[i].sector[61] = 1;
	bot_type[i].sector[52] = 1;
	bot_type[i].sector[62] = 1;
	bot_type[i].sector[57] = 1;
	bot_type[i].sector[53] = 1;
	bot_type[i].sector[51] = 1;
	
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Xi Solix",7,7,320,2,2250);
	set_server_bot_info(s,i,k++,"Xi Gren",7,7,320,2,2250);
	set_server_bot_info(s,i,k++,"Xi Twor",7,7,320,2,2250);
	set_server_bot_info(s,i,k++,"Xi Voltex",7,7,320,2,2250);
	set_server_bot_info(s,i,k++,"Xi Vox",7,7,320,2,2250);
	set_server_bot_info(s,i,k++,"Xi Zen",7,7,320,2,2250);
	
	set_server_bot_info(s,i,k++,"Xi Gren",7,6,300,2,2000);
	set_server_bot_info(s,i,k++,"Xi Kat",7,6,300,2,2000);
	set_server_bot_info(s,i,k++,"Xi Shin",7,6,300,2,2000);
	set_server_bot_info(s,i,k++,"Xi Wido",7,6,300,2,2000);
	
	set_server_bot_info(s,i,k++,"Xi Been",7,5,280,2,1700);
	set_server_bot_info(s,i,k++,"Xi Faled",7,5,280,2,1700);
	*/
	//void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=20;
	set_server_bot_type_info(s,i,-1,"Raider","Raider",2,3,0,2,0);
	for(j=0;j<74;j++)
	bot_type[i].sector[j] = 1;
	
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Jo Kar",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Tin",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Bob",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Heen",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo De",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Her",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Ter",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Kins",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Yeet",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Qio",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Hi",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Maske",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Sak",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Xi",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Teka",3,1,30,1,180);
	set_server_bot_info(s,i,k++,"Jo Nonst",3,1,30,1,180);
	
	set_server_bot_info(s,i,k++,"Ji Kon",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Shar",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Ko Hoo",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Hy",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Kin",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Po Ween",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Yogo",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Honis",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Yetis",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Hee",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Donko",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Jin",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Foo Char",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Hon",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Yuris",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Ho",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Gin",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Ky Jon",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Kerr",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Hay",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Lice",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Slitsh",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Sloon",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Too",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Fari",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Loo",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Kin Fi",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Re",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Gir",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Crij",3,0,20,1,120);
	set_server_bot_info(s,i,k++,"Ji Kark",3,0,20,1,120);
	*/
    //void set_server_bot_type_info(s,int i, int dum, char *name, char *guild, int class, int aggression, int intelligence, int zone, int field_type)
	/*
	i=21;
	set_server_bot_type_info(s,i,-1,"Velic Pirate","Pirate",2,2,0,2,0);
	for(j=0;j<29;j++)
	bot_type[i].sector[j] = 1;
	
	
	//void set_server_bot_info(s,int i, int k, char *name, int ship_class, int ship_kind, int gun, int def_multi, int exp)
	k=0;
	set_server_bot_info(s,i,k++,"Dert",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Fint",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Wurr",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Tyr Ken",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Iit",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Yerd",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Jome",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Lurkis",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Proder",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Sinch",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Takis",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Jefor",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Cubbon",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Lawd",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Fing",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Dowge",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Tinge",0,0,10,1,50);
	set_server_bot_info(s,i,k++,"Gaurd",0,0,10,1,50);
	*/
	
	printf("database created- purpose:bot\n");
}

void create_missile_db()
{
	int i = 0;
	//set_server_missile_info(int i, int k, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type)
	set_server_missile_info(i,0,"V1 Torpedo",400,5,60,60,0,0,0,0);
	set_server_missile_info(i,1,"V1 Sidewinder",800,5,65,90,5,0,0,0);
	set_server_missile_info(i,2,"DSP Bomb",5900,20,60,60,10,0,0,2);
	set_server_missile_info(i,3,"Headhunter",7600,20,80,90,100,0,0,3);
	set_server_missile_info(i,4,"Ion Burst",15800,30,80,70,200,20,0,0);
	set_server_missile_info(i,5,"RK1 Blade",15500,30,90,70,300,100,0,0);
	set_server_missile_info(i,6,"Razor Bomb",18400,40,80,50,400,200,0,4);
	set_server_missile_info(i,7,"SilverScythe",25000,40,85,90,600,250,0,5);
	set_server_missile_info(i,8,"M2 Stinger",45000,60,90,80,1200,400,0,0);
	set_server_missile_info(i,9,"M2 Annihilator",50000,120,50,80,1600,600,0,0);
	
	printf("database created- purpose:missile\n");
}

void set_server_sector_info(int i, int k, int z, int planet, int shield, int x, int y, int sec1, int sec2, int sec3, int sec4, int sec5, int range, int is_sun, int is_nebula)
{
	zone[k].sector[z].planet = planet;
	zone[k].sector[z].shield = shield;
	zone[k].sector[z].x = x;
	zone[k].sector[z].y = y;
	zone[k].sector[z].sector[0] = sec1;
	zone[k].sector[z].sector[1] = sec2;
	zone[k].sector[z].sector[2] = sec3;
	zone[k].sector[z].sector[3] = sec4;
	zone[k].sector[z].sector[4] = sec5;
	zone[k].sector[z].range = range;
	zone[k].sector[z].is_sun = is_sun;
	zone[k].sector[z].is_nebula = is_nebula;
}

void set_server_planet_info(int i, int k, int is_gate, int is_home, char *planet_name, char *guild_name, int home_class, char *bar_name, int has_guild_halls, int is_server_gate)
{
	strcpy(planet[k].name,planet_name);
	strcpy(planet[k].guild,guild_name);
	strcpy(planet[k].bar_name,bar_name);
	planet[k].is_gate = is_gate;
	planet[k].is_home = is_home;
	planet[k].has_guild_halls = has_guild_halls;
	planet[k].home_class = home_class;
	planet[k].is_server_gate = is_server_gate;
}

void set_server_gun_info(int i, int k, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type)
{
	strcpy(gun[k].name,weapon_name);
	gun[k].cost 		= cost;
	gun[k].damage 		= damage;
	gun[k].speed 		= speed;
	gun[k].accuracy 	= accuracy;
	gun[k].exp_static 		= exp_static;
	gun[k].exp_added 		= exp_added;
	gun[k].tech_level 		= tech_level;
	gun[k].type 	= type;
}

void set_server_missile_info(int i, int k, char *weapon_name, int cost, int damage, int accuracy, int speed, int exp_static, int exp_added, int tech_level, int type)
{
	strcpy(missile[k].name,weapon_name);
	missile[k].cost 		= cost;
	missile[k].damage 	= damage;
	missile[k].speed 	= speed;
	missile[k].accuracy 	= accuracy;
	missile[k].exp_static 		= exp_static;
	missile[k].exp_added 		= exp_added;
	missile[k].tech_level 		= tech_level;
	missile[k].type 	= type;
}

void set_server_ship_info(int i, int k, int z, int cost, int exp, int hull, int shield, int evasion, int speed, int range, int cargo, int guns, int missiles, int miscs)
{
	ship[k].cost[z]			= cost;
	ship[k].exp[z]			= exp;
	ship[k].hull[z]			= hull;
	ship[k].shield[z]		= shield;
	ship[k].evasion[z]		= evasion;
	ship[k].speed[z]		= speed;
	ship[k].range[z]		= range;
	ship[k].cargo[z]		= cargo;
	ship[k].gun_ammount[z]		= guns;
	ship[k].missile_ammount[z]	= missiles;
	ship[k].misc_ammount[z]		= miscs;
}

void set_server_bot_type_info(int s, int i, int dum, char *name, char *guild, int the_class, int aggression, int intelligence, int zone, int field_type)
{
	strcpy(bot_type[i].name,name);
	strcpy(bot_type[i].guild,guild);
	bot_type[i].bot_class = the_class;
	bot_type[i].aggression = aggression;
	bot_type[i].intelligence = intelligence;
	bot_type[i].zone = zone;
	bot_type[i].field_type = field_type;
}

void set_server_bot_info(int s, int i, int k, char *name, int ship_class, int ship_kind, int gun, float def_multi, int exp)
{
	strcpy(bot_type[i].bot[k].name,name);
	bot_type[i].bot[k].ship_class = ship_class;
	bot_type[i].bot[k].ship_kind = ship_kind;
	bot_type[i].bot[k].gun = gun;
	bot_type[i].bot[k].def_multi = def_multi;
	bot_type[i].bot[k].exp = exp;
}
