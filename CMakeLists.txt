cmake_minimum_required (VERSION 3.1)
project (jetter CXX)

# use proper C++ standards...there is "proper" way (target_compile_features) to do this right now
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

include (${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup ()

# serial library stuff
set (serial_BASE_DIR "third_party/serial")
add_library (serial
    ${serial_BASE_DIR}/src/serial.cc
)
add_library (serial::serial ALIAS serial)
target_compile_features (serial
    PUBLIC
        cxx_std_11
)
target_include_directories (serial
    PUBLIC
        ${serial_BASE_DIR}/include
)
if (APPLE)
    target_sources (serial
        PRIVATE
            ${serial_BASE_DIR}/src/impl/unix.cc
            ${serial_BASE_DIR}/src/impl/list_ports/list_ports_osx.cc
    )
    target_link_libraries (serial
        PUBLIC
            ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY}
    )
elseif (UNIX)
    target_sources (serial
        PRIVATE
            ${serial_BASE_DIR}/src/impl/unix.cc
            ${serial_BASE_DIR}/src/impl/list_ports/list_ports_linux.cc
    )
    target_link_libraries (serial
        PUBLIC
            rt pthread
    )
elseif (WIN32)
    target_sources (serial
        PRIVATE
            ${serial_BASE_DIR}/src/impl/win.cc
            ${serial_BASE_DIR}/src/impl/list_ports/list_ports_win.cc
    )
    target_link_libraries (serial
        PUBLIC
            setupapi
    )
else ()
    message (FATAL_ERROR "serial lib only works only on Windows, MacOS and Linux/Unix")
endif ()
# end of serial library stuff

add_library (jetter
    src/JetterCom.cpp
    src/JetterDevice.cpp
    src/SerialDevice.cpp
    src/encoding.cpp
)
add_library (jetter::jetter ALIAS jetter)
target_include_directories (jetter
    PUBLIC
        include
)
target_link_libraries (jetter
    PUBLIC
        serial::serial
        ${CONAN_LIBS}
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
)

add_executable (unit_tests tests/serial.cpp tests/encoding.cpp)
target_link_libraries (unit_tests
    PUBLIC
        jetter
        ${CONAN_LIBS}
)
