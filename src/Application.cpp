#include "../include/Application.h"
#include "../include/VideoEditor.h"
#include <iostream>
#include <vector>
#include <string>

int videts::Application::Run(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << (argc>0?argv[0]:"videts") << " <input> <command> <output> [args...]" << std::endl;
        return 1;
    }

    std::string input = argv[1];
    std::string cmd = argv[2];
    std::string output = argv[3];

    std::vector<std::string> args;
    for (int i = 4; i < argc; ++i) args.emplace_back(argv[i]);

    try {
        VideoEditor editor(input);
        editor.execute(cmd, output, args);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 2;
    }

    return 0;
}
