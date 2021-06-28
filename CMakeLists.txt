cmake_minimum_required(VERSION 3.13)
project(project)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

include_directories(include)

add_library(client src/client.cpp include/client.hpp)
add_library(server src/server.cpp include/server.hpp)
add_library(interpreter src/interpreter.cpp include/interpreter.hpp)
add_library(scheduler src/scheduler.cpp include/scheduler.hpp)
add_library(task src/task.cpp include/task.hpp)
add_library(time src/time.cpp include/time.hpp)
add_library(utils src/utils.cpp include/utils.hpp)
add_library(logger src/logger.cpp include/logger.hpp)

add_executable(project main.cpp)

target_link_libraries(project ${CMAKE_THREAD_LIBS_INIT} rt client server interpreter scheduler task time utils logger)