#include "player.hpp"
#include "utils.hpp"

lighter::Player::Player(libvlc_instance_t *vlcInstance, Playlist *playlist) {
  INFO("Initializing Player...");
  player = libvlc_media_player_new(vlcInstance);
  INFO("Player initialized successfully.");
}

lighter::Player::~Player() {
  INFO("Clearing Player resources...");
  if (player) {
    libvlc_media_player_release(player);
    player = nullptr;
  }
  INFO("Player cleanup complete.");
}
