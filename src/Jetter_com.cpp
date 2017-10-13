#include <jetter/Jetter_com.h>

using namespace jetter;

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

bytestring unescape(const bytestring& data) {
    bytestring ret;
    bool unescape_next = false;

    for (const auto& byte : data) {
        if (byte == JETTER_SCF) {
            unescape_next = true;
            continue;
        }
        if (unescape_next) {
            ret += byte | (1<<4);
        } else {
            ret += byte;
        }
        unescape_next = false;
    }
    return ret;
}

byte_t checksum(const bytestring& data) {
    uint8_t sum = 0;
    for (const auto& byte : data) {
        sum += byte;
    }
    return sum;
}

size_t Jetter_com::send_command(const bytestring& data) const {
    bytestringstream cmd;

    auto esc_data = escape(data);

    cmd << JETTER_START
        << esc_data
        << escape({checksum(esc_data)})
        << JETTER_STOP;
    auto cmd_str = cmd.str();

    return dev_->write(std::vector<byte_t>(cmd_str.begin(), cmd_str.end()));
}


bytestring Jetter_com::get_response() const {
    uint8_t buf[1];
    bytestring out;

    while (true) {
        // this will time out, if no more data can be read
        dev_->read(buf, 1);
        out += buf[0];

        if (buf[0] == JETTER_START) {
            out = buf;
        } else
        if (buf[0] == JETTER_STOP) {
            break;
        }
    }
    return out;
}


bytestring Jetter_com::sync_command(const bytestring& cmd) {
    std::lock_guard<std::mutex> guard(this->lock_);
    this->send_command(cmd);
    auto out = get_response();

    return unescape(out);
}
