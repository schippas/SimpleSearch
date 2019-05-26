/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class HTMLParser{

public:
	//Parses HTML data to be inserted into the database.
	void parse(char *buffer, int size);

	//Function to compare text.
	int cmp(char **buf, const char *m);
	
	//Stores a website's title
	virtual void onTitleFound(char *title);

	//Parses and stores a website's data
	virtual void onContentFound(char c);
	
	//Parses and stores new urls
	virtual void onAnchorFound(char *url);

};
