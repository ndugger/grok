# pragma once

# include <experimental/filesystem>
# include <sstream>
# include <string>

# include "git2/clone.h"
# include "git2/global.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/core/utilities.cpp"

namespace grok::core {
    using std::string;
    using std::stringstream;

    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;
    }

    namespace package {

        void download (string package_name, string package_repository, string package_release) {
            git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
            clone_opts.checkout_branch = package_release.c_str();

            git_repository* repository = nullptr;

            git_clone(&repository, package_repository.c_str(), (fs::current_path() / ".grok" / package_name).c_str(), &clone_opts);
        }
    }
}