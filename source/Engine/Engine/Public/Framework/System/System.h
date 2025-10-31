#pragma once
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Utils/Public/Object/Object.h"

namespace yjw
{
    class Meta() System : public MObject
    {
        GENERATED_BODY()
    public:
        System() {}
        virtual ~System() {};
        void OnLoaded() {}

        virtual void Update(float deltaTime) {};
        Level* GetLevel() { return m_level; };
        void AttachToLevel(Level* level) { m_level = level; }
        void DettachToLevel() { m_level = nullptr; }

    private:
        Level* m_level = nullptr;
    };
}