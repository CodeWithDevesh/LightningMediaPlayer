#pragma once

#ifndef CORE_EVENTMANAGER
#define CORE_EVENTMANAGER

#include "utils.hpp"
#include "events.hpp"

namespace lighter
{
  class EventManager
  {
    using Callback = std::function<void(std::any)>;

  public:
    EventManager();
    ~EventManager();

    int registerListner(Events::EventType type, Callback callback);
    void unregisterListner(Events::EventType type , int eventId);
    void triggerEvent(Events::EventType type, std::any data);

  private:
    std::thread eventThread;
    std::map<Events::EventType, std::map<int, Callback>> eventCallbacks;
    std::atomic<int> listnerId = 0;
  };
}

#endif // CORE_EVENTMANAG