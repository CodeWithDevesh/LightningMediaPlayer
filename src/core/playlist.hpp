#pragma once

#ifndef CORE_PLAYLIST
#define CORE_PLAYLIST


#include "events.hpp"
#include "eventmanager.hpp"
#include "utils.hpp"
#include <fmt/format.h>

namespace lighter
{
  enum LoopMode
  {
    NO_LOOP,
    LOOP_ALL,
    LOOP_ONE
  };

  class Playlist
  {
  public:
    Playlist(EventManager* eventManager);
    ~Playlist();
    void shuffel();
    void setLoopMode(LoopMode loopMode);
    inline LoopMode getLoopMode() const { return loopMode; }
    void addMedia(const std::string &filePath);
    void removeMedia(int index);
    void clear();
    std::string getCurrentMedia() const;
    bool next();
    bool previous();

  private:
    std::vector<std::string> mediaFiles;
    size_t currentIndex;
    LoopMode loopMode = NO_LOOP;
    EventManager *eventManager = nullptr;
  };
}

// Log formatter for LoopMode
template <>
struct fmt::formatter<lighter::LoopMode>
{
  constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const lighter::LoopMode &mode, FormatContext &ctx) -> decltype(ctx.out())
  {
    std::string modeStr;
    switch (mode)
    {
    case lighter::NO_LOOP:
      modeStr = "NO_LOOP";
      break;
    case lighter::LOOP_ALL:
      modeStr = "LOOP_ALL";
      break;
    case lighter::LOOP_ONE:
      modeStr = "LOOP_ONE";
      break;
    default:
      modeStr = "UNKNOWN";
      break;
    }
    return fmt::format_to(ctx.out(), "{}", modeStr);
  }
};
#endif // !CORE_PLAYLIST