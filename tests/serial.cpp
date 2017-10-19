#include <map>
#include <random>

#include <gtest/gtest.h>

#include <jetter/internal/serial_escape.h>

using namespace jetter;
using namespace jetter::internal;

bytestring random_bytestring(size_t length) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<byte_t> uniform;

    bytestring rnd_string;

    for (;length;--length) {
        rnd_string += uniform(gen);
    }

    return rnd_string;
}

std::map<bytestring, bytestring> test_data {
    {{0xAA}, {0xAA}},
    {{0xD8}, {JETTER_SCF, 0xC8}},
    {{0xD9}, {JETTER_SCF, 0xC9}},
    {{0xDA}, {JETTER_SCF, 0xCA}},
    {{0xDB}, {JETTER_SCF, 0xCB}},
    {{0xDC}, {JETTER_SCF, 0xCC}},
    {{0xDD}, {JETTER_SCF, 0xCD}},
    {{0xDE}, {JETTER_SCF, 0xCE}},
    {{0xDF}, {JETTER_SCF, 0xCF}},
    {{0xDA, 0xDB}, {JETTER_SCF, 0xCA, JETTER_SCF, 0xCB}}
};

TEST(SerialTest, Escape) {
    for (const auto& x : test_data) {
        EXPECT_EQ(x.second, escape(x.first));
    }
}

TEST(SerialTest, Unescape) {
    for (const auto& x : test_data) {
        EXPECT_EQ(x.first, unescape(x.second));
    }
}

TEST(SerialTest, EscapeRandom) {
    bytestring test_string = random_bytestring(512);

    for (const auto& c : escape(test_string)) {
        ASSERT_FALSE((c != JETTER_SCF) && (0xD8 <= c) && (c <= 0xDF));
    }
}

TEST(SerialTest, UnescapeEscapeRandom) {
    bytestring test_string = random_bytestring(512);
    ASSERT_EQ(test_string, unescape(escape(test_string)));
}
