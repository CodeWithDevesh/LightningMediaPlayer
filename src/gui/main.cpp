#include "main.hpp"
#include <QtWidgets>
#include "core/lightning_core.hpp"

int start(int argc, char **argv)
{
    qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    QApplication app(argc, argv);

    lighter::Core core_instance;
    lighter::EventManager *eventManager = core_instance.getEventManager();

    eventManager->registerListner(lighter::Events::EventType::MEDIA_ADDED, [&](std::any data)
                                  {auto e = std::any_cast<lighter::Events::MediaAddedEvent>(data); DEBUG("Media added: {} - {}", e.name, e.path); });

    lighter::Playlist *playlist = core_instance.getPlaylist();
    lighter::Player *player = core_instance.getPlayer();

    // Main window
    QWidget window;
    window.setWindowTitle("Lightning Media Player");
    window.resize(800, 600);

    // Placeholder for video widget
    QWidget *videoWidget = new QWidget();

    // Playback controls
    QPushButton *playButton = new QPushButton("Play");
    QPushButton *pauseButton = new QPushButton("Pause");
    QPushButton *stopButton = new QPushButton("Stop");
    QSlider *seekSlider = new QSlider(Qt::Horizontal);
    seekSlider->setRange(0, 100);

    // Volume control
    QLabel *volumeLabel = new QLabel("Volume:");
    QSlider *volumeSlider = new QSlider(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    // Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    QHBoxLayout *volumeLayout = new QHBoxLayout;

    // Add widgets to layouts
    controlsLayout->addWidget(playButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(stopButton);
    controlsLayout->addWidget(seekSlider);

    volumeLayout->addWidget(volumeLabel);
    volumeLayout->addWidget(volumeSlider);

    mainLayout->addWidget(videoWidget); // Add the placeholder video widget
    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(volumeLayout);

    window.setLayout(mainLayout);

    // Show the window
    window.show();
    player->setXWindow(videoWidget->winId());

    playlist->addMedia("/home/devesh/Downloads/blah.webm");
    return app.exec();
}