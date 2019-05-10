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

//A simple HTTP server
class HTTPD{
	//default port and queue length values
	int httpdPort = 8989;
	int QueueLength = 5;
public:	
	//constructor for a new instance of the server
	HTTPD(int port);

	//Responds to requests with appropriate data
	virtual void response(FILE * out, const char * document);

	//Processes incoming HTTP requests
	void processHTTPRequest();


};
