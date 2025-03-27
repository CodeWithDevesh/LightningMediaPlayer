#include <iostream>
#include <sstream>
#include <string>
#include "core/core.hpp"
#include "cli_handler.hpp"
#include "main.hpp"

void displayHelp()
{
    std::cout << "Available commands:\n"
              << "  play            - Start playback\n"
              << "  pause           - Pause playback\n"
              << "  stop            - Stop playback\n"
              << "  seek <time>     - Seek to <time> in seconds\n"
              << "  volume <level>  - Set volume (0-100)\n"
              << "  info            - Display media information\n"
              << "  exit            - Exit the player\n";
}

int start(int argc, char **argv)
{
    INFO("Running in console mode");

    CLI::App app{"Lightning Media Player"};
    std::string mediaFile = "~/Downloads/blah.webm";

    cli::setupCLI(app, mediaFile);
    CLI11_PARSE(app, argc, argv);
    try
    {
        lighter::Core core_instance;
        lighter::EventManager *eventManager = core_instance.getEventManager();

        lighter::Playlist *playlist = core_instance.getPlaylist();
        playlist->addMedia(mediaFile);
    }
    catch (const std::exception &ex)
    {
        ERROR(ex.what());
        return -1;
    }

    return 0;
}