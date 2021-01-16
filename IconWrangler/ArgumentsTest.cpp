#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "Arguments.h"

TEST(ArgumentsTest, ifTestIsGivenRunTestsIsSet) {
    const std::vector<std::string> commandLine{"iw.exe", "--test"};
    const IconWrangler::Arguments arguments{commandLine};
    EXPECT_TRUE(arguments.runTests);
}

TEST(ArgumentsTest, noCommandSetsCommandToNo) {
    const std::vector<std::string> commandLine{"iw.exe"};
    const IconWrangler::Arguments arguments{commandLine};
    EXPECT_FALSE(arguments.command);
}

TEST(ArgumentsTest, saveSetsCommandToSave) {
    const std::vector<std::string> commandLine0{"iw.exe", "save"}, commandLine1{"iw.exe", "SAVE"};
    const IconWrangler::Arguments arguments0{commandLine0}, arguments1{commandLine1};

    ASSERT_TRUE(arguments0.command);
    EXPECT_EQ(*arguments0.command, IconWrangler::Arguments::Command::Save);

    ASSERT_TRUE(arguments1.command);
    EXPECT_EQ(*arguments1.command, IconWrangler::Arguments::Command::Save);
}

TEST(ArgumentsTest, loadSetsCommandToLoad) {
    const std::vector<std::string> commandLine0{"iw.exe", "load"}, commandLine1{"iw.exe", "LOAD"};
    const IconWrangler::Arguments arguments0{commandLine0}, arguments1{commandLine1};

    ASSERT_TRUE(arguments0.command);
    EXPECT_EQ(*arguments0.command, IconWrangler::Arguments::Command::Load);

    ASSERT_TRUE(arguments1.command);
    EXPECT_EQ(*arguments1.command, IconWrangler::Arguments::Command::Load);
}