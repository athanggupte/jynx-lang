cmake_minimum_required(VERSION 3.19)

project(Jynx VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(jynx STATIC
	include/jynx/common.h
    include/jynx/lex.h
    include/jynx/vm.h
    src/lex.cpp
    src/vm.cpp
)

target_include_directories(jynx PUBLIC include)

add_executable(jynx-lex src/lexer.cpp)
target_link_libraries(jynx-lex jynx)
