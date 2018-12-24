# pragma once

# include <string>
# include <vector>

# include "grok/core/utilities.cpp"

namespace grok::commands {
    using std::string;
    using std::vector;

    using namespace grok::core;

    int update (const bool& command_by_user, const vector<string>& command_arguments) {
        string package_name;
        string package_release;

        if (command_arguments.empty()) {
            utilities::print("update what?");
            return utilities::uninitialize(1);
        }

        package_name = command_arguments.at(0);

        if (command_arguments.size() > 1) {
            package_release = command_arguments.at(1);
        }
        else {
            package_release = "master";
        }

        utilities::call_script("update_package.sh", package_name + " " + package_release);
        utilities::print("updated " + package_name);
        return utilities::uninitialize(0);
    }
}