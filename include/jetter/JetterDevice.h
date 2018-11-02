#ifndef __JETTER_DEVICE_H
#define __JETTER_DEVICE_H

#include <vector>

#include <jetter/JetterCom.h>
#include <jetter/internal/encoding.h>

namespace jetter {

using reg_t = internal::ValueTypeToCType<internal::ValueType::INT24>::type;
using addr_t = internal::ValueTypeToCType<internal::ValueType::UINT24>::type;


namespace internal {
enum ProgramState {
    Restart = 1,
    Stop,
    Continue,
};
} // namespace internal


class IJetterDevice {
    public:
        virtual ~IJetterDevice() {}

        // program control
        virtual void continue_program(uint8_t task) const = 0;
        virtual void stop_program(uint8_t task) const = 0;
        virtual void restart_program(uint8_t task) const = 0;
        // register control
        virtual std::vector<reg_t> get_register_block(addr_t reg_base, size_t count) const = 0;
        virtual reg_t get_register(addr_t reg) const = 0;
        virtual void set_register(addr_t reg, reg_t value) const = 0;
        // flag control
        virtual bool get_flag(uint16_t flag) const = 0;
        virtual void set_flag(uint16_t flag) const = 0;
        virtual void reset_flag(uint16_t flag) const = 0;
};


class JetterDevice : public IJetterDevice {
    public:
        JetterDevice(std::unique_ptr<jetter::JetterCom>&& dev)
            : dev_(std::move(dev)) {}
        JetterDevice(const std::string& port_name)
            : JetterDevice(std::make_unique<jetter::JetterCom>(port_name)) {}

        // program control
        void continue_program(uint8_t task) const override;
        void stop_program(uint8_t task) const override;
        void restart_program(uint8_t task) const override;
        // register control
        std::vector<reg_t> get_register_block(addr_t reg_base, size_t count) const override;
        reg_t get_register(addr_t reg) const override;
        void set_register(addr_t reg, reg_t value) const override;
        // flag control
        bool get_flag(uint16_t flag) const override;
        void set_flag(uint16_t flag) const override;
        void reset_flag(uint16_t flag) const override;

    private:
        std::unique_ptr<jetter::JetterCom> dev_;

        bytestring sync_command(const bytestring& data) const;
        void set_program_state(uint8_t program, internal::ProgramState state) const;
        std::vector<reg_t> get_register_block_impl_(addr_t reg_base, uint8_t count, uint8_t mod) const;
};

}

#endif // __JETTER_DEVICE_H
