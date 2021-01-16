#ifndef ICONWRANGLER_ARGUMENTS_H
#define ICONWRANGLER_ARGUMENTS_H

#include <vector>
#include <string>
#include <optional>

namespace IconWrangler {

/* Parse command line arguments into a semantic representation.
 */
class Arguments {
public:
    enum class Command {
        Save,
        Load,
    };

public:
    // Parse the flags. Can include the executable name.
    explicit Arguments(const std::vector<std::string>& commandLine);

public:
    bool runTests = false;
    std::optional<Command> command;
};

}

#endif //ICONWRANGLER_ARGUMENTS_H
