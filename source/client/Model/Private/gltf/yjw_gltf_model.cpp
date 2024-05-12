#include "yjw_gltf_model.h"
#include "tiny_gltf.h"
#include <memory>
#include <iostream>

namespace yjw
{
    bool LoadGLTFModel(tinygltf::Model& model, const char* filename)
    {
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        if (!warn.empty()) {
            std::cout << "WARN: " << warn << std::endl;
        }

        if (!err.empty()) {
            std::cout << "ERR: " << err << std::endl;
        }

        if (!res)
            std::cout << "Failed to load glTF: " << filename << std::endl;
        else
            std::cout << "Loaded glTF: " << filename << std::endl;

        return res;
    }

    void GLTFModelBuilder::FillModelData(const std::string& filePath, const std::string& fileName)
    {
        tinygltf::Model gltfModel;
        std::string holePath = filePath + "/" + fileName;
        LoadGLTFModel(gltfModel, holePath.c_str());


    }
}