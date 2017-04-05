cmake_minimum_required(VERSION 2.8)

project(ImagingSoft CXX)

add_definitions("-Wall -std=c++11")

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

if(ROOT_AUTOSEARCH)
    execute_process(
    COMMAND mdfind -name FindROOT.cmake
    COMMAND grep etc
    COMMAND sed -e "s/\\/FindROOT.cmake//"
    OUTPUT_VARIABLE RCMAKE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${RCMAKE_DIR})
else(ROOT_AUTOSEARCH)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{ROOTSYS}/etc/root/cmake)
endif(ROOT_AUTOSEARCH)

find_package(ROOT REQUIRED COMPONENTS RIO Net)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ROOT_INCLUDE_DIRS})

message ("-- ROOT include directory : ${ROOT_INCLUDE_DIR}")
message ("-- ROOT libraries : ${ROOT_LIBRARIES}")

include_directories(${CMAKE_SOURCE_DIR}/include)

#execute_process(
#COMMAND mdfind -name libInSoft.a
#COMMAND grep src
#OUTPUT_VARIABLE libInSoft_DIR
#OUTPUT_STRIP_TRAILING_WHITESPACE
#)

set(libInSoft_DIR ${CMAKE_SOURCE_DIR}/../share/build/src/libInSoft.a)
include_directories(${CMAKE_SOURCE_DIR}/../share/include)

add_subdirectory(bin)
