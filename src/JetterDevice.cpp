#include <memory>

#include <jetter/JetterDevice.h>
#include <jetter/internal/encoding.h>

namespace jetter {

std::pair<bool, bytestring> JetterDevice::sync_command(const bytestring& data) const {
    auto resp = dev_->sync_command(data);
    return std::pair<bool, bytestring>({(resp[0] & 0x01), resp.substr(1)});
}

void JetterDevice::continue_program() const {}
void JetterDevice::stop_program() const {}
void JetterDevice::restart_program() const {}
std::vector<reg_t> JetterDevice::get_register_block(uint32_t reg_base, uint32_t count) const {
    return std::vector<reg_t>({0});
}
reg_t JetterDevice::get_register(addr_t reg) const {
    bytestring req = {'Q'};
    /*
    req += internal::encode<internal::ValueType::UINT24>(reg);
    auto ret = sync_command(req);
    if (ret.first) {
        return internal::decode<internal::ValueType::INT24>(ret.second);
    }
    */
    return 0;
}
void JetterDevice::set_register(addr_t reg, reg_t value) const {}
bool JetterDevice::get_flag(addr_t flag) const {
    return false;
} 
void JetterDevice::set_flag(addr_t flag) const {} 
void JetterDevice::reset_flag(addr_t flag) const {} 

}
