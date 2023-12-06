cmake_minimum_required(VERSION 3.27)
project(P1_funktion)

set(CMAKE_CXX_STANDARD 17)

add_executable(P1_funktion main.c
        Internal_Libraries/cJSON.c
        Internal_Libraries/cJSON.h
        JSON_Files/cJSON_Test_Functions.c
        JSON_Files/cJSON_Test_Functions.h
)
