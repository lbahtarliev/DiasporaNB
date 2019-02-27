#include "main.h"

extern void split(char dest[], char message[], char char_spliter, int *initial)
{
        char temp[80];
        int i, a = 0;

    	for(i = *initial; i < 100100100; i++)
        {
	        if (message[i] == 0) // check to see if at end of string
	                break;

        	if (message[i] != char_spliter) //check to see if at the split mark
	        {
	        	dest[a] = message[i];
                        a++;
        	}
                else
                        break;
        }

        dest[a] = '\0';          //"cap" return string
	if (message[i] == 0) //return point to continue search
		*initial = i;
	else
		*initial = ++i;
        return;			//return
}

extern int str_match(char *str1, char *str2)
{
	if(str1[0] == '\0' || str2[0] == '\0')
	{
		if(str1[0] == str2[0])
			return 1;
		else
			return 0;
	}
	
	if (strcmp(str1,str2) == 0)
		return 1;
	else
		return 0;
}

int str_match_lcase(char *str1, char *str2)
{
	char l_str1[20000], l_str2[20000];
    
	lcase(l_str1, str1);
	lcase(l_str2, str2);
    
	if(str1[0] == '\0' || str2[0] == '\0')
	{
		if(str1[0] == str2[0])
			return 1;
		else
			return 0;
	}
	
	if (strcmp(l_str1,l_str2) == 0)
		return 1;
	else
		return 0;
}

extern void lcase(char *dest, char *src)
{
	int i;
	
	for(i=0;src[i] != '\0';i++)
	{
		if (src[i] >= 'A' && src[i] <= 'Z')
			dest[i] = src[i] + 32;
		else
			dest[i] = src[i];
	}
	
	dest[i] = '\0';
}

extern void right(char *dest, char *src, int point)
{
	int i = 0;
	while(src[point] != '\0')
	{
		dest[i] = src[point];
		point++;
		i++;
	}
	
	dest[i] = '\0';
}

void uni_pause(int m_sec)
{
#ifdef _WIN32 //if windows
	Sleep(m_sec);	//win version
#else
	usleep(m_sec * 1000);	//lin version
#endif
}

extern int misc_pod_value(int misc_pod_number)
{
	switch (misc_pod_number)
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

extern int misc_pod_exp(int misc_pod_number)
{
	switch (misc_pod_number)
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

extern int misc_pod_cost(int misc_pod_number)
{
	switch (misc_pod_number)
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

void printd(char *message)
{
	FILE *ofp;
	
	ofp = fopen("debug.txt","a");

	fprintf(ofp, "%s\n", message);
		
	fclose(ofp);
}

void printd_admin(char *message)
{
	FILE *ofp;
	
	ofp = fopen("admin_log.txt","a");

	fprintf(ofp, "%s\n", message);
		
	fclose(ofp);
}

void printd_ds(char *message)
{
	FILE *ofp;
	struct tm *ptr;
	time_t lt;

	lt = time(NULL);
	ptr = localtime(&lt);
	
	ofp = fopen("ds_log.txt","a");

	fprintf(ofp, "%s ^^:: %s\n", asctime(ptr), message);
		
	fclose(ofp);
}

double current_time()
{
	static int first_sec = 0;
	static int first_msec = 0;
	struct timeb new_time;
	
	//set current time
	ftime(&new_time);

	//set if not set
	if(!first_sec)
	{
		first_sec = new_time.time;
		first_msec = new_time.millitm;
	}
	
	return (new_time.time - first_sec) + ((new_time.millitm - first_msec) * 0.001);
}

char *to_jabber_name(char *dest, char *username)
{
	int len;
	int i;
	
	lcase(dest,username);
	
	len = strlen(dest);
	
	for(i=0;i<len;i++)
		if(dest[i] = ' ')
			dest[i] = '_';
	
	dest[i] = 0;
	
	return dest;
}

char *file_date(char *filename)
{
	time_t curtime;
	struct tm *loctime;
	char buf[200];
	char the_true[200] = "backup/";

	create_backup_folder();
	
	/* Get the current time.  */
	curtime = time (NULL);

	/* Convert it to local time representation.  */
	loctime = localtime (&curtime);
	
	strftime (buf, 20, "%y-%m-%d_", loctime);
	
	strcat(the_true,buf);
	strcat(the_true,filename);
	strcpy(filename,the_true);
	
	return filename;
}

char *ship_name(int ship_number, char *dest)
{
	switch (ship_number)
	{
		case 0:
			strcpy(dest,"Seeker");
			break;
		case 1:
			strcpy(dest,"Fighter");
			break;
		case 2:
			strcpy(dest,"Carrier");
			break;
		case 3:
			strcpy(dest,"Hunter");
			break;
		case 4:
			strcpy(dest,"Freighter");
			break;
		case 5:
			strcpy(dest,"Attacker");
			break;
		case 6:
			strcpy(dest,"Destroyer");
			break;
		case 7:
			strcpy(dest,"Behemoth");
			break;
	}
	
	return dest;
}

int add_will_overflow(int a, int b)
{
    int c;
    
    if(!a) return 0;
    if(!b) return 0;
    
    c = a + b;
    
    //both positive and the result negative?
    if(a > 0 && b > 0 && c < 0) return 1;
    
    //both negative and the result positive?
    if(a < 0 && b < 0 && c > 0) return 1;
    
    return 0;
    
}
