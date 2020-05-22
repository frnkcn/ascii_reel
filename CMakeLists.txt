cmake_minimum_required(VERSION 3.14)

project(ascii_reel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# OpenCV
set(OpenCV_DIR $ENV{OPENCV_CMAKE_PATH})
find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main "main.cpp")
target_link_libraries(main ${OpenCV_LIBS})
