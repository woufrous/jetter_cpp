#ifndef __JETTER_SERIAL_H
#define __JETTER_SERIAL_H

#include <memory>

#include <serial/serial.h>

#include <jetter/types.h>

namespace jetter {

class ISerialDevice {
    public:
        virtual ~ISerialDevice() {};

        virtual size_t write(const bytestring& data) const = 0;
        virtual byte_t read_byte() const = 0;
};

class SerialDevice : public ISerialDevice {
    public:
        SerialDevice(const std::string& dev_name) {
            dev_ = std::make_unique<serial::Serial>(dev_name);
        }

        SerialDevice() = delete;
        SerialDevice(SerialDevice&) = delete;
        SerialDevice(const SerialDevice&) = delete;
        SerialDevice& operator=(SerialDevice&) = delete;

        size_t write(const bytestring& data) const override final {
            return dev_->write(std::vector<byte_t>(data.begin(), data.end()));
        }
        byte_t read_byte() const override final {
            uint8_t buf;
            dev_->read(&buf, 1);
            return buf;
        }

    private:
        std::unique_ptr<serial::Serial> dev_;
};

}
#endif // __JETTER_SERIAL_H
