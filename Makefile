all: clean
	gcc src/main.c src/core/server.c src/network/network.c src/utils/timer.c src/utils/tables.c -I src/include -lrt -o server

clean:
	rm -f server
