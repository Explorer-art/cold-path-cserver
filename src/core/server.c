#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <server.h>
#include <network.h>
#include <utils/timer.h>
#include <utils/tables.h>
#include <config.h>

// Состояние сервера
//
// 0 - starting
// 1 - in_game

int server_state;
ClientData clients_data[MAX_CLIENTS] = {0};
bool clients_ready[MAX_CLIENTS] = {0};
HashTable* preferred_civs;
Timer* timer;

// Подключение игрока

void on_client_connected(int client, char* ip, int port) {
	printf("[INFO] %s:%d connected\n", ip, port);

	strncpy(clients_data[client].ip, ip, sizeof(clients_data[client].ip) - 1);
	clients_data[client].ip[sizeof(clients_data[client].ip)] = '\0';
	clients_data[client].port = port;
	clients_data[client].state = 0;
	// clients_game_data[client] = {0};
}

void on_client_disconnected(int client, char* ip, int port) {
	printf("[INFO] %s:%d disconnected\n", ip, port);

	// if (clients_data[client].state == 1 || clients_data[client].state == 2) {
	// 	plugin_on_player_disconnected();
	// 	update_player_list();
	// 	check_ready();
	// }

	memset(&clients_data[client], 0, sizeof(ClientData));
	// memset(clients_game_data[client], 0, sizeof(ClientGameData));
}

// Обработка следующего хода

void next() {
	printf("Next turn!\n");
}

void check_ready(); // Проверка готовности игроков
void next_timer(); // Таймер для следующего хода

void server_run() {
	server_state = 0;
	printf("[INFO] Server starting...\n");
	preferred_civs = create_table();

	network_start();

	// Init plugins

	// set_is_player_function()
	// set_difficulty_list()
	// set_game_end_callback()
	// set_update_land_data_callback()
	// set_accept_offer_callback()

	timer = after(TIME_TO_TURN, next);

	network_process();

	server_state = 1;
}

void server_shutdown() {
	printf("[INFO] Server stopped...\n");

	delete_timer(timer);
	network_stop();
}
