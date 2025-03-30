#include "main.hpp"
#include "core/lightning_core.hpp"
#include "dummy_widget.hpp"

int start(int argc, char *argv[])
{
  qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));

  libvlc_instance_t *vlc = libvlc_new(0, nullptr);

  libvlc_media_player_t *player = libvlc_media_player_new(vlc);
  libvlc_media_t *media = libvlc_media_new_path("/home/devesh/Downloads/blah.webm");
  libvlc_media_player_set_media(player, media);
  libvlc_media_release(media);
  media = nullptr;

  QApplication app(argc, argv);
  dummyW d;
  d.show();
  d.setWindowTitle(QApplication::translate("toplevel", "Dummy Widget"));
  INFO(d.winId());

  libvlc_media_player_set_xwindow(player, d.winId());

  libvlc_media_player_play(player);
  std::cout << QGuiApplication::platformName().toStdString() << std::endl;
  app.exec();

  libvlc_media_player_stop_async(player);
  libvlc_media_player_release(player);
  player = nullptr;
  libvlc_release(vlc);
  vlc = nullptr;
  return 0;
}
