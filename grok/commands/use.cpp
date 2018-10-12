# pragma once

# include <iostream>
# include <regex>
# include <string>
# include <vector>

# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/core/utilities.cpp"

using namespace std;
using namespace nlohmann;
using namespace grok::core;

namespace grok::commands {

    int use (string command_from, vector<string> command_arguments, bool triggered_by_user = true) {
        initialize();

        if (!project_exists()) {
            display_message("project has not yet been initialized; run 'grok new' to get started");
            uninitialize();

            return 1;
        }

        if (command_arguments.empty()) {
            display_message("use what?");
            uninitialize();

            return 1;
        }

        string package_name = command_arguments.size() > 0 ? command_arguments[ 0 ] : "";
        string package_release = command_arguments.size() > 1 ? command_arguments[ 1 ] : "";

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

                if (triggered_by_user) {
                    add_dependency_to_project(package_name, package_release);
                }

                json dependencies = registered_package[ "dependencies" ];

                if (dependencies != nullptr) {
                    for (json::iterator dependency = dependencies.begin(); dependency != dependencies.end(); ++dependency) {
                        use(command_from, { dependency.key(), dependency.value() }, false);
                    }
                }

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