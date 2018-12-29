# pragma once

# include <iostream>
# include <string>

namespace grok::util {

    void print (const std::string& message) {
        std::cout << message << std::endl;
    }
}