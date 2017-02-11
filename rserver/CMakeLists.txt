cmake_minimum_required(VERSION 3.5)

set(n "rserver")

project(${n})

find_package(Qt5Core)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_executable(${n}
  main.cpp 
)

target_link_libraries(${n} resty)

qt5_use_modules(${n} Core Network)


