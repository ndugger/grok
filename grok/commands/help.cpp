# pragma once

# include <iostream>
# include <string>

using namespace std;

namespace grok::commands {

    int help (string arguments[]) {
        cout << "> grok help" << endl;

        return 0;
    }
}