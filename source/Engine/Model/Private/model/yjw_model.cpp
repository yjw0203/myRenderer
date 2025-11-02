#include "Engine/Model/Public/yjw_model.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Engine/Utils/Public/yjw_unicode_util.h"
#include "projectInfo.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_header.h"
#include <glm/glm.hpp>
#include "Engine/Model/Private/model/yjw_pmx_model.h"
#include "Engine/Model/Private/model/yjw_gltf_model.h"
#include <filesystem>

std::unique_ptr<ModelBuilder> Model::load(std::string filePath)
{
    ModelFileFormat format{};
    std::filesystem::path path(filePath);
    std::string extension = path.extension().string();
    if (extension == ".pmx")
    {
        format = ModelFileFormat::model_file_format_pmx;
    }
    else if (extension == ".glb")
    {
        format = ModelFileFormat::model_file_format_glb;
    }
    else if (extension == ".gltf")
    {
        format = ModelFileFormat::model_file_format_gltf;
    }
    else
    {
        return nullptr;
    }

    std::string dir_path = path.parent_path().string();
    std::string file_name = path.filename().string();
    std::unique_ptr<ModelBuilder> builder = std::make_unique<ModelBuilder>();
    builder->build(dir_path, file_name, format);
    return builder;
}

void ModelBuilder::build(std::string filePath, std::string fileName, ModelFileFormat format)
{
    model = std::make_shared<Model>();
    if (format == model_file_format_pmx)
    {
        PMXModelBuilder builder;
        model->m_cpu_model = builder.Build(filePath, fileName);
    }
    else if (format == model_file_format_gltf)
    {
        GLTFModelBuilder builder(false);
        model->m_cpu_model = builder.Build(filePath, fileName);
    }
    else if (format == model_file_format_glb)
    {
        GLTFModelBuilder builder(true);
        model->m_cpu_model = builder.Build(filePath, fileName);
    }
}
