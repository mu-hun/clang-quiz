cmake_minimum_required(VERSION 3.29)
project(quiz C)

set(CMAKE_C_STANDARD 11)

# Locate the json-c and OpenSSL packages
find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONC REQUIRED json-c)
include_directories(${JSONC_INCLUDE_DIRS})
link_directories(${JSONC_LIBRARY_DIRS})

add_executable(quiz main.c)

# Link the json-c and OpenSSL libraries
target_link_libraries(quiz ${JSONC_LIBRARIES})
