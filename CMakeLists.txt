cmake_minimum_required(VERSION 3.10)

project(ClearAsteroids VERSION 0.01)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRCS "src/*.cpp")
include_directories("inc")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_executable(ClearAsteroids src/main.cpp)
target_link_libraries(ClearAsteroids sfml-graphics sfml-audio)


