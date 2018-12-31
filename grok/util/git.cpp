# pragma once

# include <experimental/filesystem>
# include <string>

# include "git2.h"

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::util::git {

    void initialize () {
        git_libgit2_init();
    }

    void uninitialize () {
        git_libgit2_shutdown();
    }

    class repository {

        private:
            std::string repository_address;
            git_repository* repository_git_repository;

        public:
            explicit repository (const std::string& address) {
                repository_address = address;
                repository_git_repository = nullptr;
            }

            ~ repository () {
                git_repository_free(repository_git_repository);
            }

            bool clone (const fs::path& directory) {
                git_clone_options clone_options = GIT_CLONE_OPTIONS_INIT;
                return git_clone(&repository_git_repository, repository_address.c_str(), directory.c_str(), &clone_options) == 0;
            }

            bool checkout (const std::string& branch) {
                return true;
            }
        };
}