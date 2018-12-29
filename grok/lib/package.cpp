# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <sstream>
# include <string>

# include "grok/util/json.cpp"
# include "grok/util/repository.cpp"

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::lib {

    class package {

        private:
            util::json package_json;
            std::string package_name;
            std::string package_release;

        public:
            explicit package (const std::string& name, const std::string& release = "master") {
                package_name = name;
                package_release = release;
            }

            ~ package () {
                save();
            }

            util::json operator [ ] (const std::string& key) {
                return package_json[ key ];
            }

            bool exists () {
                return fs::exists(fs::current_path() / ".grok" / package_name);
            }

            std::string name () {
                return package_name;
            }

            std::string release () {
                return package_release;
            }

            bool valid () {
                return true;
            }

            void overwrite (util::json json) {
                package_json = json;
            }

            bool save () {
                return true;
            }

            bool download () {
                util::repository repository(package_json[ "package" ][ "repository" ]);

                repository.clone(fs::current_path() / ".grok" / package_name);
                repository.checkout(package_release);

                return true;
            }
    };
}