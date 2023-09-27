cmake_minimum_required(VERSION 3.26)

#boost
project(chat_client)
find_package(Boost 1.65.1 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})


add_executable(chat_client ../NetServer/net_server.cpp
        ../NetCommon/net_common.h
        ../NetCommon/net_message.h
        ../NetCommon/net_server.h
        ../NetCommon/net_tsqueue.h
        ../NetCommon/net_client.h
        ../NetCommon/net_connection.h
        ../NetServer/net_server.cpp
        ../NetCommon/net_header.h)

target_link_libraries(chat_client ${Boost_LIBRARIES})

