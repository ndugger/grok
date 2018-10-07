# pragma once

# include <iostream>
# include <string>

using namespace std;

namespace grok::internal {

    int unrecognized (string command_name) {
        cout << "unrecognized command: " << command_name << endl;
        cout << "try 'grok help' for a list of available commands" << endl;

        return 1;
    }
}