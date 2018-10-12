# include <functional>
# include <iostream>
# include <map>
# include <string>
# include <vector>

# include "grok/commands/help.cpp"
# include "grok/commands/make.cpp"
# include "grok/commands/sync.cpp"
# include "grok/commands/update.cpp"
# include "grok/commands/use.cpp"
# include "grok/core/unrecognized.cpp"

using namespace std;
using namespace grok::commands;
using namespace grok::core;

using command = function<int(string, vector<string>, bool)>;

map<string, command> commands = {
    { "help", help },
    { "make", make },
    { "sync", sync },
    { "update", update },
    { "use", use }
};

int main (int count, char* arguments[]) {
    string command_from = arguments[ 0 ];
    string command_name = arguments[ 1 ];

    vector<string> command_arguments;

    for (int i = 0; i < count - 2; ++i) {
        command_arguments.emplace_back(arguments[ i + 2 ]);
    }

    if (count < 2) {
        return help(command_from, command_arguments);
    }
    else if (commands[ command_name ] == nullptr) {
        return unrecognized(command_name);
    }
    else {
        return commands[ command_name ](command_from, command_arguments, true);
    }
}
