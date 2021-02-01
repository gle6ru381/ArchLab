cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-O0 -std=c17")

ADD_LIBRARY(MySimpleComputer STATIC memory.c command.c memory.h command.h)

SET(APP_EXE LibraryTest)

ADD_EXECUTABLE(${APP_EXE} main.c mysimplecomputer.h)

TARGET_LINK_LIBRARIES(${APP_EXE} MySimpleComputer)
