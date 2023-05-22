cmake_minimum_required(VERSION 3.22) # 指定最低版本
project(Mahjong) # 指定项目名称

include(CTest)
include(./cmake/clang-tools.cmake)

set(CMAKE_CXX_STANDARD 17) # 使用 C++17 标准
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 生成 compile_commands.json，防止无法识别 include
set(CMAKE_BUILD_TYPE Debug) # 指定编译选项

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin) # 指定可执行文件输出目录
set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/build) # 指定编译目录

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/Common
  ${CMAKE_SOURCE_DIR}/include/Tile
  ${CMAKE_SOURCE_DIR}/include/Yakus
)# 指定头文件目录

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party)
add_subdirectory(${CMAKE_SOURCE_DIR}/test)