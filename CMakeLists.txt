cmake_minimum_required(VERSION 3.0)
project(ScreenRotator)

find_package(Qt5 REQUIRED COMPONENTS Gui Widgets X11Extras Sensors DBus)

add_definitions(-DQT_NO_DEBUG_OUTPUT)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_subdirectory(src)
