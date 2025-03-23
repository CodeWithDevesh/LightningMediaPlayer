#ifndef CORE_CORE
#define CORE_CORE

#include "player.hpp"
#include "playlist.hpp"
#include <vlc/vlc.h>

namespace lighter {
class Core {
public:
  libvlc_instance_t *vlcInstance = nullptr;
  // Constructor: Initializes the Core
  Core() {
    if (!init()) {
      throw std::runtime_error("Failed to initialize Core");
    }
  }
  // Destructor: Cleans up resources
  ~Core() { cleanup(); }
  // Delete copy constructor and assignment operator
  Core(const Core &) = delete;
  Core &operator=(const Core &) = delete;

  // Get the Player instance
  Player *getPlayer() const { return player; }
  // Get the Playlist instance
  Playlist *getPlaylist() const { return playlist; }

private:
  bool init();
  void cleanup();
  Player *player = nullptr;
  Playlist *playlist = nullptr;
};
} // namespace lighter

#endif // !CORE_CORE
