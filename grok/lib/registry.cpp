# pragma once

# include <fstream>
# include <sstream>
# include <string>

# include "grok/lib/package.cpp"
# include "grok/util/file.cpp"
# include "grok/util/json.cpp"

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

                registry_json = grok::util::json::parse(registry_string);
                registry_directory = grok::util::file::path(registry_json[ "directory" ]);
            }

            void synchronize () {

            }

            bool contains (const std::string& package_name) {
                return fs::exists(registry_directory / (package_name + ".grokpackage"));
            }

            grok::lib::package open (const std::string& package_name) {
                std::ifstream package_stream(registry_directory / (package_name + ".grokpackage"));
                std::stringstream package_string;

                package_string << package_stream.rdbuf();

                std::string x(package_string.str());

                grok::util::json package_json(util::json::parse(package_string));

                return grok::lib::package(package_json);
            }
    };
}