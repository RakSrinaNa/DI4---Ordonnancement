cmake_minimum_required(VERSION 3.6)
project(Tabou)

add_definitions(-std=c99 -Wall -Werror -W -pedantic -pedantic-errors -static -g)
set_property(TARGET PROPERTY C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

file(GLOB base_SRC "src/headers/*.h" "src/*.c")
file(GLOB test_SRC "src/unit/headers/*.h" "src/unit/*.c")

set(SOURCE_FILES ${base_SRC} src/FLAGS.h ${test_SRC})
add_executable(Tabou ${SOURCE_FILES})
