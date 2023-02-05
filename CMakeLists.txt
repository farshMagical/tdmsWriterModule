cmake_minimum_required(VERSION 3.22.0) 

set(TARGET tdmsWriterModule)

set(SOURCE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include_directories(${SOURCE_DIRS})

foreach(DIR ${SOURCE_DIRS})
    aux_source_directory(${DIR} TARGET_SOURCES)
endforeach()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++1z -Wall -g")

add_library(${TARGET} STATIC ${TARGET_SOURCES})
