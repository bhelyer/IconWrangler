#include "Desktop.h"

#include <ShlObj.h>
#include <ExDisp.h>
#include <Shlwapi.h>
#include <atlbase.h>

#include "Util.h"

namespace {
// Colons can't appear in filenames, use it rather than a comma.
constexpr char delim = ':';
}

std::ostream& IconWrangler::operator<<(std::ostream& os, const IDesktop& desktop) {
    const std::vector<DesktopIcon> icons = desktop.getIcons();
    for (size_t i = 0; i < icons.size(); ++i) {
        if (i > 0) {
            os << '\n';
        }
        os << icons[i].name << delim << icons[i].x << delim << icons[i].y;
    }
    return os;
}

std::istream& IconWrangler::operator>>(std::istream& is, IDesktop& desktop) {
    std::vector<DesktopIcon> icons;

    for (std::string line; std::getline(is, line); ) {
        const std::vector<std::string> elements = split(line, delim);
        if (elements.size() != 3) {
            continue;
        }

        DesktopIcon icon;
        icon.name = elements[0];
        icon.x = std::stoi(elements[1]);
        icon.y = std::stoi(elements[2]);
        icons.push_back(icon);
    }

    desktop.setIcons(icons);
    return is;
}

namespace {
void FindDesktopFolderView(REFIID riid, void** ppv) {
    CComPtr<IShellWindows> spShellWindows;
    spShellWindows.CoCreateInstance(CLSID_ShellWindows);

    CComVariant vtLoc(CSIDL_DESKTOP);
    CComVariant vtEmpty;
    long lhwnd;
    CComPtr<IDispatch> spdisp;
    spShellWindows->FindWindowSW(&vtLoc, &vtEmpty, SWC_DESKTOP, &lhwnd, SWFO_NEEDDISPATCH, &spdisp);

    CComPtr<IShellBrowser> spBrowser;
    CComQIPtr<IServiceProvider>(spdisp)->QueryService(SID_STopLevelBrowser, IID_PPV_ARGS(&spBrowser));

    CComPtr<IShellView> spView;
    spBrowser->QueryActiveShellView(&spView);
    spView->QueryInterface(riid, ppv);
}
}

IconWrangler::Win32Desktop::Win32Desktop() {
    CComPtr<IFolderView> spView;
    FindDesktopFolderView(IID_PPV_ARGS(&spView));
    CComPtr<IShellFolder> spFolder;
    spView->GetFolder(IID_PPV_ARGS(&spFolder));

    CComPtr<IEnumIDList> spEnum;
    spView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
    for (CComHeapPtr<ITEMID_CHILD> spidl; spEnum->Next(1, &spidl, nullptr) == S_OK; spidl.Free()) {
        STRRET str;
        spFolder->GetDisplayNameOf(spidl, SHGDN_NORMAL, &str);
        CComHeapPtr<char> spszName;
        StrRetToStrA(&str, spidl, &spszName);

        POINT pt;
        spView->GetItemPosition(spidl, &pt);

        IconWrangler::DesktopIcon icon;
        icon.name = spszName;
        icon.x = pt.x;
        icon.y = pt.y;
        mIcons.push_back(icon);
    }
}

std::vector<IconWrangler::DesktopIcon> IconWrangler::Win32Desktop::getIcons() const {
    return mIcons;
}

void IconWrangler::Win32Desktop::setIcons(const std::vector<DesktopIcon>& icons) {
    mIcons = icons;
}