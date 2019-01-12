# pragma once

# include <fstream>
# include <sstream>
# include <string>

# include "fs/file.cpp"
# include "fs/path.cpp"

# include "grok/lib/package.cpp"
# include "grok/util/json.cpp"

namespace grok::lib {

    class registry {

        private:
            fs::path registry_directory;
            util::json registry_json;

        public:
            explicit registry (fs::path directory) {
                std::ifstream registry_stream(directory / ".grokregistry");
                std::stringstream registry_string;

                registry_string << registry_stream.rdbuf();

                registry_json = grok::util::json::parse(registry_string);
                registry_directory = fs::path(registry_json[ "directory" ]);
            }

            void synchronize () {

            }

            bool contains (const std::string& package_name) {
                return fs::file(registry_directory / (package_name + ".grokpackage")).exists();
            }

            grok::lib::package open (const std::string& package_name) {
                std::ifstream package_stream(registry_directory / (package_name + ".grokpackage"));
                std::stringstream package_string;

                package_string << package_stream.rdbuf();

                grok::util::json package_json(util::json::parse(package_string));

                return grok::lib::package(package_json);
            }
    };
}