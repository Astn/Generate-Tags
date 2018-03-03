cmake_minimum_required(VERSION 3.9)
project(generateRaceC99 C)

set(CMAKE_C_STANDARD 99)

add_executable(generateRaceC99 entrant_t.h config.h main.h main.c config.c entrant_t.c)