#include "playlist.hpp"

namespace lighter
{

  Playlist::Playlist(EventManager *eventManager)
  {
    INFO("Initializing Playlist...");
    currentIndex = 0;
    this->eventManager = eventManager;
    INFO("Playlist initialized successfully.");
  }

  Playlist::~Playlist()
  {
    INFO("Clearing Playlist resources...");
    mediaFiles.clear();
    INFO("Playlist cleanup complete.");
  }

  void Playlist::shuffel()
  {
    INFO("Shuffling Playlist...");
    // std::random_shuffle(mediaFiles.begin(), mediaFiles.end());

    INFO("Playlist shuffled successfully.");
  }

  void Playlist::setLoopMode(LoopMode loopMode)
  {
    INFO("Setting Loop Mode...");
    this->loopMode = loopMode;
    INFO("Loop Mode set to: {}", loopMode);
  }

  void Playlist::addMedia(const std::string &filePath)
  {
    INFO("Adding media to Playlist: {}", filePath);

    mediaFiles.push_back(filePath);
    Events::MediaAddedEvent e;
    e.name = getFileName(filePath);
    e.path = filePath;
    // TODO : add checks if the file exists
    eventManager->triggerEvent(Events::MEDIA_ADDED, e);
    INFO("Media added successfully.");
  }

  void Playlist::removeMedia(int index)
  {
    INFO("Removing media from Playlist at index: {}", index);
    if (index < mediaFiles.size())
    {
      mediaFiles.erase(mediaFiles.begin() + index);
      INFO("Media removed successfully.");
    }
    else
    {
      WARN("Invalid index: {}", index);
    }
  }

  void Playlist::clear()
  {
    INFO("Clearing Playlist...");
    mediaFiles.clear();
    INFO("Playlist cleared successfully.");
  }

  std::string Playlist::getCurrentMedia() const
  {
    if (currentIndex < mediaFiles.size())
    {
      return mediaFiles[currentIndex];
    }
    else
    {
      return "";
    }
  }

  bool Playlist::next()
  {
    INFO("Switching to next media...");
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
    INFO("Switched to next media successfully.");
    return true;
  }

}