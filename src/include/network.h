#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <netinet/in.h>

typedef struct {
	int client;
	struct sockaddr_in client_addr;
} ClientSocketData;

typedef void (*client_connected_callback_t)(int client, const char* ip, int port);
typedef void (*client_disconnected_callback_t)(int client, const char* ip, int port);
typedef void (*client_data_callback_t)(int client, const char* data);

bool network_start(const char* ip, int port);
void network_process();
void network_stop();
void client_send(int client, const char* data);
void client_kick(int client);
void set_client_connected_callback(client_connected_callback_t callback);
void set_client_disconnected_callback(client_disconnected_callback_t callback);
void set_client_data_callback(client_data_callback_t callback);

#endif
