# pragma once

# include <iostream>
# include <string>
# include <vector>


namespace grok::commands {

    using namespace std;

    int help (string command_from, vector<string> command_arguments, bool triggered_by_user = true) {
        cout << "> grok help" << endl;

        return 0;
    }
}