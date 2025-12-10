#include <gtest/gtest.h>

#include "library.h"

// Demonstrate some basic assertions.
TEST(Z7Index, UserLiteral) {
    const auto index = 1111_Z7;

    EXPECT_EQ(index.hierarchy.base, 11);
    EXPECT_EQ(index.hierarchy.i01, 1);
    EXPECT_EQ(index.hierarchy.i02, 1);
    EXPECT_EQ(index.hierarchy.i03, 7);
    EXPECT_EQ(index.hierarchy.i20, 7);
}

TEST(Z7Index, BrackerOperatorReadAccess) {
    const auto index = 0222_Z7;

    EXPECT_EQ(index[0], 2);
    EXPECT_EQ(index[1], 2);
    EXPECT_EQ(index[2], 2);
    EXPECT_EQ(index[3], 7);
    EXPECT_EQ(index[20], 7);
}

TEST(Z7Index, BrackerOperatorWriteAccess) {
    Z7::Z7Index index;
    index[0] = 5;
    index[1] = 3;
    index[2] = 6;

    EXPECT_EQ(index.hierarchy.base, 5);
    EXPECT_EQ(index.hierarchy.i01, 3);
    EXPECT_EQ(index.hierarchy.i02, 6);
}

TEST(Z7Index, Resolution) {
    {
        const auto index = 0012_Z7;
        EXPECT_EQ(index.resolution(), 2);
    }
    {
        const auto index = 00123_Z7;
        EXPECT_EQ(index.resolution(), 3);
    }
    {
        const auto index = 001234_Z7;
        EXPECT_EQ(index.resolution(), 4);
    }
    {
        const auto index = 00123456777_Z7;
        EXPECT_EQ(index.resolution(), 6);
    }
    {
        const auto index = 0012345601234560123456_Z7;
        EXPECT_EQ(index.resolution(), 20);
    }
}
