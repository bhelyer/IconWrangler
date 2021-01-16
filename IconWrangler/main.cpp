#include <fstream>
#include <iostream>

#include <Windows.h>
#include <Shlwapi.h>

#include "gtest/gtest.h"

#include "Desktop.h"
#include "Arguments.h"

class CCoInitialize {
public:
    CCoInitialize() : m_hr(CoInitialize(nullptr)) { }
    ~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
    HRESULT m_hr;
};

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
    IconWrangler::Win32Desktop desktop;

    if (*arguments.command == IconWrangler::Arguments::Command::Save) {
        std::ofstream ofs{"icons.txt"};
        if (!ofs) {
            std::cerr << "Couldn't open 'icons.txt' for writing.\n";
            return 1;
        }
        ofs << desktop;
        if (!ofs) {
            std::cerr << "Failed to write to icons.txt.\n";
            return 1;
        }
    } else {
        // TODO: Load.
    }

    return 0;
}