cmake_minimum_required(VERSION 3.25)
project(CvenZIP C)


set(CMAKE_C_STANDARD 11)
add_executable(CvenZIP main.c brutforceFile.c brutforceFile.h basicTreatment.c basicTreatment.h menu.c menu.h)

target_link_libraries(CvenZIP libzip.so)
