/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "httpd.h"

	//Used when using a pool of threads to serve requests.
	void *threadPool(int fd);

	//mutex lock
	pthread_mutex_t tid1;
	pthread_mutexattr_t attr1;

//Constructr for HTTP Server
HTTPD::HTTPD(int port, int thread){
	httpdPort = port;
	poolC = thread;
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
	
	//If the application is being threaded, it skips this step and calls a new function.
	if(!poolC){
		//Loop that accepts incoming connections indefinitely.
		while(1){
			struct sockaddr_in clientIPAddress;
			socklen_t alen = sizeof(clientIPAddress);
			int slaveSocket = accept(masterSocket, 
			(struct sockaddr *)&clientIPAddress, &alen);

			if(slaveSocket < 0){
				perror("Accept");
				exit(-1);
			}
			//process request
			processHTTPRequest(slaveSocket);
			close(slaveSocket);
		}
	}

	else{
		//Adds concurrency to the http server
		pthread_attr_t attr;
		pthread_mutexattr_init(&attr1);
		pthread_mutex_init(&tid1, &attr1);
		pthread_attr_init(&attr);

		pthread_t tid[QueueLength];
		for(int i=0; i<QueueLength; i++){
			pthread_create(&tid[i], &attr, (void* (*)(void*))threadPool(masterSocket), 
			(void *)masterSocket);
		}
		pthread_join(tid[0], NULL);
	}
}

//Used when using a pool of threads to serve requests.
int HTTPD::threadPool(int fd){
	//accept incoming connections
	while(1){
		pthread_mutex_lock(&tid1);
		struct sockaddr_in clientIPAddress;
		socklen_t alen = sizeof(clientIPAddress);
		int slaveSocket = accept(fd, 
		(struct sockaddr *)&clientIPAddress, &alen);
    		pthread_mutex_unlock(&tid1);

		if(slaveSocket < 0){
			if(errno = EINTR){
				continue;
			}
			perror("accept");
			exit(-1);
		}
		
		processHTTPRequest(slaveSocket);
		close(slaveSocket);
	}
}

//Processes incoming HTTP requests
void HTTPD::processHTTPRequest(int fd){
	//Buffer used to store the line received from the client
	const int maxSize = 1024;
	char data[maxSize+1];
	char request[maxSize+1];
	int dataLength = 0;
	int reqLength = 0;

	//boolean to track when to copy request.
	int getRequest = 0; 

	//current character being read
	unsigned char newChar;

	//last characters read
	unsigned char lastChar[3];
	lastChar[0] = '\0';
	lastChar[1] = '\0';
	lastChar[2] = '\0';
	int n;
	//Reads new characters, and looks for the <CR><LF> from the client
	while((dataLength < maxSize) && (n = read(fd, &newChar, sizeof(newChar))>0)){

		if (lastChar[0] == '\015' && lastChar[1] == '\012' 
		&& lastChar[2] == '\015' && newChar == '\012') {
			break;
		}
		
		//parses request
		if(getRequest == 1){
			request[reqLength] = newChar;
			reqLength++;
		}

		//Skips the 'GET' statement, and ends after the request has ended.
		if(newChar == ' '){
			if(getRequest == 0){
				getRequest = 1;
			}else{
				request[reqLength-1] = '\0';
				getRequest++;
			}
		}

		data[dataLength] = newChar;
		dataLength++;
		lastChar[0] = lastChar[1];
		lastChar[1] = lastChar[2];
		lastChar[2] = newChar;		
	} 
	//add null character to end of string
	data[dataLength] = 0;
	
	//printf( "request=%s\n", data );		//for debugging

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


