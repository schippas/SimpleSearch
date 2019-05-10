CXX = g++ -fPIC
NETLIBS= -lnsl

all: search use-dlopen
 
search: simple-search.o httpd.o
	g++ -g -o simple-search simple-search.o httpd.o -lnsl

simple-search.o: simple-search.h simple-search.cpp
	g++ -g -c simple-search.cpp

httpd.o: httpd.h httpd.cpp
	g++ -g -c httpd.cpp

use-dlopen: use-dlopen.o
	$(CXX) -o $@ $@.o $(NETLIBS) -ldl
