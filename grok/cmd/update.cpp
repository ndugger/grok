# pragma once

# include <string>

# include "common/json.cpp"

# include "fs/current_path.cpp"
# include "fs/file.cpp"
# include "fs/path.cpp"

# include "git/manager.cpp"
# include "git/open.cpp"
# include "git/remote.cpp"
# include "git/repository.cpp"

# include "grok/lib/project.cpp"
# include "grok/lib/registry.cpp"
# include "grok/util/print.cpp"

namespace grok::cmd {

    void update (const std::string& package_name) {
        grok::lib::project project;

        if (!project.exists()) {
            grok::util::print("project has not yet been initialized; run 'grok create' to get started");
            return;
        }

        if (!project.uses(package_name)) {
            grok::util::print("project doesn't use package; did you mean 'grok use " + package_name + "'?");
            return;
        }

        if (!fs::file(fs::current_path() / ".grok" / package_name).exists()) {
            grok::util::print("package is missing; please run 'grok use' to download dependencies");
            return;
        }

        if (package_name.empty()) {
            for (std::pair<std::string, std::string> entry : project.package().dependencies()) {
                if (!fs::file(fs::current_path() / ".grok" / entry.first).exists()) {
                    update(entry.first);
                }
            }

            return;
        }

        git::manager::init();

        for (const common::json& directory : project.configuration().option("registries")) {
            grok::lib::registry registry(directory);

            if (registry.contains(package_name)) {
                grok::lib::package package(registry.open(package_name));
                git::repository repository(git::open(fs::current_path() / ".grok" / package_name));

                if (repository.exists()) {
                    git::branch local(repository.branch(package.release()));
                    git::remote origin(repository.remote("origin"));

                    origin.fetch();

                    git::branch release(origin.branch(package.release()));

                    repository.merge(local, release);

                    grok::util::print("updated package '" + package.name() + "'");
                }
                else {
                    grok::util::print("missing repository");
                }

                break;
            }
        }

        git::manager::clean_up();
    }
}