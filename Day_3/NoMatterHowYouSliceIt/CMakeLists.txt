cmake_minimum_required (VERSION 3.8)

add_executable (NoMatterHowYouSliceIt "NoMatterHowYouSliceIt.cpp" "NoMatterHowYouSliceIt.h")
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} /std:c++17 ")
