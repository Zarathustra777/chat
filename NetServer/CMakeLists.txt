cmake_minimum_required(VERSION 3.26)

#boost
project(chat_server)
find_package(Boost 1.65.1 REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories (../NetClient)
include_directories (../NetCommon)

add_executable(chat_server net_server.cpp
)

target_link_libraries(chat_server ${Boost_LIBRARIES})

