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
void HTMLParser::parse(char *buffer, int size, int currentUrl){

	enum { START, TITLE } state;

	//Initial State of the parser
	state = START;

	//Max length for title, url, and description buffers. Probably change later.
	int maxLength = 2048;
	int count = 0;

	//Booleans for when content is found
	int titleFound = 0;

	char *buf = buffer;
	char *title_buf = new char[maxLength];
	char * buf_end = buffer + size;
	
	while(buf < buf_end){
		
		//switch for different states in the parser.
		switch(state){
		case START: {
			if((cmp(&buf, "<TITLE>")) && !(titleFound)){
				state = TITLE;
			}else{
				char c = *buf;
				
				buf++;
			}

			break;
		}
		case TITLE: {
			//next </TITLE>
			if(cmp(&buf, "<")){
				state = START;
				title_buf[count]  = '\0';
				titleFound = 1;
				onTitleFound(title_buf, currentUrl);
				//printf("%s\n", title_buf);		//for debugging
			}else{	
				if(count < maxLength){
					title_buf[count] = *buf;					
					count++;
				}
				buf++;
			}
		}//case
		}//switch

	}//while
	
	//Make sure everything is freed!
	delete(title_buf);
}

//Stores a website's title
void HTMLParser::onTitleFound(char *title, int count){

}

//Parses and stores a website's data
void HTMLParser::onContentFound(char c){

}
	
//Parses and stores new urls
void HTMLParser::onAnchorFound(char *url){

}

