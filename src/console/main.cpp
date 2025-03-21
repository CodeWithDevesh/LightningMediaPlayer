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
    std::string mediaFile = "./blah.webm";

    cli::setupCLI(app, mediaFile);
    CLI11_PARSE(app, argc, argv);

    try
    {
        lighter::Core core_instance;

        if (!core_instance.loadMedia(mediaFile))
        {
            ERROR("Failed to load media file.");
            return -1;
        }

        std::cout << "Media loaded successfully: " << mediaFile << "\n";
        displayHelp();

        std::string command;
        while (true)
        {
            std::cout << "> ";
            std::getline(std::cin, command);

            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd;

            if (cmd == "play")
            {
                core_instance.play();
            }
            else if (cmd == "pause")
            {
                core_instance.pause();
            }
            else if (cmd == "stop")
            {
                core_instance.stop();
            }
            else if (cmd == "seek")
            {
                double time;
                if (iss >> time)
                {
                    core_instance.seek(time);
                }
                else
                {
                    std::cout << "Invalid usage. Example: seek 30\n";
                }
            }
            else if (cmd == "volume")
            {
                int level;
                if (iss >> level)
                {
                    core_instance.setVolume(level);
                }
                else
                {
                    std::cout << "Invalid usage. Example: volume 50\n";
                }
            }
            else if (cmd == "info")
            {
                int duration = core_instance.getDuration();
                auto [width, height] = core_instance.getVideoResolution();
                std::cout << "Duration: " << duration << " seconds\n";
                std::cout << "Resolution: " << width << "x" << height << "\n";
            }
            else if (cmd == "exit")
            {
                core_instance.stop();
                break;
            }
            else
            {
                std::cout << "Unknown command. Type 'help' for a list of commands.\n";
            }
        }
    }
    catch (const std::exception &ex)
    {
        ERROR(ex.what());
        return -1;
    }

    return 0;
}