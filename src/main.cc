#include <memory>

#include <jetter/Jetter_com.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    jetter::Jetter_com jetter(std::make_unique<serial::Serial>(argv[1], 9600));
    return 0;
}
