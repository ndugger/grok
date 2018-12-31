# pragma once

# include <string>

# include "grok/lib/configuration.cpp"
# include "grok/lib/package.cpp"
# include "grok/lib/project.cpp"
# include "grok/lib/registry.cpp"
# include "grok/util/file.cpp"
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
                            package.remove();
                            util::print("missing or malformed package file; please contact the repository's owner");
                        }
                    }
                    else {
                        util::print("unable to clone repository for " + package_name);
                    }
                }
                else {
                    util::print("missing or malformed package file; please contact the repository's owner");
                }

                break;
            }
            else {
                util::git::repository repository(package_name);
                repository.clone(util::file::current_path() / ".grok" / ".temp");

                util::file file(util::file::current_path() / ".grok" / ".temp" / ".grokpackage");

                if (file.open()) {
                    package.overwrite(util::json::parse(file.contents()));

                    if (package.valid()) {

                        if (package.exists()) {

                            if (project.uses(package)) {
                                util::print("project already uses package; did you mean 'grok update " + package_name + "'?");
                            }
                            else {
                                util::print("dependency conflict; package already exists for " + package_name);
                            }
                        }
                        else {
                            util::file::rename(
                                 util::file::current_path() / ".grok" / ".temp",
                                 util::file::current_path() / ".grok" / package.json()[ "package "][ "name" ]
                            );
                            util::print("now using " + std::string(package.json()[ "package "][ "name" ]));
                        }
                    }
                    else {
                        package.remove();
                        util::print("missing or malformed package file; please contact the repository's owner");
                    }
                }
                else {
                    util::print("missing or malformed package file; please contact the repository's owner");
                }
            }
        }

        util::git::uninitialize();
    }
}