all:
	gcc TCP client.c -o TCP client
	gcc TCP server.c -o TCP server
	gcc UDP client.c -o UDP client
	gcc UDP server.c -o UDP server
