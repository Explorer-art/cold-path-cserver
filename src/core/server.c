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

cJSON* get_server_info() {
	int players = 0;
	int size = 10;
	cJSON* server_data = cJSON_CreateObject();

	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients_data[i].state == 1) {
			players++;
		}
	}

	// get size

	cJSON_AddStringToObject(server_data, "name", SERVER_NAME);
	cJSON_AddNumberToObject(server_data, "players", players);
	cJSON_AddStringToObject(server_data, "server_ip", IP);
	cJSON_AddNumberToObject(server_data, "server_port", PORT);
	cJSON_AddNumberToObject(server_data, "size", size);

	return server_data;
}

DecodedData decode_data(const char* data) {
	DecodedData decoded_data = { NULL, NULL };
	
	char* data_copy = strdup(data);
	char* token = strtok(data_copy, "`");

	if (token != NULL) {
		decoded_data.type = strdup(token);
		token = strtok(NULL, "`");

		if (token != NULL) {
			decoded_data.data = cJSON_Parse(token);
		}
	}

	free(data_copy);
	return decoded_data;
}

void free_decoded_data(DecodedData* decoded_data) {
	if (decoded_data->type) {
		free(decoded_data->type);
	}

	if (decoded_data->data) {
		cJSON_Delete(decoded_data->data);
	}
}

// Подключение игрока

void on_client_connected(int client, const char* ip, int port) {
	printf("[INFO] %s:%d connected\n", ip, port);

	strncpy(clients_data[client].ip, ip, sizeof(clients_data[client].ip) - 1);
	clients_data[client].ip[sizeof(clients_data[client].ip)] = '\0';
	clients_data[client].port = port;
	clients_data[client].state = 0;
	// clients_game_data[client] = {0};
}

void on_client_disconnected(int client, const char* ip, int port) {
	printf("[INFO] %s:%d disconnected\n", ip, port);

	// if (clients_data[client].state == 1 || clients_data[client].state == 2) {
	// 	plugin_on_player_disconnected();
	// 	update_player_list();
	// 	check_ready();
	// }

	memset(&clients_data[client], 0, sizeof(ClientData));
	// memset(clients_game_data[client], 0, sizeof(ClientGameData));
}

void on_client_data(int client, const char* data) {
	printf("[INFO] Received from %s:%d: %s\n", clients_data[client].ip, clients_data[client].port, data);

	if (server_state == 0) {
		return;
	}

	DecodedData decoded_data = decode_data(data);

	if (decoded_data.type && decoded_data.data) {
		if (strcmp(decoded_data.type, "get_server_info") == 0) {
			cJSON* server_data = get_server_info();
			cJSON_SetValuestring(cJSON_GetObjectItem(server_data, "server_ip"), cJSON_GetObjectItem(decoded_data.data, "server_ip")->valuestring);
			char* server_data_str = cJSON_PrintUnformatted(server_data);

			cJSON_Delete(server_data);

			client_send(client, server_data_str);

			free(server_data_str);

			client_kick(client);
		}
	}
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

	network_start(IP, PORT);

	set_client_connected_callback(on_client_connected);
	set_client_disconnected_callback(on_client_disconnected);
	set_client_data_callback(on_client_data);

	// Init plugins

	// set_is_player_function()
	// set_difficulty_list()
	// set_game_end_callback()
	// set_update_land_data_callback()
	// set_accept_offer_callback()

	timer = after(TIME_TO_TURN, next);

	server_state = 1;

	network_process();
}

void server_shutdown() {
	printf("[INFO] Server stopped...\n");

	delete_timer(timer);
	network_stop();
}
