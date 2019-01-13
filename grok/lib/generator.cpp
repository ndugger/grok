# pragma once

# include <string>

# include "grok/lib/package.cpp"

namespace grok::lib {

    class generator {

        public:
            explicit generator () = default;

            virtual std::string generate (grok::lib::package& package) {
                return "";
            }
    };
}