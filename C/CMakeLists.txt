cmake_minimum_required(VERSION 3.6)
project(Tabou)

add_definitions(-std=c99 -Wall -Werror -W -pedantic -static -g)
set_property(TARGET PROPERTY C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.c
        src/unit/mainUnit.c src/unit/headers/mainUnit.h
        src/unit/unit.c src/unit/headers/unit.h
        src/headers/utils.h
        )
add_executable(Tabou ${SOURCE_FILES})
