#ifndef CORE_CORE
#define CORE_CORE

#include "lightning_utils.hpp"
#include "lightning_eventmanager.hpp"
#include "lightning_events.hpp"
#include "lightning_playlist.hpp"
#include "lightning_player.hpp"

namespace lighter
{
  class Core
  {
  public:
    libvlc_instance_t *vlcInstance = nullptr;
    // Constructor: Initializes the Core
    Core()
    {
      if (!init())
      {
        throw std::runtime_error("Failed to initialize Core");
      }
    }
    // Destructor: Cleans up resources
    ~Core() { cleanup(); }
    // Delete copy constructor and assignment operator
    Core(const Core &) = delete;
    Core &operator=(const Core &) = delete;

    // Get the Player
    Player *getPlayer() const { return player; }
    // Get the Playlist
    Playlist *getPlaylist() const { return playlist; }
    // Get the EventManager
    EventManager *getEventManager() const { return eventManager; }

  private:
    bool init();
    void cleanup();
    Player *player = nullptr;
    Playlist *playlist = nullptr;
    EventManager *eventManager = nullptr;

    static void vlc_log_callback(void *data, int level, const libvlc_log_t *ctx, const char *fmt, va_list args);
  };
} // namespace lighter

#endif // !CORE_CORE
