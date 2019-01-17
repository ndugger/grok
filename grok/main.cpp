# include <iostream>
# include <string>
# include <vector>

# include "grok/cli/command.cpp"
# include "grok/cli/initialize.cpp"

# include "reactor/core.cpp"
# include "reactor/event.cpp"

namespace grok {

    int main (std::vector<std::string> arguments) {
        const std::string command_name(arguments.size() > 1 ? arguments.at(1) : "");
        const std::vector<std::string> command_arguments(arguments.begin() + 1, arguments.end());
        grok::cli::command command(command_name, command_arguments);


        reactor::core application({ cli::initialize });

        application.start();
        application.emit("command", &command);

        while (application.running());
        return application.status();
    }
}

int main (int arguments_size, char* arguments_values[]) {
    return grok::main(std::vector<std::string>(arguments_values, arguments_values + arguments_size));
}