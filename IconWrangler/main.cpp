#include <cstdio>
#include <iostream>

#include <Windows.h>
#include <ShlObj.h>
#include <ExDisp.h>
#include <Shlwapi.h>
#include <atlbase.h>
#include <atlalloc.h>

#include "gtest/gtest.h"

#include "Arguments.h"

class CCoInitialize {
public:
    CCoInitialize() : m_hr(CoInitialize(nullptr)) { }
    ~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
    HRESULT m_hr;
};

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

int main(int argc, char** argv) {
    const std::vector<std::string> commandLine(argv, argv + argc);
    IconWrangler::Arguments arguments{commandLine};

    if (arguments.runTests) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

    if (!arguments.command) {
        std::cout << "Usage: " << commandLine[0] << " (--test) [save] [load]\n";
        return 1;
    }

    CCoInitialize init;
    CComPtr<IFolderView> spView;
    FindDesktopFolderView(IID_PPV_ARGS(&spView));
    CComPtr<IShellFolder> spFolder;
    spView->GetFolder(IID_PPV_ARGS(&spFolder));

    CComPtr<IEnumIDList> spEnum;
    spView->Items(SVGIO_ALLVIEW, IID_PPV_ARGS(&spEnum));
    for (CComHeapPtr<ITEMID_CHILD> spidl; spEnum->Next(1, &spidl, nullptr) == S_OK; spidl.Free()) {
        STRRET str;
        spFolder->GetDisplayNameOf(spidl, SHGDN_NORMAL, &str);
        CComHeapPtr<wchar_t> spszName;
        StrRetToStr(&str, spidl, &spszName);

        POINT pt;
        spView->GetItemPosition(spidl, &pt);

        wprintf(L"At %4d,%4d is %ls.\n", pt.x, pt.y, spszName);
    }

    return 0;
}