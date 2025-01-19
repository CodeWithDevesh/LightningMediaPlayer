#include "cli_handler.hpp"

namespace cli {
	void setupCLI(CLI::App& app, std::string& mediaFile) {
		app.add_option("-f,--file", mediaFile, "Media file to play");
	}
}
