# pragma once

# include <string>
# include <vector>

# include "grok/core/utilities.cpp"

namespace grok::commands {
    using std::string;
    using std::vector;

    using namespace grok::core;

    int sync (const bool& command_by_user, const vector<string>& command_arguments) {
        utilities::call_script("sync_registry.sh");
        utilities::print("synced");
        return utilities::uninitialize(0);
    }
}