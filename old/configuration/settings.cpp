# pragma once

# include <string>

# include "nlohmann/json.hpp"

namespace grok::configuration {
    using std::string;
    using json = nlohmann::json;

    namespace {
        # ifdef __linux__
            const string platform = "linux";
        # elif __APPLE__
            const string platform = "mac";
        # elif __unix__
            const string platform = "unix";
        # elif _WIN32
            const string platform = "windows"
        # endif
    }

    json settings = {
        { "install_location",
            {
                { "linux", "/opt/" },
                { "mac", "/usr/bin/" },
                { "unix", "" },
                { "windows", "\\Program Files\\" }
            }
        }
    };

    string get_setting (string setting_name, bool agnostic = true) {
        if (agnostic) {
            return settings[ setting_name ];
        }
        else {
            return settings[ setting_name ][ platform ];
        }
    }
}