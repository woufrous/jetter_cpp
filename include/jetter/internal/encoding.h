#ifndef __JETTER_INTERNAL_ENCODING_H
#define __JETTER_INTERNAL_ENCODING_H

#include <cstdint>
#include <stdexcept>

#include <jetter/types.h>

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
bytestring encode<ValueType::INT8>(_CTYPE(ValueType::INT8) value);

template<>
_CTYPE(ValueType::INT8) decode<ValueType::INT8>(const bytestring& data);

// UINT8
template<>
bytestring encode<ValueType::UINT8>(_CTYPE(ValueType::UINT8) value);

template<>
_CTYPE(ValueType::UINT8) decode<ValueType::UINT8>(const bytestring& data);

// INT16
template<>
bytestring encode<ValueType::INT16>(_CTYPE(ValueType::INT16) value);

template<>
_CTYPE(ValueType::INT16) decode<ValueType::INT16>(const bytestring& data);

// UINT16
template<>
bytestring encode<ValueType::UINT16>(_CTYPE(ValueType::UINT16) value);

template<>
_CTYPE(ValueType::UINT16) decode<ValueType::UINT16>(const bytestring& data);

// INT24
template<>
bytestring encode<ValueType::INT24>(_CTYPE(ValueType::INT24) value);

template<>
_CTYPE(ValueType::INT24) decode<ValueType::INT24>(const bytestring& data);

// UINT24
template<>
bytestring encode<ValueType::UINT24>(_CTYPE(ValueType::UINT24) value);

template<>
_CTYPE(ValueType::UINT24) decode<ValueType::UINT24>(const bytestring& data);

}
}

#endif // __JETTER_INTERNAL_ENCODING_H
