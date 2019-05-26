/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "HTMLParser.h"

//Function to compare text.
int HTMLParser::cmp(char **buf, const char *c){
	int temp_len = strlen(c);
	if(strncasecmp(*buf, c, temp_len)){
		return 0;
	}else{
		(*buf)+=temp_len;
		return 1;
	}
}

//Parses HTML data to be inserted into the database.
void HTMLParser::parse(char *buffer, int size){

	enum { START } state;

	state = START;
	
	char *buf = buffer;
	char * buf_end = buffer + size;
	
	while(buf < buf_end){
		
		//switch for different states in the parser.
		switch(state){
		case START: {
			
		}//case
		}//switch

	}//while

}

//Stores a website's title
void HTMLParser::onTitleFound(char *title){

}

//Parses and stores a website's data
void HTMLParser::onContentFound(char c){

}
	
//Parses and stores new urls
void HTMLParser::onAnchorFound(char *url){

}

