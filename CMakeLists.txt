cmake_minimum_required(VERSION 3.25)
project(7zip C)

set(CMAKE_C_STANDARD 11)

#add libzip
include_directories(/usr/local/include)
link_directories(/usr/local/lib)


add_executable(7zip main.c)

target_link_libraries(7zip libzip.so)