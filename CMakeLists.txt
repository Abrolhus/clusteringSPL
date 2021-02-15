cmake_minimum_required(VERSION 3.9)
project(clustering)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#include_directories(src)

find_package( OpenCV REQUIRED ) # locate OpenCV in system

add_subdirectory(src)
file(GLOB Sources "*.cpp")
add_executable(out ${Sources})
target_link_libraries(out src_lib ${OpenCV_LIBS})
