/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "httpd.h"

//Constructr for HTTP Server
HTTPD::HTTPD(int port){
	httpdPort = port;
}

//Runs continously to process new requests
void HTTPD::run(){
	//Set IP address and port for the Server
	struct sockaddr_in serverIPAddress;
	memset(&serverIPAddress, 0, sizeof(serverIPAddress));
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) httpdPort);

	//Allocate a socket
	int masterSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(masterSocket < 0){
		perror("Socket");
		exit(-1);
	}

	//Reuse port
	int optval = 1;
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &optval, sizeof(int));

	//bind address to the socket
	if(bind(masterSocket, (struct sockaddr *)&serverIPAddress, sizeof(serverIPAddress))<0){
		perror("Bind");
		exit(-1);
	}

	//set socket to listening mode
	if(listen(masterSocket, QueueLength)<0){
		perror("Listen");
		exit(-1);
	}

	//Loop that accepts incoming connections indefinitely.
	while(1){
		struct sockaddr_in clientIPAddress;
		socklen_t alen = sizeof(clientIPAddress);
		int slaveSocket = accept(masterSocket, (struct sockaddr *)&clientIPAddress, &alen);

		if(slaveSocket < 0){
			perror("Accept");
			exit(-1);
		}
		printf("hi");
		//process request
		processHTTPRequest(slaveSocket);
		close(slaveSocket);
	}
}

//Processes incoming HTTP requests
void HTTPD::processHTTPRequest(int fd){
	printf("hello!");
}

//default response
void HTTPD::response(FILE * out, const char * document){

}


