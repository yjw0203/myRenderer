#pragma once
#include <vector>
#include "Engine/Math/Public/Math.h"
#include "Engine/Utils/Public/Object/Object.h"

namespace yjw
{
    class Level;
    class Actor;
    class RdScene;
    class Meta() Component : public MObject
    {
        GENERATED_BODY();
    public:
        virtual ~Component() {};

        template<typename T>
        T* CastTo()
        {
            // to optimize using type reflection
            return dynamic_cast<T*>(this);
        }
        virtual void OnLoaded() {};

        virtual void AttachToLevel(Level* level) {};
        virtual void DettachToLevel() {};
        virtual void AttachToScene(RdScene* scene) {};
        virtual void DettachToScene() {};
        void SetActor(Actor* actor) { m_actor = actor; }
        Actor* GetActor() { return m_actor; }

    private:
        Actor* m_actor = nullptr;
    };
}
