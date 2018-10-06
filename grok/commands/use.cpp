# pragma once

# include <iostream>
# include <string>

# include "git2/clone.h"

using namespace std;

namespace grok::commands {

    int use (string arguments[]) {
        string repository = arguments[ 0 ];

        cout << repository << endl;

        return 0;
    }
}