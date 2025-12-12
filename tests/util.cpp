#include <gtest/gtest.h>

#include "../library.h"

TEST(BitWidth, BasicTests) {
    EXPECT_EQ(Z7::Utils::bit_width(0b0000), 0);
    EXPECT_EQ(Z7::Utils::bit_width(0b0001), 1);
    EXPECT_EQ(Z7::Utils::bit_width(0b0010), 2);
    EXPECT_EQ(Z7::Utils::bit_width(0b0011), 2);
    EXPECT_EQ(Z7::Utils::bit_width(0b0100), 3);
    EXPECT_EQ(Z7::Utils::bit_width(0b0101), 3);
    EXPECT_EQ(Z7::Utils::bit_width(0b0110), 3);
    EXPECT_EQ(Z7::Utils::bit_width(0b0111), 3);
}
