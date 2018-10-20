# pragma once

# include <sstream>
# include <string>
# include <vector>

# include "nlohmann/json.hpp"

# include "grok/core/generators/cmake.cpp"
# include "grok/core/project.cpp"
# include "grok/core/package.cpp"
# include "grok/core/registry.cpp"
# include "grok/core/repository.cpp"
# include "grok/core/utilities.cpp"

namespace grok::commands {
    using std::string;
    using std::stringstream;
    using std::vector;

    using json = nlohmann::json;
    using namespace grok::core;

    const int make (const bool& command_by_user, const vector<string>& command_arguments) {

        if (project::exists()) {
            utilities::print("package already exists; do you want to overwrite it? (y/n)");

            string overwrite = utilities::input();

            if (!overwrite.empty() && overwrite.compare("y") > 0) {
                return utilities::uninitialize(0);
            }
        }

        json package;
        json package_meta;

        utilities::print("what's the name of this package? (ex: massive_legend)");
        package_meta[ "name" ] = utilities::input();

        utilities::print("what's the repository's url? (ex: https://example.com/repo.git)");
        package_meta[ "repository" ] = utilities::input();

        utilities::print("what's the release branch/tag? (ex: master)");
        package_meta[ "release" ] = utilities::input();

        utilities::print("which c++ standard do you want to use? (ex: 17)");
        package_meta[ "standard" ] = utilities::input();

        package[ "package" ] = package_meta;

        return utilities::uninitialize(0);
    }
}
