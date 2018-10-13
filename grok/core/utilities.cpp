# pragma once

# include <iostream>
# include <fstream>
# include <string>

# include <experimental/filesystem>

# include "git2/clone.h"
# include "git2/global.h"
# include "git2/repository.h"
# include "nlohmann/json.hpp"

using namespace std;
using namespace std::experimental;
using namespace nlohmann;

namespace grok::core {

    void initialize () {
        git_libgit2_init();

        if (filesystem::exists(filesystem::temp_directory_path() / ".groktemp")) {
            filesystem::remove_all(filesystem::temp_directory_path() / ".groktemp");
        }
    }

    void uninitialize () {
        git_libgit2_shutdown();

        if (filesystem::exists(filesystem::temp_directory_path() / ".groktemp")) {
            filesystem::remove_all(filesystem::temp_directory_path() / ".groktemp");
        }
    }

    void display_message (string message) {
        cout << message << endl;
    }

    bool project_exists () {
        return filesystem::exists(filesystem::current_path() / ".grokpackage");
    }

    bool registry_contains (string command_from, string package_name) {
        bool x = filesystem::exists(filesystem::path(command_from) / ".." / "registry" / (package_name + ".grokpackage"));
        return x;
    }

    json open_registered_package (string command_from, string package_name) {
        auto package_stream = ifstream(filesystem::path(command_from) / ".." / "registry" / (package_name + ".grokpackage"));
        stringstream package_json;

        package_json << package_stream.rdbuf();

        return json::parse(package_json);
    }

    bool package_in_use (string package_name) {
        return filesystem::exists(filesystem::current_path() / ".grok" / package_name);
    }

    bool package_is_available (string package_repository) {
        return true; // TODO ... how to simply just check if repo exists by its url
    }

    git_repository* discover_package (string package_repository, string package_release) {
        git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
        clone_opts.checkout_branch = package_release.c_str();

        git_repository* repository = nullptr;

        git_clone(&repository, package_repository.c_str(), (filesystem::temp_directory_path() / ".groktemp").c_str(), &clone_opts);

        return repository;
    }

    json open_discovered_package (git_repository* discovered_package_repository) {
        auto package_stream = ifstream(filesystem::temp_directory_path() / ".groktemp" / ".grokpackage");
        stringstream package_json;

        package_json << package_stream.rdbuf();

        return json::parse(package_json);
    }

    void save_discovered_package (string package_name, git_repository* discovered_package_repository) {
        filesystem::rename(
            filesystem::temp_directory_path() / ".groktemp",
            filesystem::current_path() / ".grok" / package_name
        );
    }

    git_repository* download_package (string package_name, string package_repository, string package_release) {
        git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
        clone_opts.checkout_branch = package_release.c_str();

        git_repository* repository = nullptr;

        git_clone(&repository, package_repository.c_str(), (filesystem::current_path() / ".grok" / package_name).c_str(), &clone_opts);

        return repository;
    }

    void add_dependency_to_project (string package_name, string package_release) {
        auto package_stream = ifstream(filesystem::current_path() / ".grokpackage");
        stringstream package_json;

        package_json << package_stream.rdbuf();

        json package = json::parse(package_json);

        if (package[ "dependencies" ] == nullptr) {
            package[ "dependencies" ] = { { package_name, package_release } };
        }
        else {
            package[ "dependencies" ][ package_name ] = package_release;
        }

        auto file_stream = ofstream(filesystem::current_path() / ".grokpackage");

        file_stream << package.dump(4);
    }

    void add_include_to_project (string package_name, string package_release, string include_location) {

    }

    void add_library_to_project (string package_name, string package_release, string library_name, string library_location) {

    }
}