all: clean
	gcc src/main.c -I src/include -o server

clean:
	rm -f server
