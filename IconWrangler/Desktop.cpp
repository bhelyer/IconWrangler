#include "Desktop.h"

#include <cassert>
#include <algorithm>

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

void IconWrangler::Win32Desktop::arrangeIcons() {
    CComPtr<IFolderView> spView;
    FindDesktopFolderView(IID_PPV_ARGS(&spView));
    CComPtr<IShellFolder> spFolder;
    spView->GetFolder(IID_PPV_ARGS(&spFolder));

    std::vector<LPCITEMIDLIST> items;
    std::vector<POINT> points;

    CComPtr<IEnumIDList> spEnum;
    spView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
    for (CComHeapPtr<ITEMID_CHILD> spIdl; spEnum->Next(1, &spIdl, nullptr) == S_OK;) {
        STRRET str;
        spFolder->GetDisplayNameOf(spIdl, SHGDN_NORMAL, &str);
        CComHeapPtr<char> spszName;
        StrRetToStrA(&str, spIdl, &spszName);
        const std::string name = (char*)spszName;

        auto it = std::find_if(mIcons.begin(), mIcons.end(), [&](const auto& icon) {
            return name == icon.name;
        });
        if (it == mIcons.end()) {
            spIdl.Free();
            continue;
        }

        POINT point;
        point.x = it->x;
        point.y = it->y;

        items.push_back(spIdl);
        points.push_back(point);
        spIdl.Detach();
    }

    assert(items.size() == points.size() && "items and points are not the same size");
    const auto cidl = static_cast<UINT>(items.size());
    spView->SelectAndPositionItems(cidl, items.data(), points.data(), 0);

    for (auto item : items) {
        /* If the rest of this garbage hasn't convinced you that I have
         * no clue with what I'm doing with regards to COM, this has to
         * do the trick.
         */
        CComHeapPtr<ITEMID_CHILD> spIdl;
        spIdl.Attach((_ITEMIDLIST*)(item));
        spIdl.Free();
    }
}

std::vector<IconWrangler::DesktopIcon> IconWrangler::Win32Desktop::getIcons() const {
    return mIcons;
}

void IconWrangler::Win32Desktop::setIcons(const std::vector<DesktopIcon>& icons) {
    mIcons = icons;
}