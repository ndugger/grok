# include <functional>
# include <iostream>
# include <map>
# include <string>

# include "grok/commands/help.cpp"
# include "grok/commands/make.cpp"
# include "grok/commands/sync.cpp"
# include "grok/commands/update.cpp"
# include "grok/commands/use.cpp"

# include "grok/internal/unrecognized.cpp"

using namespace std;
using namespace grok::commands;
using namespace grok::internal;

map<string, function<int(string[])>> commands = {
    { "help", help },
    { "make", make },
    { "sync", sync },
    { "update", update },
    { "use", use }
};

int main (int size, char* arguments[]) {
    string command_name = arguments[ 1 ];
    string command_arguments[ size - 2 ];

    for (int i = 0; i < size - 2; ++i) {
        command_arguments[ i ] = arguments[ i + 2 ];
    }

    if (size < 2) {
        return help(command_arguments);
    }
    else if (commands[ command_name ] == nullptr) {
        return unrecognized(command_name);
    }
    else {
        return commands[ command_name ](command_arguments);
    }
}
