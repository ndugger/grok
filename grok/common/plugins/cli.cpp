# pragma once

# include <iostream>
# include <string>
# include <vector>

# include "fusion/instruction.cpp"
# include "fusion/program.cpp"

namespace common::plugins {

    class cli_command {

        private:
            std::vector<std::string> cli_command_arguments;

        public:
            explicit cli_command (std::vector<std::string> arguments = { }) {
                cli_command_arguments = arguments;
            }

            std::vector<std::string>& arguments () {
                return cli_command_arguments;
            }
    };

    void cli (program<cli_command>& program) {
        program.subscribe("cli").observe("initialize").for_each([ & ] (fusion::instruction<cli_command> instruction) {
            for (const std::string& argument : instruction.details().arguments()) {
                std::cout << argument << std::endl;
            }
        });
    }
}