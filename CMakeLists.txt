cmake_minimum_required(VERSION 3.10)

# 选择编译版本（可以通过 vscode 指定）
# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_BUILD_TYPE Debug)

# 交叉编译器（可以通过 vscode 指定）
# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-fPIC")
set(CMAKE_C_FLAGS_DEBUG "-O2 -g -D DEBUG")
set(CMAKE_C_FLAGS_RELEASE "-Os")

project(example LANGUAGES C CXX)

# 更详细的编译信息
# set(CMAKE_VERBOSE_MAKEFILE on)

# 存放可执行软件的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/bin)
# 默认存放静态库的文件夹位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/archive)
# 默认存放动态库的文件夹位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/library)

# 开启测试功能
enable_testing()

# 添加编译参数
add_compile_options(-Wall -fdata-sections -ffunction-sections)

# 递归调用子文件的 CMakeLists.txt
# add_subdirectory(lib)

# file(GLOB_RECURSE SRCS "src/*.c")

# ring_fifo 源文件
add_library(ring_fifo STATIC src/ring_fifo.c)
# ring_fifo 头文件路径
target_include_directories(ring_fifo PUBLIC inc)

# ring_fifo_32 源文件
add_library(ring_fifo_32 STATIC src/ring_fifo.c)
# ring_fifo_32 头文件路径
target_include_directories(ring_fifo_32 PUBLIC inc)
# 目标宏定义
target_compile_definitions(ring_fifo_32 PUBLIC NUM_BITS=32)

# ring_fifo_64 源文件
add_library(ring_fifo_64 STATIC src/ring_fifo.c)
# ring_fifo_64 头文件路径
target_include_directories(ring_fifo_64 PUBLIC inc)
# 目标宏定义
target_compile_definitions(ring_fifo_64 PUBLIC NUM_BITS=64)

# example
add_executable(example example.c)
# 目标需要链接的库
target_link_libraries(example ring_fifo)
add_dependencies(example ring_fifo)

# 测试
add_test(
    NAME example
    COMMAND ./bin/example
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/output
)

# example32
add_executable(example32 example.c)
# 目标需要链接的库
target_link_libraries(example32 ring_fifo_32)
add_dependencies(example32 ring_fifo_32)

# 测试
add_test(
    NAME example32
    COMMAND ./bin/example32
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/output
)

# example64
add_executable(example64 example.c)
# 目标需要链接的库
target_link_libraries(example64 ring_fifo_64)
add_dependencies(example64 ring_fifo_64)

# 测试
add_test(
    NAME example64
    COMMAND ./bin/example64
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/output
)
