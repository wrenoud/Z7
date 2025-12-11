#include <gtest/gtest.h>

#include "library.h"

// Demonstrate some basic assertions.
TEST(Z7Index, UserLiteral) {
    const auto index = "1111"_Z7;

    EXPECT_EQ(index.hierarchy.base, 11);
    EXPECT_EQ(index.hierarchy.i01, 1);
    EXPECT_EQ(index.hierarchy.i02, 1);
    EXPECT_EQ(index.hierarchy.i03, 7);
    EXPECT_EQ(index.hierarchy.i20, 7);
}

TEST(Z7Index, BrackerOperatorReadAccess) {
    const auto index = "0222"_Z7;

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
        const auto index = "00"_Z7;
        EXPECT_EQ(index.resolution(), 0);
    }
    {
        const auto index = "0012"_Z7;
        EXPECT_EQ(index.resolution(), 2);
    }
    {
        const auto index = "00123"_Z7;
        EXPECT_EQ(index.resolution(), 3);
    }
    {
        const auto index = "001234"_Z7;
        EXPECT_EQ(index.resolution(), 4);
    }
    {
        const auto index = "00123456777"_Z7;
        EXPECT_EQ(index.resolution(), 6);
    }
    {
        const auto index = "0012345601234560123456"_Z7;
        EXPECT_EQ(index.resolution(), 20);
    }
}

TEST(Z7Index, str) {
    const auto a = "091201"_Z7;
    EXPECT_EQ("091201", a.str());
    const auto b = "1001201"_Z7;
    EXPECT_EQ("1001201", b.str());
    const auto c = "0012345601201234560122"_Z7;
    EXPECT_EQ(20, c.resolution());
    EXPECT_EQ("0012345601201234560122", c.str());
}

TEST(Z7Index, negation) {
    {
        const auto a = "071201"_Z7;
        const Z7::Z7Index r = -a;
        const Z7::Z7Index res = "076506"_Z7;
        EXPECT_TRUE(res == r);
    }
}

TEST(Z7Index, addition_simple) {
    {
        const auto a = "001201"_Z7;
        const auto b = "000002"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "001203"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
}

TEST(Z7Index, addition_with_carries) {
    {
        const auto a = "08006666"_Z7;
        const auto b = "08003466"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "08006245"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "080016666"_Z7;
        const auto b = "080016466"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "080162245"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "080044"_Z7;
        const auto b = "080056"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "080563"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "080153"_Z7;
        const auto b = "080045"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "081621"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "080465"_Z7;
        const auto b = "080136"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "080524"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "080465"_Z7;
        const auto b = "080251"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "080656"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "0806666666666666666666"_Z7;
        EXPECT_EQ(20, a.resolution());
        const Z7::Z7Index r = a + a;
        const Z7::Z7Index res = "0864444444444444444445"_Z7;
        EXPECT_TRUE(res == r);
        const auto c = r + (-a);
        EXPECT_TRUE(c == a);
    }
    {
        const auto a = "0846666666666666666666"_Z7;
        const auto b = "0816666666666666666666"_Z7;
        EXPECT_EQ(20, a.resolution());
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "0844444444444444444445"_Z7;
        EXPECT_EQ(20, r.resolution());
        EXPECT_TRUE(res == r);
        const auto c = r + (-b);
        EXPECT_TRUE(c == "15"_Z7);
    }
    {
        const auto a = "0866666666666666666666"_Z7;
        EXPECT_EQ(20, a.resolution());
        const Z7::Z7Index r = a + a;
        const Z7::Z7Index res = "15"_Z7; // invalid
        EXPECT_TRUE(res == r);
    }
    {
        const auto a = "0816666"_Z7;
        const auto b = "0816466"_Z7;
        const Z7::Z7Index r = a + b;
        const Z7::Z7Index res = "15"_Z7; // invalid
        EXPECT_TRUE(res == r);
    }
}
