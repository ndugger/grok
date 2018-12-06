# include <string>
# include <vector>

# include "common/program.cpp"

namespace grok {

    int main (std::vector<std::string> arguments) {
        common::program cli;

        cli.start();

        while (cli.running());
        return cli.status();
    }
}

int main (int arguments_size, char* arguments_values[]) {
    return grok::main(std::vector<std::string>(arguments_values, arguments_values + arguments_size));
}