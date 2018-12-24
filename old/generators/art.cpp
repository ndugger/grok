# include <string>
# include <vector>

namespace grok::core::generators {
    using std::string;
    using std::vector;

    vector<string> art () {
        return {
            "                              _",
            "                             //",
            "     ____    ____   ____    //",
            "    //  \\\\  //  \\\\ //  \\\\  //___",
            "   //   // //     //   // //\\\\",
            "  //   // //     //   // //  \\\\",
            "  \\\\__// //      \\\\__// //    \\\\",
            "     //  PACKAGE MANAGER",
            "\\\\__//"
        };
    }
}