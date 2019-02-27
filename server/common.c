#include "main.h"

extern void split ( char dest[], char message[], char char_spliter, int *initial )
{
	char temp[80];
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

extern void lcase ( char *dest, char *src )
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

extern int str_match ( char *str1, char *str2 )
{
	if ( str1[0] == '\0' || str2[0] == '\0' )
	{
		if ( str1[0] == str2[0] )
			return 1;
		else
			return 0;
	}

	if ( strcmp ( str1,str2 ) == 0 )
		return 1;
	else
		return 0;
}

int str_match_lcase ( char *str1, char *str2 )
{
	char l_str1[20000], l_str2[20000];

	lcase ( l_str1, str1 );
	lcase ( l_str2, str2 );

	if ( str1[0] == '\0' || str2[0] == '\0' )
	{
		if ( str1[0] == str2[0] )
			return 1;
		else
			return 0;
	}

	if ( strcmp ( l_str1,l_str2 ) == 0 )
		return 1;
	else
		return 0;
}

extern void right ( char *dest, char *src, int point )
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

extern void left ( char *dest, char *src, int point )
{
	int i=0;
	for ( i=0;src[i] != '\0' && i != point;i++ )
		dest[i] = src[i];

	dest[i] = '\0';
}

void uni_pause ( int m_sec )
{
#ifdef _WIN32 //if windows
	Sleep ( m_sec );	//win version
#else
	usleep ( m_sec * 1000 );	//lin version
#endif
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

char *good_name ( int good_number, char *dest )
{
	switch ( good_number )
	{
		case 0:
			strcpy ( dest,"Tynosine" );
			break;
		case 1:
			strcpy ( dest,"Gryn Stone" );
			break;
		case 2:
			strcpy ( dest,"Cryonite" );
			break;
		case 3:
			strcpy ( dest,"Kengo Weed" );
			break;
		case 4:
			strcpy ( dest,"Paralium" );
			break;
		case 5:
			strcpy ( dest,"Zynine" );
			break;
		case 6:
			strcpy ( dest,"Medical Goods" );
			break;
		case 7:
			strcpy ( dest,"Vensium" );
			break;
		case 8:
			strcpy ( dest,"Gen Synth" );
			break;
		case 9:
			strcpy ( dest,"Gen Elium" );
			break;
	}

	return dest;
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
		case 2: //2=carrier -1
			return 1;
		case 4: //4=freightor -2
			return 2;
		case 5: //5=attacker
			return -2;
		case 7: //7=behemoth -1
			return 1;
	}

	return 0;
}

double ship_damage_difference ( int s_k, double damage )
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
		case 2:
			return damage * 0.8;
		case 5:
			return damage * 1.2;
		case 6:
			return damage * 1.2;
		case 7:
			return damage * 1.2;
	}

	return damage;
}

double ship_damage_defense ( int s_k, double damage )
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
		case 5:
			return damage * 1.5;
		case 6:
			return damage * 0.95;
		case 7:
			return damage * 0.7;
	}

	return damage;
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

int pthread_create_always ( pthread_t *thread, const pthread_attr_t *attr, void * ( *start_routine ) ( void * ), void *arg )
{
	pthread_create ( thread, NULL, handle_pthread_create_always, ( void* ) start_routine );

	return 0;
}

void *handle_pthread_create_always ( void *start_routine )
{
	pthread_t the_thread;
	void *value_ptr;

	while ( 1 )
	{
		//create
		pthread_create ( &the_thread, NULL, ( void* ( * ) ( void* ) ) start_routine, NULL );

		//join
		pthread_join ( the_thread, &value_ptr );

		printf ( "program closed with thread\n" );

		exit ( 0 );

		uni_pause ( 1000 );
	}
}

double random_chance()
{
	return ( rand() % 10000 ) / 10000.0;
}

