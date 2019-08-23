cmake_minimum_required(VERSION 3.14)
project(PDS_Server)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network)
list(REMOVE_DUPLICATES LIBRARIES)

add_executable(PDS_Server
        main.cpp
        NetworkServer.cpp
        NetworkServer.h
        Message.cpp
        Message.h
        Symbol.cpp
        Symbol.h
        Request.cpp
        Request.h
        SharedEditor.cpp
        SharedEditor.h)

target_link_libraries(PDS_Server ${LIBRARIES})