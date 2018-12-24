# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <sstream>
# include <string>

# include "git2/clone.h"
# include "git2/global.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

namespace grok::core {
    using std::ifstream;
    using std::string;
    using std::stringstream;

    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;
    }

    namespace repository {

        bool exists (string package_repository, string package_release) {
            git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
            clone_opts.checkout_branch = package_release.c_str();

            git_repository* repository = nullptr;

            git_clone(&repository, package_repository.c_str(), (fs::temp_directory_path() / ".groktemp").c_str(), &clone_opts);

            return (repository != nullptr);
        }

        bool is_package () {
            return fs::exists(fs::temp_directory_path() / ".groktemp" / ".grokpackage");
        }

        json open_package () {
            auto package_stream = ifstream(fs::temp_directory_path() / ".groktemp" / ".grokpackage");
            stringstream package_json;

            package_json << package_stream.rdbuf();

            return json::parse(package_json);
        }

        void save_package (string package_name, string package_release) {
            fs::rename(
                fs::temp_directory_path() / ".groktemp",
                fs::current_path() / ".grok" / package_name
            );
        }
    }
}