# include <iostream>
# include <string>
# include <vector>

# include "grok/cli/command.cpp"
# include "grok/cli/initialize.cpp"
# include "reactor/core.cpp"

namespace grok {

    int main (std::vector<std::string> arguments) {
        const std::string command_name = arguments.size() > 1 ? arguments.at(1) : "";
        const std::string command_target = arguments.size() > 2 ? arguments.at(2) : "";

        reactor::core application({ cli::initialize });

        application.start();
        application.emit("command", new cli::command(command_name, command_target));

        while (application.running());
        return application.status();
    }
}

int main (int arguments_size, char* arguments_values[]) {
    return grok::main(std::vector<std::string>(arguments_values, arguments_values + arguments_size));
}