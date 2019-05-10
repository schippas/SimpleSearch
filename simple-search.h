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

public:
	//Constructor
	SimpleSearch(int port);

	//Responds to requests
	void response(FILE * out, const char * document);	
};
