#include "Arguments.h"

IconWrangler::Arguments::Arguments(const std::vector<std::string>& commandLine) {
    for (const std::string& arg : commandLine) {
        if (arg.empty()) {
            continue;
        }
        const char firstCharacter = arg[0];

        if (arg == "--test") {
            runTests = true;
        } else if (firstCharacter == 's' || firstCharacter == 'S') {
            command = Command::Save;
        } else if (firstCharacter == 'l' || firstCharacter == 'L') {
            command = Command::Load;
        }
    }
}