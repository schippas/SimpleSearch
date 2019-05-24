/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include <mysql/mysql.h>

//A simple HTTP server
class HTTPD{
	//default port and queue length values
	int httpdPort = 8989;
	int QueueLength = 5;
	int poolC = 0;


public:	
	//constructor for a new instance of the server
	HTTPD(int port, int thread);

	//Responds to requests with appropriate data
	virtual void response(int fd, const char * document);

	void run();

	//Used when using a pool of threads to serve requests.
	int threadPool(int fd);
	
	//Processes incoming HTTP requests
	void processHTTPRequest(int fd);


};
