# pragma once

# include <iostream>
# include <queue>
# include <string>

using namespace std;

namespace grok::commands {

    int help (queue<string> arguments) {
        cout << "> grok help" << endl;

        return 0;
    }
}