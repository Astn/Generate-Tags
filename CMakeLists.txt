cmake_minimum_required(VERSION 3.9)
project(generateRaceC99 C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(ccan/tal)

include_directories(ccan/tal/)

add_executable(generateRaceC99 src/main.c src/config.c src/entrant_t.c)

target_link_libraries(generateRaceC99 tal)