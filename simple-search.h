/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "httpd.h"

//This class inherits from the HTTPD class
class SimpleSearch : public HTTPD{
	//default port value
	int searchPort = 8989;
	int searchPoolC = 0;

	//mutex lock
	pthread_mutex_t search_tid1;
	pthread_mutexattr_t search_attr1;
	
public:
	//Constructor
	SimpleSearch(int port, int thread);

	//Responds to requests
	void response(int fd, const char * document);	
};
