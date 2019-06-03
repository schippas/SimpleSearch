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

	enum { START, TITLE, ANCHOR, CONTENT } state;

	//Initial State of the parser
	state = START;

	//Max length for title, url, and description buffers.
	//2048 is the maxlength the database is set to handle.
	int maxLength = 2048;
	int count = 0;
	int descCount = 0;

	//Booleans for when content is found
	int titleFound = 0;
	int descFound = 0;

	//Buffers for storing data.
	char *buf = buffer;
	char *title_buf = new char[maxLength];
	char *href_buf = new char[maxLength];
	char *desc_buf = new char[maxLength];
	char * buf_end = buffer + size;
	
	while(buf < buf_end){
		
		//switch for different states in the parser.
		//TODO: Find a way to reliably parse data for descriptions!
		switch(state){
		case START: {
			if((cmp(&buf, "<TITLE>")) && !(titleFound)){		
				state = TITLE;
			}else if(cmp(&buf, "A HREF=\"ht")){
				//only parse http links
				href_buf[0] = 'h';
				href_buf[1] = 't';
				count = 2;
				state = ANCHOR;
			}else if((cmp(&buf, "<META CONTENT=\"")) && !(descFound)){
				descCount = 0;
				//Still needs some work to parse more pages!
				state = CONTENT;
			}else{
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
				count = 0;
				onTitleFound(title_buf, currentUrl);
				//printf("%s\n", title_buf);		//for debugging
			}else{	
				if(count < maxLength){
					title_buf[count] = *buf;					
					count++;
				}
				buf++;
			}
			break;
		}
		case ANCHOR: {
			//end of href
			if(cmp(&buf, "\"")){
				state = START;
				href_buf[count] = '\0';
				count = 0;
				onAnchorFound(href_buf);
				//printf("%s\n", href_buf);		//for debugging
			}else{
				if(count < maxLength){
					href_buf[count] = *buf;
					count++;
				}
				buf++;
			}
			break;
		}
		case CONTENT: {
			if(cmp(&buf, "\" name=\"description")){
				state = START;
				desc_buf[descCount] = '\0';
				descFound = 1;
				descCount = 0;
				//printf("%s\n", desc_buf);	//for debugging
				onContentFound(desc_buf, currentUrl);
			}else if(cmp(&buf, ">")){
				state = START;
				descCount = 0;
				desc_buf[descCount] = '\0';
			}else{
				if(descCount < maxLength-1){
					desc_buf[descCount] = *buf;
					descCount++;
				}
				buf++;
			}
			break;
		}//case
		}//switch

	}//while
	
	//Make sure everything is freed!
	delete(title_buf);
	delete(href_buf);
	delete(desc_buf);
}

//Stores a website's title
void HTMLParser::onTitleFound(char *title, int count){

}

//Parses and stores a website's data
void HTMLParser::onContentFound(char *desc, int count){

}
	
//Parses and stores new urls
void HTMLParser::onAnchorFound(char *url){

}

