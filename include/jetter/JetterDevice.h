#ifndef __JETTER_DEVICE_H
#define __JETTER_DEVICE_H

#include <vector>

#include <jetter/JetterCom.h>

namespace jetter {

using reg_t = int32_t;
using addr_t = uint32_t;


namespace internal {
enum StatusBit {
    Data = 0,
    InvalidParam,
    InvalidCommand,
    Timeout,
    General,
    NoError,
    Reception,
};
} // namespace internal



class JetterException : std::exception {
    public:
        JetterException(byte_t status) : status_(status) {}
        virtual const char* what() const throw() {
            std::string msg = "The following errorbits have been set:\n";
            if (status_ & (1<<internal::StatusBit::InvalidParam)) {
                msg += "Invalid parameter ";
            }
            if (status_ & (1<<internal::StatusBit::InvalidCommand)) {
                msg += "Invalid command ";
            }
            if (status_ & (1<<internal::StatusBit::Timeout)) {
                msg += "Timeout ";
            }
            if (status_ & (1<<internal::StatusBit::General)) {
                msg += "General error ";
            }
            if (status_ & (1<<internal::StatusBit::Reception)) {
                msg += "Invalid command ";
            }
            return msg.c_str();
        }

    private:
        byte_t status_;
};


class IJetterDevice {
    public:
        virtual ~IJetterDevice() {}

        // program control
        virtual void continue_program() const = 0;
        virtual void stop_program() const = 0;
        virtual void restart_program() const = 0;
        // register control
        virtual std::vector<reg_t> get_register_block(uint32_t reg_base, uint32_t count) const = 0;
        virtual reg_t get_register(addr_t reg) const = 0;
        virtual void set_register(addr_t reg, reg_t value) const = 0;
        // flag control
        virtual bool get_flag(addr_t flag) const = 0; 
        virtual void set_flag(addr_t flag) const = 0; 
        virtual void reset_flag(addr_t flag) const = 0; 
};


class JetterDevice : public IJetterDevice {
    public:
        JetterDevice(std::unique_ptr<jetter::JetterCom>&& dev)
            : dev_(std::move(dev)) {}
        JetterDevice(const std::string& port_name)
            : JetterDevice(std::make_unique<jetter::JetterCom>(port_name)) {}

        // program control
        void continue_program() const override;
        void stop_program() const override;
        void restart_program() const override;
        // register control
        std::vector<reg_t> get_register_block(uint32_t reg_base, uint32_t count) const override;
        reg_t get_register(addr_t reg) const override;
        void set_register(addr_t reg, reg_t value) const override;
        // flag control
        bool get_flag(addr_t flag) const override; 
        void set_flag(addr_t flag) const override; 
        void reset_flag(addr_t flag) const override; 

    private:
        std::unique_ptr<jetter::JetterCom> dev_;

        bytestring sync_command(const bytestring& data) const;
};

}

#endif // __JETTER_DEVICE_H
