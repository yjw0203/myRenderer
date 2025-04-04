#include <iostream>
#include "Engine/Engine/Public/Asset/Mesh.h"
#include "Engine/Engine/Public/Asset/Material.h"
#include "Engine/Model/Public/yjw_model.h"
#include "generate1/projectInfo.h"
#include <filesystem>

namespace yjw
{
void ImportTexture(const std::string& out_name, const std::string& texture_path)
{
    std::string abs_path = RESOURCE_PATH + std::string("/") + out_name;
    std::filesystem::copy(texture_path, abs_path, std::filesystem::copy_options::recursive);
}

void ImportMaterial(Asset<MaterialAST>& ast, CPUModel::Material& mat)
{
    MaterialAST* mat_ptr = ast.GetData();
    mat_ptr->m_shader = mat.m_shader;
    mat_ptr->m_entry = mat.m_entry;
    ast.Save();
}

void ImportMaterialInstance(Asset<MaterialInstanceAST>& ast, CPUModel::MaterialIns& mat)
{
    MaterialInstanceAST* mat_ptr = ast.GetData();
    mat_ptr->m_float_params = mat.m_float_params;
    mat_ptr->m_texture_params = mat.m_decided_texture_params;
    mat_ptr->m_material_template.m_url = mat.m_imported_mat_name;
    ast.Save();
}

void ImportModel(const std::string& out_path, const std::string& in_path)
{
    std::shared_ptr<Model> model = *Model::load(in_path);

    if (!model)return;

    std::string name = out_path;

    std::vector<std::string> mat_names;
    for (int i = 0; i < model->m_cpu_model->m_material.size(); i++)
    {
        std::string mat_name = name + std::to_string(i) + ".material.ast";
        Asset<MaterialAST> mat_ast(mat_name.c_str());
        ImportMaterial(mat_ast, model->m_cpu_model->m_material[i]);
        mat_names.push_back(mat_name);
    }

    for (int i = 0; i < model->m_cpu_model->m_pool_textures.size(); i++)
    {
        std::string texture_name = name + std::to_string(i) + ".png";
        ImportTexture(texture_name, model->m_cpu_model->m_pool_textures[i].texture_name);
        model->m_cpu_model->m_pool_textures[i].m_imported_name = texture_name;
    }

    std::vector<std::string> mat_ins_names;
    for (int i = 0; i < model->m_cpu_model->m_material_instances.size(); i++)
    {
        CPUModel::MaterialIns& mat_ins = model->m_cpu_model->m_material_instances[i];
        mat_ins.m_imported_mat_name = mat_names[mat_ins.material_id];
        for (auto param : mat_ins.m_texture_params)
        {
            mat_ins.m_decided_texture_params[param.first] = model->m_cpu_model->m_pool_textures[param.second].m_imported_name;
        }
        std::string mat_ins_name = name + std::to_string(i) + ".material_instance.ast";
        Asset<MaterialInstanceAST> mat_ins_ast(mat_ins_name.c_str());
        ImportMaterialInstance(mat_ins_ast, mat_ins);
        mat_ins_names.push_back(mat_ins_name);
    }

    std::string mesh_name = name + ".mesh.ast";
    Asset<MeshAST> mesh_ast(mesh_name.c_str());
    MeshAST* mesh = mesh_ast.GetData();
    for (int i = 0; i < mat_ins_names.size(); i++)
    {
        AssetReferece<MaterialInstanceAST> ref{};
        ref.m_url = mat_ins_names[i];
        mesh->m_materials[std::to_string(i)] = ref;
    }
    
    for (CPUModel::CPUBuffer& buffer : model->m_cpu_model->m_pool_buffers)
    {
        if (buffer.type == MeshVertexType::INDEX)
        {
            mesh->m_index_buffer = RawBuffer(buffer.buffer.begin(),buffer.buffer.end());
        }
        else
        {
            MeshVertexBuffer vex_buffer{};
            vex_buffer.m_type = buffer.type;
            vex_buffer.m_vertexes = RawBuffer(buffer.buffer.begin(), buffer.buffer.end());
            mesh->m_vertex_buffers.push_back(vex_buffer);
        }
    }
    for (CPUModel::Entity& entity : model->m_cpu_model->m_entities)
    {
        CPUModel::CPUMesh& cpu_mesh = model->m_cpu_model->m_meshes[entity.mesh_id];
        SubMeshInfo info{};
        info.m_start_index = cpu_mesh.first_index;
        info.m_index_count = cpu_mesh.index_count;
        info.m_material_slot = std::to_string(entity.material_ins_id);
        mesh->m_sub_meshes.push_back(info);
        mesh->m_is_index_short = cpu_mesh.is_indices_16bit;
    }
    mesh_ast.Save();
}
}

/*
int main()
{
    ImportModel("naxita/草神", "E:/workspace/素材/cao/纳西妲.pmx");
    return 0;
}
*/
