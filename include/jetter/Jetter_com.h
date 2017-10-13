#ifndef _JETTER_COM_H
#define _JETTER_COM_H

#include <mutex>
#include <string>
#include <memory>

#include <serial/serial.h>

namespace jetter {

using byte_t = uint8_t;
using bytestring = std::basic_string<byte_t>;
using bytestringstream = std::basic_stringstream<byte_t>;
using obytestringstream = std::basic_ostringstream<byte_t>;

class Jetter_com {
    public:
        Jetter_com(std::unique_ptr<serial::Serial>&& dev) : dev_(std::move(dev)) {}
        Jetter_com() = delete;
        Jetter_com(const Jetter_com&) = delete;
        Jetter_com& operator=(const Jetter_com&) = delete;

        bytestring sync_command(const bytestring& cmd);

    private:
        std::mutex lock_;
        std::unique_ptr<serial::Serial> dev_;

        size_t send_command(const bytestring& data) const;
        bytestring get_response() const;
};

}

#endif // _JETTER_COM_H
