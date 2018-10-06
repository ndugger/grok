# include <exception>
# include <functional>
# include <iostream>
# include <map>
# include <queue>
# include <string>

# include "grok/commands/help.cpp"
# include "grok/commands/make.cpp"
# include "grok/commands/sync.cpp"
# include "grok/commands/update.cpp"
# include "grok/commands/use.cpp"

using namespace std;
using namespace grok::commands;

const map<string, function<int(queue<string>)>> commands = {
    { "help", help },
    { "make", make },
    { "sync", sync },
    { "update", update },
    { "use", use }
};

queue<string> arguments;

int main (int arg_count, char* arg_vector[]) {

    if (arg_count < 2) {
        return help(arguments);
    }
    else {
        string name = arg_vector[ 1 ];

        for (int i = 2; i < arg_count; ++i) {
            arguments.push(arg_vector[ i ]);
        }

        return commands.at(name)(arguments);
    }
}