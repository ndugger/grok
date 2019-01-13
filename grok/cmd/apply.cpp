# pragma once

# include "fs/current_path.cpp"
# include "fs/file.cpp"

# include "grok/lib/project.cpp"
# include "grok/gen/cmake.cpp"
# include "grok/util/print.cpp"

namespace grok::cmd {

    void apply () {
        grok::lib::project project;

        if (!project.exists()) {
            grok::util::print("project has not yet been initialized; run 'grok create' to get started");
            return;
        }

        fs::file(fs::current_path() / ".grokmake").write(grok::gen::cmake().generate(project.info()), fs::file::mode::out);

        grok::util::print("project has been applied");
    }
}