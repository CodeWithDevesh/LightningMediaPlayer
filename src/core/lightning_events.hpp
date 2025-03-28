#pragma once

#ifndef CORE_EVENTS
#define CORE_EVENTS

#include "lightning_utils.hpp"
#include "fmt/format.h"

namespace lighter
{

  namespace Events
  {
    enum EventType
    {
      MEDIA_ADDED,                    // Generic
      MEDIA_REMOVED,                  // Generic
      PLAYLIST_UPDATED,               // Used to update the UI
      PLAYBACK_MEDIA_CHANGED,         // Notifies the player that the media has changed
    };

    struct Event
    {
      EventType type;
    };

    struct MediaAddedEvent : Event
    {
    public:
      EventType type = EventType::MEDIA_ADDED;
      std::string name;
      std::string path;
    };

    struct MediaRemovedEvent : Event
    {
      EventType type = EventType::MEDIA_REMOVED;
      std::string name;
      std::string path;
      bool wasCurrent;
      int index;
    };

    struct PlaylistUpdatedEvent : Event
    {
      EventType type = EventType::PLAYLIST_UPDATED;
      std::string change = "";
    };

    struct PlaybackMediaChangedEvent : Event
    {
      EventType type = EventType::PLAYBACK_MEDIA_CHANGED;
      std::string name;
      std::string path;
      int index;
    };
  }
}

template <>
struct fmt::formatter<lighter::Events::EventType>
{
  constexpr auto parse(fmt::format_parse_context &ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const lighter::Events::EventType &type, FormatContext &ctx) -> decltype(ctx.out())
  {
    std::string typeStr;
    switch (type)
    {
    case lighter::Events::MEDIA_ADDED:
      typeStr = "MEDIA_ADDED";
      break;
    case lighter::Events::MEDIA_REMOVED:
      typeStr = "MEDIA_REMOVED";
      break;
    case lighter::Events::PLAYLIST_UPDATED:
      typeStr = "PLAYLIST_UPDATED";
      break;
    case lighter::Events::PLAYBACK_MEDIA_CHANGED:
      typeStr = "PLAYBACK_MEDIA_CHANGED";
      break;
    default:
      typeStr = "UNKNOWN";
      break;
    }
    return fmt::format_to(ctx.out(), "{}", typeStr);
  }
};

#endif // CORE_EVENTS