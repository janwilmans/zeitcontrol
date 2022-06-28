cmake_minimum_required(VERSION 3.17)

project(zeitcontrol)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(zeit 
    src/main.cpp
    src/serialport.cpp
    src/zeitcontrol.cpp
)

target_link_libraries(zeit pthread)

target_include_directories(zeit PUBLIC
   fmt-8.1.1/include
) 
