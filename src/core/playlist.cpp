#include "lightning_playlist.hpp"

namespace lighter
{

  Playlist::Playlist(EventManager *eventManager)
  {
    DEBUG("Initializing Playlist...");
    this->eventManager = eventManager;
    DEBUG("Playlist initialized successfully.");
  }

  Playlist::~Playlist()
  {
    DEBUG("Clearing Playlist resources...");
    mediaFiles.clear();
    DEBUG("Playlist cleanup complete.");
  }

  void Playlist::shuffel()
  {
    DEBUG("Shuffling Playlist...");
    std::random_shuffle(mediaFiles.begin(), mediaFiles.end());
    eventManager->triggerEvent(Events::PLAYLIST_UPDATED, Events::PlaylistUpdatedEvent{.change = "shuffle"});
    DEBUG("Playlist shuffled successfully.");
  }

  void Playlist::setLoopMode(LoopMode loopMode)
  {
    DEBUG("Setting Loop Mode...");
    this->loopMode = loopMode;
    eventManager->triggerEvent(Events::PLAYLIST_UPDATED, Events::PlaylistUpdatedEvent{.change = "loopMode"});
    DEBUG("Loop Mode set to: {}", loopMode);
  }

  void Playlist::addMedia(const std::string &filePath)
  {
    DEBUG("Adding media to Playlist: {}", filePath);

    mediaFiles.push_back(filePath);
    Events::MediaAddedEvent e;
    e.name = getFileName(filePath);
    e.path = filePath;
    // TODO : add checks if the file exists
    eventManager->triggerEvent(Events::MEDIA_ADDED, e);
    DEBUG("Media added successfully.");

    if (mediaFiles.size() == 1)
    {
      DEBUG("First media added, notifying player");
      currentIndex = 0;
      switchedMediaEv();
    }
  }

  void Playlist::removeMedia(int index)
  {
    DEBUG("Removing media from Playlist at index: {}", index);
    if (index < mediaFiles.size())
    {
      Events::MediaRemovedEvent e;
      e.name = getFileName(mediaFiles[index]);
      e.path = mediaFiles[index];
      e.index = index;
      e.wasCurrent = (index == currentIndex);
      mediaFiles.erase(mediaFiles.begin() + index);
      eventManager->triggerEvent(Events::MEDIA_REMOVED, e);
      DEBUG("Media removed successfully.");
    }
    else
    {
      WARN("Invalid index: {}", index);
    }
  }

  void Playlist::clear()
  {
    DEBUG("Clearing Playlist...");
    mediaFiles.clear();
    DEBUG("Playlist cleared successfully.");
  }

  std::string Playlist::getCurrentMedia() const
  {
    if (currentIndex < mediaFiles.size())
    {
      return mediaFiles[currentIndex];
    }
    else
    {
      return NULL;
    }
  }

  bool Playlist::next()
  {
    DEBUG("Switching to next media...");
    if (currentIndex < mediaFiles.size() - 1)
    {
      currentIndex++;
    }
    else
    {
      if (loopMode == LOOP_ALL)
      {
        currentIndex = 0;
      }
      else
      {
        WARN("End of Playlist reached.");
        return false;
      }
    }
    switchedMediaEv();
    DEBUG("Switched to next media successfully.");
    return true;
  }

  bool Playlist::previous()
  {
    if (currentIndex > 0 && currentIndex < mediaFiles.size())
    {
      currentIndex--;
    }
    else
    {
      WARN("Already at the start of the playlist");
      return false;
    }
    switchedMediaEv();
    return true;
  }

  void Playlist::switchedMediaEv()
  {
    Events::PlaybackMediaChangedEvent e;
    e.name = getFileName(mediaFiles[currentIndex]);
    e.path = mediaFiles[currentIndex];
    e.index = currentIndex;
    eventManager->triggerEvent(Events::PLAYBACK_MEDIA_CHANGED, e);
  }
}