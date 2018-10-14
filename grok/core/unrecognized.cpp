# pragma once

# include <iostream>
# include <string>

# include "grok/core/utilities.cpp"

namespace grok::core {

    using namespace std;
    using namespace grok::core;

    int unrecognized (string command_name) {
        display_message("unrecognized command: " + command_name);
        display_message("try 'grok help' for a list of available commands");

        return 1;
    }
}