#include <sstream>

#include <jetter/exceptions.h>

namespace jetter {

namespace internal {

std::string error_msg_from_status(byte_t status) {
    std::stringstream msg("");
    msg << "The following errorbits have been set (0x" << std::hex << int(status) << "):\n";
    if (status & (1<<internal::StatusBit::InvalidParam)) {
        msg << "Invalid parameter ";
    }
    if (status & (1<<internal::StatusBit::InvalidCommand)) {
        msg << "Invalid command ";
    }
    if (status & (1<<internal::StatusBit::Timeout)) {
        msg << "Timeout ";
    }
    if (status & (1<<internal::StatusBit::General)) {
        msg << "General error ";
    }
    if (status & (1<<internal::StatusBit::Reception)) {
        msg << "Reception error ";
    }
    return msg.str();
}

} // namespace internal
} // namespace jetter
