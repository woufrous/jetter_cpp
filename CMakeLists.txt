cmake_minimum_required (VERSION 3.1)
project (jetter CXX)

# select ISO C++ 11 and fail if not supported
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# set compiler flags
add_compile_options (-Wall -Wextra -Wpedantic -pthread)

set (serial_SRCS
    third_party/serial/src/serial.cc
    third_party/serial/src/impl/unix.cc
)

add_library (serial ${serial_SRCS})

include_directories (
    "third_party/serial/include"
)

set (jetter_SRCS src/main.cc)

add_executable (jetter ${jetter_SRCS})
target_link_libraries (jetter PUBLIC
    serial
    "pthread"
)
