#include <mutex>
#include <string>

#include <serial/serial.h>

using byte_t = uint8_t;
using bytestring = std::basic_string<byte_t>;
using bytestringstream = std::basic_stringstream<byte_t>;
using obytestringstream = std::basic_ostringstream<byte_t>;

const byte_t JETTER_START = 0xD8;
const byte_t JETTER_STOP = 0xDA;
const byte_t JETTER_SCF = 0xDA;

bytestring escape(const bytestring& data) {
    obytestringstream ret;
    for (const byte_t& byte : data) {
        if ((byte >= 0xD8) && (byte <= 0xDF)) {
            ret << JETTER_SCF << (byte & !(1<<4));
        } else {
            ret << byte;
        }
    }
    return ret.str();
}

byte_t checksum(const bytestring& data) {
    uint8_t sum = 0;
    for (const byte_t& byte : data) {
        sum += byte;
    }
    return sum;
}

struct SerialConfig {
    std::string dev_name;
    uint32_t baudrate;
};

class JetterSerial {
    public:
        JetterSerial(const SerialConfig&);
        JetterSerial() = delete;
        JetterSerial(const JetterSerial&) = delete;
        JetterSerial& operator=(const JetterSerial&) = delete;

        std::string sync_command(std::string cmd);

    private:
        size_t send_command(const bytestring& data);
        bytestring _get_response();
        std::mutex _lock;
        serial::Serial _dev;
};

JetterSerial::JetterSerial(const SerialConfig& cfg) :
    _dev(cfg.dev_name, cfg.baudrate) {
}

size_t JetterSerial::send_command(const bytestring& data) {
    obytestringstream cmd;

    auto esc_data = escape(data);

    cmd << JETTER_START
        << esc_data
        << escape({checksum(esc_data)})
        << JETTER_STOP;

    std::vector<byte_t> ret;
    for (const byte_t& byte : ret) {
        ret.push_back(byte);
    }

    return _dev.write(ret);
}


int main() {
    return 0;
}
