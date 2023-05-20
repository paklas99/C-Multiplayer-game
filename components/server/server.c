#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include "server.h"
#include "../interfaces/network_UDP.h"
#include "../interfaces/player.h"
#include "../interfaces/game.h"

typedef struct Packet_counter{
	int sent,recieved;
}Packet_counter;
typedef struct Server{
	Network_data data_sending;
	Network_data data_recieving;
	UDPsocket socket;
	size_t packet_size;
	UDPpacket* packet_sending;
	UDPpacket* packet_recieving;
	Packet_counter counter;
	IPaddress* clients[CLIENTS];
	bool quit;
}Server;
Server* initialize_Server(void);
void close(Server* s);
void connect_clients(Server* s);
void broadcast_start_signal(Server* s);

typedef struct Thread_data_UDP_send{
	IPaddress* clients[CLIENTS];
	UDPsocket socket;
	UDPpacket* packet;
	Network_data* data;
	int* packet_counter;
	bool terminate;
}Thread_data_UDP_send;
Thread_data_UDP_send* initialize_Thread_data_UDP_Send(Server* s);
int thread_UDP_send(void* tdus);

int main(int argc, char** argv){
    printf("\n---SERVER---\n");
	Server* s=initialize_Server();
	connect_clients(s);
	broadcast_start_signal(s);

	// Thread UDP send
	Thread_data_UDP_send* tdus=initialize_Thread_data_UDP_Send(s);
	SDL_Thread* new_thread_UDP_send=SDL_CreateThread(thread_UDP_send,"Thread: UDP send",(void*)tdus);
	SDL_SpinLock lock=0;

	// Main loop
	while(!s->quit){
		if(SDLNet_UDP_Recv(s->socket,s->packet_recieving)){
			memcpy(&s->data_recieving,s->packet_recieving->data,s->packet_size);
			print_packet_recieved(s->data_recieving.client_ID,s->packet_recieving,&s->counter.recieved);

			SDL_AtomicLock(&lock);

			s->data_sending.players[s->data_recieving.client_ID].x=s->data_recieving.players[s->data_recieving.client_ID].x;
			s->data_sending.players[s->data_recieving.client_ID].y=s->data_recieving.players[s->data_recieving.client_ID].y;
			s->data_sending.frame[s->data_recieving.client_ID]=s->data_recieving.frame[s->data_recieving.client_ID];
			s->data_sending.client_ID=s->data_recieving.client_ID;
			s->data_sending.has_coin_index[s->data_recieving.client_ID]=s->data_recieving.has_coin_index[s->data_recieving.client_ID];
			s->data_sending.points[s->data_recieving.client_ID]=s->data_recieving.points[s->data_recieving.client_ID];
			s->data_sending.punched[s->data_recieving.client_ID] = s->data_recieving.punched[s->data_recieving.client_ID];
			s->data_sending.got_hit[s->data_recieving.client_ID] = s->data_recieving.got_hit[s->data_recieving.client_ID];

			for(int i=0; i<COINS; i++){
				if((s->data_recieving.has_coin_index[s->data_recieving.client_ID])==i){
					s->data_sending.coins[i].position.x=s->data_recieving.coins[i].position.x;
					s->data_sending.coins[i].position.y=s->data_recieving.coins[i].position.y;
				}
			}

			s->data_sending.boost.position.x=s->data_recieving.boost.position.x;
			s->data_sending.boost.position.y=s->data_recieving.boost.position.y;
			
			SDL_AtomicUnlock(&lock);
		}
		SDL_Delay(1);
	}

	close(s);
    return 0;
}
void broadcast(IPaddress* clients[CLIENTS],UDPsocket socket,UDPpacket* packet_sending,int *packet_counter){
	for(int i=0;i<CLIENTS;i++){
		packet_sending->address.host=clients[i]->host;
		packet_sending->address.port=clients[i]->port;
		SDLNet_UDP_Send(socket,-1,packet_sending);
		print_packet_sent(i,packet_sending,packet_counter);
	}
}
bool port_is_bound(IPaddress* clients[CLIENTS],Uint16 port){
	for(int i=0;i<CLIENTS;i++)
		if(clients[i]->port==port)
			return 1;
	return 0;
}
Server* initialize_Server(void){
	printf("Initializing server...\n");
	Server* s=malloc(sizeof(Server));
	
	init_SDLNet();
	s->socket=open_socket(SERVER_PORT);
	s->packet_size=sizeof(Network_data);
	printf("packet_size=%zu.\n",s->packet_size);
	s->packet_sending=allocate_packet_memory(s->packet_size,"Sending");
	s->packet_sending->len=s->packet_size;
    s->packet_recieving=allocate_packet_memory(s->packet_size,"Recieving");
	s->packet_recieving->len=s->packet_size;
	s->counter.sent=s->counter.recieved=0;
	s->quit=false;
	s->clients[CLIENTS];
	for(int i=0;i<CLIENTS;i++){
		s->clients[i]=malloc(sizeof(IPaddress));
		s->clients[i]->host=0;
		s->clients[i]->port=0;
	}

	// Network_data initialization
	s->data_sending.client_ID=s->data_recieving.client_ID=-1;
	s->data_sending.status=s->data_recieving.status=false;
	s->data_sending.players[0].x=s->data_recieving.players[0].x=SP_P1_X;
	s->data_sending.players[0].y=s->data_recieving.players[0].y=SP_P1_Y;
	s->data_sending.players[1].x=s->data_recieving.players[1].x=SP_P2_X;
	s->data_sending.players[1].y=s->data_recieving.players[1].y=SP_P2_Y;
	s->data_sending.players[2].x=s->data_recieving.players[2].x=SP_P3_X;
	s->data_sending.players[2].y=s->data_recieving.players[2].y=SP_P3_Y;
	s->data_sending.players[3].x=s->data_recieving.players[3].x=SP_P4_X;
	s->data_sending.players[3].y=s->data_recieving.players[3].y=SP_P4_Y;
	for(int i=0; i<CLIENTS; i++){
		s->data_sending.frame[i]=7;
		s->data_sending.has_coin_index[i]=s->data_recieving.has_coin_index[i]=-1;
		s->data_sending.points[i]=s->data_recieving.points[i]=0;
		s->data_sending.got_hit[i]=s->data_recieving.got_hit[i]=0;
		s->data_sending.punched[i]=s->data_recieving.punched[i]=-1;
	}
	s->data_sending.coins[0].position.x=s->data_recieving.coins[0].position.x=SP_C0_X;
	s->data_sending.coins[0].position.y=s->data_recieving.coins[0].position.y=SP_C0_Y;
	s->data_sending.coins[1].position.x=s->data_recieving.coins[1].position.x=SP_C1_X;
	s->data_sending.coins[1].position.y=s->data_recieving.coins[1].position.y=SP_C1_Y;
	s->data_sending.coins[2].position.x=s->data_recieving.coins[2].position.x=SP_C2_X;
	s->data_sending.coins[2].position.y=s->data_recieving.coins[2].position.y=SP_C2_Y;
	s->data_sending.boost.position.x=s->data_recieving.boost.position.x=SP_BOOST_X;
	s->data_sending.boost.position.y=s->data_recieving.boost.position.y=SP_BOOST_Y;

	return s;
}
Thread_data_UDP_send* initialize_Thread_data_UDP_Send(Server* s){
	Thread_data_UDP_send* tdus=malloc(sizeof(Thread_data_UDP_send));

	for(int i=0;i<CLIENTS;i++){
		tdus->clients[i]=malloc(sizeof(IPaddress));
		tdus->clients[i]->host=s->clients[i]->host;
		tdus->clients[i]->port=s->clients[i]->port;
	}
	tdus->socket=s->socket;
	tdus->packet=s->packet_sending;
	tdus->data=&s->data_sending;
	tdus->packet_counter=&s->counter.sent;
	tdus->terminate=false;

	return tdus;
}
void close(Server* s){
	SDLNet_FreePacket(s->packet_sending);
	SDLNet_FreePacket(s->packet_recieving);
	printf("Packet memory freed.\n");
	SDLNet_UDP_Close(s->socket);
	printf("Server socket closed.\n");
	SDLNet_Quit();
    printf("SDLNet closed.\n");
}
int thread_UDP_send(void* tdus){
    Thread_data_UDP_send* p=tdus;
	SDL_SpinLock lock=0;
	int delay=DELAY;
    while(!p->terminate){
		SDL_AtomicLock(&lock);
		memcpy(p->packet->data,p->data,p->packet->len);
		broadcast(p->clients,p->socket,p->packet,p->packet_counter);
		SDL_AtomicUnlock(&lock);
		SDL_Delay(delay);
    }
    return 0;
}
void connect_clients(Server* s){
	printf("\n---UDP connections phase---\n");
	printf("Number of clients expected = %d.\n",CLIENTS);
	printf("Waiting for clients...\n");
	int i=0;
	while(i<CLIENTS){
		if(SDLNet_UDP_Recv(s->socket,s->packet_recieving)){
			print_packet_recieved(s->data_recieving.client_ID,s->packet_recieving,&s->counter.recieved);
			if(s->clients[i]->host==0 && !port_is_bound(s->clients,s->packet_recieving->address.port)){
				s->clients[i]->host=s->packet_recieving->address.host;
				s->clients[i]->port=s->packet_recieving->address.port;
				printf("clients[%d]:[%x][%x] saved!\n",i,s->clients[i]->host,s->clients[i]->port);
				
				printf("Confirming client ID...\n");
				s->packet_sending->address.host=s->clients[i]->host;
				s->packet_sending->address.port=s->clients[i]->port;
				s->data_sending.client_ID=i;
				memcpy(s->packet_sending->data,&s->data_sending,s->packet_size);
				do{
					SDLNet_UDP_Send(s->socket,-1,s->packet_sending);
					print_packet_sent(s->data_sending.client_ID,s->packet_sending,&s->counter.sent);
					if(SDLNet_UDP_Recv(s->socket,s->packet_recieving)){
						if(s->packet_recieving->address.host==s->clients[i]->host && s->packet_recieving->address.port==s->clients[i]->port){
							memcpy(&s->data_recieving,s->packet_sending->data,s->packet_size);
							print_packet_recieved(s->data_recieving.client_ID,s->packet_recieving,&s->counter.recieved);
						}
					}
					SDL_Delay(1000);
				}while(s->data_recieving.client_ID!=i);
				printf("Client %d connected!\n",s->data_recieving.client_ID);
				s->data_recieving.client_ID=-1;
				i++;
			}
		}
	}
	printf("All clients connected!\n");
}
void broadcast_start_signal(Server* s){
	printf("Broadcasting start signal...\n");
	s->data_sending.status=true;
	memcpy(s->packet_sending->data,&s->data_sending,s->packet_size);
	for(int i=0;i<5;i++)broadcast(s->clients,s->socket,s->packet_sending,&s->counter.sent);
}