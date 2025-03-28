#pragma once

#ifndef CORE_EVENTMANAGER
#define CORE_EVENTMANAGER

#include "lightning_utils.hpp"
#include "lightning_events.hpp"

namespace lighter
{
  class EventManager
  {
    using Callback = std::function<void(std::any)>;

  public:
    EventManager();
    ~EventManager();

    int registerListner(Events::EventType type, Callback callback);
    void unregisterListner(Events::EventType type, int eventId);
    void triggerEvent(Events::EventType type, std::any data);

  private:
    std::map<Events::EventType, std::map<int, Callback>> eventCallbacks;
    std::atomic<int> listnerId = 0;

    std::queue<std::pair<Events::EventType, std::any>> eventQueue;
    std::thread eventThread;
    std::condition_variable cv;
    std::mutex mtx;

    std::atomic<bool> running = true;
    void processEvents();
  };
}

#endif // CORE_EVENTMANAG