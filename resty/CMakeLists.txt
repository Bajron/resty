cmake_minimum_required(VERSION 3.5)

set(n "resty")

project(${n})

find_package(Qt5Core)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_library(${n}
  mux/Router.cpp
  mux/Route.cpp
  mux/RouteMatch.cpp
)

set(public_headers
  mux/Router.h
  mux/Route.h
  mux/RouteMatch.h
)

target_include_directories(${n} PUBLIC ..)

target_link_libraries(${n} qhttp)

qt5_use_modules(${n} Core Network)

set_target_properties(${n} PROPERTIES PUBLIC_HEADER ${public_headers})


add_executable(test_${n}
  RegExBuilder_test.cpp
)

target_link_libraries(test_${n} ${n} gmock gtest_main)

add_test(${n} test_${n})

