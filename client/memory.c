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

#include "main.h"

//sdl
SDL_Surface *screen;
TTF_Font *ttf_font;
struct sound_sample game_sound[MAX_SOUND];
int sound_max;
//SDL_mutex* space_mutex;
SDL_mutex* flip_mutex;

//game
struct game_memory game;
struct server_db server[MAX_SERVER];

//ignore list
char *ignore_list[MAX_IGNORE];
int ignore_list_max;

//sos list
char *sos_list[MAX_SOS];
int sos_list_max;

//sounds
int sound_explosion;
int sound_engine;
int sound_jump;
int sound_warning;
int sound_gun;
int sound_enemy_gun[2];
int sound_missile;
int sound_shield;
int sound_impact[5];
int sound_button;


char trade_disaster_text[10][2][MAX_TRADE_DISASTER][500];
int trade_disaster_max[10][2];

void add_trade_disaster_text(int g, int lowers_price, char *message);

void set_trade_disaster_texts()
{
     int i, j, k;
     
     //first clear all texts
     for(i=0;i<10;i++)
         for(j=0;j<2;j++)
                 trade_disaster_max[i][j] = 0;
             
     add_trade_disaster_text(1,0,"A cold winter and increased ship repairs has made the price of Gryn Stone rise considerably.");
     add_trade_disaster_text(6,0,"A combination of near-space battles and an epidemic on the planet surface has been putting enormous pressure on medical supplies. Nearby planets are being asked to increase trade to ease the very high market value.");
     add_trade_disaster_text(7,1,"A drop in ship repair demand has lead to a corresponding fall in the price of Vensium. Traders and fighters are now going to nearby planets where the quality of the repairs is higher. The Cloning Bank has been asked to intervene.");
     add_trade_disaster_text(7,1,"A fashion industry move away from using Vensium has had a greater impact on price levels than expected. Its use has now been confined to gun casings, and the market cannot stand so much of the precious stone. Prices are flatlining.");
     add_trade_disaster_text(1,1,"A huge Gryn Stone deposit has been found and the market is saturated. Prices are at their lowest.");
     add_trade_disaster_text(0,1,"A large find of raw materials for Tynosine production has flooded the market, leading to a sharp decrease in trading value.");
     add_trade_disaster_text(5,1,"A massive crash in the planet's financial sector fuelled by external debt and unemployment has wiped billions off the value of Zynine. A good buying price, but will take a long time to recuperate to regular levels.");
     add_trade_disaster_text(4,1,"A metal-eating virus has reappeared on the planet, putting off most traders from investing in Paralium. Prices are low and attractive, but carry the risk of buying infected metal.");
     add_trade_disaster_text(0,1,"A new manufacturing process has allowed huge amounts of Tynosine to be grown. Its value has dropped to an all-time low. A recommended buy!");
     add_trade_disaster_text(2,1,"A new reserve of Cryonite has been discovered, which has put more of the oil onto the market and lowered its price.");
     add_trade_disaster_text(8,1,"A new reserve of Gen Synth has been discovered, which has offloaded large quantities onto the market and lowered its price.");
     add_trade_disaster_text(4,1,"A new reserve of Paralium has been discovered, which has put more of the metal onto the market and lowered its price.");
     add_trade_disaster_text(1,0,"A planet-wide Gryn mining strike has stopped all production of Gryn Stone. The shortage has greatly increased its value.");
     add_trade_disaster_text(8,0,"A planetwide laboratory strike has stopped all production of Gen Synth. This shortage has greatly increased its value.");
     add_trade_disaster_text(4,0,"A planetwide mining strike has stopped all production of Paralium. The shortage has greatly increased its value.");
     add_trade_disaster_text(3,1,"A price war among Kengo Weed processors has knocked a small fortune off the unit price which is close to rock bottom.");
     add_trade_disaster_text(2,0,"A valuable naturally-occurring oil used as a fuel source. Demand for Cryonite has recently grown greater than its ability for self-recycling, leading to higher prices and environmental concerns.");
     add_trade_disaster_text(6,1,"A warm summer, and a Cloning Bank health drive has dealt a fatal blow to the price of medical goods. The Bank has issued an apology to manufacturers, but says their health drive was not the main cause of the price crash.");
     add_trade_disaster_text(1,1,"A warm winter and emigration from the planet has created a surplus of Gryn Stone which has had a knock-on effect on trading prices.");
     add_trade_disaster_text(8,1,"Although analysts are agreeing that the worst of the recent financial crash is over, the stock price for Gen Synth is still close to rock bottom");
     add_trade_disaster_text(5,1,"Although analysts are agreeing that the worst of the recent financial crash is over, the stock price for the liquid metal Zynine is still very low.");
     add_trade_disaster_text(1,1,"An accident in the Gryn Mines has lead to renewed safety concerns about the stability of Gryn Stone. Demand has fallen slightly - as has the price.");
     add_trade_disaster_text(5,0,"An inert liquid metal, Zynine is mainly used for manufacturing and lubrication. Its beauty means that it is also used as a display of wealth.");
     add_trade_disaster_text(0,0,"An ongoing industrial renovation project requiring Tynosine-based processors has slightly increased market prices.");
     add_trade_disaster_text(2,0,"An upturn in demand for Cryonite has lead to a slight price rise in the markets.");
     add_trade_disaster_text(4,0,"An upturn in demand for Paralium has lead to a slight price rise in the markets.");
     add_trade_disaster_text(2,1,"Another Cloning Bank sell-off of Cryonite has had a devastating effect on market prices which have slumped to rock bottom.");
     add_trade_disaster_text(7,0,"Another attack by a plague of Whanet tunnelers has attacked a major Vensium reserve, causing the most stones to be useless to traders. Clear stones are rare and are at a premium.");
     add_trade_disaster_text(1,0,"Archaeological excavations have shown that Gryn Stone was used as a form of currency on some planets. It has found new value as a precious metal and is used for jewellry and sculpture.");
     add_trade_disaster_text(6,1,"Closure of hospitals on the planet surface has lead to a price drop for medical goods.");
     add_trade_disaster_text(2,1,"Environmental scares have lead to further restrictions on Cryonite use, greatly decreasing its consumer appeal and its value.");
     add_trade_disaster_text(6,0,"Fears of renewed fighting in the planet's near-space have caused hospitals and surgeries to increase their stocks of medical goods. While these fears remain unfounded, there has been a consequent rise in prices.");
     add_trade_disaster_text(3,0,"Fire has returned to the Kengo Weed fields with various farms affected. Next year's reserves are insecure, leading to hoarding and sharp rise in the unit price.");
     add_trade_disaster_text(9,1,"Gen Elium remains valuable but with very little hope of finding this material the price is no longer at its highest.");
     add_trade_disaster_text(1,1,"Gryn miners have been given new, more efficient machinery. Gryn production has risen and the prices have dropped.");
     add_trade_disaster_text(1,0,"Gryn mines dry up. Gryn mining has come to a halt while the search for new deposits is started and its value rises to its highest ever.");
     add_trade_disaster_text(6,0,"In these war-ridden times medical goods have become a valuable commodity. Hoarding of medicines is frequent, leading to prices fluctuating wildly. A war will always push the price up.");
     add_trade_disaster_text(8,0,"Increased demand for Gen Synth on other planets has created a lack of adequate supplies which in turn has triggered a price rise.");
     add_trade_disaster_text(3,0,"Increased demand for Kengo Weed on other planets has created a lack of adequate supplies which in turn has triggered a price rise.");
     add_trade_disaster_text(0,0,"Inter-planetary shortages of Tynosine combined with increased demand from traders has made the price rocket. A time to sell.");
     add_trade_disaster_text(7,0,"Inter-planetary shortages of Vensium combined with increased demand from traders has made the price rocket. A time to sell.");
     add_trade_disaster_text(3,1,"Kengo Weed production has hit a stable import/export level and current prices reflect this.");
     add_trade_disaster_text(3,0,"Kengo Weed was found to have amazing nutritional properties with its high levels of protein, vitamins and rehydration capacity. It is now harvested and is the known planets' main food resource.");
     add_trade_disaster_text(6,0,"Large numbers of people have been caught up in the Irina flu epidemic which has put a strain on medical staff and the availability of appropriate medical treatment. There has been a temporary shortage which has increased market values.");
     add_trade_disaster_text(8,1,"New production methods have been introduced to the Gen Synth processing plants, increasing productivity levels, and reducing the price to the consumer.");
     add_trade_disaster_text(7,1,"New production methods have been introduced to the Vensium processing plants, increasing productivity levels, and reducing the price to the consumer.");
     add_trade_disaster_text(5,1,"New production methods have been introduced to the Zynine processing plants, increasing productivity levels, and reducing the price to the consumer.");
     add_trade_disaster_text(6,0,"OH MY GOD!  The earthquake was devastating. Buildings have been turned to rumble. There is fire across major cities. Medical supplies have nearly run out. We are living in fear of after-shocks.");
     add_trade_disaster_text(2,0,"Once again a plague of Whanet tunnelers has attacked a Cryonite reserve, causing the oil to spill back into the planet's soil. Cryonite has been rationed, and the commodity is at a premium.");
     add_trade_disaster_text(7,1,"Ongoing demand for Vensium has kept the price of this stone stable.");
     add_trade_disaster_text(5,1,"Ongoing demand for Zynine has kept the price of this liquid metal as stable as a Behemoth in clear black space.");
     add_trade_disaster_text(4,0,"Paralium mines dry up. Paralium mining has come to a halt while the search for new deposits is started and its value rises to one of the highest ever levels.");
     add_trade_disaster_text(8,0,"Persistent power failures in the main Gen Synth laboratories have lowered production rates, and pushed up prices.");
     add_trade_disaster_text(0,0,"Persistent power failures in the main Tynosine laboratories have lowered production rates, and pushed up prices.");
     add_trade_disaster_text(8,0,"Population growth continues apace, having an parallel effect on the price of Gen Synth.");
     add_trade_disaster_text(3,0,"Population growth continues apace, having an parallel effect on the price of Kengo Weed.");
     add_trade_disaster_text(2,1,"Production and demand levels have are close to equal once again, stablising the market for Cryonite.");
     add_trade_disaster_text(8,1,"Production and demand levels are close to equal once again, stablising the market for Gen Synth.");
     add_trade_disaster_text(4,1,"Production and demand levels are close to equal once again, stablising the market for Paralium.");
     add_trade_disaster_text(0,1,"Production of Tynosine remains constant with only a slight drop in prices.");
     add_trade_disaster_text(6,0,"Recent fears of violence in the planet's near-space have been made a reality as casualties from the fighting continue to stream into the planet's hosptials. Medical supplies are at a premium, and their price has risen sharply.");
     add_trade_disaster_text(2,0,"Recent fighting around the planet has created a strong demand in the repair zones for Cryonite, which has pushed prices upwards.");
     add_trade_disaster_text(4,0,"Recent fighting around the planet has created a strong demand in the repair zones for Paralium hulls and weapons casing, which has pushed prices upwards.");
     add_trade_disaster_text(2,1,"Reserves of Cryonite are building up again, making it a reasonable investment for traders.");
     add_trade_disaster_text(4,1,"Reserves of Paralium are building up again, making it a reasonable investment for traders.");
     add_trade_disaster_text(1,0,"Rumours of falling Gryn production have raised the price slightly.");
     add_trade_disaster_text(2,0,"Rumours of low reserves of Cryonite have raised the price once more.");
     add_trade_disaster_text(4,0,"Rumours of low reserves of Paralium have raised the price once more.");
     add_trade_disaster_text(9,0,"Scientists researching the elusive Gen Elium find a new method of harnessing its power. The new demand forces its price to its highest ever.");
     add_trade_disaster_text(5,0,"Security has failed again to stop anti-capitalist campaigners damaging large stocks of Zynine. All reserves have been contaminated with Vensium and will need to be destroyed. Prices levels have risen dramtically as traders buy-to-hoard.");
     add_trade_disaster_text(1,1,"Several new Gryn mines have just opened. Prices have dropped very low as a result of the increased mining efforts.");
     add_trade_disaster_text(1,1,"Stablised production techniques and a steady demand has protected Gryn Stone from the price fluctuations of other goods.");
     add_trade_disaster_text(6,1,"Stockpiling by traders and the recent peaceful times have greatly reduced the price of all medical goods.");
     add_trade_disaster_text(8,0,"Terrorists have hit the Gen Synth supply vats on the planet destroying all reserves. External help has not been forthcoming, and the price has risen to an all-time high.");
     add_trade_disaster_text(3,0,"Terrorists have hit the Kengo Weed supply vats on the planet, reducing two years supply to dust. Supplies are coming in from other planets slower than expected, and the planet is suffering an enormous price hike on this basic food material.");
     add_trade_disaster_text(7,0,"The Cloning Bank has again put increased tourism to the top of its agenda. Demand for Vensium for crafts and gun casings has increased - as has the price.");
     add_trade_disaster_text(5,0,"The Cloning Bank has again put increased tourism to the top of its agenda. Demand for Zynine and other crafts materials has increased - as has the price.");
     add_trade_disaster_text(5,1,"The Cloning Bank has issued a formal warning that it will intervene to lower prices of Zynine. Underinvestment in filtration processes have slowed production to a crawl, while a buoyant ecomony has created massive demand.");
     add_trade_disaster_text(2,0,"The Cryonite filtration process has caused another major explosion on the planet, causing all processors to shut down and halt filtration. Very little Cryonite is on the market, and is almost priceless.");
     add_trade_disaster_text(3,1,"The Kengo Weed fields are growing out of control again after the recent heavy rains. The harvesters cannot keep up with the fields' growth. The market has not been able to provide any demand, and the price has collapsed totally.");
     add_trade_disaster_text(5,1,"The consolidation of the Zynine producers has put further pressure on the market, with Zynine being the latest material to be devalued.");
     add_trade_disaster_text(0,0,"The construction of another Military training base on the planet has put pressure on Tynosine production, and increased its value.");
     add_trade_disaster_text(9,1,"The interest in Gen Elium has died down as experimantal resources are growing thin. Its price is at its lowest ever.");
     add_trade_disaster_text(3,1,"The mild summer and increased imports from other food-producing planets has decreased Kengo Weed demand on the planet. Prices have dived as a consequence as consumers opt for alternative foods.");
     add_trade_disaster_text(6,1,"The military curfew remains in force, keeping the nunber of violent acts and injuries down. The price for medical goods matches this reduction.");
     add_trade_disaster_text(7,0,"The monthly solar flare activity has damaged the Vensium storage depots. Recovery will take some weeks. Prices have risen as supplies become scarce.");
     add_trade_disaster_text(5,0,"The monthly solar flare activity has damaged the Zynine storage depots. Decontamination will take some weeks. Prices have risen as supplies become scarce.");
     add_trade_disaster_text(7,0,"The planet's strengthening economy has had a large effect on the price of Vensium, which shows no sign of slowing the rapid rise of its market value.");
     add_trade_disaster_text(5,0,"The planet's strengthening economy has had a large effect on the price of Zynine, which shows no sign of slowing the rapid rise of its market value.");
     add_trade_disaster_text(8,0,"The recent hopes of an improvement in Gen Synth are disproved and Gen Elium remains at a high price.");
     add_trade_disaster_text(7,1,"The recent rumours of Vensium devaluation have lead to a massive offloading of stock by traders and private investors. The market is totally saturated, and prices for Vensium are at their lowest in living memory.");
     add_trade_disaster_text(0,1,"Tynosine demand has tailed off and its price goes down.");
     add_trade_disaster_text(0,0,"Tynosine prices remain stable with only a small rise in price. Demand is steady.");
     add_trade_disaster_text(0,1,"Tynosine production has been stepped up, and with a drop in demand the prices have lowered.");
     add_trade_disaster_text(3,0,"Vermin have sliced through the Kengo Weed fields again, decimating the planet's current crops and reserves. Prices will take some time to stabilise down to normal levels.");
     add_trade_disaster_text(3,1,"Warnings of illness through anti-bacterial Kengo Weed sprays has diminished the public's confidence in a safe food chain. Demand and the market value have suffered mildly as a result.");
     add_trade_disaster_text(9,1,"With recent improvements to Gen Synth, Gen Elium drops to its lowest price ever.");
}

char *get_trade_disaster_text(int g, int lowers_price, int i)
{
     return trade_disaster_text[g][lowers_price][i];
}

void add_trade_disaster_text(int g, int lowers_price, char *message)
{
     int i;
     
     //find first available
     i=trade_disaster_max[g][lowers_price];
     
     if(i==MAX_TRADE_DISASTER) return;
     
     //copy over
     strcpy(trade_disaster_text[g][lowers_price][i], message);
     
     //set new
     trade_disaster_max[g][lowers_price]++;
}
