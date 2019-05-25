/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "simple-search.h"

class Webcrawler{
	int maxUrls;
	urlList **list;

public:
	//constructor
	Webcrawler(int max_urls);

	//crawl the web
	void crawl();

	//Write array of urls to database
	void writeToDatabase();
	

};
