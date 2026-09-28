/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include "webcrawler.h"

//Constructor for the webcrawler
Webcrawler::Webcrawler(int max_urls){
	maxUrls = max_urls;
	urlCount = 0;
	existingUrls = 0;

	//URL Record inizialization
	list = new urlList*[maxUrls];
	for(int i=0; i<maxUrls; i++){
		list[i] = new urlList;
		list[i]->words_url = 0;
	}

	//initialize database connection
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn, mysql_address, mysql_user, mysql_pass, mysql_schema, mysql_port, NULL, 0)){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}

	//Ensure that the database can reconnect after waiting a while
	my_bool reconnect = 0;
	mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);

	//SQL request for receiving urls in the database
	if(mysql_query(conn, "SELECT * FROM url")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}	

	//store results
	res = mysql_use_result(conn);

	//Fills array with initial urls
	for(int i = 0; i<maxUrls; i++){
		if((row = mysql_fetch_row(res)) != NULL){
			list[i]->words_url = atoi(row[0]);
			list[i]->url_data = strdup(row[1]);
			urlCount++;
			existingUrls++;
		}
		
	}

	mysql_free_result(res);
}

//Crawls the web for links and content
void Webcrawler::crawl(){
	int size;
	int count = 0;
	while(count < urlCount){
		char *buffer = fetchHTML(list[count]->url_data, &size);	
		if(buffer != NULL){
			parse(buffer, size, count);
		}
		free(buffer);
		count++;
	}
	
}



//Receive the HTML Document to be parsed, using curl commands
char *Webcrawler::fetchHTML(const char *url, int *size){
	CURLcode res;
	CURL * curl;

	//buffer holding HTML data from curl
	parseString buffer;
	buffer.len = 0;
	buffer.data = (char*)malloc(buffer.len+1);
	buffer.data[0] = '\0';

	//initialize curl
	if(curl_global_init(CURL_GLOBAL_DEFAULT) != 0){
		fprintf(stderr, "Curl Initialization Failed!\n");
		exit(-1);
	}
	curl = curl_easy_init();

	//Sets curl operation parameters
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteFunction); 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

	//Prevents problems from slow or unresponsive sites.
	//Use a higher number than 10 if you have problems.
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

	res = curl_easy_perform(curl);
	
    	if (res != CURLE_OK) {
        	fprintf(stderr, "Curl Failed!: %s\n", curl_easy_strerror(res));
    	}

	curl_easy_cleanup(curl);

	*size = buffer.len;

	return buffer.data;	
}

//A function that curl uses to write data to a buffer.
size_t Webcrawler::curlWriteFunction(void *ptr, size_t size, size_t nmemb, parseString *buffer){
	size_t temp_len = buffer->len + (size * nmemb);
	buffer->data = (char*)realloc(buffer->data, temp_len+1);

	//copies data over
	memcpy(buffer->data + buffer->len, ptr, size * nmemb);
	buffer->data[temp_len] = '\0';
	buffer->len = temp_len;

	return size*nmemb;
}

//Writes and updates data on the database
void Webcrawler::writeToDatabase(){
	
	int error = 0;

	//Reconnect to the database, in case it times out during the webcrawl.
	mysql_ping(conn);

	//Update Existing URLs with new information.	
	//TODO: Find more efficient way to do query.
	for(int i = 0; i < existingUrls; i++){
		do{	
			char *temp = new char[strlen(updateQuery1) + strlen(updateQuery2) 
			+ strlen(updateQuery3) + strlen(updateQuery4) + MAX_LENGTH + MAX_LENGTH]();

			strcat(temp, updateQuery1);

			std::stringstream temp_str;
			temp_str<<(list[i]->url_title);
			std::string str = temp_str.str();
			const char* cstr2 = str.c_str();
			strcat(temp, cstr2);

			strcat(temp, updateQuery2);

			std::stringstream temp_str2;
			temp_str2<<(list[i]->url_title);
			std::string str2 = temp_str2.str();
			const char* cstr3 = str2.c_str();
			strcat(temp, cstr3);

			strcat(temp, updateQuery3);

			std::stringstream temp_str3;
			temp_str3<<(list[i]->words_url);
			std::string str3 = temp_str3.str();
			const char* cstr4 = str3.c_str();
			strcat(temp, cstr4);

			strcat(temp, updateQuery4);

			const char *query = temp;

			//printf("%s\n", query);	//used for debugging

			error = 0;

			if(mysql_query(conn, query)){
				fprintf(stderr, "%s\n", mysql_error(conn));
				error = 1;
				free(list[i]->url_title);
				list[i]->url_title = strdup("Invalid Title");
				//delete(temp);
				//return;
			}

			delete(temp);
		}while(error);
	}//for
	for(int i = existingUrls; i < urlCount; i++){
		do{
			char *temp = new char[strlen(insertQuery1) + strlen(insertQuery2) 
			+ strlen(insertQuery3) + strlen(insertQuery4) + MAX_LENGTH 
			+ MAX_LENGTH + MAX_LENGTH]();

			strcat(temp, insertQuery1);

			strcat(temp, insertQuery2);

			std::stringstream temp_str;
			temp_str<<(list[i]->url_data);
			std::string str = temp_str.str();
			const char* cstr2 = str.c_str();
			strcat(temp, cstr2);

			strcat(temp, insertQuery3);

			std::stringstream temp_str2;
			temp_str2<<(list[i]->url_title);
			std::string str2 = temp_str2.str();
			const char* cstr3 = str2.c_str();
			strcat(temp, cstr3);

			strcat(temp, insertQuery3);

			std::stringstream temp_str3;
			temp_str3<<(list[i]->url_title);
			std::string str3 = temp_str3.str();
			const char* cstr4 = str3.c_str();
			strcat(temp, cstr4);

			strcat(temp, insertQuery3);

			std::stringstream temp_str4;
			temp_str4<<(list[i]->words_url);
			std::string str4 = temp_str4.str();
			const char* cstr5 = str4.c_str();
			strcat(temp, cstr5);

			strcat(temp, insertQuery4);

			const char *query = temp;

			//printf("%s\n", query);	//used for debugging
			
			error = 0;

			if(mysql_query(conn, query)){
				fprintf(stderr, "%s\n", mysql_error(conn));
				error = 1;
				free(list[i]->url_title);
				list[i]->url_title = strdup("Invalid Title");
				//delete(temp);
				//return;
			}

			delete(temp);
		}while(error);
	}
	
}

//An easy way to find search terms from items in the database.
//TODO:Overhaul word parsing to be more comprehensive.
//Note: this is just going by title right now.
void Webcrawler::wordParse(){


	//Truncate word table. Easy way to avoid duplicate entries for now.
	if(mysql_query(conn, "TRUNCATE TABLE SimpleSearch.words;")){
		fprintf(stderr, "%s\n", mysql_error(conn));
		return;
	}

	int wordCount = 0;
	maxWords = 1024;
	char *word = new char[MAX_LENGTH];
	word_list = new wordList*[maxWords];
	for(int i=0; i<maxWords; i++){
		word_list[i] = new wordList;
		word_list[i]->words_id = 0;
	}

	for(int i = 0; i < urlCount; i++){
		word = strtok(list[i]->url_title, " ");
		while(word != NULL){
			//printf("%s\n", word);		//for debugging
			if(wordCount < maxWords){
				word_list[wordCount]->words_data = strdup(word);
				word_list[wordCount]->words_id = i+1;
				wordCount++;
			}else{
				//Resizes word list array when the max number of words has been hit.
				wordList **temp_words = new wordList*[maxWords];
				for(int i=0; i<maxWords; i++){
					temp_words[i] = new wordList;
					temp_words[i]->words_data = word_list[i]->words_data;
					temp_words[i]->words_id = word_list[i]->words_id;
				}

				delete[] word_list;
				
				word_list = new wordList*[maxWords*2];
				for(int i=0; i<maxWords; i++){
					word_list[i] = new wordList;
					word_list[i]->words_data = temp_words[i]->words_data;
					word_list[i]->words_id = temp_words[i]->words_id;
				}
				for(int i=maxWords; i<maxWords*2; i++){
					word_list[i] = new wordList;
					word_list[i]->words_id = 0;
				}	

				maxWords = maxWords * 2;				

				delete[] temp_words;
			}
			word = strtok(NULL, " ");
		}

	}

	//Reconnect to the database, in case it times out during the webcrawl.
	mysql_ping(conn);

	//Write words to database
	for(int i = 0; i < wordCount; i++){
		char *temp = new char[strlen(wordQuery1) + strlen(wordQuery2) + strlen(wordQuery3)
		+ strlen(wordQuery4) + MAX_LENGTH + sizeof(int)]();

		strcat(temp, wordQuery1);
		strcat(temp, wordQuery2);
		
		std::stringstream temp_str;
		temp_str<<(word_list[i]->words_data);
		std::string str = temp_str.str();
		const char* cstr2 = str.c_str();
		strcat(temp, cstr2);

		strcat(temp, wordQuery3);

		std::stringstream temp_str2;
		temp_str2<<(word_list[i]->words_id);
		std::string str2 = temp_str2.str();
		const char* cstr3 = str2.c_str();
		strcat(temp, cstr3);

		strcat(temp, wordQuery4);

		const char *query = temp;

		if(mysql_query(conn, query)){
			fprintf(stderr, "%s\n", mysql_error(conn));
			//delete(temp);
			//return;
		}

		delete(temp);
	}
	delete[] word_list;
	delete(word);

}

//Stores a website's title
void Webcrawler::onTitleFound(char *title, int count){

	list[count]->url_title = strdup(title);
}

//Parses and stores a website's data
//Description is not really parsed and used, as of now.
void Webcrawler::onContentFound(char *desc, int count){
	
	//store the url description in the list to be written later.
	list[count]->url_desc = strdup(desc);
}
	
//Parses and stores new urls
void Webcrawler::onAnchorFound(char *url){
	int duplicate = 0;	

	//Stores mew urls in the list to be written later.
	if(urlCount < maxUrls){

		//This prevents duplicates, because the same site could be parsed with
		//and without the '/'.
		if(url[strlen(url)-1] == '/'){
			url[strlen(url)-1] = '\0';
		}

		for(int i = 0; i<urlCount; i++){
			if(!strcmp(url, list[i]->url_data)){
				duplicate = 1;
			}
		}
		//Checks to make sure there are no duplicate links.
		if(!duplicate){
			list[urlCount]->url_data = strdup(url);
			list[urlCount]->words_url = urlCount+1;
			//printf("%s, %d\n", url, urlCount);		//for debugging.
			urlCount++;
		}
	}
}

int main(int argc, char ** argv){

	int max_urls;
	
	if((argc < 2) || (argc > 2)){
		printf("Usage: ./webcrawler \'number of links\'\n");
		exit(-1);
	}else{
		max_urls = atoi(argv[1]);
	}
	
	Webcrawler webcrawler(max_urls);

	webcrawler.crawl();

	webcrawler.writeToDatabase();

	webcrawler.wordParse();

	//make sure to free or delete everything!
}

