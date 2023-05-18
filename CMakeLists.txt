cmake_minimum_required(VERSION 3.22) # 指定最低版本
project(Mahjong) # 指定项目名称
enable_testing()

set(CMAKE_CXX_STANDARD 17) # 使用 C++17 标准
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成 compile_commands.json，防止无法识别 include
set(CMAKE_CXX_COMPILER "clang++") # 指定编译器
set(CMAKE_BUILD_TYPE Debug) # 指定编译选项

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin) # 指定可执行文件输出目录
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build) # 指定编译目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) # 指定头文件目录
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src) # 添加子目录

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang-tools.cmake)