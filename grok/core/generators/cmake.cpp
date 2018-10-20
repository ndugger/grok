# pragma once

# include <experimental/filesystem>

# include <iostream>
# include <sstream>
# include <string>

# include "nlohmann/json.hpp"

# include "grok/core/generators/art.cpp"
# include "grok/core/registry.cpp"
# include "grok/core/utilities.cpp"

namespace grok::core::generators {
    using std::endl;
    using std::string;
    using std::stringstream;
    using namespace nlohmann;

    namespace {
        namespace fs = std::experimental::filesystem;
    }

    string cmake (json package, bool is_root = false) {
        json package_meta = package.at("package");
        string package_name = package_meta.at("name");

        stringstream generated_cmake;

        for (const string& line : art()) {
            generated_cmake << "#  " << line << endl;
        }

        generated_cmake << endl;

        if (is_root) {
            json package_executables = package_meta.at("executables");
            json package_flags = package_meta.at("flags");

            generated_cmake << "project(" << package_name << ")" << endl;
            generated_cmake << endl;

            if (package_meta.find("standard") != package_meta.end()) {
                generated_cmake << "set(CMAKE_CXX_STANDARD " << package_meta.at("standard") << ")" << endl;
            }
            else {
                generated_cmake << "set(CMAKE_CXX_STANDARD 17)" << endl;
            }

            if (package_meta.find("flags") != package_meta.end()) {
                for (string flag : package_meta.at("flags")) {
                    generated_cmake << "set(CMAKE_CXX_FLAGS -" << flag << ")" << endl;
                }
            }

            if (package_meta.find("executables") != package_meta.end()) {
                generated_cmake << endl;

                for (string executable : package_meta.at("executables")) {
                    generated_cmake << "add_executable(" << package_name << " " << executable << ")" << endl;
                }
            }

            generated_cmake << endl;
        }

        generated_cmake << "# ==========: " << package_name << " :==========" << endl;

        if (package.find("includes") != package.end()) {
            json includes = package.at("includes");

            generated_cmake << "# - includes:" << endl;

            for (json::iterator include = includes.begin(); include != includes.end(); ++include) {
                if (is_root) {
                    generated_cmake << "include_directories(" << fs::current_path() / include.value() << ")" << endl;
                }
                else {
                    generated_cmake << "include_directories(" << fs::current_path() / ".grok" / package_name / include.value() << ")" << endl;
                }
            }
        }

        if (package.find("libraries") != package.end()) {
            json libraries = package.at("libraries");
            vector<string> target_link_libraries;

            generated_cmake << "# - libraries" << endl;

            for (json::iterator library = libraries.begin(); library != libraries.end(); ++library) {
                target_link_libraries.emplace_back(library.key());

                if (!library.value().empty()) {
                    generated_cmake << "add_library(" << library.key() << " SHARED IMPORTED)" << endl;

                    if (is_root) {
                        generated_cmake << "set_target_properties(" << library.key() << " PROPERTIES IMPORTED_LOCATION " << fs::current_path() / library.value() << ")" << endl;
                    }
                    else {
                        generated_cmake << "set_target_properties(" << library.key() << " PROPERTIES IMPORTED_LOCATION " << fs::current_path() / ".grok" / package_name / library.value() << ")" << endl;
                    }
                }
            }

            generated_cmake << "target_link_libraries(" << package_name;

            for (const string& target_link_library : target_link_libraries) {
                generated_cmake << " " << target_link_library;
            }

            generated_cmake << ")" << endl;
        }

        if (package.find("dependencies") != package.end()) {
            json dependencies = package.at("dependencies");

            for (json::iterator dependency = dependencies.begin(); dependency != dependencies.end(); ++dependency) {
                generated_cmake << endl;

                if (registry::contains(dependency.key())) {
                    generated_cmake << cmake(registry::open(dependency.key())) << endl;
                }
                else {
                    generated_cmake << "YOUR MUM" << endl;
                }
            }
        }

        return generated_cmake.str();
    }
}