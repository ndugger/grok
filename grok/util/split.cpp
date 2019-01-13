# pragma once

# include <regex>
# include <sstream>
# include <string>
# include <vector>

namespace grok::util {

    std::vector<std::string> split (const std::string& characters, const char delimiter = ' ') {
        std::stringstream line(std::regex_replace(characters, std::regex("\\s"), ""));
        std::vector<std::string> tokens;
        std::string token;

        while (std::getline(line, token, delimiter)) {
            tokens.emplace_back(token);
        }

        return tokens;
    }
}