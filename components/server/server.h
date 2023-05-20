#ifndef server_h
#define server_h
#define PUBLIC
#define PRIVATE static
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_net.h>
#include "../interfaces/network_UDP.h"

typedef struct Broadcast{
    UDPpacket* packet_sending;
    IPaddress* clients[CLIENTS];
    UDPsocket* server_socket;
    int* packet_counter_sent;
}Broadcast;

// Private
PRIVATE void broadcast(IPaddress** client,UDPsocket socket,UDPpacket* packet_sending,int* packet_counter);
PRIVATE int get_packet_sender(IPaddress** client, Uint16 port);
PRIVATE bool port_is_bound(IPaddress** client,Uint16 port);

#endif // server_h