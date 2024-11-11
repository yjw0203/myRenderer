#include "yjw_scene.h"

namespace yjw
{
    /*
    std::vector<RenderEntity> Scene::buildEntitys()
    {
        std::vector<RenderEntity> res;
        for (std::weak_ptr<Model>& model : models)
        {
            std::shared_ptr<Model> model_ptr = model.lock();
            if (model_ptr)
            {
                for (int entity_id = 0; entity_id < model_ptr->m_gpu_model->GetEntityCount(); entity_id++)
                {
                    RenderEntity entity{};
                    entity.m_material = model_ptr->m_gpu_model->GetMaterial(entity_id);
                    entity.m_resource_binding = entity.m_material->GetResourceBinding();
                    entity.m_primitive_binding = rpi::RPICreatePrimitiveBinding(entity.m_material->GetPipeline());
                    GPUModel::GPUMesh* mesh = model_ptr->m_gpu_model->GetGPUMesh(entity_id);
                    for (GPUModel::VertexBuffer& vertexBuffer : mesh->vertex_buffers)
                    {
                        entity.m_primitive_binding.SetVertexBuffer(CastToRHIName(vertexBuffer.type), vertexBuffer.buffer);
                    }
                    entity.m_primitive_binding.SetIndexBuffer(mesh->index_buffer, mesh->first_index, mesh->index_count, mesh->is_indices_16bit);
                    res.push_back(entity);
                }
            }
        }
        return res;
    }*/
}