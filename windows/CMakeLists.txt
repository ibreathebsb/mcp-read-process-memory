cmake_minimum_required(VERSION 3.5)

project(PMM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories("cpp-httplib")
include_directories(nlohmann-json/include)

add_executable(PMM main.cpp pmm.hpp)

