# pragma once

# include <iostream>
# include <string>
# include <vector>

# include "grok/core/utilities.cpp"

namespace grok::commands {
    using std::string;
    using std::vector;

    using namespace grok::core;

    int help (const bool& command_by_user, const vector<string>& command_arguments) {
        utilities::print("========: help :========");
        utilities::print("");
        utilities::print("# make: > grok make");
        utilities::print("The make command is used to create a new package (project). It will ask you a series of questions that it will use to generate a .grokpackage file, and then a .grokmake file (which is cmake code generated from the package)");
        utilities::print("");
        utilities::print("# use: > grok use <package_name> [<package_release>]");
        utilities::print("The use command will track down a dependency, save it to the .grok folder, and apply it to your project. If a <package_release> was provided, then it will also be applied.");
        utilities::print("");
        utilities::print("# update: > grok update <package_name> [<package_release>]");
        utilities::print("The update command retrieves updates from the repository of the package and saves them to your .grok folder. If a <package_release> was provided, then it will also be applied.");
        utilities::print("");
        utilities::print("# apply: > grok apply");
        utilities::print("The apply command allows you to regenerate your .grokmake file if any changes have occurred that grok is unaware of.");
        utilities::print("");
        utilities::print("# sync: > grok sync");
        utilities::print("The sync command will pull down the registry from grok's source. This updates your local registry to be aware of any potentially new packages that may have been added.");
        return 0;
    }
}