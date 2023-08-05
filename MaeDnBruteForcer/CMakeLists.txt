cmake_minimum_required(VERSION 3.23)
project(MaeDn_Analysis C)

set(CMAKE_C_STANDARD 11)

add_executable(MaeDnBruteForcer
        brute_forcer.c)

target_include_directories(MaeDnBruteForcer PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
