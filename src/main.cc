#include <memory>
#include <iostream>

#include <fmt/printf.h>

#include <jetter/JetterDevice.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fmt::print("Usage: {} COM-PORT\n", argv[0]);
        return 1;
    }

    fmt::print("Connecting to {}\n", argv[1]);
    jetter::JetterDevice jetter(argv[1]);
    return 0;
}
