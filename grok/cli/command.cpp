# pragma once

# include <string>
# include <vector>

# include "reactor/event.cpp"

namespace grok::cli {

    class command : public reactor::event {

        private:
            std::string command_name;
            std::string command_target;


        public:
            explicit command (const std::string& name, const std::string& target) : reactor::event("command") {
                command_name = name;
                command_target = target;
            }

            std::string name () {
                return command_name;
            }

            std::string target () {
                return command_target;
            }
    };
}