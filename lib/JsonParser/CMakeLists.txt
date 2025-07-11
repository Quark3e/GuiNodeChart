cmake_minimum_required(VERSION 3.19)
project(JsonParser)

set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(DIR_LIBRARY_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(DIR_HEADERS "${DIR_LIBRARY_ROOT}/include")
set(DIR_SOURCES "${DIR_LIBRARY_ROOT}/src")


add_library(JsonParser STATIC
    ${DIR_HEADERS}/JsonParser.hpp
    ${DIR_SOURCES}/JsonParser.cpp
)
add_library(JsonParser::JsonParser ALIAS JsonParser)

target_include_directories(JsonParser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib//Pos2d/include/")
target_include_directories(JsonParser PUBLIC ${DIR_HEADERS})

include(GNUInstallDirs)

install(TARGETS JsonParser
        EXPORT JsonParser_targets
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
export( EXPORT JsonParser_targets
        NAMESPACE JsonParser::
        FILE "${CMAKE_INSTALL_LIBDIR}/cmake/JsonParser/JsonParser-config.cmake")
