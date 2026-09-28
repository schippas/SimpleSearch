/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#ifndef SIMPLESEARCH_HTTPD_H_
#define SIMPLESEARCH_HTTPD_H_

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

#include <sstream>

//The max length the database is currently set to handle for variables.
const int MAX_LENGTH = 2048;

//A data structure for storing url information
struct urlList{
	int words_url;
	char *url_data;
	char *url_title;
	char *url_desc;
	int relevance;
};

//A simple HTTP server
class HTTPD{
	//default port and queue length values
	int httpdPort = 8989;
	int QueueLength = 5;
	int poolC = 0;

public:	

	//Credentials for the MySQL database
	//Change as needed.
	const char *mysql_address = "127.0.0.1";
	const char *mysql_user = "root";
	const char *mysql_pass = NULL;
	const char *mysql_schema = "SimpleSearch";
	int mysql_port = 3306;


	//constructor for a new instance of the server
	HTTPD(int port, int thread);

	//Responds to requests with appropriate data
	virtual void response(int fd, const char * document);

	//Runs continously to process new requests
	void run();

	//Used when using a pool of threads to serve requests.
	int threadPool(int fd);
	
	//Processes incoming HTTP requests
	void processHTTPRequest(int fd);


};

#endif //SIMPLESEARCH_HTTPD_H_
