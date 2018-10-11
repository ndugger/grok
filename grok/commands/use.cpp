# pragma once

# include <iostream>
# include <regex>
# include <string>
# include <vector>

# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/utilities/utilities.cpp"

using namespace std;
using namespace nlohmann;
using namespace grok::internal;

namespace grok::commands {

    int use (string command_from, vector<string> arguments) {
        initialize();

        if (!project_exists()) {
            display_message("project has not yet been initialized; run 'grok new' to get started");
            uninitialize();

            return 1;
        }

        if (arguments.empty()) {
            display_message("use what?");
            uninitialize();

            return 1;
        }

        string package_name = arguments.size() > 0 ? arguments.at(0) : "";
        string package_release = arguments.size() > 1 ? arguments.at(1) : "";

        if (package_in_use(package_name)) {
            display_message(package_name + " is already being used; did you mean 'grok update " + package_name + "'?");
            uninitialize();

            return 1;
        }

        if (registry_contains(package_name)) {
            json registered_package = open_registered_package(package_name);
            string package_repository = registered_package[ "package" ][ "repository" ];

            if (package_release.empty()) {
                package_release = registered_package[ "package" ][ "release" ];
            }

            if (package_release.empty()) {
                package_release = "master";
            }

            if (package_is_available(package_repository)) {
                download_package(package_name, package_repository, package_release);
                add_dependency_to_project(package_name, package_release);
                display_message("now using " + package_name);
            }
            else {
                display_message("unable to clone repository @ " + package_repository + " | " + package_release);
                uninitialize();

                return 1;
            }
        }
        else {
            string package_repository = package_name;

            if (package_release.empty()) {
                package_release = "master";
            }

            git_repository* discovered_repository = discover_package(package_repository, package_release);

            if (discovered_repository == nullptr) {
                display_message("unable to clone repository @ " + package_repository + " | " + package_release);
                uninitialize();

                return 1;
            }

            json discovered_package = open_discovered_package(discovered_repository);
            string discovered_package_name = discovered_package[ "package" ][ "name" ];

            package_name = discovered_package_name;

            if (package_in_use(package_name)) {
                display_message(package_name + " is already being used; did you mean 'grok update " + package_name + "'?");
                uninitialize();

                return 1;
            }

            save_discovered_package(package_name, discovered_repository);
            add_dependency_to_project(package_name, package_release);
            display_message("now using " + package_name);
        }

        uninitialize();

        return 0;
    }
}