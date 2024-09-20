cmake_minimum_required(VERSION 3.16)
 
project(TriggerPrimiticAnalysisTools)

set(CMAKE_CXX_FLAGS "-std=c++17")

message("CMAKE CXX standard = ${CMAKE_CXX_FLAGS}")

find_package(ROOT REQUIRED)

message("ROOT include dirs = ${ROOT_INCLUDE_DIRS}")
message("ROOT library dir = ${ROOT_LIBRARY_DIR}")

include_directories(${PROJECT_SOURCE_DIR}/inc ${ROOT_INCLUDE_DIRS})

#link_directories($ENV{ROOTSYS}/lib)
link_directories(${ROOT_LIBRARY_DIR})

add_subdirectory(srcs)
add_subdirectory(app)
