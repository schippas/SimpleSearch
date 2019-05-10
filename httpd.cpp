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
void HTTPD::processHTTPRequest(){

}

//default response
void HTTPD::response(FILE * out, const char * document){

}


