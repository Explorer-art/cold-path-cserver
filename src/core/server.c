#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <server.h>
#include <network.h>
#include <utils/timer.h>
#include <config.h>

// Server state
//
// 0 - starting
int server_state;
ClientData clients_data[MAX_CLIENTS] = {0};
bool clients_ready[MAX_CLIENTS] = {0};

void on_client_connected(int client, char *ip, int port);
void on_client_desconnected(int client);

// Обработка следующего хода
void next() {
	printf("Next turn!\n");
}

void check_ready(); // Проверка готовности игроков
void next_timer(); // Таймер для следующего хода

void server_run() {
	server_state = 0;
	printf("[INFO] Server starting...");

	network_start();

	// Init plugins

	Timer *timer = after(TIME_TO_TURN, next);

	network_process();
}

void server_shutdown() {
	network_stop();
}
