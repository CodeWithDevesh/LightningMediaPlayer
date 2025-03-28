#include "lightning_core.hpp"

namespace lighter
{

  bool Core::init()
  {
    try
    {
      DEBUG("Initializing libVLC...");

      const char *vlc_args[] = {"--quiet", "--no-video-title-show",
                                "--avcodec-hw=any"};

      // Before anything initialize the event manager
      eventManager = new EventManager();

      vlcInstance = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
      if (!vlcInstance)
      {
        ERROR("Failed to initialize libVLC.");
        return false;
      }

      libvlc_log_set(vlcInstance, vlc_log_callback, nullptr);


      DEBUG("libVLC initialized successfully.");
      playlist = new Playlist(eventManager);
      player = new Player(vlcInstance, playlist, eventManager);

      return true;
    }
    catch (const std::exception &ex)
    {
      ERROR("Core initialization failed: {}", ex.what());
      return false;
    }
  }

  void Core::cleanup()
  {

    DEBUG("Cleaning up libVLC resources...");

    if (playlist)
    {
      delete playlist;
      playlist = nullptr;
    }
    if (player)
    {
      delete player;
      player = nullptr;
    }
    if (eventManager)
    {
      delete eventManager;
      eventManager = nullptr;
    }

    DEBUG("Releasing libVLC instance...");
    if (vlcInstance)
    {
      libvlc_release(vlcInstance);
      vlcInstance = nullptr;
    }

    DEBUG("Core module cleanup complete.");
  }

  void Core::vlc_log_callback(void *data, int level, const libvlc_log_t *ctx, const char *fmt, va_list args)
  {
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);

    std::string log_msg = "[LibVLC] " + std::string(buffer);

    // Map VLC log levels to spdlog
    switch (level)
    {
    case LIBVLC_DEBUG:
      spdlog::debug(log_msg);
      break;
    case LIBVLC_NOTICE:
      spdlog::info(log_msg);
      break;
    case LIBVLC_WARNING:
      spdlog::warn(log_msg);
      break;
    case LIBVLC_ERROR:
      spdlog::error(log_msg);
      break;
    default:
      spdlog::trace(log_msg);
      break;
    }
  }

} // namespace lighter
