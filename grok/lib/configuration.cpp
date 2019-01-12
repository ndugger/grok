# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <sstream>

# include "fs/current_path.cpp"
# include "fs/file.cpp"
# include "fs/path.cpp"

# include "common/json.cpp"

namespace grok::lib {

    class configuration {

        private:
            common::json configuration_options = {
                { "install_directory",
                    # ifdef __linux__
                        "/opt/grok"
                    # elif __APPLE__
                        "/usr/bin/grok"
                    # elif __unix__
                        ""
                    # elif _WIN32
                        "\\Program Files\\grok"
                    # endif
                },
                { "registries",
                    {
                        # ifdef __linux__
                            "/opt/grok"
                        # elif __APPLE__
                            "/usr/bin/grok"
                        # elif __unix__
                            ""
                        # elif _WIN32
                            "\\Program Files\\grok"
                        # endif
                    }
                }
            };

        public:
            explicit configuration () {
                if (fs::file(fs::current_path() / ".grokconfig").exists()) {
                    std::ifstream config_stream(fs::current_path() / ".grokconfig");
                    std::stringstream config_string;

                    config_string << config_stream.rdbuf();

                    common::json config_json = common::json::parse(config_string);

                    if (config_json.find("registries") != config_json.end()) {
                        configuration_options[ "registries" ] = config_json[ "registries" ];
                    }
                }
            }

            common::json option (const std::string& name) {
                return configuration_options[ name ];
            }
    };
}