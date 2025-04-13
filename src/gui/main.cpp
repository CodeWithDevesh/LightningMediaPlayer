#include "main.hpp"
#include <QtWidgets>
#include "core/lightning_core.hpp"
#include "widgets/videoWidget.hpp"
#include "widgets/mainWindow.hpp"

int start(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    spdlog::set_level(spdlog::level::debug);
    QApplication app(argc, argv);

    lighter::Core core_instance;
    lighter::EventManager *eventManager = core_instance.getEventManager();

    eventManager->registerListner(lighter::Events::EventType::MEDIA_ADDED, [&](std::any data)
                                  {auto e = std::any_cast<lighter::Events::MediaAddedEvent>(data); DEBUG("Media added: {} - {}", e.name, e.path); });
    lighter::Playlist *playlist = core_instance.getPlaylist();

    // Main window
    MainWindow window(&core_instance);
    window.setWindowTitle("Lightning Player");
    window.resize(800, 600);

    window.show();

    playlist->addMedia("/home/devesh/Downloads/blah.webm");
    return app.exec();
}