# pragma once

# include <string>

# include "grok/lib/configuration.cpp"
# include "grok/lib/package.cpp"
# include "grok/lib/registry.cpp"
# include "grok/util/json.cpp"
# include "grok/util/print.cpp"

namespace grok::cmd {

    void use (const std::string& package_name) {

        if (!lib::package().exists()) {
            util::print("project has not yet been initialized; run \"grok create\" to get started");
            return;
        }

        if (package_name.empty()) {
            util::print("use what?");
            return;
        }

        lib::configuration configuration;
        lib::package package(package_name);

        if (package.exists()) {
            util::print("package already exists; did you mean \"grok update " + package_name + "\"?");
            return;
        }

        for (std::size_t i(configuration.option("registries").size() - 1); i >= 0; --i) {
            lib::registry registry(configuration.option("registries")[ i ]);

            if (registry.contains(package)) {
                util::json package_json(registry.open(package));

                package.load(package_json);
                package.download();

                break;
            }
        }

        util::print("now using " + package_name);
    }
}