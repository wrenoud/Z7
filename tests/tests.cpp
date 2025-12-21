// SPDX-FileCopyrightText: © 2025 Javier Jimenez Shaw <https://github.com/jjimenezshaw>
// SPDX-FileCopyrightText: © 2025 Weston James Renoud <https://github.com/wrenoud>

#include <gtest/gtest.h>

#include "../library.h"

namespace Z7 {
void PrintTo(const Z7Index &index, std::ostream *os) { *os << index.str(); }
} // namespace Z7

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
    EXPECT_EQ("00"_Z7.resolution(), 0);
    EXPECT_EQ("0012"_Z7.resolution(), 2);
    EXPECT_EQ("00123"_Z7.resolution(), 3);
    EXPECT_EQ("001234"_Z7.resolution(), 4);
    EXPECT_EQ("00123456777"_Z7.resolution(), 6);
    EXPECT_EQ("0012345601234560123456"_Z7.resolution(), 20);
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
        EXPECT_EQ("001203"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
}

TEST(Z7Index, addition_with_carries) {
    {
        const auto a = "08006666"_Z7;
        const auto b = "08003466"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("08006225"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "080016666"_Z7;
        const auto b = "080016466"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("080014043"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "080044"_Z7;
        const auto b = "080056"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("080403"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "080153"_Z7;
        const auto b = "080045"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("080531"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "080465"_Z7;
        const auto b = "080136"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("080524"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "080465"_Z7;
        const auto b = "080251"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("080656"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "0806666666666666666666"_Z7;
        EXPECT_EQ(20, a.resolution());
        const Z7::Z7Index r = a + a;
        EXPECT_EQ("0864242424242424242425"_Z7, r);
        const auto c = r + (-a);
        EXPECT_EQ(c, a);
    }
    {
        const auto a = "0846666666666666666666"_Z7;
        const auto b = "0816666666666666666666"_Z7;
        EXPECT_EQ(20, a.resolution());
        const Z7::Z7Index r = a + b;
        EXPECT_EQ(20, r.resolution());
        EXPECT_EQ("0844242424242424242425"_Z7, r);
        const auto c = r + (-b);
        EXPECT_EQ("15"_Z7, c); // invalid
    }
    {
        const auto a = "0866666666666666666666"_Z7;
        EXPECT_EQ(20, a.resolution());
        const Z7::Z7Index r = a + a;
        EXPECT_EQ("15"_Z7, r); // invalid
    }
    {
        const auto a = "0816666"_Z7;
        const auto b = "0816466"_Z7;
        const Z7::Z7Index r = a + b;
        EXPECT_EQ("15"_Z7, r); // invalid
    }
}

TEST(Z7Index, first_non_zero) {
    EXPECT_EQ(6, Z7::first_non_zero("0000000"_Z7));
    EXPECT_EQ(6, Z7::first_non_zero("1000000"_Z7));
    EXPECT_EQ(1, Z7::first_non_zero("1234000"_Z7));
    EXPECT_EQ(3, Z7::first_non_zero("1200567"_Z7));
    EXPECT_EQ(0, Z7::first_non_zero("1277777"_Z7));
}

TEST(Z7Index, neighbor) {
    EXPECT_EQ("0800046"_Z7, Z7::neighbor<1>("0800433"_Z7, 5).z7);
    EXPECT_EQ("0800064"_Z7, Z7::neighbor<2>("0800433"_Z7, 5).z7);
    EXPECT_EQ("0800065"_Z7, Z7::neighbor<3>("0800433"_Z7, 5).z7);
    EXPECT_EQ("0800430"_Z7, Z7::neighbor<4>("0800433"_Z7, 5).z7);
    EXPECT_EQ("0800431"_Z7, Z7::neighbor<5>("0800433"_Z7, 5).z7);
    EXPECT_EQ("0800432"_Z7, Z7::neighbor<6>("0800433"_Z7, 5).z7);

    EXPECT_EQ("0006464460645425020"_Z7, Z7::neighbor<1>("0006464460645425026"_Z7, 17).z7);
    EXPECT_EQ("0006464460645425245"_Z7, Z7::neighbor<2>("0006464460645425026"_Z7, 17).z7);
    EXPECT_EQ("0006464460645425022"_Z7, Z7::neighbor<3>("0006464460645425026"_Z7, 17).z7);
}

TEST(Z7Index, neighbors) {
    const auto a = "0800432"_Z7;
    const auto neig = Z7::neighbors(a, Z7::Z7Configuration{});
    EXPECT_EQ("0800433"_Z7, neig[0]);
    EXPECT_EQ("0800651"_Z7, neig[1]);
    EXPECT_EQ("0800064"_Z7, neig[2]);
    EXPECT_EQ("0800436"_Z7, neig[3]);
    EXPECT_EQ("0800430"_Z7, neig[4]);
    EXPECT_EQ("0800655"_Z7, neig[5]);
}

TEST(Z7Index, neighbors_exclusion_zone_1) {
    const auto a = "0300613"_Z7;
    const auto neig = Z7::neighbors(a, Z7::Z7Configuration{});
    EXPECT_EQ("0300032"_Z7, neig[0]);
    EXPECT_EQ("0300366"_Z7, neig[1]);
    EXPECT_EQ("0300364"_Z7, neig[2]);
    EXPECT_EQ("0300610"_Z7, neig[3]);
    EXPECT_EQ("0300611"_Z7, neig[4]);
    EXPECT_EQ("0300612"_Z7, neig[5]);
}

TEST(Z7Index, neighbors_exclusion_zone_2) {
    const auto a = "0300036"_Z7;
    const auto neig = Z7::neighbors(a, Z7::Z7Configuration{});
    EXPECT_EQ("0300030"_Z7, neig[0]);
    EXPECT_EQ("0300611"_Z7, neig[1]);
    EXPECT_EQ("0300032"_Z7, neig[2]);
    EXPECT_EQ("0300063"_Z7, neig[3]);
    EXPECT_EQ("0300034"_Z7, neig[4]);
    EXPECT_EQ("0300062"_Z7, neig[5]);
}

TEST(Z7Index, neighbors_exclusion_zone_3) {
    const auto a = "080041"_Z7;
    const auto neig = Z7::neighbors(a, Z7::Z7Configuration{});
    EXPECT_EQ("080016"_Z7, neig[0]);
    EXPECT_EQ("080043"_Z7, neig[1]);
    EXPECT_EQ("080004"_Z7, neig[2]);
    EXPECT_EQ("080045"_Z7, neig[3]);
    EXPECT_EQ("080014"_Z7, neig[4]);
    EXPECT_EQ("080040"_Z7, neig[5]);
}

TEST(Z7Index, neighbors_exclusion_zone_4) {
    const auto a = "080004"_Z7;
    const auto neig = Z7::neighbors(a, Z7::Z7Configuration{});
    EXPECT_EQ("080001"_Z7, neig[0]);
    EXPECT_EQ("080006"_Z7, neig[1]);
    EXPECT_EQ("080000"_Z7, neig[2]);
    EXPECT_EQ("080041"_Z7, neig[3]);
    EXPECT_EQ("080016"_Z7, neig[4]);
    EXPECT_EQ("080043"_Z7, neig[5]);
}

TEST(Z7Index, neighbors_pentagon) {
    const auto a = "0800"_Z7;
    const auto neig = Z7::neighbors(a, Z7::Z7Configuration{});
    EXPECT_EQ("0801"_Z7, neig[0]);
    EXPECT_EQ("0802"_Z7, neig[1]);
    EXPECT_EQ("0803"_Z7, neig[2]);
    EXPECT_EQ("0804"_Z7, neig[3]);
    EXPECT_EQ(Z7::Z7Index::invalid(), neig[4]);
    EXPECT_EQ("0806"_Z7, neig[5]);
}
