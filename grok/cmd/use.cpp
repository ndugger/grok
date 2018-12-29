# pragma once

# include <string>

# include "grok/lib/configuration.cpp"
# include "grok/lib/package.cpp"
# include "grok/lib/project.cpp"
# include "grok/lib/registry.cpp"
# include "grok/util/git.cpp"
# include "grok/util/json.cpp"
# include "grok/util/print.cpp"

namespace grok::cmd {

    void use (const std::string& package_name, const std::string package_release = "master") {
        lib::project project;

        if (!project.exists()) {
            util::print("project has not yet been initialized; run 'grok create' to get started");
            return;
        }

        if (package_name.empty()) {
            util::print("use what?");
            return;
        }

        lib::configuration configuration;
        lib::package package(package_name);

        if (package.exists()) {

            if (project.uses(package)) {
                util::print("project already uses package; did you mean 'grok update " + package_name + "'?");
            }
            else {
                util::print("dependency conflict; package already exists for " + package_name);
            }

            return;
        }

        util::git::initialize();

        for (const util::json& directory : configuration.option("registries")) {
            lib::registry registry(directory);

            if (registry.contains(package)) {

                if (!registry.open(package)) {
                    util::print("unable to open package file for " + package_name);
                }

                if (package.valid()) {

                    if (package.download()) {

                        if (package.valid()) {
                            util::print("now using " + package_name);
                        }
                        else {
                            util::print("missing or malformed package file");
                        }
                    }
                    else {
                        util::print("unable to clone repository for " + package_name);
                    }
                }
                else {
                    util::print("missing or malformed package file");
                }

                break;
            }
            else {
                // use name as repo, download, read name from .grokpackage file, save to project
            }
        }

        util::git::uninitialize();
    }
}