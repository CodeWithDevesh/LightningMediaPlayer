#include "core.hpp"

namespace lighter {

bool Core::init() {
  try {
    INFO("Initializing libVLC...");

    const char *vlc_args[] = {"--verbose=2", "--no-video-title-show",
                              "--avcodec-hw=any"};

    vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
    if (!vlcInstance) {
      ERROR("Failed to initialize libVLC.");
      return false;
    }

    INFO("libVLC initialized successfully.");
    playlist = new Playlist();
    player = new Player(vlcInstance, playlist);

    return true;
  } catch (const std::exception &ex) {
    ERROR("Core initialization failed: {}", ex.what());
    return false;
  }
}

void Core::cleanup() {
  INFO("Cleaning up libVLC resources...");

  if (playlist) {
    delete playlist;
    playlist = nullptr;
  }
  if (player) {
    delete player;
    player = nullptr;
  }

  INFO("Releasing libVLC instance...");
  if (vlcInstance) {
    libvlc_release(vlcInstance);
    vlcInstance = nullptr;
  }

  INFO("Core module cleanup complete.");
}

} // namespace lighter
