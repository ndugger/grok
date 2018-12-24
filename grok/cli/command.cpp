# pragma once

# include <string>
# include <vector>

# include "reactor/event.cpp"

namespace grok::cli {

    class command : public reactor::event {

        private:
            std::vector<std::string> command_arguments;

        public:
            explicit command (std::vector<std::string>& arguments) : reactor::event("command") {
                command_arguments = arguments;
            }

            std::vector<std::string> arguments () {
                return command_arguments;
            }
    };
}