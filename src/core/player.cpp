#include "lightning_player.hpp"

lighter::Player::Player(libvlc_instance_t *vlcInstance, Playlist *playlist, EventManager *eventManager)
{
  DEBUG("Initializing Player...");
  player_inst = libvlc_media_player_new(vlcInstance);
  this->playlist = playlist;
  this->eventManager = eventManager;
  this->vlc_inst = vlcInstance;
  eventManager->registerListner(Events::EventType::PLAYBACK_MEDIA_CHANGED, [this](std::any arg)
                                { onMediaChanged(arg); });
  DEBUG("Player initialized successfully.");
}

lighter::Player::~Player()
{
  DEBUG("Clearing Player resources...");
  stop();
  if (player_inst)
  {
    libvlc_media_player_release(player_inst);
    player_inst = nullptr;
  }
  DEBUG("Player cleanup complete.");
}

void lighter::Player::onMediaChanged(std::any arg)
{
  auto e = std::any_cast<Events::PlaybackMediaChangedEvent>(arg);
  DEBUG("Media Changed: {}", e.name);
  if (isPlaying)
  {
    stop();
  }
  libvlc_media_t *media = libvlc_media_new_path(vlc_inst, e.path.c_str());
  libvlc_media_player_set_media(player_inst, media);
  libvlc_media_release(media);
  play();
}

void lighter::Player::play()
{
  if (player_inst)
  {
    libvlc_media_player_play(player_inst);
    isPlaying = true;
  }
}

void lighter::Player::pause()
{
  if (player_inst)
  {
    libvlc_media_player_pause(player_inst);
    isPlaying = false;
  }
}

void lighter::Player::stop()
{
  if (player_inst)
  {
    libvlc_media_player_stop(player_inst);
    isPlaying = false;
  }
}

void lighter::Player::seek(double seconds)
{
  if (player_inst)
  {
    libvlc_media_player_set_time(player_inst, seconds * 1000);
  }
}

void lighter::Player::setVolume(int volume)
{
  if (player_inst)
  {
    libvlc_audio_set_volume(player_inst, volume);
  }
}