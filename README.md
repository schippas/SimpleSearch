# SimpleSearch

A Simple Search engine and Webcrawler combo project created in C++. It parses out links and search terms using the webcrawler, and stores them into a MySQL database. The search engine can than search the database and deliver the results to a user. The search engine also includes an http server that uses multiple threads to serve more than 1 user or instance, if needed.

## Getting Started

### Prerequisites

You will need a MySQL server and database instance. I recommend using XAMPP. Installing via the terminal or another utility works as well.

### Configuration

You will need to create an MySQL schema named SimpleSearch, with a table named url, and  a table named words. They have the following columns:

url
* name: id_url     &nbsp;&nbsp;  datatype: INT(10)       &nbsp;&nbsp;   etc: PK and NN
* name: url_data   &nbsp;&nbsp;  datatype: VARCHAR(2048) &nbsp;&nbsp;   etc: NN
* name: url_title  &nbsp;&nbsp;  datatype: VARCHAR(2048)  &nbsp;&nbsp;  etc: default expression = ''
* name: url_desc   &nbsp;&nbsp;  datatype: VARCHAR(2048)  &nbsp;&nbsp;  etc: default expression = ''

words
* name: id_words  &nbsp;&nbsp;   datatype: INT(10)    &nbsp;&nbsp;      etc: PK, NN, and AI
* name: words_data  &nbsp;&nbsp; datatype: VARCHAR(2048)  &nbsp;&nbsp;  etc: NN
* name: words_url  &nbsp;&nbsp;  datatype: INT(10)   &nbsp;&nbsp;       etc: NN

Make sure to add an initial entry into the url table, with a valid link. This is to give the Webcrawler somewher to start from, so feel free to add whatever and however many starter links you wnat.

In HTMLParser.h lines 22-26 and httpd.h lines 50-54 you will need to provide your own data for connecting to your MySQL instance. Additionally, if you would like to change the VARCHAR(2048) variables in the database to a different size, make sure to set line 28 of httpd.h to your new value.

### How to Run

Assuming you are using a Linux environment, start by going to the project directory and typing 'make' to compile the program.

To run the Webcrawler, type './webcrawler \<maximum nember of links you want parsed\>'. Make sure that before you run it, that there is at least one url present in the database. Also note that the Webcrawler takes some time to parse out links. It works at a rate of about 100 links per minute on my virtual machine from what I've tested so far.
  
To run the Search Engine, type './simple-search -p \<port number\>. The -p option simply allows the server to run with multiple threads, so I recommend always using it. To connect to the server while it is runnning, go to localhost:<port number>, or the ip address of whatever machine you have it running on.
  
## Ideas For Future Improvements

* Make the Webcrawler reliably parse text from webpages to generate descriptions.
* Look into ways to speed up the Webcrawler and word parsing process. Potentially make it multi threaded.
* Consider making more frequent write operations in the Webcrawler, or specify a starting point for it. I'm afraid it could possibly run out of memory otherwise.
* Have the Search Engine generate multiple pages of results, instead of just having one continuous page.
* Add a timer and result counter to display how long the Search Engine takes and how many results it produces.
* Create nicer looking pages for the Search Engine.
* Create a configuration utility to allow users to easily set variables, without going into the code.
* Implement a faster page sort algorithm. Merge Sort would be O(n log n) instead of O(n^2).
* Look into better ways of ranking search results.

Email me at schippas@purdue.edu if you have any questions, comments, or suggestions.
