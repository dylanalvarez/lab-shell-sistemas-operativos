cmake_minimum_required(VERSION 3.5)
project(lab-shell)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -Wpedantic -std=c99")
# -Werror is pending until some functions are actually implemented
set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(shell src/main.c src/builtin.c src/createcmd.c src/exec.c src/freecmd.c src/parsing.c src/printstatus.c src/readline.c src/runcmd.c src/utils.c)
