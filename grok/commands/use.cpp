# pragma once

# include <iostream>
# include <string>

# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/internal/utilities.cpp"

using namespace std;
using namespace nlohmann;
using namespace grok::internal;

namespace grok::commands {

    int use (string arguments[]) {
        string package_name = arguments[ 0 ];

        if (registry_contains(package_name)) {
            json package_info = package_from_registry(package_name);
            string package_release = package_info[ "package" ][ "release" ];
            string package_repository = package_info[ "package" ][ "repository" ];

            git_repository* repository = clone_package_repository(package_name, package_repository, package_release);
        }
        else {

        }

        return 0;
    }
}