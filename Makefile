all: clean
	gcc src/main.c src/core/server.c src/network/network.c -I src/include -o server

clean:
	rm -f server
