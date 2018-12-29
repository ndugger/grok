# pragma once

# include <iostream>
# include <string>

namespace grok::util {

    void print (const std::string& message, bool decorative = true) {
        std::cout << (decorative ? "grok | " : "") << message << std::endl;
    }
}