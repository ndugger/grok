# pragma once

# include <experimental/filesystem>
# include <fstream>
# include <sstream>

# include "grok/util/json.cpp"

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::lib {

    class configuration {

        private:
            util::json configuration_options = {
                { "install_directory",
                    {
                        { "linux", "/opt/grok" },
                        { "mac", "/usr/bin/grok" },
                        { "unix", "" },
                        { "windows", "\\Program Files\\grok" }
                    }
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

            # ifdef __linux__
                const std::string configuration_platform = "linux";
            # elif __APPLE__
                const std::string configuration_platform = "mac";
            # elif __unix__
                const std::string configuration_platform = "unix";
            # elif _WIN32
                const std::string configuration_platform = "windows"
            # endif

        public:
            explicit configuration () {
                if (fs::exists(fs::current_path() / ".grokconfig")) {
                    std::ifstream config_stream(fs::current_path() / ".grokconfig");
                    std::stringstream config_string;

                    config_string << config_stream.rdbuf();

                    util::json config_json = util::json::parse(config_string);

                    if (config_json.find("registries") != config_json.end()) {
                        configuration_options[ "registries" ] = config_json[ "registries" ];
                    }
                }
            }

            ~ configuration () {
                option("");
            }

            util::json option (const std::string& name, bool agnostic = true) {
                if (agnostic) {
                    return configuration_options[ name ];
                }
                else {
                    return configuration_options[ name ][ configuration_platform ];
                }
            }
    };
}