#ifndef SERVER_H
#define SERVER_H

typedef struct {
	char ip[16];
	int port;
	int state; // Client state; 0 - connected
} ClientData;

typedef struct {
	char type[10];
	void* data;
} ClientGameData;

void server_run();
void server_shutdown();

#endif