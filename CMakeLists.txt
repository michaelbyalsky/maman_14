cmake_minimum_required(VERSION 3.25)
project(maman_14 C)

set(CMAKE_C_STANDARD 11)

add_executable(maman_14 assembler.c pre_assembler.c first_run.c first_run.h helpers.c helpers.h tables.c tables.h code.c code.h second_run.c second_run.h file_helpers.c file_helpers.h)
