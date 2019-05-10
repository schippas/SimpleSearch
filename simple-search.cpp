/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "simple-search.h"

//Constructor for simple search and HTTP server
SimpleSearch::SimpleSearch(int port):HTTPD(port){
	searchPort = port;
}

//responds to requests
void SimpleSearch::response(FILE * out, const char * document){

}

int main(int argc, char ** argv){
	//checks for correct amount of arguments
	if(argc < 2){
		printf("Error: Please pass a port number!");
		exit(-1);
	}
	
	//Get port from arguments
	int port;
	port = atoi(argv[1]);

	SimpleSearch search(port);

	search.run();
}
