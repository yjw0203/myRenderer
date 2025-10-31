#pragma once
#include "Engine/Utils/Public/Object/Object.h"
#include "Engine/Engine/Public/Framework/Components/SceneRootComponent.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Render/Public/RenderAPI.h"
namespace yjw
{
    class Component;
    class Level;
    class World;
    class Meta() Actor : public MObject
    {
        GENERATED_BODY();
    public:
        Actor();
        ~Actor() {}

        void SetActorId(int id) { m_id = id; }
        void SetName(const char* name) { m_name = name; }
        void SetTransform(const Transform& transform)
        {
            m_root_component.SetRelativeTransform(transform);
        }
        int GetActorId() const { return m_id; };
        const char* GetName() const { return m_name.c_str(); };
        Transform GetTransform() { return m_root_component.GetTransform(); }
        virtual void OnSpawned() {}
        World* GetWorld();
        Level* GetLevel();

        virtual void OnLoaded();
        virtual void AttachToLevel(Level* level);
        virtual void DettachToLevel();

        template<typename T>
        T* AddComponent() {
            T* res = new T();
            m_components.push_back(res);
            res->SetActor(this);
            return res;
        }

        template<typename T>
        T* GetComponent() {
            for (Component* component : m_components)
            {
                if (T* ptr = component->CastTo<T>())
                {
                    return ptr;
                }
            }
            return nullptr;
        }
    private:
        Level* m_level{};
        
        int m_id{};

        Meta()
        SceneRootComponent m_root_component;

        Meta()
        std::string m_name;

    public:
        Meta()
        std::vector<Component*> m_components;
    };

    class Meta() MeshActor : public Actor
    {
        GENERATED_BODY();
    public:
        MeshActor() {}
        MeshActor(const char* name)
        {
            m_primitive_url = name;
        }
        virtual void OnSpawned() override;
    private:
        Meta()
        std::string m_primitive_url{};
    };
}
