/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "webcrawler.h"

//Constructor for the webcrawler
Webcrawler::Webcrawler(int max_urls){
	maxUrls = max_urls;

	//url records. needs to be expandable at some point.
	list = new urlList*[maxUrls];

	//initialize database connection
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, "127.0.0.1", "root", NULL, "SimpleSearch", 3306, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}
}

//Crawls the web for links and content
void Webcrawler::crawl(){

}

//Writes and updates data on the database
void Webcrawler::writeToDatabase(){

}

int main(int argc, char ** argv){

	int max_urls;
	
	if(argc < 2){
		printf("Error: Please pass a maximum url number!\n");
		exit(-1);
	}else{
		max_urls = atoi(argv[1]);
	}
	
	Webcrawler webcrawler(max_urls);

	webcrawler.crawl();
}
