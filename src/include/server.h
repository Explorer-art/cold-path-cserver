#ifndef SERVER_H
#define SERVER_H

#include <utils/cJSON.h>

typedef struct {
	char ip[16];
	int port;
	int state; // Client state; 0 - connected; 1 - in_game;
} ClientData;

typedef struct {
	char* type;
	cJSON* data;
} DecodedData;

typedef struct {
	char type[10];
	void* data;
} ClientGameData;

void server_run();
void server_shutdown();

#endif
