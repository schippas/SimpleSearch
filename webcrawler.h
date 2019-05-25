/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include <curl/curl.h>
#include "httpd.h"

class Webcrawler{
	int maxUrls;
	int urlCount;
	urlList **list;

	//Create a MYSQL Connection for database.
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

public:
	//constructor
	Webcrawler(int max_urls);

	//crawl the web
	void crawl();

	//Receive the HTML Document to be parsed
	char *fetchHTML(const char *url, int *size);

	//A function that curl uses to write data to a buffer.
	static size_t curlWriteFunction(void *ptr, size_t size, size_t nmemb, char *buffer);

	//Write array of urls to database
	void writeToDatabase();
	

};
