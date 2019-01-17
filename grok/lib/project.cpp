# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <map>
# include <sstream>
# include <string>

# include "common/json.cpp"

# include "fs/current_path.cpp"
# include "fs/file.cpp"

# include "git/clone.cpp"
# include "git/repository.cpp"

# include "grok/lib/configuration.cpp"
# include "grok/lib/generator.cpp"
# include "grok/lib/package.cpp"

namespace grok::lib {

    class project {

        private:
            grok::lib::configuration project_configuration;
            grok::lib::package project_package;

        public:
            explicit project (const common::json& json = nullptr) : project_package(json, true) {
                if (exists()) {
                    std::ifstream package_stream(fs::current_path() / ".grokpackage");
                    std::stringstream package_string;

                    package_string << package_stream.rdbuf();

                    project_package = grok::lib::package(common::json::parse(package_string), true);
                }
            }

            grok::lib::configuration& configuration () {
                return project_configuration;
            }

            grok::lib::package& info () {
                return project_package;
            }

            bool exists () {
                return fs::file(fs::current_path() / ".grokpackage").exists();
            }

            bool uses (const std::string& package_name) {
                std::map<std::string, std::string> dependencies(project_package.json()[ "dependencies" ]);

                return dependencies.find(package_name) != dependencies.end();
            }

            void flag (const std::string& flag, const std::string& value = "") {
                project_package.json().at("flags").emplace(flag, value);
            }

            void include (const std::string& directory) {
                project_package.json().at("includes").emplace_back(directory);
            }

            void release (const std::string& release) {
                project_package.json()[ "package" ][ "release" ] = release;
            }

            void remote (const std::string& address) {
                project_package.json()[ "package" ][ "address" ] = address;
            }

            void rename (const std::string& package_name) {
                project_package.json()[ "package" ][ "name" ] = package_name;
            }

            void require (const std::string& library, const std::string& location = "") {
                project_package.json().at("libraries").emplace(library, location);
            }

            void source (const std::string& directory) {
                project_package.json().at("sources").emplace_back(directory);
            }

            void standard (const std::string& standard) {
                project_package.json()[ "package" ][ "standard" ] = standard;
            }

            void use (lib::package& package) {
                git::repository repository(git::clone(package.address(), fs::current_path() / ".grok" / package.name()));

                if (repository.exists()) {
                    project_package.json()[ "dependencies" ][ package.name() ] = package.release();
                }
            }

            void save () {
                fs::file(fs::current_path() / ".grokpackage").write(project_package.json().dump(4), fs::file::mode::out);
            }
    };
}