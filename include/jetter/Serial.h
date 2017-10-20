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
        SerialDevice(const std::string& dev_name);

        SerialDevice() = delete;
        SerialDevice(SerialDevice&) = delete;
        SerialDevice(const SerialDevice&) = delete;
        SerialDevice& operator=(SerialDevice&) = delete;
        SerialDevice& operator=(const SerialDevice&) = delete;

        size_t write(const bytestring& data) const override final;
        byte_t read_byte() const override final;

    private:
        std::unique_ptr<serial::Serial> dev_;
};

}
#endif // __JETTER_SERIAL_H
