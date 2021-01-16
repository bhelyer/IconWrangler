#include "Desktop.h"

#include <ShlObj.h>
#include <ExDisp.h>
#include <Shlwapi.h>
#include <atlbase.h>

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