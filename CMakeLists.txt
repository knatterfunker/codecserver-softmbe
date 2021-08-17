cmake_minimum_required (VERSION 3.6)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project (codecserver-libsoftmbe VERSION 0.0.1)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

#include(GNUInstallDirs)
include(FindProtobuf)

find_package(Protobuf 3.0 REQUIRED)
find_package(CodecServer REQUIRED)
include_directories("/usr/include/codecserver") # FIXME: cmake should do this on its own

SET(CMAKE_CXX_FLAGS_DEBUG "-g -O3 -rdynamic")
SET(CMAKE_C_FLAGS_DEBUG "-g -O3")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -rdynamic")
SET(CMAKE_C_FLAGS_RELEASE "-O3")

add_subdirectory(src)
