# include <exception>
# include <iostream>
# include <queue>
# include <string>

# include "grok/objects/command.cpp"

using namespace std;
using namespace grok::objects;

int main (int arg_count, char* arg_vector[]) {
    auto arg_queue = queue<string>();

    if (arg_count < 2) {
        // TODO run help command? show message? allow flags? (-v)
    }
    else try {
        string name = arg_vector[ 1 ];

        for (int i = 2; i < arg_count; ++i) {
            arg_queue.push(arg_vector[ i ]);
        }

        int status = command(name).execute(arg_queue);

        if (status != 0) {
            throw exception();
        }

        return status;
    }
    catch (exception& error) {
        return 1;
    }
}