#ifndef __JETTER_INTERNAL_ESCAPE_H
#define __JETTER_INTERNAL_ESCAPE_H

#include <jetter/types.h>

namespace jetter {
namespace internal {

const jetter::byte_t JETTER_START = 0xDA;
const jetter::byte_t JETTER_STOP = 0xDB;
const jetter::byte_t JETTER_SCF = 0xD8;

bytestring escape(const bytestring& data);
bytestring unescape(const bytestring& data);
byte_t checksum(const bytestring& data);

}
}

#endif // __JETTER_INTERNAL_ESCAPE_H
