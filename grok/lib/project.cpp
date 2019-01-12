# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <map>
# include <sstream>
# include <string>

# include "fs/current_path.cpp"
# include "fs/file.cpp"

# include "git/clone.cpp"

# include "grok/lib/configuration.cpp"
# include "grok/lib/package.cpp"
# include "grok/util/json.cpp"

namespace grok::lib {

    class project {

        private:
            grok::lib::configuration project_configuration;
            grok::lib::package project_package;

        public:
            explicit project (grok::util::json json = nullptr) : project_package(json) {
                if (project_package.json().is_null()) {
                    std::ifstream package_stream(fs::current_path() / ".grokpackage");
                    std::stringstream package_string;

                    package_string << package_stream.rdbuf();
                    project_package = grok::lib::package(util::json::parse(package_string));
                }
            }

            grok::lib::configuration& configuration () {
                return project_configuration;
            }

            bool exists () {
                return fs::file(fs::current_path() / ".grokpackage").exists();
            }

            bool uses (const std::string& package_name) {
                std::map<std::string, std::string> dependencies(project_package.json()[ "dependencies" ]);
                return dependencies.find(package_name) != dependencies.end();
            }

            void use (lib::package& package) {
                git::clone(package.address(), fs::current_path() / ".grok" / package.name());
                project_package.json()[ "dependencies" ][ package.name() ] = package.release();
            }

            void save () {
                fs::file(fs::current_path() / ".grokpackage").write(project_package.json().dump(4), fs::file::mode::out);
            }
    };
}