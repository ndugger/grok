# pragma once

# include <functional>
# include <map>
# include <queue>
# include <string>
# include <utility>

# include "grok/commands/help.cpp"

using namespace std;

namespace grok::objects {

    namespace {
        map<string, function<int(queue<string>)>> functions = {
//            { "help", move(grok::commands::help_command) }
        };
    }

    class command {

        private: string name;

        public: explicit command (string name) {
            this->name = name;
        }

        public: int execute (queue<string> arguments) {
            if (functions.find(name) != functions.end()) {
//                return functions.at(name)(move(arguments));
            }

            return 1;
        }
    };
}