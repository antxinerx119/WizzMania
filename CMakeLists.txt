cmake_minimum_required(VERSION 3.16)

project(Mess VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# Configuration de Qt 6 avec auto-detection
if(NOT Qt6_DIR AND NOT CMAKE_PREFIX_PATH)
    # Essayer les emplacements courants
    set(QT6_POSSIBLE_PATHS
        "C:/Qt/6.10.1/mingw_64"
        "C:/Qt/6.10.1/msvc2022_64"
        "C:/Qt/6.10.0/mingw_64"
        "C:/Qt/6.9.0/mingw_64"
        "C:/Tools/Qt/6.10.1/mingw_64"
        "${CMAKE_CURRENT_SOURCE_DIR}/Qt"
    )
    
    foreach(QT_PATH ${QT6_POSSIBLE_PATHS})
        if(EXISTS "${QT_PATH}/lib/cmake/Qt6")
            set(CMAKE_PREFIX_PATH "${QT_PATH}" CACHE PATH "Qt6 prefix path")
            message(STATUS "Qt6 trouvé à: ${QT_PATH}")
            break()
        endif()
    endforeach()
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql)

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    client/src/front/mainwindow.cpp
    client/src/front/mainwindow.h
    client/src/front/mainwindow.ui
    client/src/front/messagewindow.h
    client/src/front/messagewindow.cpp
    client/src/front/loginwindow.h
    client/src/front/loginwindow.cpp
    src/common/Protocol.h
    src/common/Protocol.cpp
    src/common/Message.h
    src/common/Message.cpp
    client/src/back/ClientNetwork.h
    client/src/back/ClientNetwork.cpp
    server/Server.h
    server/Server.cpp
    # server/server.hpp supprimé (obsolète)
    server/database.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Mess
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Mess SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Mess
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Mess PRIVATE Qt6::Widgets Qt6::Network Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Mess)
endif()
set_target_properties(Mess PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Mess
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Mess)
endif()

# Test executable for Message serialization
add_executable(TestMessage tests/test_message.cpp
    src/common/Protocol.h
    src/common/Protocol.cpp
    src/common/Message.h
    src/common/Message.cpp
)
target_link_libraries(TestMessage PRIVATE Qt6::Core)
