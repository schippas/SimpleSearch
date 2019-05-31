/*
   SimpleSearch
   author: Sam Chippas
   email: schippas@purdue.edu
   Copyright (c) 2019
   All Rights Reserved
 */

#include <curl/curl.h>
#include "HTMLParser.h"
#include "httpd.h"

//Struct for the HTML data being received from curl.
struct parseString{
	char *data;
	size_t len;
};

//Struct for parsing out arrays of strings for search terms
struct wordList{
	char *words_data;
	int words_id;
	
};

class Webcrawler : public HTMLParser{
	int maxUrls;
	int urlCount;
	int existingUrls;
	urlList **list;
	int maxWords;
	wordList **word_list;

	//Create a MYSQL Connection for database.
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	const char *updateQuery1 = "UPDATE url SET url_title = \"";
	const char *updateQuery2 = "\", url_desc = \"";
	const char *updateQuery3 = "\" WHERE (id_url = ";
	const char *updateQuery4 = ");";

	const char *insertQuery1 = "INSERT INTO `SimpleSearch`.`url` (`url_data`, `url_title`, `url_desc`, `id_url`) ";
	const char *insertQuery2 = "VALUES (\"";
	const char *insertQuery3 = "\", \"";
	const char *insertQuery4 = "\");";

	const char *wordQuery1 = "INSERT INTO `SimpleSearch`.`words` (`words_data`, `words_url`) ";
	const char *wordQuery2 = "VALUES (\"";
	const char *wordQuery3 = "\", \"";
	const char *wordQuery4 = "\");";

public:
	//constructor
	Webcrawler(int max_urls);

	//crawl the web
	void crawl();

	//Receive the HTML Document to be parsed
	char *fetchHTML(const char *url, int *size);

	//A function that curl uses to write data to a buffer.
	static size_t curlWriteFunction(void *ptr, size_t size, size_t nmemb, parseString *buffer);

	//Write array of urls to database
	void writeToDatabase();

	//An easy way to find search terms from items in the database.
	//should be expanded in the future.
	void wordParse();

	//Stores a website's title
	void onTitleFound(char *title, int count);

	//Parses and stores a website's data
	void onContentFound(char *desc, int count);
	
	//Parses and stores new urls
	void onAnchorFound(char *url);
	

};
