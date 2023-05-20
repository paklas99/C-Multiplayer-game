#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include "../interfaces/menu.h"
#include "../interfaces/network_UDP.h"

typedef struct Packet_counter{
	int sent;
	int recieved;
}Packet_counter;
struct Network_UDP_type{
	char* server_host;
	int server_port;
	IPaddress* server_address;
	int client_ID;
	size_t packet_size;
	UDPsocket client_socket;
	UDPpacket* packet_sending;
	UDPpacket* packet_recieving;
	Packet_counter packet_counter;
	Network_data data_recieving;
	Network_data data_sending;
	bool quit;
}Network_UDP_type;

#define SERVER_LOOPBACK 			"127.0.0.1"

PUBLIC Network_UDP new_Network_UDP(Menu menu){
	Network_UDP n = malloc(sizeof(Network_UDP_type));
	printf("\n---NETWORK_UDP---\n");
    init_SDLNet();
	n->server_host=get_ip_join_array(menu);
	n->server_port=SERVER_PORT;
	n->client_ID=-1;
    n->packet_size=sizeof(Network_data);
    printf("Packet size = %zu B.\n",n->packet_size);
    printf("Jitter constant = %d.\n",JITTER);
    n->client_socket=open_socket(0);
    n->server_address=resolve_host(n->server_host,n->server_port);
    n->packet_sending=allocate_packet_memory(n->packet_size,"Sending");
	n->packet_sending->len=n->packet_size;
    n->packet_recieving=allocate_packet_memory(n->packet_size,"Recieving");
	n->packet_recieving->len=n->packet_size;
	n->packet_counter.recieved=0;
	n->packet_counter.sent=0;
	n->data_recieving.client_ID=-1;
	n->data_sending.client_ID=-1;
	n->data_recieving.status=false;
	n->data_sending.status=false;
	n->quit=false;
	n->packet_sending->address.host=n->server_address->host;
	n->packet_sending->address.port=n->server_address->port;

	printf("Establishing connection...\n");
	SDL_Event event;
	while(n->client_ID==-1 && !n->quit){
        while(SDL_PollEvent(&event))if(event.type==SDL_QUIT||(event.type==SDL_KEYDOWN&&event.key.keysym.scancode==SDL_SCANCODE_ESCAPE))
			n->quit=true;
		SDLNet_UDP_Send(n->client_socket,-1,n->packet_sending);
		print_packet_sent(n->client_ID,n->packet_sending,&n->packet_counter.sent);
		if(SDLNet_UDP_Recv(n->client_socket,n->packet_recieving)){
			memcpy(&n->data_recieving,n->packet_recieving->data,n->packet_size);
			n->client_ID=n->data_recieving.client_ID;
			print_packet_recieved(-1,n->packet_recieving,&n->packet_counter.recieved);
		}
		SDL_Delay(500);
	}
	printf("Connection established. I am Client %d!\n",n->client_ID);
	printf("Awaiting start signal...\n");
	while(n->data_recieving.status==false && !n->quit){
        while(SDL_PollEvent(&event))
			if(event.type==SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.scancode==SDL_SCANCODE_ESCAPE))
				n->quit=true;
		if(SDLNet_UDP_Recv(n->client_socket,n->packet_recieving)){
			print_packet_recieved(-1,n->packet_recieving,&n->packet_counter.recieved);
			memcpy(&n->data_recieving,n->packet_recieving->data,n->packet_size);
		}
	}
	printf("Start signal recieved!\n");

	return n;
}
PUBLIC int get_client_ID(Network_UDP n){
	return n->client_ID;
}
PUBLIC bool upload_player_position(Network_UDP n,int x, int y){
	n->data_sending.players[n->client_ID].x=x;
	n->data_sending.players[n->client_ID].y=y;
}
PUBLIC int download_player_position_x(Network_UDP n,int client){
	return n->data_recieving.players[client].x;
}
PUBLIC int download_player_position_y(Network_UDP n,int client){
	return n->data_recieving.players[client].y;
}
PUBLIC void upload_player_frame(Network_UDP n, int frame){
	n->data_sending.frame[n->client_ID]=frame;
	return;
}
PUBLIC int download_player_frame(Network_UDP n, int client){
	return n->data_recieving.frame[client];
}
PUBLIC void upload_has_coin_index(Network_UDP n, int has_coin_index){
	n->data_sending.has_coin_index[n->client_ID]=has_coin_index;
	return;
}
PUBLIC void upload_punched(Network_UDP n, int punched){
	n->data_sending.punched[n->client_ID]=punched;
	return;
}
PUBLIC void upload_got_hit(Network_UDP n, int got_hit){
	n->data_sending.got_hit[n->client_ID]=got_hit;
	return;
}
PUBLIC int download_has_coin_index(Network_UDP n, int client){
	return n->data_recieving.has_coin_index[client];
}
PUBLIC void upload_points(Network_UDP n, int my_points){
	n->data_sending.points[n->client_ID]=my_points;
	return;
}
PUBLIC int download_punched(Network_UDP n, int client){
	return n->data_recieving.punched[client];
}
PUBLIC int download_got_hit(Network_UDP n, int client){
	return n->data_recieving.got_hit[client];
}
PUBLIC int download_points(Network_UDP n, int client){
	return n->data_recieving.points[client];
}
PUBLIC bool upload_item_position(Network_UDP n,int coin_index,int x, int y){
	n->data_sending.coins[coin_index].position.x=x;
	n->data_sending.coins[coin_index].position.y=y;
}
PUBLIC int download_item_position_x(Network_UDP n, int item_index){
	return n->data_recieving.coins[item_index].position.x;
}
PUBLIC int download_item_position_y(Network_UDP n, int item_index){
	return n->data_recieving.coins[item_index].position.y;
}
PUBLIC bool upload_boost_position(Network_UDP n, int boost, int x, int y){
	n->data_sending.boost.position.x=x;
	n->data_sending.boost.position.y=y;
}
PUBLIC int download_boost_position_x(Network_UDP n, int boost_index){
	return n->data_recieving.boost.position.x;
}
PUBLIC int download_boost_position_y(Network_UDP n, int boost_index){
	return n->data_recieving.boost.position.y;
}
PUBLIC bool upload(Network_UDP n){
	n->data_sending.client_ID=n->client_ID;
	memcpy(n->packet_sending->data,&n->data_sending,n->packet_size);
	SDLNet_UDP_Send(n->client_socket,-1,n->packet_sending);
}

PUBLIC void init_SDLNet(void){
	if (SDLNet_Init()<0){
		fprintf(stderr,"SDLNet_Init: %s\n",SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    printf("SDLNet initialized.\n");
}
PUBLIC UDPsocket open_socket(int server_port){
    UDPsocket socket;
	if (!(socket=SDLNet_UDP_Open(server_port))){
		fprintf(stderr,"SDLNet_UDP_Open: %s\n",SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    printf("Socket opened at port %d.\n",server_port);
    return socket;
}
PUBLIC UDPpacket* allocate_packet_memory(int packet_size,char* name){
	UDPpacket* packet;
    if (!(packet = SDLNet_AllocPacket(packet_size))){
		fprintf(stderr,"SDLNet_AllocPacket: %s\n",SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    printf("Packet \"%s\" allocated %d bytes.\n",name,packet_size);
    return packet;
}
PUBLIC IPaddress* resolve_host(char IP_string[100], Uint16 port){
    IPaddress* IP=malloc(sizeof(IPaddress));
	if (SDLNet_ResolveHost(IP,IP_string,port)){
		fprintf(stderr,"SDLNet_ResolveHost(%s %d): %s\n",IP_string,port,SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
    printf("IP address resolved: [%x|%x]\n",IP->host,IP->port);
    return IP;
}
PUBLIC void print_packet_recieved(int client_ID,UDPpacket* packet,int* packet_counter){
	(*packet_counter)++;
	printf("->[%d|%x|%x|%d]\n",client_ID,packet->address.host,packet->address.port,*packet_counter);
}
PUBLIC void print_packet_sent(int client_ID,UDPpacket* packet,int* packet_counter){
	(*packet_counter)++;
	printf("<-[%d|%x|%x|%d]\n",client_ID,packet->address.host,packet->address.port,*packet_counter);
}
PUBLIC void destroy_Network(Network_UDP network){
	SDLNet_FreePacket(network->packet_recieving);
	SDLNet_FreePacket(network->packet_sending);
	printf("Packet memory freed.\n");
	SDLNet_UDP_Close(network->client_socket);
	printf("Client socket closed.\n");
	SDLNet_Quit();
    printf("SDLNet closed.\n");
	free(network);
	printf("Network destroyed.\n");
}
PUBLIC Thread_data_UDP_recieve get_thread_data_UDP(Network_UDP n){
	Thread_data_UDP_recieve tdur;
	tdur.socket=n->client_socket;
	tdur.packet=n->packet_recieving;
	tdur.data=&n->data_recieving;
	tdur.packet_counter=&n->packet_counter.recieved;
	tdur.terminate=false;
	return tdur;
}
PUBLIC void close_recieve_thread(Thread_data_UDP_recieve* tdur){
	tdur->terminate=true;
}
PUBLIC bool network_quit(Network_UDP network){
	return network->quit;
}