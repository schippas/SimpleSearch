CXX = g++ -fPIC
NETLIBS= -lnsl

all: search 
 
search: httpd.o simple-search.o 
	$(CXX) -pthread -g -o simple-search httpd.o simple-search.o -lnsl

simple-search.o: simple-search.h simple-search.cpp
	g++ -g -c simple-search.cpp

httpd.o: httpd.h httpd.cpp
	g++ -g -c httpd.cpp


