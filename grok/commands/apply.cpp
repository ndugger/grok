# pragma once

# include <iostream>
# include <regex>
# include <string>
# include <vector>

# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/core/utilities.cpp"

namespace grok::commands {

    using std::string;
    using std::vector;

    using namespace nlohmann;
    using namespace grok::core;

    int apply (string command_directory, vector<string> command_arguments, bool command_by_user) {

    }
}