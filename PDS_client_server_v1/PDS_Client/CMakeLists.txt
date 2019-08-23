cmake_minimum_required(VERSION 3.14)
project(PDS_Client)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_BUILD_TYPE DEBUG)

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

add_executable(PDS_Client
        main.cpp
        SharedEditor.cpp
        SharedEditor.h
        Symbol.cpp
        Symbol.h
        Message.cpp
        Message.h
        Request.cpp
        Request.h)

target_link_libraries(PDS_Client ${LIBRARIES})
