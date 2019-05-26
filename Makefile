CXX = g++ -fPIC
NETLIBS= -lnsl

all: search webcrawler
 
search: httpd.o simple-search.o 
	$(CXX) -pthread -g -o simple-search httpd.o simple-search.o -lnsl -lmysqlclient

simple-search.o: simple-search.h simple-search.cpp
	g++ -g -c simple-search.cpp

httpd.o: httpd.h httpd.cpp
	g++ -g -c httpd.cpp

webcrawler: httpd.o webcrawler.o HTMLParser.o
	$(CXX) -pthread -g -o webcrawler httpd.o webcrawler.o HTMLParser.o -lnsl -lmysqlclient -lcurl

webcrawler.o: webcrawler.h webcrawler.cpp 
	g++ -g -c webcrawler.cpp

HTMLParser.o: HTMLParser.h HTMLParser.cpp
	g++ -g -c HTMLParser.cpp


