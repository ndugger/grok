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
    using std::size_t;
    using std::string;
    using std::vector;

    typedef function<int(string, vector<string>, bool)> command;

    const map<string, command> command_map = {
        { "apply", commands::apply },
        { "help", commands::help },
        { "make", commands::make },
        { "sync", commands::sync },
        { "update", commands::update },
        { "use", commands::use }
    };

    int execute (const vector<string> arguments) {
        const string command_origin = regex_replace(arguments.at(0), regex("/grok$"), "");
        const bool command_by_user = true;

        if (arguments.size() <= 1) {
            return commands::help(command_origin, { }, command_by_user);
        }

        const string command_name = arguments.at(1);

        if (command_map.find(command_name) == command_map.end()) {
            return core::unrecognized(command_name);
        }

        vector<string> command_arguments;

        for (size_t i = 0; i < arguments.size() - 2; ++i) {
            command_arguments.emplace_back(arguments.at(i + 2));
        }

        return command_map.at(command_name)(command_origin, command_arguments, command_by_user);
    }
}

int main (int arg_c, char* arg_v[ ]) {
    return grok::execute(std::vector<std::string>(arg_v, arg_v + arg_c));
}
