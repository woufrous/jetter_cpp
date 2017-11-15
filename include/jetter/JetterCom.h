#ifndef _JETTER_COM_H
#define _JETTER_COM_H

#include <mutex>
#include <string>
#include <memory>

#include <jetter/types.h>
#include <jetter/Serial.h>

namespace jetter {

class JetterCom {
    public:
        JetterCom(std::unique_ptr<jetter::ISerialDevice>&& dev) : dev_(std::move(dev)) {}
        JetterCom(const std::string& port_name) : dev_(std::make_unique<jetter::SerialDevice>(port_name)) {}

        bytestring sync_command(const bytestring& cmd);

    private:
        std::mutex lock_;
        std::unique_ptr<jetter::ISerialDevice> dev_;

        size_t send_command(const bytestring& data) const;
        bytestring get_response() const;
};

}

#endif // _JETTER_COM_H
