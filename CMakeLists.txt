cmake_minimum_required(VERSION 3.19)
project(LazyDiyDict)

set(CMAKE_CXX_STANDARD_REQUIRED 11)

set(DIR_LIBRARY_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(DIR_HEADERS "${DIR_LIBRARY_ROOT}/include")
set(DIR_SOURCES "${DIR_LIBRARY_ROOT}/src")


add_library(LazyDiyDict STATIC
    ${DIR_HEADERS}/LazyDiyDict.hpp
    ${DIR_SOURCES}/LazyDiyDict.cpp
)
add_library(LazyDiyDict::LazyDiyDict ALIAS LazyDiyDict)

target_include_directories(LazyDiyDict PUBLIC ${DIR_HEADERS})

include(GNUInstallDirs)

install(TARGETS LazyDiyDict
        EXPORT LazyDiyDict_targets
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
export( EXPORT LazyDiyDict_targets
        NAMESPACE LazyDiyDict::
        FILE "${CMAKE_INSTALL_LIBDIR}/cmake/LazyDiyDict/LazyDiyDict-config.cmake")
