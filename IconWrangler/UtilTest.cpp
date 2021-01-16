#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "Util.h"

TEST(UtilTest, splitDividesAStringByACharacter) {
    const std::string input = "hello world,b,c";
    const std::vector<std::string> elements = IconWrangler::split(input, ',');

    ASSERT_EQ(elements.size(), 3);
    EXPECT_EQ(elements[0], "hello world");
    EXPECT_EQ(elements[1], "b");
    EXPECT_EQ(elements[2], "c");
}