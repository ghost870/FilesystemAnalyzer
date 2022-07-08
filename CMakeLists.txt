cmake_minimum_required(VERSION 3.5)

project(FilesystemAnalyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(FilesystemAnalyzer src/Filesystem.cpp src/Fat32.cpp src/main.cpp)