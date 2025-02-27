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
#include <config.h>

int server;

void* client_handle(void* data) {
	return NULL;
}

void network_process() {
	int client;
	struct sockaddr_in client_addr = {0};
	socklen_t addr_len = sizeof(client_addr);

	while (true) {
		// Connect accept
		client = accept(server, (struct sockaddr*)&client_addr, &addr_len);

		struct in_addr addr = client_addr.sin_addr;
		char* client_ip = inet_ntoa(addr);
		int client_port = ntohs(client_addr.sin_port);

		if (client == -1) {
			perror("[ERROR] %s:%d connection failed\n");
			exit(EXIT_FAILURE);
		}

		printf("[INFO] %s:%d connected\n", client_ip, client_port);

		struct client_data *data = malloc(sizeof(struct client_data));

		data->client = client;
		data->client_addr = client_addr;

		pthread_t thread;

		pthread_create(&thread, NULL, client_handle, data);
	}
}

bool network_start() {
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
	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_port = htons(PORT);

	printf("[INFO] IP: %s\n", IP);
	printf("[INFO] PORT: %d\n", PORT);

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

void network_stop() {
	printf("[INFO] TCP server stopped\n");
	close(server);
}
