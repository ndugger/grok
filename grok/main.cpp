# include <functional>
# include <map>
# include <regex>
# include <string>
# include <vector>

# include "grok/commands/apply.cpp"
# include "grok/commands/help.cpp"
# include "grok/commands/make.cpp"
# include "grok/commands/sync.cpp"
# include "grok/commands/update.cpp"
# include "grok/commands/use.cpp"
# include "grok/core/unrecognized.cpp"

namespace grok {
    using std::function;
    using std::map;
    using std::regex;
    using std::regex_replace;
    using std::string;
    using std::vector;

    typedef function<int(string, vector<string>, bool)> command;

    map<string, command> command_map = {
        { "apply", commands::apply },
        { "help", commands::help },
        { "make", commands::make },
        { "sync", commands::sync },
        { "update", commands::update },
        { "use", commands::use }
    };

    int execute (vector<string> arguments) {
        string command_binary_location = regex_replace(arguments[ 0 ], regex("/grok$"), "");
        bool command_by_user = true;

        if (arguments.size() <= 1) {
            return commands::help(command_binary_location, { }, command_by_user);
        }

        string command_name = arguments[ 1 ];

        vector<string> command_arguments;

        for (int i = 0; i < arguments.size() - 2; ++i) {
            command_arguments.emplace_back(arguments[ i + 2 ]);
        }

        if (command_map[ command_name ] == nullptr) {
            return core::unrecognized(command_name);
        }
        else {
            return command_map[ command_name ](command_binary_location, command_arguments, command_by_user);
        }
    }
}

int main (int arg_c, char* arg_v[ ]) {
    return grok::execute(std::vector<std::string>(arg_v, arg_v + arg_c));
}
