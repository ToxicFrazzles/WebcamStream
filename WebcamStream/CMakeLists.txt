cmake_minimum_required(VERSION 3.16)
project(WebcamStream)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Common)
add_subdirectory(Client)
add_subdirectory(Server)
add_subdirectory(Tests)