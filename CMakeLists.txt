cmake_minimum_required(VERSION 3.20)
project(SimpleSearch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(CURL REQUIRED)
find_package(unofficial-libmysql REQUIRED)

include_directories(include)

add_executable(webcrawler
    src/httpd.cpp
    src/HTMLParser.cpp
    src/webcrawler.cpp
)

target_link_libraries(webcrawler PRIVATE 
    Threads::Threads
    CURL::libcurl
    unofficial::libmysql::libmysql
)

add_executable(simple-search
    src/httpd.cpp
    src/simple-search.cpp
)

target_link_libraries(simple-search PRIVATE 
    Threads::Threads
    unofficial::libmysql::libmysql
)