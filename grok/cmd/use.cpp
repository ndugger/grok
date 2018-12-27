# pragma once

# include <iostream>
# include <string>

# include "grok/lib/configuration.cpp"
# include "grok/lib/package.cpp"
# include "grok/lib/registry.cpp"
# include "grok/util/json.cpp"

namespace grok::cmd {

    void use (const std::string& package_name) {
        lib::configuration configuration;
        lib::package package(package_name);

        if (package.exists()) {
            std::cout << "package already exists; did you mean \"grok update " << package_name << "\"?" << std::endl;
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

        std::cout << "now using " << package_name << std::endl;
    }
}