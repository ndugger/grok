# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <sstream>
# include <string>

# include "grok/lib/package.cpp"
# include "grok/util/json.cpp"

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::lib {

    class registry {

        private:
            fs::path registry_directory;
            util::json registry_json;

        public:
            explicit registry (const fs::path& directory) {
                std::ifstream registry_stream(directory / ".grokregistry");
                std::stringstream registry_string;

                registry_string << registry_stream.rdbuf();

                registry_json = util::json::parse(registry_string);
                registry_directory = fs::path(registry_json[ "directory" ]);
            }

            void synchronize () {

            }

            bool contains (lib::package& package) {
                return fs::exists(registry_directory / (package.name() + ".grokpackage"));
            }

            bool open (lib::package& package) {
                std::ifstream package_stream(registry_directory / (package.name() + ".grokpackage"));
                std::stringstream package_string;

                package_string << package_stream.rdbuf();

                util::json package_json(util::json::parse(package_string));

                package.overwrite(package_json);

                return true;
            }
    };
}