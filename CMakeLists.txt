cmake_minimum_required(VERSION 3.16)

project(WizzMania VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configuration de Qt 6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
)

if(APPLE)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com)
    set(CMAKE_MACOSX_BUNDLE ON)
endif()

qt_add_executable(WizzMania
    ${PROJECT_SOURCES}
)

target_link_libraries(WizzMania PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)

install(TARGETS WizzMania
    BUNDLE DESTINATION .
    LIBRARY DESTINATION {CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION {CMAKE_INSTALL_BINDIR}
)
