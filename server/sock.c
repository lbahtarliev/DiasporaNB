#include "main.h"

#define MAX_DATA_STORED 120000
#define MAX_BUF_SIZE 60000

struct client_info
{
	int c_id;
	struct sockaddr_in c_in;
	char recv_data[MAX_DATA_STORED];
	int recv_amt;
	int connected;
	int sending_mutex;
};

void handle_client_hub();
void *handle_connect_hub(void *nothing);
void *start_server_user(void *nothing);


void dejunk_bytes(char *buf, int amt);
int poll_for_send(int con_id);
void uni_close_socket(int id);
int socket_good_to_recv(int s);
int socket_good_to_send(int s);
int pause_for_send(int s);

struct client_info client_hub;
struct client_info client_user[MAX_USER_CON];

void connect_to_hub()
{
	pthread_t client_thread;
	void *nothing;

	//this function should only ever be called once
	client_hub.connected = 0;
	client_hub.c_id = -1;
	
	pthread_create_always(&client_thread, NULL, handle_connect_hub, nothing);
}

void start_server()
{
	void *nothing;
	pthread_t client_thread;
	
	pthread_create_always(&client_thread, NULL, start_server_user, nothing);
}

int socket_good_to_recv(int s)
{
	fd_set read_flags;
	fd_set error_flags;
	int the_stat;
	struct timeval waitd;
	
	waitd.tv_sec = 0;
	waitd.tv_usec = 0;
	
	FD_ZERO(&read_flags);
	FD_ZERO(&error_flags);
	FD_SET(s, &read_flags);
	FD_SET(s, &error_flags);
	
	the_stat = select(s + 1, &read_flags, (fd_set*)0, &error_flags, &waitd);
	
	if(the_stat < 0)
	{
#ifdef _WIN32
		printf("select errno:%d\n",errno);
#else
		printf("select errno:%d-%s\n",errno, strerror(errno));
#endif
		return 1;
	}
	
	if(FD_ISSET(s, &error_flags))
	{
		printf("socket_good_to_recv:error flag set\n");
		return 0;
	}
	
	if(FD_ISSET(s, &read_flags))
	{
		FD_CLR(s, &read_flags);
		return 1;
	}
	else
		return 0;
}

int socket_good_to_send(int s)
{
	fd_set write_flags;
	fd_set error_flags;
	int the_stat;
	struct timeval waitd;
	
	waitd.tv_sec = 0;
	waitd.tv_usec = 0;
	
	FD_ZERO(&write_flags);
	FD_ZERO(&error_flags);
	FD_SET(s, &write_flags);
	FD_SET(s, &error_flags);
	
	the_stat = select(s + 1, (fd_set*)0, &write_flags, &error_flags, &waitd);
	
	if(the_stat < 0)
	{
#ifdef _WIN32
		printf("select errno:%d\n",errno);
#else
		printf("select errno:%d-%s\n",errno, strerror(errno));
#endif
		return 0;
	}
	
	if(FD_ISSET(s, &error_flags))
	{
		printf("socket_good_to_send:error flag set\n");
		return 0;
	}
	
	if(FD_ISSET(s, &write_flags))
	{
		FD_CLR(s, &write_flags);
		return 1;
	}
	else
		return 0;
}

int pause_for_send(int s)
{
	const double max_wait = 0.5;
	double start_time;
	
	if(socket_good_to_send(s)) return 1;
	
	start_time = current_time();
	
	do
	{
		uni_pause(10);
		
		printf("pause_for_send:had to begin pausing\n");
		
		if(socket_good_to_send(s)) return 1;
	} while(current_time() - start_time < max_wait);
	
	return 0;
}

void *start_server_user(void *nothing)
{
	int s, i, k;
	long int c_temp;
	u_long on_mode = 1;
	struct sockaddr_in sin;
	struct sockaddr_in temp_in;
	int temp_id;
	const int max_recv = 15;
	pthread_t c_thread[MAX_USER_CON];
	#ifdef _WIN32
	int c_temp_addrlen;
	#else
	socklen_t c_temp_addrlen;
	#endif

	#ifdef _WIN32 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
	#endif

	while ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("server error- reason:socket\n");
		uni_pause(5000);
	}
	
	//so the server can do a quick restart after a crash
	int on = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) == -1)
		printf("server error- reason:SO_REUSEADDR\n");

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(4600);

	while (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		printf("server error- reason:bind\n");
		uni_pause(5000);
	}

	while (listen(s, 5) == -1) {
		printf("server error- reason:listen\n");
		uni_pause(5000);
	}

	printf("server started-  port:4600\n");
	
	for(i=0;i<MAX_USER_CON;i++)
	{
		client_user[i].connected = 0;  //clear client list
		user_con[i].user_id = 0; //make sure no one is logged on
	}
	
	//make the listen socket async
#ifdef _WIN32
	ioctlsocket(s, FIONBIO, &on_mode);
#else		
	ioctl(s, FIONBIO, &on_mode);
#endif
	
	while(1)
	{
		/*
		c_temp = -1;
		for(i=0;i<MAX_USER_CON;i++)
			if (client_user[i].connected == 0)
			{
				c_temp = i;
				break;
			}

		//if no open slots avaible, look again
	      	if (c_temp == -1) continue; 
		*/

		//accept
		c_temp_addrlen = sizeof(client_user[c_temp].c_in);
		//client_user[c_temp].c_id = accept(s,(struct sockaddr*)  &client_user[c_temp].c_in, &c_temp_addrlen);
		temp_id = accept(s,(struct sockaddr*)  &temp_in, &c_temp_addrlen);

		//if accept fails, try again
		if(temp_id > -1) //got one so lets put him in
		{
			printf("id=%d\n", temp_id);
			
			//find an empty slot
			for(i=0;i<MAX_USER_CON;i++)
				if (client_user[i].connected == 0)
			{
				printf("connecting=%d\n", i);
				
				//set stuff
				client_user[i].c_id = temp_id;
				client_user[i].c_in = temp_in;
				
				//async it
#ifdef _WIN32
				ioctlsocket(temp_id, FIONBIO, &on_mode);
#else		
				ioctl(temp_id, FIONBIO, &on_mode);
#endif
				
				//clean up
				client_user[i].recv_amt = 0;
				client_user[i].recv_data[0] = 0;
				client_user[i].sending_mutex = 0;

				printf("Usr-connection %d accepted from %s\n",i,inet_ntoa(client_user[i].c_in.sin_addr));
	
				con_clear_stat(i);
				
				client_user[i].connected = 1;
				
				con_user_connect(i);
			
				//set con_top if required
				if (i > user_con_top) user_con_top = i;
			
				//start client handling in new thread
				//pthread_create(&c_thread[i], NULL, handle_client_user, (void *)i); 
				//pthread_detach(c_thread[i]);
				
				break;
			}
			
			//if no slots found then close the good fool
			if(i==MAX_USER_CON)
				uni_close_socket(temp_id);
		}

		//now do all of the packet stuff
		for(i=0;i<=user_con_top;i++)
			if(client_user[i].connected)
				handle_socket_receive_client(&(client_user[i]), i, process_packet_user);
		
		//little pause..
		uni_pause(10);
	}
	pthread_exit(NULL);
        return 0;
}

int handle_socket_receive_hub(struct client_info *cur_socket, int c_num, void (*process_func)(int, char*))
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	
	buf[0] = '\0';
	
	rcv_amt = recv(cur_socket->c_id, buf, MAX_BUF_SIZE, 0);

	#ifdef _WIN32
	if(WSAGetLastError() == 10054)
	#else
	if(!rcv_amt) 
	#endif
	{
		//they disconnected
		cur_socket->connected = 0;
		uni_close_socket(cur_socket->c_id);
		return 0;
	}
	else if(rcv_amt == -1)
	{
		//simply no packet yet
		return 0;
	}

	dejunk_bytes(buf, rcv_amt);

	//if by adding more data you excede the space given, then trash the previous 29000 chars.
	if (rcv_amt + cur_socket->recv_amt >= MAX_DATA_STORED) 
	{
		cur_socket->recv_amt = 0;
		cur_socket->recv_data[0] = '\0';
		printf("handle_socket_receive_hub: MAX_DATA_STORED\n");
	}

	//add new data onto any previous data
	memcpy(cur_socket->recv_data + cur_socket->recv_amt, buf, rcv_amt); 
	cur_socket->recv_amt += rcv_amt;
	cur_socket->recv_data[cur_socket->recv_amt] = 0;

	temp_amt = 0;
	//run through collected data and check for processable data
	for(i=0;i<cur_socket->recv_amt;) 
	{
		int prev_amt;
			
		prev_amt = i;
			
		split(buf,cur_socket->recv_data,13,&i);
			
		if(cur_socket->recv_data[i] == 0 && i < cur_socket->recv_amt) i++;
	
		//if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
		if ((i >= cur_socket->recv_amt) && (cur_socket->recv_data[cur_socket->recv_amt - 1] != 13)) 
		{
			temp_amt = cur_socket->recv_amt - prev_amt;
			memcpy(cur_socket->recv_data,buf, temp_amt);
		}
		else if (buf[0] != '\0') //why process a blank string?
		{
			process_func(c_num, buf);
		}

	}

	cur_socket->recv_data[temp_amt] = '\0';
	cur_socket->recv_amt = temp_amt;
	
	return 1;
}

int handle_socket_receive_client(struct client_info *cur_socket, int c_num, void (*process_func)(int, char*))
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	
	buf[0] = '\0';
	
	if(!socket_good_to_recv(cur_socket->c_id)) return 1;
	
	rcv_amt = recv(cur_socket->c_id, buf, MAX_BUF_SIZE, 0);
	
	#ifdef _WIN32
	if(WSAGetLastError() == 10054)
	#else
	if(!rcv_amt) 
	#endif
	{
		//they disconnected
		close_user_socket(c_num);
		user_logoff(c_num);
		return 0;
	}
	else if(rcv_amt == -1)
	{
		//simply no packet yet
		return 0;
	}

	dejunk_bytes(buf, rcv_amt);

	//if by adding more data you excede the space given, then trash the previous 29000 chars.
	if (rcv_amt + cur_socket->recv_amt >= MAX_DATA_STORED) 
	{
		cur_socket->recv_amt = 0;
		cur_socket->recv_data[0] = '\0';
	}

	//add new data onto any previous data
	memcpy(cur_socket->recv_data + cur_socket->recv_amt, buf, rcv_amt); 
	cur_socket->recv_amt += rcv_amt;
	cur_socket->recv_data[cur_socket->recv_amt] = 0;

	temp_amt = 0;

	//run through collected data and check for processable data
	for(i=0;i<cur_socket->recv_amt;) 
	{
		int prev_amt;
			
		prev_amt = i;
			
		split(buf,cur_socket->recv_data,13,&i);
			
		if(cur_socket->recv_data[i] == 0 && i < cur_socket->recv_amt) i++;
	
			//if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
		if ((i >= cur_socket->recv_amt) && (cur_socket->recv_data[cur_socket->recv_amt - 1] != 13)) 
		{
			temp_amt = cur_socket->recv_amt - prev_amt;
			memcpy(cur_socket->recv_data,buf, temp_amt);
		}
		else if (buf[0] != '\0') //why process a blank string?
		{
			process_func(c_num, buf);
		}

	}

	cur_socket->recv_data[temp_amt] = '\0';
	cur_socket->recv_amt = temp_amt;
	
	return 1;
}

void *handle_connect_hub(void *nothing)
{
	struct hostent * host;
	pthread_t client_thread;

	#ifdef _M_IX86 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
	#endif
	
	//first close it if it may already exist (it is possible)
	if(client_hub.connected && client_hub.c_id > -1)
	{
		client_hub.connected = 0;
		uni_close_socket(client_hub.c_id);
	}
	
	while ((client_hub.c_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		printf("Socket creation failed.\n");
		uni_pause(5000);
	}
	
 	host = gethostbyname(server_info.hub_address);
 	memcpy((char*)&client_hub.c_in.sin_addr, (char*)host->h_addr, host->h_length);
 	client_hub.c_in.sin_family = AF_INET;
 	client_hub.c_in.sin_port = htons(4000);
 
	
 	while(connect(client_hub.c_id, (struct sockaddr *)&client_hub.c_in, sizeof client_hub.c_in) < 0)
	{
 		printf("Connection to hub server failed, reconnecting...\n");
#ifdef _WIN32
		Sleep(10000);
#else
		sleep(10);
#endif
	}
	
	printf("Connection established, sending login...\n");
	
	client_hub.connected = 1;
	
	//pthread_create_always(&client_thread, NULL, handle_client_hub, nothing);
	connect_hub_process();
	handle_client_hub();
	
	pthread_exit(NULL);
	return 0;
}

void handle_client_hub()
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;

        client_hub.recv_amt = 0; //clear any left over data collected from a previous connection
        client_hub.recv_data[0] = '\0';
	client_hub.sending_mutex = 0;
		
	while(client_hub.connected)  // if you rcv 0 data, the connection closed
	{
		handle_socket_receive_hub(&client_hub, 0, process_packet_hub);
		
		/*
		buf[0] = '\0';

		rcv_amt = recv(client_hub.c_id, buf, MAX_BUF_SIZE, 0);
		if(rcv_amt <= 0) break;
		
		dejunk_bytes(buf, rcv_amt);

		if (rcv_amt + client_hub.recv_amt >= MAX_DATA_STORED) //if by adding more data you excede the space given, then trash the previous chars.
                {
			client_hub.recv_amt = 0;
			client_hub.recv_data[0] = '\0';
                }

		memcpy(client_hub.recv_data + client_hub.recv_amt, buf, rcv_amt); //add new data onto any previous data
		client_hub.recv_amt += rcv_amt;
		client_hub.recv_data[client_hub.recv_amt] = 0;

		for(i=temp_amt=0;i<client_hub.recv_amt;) //run through collected data and check for processable data
                {
			int prev_amt;
			
			prev_amt = i;
			
			split(buf,client_hub.recv_data,13,&i);
			
			if(client_hub.recv_data[i] == 0 && i < client_hub.recv_amt) i++;
	
			if ((i >= client_hub.recv_amt) && (client_hub.recv_data[client_hub.recv_amt - 1] != 13)) //if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
			{
				temp_amt = client_hub.recv_amt - prev_amt;
				memcpy(client_hub.recv_data,buf, temp_amt);
			}
			else if (buf[0] != '\0') //why process a blank string?
			{
				process_packet_hub(buf);
                        }

                }
                
		client_hub.recv_data[temp_amt] = '\0';
		client_hub.recv_amt = temp_amt;
		*/
	}
	
	uni_close_socket(client_hub.c_id);
	
	client_hub.connected = 0;  // set avaible again
	
	//reconnect
	//connect_to_hub();
	
	pthread_exit(NULL);
        return;
}

void send_con_hub(char *message)
{
        char c[2] = {13,'\0'};
	int error_no;
	
	if(!client_hub.connected) return;
	
	//gar
	while(client_hub.sending_mutex)
		uni_pause(1);
	
	client_hub.sending_mutex = 1;
	
	//printf("hub-snt:%s\n", message);
	strcat(message,c);
	error_no = send(client_hub.c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		client_hub.connected = 0;
		uni_close_socket(client_hub.c_id);
	}
	
	client_hub.sending_mutex = 0;
}

int poll_for_send(int con_id)
{
#ifdef _WIN32
	fd_set the_set;
	int ret_val;
	
	FD_ZERO(&the_set);
	FD_SET(con_id, &the_set);
	
	ret_val = select(con_id + 1, NULL, &the_set, NULL, NULL);
	
	if(ret_val > 0)
	{
		printf("socket good\n");
		return 1;
	}
	if(ret_val == -1)
	{
		printf("socket bad\n");
		return 0;
	}
#else
	pollfd pfd;
	
	//poll stuff
	pfd.fd = con_id;
	while(1)
	{
		pfd.events = POLLOUT;
		pfd.revents = 0;
		poll(&pfd, 1, -1);
		
		if(pfd.revents & POLLOUT)
			return 1;
		else if(pfd.revents & (POLLERR + POLLHUP + POLLNVAL))
			return 0;
		
		uni_pause(1);
		printf("poll loop %d\n", pfd.revents);
	}
#endif
}

void send_con_user(int con, char* message)
{
        char c[2] = {13,'\0'};
	int error_no;
	
	if(!client_user[con].connected)
	{
		printf("send on usr closed con:%d\n", con);
		return;
	}
	
	//gar
	while(client_user[con].sending_mutex)
		uni_pause(1);

	client_user[con].sending_mutex = 1;
	
	//poll
	if(!pause_for_send(client_user[con].c_id))
	{
		close_user_socket(con);
		user_logoff(con);
		printf("!socket_good_to_send\n");
		client_user[con].sending_mutex = 0;
		return;
	}
	
	printf("usr-snt-%d:%s\n", con, message);
	
	strcat(message,c);

	error_no = send(client_user[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		printf("closing socket send error\n");
		close_user_socket(con);
		user_logoff(con);
		printf("closing socket send error out\n");
	}
	
	client_user[con].sending_mutex = 0;
}

 void raw_send_con_user(int con, char* message)
{
	int error_no;
	
	if(!client_user[con].connected)
	{
		printf("send on usr closed con:%d\n", con);
		return;
	}
	
	//gar
	while(client_user[con].sending_mutex)
		uni_pause(1);
	
	client_user[con].sending_mutex = 1;
	
	//poll
	if(!pause_for_send(client_user[con].c_id))
	{
		close_user_socket(con);
		user_logoff(con);
		printf("!socket_good_to_send\n");
		client_user[con].sending_mutex = 0;
		return;
	}
	
	//poll stuff
	
	printf("usr-snt-raw-%d:%s\n", con, message);

	error_no = send(client_user[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		printf("closing socket send error\n");
		close_user_socket(con);
		user_logoff(con);
		printf("closing socket send error out\n");
	}
	
	client_user[con].sending_mutex = 0;
}

void get_con_user_ip(int con, char *dest)
{
	strcpy(dest,(char*)inet_ntoa(client_user[con].c_in.sin_addr));
}

void do_sleep()
{
	while(1)
	{
		#ifdef _WIN32 //if windows
		Sleep(15 * 5000);	//windows
		#else
		sleep(15 * 5);	//linux
		#endif
		bot_clear_all_bot_goods();
	}
}

void dejunk_bytes(char *buf, int amt)
{
	int i;
	
	for(i=0;i<amt;i++)
	{
		int cur;
		
		cur = buf[i];
		
		if(cur == 13) continue;
		if(cur >= 32 && cur <= 127) continue;
		
		buf[i] = 0;
	}
}

void uni_close_socket(int id)
{
#ifdef _WIN32
	closesocket(id);
#else
	close(id);
#endif
}

void close_user_socket(int con)
{
	if(client_user[con].connected)
	{
		client_user[con].connected = 0;
		printf("closing socket:%d\n", con);
		uni_close_socket(client_user[con].c_id);
		printf("closed socket:%d\n", con);
	}
}

unsigned long get_user_s_addr(int con)
{
	return client_user[con].c_in.sin_addr.s_addr;
}
