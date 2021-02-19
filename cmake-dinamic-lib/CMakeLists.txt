cmake_minimum_required(VERSION 3.19.5)

project(multilang_greetings)
include_directories(include)
file(GLOB SOURCES "src/*.c")

add_library(greetings_enjavi SHARED ${SOURCES})
