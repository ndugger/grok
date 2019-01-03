# pragma once

# include <string>

# include "git/manager.cpp"
# include "git/repository.cpp"

# include "grok/lib/configuration.cpp"
# include "grok/lib/package.cpp"
# include "grok/lib/project.cpp"
# include "grok/lib/registry.cpp"
# include "grok/util/file.cpp"
# include "grok/util/json.cpp"
# include "grok/util/print.cpp"

namespace grok::cmd {

    void use (const std::string& package_name, const std::string package_release = "master") {
        grok::lib::configuration configuration;
        grok::lib::project project;

        if (!project.exists()) {
            grok::util::print("project has not yet been initialized; run 'grok create' to get started");
            return;
        }

        if (package_name.empty()) {
            grok::util::print("use what?");
            return;
        }

        if (project.uses(package_name)) {
            grok::util::print("project already uses package; did you mean 'grok update " + package_name + "'?");
            return;
        }

        git::manager::init();

        for (const util::json& directory : configuration.option("registries")) {
            grok::lib::registry registry(directory);

            if (registry.contains(package_name)) {
                grok::lib::package package(registry.open(package_name));

                project.use(package);

                if (project.uses(package.name())) {
                    grok::util::print("now using " + package.name());
                }
                else {
                    grok::util::print("unable to use '" + package_name + "' at this time");
                }

                break;
            }
            else {
                git::repository repository(git::clone(package_name, grok::util::file::current_path() / ".grok" / ".temp"));
                grok::util::file file(grok::util::file::current_path() / ".grok" / ".temp" / ".grokpackage");
                grok::lib::package package(grok::util::json::parse(file.contents()));

                if (package.valid()) {

                    if (project.uses(package.name())) {
                        grok::util::print("project already uses package; did you mean 'grok update " + package.name() + "'?");
                    }
                    else {
                        grok::util::file::rename(
                            util::file::current_path() / ".grok" / ".temp",
                            util::file::current_path() / ".grok" / package.name()
                        );

                        project.use(package);

                        grok::util::print("now using " + package.name());
                    }
                }
                else {
                    grok::util::file::remove(grok::util::file::current_path() / ".grok" / ".temp");
                    grok::util::print("missing or malformed package file; please contact the repository's owner");
                }
            }
        }

        git::manager::clean_up();
    }
}