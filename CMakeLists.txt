cmake_minimum_required(VERSION 3.10)
project(dag-opt)

add_executable(opt opt.c common.c) 
target_link_libraries(opt PUBLIC m)

add_executable(our4 our4.c common.c) 
target_link_libraries(our4 PUBLIC m)

add_executable(our5 our5.c common.c) 
target_link_libraries(our5 PUBLIC m)

add_executable(our6 our6.c common.c) 
target_link_libraries(our6 PUBLIC m)

add_executable(opt4 opt4.c common.c) 
target_link_libraries(opt4 PUBLIC m)

add_executable(opt5 opt5.c common.c) 
target_link_libraries(opt5 PUBLIC m)

add_executable(opt6 opt6.c common.c) 
target_link_libraries(opt6 PUBLIC m)

add_executable(sopt sopt.c common.c) 
target_link_libraries(sopt PUBLIC m)

include_directories(.)
