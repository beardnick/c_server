cmake_minimum_required(VERSION 3.16)
project(web_server)

# 使用11标准
set(CMAKE_CXX_STANDARD 11)

# 将socket thread文件夹作为include库
#include_directories(socket)
#include_directories(thread)

# 将项目根目录下所有的都include
include_directories(${CMAKE_CURRENT_LIST_DIR})

# 将整个socket目录内容放入socket_src中
aux_source_directory(socket socket)

# cmake --build . --target client
add_executable(client
        ${socket}
        test/client.cc
        )
add_executable(select_server
        ${socket}
        test/select_echo.cc
        )

add_executable(thread_server
        ${socket}
        test/thread_echo.cc
        )

add_executable(fork_server
        ${socket}
        test/fork_echo.cc
        )
