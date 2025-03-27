#include "eventmanager.hpp"

namespace lighter
{

  EventManager::EventManager()
  {
    INFO("Initializing EventManager...");
    INFO("EventManager initialized successfully.");
  }

  EventManager::~EventManager()
  {
    INFO("Uninitializing EventManager");
    INFO("EventManager Uninitialized");
  }

  int EventManager::registerListner(Events::EventType type, Callback callback)
  {
    try
    {
      eventCallbacks[type][listnerId] = callback;
      DEBUG("Event registered for {} with ID {}", type, listnerId.load());
      return listnerId++;
    }
    catch (const std::exception &e)
    {
      ERROR(e.what());
      return -1;
    }
  }

  void EventManager::unregisterListner(Events::EventType type, int id)
  {
    DEBUG("Removing Event registered for {} with ID {}", type, id);
    if (eventCallbacks[type].erase(id) == 0)
      ERROR("Event ID {} not found...", id);
  }

  void EventManager::triggerEvent(Events::EventType type, std::any data)
  {
    if (eventCallbacks.find(type) != eventCallbacks.end())
    {
      DEBUG("Triggered event {}", type);
      for (auto &[id, callback] : eventCallbacks[type])
        callback(data);
    }
  }

} // namespace lighter