#include <jetter/Serial.h>

namespace jetter {

SerialDevice::SerialDevice(const std::string& dev_name) {
    dev_ = std::make_unique<serial::Serial>(dev_name);
}

size_t SerialDevice::write(const bytestring& data) const {
    return dev_->write(std::vector<byte_t>(data.begin(), data.end()));
}

byte_t SerialDevice::read_byte() const {
    uint8_t buf;
    dev_->read(&buf, 1);
    return buf;
}

}
