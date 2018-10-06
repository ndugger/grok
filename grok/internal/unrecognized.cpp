# pragma once

# include <iostream>
# include <string>

using namespace std;

namespace grok::internal {

    string application_name = "grok";

    int unrecognized (string command_name) {
        cout << application_name << " unrecognized command: " << command_name << endl;
        cout << "try '" << application_name << " help' for a list of available commands" << endl;

        return 1;
    }
}