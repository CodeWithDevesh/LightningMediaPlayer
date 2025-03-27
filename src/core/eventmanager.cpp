#include "eventmanager.hpp"

namespace lighter
{

  EventManager::EventManager()
  {
    INFO("Initializing EventManager...");
    eventThread = std::thread(&EventManager::processEvents, this);
    INFO("EventManager initialized successfully.");
  }

  void EventManager::processEvents()
  {
    while (running)
    {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [&]
              { return !eventQueue.empty() || !running; }); // Wait till eventQueue is not empty or not running

      if (!running)
      {
        DEBUG("Event thread shutting down");
        lock.unlock();
        break;
      }

      auto [type, data] = eventQueue.front();
      eventQueue.pop();
      lock.unlock();

      if (eventCallbacks.find(type) != eventCallbacks.end())
      {
        DEBUG("Processing event {}", type);
        for (auto &[id, callback] : eventCallbacks[type])
          callback(data);
      }
    }
  }

  EventManager::~EventManager()
  {
    INFO("Uninitializing EventManager");
    running = false;
    DEBUG("Running set to false");
    DEBUG("notifying event thread");
    cv.notify_one();

    DEBUG("Joining event thread");
    if (eventThread.joinable()){
      eventThread.join();
      DEBUG("Event thread joined");
    }
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
    DEBUG("Triggered event {}", type);
    {
      std::lock_guard<std::mutex> lock(mtx);
      eventQueue.emplace(type, data);
    }
    cv.notify_one();
  }
} // namespace lighter