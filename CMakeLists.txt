cmake_minimum_required (VERSION 3.1)
project (basecsv)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_executable (basecsv main.cpp lead.hpp lead.cxx)