cmake_minimum_required(VERSION 3.11)
project(coms10008_week10)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Dtest_lists")

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
 SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -fsanitize=undefined -fsanitize=address")
endif()

add_library("hashmap" "hash_map.c")

add_executable("HashMapTest" "hash_map.c" "hash_map_test.c")
add_test (NAME HashMapTest COMMAND HashMapTest)
