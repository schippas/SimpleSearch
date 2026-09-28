/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#ifndef SIMPLESEARCH_SIMPLESEARCH_H_
#define SIMPLESEARCH_SIMPLESEARCH_H_

#include "httpd.h"

//This class inherits from the HTTPD class
class SimpleSearch : public HTTPD{
	//default port value
	int searchPort = 8989;
	int searchPoolC = 0;

	//List of URLs to be searched.
	urlList **list; 

	//mutex lock
	pthread_mutex_t search_tid1;
	pthread_mutexattr_t search_attr1;

	//Create a MYSQL Connection for database.
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char *wordsQuery = "SELECT words_url FROM words WHERE words_data=\"";
	const char *urlQuery = "SELECT url_data, url_title, url_desc FROM url WHERE id_url=";
	
	
public:
	//Constructor
	SimpleSearch(int port, int thread);

	//Responds to requests
	void response(int fd, const char * document);
	
	//Sorts repsonses by relevance
	void relevanceSort(int count);	
};

#endif //SIMPLESEARCH_SIMPLESEARCH_H_
