# pragma once

# include <map>
# include <sstream>
# include <string>
# include <vector>

# include "nlohmann/json.hpp"

# include "grok/core/generators/cmake.cpp"
# include "grok/core/project.cpp"
# include "grok/core/utilities.cpp"

namespace grok::commands {
    using std::map;
    using std::string;
    using std::stringstream;
    using std::vector;

    using json = nlohmann::json;
    using namespace grok::core;

    const int make (const bool& command_by_user, const vector<string>& command_arguments) {

        if (project::exists()) {
            utilities::print("package already exists; do you want to overwrite it? (y/n)");

            string overwrite = utilities::input();

            if (!overwrite.empty() && overwrite != "y") {
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

        utilities::print("are there any flags you want to build with? (ex: pthread,foo=1)");
        package_meta[ "flags" ] = utilities::split(utilities::input(), ',');

        utilities::print("where are the executables? (ex: src/main.cpp,src/foo.cpp)");
        package_meta[ "executables" ] = utilities::split(utilities::input(), ',');

        package[ "package" ] = package_meta;

        utilities::print("what should the 'include' directories be? (ex: foo/,bar/");
        package[ "includes" ] = utilities::split(utilities::input(), ',');

        utilities::print("are there any libraries you need? (ex: stdc++fs,foo=libs/foo.so)");
        package[ "libraries" ] = map<string, string>();

        const vector<string> pairs = utilities::split(utilities::input(), ',');

        for (string library : pairs) {
            const vector<string> pair = utilities::split(library, '=');
            package[ "libraries" ][ pair.front() ] = (pair.size() > 1) ? pair.back() : "";
        }

        package[ "dependencies" ] = map<string, string>();

        project::save(package.dump(4));
        project::apply(generators::cmake(package));

        utilities::print("project created");
        return utilities::uninitialize(0);
    }
}
