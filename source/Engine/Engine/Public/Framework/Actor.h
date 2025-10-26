#pragma once
#include "Engine/Utils/Public/Object/Object.h"
#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"
#include "Engine/Engine/Public/Framework/World.h"
#include "Engine/Render/Public/RenderAPI.h"
namespace yjw
{
    class Meta() Actor : public MObject
    {
        GENERATED_BODY();
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
        virtual void OnSpawned()
        {
            m_components.push_back(new StaticMeshComponent());
            ((StaticMeshComponent*)m_components[0])->SetPrimitive(m_primitive_url.c_str());

            RdEntityPtr entity_handle = rdAddEntity(GetWorld()->GetScene());
            SetSceneEntity(entity_handle);
            RdGeometryPtr mesh_handle = rdCreateGeometry(((StaticMeshComponent*)m_components[0])->GetPrimitive());
            rdUpdateEntityGeometry(GetWorld()->GetScene(), entity_handle, mesh_handle);
            rdUpdateEntityTransform(GetWorld()->GetScene(), entity_handle, GetTransform());
            rdUpdateEntityPickFlag(GetWorld()->GetScene(), entity_handle, GetActorId());
        }
    private:
        std::string m_primitive_url{};
    };

    class Meta() TestBoxActor : public Actor
    {
        GENERATED_BODY();
    public:
        TestBoxActor() {}
        virtual void OnSpawned()
        {
            m_components.push_back(new StaticMeshComponent());
            ((StaticMeshComponent*)m_components[0])->SetPrimitive("naxita/纳西妲.mesh.ast");

            RdEntityPtr entity_handle = rdAddEntity(GetWorld()->GetScene());
            SetSceneEntity(entity_handle);
            RdGeometryPtr mesh_handle = rdCreateGeometry(((StaticMeshComponent*)m_components[0])->GetPrimitive());
            rdUpdateEntityGeometry(GetWorld()->GetScene(), entity_handle, mesh_handle);
            rdUpdateEntityTransform(GetWorld()->GetScene(), entity_handle, GetTransform());
            rdUpdateEntityPickFlag(GetWorld()->GetScene(), entity_handle, GetActorId());
        }
    };
}
