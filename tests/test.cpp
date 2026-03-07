#include <gtest/gtest.h>
#include "chip8.h"

class testF : public ::testing::Test {
protected:
    TemplateProject::Library library;
    void SetUp() override {
    }
    void TearDown() override {}
};

TEST(test, test) {
    EXPECT_EQ(1, 1);
}

TEST_F(testF, test) {
    EXPECT_EQ(3, library.add(1, 2));
}