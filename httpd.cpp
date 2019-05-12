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
		//process request
		processHTTPRequest(slaveSocket);
		close(slaveSocket);
	}
}

//Processes incoming HTTP requests
void HTTPD::processHTTPRequest(int fd){
	//Buffer used to store the line received from the client
	const int maxSize = 1024;
	char request[maxSize+1];
	int length = 0;

	//current character being read
	unsigned char newChar;

	//last characters read
	unsigned char lastChar[3];
	lastChar[0] = '\0';
	lastChar[1] = '\0';
	lastChar[2] = '\0';
	int n;
	//Reads new characters, and looks for the <CR><LF> from the client
	while((length < maxSize) && (n = read(fd, &newChar, sizeof(newChar))>0)){

		if (lastChar[0] == '\015' && lastChar[1] == '\012' && lastChar[2] == '\015' && newChar == '\012') {
			break;
		}

		request[length] = newChar;
		length++;
		lastChar[0] = lastChar[1];
		lastChar[1] = lastChar[2];
		lastChar[2] = newChar;		
	} 
	//add null character to end of string
	request[length] = 0;
	
	printf( "request=%s\n", request );

	//HTML response
	const char * header = 
          "HTTP/1.1 200 Document follows\r\n"
          "Server: SimpleSearch \r\n"
          "Content-type: text/html\r\n"
          "\r\n";
          
	write(fd, header, strlen(header));
	
	const char * doc = request;
	response(fd, doc);
}

//default response
void HTTPD::response(int fd, const char * document){
	const char * text = 
	   "<TITLE>Simple Search</Title>";
	
	write(fd, text, strlen(text));
}


