cmake_minimum_required (VERSION 2.6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 /std:c++17")

project (ReposeRecord)


add_executable(ReposeRecord ReposeRecord.cpp)

#find_package(Boost 1.36.0)
#if(Boost_FOUND)
#  include_directories(${Boost_INCLUDE_DIRS})
#  add_executable(ReposeRecord ReposeRecord.cpp)
#endif()