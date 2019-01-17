# pragma once

# include <string>
# include <vector>

# include "reactor/event.cpp"

namespace grok::cli {

    class command : public reactor::event {

        private:
            std::vector<std::string> command_arguments;
            std::string command_name;

        public:
            explicit command (const std::string& name, std::vector<std::string> arguments) : reactor::event("command") {
                command_arguments = arguments;
                command_name = name;
            }

            std::vector<std::string> arguments () {
                return command_arguments;
            }

            std::string name () {
                return command_name;
            }
    };
}