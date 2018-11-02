#ifndef __JETTER_EXCEPTION_H
#define __JETTER_EXCEPTION_H

#include <string>

#include <jetter/types.h>

namespace jetter {

namespace internal {
enum StatusBit {
    Data = 0,
    InvalidParam,
    InvalidCommand,
    Timeout,
    General,
    NoError,
    Reception,
};

std::string error_msg_from_status(byte_t status);
} // namespace internal



class JetterException : public std::exception {
    public:
        JetterException(std::string what) :
            what_(what) {
        }
        virtual const char* what() const throw() {
            return what_.c_str();
        }

    private:
        std::string what_;
};

} // namespace jetter

#endif // __JETTER_EXCEPTION_H
