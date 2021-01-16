#include "Desktop.h"

std::ostream& IconWrangler::operator<<(std::ostream& os, const IDesktop& desktop) {
    const std::vector<DesktopIcon> icons = desktop.getIcons();
    for (size_t i = 0; i < icons.size(); ++i) {
        if (i > 0) {
            os << '\n';
        }
        os << icons[i].name << ',' << icons[i].x << ',' << icons[i].y;
    }
    return os;
}