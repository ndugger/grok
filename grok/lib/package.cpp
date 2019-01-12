# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <map>
# include <sstream>
# include <string>

# include "common/json.cpp"

# include "git/clone.cpp"
# include "git/manager.cpp"
# include "git/repository.cpp"

namespace grok::lib {

    class package {

        private:
            common::json package_json;

        public:
            explicit package (common::json json) {
                package_json = json;
            }

            common::json operator [ ] (const std::string& key) {
                return package_json[ key ];
            }

            bool valid () {
                return true;
            }

            common::json& json () {
                return package_json;
            }

            std::string name () {
                return common::json(package_json[ "package" ][ "name" ]).get<std::string>();
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