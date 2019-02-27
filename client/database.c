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

void read_host_info()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[1024] = "", temp_str[3][21];
	int i = 0; //use this as the length of the line read

	ifp = fopen("host.txt","r");  //setup this declared file to open user.db to read from
	
	if (!ifp) //if the file doesnt exist, then create it. and setup standard locations
	{
		strcpy(game.hub_address,"hestia.nighsoft.net");
		write_host_info();
		ifp = fopen("host.txt","r");
	}
	
	fscanf(ifp,"%50s",game.hub_address);

	fclose(ifp);
}

void write_host_info()
{
	FILE *ofp;
	
	ofp = fopen("host.txt","w");

	fprintf(ofp, "%s", game.hub_address);
		
	fclose(ofp);
}

void read_config_info()
{
	FILE *ifp; //declare file pointer, its just how this stuff works
	char cur_line[1024] = "", temp_str[3][21];
	int i = 0; //use this as the length of the line read

	ifp = fopen("config.txt","r");  //setup this declared file to open user.db to read from
	
	if (!ifp) //if the file doesnt exist, then create it. and setup standard locations
	{
		game.sound_on = 1;
		game.engine_on = 1;
		
		write_host_info();
		return;
	}
	
	fscanf(ifp,"%d %d", &(game.sound_on), &(game.engine_on));

	fclose(ifp);
}

void write_config_info()
{
	FILE *ofp;
	
	ofp = fopen("config.txt","w");

	fprintf(ofp, "%d %d", game.sound_on, game.engine_on);
		
	fclose(ofp);
}
