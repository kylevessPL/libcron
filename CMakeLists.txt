cmake_minimum_required(VERSION 3.13)
project(projekt2)

set(CMAKE_CXX_STANDARD 17)

add_executable(projekt2 main.cpp client.cpp client.hpp server.cpp server.hpp query.hpp interpreter.cpp interpreter.hpp invalid_args_exception.hpp scheduler.cpp scheduler.hpp task.cpp task.hpp time.cpp time.hpp utils.cpp utils.hpp logger.cpp logger.hpp)