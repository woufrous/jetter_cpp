#include <mutex>
#include <string>
#include <memory>

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
    for (const auto& byte : data) {
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
    for (const auto& byte : data) {
        sum += byte;
    }
    return sum;
}

class Jetter_com {
    public:
        Jetter_com(std::unique_ptr<serial::Serial>&& dev) : dev_(std::move(dev)) {}
        Jetter_com() = delete;
        Jetter_com(const Jetter_com&) = delete;
        Jetter_com& operator=(const Jetter_com&) = delete;

        /* std::string sync_command(std::string cmd); */

    private:
        size_t send_command(const bytestring& data) const;
        bytestring get_response() const;
        std::mutex lock_;
        std::unique_ptr<serial::Serial> dev_;
};

size_t Jetter_com::send_command(const bytestring& data) const {
    bytestringstream cmd;

    auto esc_data = escape(data);

    cmd << JETTER_START
        << esc_data
        << escape({checksum(esc_data)})
        << JETTER_STOP;
    auto cmd_str = cmd.str();

    /* std::vector<byte_t> ret(cmd_str.begin(), cmd_str.end()); */

    return dev_->write(std::vector<byte_t>(cmd_str.begin(), cmd_str.end()));
}


int main(int argc, char** argv) {
    if (argc < 2) {
        return 1;
    }
    Jetter_com jetter(std::make_unique<serial::Serial>(argv[1], 9600));
    return 0;
}
