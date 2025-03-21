#pragma once

#ifndef CORE_PLAYLIST
#define CORE_PLAYLIST
#include "utils.hpp"

namespace lighter
{
  class Playlist
  {
  public:
    Playlist();
    ~Playlist();
    void shuffel();
    void loop();
    void addMedia(const std::string &filePath);
    void removeMedia(size_t index);
    void clear();
    std::string getCurrentMedia() const;
    bool next();
    bool previous();
  private:
    std::vector<std::string> mediaFiles;
    size_t currentIndex;
  };
}
#endif // !CORE_PLAYLIST