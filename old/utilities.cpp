# pragma once

# include <iostream>
# include <experimental/filesystem>
# include <fstream>
# include <regex>
# include <sstream>
# include <string>
# include <vector>

# include "stdlib.h"
# include "git2/clone.h"
# include "git2/global.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

# include "grok/configuration/settings.cpp"

namespace grok::core {
    using std::cin;
    using std::cout;
    using std::endl;
    using std::getline;
    using std::ifstream;
    using std::regex;
    using std::regex_replace;
    using std::string;
    using std::stringstream;
    using std::system;
    using std::vector;

    using namespace grok::configuration;
    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;

        fs::path location;
    }

    namespace utilities {

        fs::path get_location () {
            return location;
        }

        void initialize (const string& program_name) {
            location = fs::path(get_setting("install_location", false)) / "grok";

            git_libgit2_init();

            if (fs::exists(fs::temp_directory_path() / ".groktemp")) {
                fs::remove_all(fs::temp_directory_path() / ".groktemp");
            }
        }

        int uninitialize (const int code) {
            git_libgit2_shutdown();

            if (fs::exists(fs::temp_directory_path() / ".groktemp")) {
                fs::remove_all(fs::temp_directory_path() / ".groktemp");
            }

            return code;
        }

        void print (const string& characters) {
            cout << "[ grok ] " << characters << endl;
        }

        string input () {
            cout << "> ";
            string characters;
            getline(cin, characters);
            return characters;
        }

        int unrecognized (const string& command_name) {
            print("unrecognized command: " + command_name);
            print("try 'grok help' for a list of available commands");
            return 1;
        }

        vector<string> split (const string& characters, const char delimiter = ' ') {
            stringstream line(regex_replace(characters, regex("\\s"), ""));
            vector<string> tokens;
            string token;

            while (getline(line, token, delimiter)) {
                tokens.emplace_back(token);
            }

            return tokens;
        }

        void call_script (const string& script_name, const string& arguments = "") {
            system(((location / "scripts" / script_name).string() + " " + arguments).c_str());
        }
    }
}