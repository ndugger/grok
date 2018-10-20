# pragma once

# include <iostream>
# include <string>
# include <vector>


namespace grok::commands {

    using namespace std;

    int help (const bool& command_by_user, const vector<string>& command_arguments) {
        cout << "> grok help" << endl;

        return 0;
    }
}