# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <map>
# include <sstream>
# include <string>

# include "git/clone.cpp"
# include "git/manager.cpp"
# include "git/repository.cpp"

# include "grok/util/json.cpp"

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::lib {

    class package {

        private:
            util::json package_json;

        public:
            explicit package (util::json json) {
                package_json = json;
            }

            util::json operator [ ] (const std::string& key) {
                return package_json[ key ];
            }

            bool valid () {
                return true;
            }

            grok::util::json& json () {
                return package_json;
            }

            std::string name () {
                return grok::util::json(package_json[ "package" ][ "name" ]).get<std::string>();
            }

            std::string release () {
                return package_json[ "package" ][ "release" ];
            }

            std::string address () {
                return package_json[ "package" ][ "repository" ];
            }

            void save () {

            }
    };
}