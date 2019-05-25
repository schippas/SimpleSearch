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

	//Write array of urls to database
	void writeToDatabase();
	

};
