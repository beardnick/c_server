cmake_minimum_required(VERSION 3.16)
project(web_server)

# 使用11标准
set(CMAKE_CXX_STANDARD 11)

# 将socket thread文件夹作为include库
#include_directories(socket)
#include_directories(thread)

# 将项目根目录下所有的都include
include_directories(${CMAKE_CURRENT_LIST_DIR})

# 将整个socket目录内容放入socket变量中
aux_source_directory(socket socket)

# 生成的可执行文件全部放在一个目录下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# 引用socket变量
# cmake --build . --target client
add_executable(client client.cpp ${socket})

add_executable(select_server select_echo.cpp ${socket})

add_executable(thread_server thread_echo.cpp ${socket})

add_executable(fork_server fork_echo.cpp ${socket})

add_executable(my_server my_server.cpp ${socket})
