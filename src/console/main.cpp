#include <iostream>
#include "core/core.hpp"
#include "cli_handler.hpp"

int main(int argc, char** argv) {
	INFO("Running in console mode");

	CLI::App app{ "Lightning Media Player" };
	std::string mediaFile = "./blah.mkv";

	cli::setupCLI(app, mediaFile);
	CLI11_PARSE(app, argc, argv);

	try
	{
		// Keep this instance alive till code is running
		lighter::Core core_instance;
		lighter::Player player;
		player.loadMedia(mediaFile);
	}
	catch (const std::exception& ex)
	{
		ERROR(ex.what());
		return -1;
	}

	return 0;
}
