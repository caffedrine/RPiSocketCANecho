cmake_minimum_required(VERSION 3.12)
project(SocketCANecho)

set(CMAKE_CXX_STANDARD 11)

add_executable(SocketCANecho main.cpp SocketCAN.cpp SocketCAN.h)