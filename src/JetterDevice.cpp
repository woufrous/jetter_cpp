#include <jetter/JetterDevice.h>
#include <jetter/exceptions.h>
#include <jetter/internal/encoding.h>

namespace jetter {

bytestring JetterDevice::sync_command(const bytestring& data) const {
    auto resp = dev_->sync_command(data);
    if (!(resp[0] & (1<<internal::StatusBit::NoError))) {
        throw JetterException(internal::error_msg_from_status(resp[0]));
    }
    return resp.substr(1);
}

void JetterDevice::set_program_state(uint8_t task, internal::ProgramState state) const {
    if (task > 0xAA) {
        throw JetterException(internal::error_msg_from_status((1<<internal::StatusBit::InvalidParam)));
    }
    bytestring req = {'N'};
    req += internal::encode<internal::ValueType::UINT8>(static_cast<uint8_t>(state));
    req += internal::encode<internal::ValueType::UINT8>(task);

    sync_command(req);
}

void JetterDevice::continue_program(uint8_t task) const {
    set_program_state(task, internal::ProgramState::Continue);
}

void JetterDevice::stop_program(uint8_t task) const {
    set_program_state(task, internal::ProgramState::Stop);
}

void JetterDevice::restart_program(uint8_t task) const {
    set_program_state(task, internal::ProgramState::Restart);
}

std::vector<reg_t>
JetterDevice::get_register_block_impl_(addr_t reg_base, uint8_t count, uint8_t mod) const {
    std::vector<reg_t> ret(32);
    bytestring req = {'h'};
    req += internal::encode<internal::ValueType::UINT8>(mod);
    req += internal::encode<internal::ValueType::UINT8>(count);
    req += internal::encode<internal::ValueType::UINT24>(reg_base);

    auto resp = sync_command(req);
    reg_t tmp;
    for (size_t i=0; i<resp.length(); i+=3) {
        tmp = internal::decode<internal::ValueType::INT24>(resp.substr(i));
        ret.push_back(tmp);
    }
    return ret;
}

std::vector<reg_t> JetterDevice::get_register_block(addr_t reg_base, size_t count) const {
    std::vector<reg_t> ret(count);
    while (count > 0) {
        auto tmp = get_register_block_impl_(reg_base, count < 32 ? count : 32, 0x01);
        ret.insert(ret.end(), tmp.begin(), tmp.end());
        count -= tmp.size();
    }
    return ret;
}

reg_t JetterDevice::get_register(addr_t reg) const {
    bytestring req = {'Q'};
    req += internal::encode<internal::ValueType::UINT24>(reg);

    auto resp = sync_command(req);
    return internal::decode<internal::ValueType::INT24>(resp);
}

void JetterDevice::set_register(addr_t reg, reg_t value) const {
    bytestring req = {'R'};
    req += internal::encode<internal::ValueType::UINT24>(reg);
    req += internal::encode<internal::ValueType::INT24>(value);

    sync_command(req);
}

bool JetterDevice::get_flag(uint16_t flag) const {
    bytestring req = {'K'};
    req += internal::encode<internal::ValueType::UINT16>(flag);

    auto resp = sync_command(req);
    return resp[0] & (1<<internal::StatusBit::Data);
}

void JetterDevice::set_flag(uint16_t flag) const {
    bytestring req = {'L'};
    req += internal::encode<internal::ValueType::UINT16>(flag);

    sync_command(req);
}

void JetterDevice::reset_flag(uint16_t flag) const {
    bytestring req = {'M'};
    req += internal::encode<internal::ValueType::UINT16>(flag);

    sync_command(req);
}

}
