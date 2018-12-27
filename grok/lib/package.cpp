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
            std::string package_name;
            std::string package_release;
            std::string package_repository;

        public:
            explicit package (const std::string& name = ".", const std::string& release = "master") {
                package_name = name;
                package_release = release;

                if (name == ".") {
                    std::ifstream package_stream(fs::current_path() / ".grokpackage");
                    std::stringstream package_string;

                    package_string << package_stream.rdbuf();

                    load(util::json::parse(package_string));
                }
            }

            bool exists () {

                if (package_name == ".") {
                    return fs::exists(fs::current_path() / ".grokpackage");
                }

                return fs::exists(fs::current_path() / ".grok" / package_name);
            }

            std::string name () {
                return package_name;
            }

            std::string release () {
                return package_release;
            }

            void load (util::json json) {
                package_release = json[ "package" ][ "release" ];
                package_repository = json[ "package" ][ "repository" ];
            }

            void download () {
                util::repository repository(package_repository);

                repository.clone(fs::current_path() / ".grok" / package_name);
                repository.checkout(package_release);
            }
    };
}