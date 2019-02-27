#include "main.h"

void *handle_client_admin(void *clientid);
// void *handle_client_user(void *clientid);
void *handle_client_server(void *clientid);
int handle_socket_receive(struct client_info *cur_socket, int c_num, void (*process_func)(int, char*), void (*log_off_func)(int));
void *start_server_admin(void *nothing);
void *start_server_user(void *nothing);
void *start_server_server(void *nothing);
void start_server_sockets();

void *spam_client_user(void *clientid);

int active_connections(struct in_addr *our_addr);
int setup_socket_listen(int port, char *socket_name);
int poll_for_send(int con_id);
void uni_close_socket(int id);
int socket_good_to_recv(int s);
int socket_good_to_send(int s);
int pause_for_send(int s);

struct client_info client_admin[MAX_ADMIN_CON];
struct client_info client_user[MAX_USER_CON];
struct client_info client_server[MAX_SERVER_CON];

extern void start_server()
{
        //pthread_t admin_thread, user_thread, server_thread;
        void *nothing;

        //pthread_create(&admin_thread, NULL, start_server_admin, nothing);
        //pthread_create(&user_thread, NULL, start_server_user, nothing);
        //pthread_create(&server_thread, NULL, start_server_server, nothing);
	//pthread_create(&server_thread, NULL, start_server_sockets, nothing);
	start_server_sockets();
}

int setup_socket_listen(int port, char *socket_name)
{
	int s;
	struct sockaddr_in sin;
	u_long on_mode = 1;
	
#ifdef _WIN32 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
#endif
	
	while ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("server error- reason:socket purpose:%s\n", socket_name);
		uni_pause(5000);
	}
	
    //so the server can do a quick restart after a crash
	int on = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) == -1)
		printf("server error- reason:SO_REUSEADDR purpose:%s\n", socket_name);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);

	while (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		printf("server error- reason:bind purpose:%s\n", socket_name);
		uni_pause(5000);
	}

	while (listen(s, 5) == -1) {
		printf("server error- reason:listen purpose:%s\n", socket_name);
		uni_pause(5000);
	}

#ifdef _WIN32
	ioctlsocket(s, FIONBIO, &on_mode);
#else		
	ioctl(s, FIONBIO, &on_mode);
#endif
	
	printf("server started-  port:%d purpose:%s\n", port, socket_name);
	
	return s;
}

int socket_good_to_recv(int s)
{
	fd_set read_flags;
	int the_stat;
	struct timeval waitd;
	
	waitd.tv_sec = 0;
	waitd.tv_usec = 0;
	
	FD_ZERO(&read_flags);
	FD_SET(s, &read_flags);
	
	the_stat = select(s + 1, &read_flags, (fd_set*)0, (fd_set*)0, &waitd);
	
	if(the_stat < 0)
	{
#ifdef _WIN32
		printf("select errno:%d\n",errno);
#else
		printf("select errno:%d-%s\n",errno, strerror(errno));
#endif
		return 1;
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
	int the_stat;
	struct timeval waitd;
	
	waitd.tv_sec = 0;
	waitd.tv_usec = 250000;
	
	FD_ZERO(&write_flags);
	FD_SET(s, &write_flags);
	
	the_stat = select(s + 1, (fd_set*)0, &write_flags, (fd_set*)0, &waitd);
	
	if(the_stat < 0)
	{
#ifdef _WIN32
		printf("select errno:%d\n",errno);
#else
		printf("select errno:%d-%s\n",errno, strerror(errno));
#endif
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

int handle_socket_receive(struct client_info *cur_socket, int c_num, void (*process_func)(int, char*), void (*log_off_func)(int))
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	int is_error = 0, is_error_len;
	
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
		cur_socket->connected = 0;
		uni_close_socket(cur_socket->c_id);
		if(log_off_func) log_off_func(c_num);
		printf("socket disconnected\n");
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
			
		//to get us through null terminators in the stream
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

void start_server_sockets()
{
	int admin_s;
	int user_s;
	int server_s;
	int i;
	struct sockaddr_in temp_in;
	int temp_id;
#ifdef _WIN32
	int c_temp_addrlen;
#else
	socklen_t c_temp_addrlen;
#endif
	
	
#ifdef _WIN32 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
#endif

	//clear everything out
	for(i=0;i<MAX_ADMIN_CON;i++)
	{
		client_admin[i].connected = 0;  //clear client list
		admin_con_id[i] = -1; //make sure no one is logged on	
	}
	
	for(i=0;i<MAX_USER_CON;i++)
	{
		client_user[i].connected = 0;  //clear client list
		user_con_id[i] = -1; //make sure no one is logged on
		user_con_version[i] = 0; //make sure they dont have a set version already
	}
	
	for(i=0;i<MAX_SERVER_CON;i++)
	{
		client_server[i].connected = 0;  //clear client list
		server_con_id[i] = -1; //make sure no one is logged on
		server_con_version[i] = 0;
	}
	
	//setup the sockets
	admin_s = setup_socket_listen(ADMIN_PORT, "admin");
	server_s = setup_socket_listen(SERVER_PORT, "server");
	user_s = setup_socket_listen(USER_PORT, "user");
	
	//now do all their reading and accepting stuff
	while(1)
	{
		//accept an admin?
		c_temp_addrlen = sizeof(temp_in);
		temp_id = accept(admin_s,(struct sockaddr*)  &temp_in, &c_temp_addrlen);
		
		if(temp_id > -1 && active_connections(&(temp_in.sin_addr)) >= MAX_CONNECTIONS_PER_IP)
		{
			printf("disallowed connection, max per ip reached:%d:%s\n", MAX_CONNECTIONS_PER_IP, inet_ntoa(temp_in.sin_addr));
			uni_close_socket(temp_id);
			temp_id = -1;
		}
		
		//if accept fails, try again
		if(temp_id > -1) //got one so lets put him in
		{
			//find an empty slot
			for(i=0;i<MAX_ADMIN_CON;i++)
				if (client_admin[i].connected == 0)
			{
				u_long on_mode = 1;
				//async it
#ifdef _WIN32
				ioctlsocket(temp_id, FIONBIO, &on_mode);
#else		
				ioctl(temp_id, FIONBIO, &on_mode);
#endif
				//set stuff
				client_admin[i].c_id = temp_id;
				client_admin[i].c_in = temp_in;
				
				//clean up
				client_admin[i].recv_amt = 0;
				client_admin[i].recv_data[0] = 0;
				client_admin[i].sending_mutex = 0;
			
				//make sure of this
				admin_con_id[i] = -1;
				
				//set con_top if required
				if (i > admin_con_top) admin_con_top = i;
				
				//official
				client_admin[i].connected = 1;
				
				printf("adm-connection %d accepted from %s\n",i,inet_ntoa(temp_in.sin_addr));
				
				break;
			}

			//if no slots found then close the good fool
			if(i==MAX_ADMIN_CON)
				uni_close_socket(temp_id);
		}
		
		//accept an user?
		c_temp_addrlen = sizeof(temp_in);
		temp_id = accept(user_s,(struct sockaddr*)  &temp_in, &c_temp_addrlen);

		if(temp_id > -1 && active_connections(&(temp_in.sin_addr)) >= MAX_CONNECTIONS_PER_IP)
		{
			printf("disallowed connection, max per ip reached:%d:%s\n", MAX_CONNECTIONS_PER_IP, inet_ntoa(temp_in.sin_addr));
			uni_close_socket(temp_id);
			temp_id = -1;
		}
		
		//if accept fails, try again
		if(temp_id > -1) //got one so lets put him in
		{
			//find an empty slot
			for(i=0;i<MAX_USER_CON;i++)
				if (client_user[i].connected == 0)
			{
				u_long on_mode = 1;
				//async it
#ifdef _WIN32
				ioctlsocket(temp_id, FIONBIO, &on_mode);
#else		
				ioctl(temp_id, FIONBIO, &on_mode);
#endif
				//log it off just incase
				user_logoff(i);
				
				//set stuff
				client_user[i].c_id = temp_id;
				client_user[i].c_in = temp_in;
				
				//clean up
				client_user[i].recv_amt = 0;
				client_user[i].recv_data[0] = 0;
				client_user[i].sending_mutex = 0;
			
				//make sure of this
				user_con_id[i] = -1;
				
				//set con_top if required
				if (i > user_con_top) user_con_top = i;
				
				//official
				client_user[i].connected = 1;
				
				//send shit
				con_user_connect(i);
				
				printf("usr-connection %d accepted from %s\n",i,inet_ntoa(client_user[i].c_in.sin_addr));
				
				break;
			}
			//if no slots found then close the good fool
			if(i==MAX_USER_CON)
				uni_close_socket(temp_id);
		}
		
		//accept an server?
		c_temp_addrlen = sizeof(temp_in);
		temp_id = accept(server_s,(struct sockaddr*)  &temp_in, &c_temp_addrlen);
		if(temp_id > -1 && active_connections(&(temp_in.sin_addr)) >= MAX_CONNECTIONS_PER_IP)
		{
			printf("disallowed connection, max per ip reached:%d:%s\n", MAX_CONNECTIONS_PER_IP, inet_ntoa(temp_in.sin_addr));
			uni_close_socket(temp_id);
			temp_id = -1;
		}
		
		//if accept fails, try again
		if(temp_id > -1) //got one so lets put him in
		{
			//find an empty slot
			for(i=0;i<MAX_SERVER_CON;i++)
				if (client_server[i].connected == 0)
			{
				u_long on_mode = 1;
				//async it
#ifdef _WIN32
				ioctlsocket(temp_id, FIONBIO, &on_mode);
#else		
				ioctl(temp_id, FIONBIO, &on_mode);
#endif
				//clean it up
				server_logoff(i);
				
				//set stuff
				client_server[i].c_id = temp_id;
				client_server[i].c_in = temp_in;
				
				//clean up
				client_server[i].recv_amt = 0;
				client_server[i].recv_data[0] = 0;
				client_server[i].sending_mutex = 0;
			
				//make sure of this
				server_con_id[i] = -1;
				
				//set con_top if required
				if (i > server_con_top) server_con_top = i;
				
				//official
				client_server[i].connected = 1;
				
				printf("srv-connection %d accepted from %s\n",i,inet_ntoa(client_server[i].c_in.sin_addr));
				
				break;
			}
			
			//if no slots found then close the good fool
			if(i==MAX_SERVER_CON)
				uni_close_socket(temp_id);
		}
		
		//now do all of the packet stuff
		for(i=0;i<=user_con_top;i++)
			if(client_user[i].connected)
				handle_socket_receive(&(client_user[i]), i, process_packet_user, user_logoff);
		
		for(i=0;i<=server_con_top;i++)
			if(client_server[i].connected)
				handle_socket_receive(&(client_server[i]), i, process_packet_server, server_logoff);
		
		for(i=0;i<=admin_con_top;i++)
			if(client_admin[i].connected)
				handle_socket_receive(&(client_admin[i]), i, process_packet_admin, admin_logoff);
		
		//do main loop too..
		main_loop();
		
		//little pause..
		uni_pause(10);
	}
}

void *start_server_admin(void *nothing)
{
	int s, i;
	long int c_temp;
	struct sockaddr_in sin;
	char ip_address[100];
	//pthread_t c_thread[MAX_ADMIN_CON];
	#ifdef _WIN32
	int c_temp_addrlen;
	#else
	socklen_t c_temp_addrlen;
	#endif
	
	
	#ifdef _WIN32 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
	#endif
	/*
	while ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("server error- reason:socket purpose:admin\n");
		uni_pause(5000);
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(4800);

	while (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		printf("server error- reason:bind purpose:admin\n");
		uni_pause(5000);
	}

	while (listen(s, 5) == -1) {
		printf("server error- reason:listen purpose:admin\n");
		uni_pause(5000);
	}

	printf("server started-  port:4800 purpose:admin\n");
	*/
	
	s = setup_socket_listen(ADMIN_PORT, "admin");

	for(i=0;i<MAX_ADMIN_CON;i++)
	{
		client_admin[i].connected = 0;  //clear client list
		admin_con_id[i] = -1; //make sure no one is logged on	
	}

	while(1)
	{
		c_temp = -1;
		for(i=0;i<MAX_ADMIN_CON;i++)
			if (client_admin[i].connected == 0)
			{
				c_temp = i;
				break;
			}

		if (c_temp == -1) continue; //if no open slots avaible, look again

		c_temp_addrlen = sizeof(client_user[c_temp].c_in);
		client_admin[c_temp].c_id = accept(s,(struct sockaddr*)  &client_admin[c_temp].c_in, &c_temp_addrlen);

		if (client_admin[c_temp].c_id == -1 ) continue; //if accept fails, try again
		
		//check banning
		get_con_admin_ip(c_temp, ip_address);
		if(ip_is_banned(ip_address))
		{
			printf("adm-ban-%d-%s\n", c_temp, ip_address);
			close_con_admin(c_temp);
			continue;
		}

		if(active_connections(&(client_admin[i].c_in.sin_addr)) >= MAX_CONNECTIONS_PER_IP)
		{
			printf("adm-maxed-%d\n", c_temp);
			close_con_admin(c_temp);
			continue;
		}
		
		client_admin[c_temp].connected = 1;
		if (c_temp > admin_con_top) admin_con_top = c_temp; //set con_top if required
		//pthread_create(&c_thread[c_temp], NULL, handle_client_admin, (void *)c_temp); //start client handling in new thread
		//pthread_detach(c_thread[c_temp]);
		
	}
	//pthread_exit(NULL);
        return 0;
}

void *start_server_user(void *nothing)
{
	int s, i;
	long int c_temp;
	char ip_address[100];
	struct sockaddr_in sin;
	//pthread_t c_thread[MAX_USER_CON];
	struct client_info client_temp;
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
		printf("server error- reason:socket purpose:user\n");
		uni_pause(5000);
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(4200);

	while (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		printf("server error- reason:bind purpose:user\n");
		uni_pause(5000);
	}

	while (listen(s, 5) == -1) {
		printf("server error- reason:listen purpose:user\n");
		uni_pause(5000);
	}

	printf("server started-  port:4200 purpose:user\n");

	for(i=0;i<MAX_USER_CON;i++)
	{
		client_user[i].connected = 0;  //clear client list
		user_con_id[i] = -1; //make sure no one is logged on
		user_con_version[i] = 0; //make sure they dont have a set version already
	}
	
	c_temp_addrlen = sizeof(client_temp.c_in);
	
	while(1)
	{
		int creation_success;
		printf("client:go for attempt\n");
		client_temp.c_id = accept(s,(struct sockaddr*)  &client_temp.c_in, &c_temp_addrlen);
		printf("client:accepted\n");
		if (client_temp.c_id == -1 ) continue; //if accept fails, try again
		printf("client:check banned\n");
		if(ip_is_banned((char*)inet_ntoa(client_temp.c_in.sin_addr)))
		{
			//printf("usr-banned-%d-%s\n", c_temp, ip_address);
			close(client_temp.c_id);
			continue;
		}
		printf("client:not banned\n");
		c_temp = -1;
		for(i=0;i<MAX_USER_CON;i++)
			if (client_user[i].connected == 0)
			{
				c_temp = i;
				break;
			}
		printf("client:somethin1\n");
		if (c_temp == -1) 
		{
			char err_str[500];
		
			sprintf(err_str,"user port handler: all connections used");
				
			printd(err_str);
			uni_pause(1000);
			
			//if no open slots avaible, close... oh well
			close(client_temp.c_id);
			continue; 
		}

		//well now lets move some data..
		client_user[c_temp].c_id = client_temp.c_id;
		client_user[c_temp].c_in = client_temp.c_in;
		printf("client:data moved\n");
		//c_temp_addrlen = sizeof(client_user[c_temp].c_in);
		//client_user[c_temp].c_id = accept(s,(struct sockaddr*)  &client_user[c_temp].c_in, &c_temp_addrlen);

		//if (client_user[c_temp].c_id == -1 ) continue; //if accept fails, try again

		//check banning
		//get_con_user_ip(c_temp, ip_address);
		//if(ip_is_banned(ip_address))
		//{
		//	printf("usr-ban-%d-%s\n", c_temp, ip_address);
		//	close(client_user[c_temp].c_id);
		//	continue;
		//}
		
		if(active_connections(&(client_user[i].c_in.sin_addr)) >= MAX_CONNECTIONS_PER_IP)
		{
			printf("usr-maxed-%d\n", c_temp);
			close_con_user(c_temp);
			continue;
		}
		
		printf("client:no time for vacation\n");
		client_user[c_temp].connected = 1;
		if (c_temp > user_con_top) user_con_top = c_temp; //set con_top if required
		//creation_success = pthread_create(&c_thread[c_temp], NULL, handle_client_user, (void *)c_temp); //start client handling in new thread
		//pthread_detach(c_thread[c_temp]);
		
		printf("client:sucess\n");
		if(creation_success)
		{
			printf("Thread Creation Error:%d ENOMEM=%d EAGAIN=%d EINVAL=%d EPERM=%d\n", creation_success, ENOMEM, EAGAIN, EINVAL, EPERM);
		}

		
	}
	//pthread_exit(NULL);
        return 0;
}

void *start_server_server(void *nothing)
{
	int s, i;
	long int c_temp;
	char ip_address[100];
	struct sockaddr_in sin;
	//pthread_t c_thread[MAX_SERVER_CON];
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
		printf("server error- reason:socket purpose:server\n\n");
		uni_pause(5000);
	}

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(4000);

	while (bind(s, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		printf("server error- reason:bind purpose:server\n");
		uni_pause(5000);
	}

	while (listen(s, 5) == -1) {
		printf("server error- reason:listen purpose:server\n");
		uni_pause(5000);
	}

	printf("server started-  port:4000 purpose:server\n");

	for(i=0;i<MAX_SERVER_CON;i++)
	{
		client_server[i].connected = 0;  //clear client list
		server_con_id[i] = -1; //make sure no one is logged on
		server_con_version[i] = 0;
	}

	while(1)
	{
		c_temp = -1;
		for(i=0;i<MAX_SERVER_CON;i++)
			if (client_server[i].connected == 0)
			{
				c_temp = i;
				break;
			}

	      	if (c_temp == -1) continue; //if no open slots avaible, look again

		c_temp_addrlen = sizeof(client_user[c_temp].c_in);
		client_server[c_temp].c_id = accept(s,(struct sockaddr*)  &client_server[c_temp].c_in, &c_temp_addrlen);

		if (client_server[c_temp].c_id == -1 ) continue; //if accept fails, try again

		//check banning
		//get_con_server_ip(c_temp, ip_address);
		//if(ip_is_banned(ip_address))
		//{
		//	printf("srv-ban-%d-%s\n", c_temp, ip_address);
		//	close(client_server[c_temp].c_id);
		//	continue;
		//}
		
		if(active_connections(&(client_server[i].c_in.sin_addr)) >= MAX_CONNECTIONS_PER_IP)
		{
			printf("srv-maxed-%d\n", c_temp);
			close_con_server(c_temp);
			continue;
		}
		
                client_server[c_temp].connected = 1;
		if (c_temp > server_con_top) server_con_top = c_temp; //set con_top if required
		//pthread_create(&c_thread[c_temp], NULL, handle_client_server, (void *)c_temp); //start client handling in new thread
		//pthread_detach(c_thread[c_temp]);

	}
	//pthread_exit(NULL);
        return 0;
}

void *handle_client_admin(void *clientid)
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	long int c_num;

	c_num = (long int)clientid;

        client_admin[c_num].recv_amt = 0; //clear any left over data collected from a previous connection
        client_admin[c_num].recv_data[0] = '\0';
	client_admin[c_num].sending_mutex = 0;

	printf("Adm-connection %d accepted from %s\n",c_num,inet_ntoa(client_admin[c_num].c_in.sin_addr));
		
	while(1)  // if you rcv 0 data, the connection closed
	{
		buf[0] = '\0';

		rcv_amt = recv(client_admin[c_num].c_id, buf, MAX_BUF_SIZE, 0);
		
		if(rcv_amt <= 0) break;
		
		//dos
		dejunk_bytes(buf, rcv_amt);

		if (rcv_amt + client_admin[c_num].recv_amt >= MAX_DATA_STORED) //if by adding more data you excede the space given, then trash the previous 29000 chars.
                {
                        client_admin[c_num].recv_amt = 0;
                        client_admin[c_num].recv_data[0] = '\0';
                }

		memcpy(client_admin[c_num].recv_data + client_admin[c_num].recv_amt, buf, rcv_amt); //add new data onto any previous data
		client_admin[c_num].recv_amt += rcv_amt;
		client_admin[c_num].recv_data[client_admin[c_num].recv_amt] = 0;

		temp_amt = 0;

		for(i=0;i<client_admin[c_num].recv_amt;) //run through collected data and check for processable data
                {
			int prev_amt;
			
			prev_amt = i;
			
                        split(buf,client_admin[c_num].recv_data,13,&i);
			
			if(client_admin[c_num].recv_data[i] == 0 && i < client_admin[c_num].recv_amt) i++;

                        if ((i >= client_admin[c_num].recv_amt) && (client_admin[c_num].recv_data[client_admin[c_num].recv_amt - 1] != 13)) //if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
                        {
				temp_amt = client_admin[c_num].recv_amt - prev_amt;
				memcpy(client_admin[c_num].recv_data, buf, temp_amt);
                        }
                        else if (buf[0] != '\0') //why process a blank string?
                        {
				try
				{process_packet_admin(c_num, buf);} // found in proto.c
				catch(int e)
				{
					char err_str[1000];
					char ip[1000];
					
					get_con_admin_ip(c_num,ip);
					
					sprintf(err_str,"admin crash ip:%s msg:%s",ip,buf);
					
					printd(err_str);
				}
                        }

                }
                
		client_admin[c_num].recv_data[temp_amt] = '\0';
		client_admin[c_num].recv_amt = temp_amt;
	}
	
	admin_logoff(c_num);

	close_con_admin(c_num);

	printf("adm-connection %d closed\n",c_num);
	
	//pthread_exit(NULL);
        return 0;
}

void *handle_client_user(void *clientid)
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	long int c_num;
	
	c_num = (long int)clientid;

        client_user[c_num].recv_amt = 0; //clear any left over data collected from a previous connection
        client_user[c_num].recv_data[0] = '\0';
	client_user[c_num].sending_mutex = 0;

	printf("Usr-connection %d accepted from %s\n",c_num,inet_ntoa(client_user[c_num].c_in.sin_addr));
	
	con_user_connect(c_num); //send client initial stuff
	
	while(1)  // if you rcv 0 data, the connection closed
	{
		buf[0] = '\0';
		
		rcv_amt = recv(client_user[c_num].c_id, buf, MAX_BUF_SIZE, 0);
		if(rcv_amt <= 0) break;

		//dos
		dejunk_bytes(buf, rcv_amt);
		
		
		if (rcv_amt + client_user[c_num].recv_amt >= MAX_DATA_STORED) //if by adding more data you excede the space given, then trash the previous 29000 chars.
                {
                        client_user[c_num].recv_amt = 0;
                        client_user[c_num].recv_data[0] = '\0';
                }
		

		memcpy(client_user[c_num].recv_data + client_user[c_num].recv_amt, buf, rcv_amt); //add new data onto any previous data
		client_user[c_num].recv_amt += rcv_amt;
		client_user[c_num].recv_data[client_user[c_num].recv_amt] = 0;
		
		temp_amt = 0;

		for(i=0;i<client_user[c_num].recv_amt;) //run through collected data and check for processable data
                {
			int prev_amt;
			
			prev_amt = i;
			
                        split(buf,client_user[c_num].recv_data,13,&i);
			
			if(client_user[c_num].recv_data[i] == 0 && i < client_user[c_num].recv_amt) i++;
			
                        if ((i >= client_user[c_num].recv_amt) && (client_user[c_num].recv_data[client_user[c_num].recv_amt - 1] != 13)) //if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
                        {
				temp_amt = client_user[c_num].recv_amt - prev_amt;
				memcpy(client_user[c_num].recv_data,buf, temp_amt);
                        }
                        else if (buf[0] != '\0') //why process a blank string?
                        {
				try
                                {process_packet_user(c_num, buf);} // found in proto.c
				catch(int e)
				{
					char err_str[1000];
					char ip[1000];
					
					get_con_user_ip(c_num,ip);
					
					sprintf(err_str,"user crash ip:%s msg:%s",ip,buf);
					
					printd(err_str);
				}
                        }

                }
		
		client_user[c_num].recv_data[temp_amt] = '\0';
		client_user[c_num].recv_amt = temp_amt;
	}
	
	user_logoff(c_num);
	
	close_con_user(c_num);

	printf("usr:connection %d closed\n",c_num);
	
	//pthread_exit(NULL);
        return 0;
}

void *handle_client_server(void *clientid)
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	long int c_num;

	c_num = (long int)clientid;

    client_server[c_num].recv_amt = 0; //clear any left over data collected from a previous connection
    client_server[c_num].recv_data[0] = '\0';
	client_server[c_num].sending_mutex = 0;

	printf("Srv-connection %d accepted from %s\n",c_num,inet_ntoa(client_server[c_num].c_in.sin_addr));
		
	while(1)  // if you rcv 0 data, the connection closed
	{
		buf[0] = '\0';

		rcv_amt = recv(client_server[c_num].c_id, buf, MAX_BUF_SIZE, 0);
		if(rcv_amt <= 0) break;
		
		//dos
		dejunk_bytes(buf, rcv_amt);

		if (rcv_amt + client_server[c_num].recv_amt >= MAX_DATA_STORED) //if by adding more data you excede the space given, then trash the previous 29000 chars.
                {
                        client_server[c_num].recv_amt = 0;
                        client_server[c_num].recv_data[0] = '\0';
                }

		memcpy(client_server[c_num].recv_data + client_server[c_num].recv_amt, buf, rcv_amt); //add new data onto any previous data
		client_server[c_num].recv_amt += rcv_amt;
		client_server[c_num].recv_data[client_server[c_num].recv_amt] = 0;

		temp_amt = 0;

		for(i=0;i<client_server[c_num].recv_amt;) //run through collected data and check for processable data
                {
			int prev_amt;
			
			prev_amt = i;
			
                        split(buf,client_server[c_num].recv_data,13,&i);
			
			if(client_server[c_num].recv_data[i] == 0 && i < client_server[c_num].recv_amt) i++;

                        if ((i >= client_server[c_num].recv_amt) && (client_server[c_num].recv_data[client_server[c_num].recv_amt - 1] != 13)) //if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
                        {
				temp_amt = client_server[c_num].recv_amt - prev_amt;
				memcpy(client_server[c_num].recv_data,buf, temp_amt);
                        }
                        else if (buf[0] != '\0') //why process a blank string?
                        {
				try
                                {process_packet_server(c_num, buf);} // found in proto.c
				catch(int e)
				{
					char err_str[1000];
					char ip[1000];
					
					get_con_server_ip(c_num,ip);
					
					sprintf(err_str,"server crash ip:%s msg:%s",ip,buf);
					
					printd(err_str);
				}
                        }

                }
                
		client_server[c_num].recv_data[temp_amt] = '\0';
		client_server[c_num].recv_amt = temp_amt;
	}
	
	server_logoff(c_num);
	
	close_con_server(c_num);

	printf("connection %d closed\n",c_num);
	
	//pthread_exit(NULL);
        return 0;
}

int poll_for_send(int con_id)
{
#ifdef _WIN32
	fd_set fd_wset;
	fd_set fd_eset;
	int ret_val;
	
	FD_ZERO(&fd_wset);
	FD_SET(con_id, &fd_wset);
	FD_ZERO(&fd_eset);
	FD_SET(con_id, &fd_eset);
	
	ret_val = select(con_id + 1, NULL, &fd_wset, &fd_eset, NULL);
	
	if(ret_val > 0 && FD_ISSET(con_id, &fd_wset))
	{
		printf("socket good\n");
		return 1;
	}
	else if(ret_val > 0 && FD_ISSET(con_id, &fd_eset))
	{
		printf("socket bad\n");
		return 0;
	}
	if(ret_val == -1)
	{
		printf("socket bad\n");
		return 0;
	}
	
	printf("socket ? %d\n", ret_val);
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

void send_con_admin(int con, char* message)
{
        char c[2] = {13,'\0'};
	int error_no;
	
	if(!client_admin[con].connected)
		return;
	
	if(!pause_for_send(client_admin[con].c_id))
	{
		client_admin[con].connected = 0;
		uni_close_socket(client_admin[con].c_id);
		admin_logoff(con);
		printf("socket disconnected:adm:%d\n", con);
		return;
	}
	
	strcat(message,c);
	error_no = send(client_admin[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		client_admin[con].connected = 0;
		uni_close_socket(client_admin[con].c_id);
		admin_logoff(con);
		printf("socket disconnected:adm:%d\n", con);
	}
}

void send_con_user(int con, char* message)
{
        char c[2] = {13,'\0'};
	char rtn_str[1060];
	int error_no;
	
	if(!client_user[con].connected)
		return;
	
	if(!pause_for_send(client_user[con].c_id))
	{
		client_user[con].connected = 0;
		uni_close_socket(client_user[con].c_id);
		user_logoff(con);
		printf("socket disconnected:usr:%d\n", con);
		return;
	}
	
	if (user_con_id[con] != -1 && user[user_con_id[con]].debug == 1) //if the user is being debuged by someone..
	{
		sprintf(rtn_str, "2,user-snd:%s:%s", user[user_con_id[con]].username, message);
		send_user_debug(con,rtn_str);
	}
	
	printf("usr-snt-%d:%s\n", con, message);

	strcat(message,c);
	error_no = send(client_user[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		client_user[con].connected = 0;
		uni_close_socket(client_user[con].c_id);
		user_logoff(con);
		printf("socket disconnected:usr:%d\n", con);
	}
}

void raw_send_con_user(int con, char* message)
{
	char rtn_str[1060];
	int error_no;
	
	if(!client_user[con].connected)
		return;
	
	if(!pause_for_send(client_user[con].c_id))
	{
		client_user[con].connected = 0;
		uni_close_socket(client_user[con].c_id);
		user_logoff(con);
		printf("socket disconnected:usr:%d\n", con);
		return;
	}

	if (user_con_id[con] != -1 && user[user_con_id[con]].debug == 1) //if the user is being debuged by someone..
	{
		sprintf(rtn_str, "2,user-snd:%s:%s", user[user_con_id[con]].username, message);
		send_user_debug(con,rtn_str);
	}
	printf("usr-snt-%d:%s\n", con, message);
	error_no = send(client_user[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		client_user[con].connected = 0;
		uni_close_socket(client_user[con].c_id);
		user_logoff(con);
		printf("socket disconnected:usr:%d\n", con);
	}
}

void close_con_admin(int con)
{
	uni_close_socket(client_admin[con].c_id);
	client_admin[con].connected = 0;
}

void close_con_user(int con)
{
	uni_close_socket(client_user[con].c_id);
	client_user[con].connected = 0;
}

void close_con_server(int con)
{
	uni_close_socket(client_server[con].c_id);
	client_server[con].connected = 0;
}

void send_con_server(int con, char* message)
{
        char c[2] = {13,'\0'};
	int error_no;
	
	if(!client_server[con].connected) return;
	
	if(!pause_for_send(client_server[con].c_id))
	{
		client_server[con].connected = 0;
		uni_close_socket(client_server[con].c_id);
		server_logoff(con);
		printf("socket disconnected:srv:%d\n", con);
		return;
	}
	
	//printf("srv-snt-%d:%s\n", con, message);
	strcat(message,c);
	error_no = send(client_server[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		client_server[con].connected = 0;
		uni_close_socket(client_server[con].c_id);
		server_logoff(con);
		printf("socket disconnected:srv:%d\n", con);
	}
}

void raw_send_con_server(int con, char* message)
{
	int error_no;
	
	if(!client_server[con].connected)
		return;
	
	if(!pause_for_send(client_server[con].c_id))
	{
		client_server[con].connected = 0;
		uni_close_socket(client_server[con].c_id);
		server_logoff(con);
		printf("socket disconnected:srv:%d\n", con);
		return;
	}

	//printf("srv-snt-%d:%s\n", con, message);

	error_no = send(client_server[con].c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
	{
		client_server[con].connected = 0;
		uni_close_socket(client_server[con].c_id);
		server_logoff(con);
		printf("socket disconnected:srv:%d\n", con);
	}
}

void get_con_admin_ip(int con, char *dest)
{
	strcpy(dest,(char*)inet_ntoa(client_admin[con].c_in.sin_addr));
}

void get_con_user_ip(int con, char *dest)
{
	strcpy(dest,(char*)inet_ntoa(client_user[con].c_in.sin_addr));
}

void get_con_server_ip(int con, char *dest)
{
	strcpy(dest,(char*)inet_ntoa(client_server[con].c_in.sin_addr));
}

char *ip_from_s_addr(unsigned long s_addrw, char *dest)
{
	struct in_addr temp_in_addr;
	
	temp_in_addr.s_addr = s_addrw;
	
	strcpy(dest, (char*)inet_ntoa(temp_in_addr));
	
	return dest;
}

int active_connections(struct in_addr *our_addr)
{
	int i, n = 0;

	for(i=0;i<=user_con_top;i++)
		if(client_user[i].connected && our_addr->s_addr ==
				 client_user[i].c_in.sin_addr.s_addr)
			n++;
	
	for(i=0;i<=admin_con_top;i++)
		if(client_admin[i].connected && our_addr->s_addr ==
				 client_admin[i].c_in.sin_addr.s_addr)
			n++;

	for(i=0;i<=server_con_top;i++)
		if(client_server[i].connected && our_addr->s_addr ==
				 client_server[i].c_in.sin_addr.s_addr)
			n++;
	
	return n;
}

int dejunk_bytes(char *buf)
{
	int i;
	
	for(i=0;buf[i];i++)
	{
		int cur;
		
		cur = buf[i];
		
		if(cur == 13) continue;
		if(cur >= 32 && cur <= 127) continue;
		
		buf[i] = 0;
		return i;
	}
	
	return i;
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
