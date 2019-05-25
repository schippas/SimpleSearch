CXX = g++ -fPIC
NETLIBS= -lnsl

all: search webcrawler
 
search: httpd.o simple-search.o 
	$(CXX) -pthread -g -o simple-search httpd.o simple-search.o -lnsl -lmysqlclient

simple-search.o: simple-search.h simple-search.cpp
	g++ -g -c simple-search.cpp

httpd.o: httpd.h httpd.cpp
	g++ -g -c httpd.cpp

webcrawler: 
	g++ -Wall -o webcrawler -g webcrawler.cpp


