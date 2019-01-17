# pragma once

# include <iostream>
# include <functional>
# include <string>

# include "grok/cli/command.cpp"
# include "grok/cmd/apply.cpp"
# include "grok/cmd/create.cpp"
# include "grok/cmd/sync.cpp"
# include "grok/cmd/update.cpp"
# include "grok/cmd/use.cpp"
# include "grok/lib/project.cpp"
# include "grok/util/print.cpp"

# include "reactor/core.cpp"
# include "reactor/event.cpp"

namespace grok::cli {

    void initialize (reactor::core& application) {
        application.observe("command").cast<command*>().for_each([ &application ] (command* cmd) {
            std::string command_name(cmd->name());

            if (command_name == "apply") {
                grok::cmd::apply();
            }

            if (command_name == "create") {
                grok::cmd::create();
            }

            if (command_name == "sync") {
                grok::cmd::sync();
            }

            if (command_name == "update") {
                std::string package_name(!cmd->arguments().empty() ? "" : cmd->arguments().at(0));

                grok::cmd::update(package_name);
            }

            if (command_name == "use") {
                std::string package_name(cmd->arguments().empty() ? "" : cmd->arguments().at(0));
                std::string package_release(cmd->arguments().size() < 2 ? "master" : cmd->arguments().at(1));

                grok::cmd::use(package_name, package_release);
            }

            application.shutdown(0);
        });
    }
}