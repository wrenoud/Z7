// SPDX-FileCopyrightText: © 2025 Javier Jimenez Shaw <https://github.com/jjimenezshaw>
// SPDX-FileCopyrightText: © 2025 Weston James Renoud <https://github.com/wrenoud>

#include <gtest/gtest.h>

#include "../library.h"

TEST(Utils, BitWidth) {
    EXPECT_EQ(Z7::Utils::bit_width(0b0000), 0);
    EXPECT_EQ(Z7::Utils::bit_width(0b0001), 1);
    EXPECT_EQ(Z7::Utils::bit_width(0b0010), 2);
    EXPECT_EQ(Z7::Utils::bit_width(0b0011), 2);
    EXPECT_EQ(Z7::Utils::bit_width(0b0100), 3);
    EXPECT_EQ(Z7::Utils::bit_width(0b0101), 3);
    EXPECT_EQ(Z7::Utils::bit_width(0b0110), 3);
    EXPECT_EQ(Z7::Utils::bit_width(0b0111), 3);
}

TEST(Utils, countl_zero) {
    EXPECT_EQ(Z7::Utils::countl_zero(0b0000000000000000000000000000000000000000000000000000000000000000ull), 64);
    EXPECT_EQ(Z7::Utils::countl_zero(0b1111111111111111111111111111111111111111111111111111111111111111ull), 0);
    EXPECT_EQ(Z7::Utils::countl_zero(0b1111000000000000000000000000000000000000000000000000000000000000ull), 0);
    EXPECT_EQ(Z7::Utils::countl_zero(0b0001111000000000000000000000000000000000000000000000000000000000ull), 3);
}

TEST(Utils, countr_zero) {
    EXPECT_EQ(Z7::Utils::countr_zero(0b0000000000000000000000000000000000000000000000000000000000000000ull), 64);
    EXPECT_EQ(Z7::Utils::countr_zero(0b1111111111111111111111111111111111111111111111111111111111111111ull), 0);
    EXPECT_EQ(Z7::Utils::countr_zero(0b1111000000000000000000000000000000000000000000000000000000000000ull), 60);
    EXPECT_EQ(Z7::Utils::countr_zero(0b0001111000000000000000000000000000000000000000000000000000000000ull), 57);
    EXPECT_EQ(Z7::Utils::countr_zero(0b00011110), 1);
    EXPECT_EQ(Z7::Utils::countr_zero(0b00011100), 2);
}

TEST(Utils, countr_one) {
    EXPECT_EQ(Z7::Utils::countr_one(0b0000), 0);
    EXPECT_EQ(Z7::Utils::countr_one(0b0001), 1);
    EXPECT_EQ(Z7::Utils::countr_one(0b0010), 0);
    EXPECT_EQ(Z7::Utils::countr_one(0b0011), 2);
    EXPECT_EQ(Z7::Utils::countr_one(0b0100), 0);
    EXPECT_EQ(Z7::Utils::countr_one(0b0101), 1);
    EXPECT_EQ(Z7::Utils::countr_one(0b0110), 0);
    EXPECT_EQ(Z7::Utils::countr_one(0b0111), 3);
    EXPECT_EQ(Z7::Utils::countr_one( 0b00000000 ), 0);
    EXPECT_EQ(Z7::Utils::countr_one( 0b11111111 ), 8);
    EXPECT_EQ(Z7::Utils::countr_one( 0b11111110 ), 0);
    EXPECT_EQ(Z7::Utils::countr_one( 0b11100011 ), 2);
}
