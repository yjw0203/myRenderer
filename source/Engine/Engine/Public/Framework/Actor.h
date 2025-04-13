#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"
#include "Engine/Render/Private/Mesh.h"
#include "Engine/Engine/Public/Framework/Level.h"
#include "Engine/Engine/Public/Framework/Render/Entity.h"
namespace yjw
{
    class Actor
    {
    public:
        Actor() {}
        ~Actor() {}
        void SetActorId(int id) { m_id = id; }
        void SetName(const char* name) { m_name = name; }
        void SetEntity(Entity entity) { m_entity = entity; }
        int GetActorId() const { return m_id; };
        const char* GetName() const { return m_name.c_str(); };
        Entity GetEntity() { return m_entity; }
        virtual void OnSpawned() {}
        void SetSceneEntity(EntityHandle handle) { m_entity_handle = handle; }
        EntityHandle GetSceneEntity() { return m_entity_handle; }
    private:

        EntityHandle m_entity_handle{};

        Entity m_entity{};
        std::string m_name;
        int m_id{};
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
            GetEntity().AddComponent<StaticMeshComponent>()->SetPrimitive(m_primitive_url.c_str());
        }
    private:
        std::string m_primitive_url{};
    };

    class TestBoxActor : public Actor
    {
    public:
        virtual void OnSpawned()
        {
            GetEntity().AddComponent<StaticMeshComponent>()->SetPrimitive("naxita/纳西妲.mesh.ast");
        }
    };
}
