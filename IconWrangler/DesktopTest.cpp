#include <array>
#include <sstream>
#include "gtest/gtest.h"
#include "Desktop.h"

class FakeDesktop : public IconWrangler::IDesktop {
public:
    [[nodiscard]] std::vector<IconWrangler::DesktopIcon> getIcons() const override {
        return icons;
    }

    void setIcons(const std::vector<IconWrangler::DesktopIcon>& icons_) override {
        icons = icons_;
    }

    std::vector<IconWrangler::DesktopIcon> icons;
};

TEST(DesktopTest, noIconsShouldWriteEmptyString) {
    FakeDesktop desktop;

    std::stringstream outputStream;
    outputStream << desktop;
    const std::string result = outputStream.str();

    EXPECT_TRUE(result.empty());
}

TEST(DesktopTest, iconsShouldBeWritten) {
    FakeDesktop desktop;
    IconWrangler::DesktopIcon icon0;
    icon0.name = "hello world";
    icon0.x = 32;
    icon0.y = 16;
    IconWrangler::DesktopIcon icon1;
    icon1.name = "MotoGP™13";
    icon1.x = 120;
    icon1.y = 44;
    desktop.icons.push_back(icon0);
    desktop.icons.push_back(icon1);

    std::stringstream outputStream;
    outputStream << desktop;
    const std::string result = outputStream.str();

    EXPECT_EQ(result, "hello world:32:16\nMotoGP™13:120:44");
}

TEST(DesktopTest, readingIconsFromFile) {
    FakeDesktop desktop;
    std::stringstream stream;
    stream << "hello world:32:16\nMotoGP™13:120:44";
    stream >> desktop;

    const std::vector<IconWrangler::DesktopIcon> icons = desktop.icons;
    ASSERT_EQ(icons.size(), 2);

    EXPECT_EQ(icons[0].name, std::string{"hello world"});
    EXPECT_EQ(icons[0].x, 32);
    EXPECT_EQ(icons[0].y, 16);

    EXPECT_EQ(icons[1].name, std::string{"MotoGP™13"});
    EXPECT_EQ(icons[1].x, 120);
    EXPECT_EQ(icons[1].y, 44);
}