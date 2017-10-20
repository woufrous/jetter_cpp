#include <cstdint>
#include <tuple>
#include <vector>

#include <gtest/gtest.h>

#include <jetter/types.h>
#include <jetter/internal/encoding.h>

using namespace ::jetter;
using namespace ::jetter::internal;

TEST(EncodingTest, Int8) {
    using signed_t = _CTYPE(ValueType::INT8);
    using unsigned_t = _CTYPE(ValueType::UINT8);

    std::vector<std::tuple<bytestring, signed_t, unsigned_t>> test_vec {
        {{0x00}, 0, 0},
        {{0x01}, 1, 1},
        {{0xFF}, -1, 255}
    };

    for (const auto& tpl : test_vec) {
        // signed
        EXPECT_EQ(
            std::get<0>(tpl),
            encode<ValueType::INT8>(std::get<1>(tpl))
        );
        EXPECT_EQ(
            std::get<1>(tpl),
            decode<ValueType::INT8>(std::get<0>(tpl))
        );
        // unsigned
        EXPECT_EQ(
            std::get<0>(tpl),
            encode<ValueType::UINT8>(std::get<2>(tpl))
        );
        EXPECT_EQ(
            std::get<2>(tpl),
            decode<ValueType::UINT8>(std::get<0>(tpl))
        );
    }
}

TEST(EncodingTest, Int16) {
    using signed_t = _CTYPE(ValueType::INT16);
    using unsigned_t = _CTYPE(ValueType::UINT16);

    std::vector<std::tuple<bytestring, signed_t, unsigned_t>> test_vec {
        {{0x00, 0x00}, 0, 0},
        {{0x00, 0x01}, 1, 1},
        {{0xFF, 0xFF}, -1, 65535},
        {{0x01, 0x00}, 256, 256},
        {{0xFF, 0x00}, -256, 65280},
    };

    for (const auto& tpl : test_vec) {
        // signed
        EXPECT_EQ(
            std::get<0>(tpl),
            encode<ValueType::INT16>(std::get<1>(tpl))
        );
        EXPECT_EQ(
            std::get<1>(tpl),
            decode<ValueType::INT16>(std::get<0>(tpl))
        );
        // unsigned
        EXPECT_EQ(
            std::get<0>(tpl),
            encode<ValueType::UINT16>(std::get<2>(tpl))
        );
        EXPECT_EQ(
            std::get<2>(tpl),
            decode<ValueType::UINT16>(std::get<0>(tpl))
        );
    }
}

TEST(EncodingTest, Int24) {
    using signed_t = _CTYPE(ValueType::INT24);
    using unsigned_t = _CTYPE(ValueType::UINT24);

    std::vector<std::tuple<bytestring, signed_t, unsigned_t>> test_vec {
        {{0x00, 0x00, 0x00}, 0, 0},
        {{0xFF, 0xFF, 0xFF}, -1, 16777215},
        {{0x00, 0x01, 0x00}, 256, 256},
        {{0xFF, 0xFF, 0x00}, -256, 16776960},
        {{0x01, 0x00, 0x00}, 65536, 65536},
        {{0xFF, 0x00, 0x00}, -65536, 16711680},
    };

    for (const auto& tpl : test_vec) {
        // signed
        EXPECT_EQ(
            std::get<0>(tpl),
            encode<ValueType::INT24>(std::get<1>(tpl))
        );
        EXPECT_EQ(
            std::get<1>(tpl),
            decode<ValueType::INT24>(std::get<0>(tpl))
        );
        // unsigned
        EXPECT_EQ(
            std::get<0>(tpl),
            encode<ValueType::UINT24>(std::get<2>(tpl))
        );
        EXPECT_EQ(
            std::get<2>(tpl),
            decode<ValueType::UINT24>(std::get<0>(tpl))
        );
    }
}
