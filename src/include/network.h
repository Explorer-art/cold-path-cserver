#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <netinet/in.h>

struct client_data {
	int client;
	struct sockaddr_in client_addr;
};

bool network_start();
void network_stop();

#endif