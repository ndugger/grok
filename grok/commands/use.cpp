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

    int use (string command_directory, vector<string> command_arguments, bool command_by_user = false) {
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

        if (registry_contains(command_directory, package_name)) {
            json registered_package = open_registered_package(command_directory, package_name);
            string package_repository = registered_package[ "package" ][ "repository" ];

            if (package_release.empty()) {
                package_release = registered_package[ "package" ][ "release" ];
            }

            if (package_release.empty()) {
                package_release = "master";
            }

            if (package_is_available(package_repository)) {
                download_package(package_name, package_repository, package_release);

                if (command_by_user) {
                    add_dependency_to_project(package_name, package_release);
                }

                json dependencies = registered_package[ "dependencies" ];
                json includes = registered_package[ "includes" ];
                json libraries = registered_package[ "libraries" ];

                if (dependencies != nullptr) {
                    for (json::iterator dependency = dependencies.begin(); dependency != dependencies.end(); ++dependency) {
                        use(command_directory, { dependency.key(), dependency.value() }, false);
                    }
                }

                if (includes != nullptr) {
                    for (json::iterator include = includes.begin(); include != includes.end(); ++include) {
                        add_include_to_project(package_name, package_release, include.value());
                    }
                }

                if (libraries != nullptr) {
                    for (json::iterator library = libraries.begin(); library != libraries.end(); ++library) {
                        add_library_to_project(package_name, package_release, library.key(), library.value());
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

            if (command_by_user) {
                add_dependency_to_project(package_name, package_release);
            }

            json dependencies = discovered_package[ "dependencies" ];
            json includes = discovered_package[ "includes" ];
            json libraries = discovered_package[ "libraries" ];

            if (dependencies != nullptr) {
                for (json::iterator dependency = dependencies.begin(); dependency != dependencies.end(); ++dependency) {
                    use(command_directory, { dependency.key(), dependency.value() }, false);
                }
            }

            if (includes != nullptr) {
                for (json::iterator include = includes.begin(); include != includes.end(); ++include) {
                    add_include_to_project(package_name, package_release, include.value());
                }
            }

            if (libraries != nullptr) {
                for (json::iterator library = libraries.begin(); library != libraries.end(); ++library) {
                    add_library_to_project(package_name, package_release, library.key(), library.value());
                }
            }

            display_message("now using " + package_name);
        }

        uninitialize();

        return 0;
    }
}