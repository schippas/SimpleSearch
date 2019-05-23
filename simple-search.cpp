/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "simple-search.h"

//Constructor for simple search and HTTP server
SimpleSearch::SimpleSearch(int port, int thread):HTTPD(port, thread){
	searchPort = port;
	searchPoolC = thread;
}

//responds to requests
void SimpleSearch::response(int fd, const char * document){
	//initial HTML page
	//note, needs some work passing characters like '?' in search
	const char * text = 
	   "<TITLE>Simple Search</Title>"
	   "<CENTER><H1><em>Simple Search</em></H1>"
	   "<H2>\n<FORM ACTION=\"search\"> Search:\n <INPUT TYPE=\"text\" NAME=\"word\"></FORM></H2>"
	   "</CENTER>";
	
	write(fd, text, strlen(text));

}

int main(int argc, char ** argv){
	//boolean for if the server is threaded
	int thread = 0;

	int port;

	//checks for correct amount of arguments
	if(argc < 2){
		printf("Error: Please pass a port number!");
		exit(-1);
	}else if(argc > 2){
		if(argv[1][1] == 'p'){
			thread = 1;	
		}
		port = atoi(argv[2]);
	}else{
	//Get port from arguments
		port = atoi(argv[1]);
	}

	SimpleSearch search(port, thread);

	search.run();
}
