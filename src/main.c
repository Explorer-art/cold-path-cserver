#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 5555
#define MAX_CLIENTS 10

struct client_data {
	int client;
	struct sockaddr_in client_addr;
};

void* client_handle(void* data) {
	return NULL;
}

int main() {
	printf("[INFO] Server starting...\n");

	// Create socket
	int server = socket(AF_INET, SOCK_STREAM, 0);

	if (server == -1) {
		perror("[ERROR] Server start failed\n");
		exit(EXIT_FAILURE);
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
		exit(EXIT_FAILURE);
	}

	// Listen
	res = listen(server, MAX_CLIENTS);

	if (res == -1) {
		perror("[ERROR] Listen failed\n");
		exit(EXIT_FAILURE);
	}

	printf("[INFO] Server started!\n");

	struct sockaddr_in client_addr = {0};
	addr_len = sizeof(client_addr);
	int client;

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

	close(server);
	
	return 0;
}
