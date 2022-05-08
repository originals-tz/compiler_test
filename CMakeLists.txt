cmake_minimum_required(VERSION 3.17)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

link_directories("/usr/local/lib")


add_executable(compiler main.cpp type.pb.cc)
target_link_libraries(compiler protobuf)