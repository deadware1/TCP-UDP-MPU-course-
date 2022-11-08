all:
	gcc TCPclient.c -o TCPclient
	gcc TCPserver.c -o TCPserver
	gcc UDPclient.c -o UDPclient
	gcc UDPserver.c -o UDPserver
