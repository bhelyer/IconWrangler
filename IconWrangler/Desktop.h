#ifndef ICONWRANGLER_DESKTOP_H
#define ICONWRANGLER_DESKTOP_H

#include <vector>
#include <string>
#include <istream>
#include <ostream>

namespace IconWrangler {

struct DesktopIcon {
    std::string name;
    int x = 0;
    int y = 0;
};

class IDesktop {
public:
    virtual ~IDesktop() = default;
    [[nodiscard]] virtual std::vector<DesktopIcon> getIcons() const = 0;
    virtual void setIcons(const std::vector<DesktopIcon>& icons) = 0;
};

std::ostream& operator<<(std::ostream& os, const IDesktop& desktop);
std::istream& operator>>(std::istream& is, IDesktop& desktop);

class Win32Desktop : public IDesktop {
public:
    Win32Desktop();
    [[nodiscard]] std::vector<DesktopIcon> getIcons() const override;
    void setIcons(const std::vector<DesktopIcon>& icons) override;

private:
    std::vector<DesktopIcon> mIcons;
};

}

#endif //ICONWRANGLER_DESKTOP_H