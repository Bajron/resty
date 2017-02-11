cmake_minimum_required(VERSION 3.5)

set(n "resty")

project(${n})

find_package(Qt5Core)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_library(${n}
  mux/Router.cpp 
)

set(public_headers
  mux/Router.h
)

target_include_directories(${n} PUBLIC ..)

target_link_libraries(${n} qhttp)

qt5_use_modules(${n} Core Network)

set_target_properties(${n} PROPERTIES PUBLIC_HEADER ${public_headers})



