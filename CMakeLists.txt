cmake_minimum_required (VERSION 3.1)
project (jetter CXX)

# use proper C++ standards...there is "proper" way (target_compile_features) to do this right now
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup (TARGETS)

add_library (jetter
    src/JetterCom.cpp
    src/JetterDevice.cpp
    src/SerialDevice.cpp
    src/encoding.cpp
    src/exceptions.cpp
)
add_library (jetter::jetter ALIAS jetter)
target_include_directories (jetter
    PUBLIC
        include
)
target_link_libraries (jetter
    PUBLIC
        CONAN_PKG::serial
        CONAN_PKG::fmt
)
target_compile_options (jetter
    PUBLIC
        -Wall -Wextra -Wpedantic
)
target_compile_features (jetter
    PUBLIC
        cxx_std_14
)

add_executable (jetter_test src/main.cc)
target_link_libraries (jetter_test
    PUBLIC
        jetter
        CONAN_PKG::fmt
)

add_executable (unit_tests tests/serial.cpp tests/encoding.cpp)
target_link_libraries (unit_tests
    PUBLIC
        jetter
        CONAN_PKG::gtest
)
