# Remove strings matching given regular expression from a list.
# @param(in,out) aItems Reference of a list variable to filter.
# @param aRegEx Value of regular expression to match.
function (filter_items aItems aRegEx)
    # For each item in our list
    foreach (item ${${aItems}})
        # Check if our items matches our regular expression
        if ("${item}" MATCHES ${aRegEx})
            # Remove current item from our list
            list (REMOVE_ITEM ${aItems} ${item})
        endif ("${item}" MATCHES ${aRegEx})
    endforeach(item)
    # Provide output parameter
    set(${aItems} ${${aItems}} PARENT_SCOPE)
endfunction (filter_items)

cmake_minimum_required(VERSION 3.6)
project(Tabou)

add_definitions(-std=c99 -Wall -Werror -W -pedantic -pedantic-errors -static -g)
set_property(TARGET PROPERTY C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

file(GLOB base_SRC "src/prod/headers/*.h" "src/prod/*.c")
file(GLOB test_SRC "src/unit/headers/*.h" "src/unit/*.c")
filter_items(base_SRC "src/prod/MainTest.cpp")
filter_items(base_SRC "src/prod/Main.cpp")

set(SOURCE_FILES_PROD ${base_SRC} src/prod/FLAGS.h src/prod/Main.c)
set(SOURCE_FILES_TEST ${base_SRC} src/prod/FLAGS.h src/prod/MainTests.c ${test_SRC})

add_executable(TabouProd ${SOURCE_FILES_PROD})
add_executable(TabouTest ${SOURCE_FILES_TEST})
