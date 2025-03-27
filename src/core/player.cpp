#include "player.hpp"
#include "utils.hpp"

lighter::Player::Player(libvlc_instance_t *vlcInstance, Playlist *playlist)
{
  INFO("Initializing Player...");
  player_int = libvlc_media_player_new(vlcInstance);
  INFO("Player initialized successfully.");
}

lighter::Player::~Player()
{
  INFO("Clearing Player resources...");
  if (player_int)
  {
    libvlc_media_player_release(player_int);
    player_int = nullptr;
  }
  INFO("Player cleanup complete.");
}

void lighter::Player::play()
{
  if (player_int)
  {
    libvlc_media_player_play(player_int);
    isPlaying = true;
  }
}