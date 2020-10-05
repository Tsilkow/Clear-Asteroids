cmake_minimum_required(VERSION 3.10)

project(ClearAsteroids VERSION 0.01)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("inc")
file(GLOB SOURCES "src/*.cpp")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
add_library(Modules SHARED ${SOURCES})

add_executable(ClearAsteroids main.cpp)
target_link_libraries(ClearAsteroids sfml-graphics sfml-audio Modules)
