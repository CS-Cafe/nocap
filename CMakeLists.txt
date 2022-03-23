cmake_minimum_required(VERSION 3.21)
project(nocap_clion)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/util src/commands src/regular_expr src/state)

add_executable(nocap_clion src/main.cpp src/commands/command_db.cpp src/commands/cmd_lexer.cpp src/regular_expr/parser.cc src/regular_expr/regex.cpp)
