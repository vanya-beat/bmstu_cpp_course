#include <gtest/gtest.h>
#include "int2str.h"

TEST(int2str, BasicTest) {
    EXPECT_STREQ(int2str(0), "0");
}

TEST(int2str, BasicTestZero) {
    EXPECT_STREQ(int2str(0), "0");
}

TEST(int2str, BasicTestZeroNext) {
    EXPECT_STREQ(int2str(-0), "0");
}

TEST(int2str, BasicTestNumbers) {
    EXPECT_STREQ(int2str(-5), "-5");
    EXPECT_STREQ(int2str(-66), "-66");
    EXPECT_STREQ(int2str(-123), "-123");
    EXPECT_STREQ(int2str(2147483647), "2147483647");
    EXPECT_STREQ(int2str(-2147483648), "-2147483648");
}
