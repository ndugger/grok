# pragma once

# include <iostream>
# include <functional>
# include <string>

# include "grok/cli/command.cpp"
# include "grok/cmd/use.cpp"
# include "grok/util/print.cpp"
# include "reactor/core.cpp"
# include "reactor/event.cpp"

namespace grok::cli {

    void initialize (reactor::core& application) {
        application.observe("command").cast<command*>().for_each([ &application ] (command* cmd) {
            std::string command_name = cmd->name();
            std::string command_target = cmd->target();

            try {
                if (command_name == "use") {
                    cmd::use(command_target);
                }
            }
            catch (int code) {
                application.shutdown(code);
            }

            application.shutdown(0);
        });
    }
}