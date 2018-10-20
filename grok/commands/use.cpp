# pragma once

# include <iostream>
# include <string>
# include <vector>

# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/core/generators/cmake.cpp"
# include "grok/core/project.cpp"
# include "grok/core/package.cpp"
# include "grok/core/registry.cpp"
# include "grok/core/repository.cpp"
# include "grok/core/utilities.cpp"

namespace grok::commands {
    using std::string;
    using std::vector;

    using json = nlohmann::json;
    using namespace grok::core;

    const int use (const bool& command_by_user, const vector<string>& command_arguments) {

        if (!project::exists()) {
            utilities::print("project has not yet been initialized; run 'grok make' to get started");
            return utilities::uninitialize(1);
        }

        if (command_arguments.empty()) {
            utilities::print("use what?");
            return utilities::uninitialize(1);
        }

        string package_name = (command_arguments.size() > 0) ? command_arguments.at(0) : "";
        string package_release = (command_arguments.size() > 1) ? command_arguments.at(1) : "";
        string package_repository;

        json package;
        json package_meta;

        if (registry::contains(package_name)) {
            package = registry::open(package_name);

            if (package.find("package") == package.end()) {
                utilities::print("malformed package file; missing 'package' field");
                return utilities::uninitialize(1);
            }

            package_meta = package.at("package");

            if (package_meta.find("repository") == package_meta.end()) {
                utilities::print("missing package repository");
                return utilities::uninitialize(1);
            }

            package_repository = package_meta.at("repository");

            if (package_release.empty() && package_meta.find("release") != package_meta.end()) {
                package_release = package_meta.at("release");
            }

            if (package_release.empty()) {
                package_release = "master";
            }

            if (package::exists(package_name)) {

                if (project::uses(package_name)) {
                    utilities::print("project is already using " + package_name + "; did you mean 'update'?");
                    return utilities::uninitialize(1);
                }

                utilities::print("dependency conflict: package already exists for " + package_name);
                return utilities::uninitialize(1);
            }

            package::download(package_name, package_repository, package_release);
        }
        else {
            package_repository = package_name;

            if (package_release.empty()) {
                package_release = "master";
            }

            if (!repository::exists(package_repository, package_release)) {
                utilities::print("unable to clone repository @ " + package_repository + " | " + package_release);
                return utilities::uninitialize(1);
            }

            if (!repository::is_package()) {
                utilities::print("repository is missing or has invalid .grokpackage file");
                return utilities::uninitialize(1);
            }

            package = repository::open_package();
            package_meta = package.at("package");
            package_name = package_meta.at("name");

            if (package::exists(package_name)) {

                if (project::uses(package_name)) {
                    utilities::print("project is already using " + package_name + "; did you mean 'update'?");
                    return utilities::uninitialize(1);
                }

                utilities::print("dependency conflict: package already exists for " + package_name);
                return utilities::uninitialize(1);
            }

            repository::save_package(package_name, package_release);
        }

        if (package.find("dependencies") != package.end()) {
            json package_dependencies = package.at("dependencies");

            for (json::iterator dependency = package_dependencies.begin(); dependency != package_dependencies.end(); ++dependency) {

                if (!project::uses(dependency.key())) {
                    use(false, { dependency.key(), dependency.value() });
                }
            }
        }

        if (!command_by_user) {
            return 0;
        }

        project::add(package_name, package_release);
        project::apply(generators::cmake(project::open()));

        utilities::print("now using " + package_name);
        return utilities::uninitialize(0);
    }
}
