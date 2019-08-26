cmake_minimum_required(VERSION 3.1.0)
project(PDS_Server)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

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

set(SOURCE_FILES
        src/main.cpp
        src/Symbol.cpp
        src/Message.cpp
        src/Request.cpp
        src/NetworkServer.cpp
        src/SharedEditor.cpp)

set(INCLUDE_FILES
        include/Symbol.h
        include/Message.h
        include/Request.h
        include/NetworkServer.h
        include/SharedEditor.h)

add_executable(PDS_Server
        ${INCLUDE_FILES}
        ${SOURCE_FILES})

target_link_libraries(PDS_Server ${LIBRARIES})