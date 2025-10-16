#include "bmstu_optional.h"
#include <gtest/gtest.h>
#include <algorithm>

struct Complex
{
    Complex() = default;
    Complex(double real, double imagine) : a(real), b(imagine) {}  
    ~Complex() {}
    
    double a;
    double b;
};

struct Cord
{
    Cord() = default;
    Cord(int real, int imagine) : a(real), b(imagine) {}  
    ~Cord() {}
    
    int a;
    int b;
};

TEST(Optional, Init)
{
    bmstu::optional<Cord> c(Cord{1, 3});
    c.emplace(5, 0xDEAD);
    int a = 5;
    ASSERT_TRUE(c.has_value());
}

TEST(Optional, ValueTest)
{
    bmstu::optional<Cord> c;
    ASSERT_FALSE(c.has_value());
    
    c.emplace(10, 20);
    ASSERT_TRUE(c.has_value());
    EXPECT_EQ(c->a, 10);
    EXPECT_EQ(c->b, 20);
}

TEST(Optional, ComplexTest)
{
    bmstu::optional<Complex> comp(Complex{1.5, 2.5});
    ASSERT_TRUE(comp.has_value());
    EXPECT_DOUBLE_EQ(comp->a, 1.5);
    EXPECT_DOUBLE_EQ(comp->b, 2.5);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

