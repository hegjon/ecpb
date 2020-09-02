cmake_minimum_required(VERSION 3.8.0 FATAL_ERROR)

project(ecpb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core  REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)

