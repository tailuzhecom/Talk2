cmake_minimum_required(VERSION 3.8)
project(Talk2Server)

set(CMAKE_CXX_STANDARD 11)
set(LINK_LIST mysqlclient muduo_net muduo_base pthread)
set(SOURCE_FILES main.cpp DB.cpp DB.h json.hpp LoginSvr.cpp LoginSvr.h)
add_executable(Talk2Server ${SOURCE_FILES})
target_link_libraries(Talk2Server ${LINK_LIST})