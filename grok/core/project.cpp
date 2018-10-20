# pragma once

# include <experimental/filesystem>

# include <fstream>
# include <sstream>
# include <string>

# include "nlohmann/json.hpp"

namespace grok::core {
    using std::ifstream;
    using std::ofstream;
    using std::string;
    using std::stringstream;

    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;
    }

    namespace project {

        bool exists () {
            return fs::exists(fs::current_path() / ".grokpackage");
        }

        json open () {
            auto project_stream = ifstream(fs::current_path() / ".grokpackage");
            stringstream project_json;

            project_json << project_stream.rdbuf();

            return json::parse(project_json);
        }

        bool uses (string package_name) {
            return false; // TODO
        }

        void add (string package_name, string package_release) {
            auto package_stream = ifstream(fs::current_path() / ".grokpackage");
            stringstream package_json;

            package_json << package_stream.rdbuf();

            json package = json::parse(package_json);

            if (package[ "dependencies" ] == nullptr) {
                package[ "dependencies" ] = { { package_name, package_release } };
            }
            else {
                package[ "dependencies" ][ package_name ] = package_release;
            }

            auto file_stream = ofstream(fs::current_path() / ".grokpackage");

            file_stream << package.dump(4);
        }
    }
}