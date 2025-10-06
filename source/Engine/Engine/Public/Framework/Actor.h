#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Render/Public/RenderAPI.h"
#include "Engine/Utils/Public/Object/Object.h"
namespace yjw
{
    class Actor : public MObject
    {
    public:
        Actor() {}
        ~Actor() {}

        void SetActorId(int id) { m_id = id; }
        void SetName(const char* name) { m_name = name; }
        void SetTransform(const Transform& transform)
        {
            m_transform = transform;
            rdUpdateEntityTransform(m_world->GetScene(), GetSceneEntity(), m_transform);
        }
        int GetActorId() const { return m_id; };
        const char* GetName() const { return m_name.c_str(); };
        Transform GetTransform() { return m_transform; }
        virtual void OnSpawned() {}
        void SetSceneEntity(RdEntityPtr handle) { m_entity_handle = handle; }
        RdEntityPtr GetSceneEntity() { return m_entity_handle; }
        void SetWorld(World* world) { m_world = world; }
        World* GetWorld() { return m_world; }

        template<typename T>
        T* AddComponent() {
            T* res = new T();
            m_components.push_back(res);
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
        World* m_world{};
        RdEntityPtr m_entity_handle{};
        Transform m_transform{};
        std::string m_name;
        int m_id{};

    public:
        std::vector<Component*> m_components;

    public:
        void Serialize(class Archive& Ar)
        {
            Ar << m_transform;
        }
    };

    class MeshActor : public Actor
    {
    public:
        MeshActor(const char* name)
        {
            m_primitive_url = name;
        }
        virtual void OnSpawned()
        {
            m_components.push_back(new StaticMeshComponent());
            ((StaticMeshComponent*)m_components[0])->SetPrimitive(m_primitive_url.c_str());
        }
    private:
        std::string m_primitive_url{};
    };

    class TestBoxActor : public Actor
    {
    public:
        virtual void OnSpawned()
        {
            m_components.push_back(new StaticMeshComponent());
            ((StaticMeshComponent*)m_components[0])->SetPrimitive("naxita/纳西妲.mesh.ast");
        }
    };
}
