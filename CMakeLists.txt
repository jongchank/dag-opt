cmake_minimum_required(VERSION 3.10)
project(dag-opt)

add_executable(opt4 opt4.c) 
target_link_libraries(opt4 PUBLIC m)

add_executable(opt5 opt5.c) 
target_link_libraries(opt5 PUBLIC m)

add_executable(opt6 opt6.c) 
target_link_libraries(opt6 PUBLIC m)

add_executable(sopt sopt.c) 
target_link_libraries(sopt PUBLIC m)

include_directories(.)
