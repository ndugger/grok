# pragma once

# include <iostream>
# include <experimental/filesystem>
# include <regex>
# include <string>

# include "git2/clone.h"
# include "git2/global.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

namespace grok::core {
    using std::cout;
    using std::endl;
    using std::regex;
    using std::regex_replace;
    using std::string;

    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;

        string location;
    }

    namespace utilities {

        string get_location () {
            return location;
        }

        void initialize (string program_name) {
            location = regex_replace(program_name, regex("grok(?:.exe)?$"), "..");

            git_libgit2_init();

            if (fs::exists(fs::temp_directory_path() / ".groktemp")) {
                fs::remove_all(fs::temp_directory_path() / ".groktemp");
            }
        }

        int uninitialize (int code) {
            git_libgit2_shutdown();

            if (fs::exists(fs::temp_directory_path() / ".groktemp")) {
                fs::remove_all(fs::temp_directory_path() / ".groktemp");
            }

            return code;
        }

        void print (const string& characters) {
            cout << characters << endl;
        }

        int unrecognized (string command_name) {
            print("unrecognized command: " + command_name);
            print("try 'grok help' for a list of available commands");
            return 1;
        }
    }
}