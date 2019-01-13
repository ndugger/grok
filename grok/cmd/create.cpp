# pragma once

# include <string>
# include <vector>

# include "fs/current_path.cpp"
# include "fs/file.cpp"

# include "grok/cmd/apply.cpp"
# include "grok/lib/project.cpp"
# include "grok/gen/cmake.cpp"
# include "grok/util/input.cpp"
# include "grok/util/print.cpp"
# include "grok/util/split.cpp"

namespace grok::cmd {

    void create (const std::string& generator_name = "cmake") {
        grok::lib::project project;

        if (project.exists()) {
            grok::util::print("project already exists; do you want to overwrite it? (y/n)");

            std::string overwrite(grok::util::input());

            if (!overwrite.empty() && overwrite != "y") {
                return;
            }
        }

        fs::file(fs::current_path() / ".grokpackage").remove();

        grok::util::print("what's the name of this package? (ex: my_library)");
        project.rename(grok::util::input());

        grok::util::print("what's the address to this package's repository? (ex: https://example.com/repo.git)");
        project.remote(grok::util::input());

        grok::util::print("which branch/tag would you like to release on? (ex: master)");
        project.release(grok::util::input());

        grok::util::print("which c++ standard does your code require? (ex: 17)");
        project.standard(grok::util::input());

        grok::util::print("are there any flags you need to supply to cmake? (ex: pthreads, foo=bar)");
        for (const std::string& flag : grok::util::split(grok::util::input(), ',')) {
            std::vector<std::string> key_value(grok::util::split(flag, '='));
            std::string key(key_value.size() > 0 ? key_value.at(0) : "");
            std::string value(key_value.size() > 1 ? key_value.at(1) : "");

            project.flag(key, value);
        }

        grok::util::print("where are the executable source files? (ex: src/main.cpp, foo/bar.cpp)");
        for (const std::string& directory : grok::util::split(grok::util::input(), ',')) {
            project.source(directory);
        }

        grok::util::print("which directories should be included? (ex: include/, foo/)");
        for (const std::string& directory : grok::util::split(grok::util::input(), ',')) {
            project.include(directory);
        }

        grok::util::print("are there any external libraries that you need? (ex: stdc++fs, foo=bar.so)");
        for (const std::string& library : grok::util::split(grok::util::input(), ',')) {
            std::vector<std::string> key_value(grok::util::split(library, '='));
            std::string key(key_value.size() > 0 ? key_value.at(0) : "");
            std::string value(key_value.size() > 1 ? key_value.at(1) : "");

            project.require(key, value);
        }

        project.save();

        grok::cmd::apply();
    }
}