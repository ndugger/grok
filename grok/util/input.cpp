# pragma once

# include <iostream>
# include <string>

namespace grok::util {

    std::string input () {
        std::string message;

        std::getline(std::cin, message);

        return message;
    }
}