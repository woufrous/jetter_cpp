#ifndef __JETTER_TYPES_H
#define __JETTER_TYPES_H

#include <string>

namespace jetter {

using byte_t = uint8_t;
using bytestring = std::basic_string<byte_t>;
using bytestringstream = std::basic_stringstream<byte_t>;
using obytestringstream = std::basic_ostringstream<byte_t>;

}

#endif // __JETTER_TYPES_H
