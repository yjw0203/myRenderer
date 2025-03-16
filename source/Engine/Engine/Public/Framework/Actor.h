#pragma once
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Engine/Public/Framework/Components/StaticMeshComponent.h"
#include "Engine/Engine/Public/Render/Mesh.h"

namespace yjw
{
    class Actor
    {
    public:
        Actor() {}
        ~Actor() {}
        void SetName(const char* name) { m_name = name; }
        void SetEntity(Entity entity) { m_entity = entity; }
        const char* GetName() const { return m_name.c_str(); };
        Entity GetEntity() { return m_entity; }
        virtual void OnSpawned() {}
    private:
        Entity m_entity{};
        std::string m_name;
    };

    class TestBoxActor : public Actor
    {
    public:
        virtual void OnSpawned()
        {
            MaterialInstance* material_instance = new MaterialInstance(&g_simple_mesh_pbr_material);
            rpi::RPITexture texture = rpi::RPICreateTexture2DFromFile(RESOURCE_FILE(cao/tex/髮1.png));
            material_instance->SetTexture("albedoTex", texture);
            Mesh* mesh = new Mesh(&g_box_primitive, material_instance);
            GetEntity().AddComponent<StaticMeshComponent>()->SetMesh(mesh);
        }
    };
}
