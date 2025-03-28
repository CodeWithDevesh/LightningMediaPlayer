#include "core/lightning_core.hpp"
#include "main.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/dom/elements.hpp"

int start(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::critical); // TODO: Remove this line
    INFO("Running in console mode");


    auto document = ftxui::hbox({ftxui::text("Playing: "), ftxui::gauge(0.5)});
    auto screen = ftxui::Screen::Create(ftxui::Dimension::Full());
    Render(screen, document);
    screen.Print();

    

    std::string mediaFile = "/home/devesh/Downloads/blah.webm";

    try
    {
        lighter::Core core_instance;
        lighter::EventManager *eventManager = core_instance.getEventManager();

        eventManager->registerListner(lighter::Events::EventType::MEDIA_ADDED, [&](std::any data)
                                      {
            auto e = std::any_cast<lighter::Events::MediaAddedEvent>(data);
            DEBUG("Media added: {} - {}", e.name, e.path); });

        lighter::Playlist *playlist = core_instance.getPlaylist();
        playlist->addMedia(mediaFile);
        sleep(10);
    }
    catch (const std::exception &ex)
    {
        ERROR(ex.what());
        return -1;
    }

    return 0;
}