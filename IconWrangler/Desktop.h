#ifndef ICONWRANGLER_DESKTOP_H
#define ICONWRANGLER_DESKTOP_H

#include <vector>
#include <string>
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
};

std::ostream& operator<<(std::ostream& os, const IDesktop& desktop);

}

#endif //ICONWRANGLER_DESKTOP_H