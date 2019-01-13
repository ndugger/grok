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
            bool package_root;

        public:
            explicit package (const common::json& json, bool root = false) {
                package_root = root;

                if (json.is_null()) {
                    package_json = {
                        { "dependencies", std::map<std::string, std::string>() },
                        { "flags", std::map<std::string, std::string>() },
                        { "includes", std::vector<std::string>() },
                        { "libraries", std::map<std::string, std::string>() },
                        { "package", std::map<std::string, std::string>() },
                        { "sources", std::vector<std::string>() }
                    };
                }
                else {
                    package_json = json;
                }
            }

            common::json operator [ ] (const std::string& key) {
                return package_json[ key ];
            }

            bool root () {
                return package_root;
            }

            bool valid () {
                return true; // check for same properties as inserting in constructor above
            }

            common::json& json () {
                return package_json;
            }

            std::map<std::string, std::string> dependencies () {
                return package_json[ "dependencies" ];
            }

            std::map<std::string, std::string> flags () {
                return package_json[ "flags" ];
            }

            std::vector<std::string> includes () {
                return package_json[ "includes" ];
            }

            std::map<std::string, std::string> libraries () {
                return package_json[ "libraries" ];
            }

            std::vector<std::string> sources () {
                return package_json[ "sources" ];
            }

            std::string address () {
                return package_json[ "package" ][ "address" ];
            }

            std::string name () {
                return package_json[ "package" ][ "name" ];
            }

            std::string release () {
                return package_json[ "package" ][ "release" ];
            }

            std::string standard () {
                return package_json[ "package" ][ "standard" ];
            }
    };
}