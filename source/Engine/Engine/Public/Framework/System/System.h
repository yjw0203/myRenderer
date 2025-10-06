#pragma once
#include "Engine/Engine/Public/Framework/Level.h"

namespace yjw
{
    class System
    {
    public:
        virtual ~System() = default;
        virtual void Update(float deltaTime) = 0;
        Level* GetLevel() { return m_level; };
        void RegisterToLevel(Level* level) { m_level = level; }

    private:
        Level* m_level = nullptr;
    };
}