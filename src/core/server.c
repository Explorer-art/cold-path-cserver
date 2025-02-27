#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <server.h>
#include <network.h>
#include <config.h>

void server_run() {
	network_start();
}

void server_shutdown() {
	network_stop();
}