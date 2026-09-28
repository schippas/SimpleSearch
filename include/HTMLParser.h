/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#ifndef SIMPLESEARCH_HTMLPARSER_H_
#define SIMPLESEARCH_HTMLPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class HTMLParser{

public:

	//Credentials for the MySQL database
	//Change as needed.
	const char *mysql_address = "127.0.0.1";
	const char *mysql_user = "root";
	const char *mysql_pass = NULL;
	const char *mysql_schema = "SimpleSearch";
	int mysql_port = 3306;

	//Parses HTML data to be inserted into the database.
	void parse(char *buffer, int size, int count);

	//Function to compare text.
	int cmp(char **buf, const char *m);
	
	//Stores a website's title
	virtual void onTitleFound(char *title, int count);

	//Parses and stores a website's data
	virtual void onContentFound(char *desc, int count);
	
	//Parses and stores new urls
	virtual void onAnchorFound(char *url);

};

#endif //SIMPLESEARCH_HTMLPARSER_H_
