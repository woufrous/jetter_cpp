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
} // namespace internal



class JetterException : std::exception {
    public:
        JetterException(std::string what="") :
            what_(what) {
        }
        virtual const char* what() const throw() {
            return what_.c_str();
        }

    private:
        std::string what_;
};

class JetterComError : JetterException {
    public:
        JetterComError(byte_t status) : status_(status) {}
        virtual const char* what() const throw() {
            std::string msg = "The following errorbits have been set:\n";
            if (status_ & (1<<internal::StatusBit::InvalidParam)) {
                msg += "Invalid parameter ";
            }
            if (status_ & (1<<internal::StatusBit::InvalidCommand)) {
                msg += "Invalid command ";
            }
            if (status_ & (1<<internal::StatusBit::Timeout)) {
                msg += "Timeout ";
            }
            if (status_ & (1<<internal::StatusBit::General)) {
                msg += "General error ";
            }
            if (status_ & (1<<internal::StatusBit::Reception)) {
                msg += "Invalid command ";
            }
            return msg.c_str();
        }

        byte_t get_status() const {
            return status_;
        }

    private:
        byte_t status_;
};

} // namespace jetter

#endif // __JETTER_EXCEPTION_H
