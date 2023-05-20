#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL_net.h>
#include "../interfaces/network_TCP.h"
#include "../interfaces/network_UDP.h"

typedef struct Packet_counter{
	int sent;
	int recieved;
}Packet_counter;

struct Network_TCP_type{
	char* server_host;
}Network_TCP_type;

PUBLIC Network_TCP new_Network_TCP(void){
	Network_TCP n = malloc(sizeof(Network_TCP_type));

	printf("\n---NETWORK_TCP---\n");

	return n;
}
PUBLIC char* get_server_host(Network_TCP n){
	return n->server_host;
}