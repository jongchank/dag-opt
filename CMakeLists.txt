cmake_minimum_required(VERSION 3.10)
project(dag-opt)

add_executable(opt opt.c) 
target_link_libraries(opt PUBLIC m)

include_directories(.)
