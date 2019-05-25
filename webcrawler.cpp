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
	urlCount = 0;

	//url records. needs to be expandable at some point.
	list = new urlList*[maxUrls];
	for(int i=0; i<1024; i++){
		list[i] = new urlList;
		list[i]->words_url = 0;
	}

	//initialize database connection
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, "127.0.0.1", "root", NULL, "SimpleSearch", 3306, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}

	//SQL request for receiving urls in the database
	if(mysql_query(conn, "SELECT * FROM url")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}	

	//store results
	res = mysql_use_result(conn);

	//Fills array with initial urls
	for(int i = 0; i<maxUrls; i++){
		if((row = mysql_fetch_row(res)) != NULL){
			list[i]->words_url = atoi(row[0]);
			list[i]->url_data = strdup(row[1]);
			list[i]->url_title = strdup(row[2]);
			list[i]->url_desc = strdup(row[3]);
			urlCount++;
		}
		
	}

	mysql_free_result(res);
}

//Crawls the web for links and content
void Webcrawler::crawl(){
	while(urlCount < maxUrls){


	}

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
