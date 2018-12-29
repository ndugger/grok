# pragma once

# include "git2.h"

namespace grok::util::git {

    void initialize () {
        git_libgit2_init();
    }

    void uninitialize () {
        git_libgit2_shutdown();
    }
}