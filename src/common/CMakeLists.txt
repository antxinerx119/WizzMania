cmake_minimum_required(VERSION 3.16)

project(WizzManiaCommon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(WizzManiaCommon STATIC
    Protocol.cpp
    Message.cpp
)

target_link_libraries(WizzManiaCommon Qt6::Core)

target_include_directories(WizzManiaCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
