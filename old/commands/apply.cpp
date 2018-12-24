# pragma once

# include <iostream>
# include <regex>
# include <string>
# include <vector>


# include "grok/core/generators/cmake.cpp"
# include "grok/core/project.cpp"
# include "grok/core/utilities.cpp"

namespace grok::commands {

    using std::string;
    using std::vector;

    using namespace nlohmann;
    using namespace grok::core;

    const int apply (const bool& command_by_user, const vector<string>& command_arguments) {
        project::apply(generators::cmake(project::open(), true));
        utilities::print("project applied");
        return utilities::uninitialize(0);
    }
}