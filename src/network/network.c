#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <network.h>

#define BUFFER_SIZE 1024

int server;
client_connected_callback_t client_connected_callback = NULL;
client_disconnected_callback_t client_disconnected_callback = NULL;
client_data_callback_t client_data_callback = NULL;

void set_client_connected_callback(client_connected_callback_t callback) {
	client_connected_callback = callback;
}

void set_client_disconnected_callback(client_disconnected_callback_t callback) {
	client_disconnected_callback = callback;
}

void set_client_data_callback(client_data_callback_t callback) {
	client_data_callback = callback;
}

void* client_handle(void* data) {
	ClientSocketData* client_socket_data = data;
	struct in_addr addr = client_socket_data->client_addr.sin_addr;
	char* client_ip = inet_ntoa(addr);
	int client_port = ntohs(client_socket_data->client_addr.sin_port);

	if (client_connected_callback != NULL) {
		client_connected_callback(client_socket_data->client, client_ip, client_port);
	}

	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;

	while (true) {
		bytes_received = recv(client_socket_data->client, buffer, sizeof(buffer) - 1, 0);

		if (bytes_received > 0) {
			buffer[bytes_received] = '\0';

			if (client_data_callback != NULL) {
				client_data_callback(client_socket_data->client, buffer);
			}
		} else if (bytes_received < 0) {
			fprintf(stderr, "[ERROR] %s:%d recv error\n", client_ip, client_port);
			break;
		} else {
			break;
		}
	}

	if (client_disconnected_callback != NULL) {
		client_disconnected_callback(client_socket_data->client, client_ip, client_port);
	}

	close(client_socket_data->client);
	free(client_socket_data);

	return NULL;
}

bool network_start(const char* ip, int port) {
	printf("[INFO] TCP server starting...\n");

	// Create socket
	server = socket(AF_INET, SOCK_STREAM, 0);

	if (server == -1) {
		perror("[ERROR] TCP server start failed\n");
		return false;
	}

	// Create struct sockaddr_in
	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	printf("[INFO] IP: %s\n", ip);
	printf("[INFO] PORT: %d\n", port);

	int res;
	socklen_t addr_len = sizeof(server_addr);

	// Bind
	res = bind(server, (struct sockaddr*)&server_addr, addr_len);

	if (res == -1) {
		perror("[ERROR] Bind failed\n");
		return false;
	}

	// Listen
	res = listen(server, MAX_CLIENTS);

	if (res == -1) {
		perror("[ERROR] Listen failed\n");
		return false;
	}

	printf("[INFO] TCP server started!\n");

	return true;
}

void network_process() {
	int client;
	struct sockaddr_in client_addr = {0};
	socklen_t addr_len = sizeof(client_addr);

	while (true) {
		// Connect accept
		client = accept(server, (struct sockaddr*)&client_addr, &addr_len);

		if (client == -1) {
			perror("[ERROR] %s:%d connection failed\n");
			exit(EXIT_FAILURE);
		}

		ClientSocketData* client_socket_data = malloc(sizeof(ClientSocketData));
		client_socket_data->client = client;
		client_socket_data->client_addr = client_addr;

		pthread_t thread;
		pthread_create(&thread, NULL, client_handle, client_socket_data);
		pthread_detach(thread);
	}
}

void network_stop() {
	printf("[INFO] TCP server stopped\n");
	close(server);
}
