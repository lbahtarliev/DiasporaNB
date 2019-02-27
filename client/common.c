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

void split ( char dest[], char message[], char char_spliter, int *initial )
{
	int i, a = 0;

	for ( i = *initial; i < 100100100; i++ )
	{
		if ( message[i] == '\0' ) // check to see if at end of string
			break;

		if ( message[i] != char_spliter ) //check to see if at the split mark
		{
			dest[a] = message[i];
			a++;
		}
		else
			break;
	}

	dest[a] = '\0';          //"cap" return string
	if ( message[i] == 0 ) //return point to continue search
		*initial = i;
	else
		*initial = ++i;
	return;			//return
}

int str_match ( char *str1, char *str2 )
{
	if ( strcmp ( str1,str2 ) == 0 )
		return 1;
	else
		return 0;
}

void lcase ( char *dest, char *src )
{
	int i;

	for ( i=0;src[i] != '\0';i++ )
	{
		if ( src[i] >= 'A' && src[i] <= 'Z' )
			dest[i] = src[i] + 32;
		else
			dest[i] = src[i];
	}

	dest[i] = '\0';
}

void right ( char *dest, char *src, int point )
{
	int i = 0;
	while ( src[point] != '\0' )
	{
		dest[i] = src[point];
		point++;
		i++;
	}
	dest[i] = '\0';
}

void left ( char *dest, char *src, int point )
{
	int i=0;
	for ( i=0;src[i] != '\0' && i != point;i++ )
		dest[i] = src[i];

	dest[i] = '\0';
}

char *ship_name ( int ship_number, char *dest )
{
	switch ( ship_number )
	{
		case 0:
			strcpy ( dest,"Seeker" );
			break;
		case 1:
			strcpy ( dest,"Fighter" );
			break;
		case 2:
			strcpy ( dest,"Carrier" );
			break;
		case 3:
			strcpy ( dest,"Hunter" );
			break;
		case 4:
			strcpy ( dest,"Freighter" );
			break;
		case 5:
			strcpy ( dest,"Attacker" );
			break;
		case 6:
			strcpy ( dest,"Destroyer" );
			break;
		case 7:
			strcpy ( dest,"Behemoth" );
			break;
	}

	return dest;
}

char *misc_pod_name ( int misc_pod_number, char *dest )
{
	switch ( misc_pod_number )
	{
		case 0:
			strcpy ( dest,"Mk1 Hull Booster" );
			break;
		case 1:
			strcpy ( dest,"Mk1 Shield Generator" );
			break;
		case 2:
			strcpy ( dest,"Mk1 Cargo Pod" );
			break;
		case 3:
			strcpy ( dest,"Mk1 Jump Drive" );
			break;
		case 4:
			strcpy ( dest,"Mk2 Hull Booster" );
			break;
		case 5:
			strcpy ( dest,"Mk2 Shield Generator" );
			break;
		case 6:
			strcpy ( dest,"Mk2 Cargo Pod" );
			break;
		case 7:
			strcpy ( dest,"Mk2 Jump Drive" );
			break;
	}

	return dest;
}

int misc_pod_value ( int misc_pod_number )
{
	switch ( misc_pod_number )
	{
		case 0:
			return 50;
			break;
		case 1:
			return 5;
			break;
		case 2:
			return 50;
			break;
		case 3:
			return 2;
			break;
		case 4:
			return 100;
			break;
		case 5:
			return 7;
			break;
		case 6:
			return 75;
			break;
		case 7:
			return 3;
			break;
	}

	return 0;
}

int misc_pod_exp ( int misc_pod_number )
{
	switch ( misc_pod_number )
	{
		case 0:
			return 0;
			break;
		case 1:
			return 0;
			break;
		case 2:
			return 0;
			break;
		case 3:
			return 0;
			break;
		case 4:
			return 200;
			break;
		case 5:
			return 250;
			break;
		case 6:
			return 100;
			break;
		case 7:
			return 300;
			break;
	}

	return 0;
}

int misc_pod_cost ( int misc_pod_number )
{
	switch ( misc_pod_number )
	{
		case 0:
			return 5000;
			break;
		case 1:
			return 4000;
			break;
		case 2:
			return 2000;
			break;
		case 3:
			return 2000;
			break;
		case 4:
			return 250000;
			break;
		case 5:
			return 500000;
			break;
		case 6:
			return 100000;
			break;
		case 7:
			return 750000;
			break;
	}

	return 0;
}

int convert_key_sym ( int key, int caps )
{
	if ( caps )
	{
		if ( key >= 'a' && key <= 'z' )
			return key - 32;

		switch ( key )
		{
			case '`':
				return '~';
			case '1':
				return '!';
			case '2':
				return '@';
			case '3':
				return '#';
			case '4':
				return '$';
			case '5':
				return '%';
			case '6':
				return '^';
			case '7':
				return '&';
			case '8':
				return '*';
			case '9':
				return '(';
			case '0':
				return ')';
			case '-':
				return '_';
			case '=':
				return '+';
			case ',':
				return '<';
			case '.':
				return '>';
			case '/':
				return '?';
			case ';':
				return ':';
			case '\'':
				return '"';
			case '[':
				return '{';
			case ']':
				return '}';
				break;
		}
	}

	return key;
}

void printd ( char *message )
{
	FILE *ofp;

	ofp = fopen ( "debug.txt","a" );

	fprintf ( ofp, "%s\n", message );

	fclose ( ofp );
}

int distance ( int x1, int y1, int x2, int y2 )
{
	return ( int ) pow ( pow ( x1 - x2,2 ) + pow ( y1 - y2,2 ),0.5 );
}

int ship_speed_difference ( int s_k )
{
	//0=seeker +1
	//1=fighter
	//2=carrier -1
	//3=hunter
	//4=freightor -2
	//5=attacker
	//6=destroyer
	//7=behemoth -1

	switch ( s_k )
	{
		case 0: //seeker
			return -1;
		case 2:
			return 1;
		case 4:
			return 2;
		case 5:
			return -2;
		case 7:
			return 1;
	}

	return 0;
}

double ship_total_seconds ( int speed, int s_k )
{
	double seconds;

	if ( speed>26 ) speed = 26;
	if ( speed<0 ) speed = 0;

	seconds = 26 - speed;
	seconds = seconds / 2.0;
	seconds += 8;
	seconds += ship_speed_difference ( s_k );

	return seconds;
}

double current_time()
{
	static int first_sec = 0;
	static int first_msec = 0;
	struct timeb new_time;

	//set current time
	ftime ( &new_time );

	//set if not set
	if ( !first_sec )
	{
		first_sec = new_time.time;
		first_msec = new_time.millitm;
	}

	return ( new_time.time - first_sec ) + ( ( new_time.millitm - first_msec ) * 0.001 );
}

double random_chance()
{
	return ( rand() % 10000 ) / 10000.0;
}

void clean_newline ( char *message, int size )
{
	int i;

	for ( i=0;i<size;i++ )
	{
		if ( message[i] == '\r' )
		{
			message[i] = 0;
			break;
		}
		else if ( message[i] == '\n' )
		{
			message[i] = 0;
			break;
		}
		else if ( !message[i] )
			break;
	}
}
