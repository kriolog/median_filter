cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(median_filter)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(src)