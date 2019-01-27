# pragma once

# include <iostream>
# include <utility>

# include "grok/lib/generator.cpp"
# include "grok/lib/package.cpp"
# include "grok/lib/project.cpp"
# include "grok/lib/registry.cpp"

namespace grok::gen {

    class cmake : public grok::lib::generator {

        public:
            cmake () = default;

            std::string generate (grok::lib::package& package) override {
                std::stringstream generated;

                if (package.root()) {
                    generated << "# ========: cmake :========" << std::endl;
                    generated << "set(CMAKE_CXX_STANDARD " + package.standard() + ")" << std::endl;

                    generated << std::endl;
                    generated << "project(" + package.name() + ")" << std::endl;
                    generated << std::endl;
                }

                generated << "# ========: " + package.name() + " :========" << std::endl;

                if (!package.flags().empty()) {
                    generated << "# - flags:" << std::endl;

                    for (std::pair<std::string, std::string> pair : package.flags()) {
                        generated << "set(CMAKE_CXX_FLAGS -" + pair.first + (pair.second.empty() ? "" : "=" + pair.second) + ")" << std::endl;
                    }
                }

                if (!package.sources().empty()) {
                    generated << "# - executables:" << std::endl;

                    for (const std::string& path : package.sources()) {
                        generated << "add_executable(" + package.name() + " " + path + ")" << std::endl;
                    }
                }

                if (!package.includes().empty()) {
                    generated << "# - includes:" << std::endl;

                    for (const std::string& path : package.includes()) {
                        if (package.root()) {
                            generated << "include_directories(\"${CMAKE_CURRENT_SOURCE_DIR}/" + path + "\")" << std::endl;
                        }
                        else {
                            generated << "include_directories(\"${CMAKE_CURRENT_SOURCE_DIR}/.grok/" + package.name() + "/" + path + "\")" << std::endl;
                        }
                    }
                }

                if (!package.libraries().empty()) {
                    generated << "# - libraries:" << std::endl;

                    std::vector<std::string> target_link_libraries;

                    for (std::pair<std::string, std::string> pair : package.libraries()) {
                        target_link_libraries.emplace_back(pair.first);

                        if (!pair.second.empty()) {
                            generated << "add_library(" + pair.first + " SHARED IMPORTED)" << std::endl;

                            if (package.root()) {
                                generated << "set_target_properties(" + pair.first + " PROPERTIES IMPORTED_LOCATION \"${CMAKE_CURRENT_SOURCE_DIR}/" + pair.second + "\")" << std::endl;
                            }
                            else {
                                generated << "set_target_properties(" + pair.first + " PROPERTIES IMPORTED_LOCATION \"${CMAKE_CURRENT_SOURCE_DIR}/.grok/" + package.name() + "/" + pair.second + "\")" << std::endl;
                            }
                        }
                    }

                    if (!target_link_libraries.empty()) {
                        generated << "target_link_libraries(" + package.name();

                        for (const std::string& library : target_link_libraries) {
                            generated << " " + library;
                        }

                        generated << ")" << std::endl;
                    }
                }

                if (!package.dependencies().empty()) {
                    grok::lib::project project;

                    for (std::pair pair : package.dependencies()) {
                        generated << std::endl;

                        for (const common::json &directory : project.configuration().option("registries")) {
                            grok::lib::registry registry(directory);

                            if (registry.contains(pair.first)) {
                                grok::lib::package package(registry.open(pair.first));

                                generated << generate(package) << std::endl;

                                break;
                            }
                        }
                    }
                }

                return generated.str();
            }
    };
}