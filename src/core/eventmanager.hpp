#pragma once 

#ifndef CORE_EVENTMANAGER
#define CORE_EVENTMANAGER

#include "utils.hpp"

class EventManager{

  public:
    EventManager() = delete;
    ~EventManager() = delete;

    


  private:
    static std::thread eventThread;

};

#endif // CORE_EVENTMANAGER