#include <map>

#include <gtest/gtest.h>

#include <jetter/internal/encode.h>

using namespace jetter;
using namespace jetter::internal;


TEST(SerialTest, UnescapeSingle) {
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

    for (const auto& x : test_data) {
        EXPECT_EQ(x.second, escape(x.first));
    }
}
