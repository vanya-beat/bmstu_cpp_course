#include <gtest/gtest.h>
#include "str2int.h"


TEST(str2int, BasicTest) {
    EXPECT_EQ(str2int("0"), 0);
}

TEST(str2int, BasicTestZero) {
    EXPECT_EQ(str2int("+0"), 0);
}

TEST(str2int, BasicTestZeroNext) {
    EXPECT_DEATH(str2int("-"), "");
}

//TEST(str2int, BasicTest1) {
//    EXPECT_EQ(str2int("1"), 1);
//}
//
//TEST(str2int, BasicTestPlus1) {
//    EXPECT_EQ(str2int("+1"), 1);
//}
//
//TEST(str2int, BasicTestMinus1) {
//    EXPECT_EQ(str2int("-1"), -1);
//}