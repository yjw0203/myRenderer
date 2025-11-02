#pragma once
#include "Engine/Utils/Public/DesignPatterns/Singleton.h"
#include "Engine/Engine/Public/Core/EngineObject.h"
#include <vector>

class EngineEvent
{
public:
    virtual ~EngineEvent() {}
};

class EngineEventManagerImplement
{
public:
    void Process();
    template<typename Event,typename... Args>
    void AddEvent(Args&&... args)
    {
        m_events.push_back(new Event(std::forward<Args>(args)...));
    }

private:
    std::vector<EngineEvent*> m_events;
};

typedef Singleton<EngineEventManagerImplement> EngineEventManager;
