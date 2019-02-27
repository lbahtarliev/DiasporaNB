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

#define MAX_DATA_STORED 120000
#define MAX_BUF_SIZE 60000

int handle_client_hub(void *nothing);
int handle_client_server(void *nothing);
int handle_connect_hub(void *nothing);
int handle_connect_server(void *nothing);

void dejunk_bytes(char *buf, int amt);

void threadless_hub_packets();
void threadless_server_packets();

struct client_info
{
	int c_id;
	struct sockaddr_in c_in;
	char recv_data[MAX_DATA_STORED];
	int recv_amt;
	int connected;
};

struct client_info client_hub;
struct client_info client_server;

void init_sockets()
{
#ifdef _M_IX86 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
#endif
	
	client_hub.c_id = -1;
	client_server.c_id = -1;
	
	client_hub.connected = 0;
	client_server.connected = 0;
	
	client_hub.recv_amt = 0;
	client_hub.recv_data[0] = 0;
	client_server.recv_amt = 0;
	client_server.recv_data[0] = 0;
}

void handle_sockets()
{
	//process connects
	threadless_hub_connect(game.hub_address);
	threadless_server_connect(game.server_address);
	
	//process hub packets
	threadless_hub_packets();
	
	//process server packets
	threadless_server_packets();
}

void threadless_hub_connect(char *address)
{
	struct hostent * host;
	u_long on_mode = 1;
	int constatus;
	
	if(!address) return;
	if(!address[0]) return;
	if(client_hub.connected) return;
	
	if(client_hub.c_id < 0)
	{
		printf("creating hub id\n");
		
		client_hub.c_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if (client_hub.c_id < 0)
		{
			if (login.visible)
			{
				draw_label(screen, "Socket creation failed.", &login.label, 255, 255, 255);
				SDL_Flip(screen);
			}
			else
			{
				strcpy(login.label.message, "Socket creation failed.");
			}
			
			return;
		}

		printf("setting hub host:%s\n", address);
		host = gethostbyname(address);
		memcpy((char*)&client_hub.c_in.sin_addr, (char*)host->h_addr, host->h_length);
		client_hub.c_in.sin_family = AF_INET;
		client_hub.c_in.sin_port = htons(4200);
			
#ifdef _WIN32
		ioctlsocket(client_hub.c_id, FIONBIO, &on_mode);
#else		
		ioctl(client_hub.c_id, FIONBIO, &on_mode);
#endif

	}
	
	//printf("attempting hub connect\n");
	
	constatus = connect(client_hub.c_id, (struct sockaddr *)&client_hub.c_in, sizeof client_hub.c_in);
	
	//leave if did not connect
#ifdef _WIN32
	if(WSAGetLastError() != 10056) return;
#else
	if(constatus < 0) return;
#endif
	
	printf("hub connected\n");
	
	client_hub.connected = 1;
	connect_hub_process();
}

void threadless_server_connect(char *address)
{
	int constatus;
	u_long on_mode = 1;
	struct hostent * host;
	
	if(!address) return;
	if(!address[0]) return;
	if(client_server.connected) return;
	
	if(client_server.c_id < 0)
	{
		client_server.c_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if (client_server.c_id < 0)
		{
			if (server_select.visible) 
			{
				draw_label(screen, "Socket creation failed.", &server_select.label, 255, 255, 255);
				SDL_Flip(screen);
			}
			else
			{
				strcpy(server_select.label.message,"Socket creation failed.");
			}
			
			return;
		}
		
		if(str_match("127.0.0.1", address))
			host = gethostbyname((char*)game.hub_address);
		else
			host = gethostbyname((char*)address);
		
		memcpy((char*)&client_server.c_in.sin_addr, (char*)host->h_addr, host->h_length);
		client_server.c_in.sin_family = AF_INET;
		client_server.c_in.sin_port = htons(4600);
		
#ifdef _WIN32
		ioctlsocket(client_server.c_id, FIONBIO, &on_mode);
#else		
		ioctl(client_server.c_id, FIONBIO, &on_mode);
#endif
	}
	
	constatus = connect(client_server.c_id, (struct sockaddr *)&client_server.c_in, sizeof client_server.c_in);
	
	//leave if did not connect
#ifdef _WIN32
	if(WSAGetLastError() != 10056) return;
#else
	if(constatus < 0) return;
#endif
	
	server_select.connecting = 0;
	server_select.connected = 1;
	client_server.connected = 1;
	
	connect_server_process();
}

void threadless_hub_packets()
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	
	if(!client_hub.connected) return;
	
	rcv_amt = recv(client_hub.c_id, buf, MAX_BUF_SIZE, 0);
	
#ifdef _WIN32
	if(WSAGetLastError() == 10054)
#else
	if(!rcv_amt)
#endif
	{
		//it closed
		close_con_hub();
	}
	else if(rcv_amt == -1)
	{
		//no packet
		return;
	}

	dejunk_bytes(buf, rcv_amt);
	
	//if by adding more data you excede the space given, then trash the previous MAX_DATA_STORED chars.
	if (rcv_amt + client_hub.recv_amt >= MAX_DATA_STORED)
	{
		client_hub.recv_amt = 0;
		client_hub.recv_data[0] = 0;
	}

	memcpy(client_hub.recv_data + client_hub.recv_amt, buf, rcv_amt); //add new data onto any previous data
	client_hub.recv_amt += rcv_amt;
	client_hub.recv_data[client_hub.recv_amt] = 0;

	temp_amt = 0;

	//run through collected data and check for processable data
	for(i=0;i<client_hub.recv_amt;)
	{
		int prev_amt;
		
		prev_amt = i;
		
		split(buf,client_hub.recv_data,13,&i);
		
		if(client_hub.recv_data[i] == 0 && i < client_hub.recv_amt) i++;
		
		//if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
		if ((i >= client_hub.recv_amt) && (client_hub.recv_data[client_hub.recv_amt - 1] != 13))
		{
			temp_amt = client_hub.recv_amt - prev_amt;
			memcpy(client_hub.recv_data,buf, temp_amt);
		}
		else if (buf[0])
		{
			process_packet_hub(buf);
		}
	}
	
	client_hub.recv_data[temp_amt] = 0;
	client_hub.recv_amt = temp_amt;
}

void threadless_server_packets()
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;
	
	if(!client_server.connected) return;

	rcv_amt = recv(client_server.c_id, buf, MAX_BUF_SIZE, 0);
	
#ifdef _WIN32
	if(WSAGetLastError() == 10054)
#else
	if(!rcv_amt)
#endif
	{
		//it closed
		close_con_server();
	}
	else if(rcv_amt == -1)
	{
		//no packet
		return;
	}

	dejunk_bytes(buf, rcv_amt);

	//if by adding more data you excede the space given, then trash the previous MAX_DATA_STORED chars.
	if (rcv_amt + client_server.recv_amt >= MAX_DATA_STORED)
	{
		client_server.recv_amt = 0;
		client_server.recv_data[0] = 0;
	}

	//add new data onto any previous data
	memcpy(client_server.recv_data + client_server.recv_amt, buf, rcv_amt);
	client_server.recv_amt += rcv_amt;
	client_server.recv_data[client_server.recv_amt] = 0;

	temp_amt = 0;

	//run through collected data and check for processable data
	for(i=0;i<client_server.recv_amt;)
	{
		int prev_amt;
		
		prev_amt = i;
		
		split(buf,client_server.recv_data,13,&i);
		
		if(client_server.recv_data[i] == 0 && i < client_server.recv_amt) i++;
		
		//if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
		if ((i >= client_server.recv_amt) && (client_server.recv_data[client_server.recv_amt - 1] != 13))
		{
			
			temp_amt = client_server.recv_amt - prev_amt;
			memcpy(client_server.recv_data,buf, temp_amt);
		}
		else if (buf[0])
		{
			process_packet_server(buf);
		}

	}

	client_server.recv_data[temp_amt] = 0;
	client_server.recv_amt = temp_amt;
}

void connect_to_hub()
{
	SDL_Thread *client_thread;
		
	if (login.visible)
	{
		draw_label(screen, "Connecting to main server...", &login.label, 255, 255, 255);
		SDL_Flip(screen);
	}
	else
	{
		strcpy(login.label.message, "Connecting to main server...");
	}

	client_thread = SDL_CreateThread(handle_connect_hub, NULL);
}

void connect_to_server(char *host_address)
{
	SDL_Thread *client_thread;
	
	if(str_match("127.0.0.1",host_address))
		client_thread = SDL_CreateThread(handle_connect_server, (void*)game.hub_address);
	else
		client_thread = SDL_CreateThread(handle_connect_server, (void*)host_address);
}

int handle_connect_hub(void *nothing)
{
	struct hostent * host;
	SDL_Thread *client_thread;

	#ifdef _M_IX86 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
	#endif
	
	if ((client_hub.c_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		if (login.visible)
		{
			draw_label(screen, "Socket creation failed.", &login.label, 255, 255, 255);
			SDL_Flip(screen);
		}
		else
		{
			strcpy(login.label.message, "Socket creation failed.");
		}
		return 0;
	}

	host = gethostbyname(game.hub_address);
 	memcpy((char*)&client_hub.c_in.sin_addr, (char*)host->h_addr, host->h_length);
 	client_hub.c_in.sin_family = AF_INET;
 	client_hub.c_in.sin_port = htons(4200);
 
 	if(connect(client_hub.c_id, (struct sockaddr *)&client_hub.c_in, sizeof client_hub.c_in) < 0)
	{
		if (login.visible)
		{
			draw_label(screen, "Connect failed, reconnecting...", &login.label, 255, 255, 255);
			SDL_Flip(screen);
		}
		else
		{
			strcpy(login.label.message, "Connect failed, reconnecting...");
		}
		
		if(connect(client_hub.c_id, (struct sockaddr *)&client_hub.c_in, sizeof client_hub.c_in) < 0)
		{
			if (login.visible)
			{
				draw_label(screen, "Reconnect failed.", &login.label, 255, 255, 255);
				SDL_Flip(screen);
			}
			else
			{
				strcpy(login.label.message, "Reconnect failed.");
			}
			return 0;
			
		}
	}
	
	client_hub.connected = 1;
	connect_hub_process();
	client_thread = SDL_CreateThread(handle_client_hub, nothing);
	
	return 0;
}

int handle_connect_server(void *message)
{
	struct hostent * host;
	SDL_Thread *client_thread;

	#ifdef _M_IX86 //if windows
	WSADATA wsaData;
	
	WSAStartup(0x202,&wsaData);
	#endif

	close_con_server();

	if ((client_server.c_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		if (server_select.visible) 
		{
			draw_label(screen, "Socket creation failed.", &server_select.label, 255, 255, 255);
			SDL_Flip(screen);
		}
		else
		{
			strcpy(server_select.label.message,"Socket creation failed.");
		}
		return 0;
	}

 	host = gethostbyname((char*)message);
 	memcpy((char*)&client_server.c_in.sin_addr, (char*)host->h_addr, host->h_length);
 	client_server.c_in.sin_family = AF_INET;
 	client_server.c_in.sin_port = htons(4600);
 
 	if(connect(client_server.c_id, (struct sockaddr *)&client_server.c_in, sizeof client_server.c_in) < 0)
	{
		if (server_select.visible) 
		{
			draw_label(screen, "Connect failed, reconnecting...", &server_select.label, 255, 255, 255);
			SDL_Flip(screen);
		}
		else
		{
			strcpy(server_select.label.message, "Connect failed, reconnecting...");
		}
		
		if(connect(client_server.c_id, (struct sockaddr *)&client_server.c_in, sizeof client_server.c_in) < 0)
		{
			if (server_select.visible) 
			{
				draw_label(screen, "Reconnect failed.", &server_select.label, 255, 255, 255);
				SDL_Flip(screen);
			}
			else
			{
				strcpy(server_select.label.message, "Reconnect failed.");
			}
			server_select.connecting = 0;
			return 0;
			
		}
	}
	
	server_select.connecting = 0;
	server_select.connected = 1;
	client_server.connected = 1;
	
	connect_server_process();
	client_thread = SDL_CreateThread(handle_client_server, NULL);
	
	return 0;
}

int handle_client_hub(void *nothing)
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;

        client_hub.recv_amt = 0; //clear any left over data collected from a previous connection
        client_hub.recv_data[0] = '\0';
		
	while(1)  // if you rcv 0 data, the connection closed
	{
		buf[0] = '\0';
		
		rcv_amt = recv(client_hub.c_id, buf, MAX_BUF_SIZE, 0);
		
		if(rcv_amt <= 0) break;

		dejunk_bytes(buf, rcv_amt);
		
		if (rcv_amt + client_hub.recv_amt >= MAX_DATA_STORED) //if by adding more data you excede the space given, then trash the previous 29000 chars.
                {
                        client_hub.recv_amt = 0;
                        client_hub.recv_data[0] = '\0';
                }

		memcpy(client_hub.recv_data + client_hub.recv_amt, buf, rcv_amt); //add new data onto any previous data
                client_hub.recv_amt += rcv_amt;
		client_hub.recv_data[client_hub.recv_amt] = 0;

		temp_amt = 0;

		for(i=0;i<client_hub.recv_amt;) //run through collected data and check for processable data
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
				process_packet_hub(buf); // found in proto.c
                        }
                }
		
		client_hub.recv_data[temp_amt] = '\0';
		client_hub.recv_amt = temp_amt;
	}
	
	//stuff to do since we lost the mothership
	close_con_hub();
	
        return 0;
}

int handle_client_server(void *nothing)
{
	char buf[MAX_BUF_SIZE + 1];
	int rcv_amt, i, temp_amt;

        client_server.recv_amt = 0; //clear any left over data collected from a previous connection
        client_server.recv_data[0] = '\0';
		
	while(1)  // if you rcv 0 data, the connection closed
	{
		buf[0] = '\0';

		rcv_amt = recv(client_server.c_id, buf, MAX_BUF_SIZE, 0);
		
		if(rcv_amt <= 0) break;

		dejunk_bytes(buf, rcv_amt);

		if (rcv_amt + client_server.recv_amt >= MAX_DATA_STORED) //if by adding more data you excede the space given, then trash the previous 29000 chars.
                {
                        client_server.recv_amt = 0;
                        client_server.recv_data[0] = '\0';
                }

		memcpy(client_server.recv_data + client_server.recv_amt, buf, rcv_amt); //add new data onto any previous data
		client_server.recv_amt += rcv_amt;
		client_server.recv_data[client_server.recv_amt] = 0;

		temp_amt = 0;

		for(i=0;i<client_server.recv_amt;) //run through collected data and check for processable data
                {
			int prev_amt;
			
			prev_amt = i;
			
			split(buf,client_server.recv_data,13,&i);
			
			if(client_server.recv_data[i] == 0 && i < client_server.recv_amt) i++;
			
			if ((i >= client_server.recv_amt) && (client_server.recv_data[client_server.recv_amt - 1] != 13)) //if you have reached the last segment and it doesnt have an end, then leave it to be processed later when it does
			{
				
				temp_amt = client_server.recv_amt - prev_amt;
				memcpy(client_server.recv_data,buf, temp_amt);
			}
			else if (buf[0] != '\0') //why process a blank string?
			{
				process_packet_server(buf); // found in proto.c
			}

                }

		client_server.recv_data[temp_amt] = '\0';
		client_server.recv_amt = temp_amt;
	}
	
	//its time
	close_con_server();
	
	return 0;
}

void send_con_hub(char *message)
{
	char c[2] = {13,'\0'};
	int error_no;
	
	if(!client_hub.connected) return;
	
	printf("snt-h-%s\n",message);
	
	strcat(message,c);
	
	error_no = send(client_hub.c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
		close_con_hub();
}

void send_con_server(char *message)
{
	char c[2] = {13,'\0'};
	int error_no;
	
	if(!client_server.connected) return;
	
	printf("snt-s-%s\n",message);
	
	strcat(message,c);
	
	error_no = send(client_server.c_id, message, strlen(message), 0);
	
	if(error_no <= 0)
		close_con_server();
}

void close_con_server()
{
	printf("disconnecting server\n");
	
	if(!client_server.connected) return;
	
#ifdef _WIN32
	closesocket(client_server.c_id);
#else
	close(client_server.c_id);
#endif
	
	client_server.connected = 0;
	server_select.connected = 0;
	
	client_server.c_id = -1;
	client_server.recv_amt = 0;
	client_server.recv_data[0] = 0;
	
	disconnect_server_process();
}

void close_con_hub()
{
	printf("disconnecting hub\n");
	
	if(!client_hub.connected) return;

#ifdef _WIN32
	closesocket(client_hub.c_id);
#else
	close(client_hub.c_id);
#endif

	client_hub.connected = 0;
	client_hub.c_id = -1;
	client_hub.recv_amt = 0;
	client_hub.recv_data[0] = 0;
	
	disconnect_hub_process();
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

