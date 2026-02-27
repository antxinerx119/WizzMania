cmake_minimum_required(VERSION 3.16)

project(WizzMania VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# Qt6 auto-detection
if(NOT Qt6_DIR AND NOT CMAKE_PREFIX_PATH)
    set(QT6_POSSIBLE_PATHS
        "C:/Qt/6.10.2/mingw_64"
        "C:/Qt/6.10.1/mingw_64"
        "C:/Qt/6.10.1/msvc2022_64"
        "C:/Qt/6.10.0/mingw_64"
        "C:/Qt/6.9.0/mingw_64"
    )

    foreach(QT_PATH ${QT6_POSSIBLE_PATHS})
        if(EXISTS "${QT_PATH}/lib/cmake/Qt6")
            set(CMAKE_PREFIX_PATH "${QT_PATH}" CACHE PATH "Qt6 prefix path")
            message(STATUS "Qt6 found at: ${QT_PATH}")
            break()
        endif()
    endforeach()
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql)

qt_standard_project_setup()

# ---------------------------------------------------------------------------
# Shared sources (common protocol, used by both server and client)
# ---------------------------------------------------------------------------
set(COMMON_SOURCES
    src/common/Protocol.h
    src/common/Protocol.cpp
    src/common/Message.h
    src/common/Message.cpp
)

# ---------------------------------------------------------------------------
# WizzServer (console application)
# ---------------------------------------------------------------------------
set(SERVER_SOURCES
    server/main_server.cpp
    server/Server.h
    server/Server.cpp
    server/database.hpp
)

qt_add_executable(WizzServer
    MANUAL_FINALIZATION
    ${SERVER_SOURCES}
    ${COMMON_SOURCES}
)

target_include_directories(WizzServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/server
)

target_link_libraries(WizzServer PRIVATE Qt6::Core Qt6::Network Qt6::Sql)

set_target_properties(WizzServer PROPERTIES
    WIN32_EXECUTABLE FALSE
)

qt_finalize_executable(WizzServer)

# ---------------------------------------------------------------------------
# WizzClient (GUI application)
# ---------------------------------------------------------------------------
set(CLIENT_SOURCES
    client/main_client.cpp
    client/src/front/mainwindow.cpp
    client/src/front/mainwindow.h
    client/src/front/mainwindow.ui
    client/src/front/messagewindow.h
    client/src/front/messagewindow.cpp
    client/src/front/loginwindow.h
    client/src/front/loginwindow.cpp
    client/src/front/registerwindow.h
    client/src/front/registerwindow.cpp
    client/src/back/ClientNetwork.h
    client/src/back/ClientNetwork.cpp
)

qt_add_executable(WizzClient
    MANUAL_FINALIZATION
    ${CLIENT_SOURCES}
    ${COMMON_SOURCES}
)

target_include_directories(WizzClient PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
    ${CMAKE_CURRENT_SOURCE_DIR}/client/src/back
    ${CMAKE_CURRENT_SOURCE_DIR}/client/src/front
)

target_link_libraries(WizzClient PRIVATE Qt6::Widgets Qt6::Network)

set_target_properties(WizzClient PROPERTIES
    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(WizzClient)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
add_executable(TestMessage
    tests/test_message.cpp
    ${COMMON_SOURCES}
)

target_include_directories(TestMessage PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
)

target_link_libraries(TestMessage PRIVATE Qt6::Core)
