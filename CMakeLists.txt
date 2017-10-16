cmake_minimum_required(VERSION 3.8)
project(reS)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_BUILD_TYPE Release)
set(SOURCE_FILES main.c)
set(TEST_FILE opt.c)
#add_executable(testProgram ${TEST_FILE})
add_executable(reS ${SOURCE_FILES})