cmake_minimum_required(VERSION 3.16)
project(async_msg_handling_cpp17)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_MIN_VER 1.71)
find_package(Boost 1.71.0 REQUIRED system)
find_package(Threads REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(MAIN_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(MESSAGES_INCLUDE_DIR "${MAIN_INCLUDE_DIR}/messages")

add_subdirectory(src/traffic_generator)
add_subdirectory(src/processor_cxx11)
add_subdirectory(src/processor_cxx17)
