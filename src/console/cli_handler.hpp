#ifndef CLI_HANDLER_HPP
#define CLI_HANDLER_HPP

#include <CLI/CLI.hpp>
#include <string>

namespace cli {
    void setupCLI(CLI::App& app, std::string& mediaFile);
}

#endif // CLI_HANDLER_HPP
