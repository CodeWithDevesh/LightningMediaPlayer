#pragma once

#ifndef CORE_EVENTS
#define CORE_EVENTS

#include "utils.hpp"
#include "fmt/format.h"

namespace lighter
{

  namespace Events
  {
    enum EventType
    {
      MEDIA_ADDED,
      MEDIA_REMOVED,
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
    default:
      typeStr = "UNKNOWN";
      break;
    }
    return fmt::format_to(ctx.out(), "{}", typeStr);
  }
};

#endif // CORE_EVENTS