# pragma once

# include <experimental/filesystem>

# include <iostream>
# include <fstream>
# include <string>
# include <vector>

# include "git2/clone.h"
# include "git2/global.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

namespace grok::core {
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::ofstream;
    using std::string;
    using std::stringstream;
    using std::vector;

    using json = nlohmann::json;

    namespace {
        namespace fs = std::experimental::filesystem;
    }

    void initialize () {
        git_libgit2_init();

        if (fs::exists(fs::temp_directory_path() / ".groktemp")) {
            fs::remove_all(fs::temp_directory_path() / ".groktemp");
        }
    }

    void uninitialize () {
        git_libgit2_shutdown();

        if (fs::exists(fs::temp_directory_path() / ".groktemp")) {
            fs::remove_all(fs::temp_directory_path() / ".groktemp");
        }
    }

    void display_message (string message) {
        cout << message << endl;
    }

    bool project_exists () {
        return fs::exists(fs::current_path() / ".grokpackage");
    }

    json open_project () {
        auto project_stream = ifstream(fs::current_path() / ".grokpackage");
        stringstream project_json;

        project_json << project_stream.rdbuf();

        return json::parse(project_json);
    }

    bool project_depends_on (string package_name) {
        return false;
    }

    bool registry_contains (string command_origin, string package_name) {
        return fs::exists(fs::path(command_origin) / "registry" / (package_name + ".grokpackage"));
    }

    json open_registered_package (string command_origin, string package_name) {
        auto package_stream = ifstream(fs::path(command_origin) / "registry" / (package_name + ".grokpackage"));
        stringstream package_json;

        package_json << package_stream.rdbuf();

        return json::parse(package_json);
    }

    bool package_in_use (string package_name) {
        return fs::exists(fs::current_path() / ".grok" / package_name);
    }

    bool package_is_available (string package_repository) {
        return true; // TODO ... how to simply just check if repo exists by its url
    }

    git_repository* discover_package (string package_repository, string package_release) {
        git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
        clone_opts.checkout_branch = package_release.c_str();

        git_repository* repository = nullptr;

        git_clone(&repository, package_repository.c_str(), (fs::temp_directory_path() / ".groktemp").c_str(), &clone_opts);

        return repository;
    }

    json open_discovered_package () {
        auto package_stream = ifstream(fs::temp_directory_path() / ".groktemp" / ".grokpackage");
        stringstream package_json;

        package_json << package_stream.rdbuf();

        return json::parse(package_json);
    }

    void save_discovered_package (string package_name, string package_release) {
        fs::rename(
            fs::temp_directory_path() / ".groktemp",
            fs::current_path() / ".grok" / package_name
        );
    }

    git_repository* download_package (string package_name, string package_repository, string package_release) {
        git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
        clone_opts.checkout_branch = package_release.c_str();

        git_repository* repository = nullptr;

        git_clone(&repository, package_repository.c_str(), (fs::current_path() / ".grok" / package_name).c_str(), &clone_opts);

        return repository;
    }

    void add_dependency_to_project (string package_name, string package_release) {
        auto package_stream = ifstream(fs::current_path() / ".grokpackage");
        stringstream package_json;

        package_json << package_stream.rdbuf();

        json package = json::parse(package_json);

        if (package[ "dependencies" ] == nullptr) {
            package[ "dependencies" ] = { { package_name, package_release } };
        }
        else {
            package[ "dependencies" ][ package_name ] = package_release;
        }

        auto file_stream = ofstream(fs::current_path() / ".grokpackage");

        file_stream << package.dump(4);
    }

    string generate_cmake (const string& command_origin, const json& parent_package, const bool is_project = false) {
        string parent_package_name = parent_package[ "package" ][ "name" ];
        stringstream cmake_source;

        if (is_project) {
            cmake_source << "project(" << parent_package_name << ")" << endl;
            cmake_source << endl;
        }

        cmake_source << "# ==========: " << parent_package_name << " :==========" << endl;

        if (parent_package.find("includes") != parent_package.end()) {
            json includes = parent_package.at("includes");

            cmake_source << "# - includes" << endl;

            for (json::iterator include = includes.begin(); include != includes.end(); ++include) {
                if (is_project) {
                    cmake_source << "include_directories(" << fs::current_path() / include.value() << ")" << endl;
                }
                else {
                    cmake_source << "include_directories(" << fs::current_path() / ".grok" / parent_package_name / include.value() << ")" << endl;
                }
            }
        }

        if (parent_package.find("libraries") != parent_package.end()) {
            json libraries = parent_package.at("libraries");
            vector<string> target_link_libraries;

            cmake_source << "# - libraries" << endl;

            for (json::iterator library = libraries.begin(); library != libraries.end(); ++library) {
                target_link_libraries.emplace_back(library.key());

                if (library.value() != nullptr) {
                    cmake_source << "add_library(" << library.key() << " SHARED IMPORTED)" << endl;

                    if (is_project) {
                        cmake_source << "set_target_properties(" << library.key() << " PROPERTIES IMPORTED_LOCATION " << fs::current_path() / library.value() << ")" << endl;
                    }
                    else {
                        cmake_source << "set_target_properties(" << library.key() << " PROPERTIES IMPORTED_LOCATION " << fs::current_path() / ".grok" / parent_package_name / library.value() << ")" << endl;
                    }

                }
            }

            cmake_source << "target_link_libraries(" << parent_package_name;

            for (const string& target_link_library : target_link_libraries) {
                cmake_source << " " << target_link_library;
            }

            cmake_source << ")" << endl;
        }

        if (parent_package.find("dependencies") != parent_package.end()) {
            json dependencies = parent_package.at("dependencies");

            for (json::iterator dependency = dependencies.begin(); dependency != dependencies.end(); ++dependency) {
                cmake_source << endl;

                if (registry_contains(command_origin, dependency.key())) {
                    cmake_source << generate_cmake(command_origin, open_registered_package(command_origin, dependency.key())) << endl;
                }
                else {
                    cmake_source << "YOUR MUM" << endl;
                }
            }
        }

        return cmake_source.str();
    }
}