# include <functional>
# include <map>
# include <regex>
# include <string>
# include <vector>

# include "grok/commands/help.cpp"
# include "grok/commands/make.cpp"
# include "grok/commands/sync.cpp"
# include "grok/commands/update.cpp"
# include "grok/commands/use.cpp"
# include "grok/core/unrecognized.cpp"

using std::function;
using std::map;
using std::regex;
using std::regex_replace;
using std::string;
using std::vector;

using namespace grok;

namespace grok {

    typedef function<int(string, vector<string>, bool)> command;

    map<string, command> command_map = {
        { "help", commands::help },
        { "make", commands::make },
        { "sync", commands::sync },
        { "update", commands::update },
        { "use", commands::use }
    };
}

int main (int count, char* arguments[ ]) {
    string command_directory = regex_replace(arguments[ 0 ], regex("/grok$"), "");
    bool command_by_user = true;

    if (count <= 1) {
        return commands::help(command_directory, { }, command_by_user);
    }

    string command_name = arguments[ 1 ];

    vector<string> command_arguments;

    for (int i = 0; i < count - 2; ++i) {
        command_arguments.emplace_back(arguments[ i + 2 ]);
    }

    if (command_map[ command_name ] == nullptr) {
        return core::unrecognized(command_name);
    }
    else {
        return command_map[ command_name ](command_directory, command_arguments, command_by_user);
    }
}
