cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-O0 -std=gnu18 -D_POSIX_C_SOURCE=200809L")

ADD_LIBRARY(MySimpleComputer STATIC memory.c command.c memory.h command.h)

ADD_LIBRARY(myTerm STATIC term.c term.h)

SET(BIGCHAR_LIB MyBigChars)

ADD_LIBRARY(${BIGCHAR_LIB} STATIC bigchars.c bigchars.h)

TARGET_LINK_LIBRARIES(${BIGCHAR_LIB} myTerm)

SET(READKEY_LIB MyReadKey)

ADD_LIBRARY(${READKEY_LIB} STATIC readkey.c readkey.h)

TARGET_LINK_LIBRARIES(${READKEY_LIB} MySimpleComputer)

SET(APP_EXE LibraryTest)

ADD_EXECUTABLE(${APP_EXE} main.c mysimplecomputer.h mainloop.c mainloop.h)

TARGET_LINK_LIBRARIES(${APP_EXE} ${READKEY_LIB} ${BIGCHAR_LIB})
