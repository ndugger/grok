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

            if (package_exists(package_name)) {
                cout << package_name << " is already being used; did you mean 'grok update " << package_name << "'?" << endl;
            }
            else {
                git_repository* repository = clone_package_repository(package_name, package_repository, package_release);

                if (repository == nullptr) {
                    cout << "unable to clone repository @ " << package_repository << " | " << package_release << endl;
                }
                else {
                    cout << "successfully downloaded " << package_name << endl;
                    cout << "add '.grok/" << package_name << "/.' to your include directories" << endl;
                }
            }
        }
        else {

        }

        return 0;
    }
}