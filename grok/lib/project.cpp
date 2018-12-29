# pragma once

# include <experimental/filesystem>
# include <string>

# include "grok/lib/package.cpp"

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::lib {

    class project {

        private:
            lib::package project_package;

        public:
            explicit project (const std::string& name = "") : project_package(name) {
                 // read .grokpackage file
            }

            bool exists () {
                return fs::exists(fs::current_path() / ".grokpackage");
            }

            bool uses (lib::package package) {
                return false;
            }
    };
}