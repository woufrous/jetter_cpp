#include <jetter/Jetter_com.h>
#include <jetter/internal/serial_escape.h>

using namespace jetter;

bytestring internal::escape(const bytestring& data) {
    bytestring ret;
    for (const auto& byte : data) {
        if ((byte >= 0xD8) && (byte <= 0xDF)) {
            ret += internal::JETTER_SCF;
            ret += byte & ~(1<<4);
        } else {
            ret += byte;
        }
    }
    return ret;
}

bytestring internal::unescape(const bytestring& data) {
    bytestring ret;
    bool unescape_next = false;

    for (const auto& byte : data) {
        if (byte == internal::JETTER_SCF) {
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

byte_t internal::checksum(const bytestring& data) {
    uint8_t sum = 0;
    for (const auto& byte : data) {
        sum += byte;
    }
    return sum;
}

size_t Jetter_com::send_command(const bytestring& data) const {
    bytestringstream cmd;

    auto esc_data = internal::escape(data);

    cmd << internal::JETTER_START
        << esc_data
        << internal::escape({internal::checksum(esc_data)})
        << internal::JETTER_STOP;

    return dev_->write(cmd.str());
}


bytestring Jetter_com::get_response() const {
    bytestring out;

    while (true) {
        // this will time out, if no more data can be read
        byte_t buf = dev_->read_byte();
        out += buf;

        if (buf == internal::JETTER_START) {
            out = buf;
        } else
        if (buf == internal::JETTER_STOP) {
            break;
        }
    }
    return out;
}


bytestring Jetter_com::sync_command(const bytestring& cmd) {
    std::lock_guard<std::mutex> guard(this->lock_);
    this->send_command(cmd);
    auto out = get_response();

    return internal::unescape(out);
}
