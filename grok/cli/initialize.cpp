# pragma once

# include <functional>
# include <string>

# include "reactor/core.cpp"
# include "reactor/event.cpp"

# include "grok/cli/command.cpp"

namespace grok::cli {

    std::function<void(reactor::core&)> initialize (const std::string& command_name, const std::string& package_name) {
        return [ command_name, package_name ] (reactor::core& application) {
            application.observe("start").for_each([ & ] (reactor::event* event) {

            });
        };
    }
}