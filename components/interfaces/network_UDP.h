#ifndef network_UDP_h
#define network_UDP_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_net.h>
#include "../interfaces/player.h"
#include "../interfaces/interactable_items.h"
#include "../interfaces/game.h"
#include "../interfaces/menu.h"

#define CLIENTS 4
#define JITTER 5

#define SERVER_PORT 4000

typedef struct Network_UDP_type* Network_UDP;

typedef struct Network_position{
	int x,y;
}Network_position;
typedef struct Network_coins{
	Network_position position;
}Network_coins;
typedef struct Network_boost{
	Network_position position;
}Network_boost;
typedef struct Network_data{
	Network_position players[PLAYERS];
	Network_coins coins[COINS];
	Network_boost boost; 
	int frame[CLIENTS];
	int client_ID;
	bool status;
	int has_coin_index[CLIENTS];
	int punched[CLIENTS];
	int got_hit[CLIENTS];
	int points[CLIENTS];
}Network_data;

typedef struct Thread_data_UDP_recieve{
	UDPsocket socket;
	UDPpacket* packet;
	Network_data* data;
	int* packet_counter;
	bool terminate;
}Thread_data_UDP_recieve;

// Public
PUBLIC Network_UDP new_Network_UDP(Menu menu);
PUBLIC int get_client_ID(Network_UDP network);
PUBLIC void destroy_Network(Network_UDP network);
PUBLIC bool upload_player_position(Network_UDP network,int x,int y);
PUBLIC void upload_player_frame(Network_UDP n, int frame);
PUBLIC void upload_has_coin_index(Network_UDP n, int has_coin_index);
PUBLIC int download_has_coin_index(Network_UDP n, int client);
PUBLIC void upload_points(Network_UDP n, int my_points);
PUBLIC int download_points(Network_UDP n, int client);
PUBLIC bool upload_item_position(Network_UDP network,int item_index,int x,int y);
PUBLIC bool upload_boost_position(Network_UDP network, int boost_index, int x, int y);
PUBLIC bool upload(Network_UDP network);
PUBLIC int download_player_position_x(Network_UDP network,int client);
PUBLIC int download_player_position_y(Network_UDP network,int client);
PUBLIC int download_player_frame(Network_UDP n, int client);
PUBLIC int download_item_position_x(Network_UDP network,int item_index);
PUBLIC int download_item_position_y(Network_UDP network,int item_index);
PUBLIC int download_boost_position_x(Network_UDP network, int boost_index);
PUBLIC int download_boost_position_y(Network_UDP network, int boost_index);
PUBLIC void print_packet_recieved(int client_ID,UDPpacket* packet,int* packet_counter);
PUBLIC void print_packet_sent(int client_ID,UDPpacket* packet,int* packet_counter);
PUBLIC Thread_data_UDP_recieve get_thread_data_UDP(Network_UDP n);
PUBLIC bool network_quit(Network_UDP network);
PUBLIC void close_recieve_thread(Thread_data_UDP_recieve* tdur);
PUBLIC void upload_punched(Network_UDP n, int punched);
PUBLIC int download_punched(Network_UDP n, int client);
PUBLIC int download_got_hit(Network_UDP n, int client);
PUBLIC void upload_got_hit(Network_UDP n, int got_hit);
PUBLIC void upload_points(Network_UDP n, int my_points);

// Shared with server.c
PUBLIC void init_SDLNet(void);
PUBLIC UDPsocket open_socket(int port);
PUBLIC UDPpacket* allocate_packet_memory(int packet_size, char* name);
PUBLIC IPaddress* resolve_host(char* IP, Uint16 port);

#endif // network_h