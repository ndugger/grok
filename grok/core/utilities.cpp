# pragma once

# include <experimental/filesystem>

# include <iostream>
# include <fstream>
# include <string>

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
        return fs::exists(fs::path(command_origin) / ".." / "registry" / (package_name + ".grokpackage"));
    }

    json open_registered_package (string command_origin, string package_name) {
        auto package_stream = ifstream(fs::path(command_origin) / ".." / "registry" / (package_name + ".grokpackage"));
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

    string generate_cmake (string command_origin, json parent_package) {
        stringstream cmake_source;

        if (parent_package.find("dependencies") != parent_package.end()) {
            json dependencies = parent_package.at("dependencies");

            for (json::iterator dependency = dependencies.begin(); dependency != dependencies.end(); ++dependency) {
                if (registry_contains(command_origin, dependency.key())) {
                    json child_package = open_registered_package(command_origin, dependency.key());

                    // append includes, libraries, flags? etc.

                    cmake_source << generate_cmake(command_origin, child_package) << endl;
                }
                else {

                }
            }
        }

        return cmake_source.str();
    }
}