#include <stdio.h>
#include <stdlib.h>
#include <server.h>

int main() {
	server_run();
	server_shutdown();

	return 0;
}
