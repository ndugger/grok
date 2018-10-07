# pragma once

# include <iostream>
# include <fstream>
# include <string>

# include <experimental/filesystem>

# include "git2/clone.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

using namespace std;
using namespace std::experimental;
using namespace nlohmann;

namespace grok::internal {

    bool registry_contains (string package_name) {
        return filesystem::exists(filesystem::path("../registry/" + package_name + ".grokinfo"));
    }

    json package_from_registry (string package_name) {
        auto package_stream = ifstream("../registry/" + package_name + ".grokinfo");
        stringstream package_json;

        package_json << package_stream.rdbuf();

        return json::parse(package_json);
    }

    bool package_exists (string package_name) {
        return filesystem::exists(filesystem::path("../.grok/" + package_name));
    }

    git_repository* clone_package_repository (string package_name, string package_repository, string package_release) {
        git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
        clone_opts.checkout_branch = package_release.c_str();

        git_repository* repository = nullptr;

        git_clone(&repository, package_repository.c_str(), ("../.grok/" + package_name).c_str(), &clone_opts);

        return repository;
    }
}