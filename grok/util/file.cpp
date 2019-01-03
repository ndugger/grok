# pragma once

# include <experimental/filesystem>
# include <string>

namespace {
    namespace fs = std::experimental::filesystem;
}

namespace grok::util {

    class file {

        public:
            static fs::path current_path () {
                return fs::current_path();
            }

            static fs::path path (const std::string& path) {
                return fs::path(path);
            }

            static void remove (const std::string path) {
                fs::remove(path);
            }

            static void rename (const std::string& from, const std::string& to) {
                fs::rename(from, to);
            }

        private:
            std::string file_contents;
            fs::path file_path;

        public:
            explicit file (const std::string& path = fs::current_path()) {
                file_path = fs::path(path);
            }

            std::string contents () {
                return file_contents;
            }

            fs::path& path () {
                return file_path;
            }

            bool open () {
                return true;
            }
    };
}