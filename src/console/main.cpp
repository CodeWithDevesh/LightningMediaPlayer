#include "core/lightning_core.hpp"
#include "main.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

int start(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::critical); // TODO: Remove this line
    INFO("Running in console mode");

    try
    {
        lighter::Core core_instance;
        lighter::EventManager *eventManager = core_instance.getEventManager();

        eventManager->registerListner(lighter::Events::EventType::MEDIA_ADDED, [&](std::any data)
                                      {
            auto e = std::any_cast<lighter::Events::MediaAddedEvent>(data);
            DEBUG("Media added: {} - {}", e.name, e.path); });

        lighter::Playlist *playlist = core_instance.getPlaylist();
        lighter::Player *player = core_instance.getPlayer();

        auto screen = ftxui::ScreenInteractive::Fullscreen();
        auto component = ftxui::Renderer([&]
                                         { return ftxui::text("Press 'q' to quit, 'p' to play, 's' to stop.") | ftxui::center; });
        screen.Print();

        component = CatchEvent(component, [&](ftxui::Event event)
                               {
                if (event == ftxui::Event::Character('q')) {
                    screen.ExitLoopClosure()();  // Exit loop
                    return true;
                }
                if (event == ftxui::Event::Character('p')) {
                    player->play();
                }
                if (event == ftxui::Event::Character('s')) {
                    player->stop();
                }
                return false; });

        std::string mediaFile = "/home/devesh/Downloads/blah.webm";
        playlist->addMedia(mediaFile);
        screen.Loop(component);
    }
    catch (const std::exception &ex)
    {
        ERROR(ex.what());
        return -1;
    }

    return 0;
}