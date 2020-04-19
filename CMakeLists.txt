cmake_minimum_required(VERSION 3.10)
project(dag-opt)

add_executable(opt opt.c common.c) 
target_link_libraries(opt PUBLIC m)

add_executable(sopt sopt.c common.c) 
target_link_libraries(sopt PUBLIC m)

include_directories(.)
