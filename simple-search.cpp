/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "simple-search.h"

//Constructor for simple search and HTTP server
SimpleSearch::SimpleSearch(int port, int thread):HTTPD(port, thread){
	searchPort = port;
	searchPoolC = thread;

	//initialize database connection
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, mysql_address, mysql_user, mysql_pass, mysql_schema, mysql_port, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}

	//Ensure that the database can reconnect after waiting a while
	my_bool reconnect = 0;
	mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);
}

//responds to requests
void SimpleSearch::response(int fd, const char * document){
	//initial HTML page
	if(!strcmp(document, "/")){
		const char * initialText = 
		   "<TITLE>Simple Search</Title>"
		   "<CENTER><H1><em>Simple Search</em></H1>"
		   "<H2>\n<FORM ACTION=\"search\"> Search:\n <INPUT TYPE=\"search\" NAME=\"word\">"
		   "</FORM></H2>"
		   "</CENTER>";
	
		write(fd, initialText, strlen(initialText));
	}else 
	
	//Search Page and search request parsing
	if(strncmp(document, "/search?words=", 13)){
		const char * searchText = 
		   "<TITLE>Simple Search</Title>"
		   "<CENTER><H1><em>Simple Search</em></H1>"
		   "<H2>\n<FORM ACTION=\"search\"> Search:\n <INPUT TYPE=\"search\" NAME=\"word\">"
		   "</FORM></H2>"
		   "</CENTER>";
	
		write(fd, searchText, strlen(searchText));
		
		//Words being searched
		char **words = new char*[MAX_LENGTH];
		char *word = new char[MAX_LENGTH];
		char *request = strdup(document+13);
		int wordCount = 0;

		//url records. needs to be expandable at some point.
		list = new urlList*[1024];
		for(int i=0; i<1024; i++){
			list[i] = new urlList;
			list[i]->words_url = 0;
			list[i]->relevance = 0;
		}
		int listCount = 0;
		
		//Use '+' as a delimiter to parse search words.
		word = strtok(request, "+");
		while(word != NULL){
			//printf("%s\n", word);		//for debugging
			words[wordCount] = strdup(word);
			wordCount++;
			word = strtok(NULL, "+");
		}

		//MYSQL
		for(int i = 0; i<wordCount; i++){

			//create const char* query for search
			char *temp = new char[strlen(wordsQuery)+strlen(words[i])+3]();
			strcat(temp, wordsQuery);
			strcat(temp, words[i]);
			strcat(temp, "\";");
			const char *query = temp;
			
			//search the database
			if(mysql_query(conn, query)){
				fprintf(stderr, "%s\n", mysql_error(conn));
				delete(temp);
				return;
			}
			
			//store results
			res = mysql_use_result(conn);
			int duplicate = 0;
			while((row = mysql_fetch_row(res)) != NULL){
				//check for duplicate entries
				for(int j=0; j<1024; j++){
					if(list[j]->words_url == atoi(row[0])){
						duplicate = 1;
						list[j]->relevance++;
					}
				}
				if(duplicate == 0){
					list[listCount]->words_url = atoi(row[0]); 
					listCount++;
				}
				duplicate = 0;	
			}
			
			mysql_free_result(res);

			delete(temp);
		}

		//Reconnect to the database, in case it times out.
		mysql_ping(conn);

		//SQL Query to get url information from the database
		for(int i=0; i<listCount; i++){

			//create const char* query for search
			char *temp = new char[strlen(urlQuery)+sizeof(list[i]->words_url)+3]();
			strcat(temp, urlQuery);
			std::stringstream temp_str;
			temp_str<<(list[i]->words_url);
			std::string str = temp_str.str();
			const char* cstr2 = str.c_str();
			strcat(temp, cstr2);
			strcat(temp, ";");
			const char *query = temp;

			//search the database
			if(mysql_query(conn, query)){
				fprintf(stderr, "%s\n", mysql_error(conn));
				delete(temp);
				return;
			}

			//store results
			res = mysql_use_result(conn);
			
			while((row = mysql_fetch_row(res)) != NULL){
				list[i]->url_data = strdup(row[0]);

				//Includes error checking for Blank values.
				if(strcmp(row[1], "")){
					list[i]->url_title = strdup(row[1]);
				}else{
					list[i]->url_title = strdup(row[0]);
				}
				if(strcmp(row[2], "")){
					list[i]->url_desc = strdup(row[2]);
				}else{
					list[i]->url_desc = strdup("No Description");
				}
			}

			mysql_free_result(res);			

			delete(temp);
			
		}	

		//Sort the links
		relevanceSort(listCount);

		//Display the webpage.
		char * text = new char[10000];
		for ( int i = 0; i < listCount; i++ ) {
			strcat(text, "<h3>");
			std::stringstream temp_str;
			temp_str<<(i+1);
			std::string str = temp_str.str();
			const char* cstr2 = str.c_str();
			strcat(text, cstr2);			
			strcat(text, " <a href=\"");
			strcat(text, list[i]->url_data);
			strcat(text, "\">");
			strcat(text, list[i]->url_title);	
			strcat(text,  "</a><h3>\n<blockquote>");
			strcat(text, list[i]->url_desc);
			strcat(text, "<p></blockquote>\n");
			write(fd, text, strlen(text));
			*text = '\0';
  		}

		//free memory, make sure everything is freed if MYSQL has errors!
		for(int i=0; i<wordCount; i++){
			free(words[i]);
		}
		delete(text);
		delete[] words;
		delete(word);
		delete[] list;
		free(request);
	}//if

}

//Sorts repsonses by relevance
void SimpleSearch::relevanceSort(int count){
	//TODO: This is O(n^2), so it would be cool to sort this faster.
	for(int i = 0; i<count; i++){
		for(int j = i; j<count; j++){
			if(list[j]->relevance > 0){
				if(list[j]->relevance > list[i]->relevance){
					urlList *temp = list[i];
					list[i] = list[j];
					list[j] = temp;
				}
			}
		}
	}
}

int main(int argc, char ** argv){
	//boolean for if the server is threaded
	int thread = 0;

	int port;

	//checks for correct amount of arguments
	if(argc < 2){
		printf("Error: Please pass a port number!\n");
		exit(-1);
	}else if(argc > 2){
		if(argv[1][1] == 'p'){
			thread = 1;	
		}
		port = atoi(argv[2]);
	}else{
	//Get port from arguments
		port = atoi(argv[1]);
	}


	SimpleSearch search(port, thread);

	search.run();
}
