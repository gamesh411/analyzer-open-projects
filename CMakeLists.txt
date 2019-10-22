cmake_minimum_required(VERSION 2.8.9)
project(analyzer_open_projects)
set(CMAKE_BUILD_TYPE Release)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES "*.cpp")
add_library(analyzer_open_projects STATIC ${SOURCES})
