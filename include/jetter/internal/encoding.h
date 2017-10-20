#ifndef __JETTER_INTERNAL_ENCODING_H
#define __JETTER_INTERNAL_ENCODING_H

#include <cstdint>
#include <stdexcept>

#include <fmt/format.h>

namespace jetter {
namespace internal {

enum class ValueType {
    INT8,
    UINT8,
    INT16,
    UINT16,
    INT24,
    UINT24
};

template<ValueType T>
struct ValueTypeToCType;

#define VAL2CTYPE(val_type, c_type) \
    template<> \
    struct ValueTypeToCType<val_type> { \
        using type = c_type; \
    }

#define _CTYPE(val_type) \
    ValueTypeToCType<val_type>::type


VAL2CTYPE(ValueType::INT8, int8_t);
VAL2CTYPE(ValueType::UINT8, uint8_t);
VAL2CTYPE(ValueType::INT16, int16_t);
VAL2CTYPE(ValueType::UINT16, uint16_t);
VAL2CTYPE(ValueType::INT24, int32_t);
VAL2CTYPE(ValueType::UINT24, uint32_t);

// template functions
template<ValueType ValT, typename ArgT = typename ValueTypeToCType<ValT>::type>
bytestring encode(ArgT);

template<ValueType ValT, typename RetT = typename ValueTypeToCType<ValT>::type>
RetT decode(const bytestring&);

// specializations

// INT8
template<>
bytestring encode<ValueType::INT8>(_CTYPE(ValueType::INT8) value) {
    return bytestring({(byte_t)value});
}

template<>
_CTYPE(ValueType::INT8) decode<ValueType::INT8>(const bytestring& data) {
    if (data.size() != 1) {
        throw std::length_error(fmt::format("One byte reqired; {:d} given", data.size()));
    }
    return static_cast<_CTYPE(ValueType::INT8)>(data[0]);
}

// UINT8
template<>
bytestring encode<ValueType::UINT8>(_CTYPE(ValueType::UINT8) value) {
    return bytestring({(byte_t)value});
}

template<>
_CTYPE(ValueType::UINT8) decode<ValueType::UINT8>(const bytestring& data) {
    if (data.size() != 1) {
        throw std::length_error(fmt::format("One byte reqired; {:d} given", data.size()));
    }
    return static_cast<_CTYPE(ValueType::UINT8)>(data[0]);
}

// INT16
template<>
bytestring encode<ValueType::INT16>(_CTYPE(ValueType::INT16) value) {
    return bytestring({
        (byte_t)((value >> 8) & 0xFF),
        (byte_t)(value & 0xFF),
    });
}

template<>
_CTYPE(ValueType::INT16) decode<ValueType::INT16>(const bytestring& data) {
    if (data.size() != 2) {
        throw std::length_error(fmt::format("Two bytes reqired; {:d} given", data.size()));
    }
    return static_cast<_CTYPE(ValueType::INT16)>(
        data[0] << 8 |
        data[1]
    );
}

// UINT16
template<>
bytestring encode<ValueType::UINT16>(_CTYPE(ValueType::UINT16) value) {
    return bytestring({
        (byte_t)((value >> 8) & 0xFF),
        (byte_t)(value & 0xFF),
    });
}

template<>
_CTYPE(ValueType::UINT16) decode<ValueType::UINT16>(const bytestring& data) {
    if (data.size() != 2) {
        throw std::length_error(fmt::format("Two bytes reqired; {:d} given", data.size()));
    }
    return static_cast<_CTYPE(ValueType::UINT16)>(
        data[0] << 8 |
        data[1]
    );
}

// INT24
template<>
bytestring encode<ValueType::INT24>(_CTYPE(ValueType::INT24) value) {
    return bytestring({
        (byte_t)((value >> 16) & 0xFF),
        (byte_t)((value >> 8) & 0xFF),
        (byte_t)(value & 0xFF),
    });
}

template<>
_CTYPE(ValueType::INT24) decode<ValueType::INT24>(const bytestring& data) {
    if (data.size() != 3) {
        throw std::length_error(fmt::format("Three bytes reqired; {:d} given", data.size()));
    }
    // This is a little tricky, since int32_t requires bit 31 to be the sign bit.
    // To work around that, we have to shift 8bits too far,  convert and then shift back.
    return static_cast<_CTYPE(ValueType::INT24)>(
        data[0] << 24 |
        data[1] << 16 |
        data[2] << 8
    ) >> 8;
}

// UINT24
template<>
bytestring encode<ValueType::UINT24>(_CTYPE(ValueType::UINT24) value) {
    return bytestring({
        (byte_t)((value >> 16) & 0xFF),
        (byte_t)((value >> 8) & 0xFF),
        (byte_t)(value & 0xFF),
    });
}

template<>
_CTYPE(ValueType::UINT24) decode<ValueType::UINT24>(const bytestring& data) {
    if (data.size() != 3) {
        throw std::length_error(fmt::format("Three bytes reqired; {:d} given", data.size()));
    }
    return static_cast<_CTYPE(ValueType::UINT24)>(
        data[0] << 16 |
        data[1] << 8 |
        data[2]
    );
}

}
}

#endif // __JETTER_INTERNAL_ENCODING_H
