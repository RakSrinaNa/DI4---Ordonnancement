cmake_minimum_required(VERSION 3.6)
project(Tabou)

add_definitions(-std=c99 -Wall -Werror -W -pedantic -static -g)
set_property(TARGET PROPERTY C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Main.c src/headers/Utils.h
        src/unit/MainUnit.c src/unit/headers/MainUnit.h
        src/unit/UnitUtils.c src/unit/headers/UnitUtils.h

        src/Task.c src/headers/Task.h
        src/unit/TaskUnit.c src/unit/headers/TaskUnit.h

        src/Pack.c src/headers/Pack.h

        src/Instance.c src/headers/Instance.h
        src/unit/InstanceUnit.c src/unit/headers/InstanceUnit.h

        src/Parser.c src/headers/Parser.h
        src/unit/ParserUnit.c src/unit/headers/ParserUnit.h

        src/Solution.c src/headers/Solution.h
        )
add_executable(Tabou ${SOURCE_FILES})
