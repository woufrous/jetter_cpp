cmake_minimum_required (VERSION 3.1)
project (jetter CXX)

# select ISO C++ 11 and fail if not supported
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# serial library stuff
# all this is copied from the serial CMakeLists.txt
set (serial_PROJECT_NAME serial)
set (serial_BASE_DIR "third_party/serial")

## Sources
set(serial_SRCS
    ${serial_BASE_DIR}/src/serial.cc
    ${serial_BASE_DIR}/include/serial/serial.h
    ${serial_BASE_DIR}/include/serial/v8stdint.h
)
if(APPLE)
    # If OSX
    list(APPEND serial_SRCS ${serial_BASE_DIR}/src/impl/unix.cc)
    list(APPEND serial_SRCS ${serial_BASE_DIR}/src/impl/list_ports/list_ports_osx.cc)
elseif(UNIX)
    # If unix
    list(APPEND serial_SRCS ${serial_BASE_DIR}/src/impl/unix.cc)
    list(APPEND serial_SRCS ${serial_BASE_DIR}/src/impl/list_ports/list_ports_linux.cc)
else()
    # If windows
    list(APPEND serial_SRCS ${serial_BASE_DIR}/src/impl/win.cc)
    list(APPEND serial_SRCS ${serial_BASE_DIR}/src/impl/list_ports/list_ports_win.cc)
endif()

## Add serial library
add_library(${serial_PROJECT_NAME} ${serial_SRCS})
if(APPLE)
    target_link_libraries(${serial_PROJECT_NAME} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY})
elseif(UNIX)
    target_link_libraries(${serial_PROJECT_NAME} rt pthread)
else()
    target_link_libraries(${serial_PROJECT_NAME} setupapi)
endif()

## Include headers
include_directories(${serial_BASE_DIR}/include)
# end of serial library stuff

add_subdirectory (third_party/fmt)

set (BUILD_GOOGLETEST TRUE)
add_subdirectory (third_party/googletest)
unset (BUILD_GOOGLETEST)

# set compiler flags
add_compile_options (-Wall -Wextra -Wpedantic)

set (jetter_SRCS
    src/Jetter_com.cpp
    src/SerialDevice.cpp
)
set (jetter_INCLUDE_DIR include)

include_directories (
    ${jetter_INCLUDE_DIR}
)

add_library (jetter ${jetter_SRCS})
target_link_libraries (jetter
    PUBLIC
        ${serial_PROJECT_NAME}
        fmt::fmt-header-only
)

add_executable (jetter_test src/main.cc)
target_link_libraries (jetter_test PUBLIC
    jetter
)

add_executable (unit_tests tests/serial.cpp tests/encoding.cpp)
target_include_directories (unit_tests PUBLIC
    ${gtest_SOURCE_DIR}/include
    ${gtest_SOURCE_DIR}
)
target_link_libraries (unit_tests PUBLIC
    jetter
    gtest
    gtest_main
)
