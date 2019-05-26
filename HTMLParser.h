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
	
	//Parses and stores a website's data
	void onContentFound(char c);
	
	//Parses and stores new urls
	void onAnchorFound(char *url);

};
