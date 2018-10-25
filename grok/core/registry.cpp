# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <sstream>
# include <string>

# include "nlohmann/json.hpp"

# include "grok/core/utilities.cpp"

namespace grok::core {
    using std::ifstream;
    using std::string;
    using std::stringstream;

    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;
    }

    namespace registry {

        bool contains (string package_name) {
            return fs::exists(utilities::get_location() / "registry" / (package_name + ".grokpackage"));
        }

        json open (string package_name) {
            auto package_stream = ifstream(utilities::get_location() / "registry" / (package_name + ".grokpackage"));
            stringstream package_json;

            package_json << package_stream.rdbuf();

            return json::parse(package_json);
        }
    }
}