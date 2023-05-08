#include "yjw_mesh.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
namespace yjw
{
    std::unique_ptr<MeshBuilder> Mesh::load(std::string filePath)
    {
        std::unique_ptr<MeshBuilder> builder = std::make_unique<MeshBuilder>();
        builder->build(std::move(filePath));
        return builder;
    }

    void MeshBuilder::build(std::string filePath)
    {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file." << std::endl;
            throw;
        }

        // Read file header
        char header[4];
        int version;
        char columSize;
        char encoding;
        char addUVNum;
        file.read(header, 4);
        file.read((char*)&version, sizeof(int));
        file.read(&columSize, sizeof(char));
        file.read(&encoding, sizeof(char));
        file.read(&addUVNum, sizeof(char));

        char indexSize[6];
        file.read(indexSize, 6);

        char modelName[4];
        char modelNameEn[4];
        char hint[4];
        char hintEn[4];
        file.read(modelName, 4);
        file.read(modelNameEn, 4);
        file.read(hint, 4);
        file.read(hintEn, 4);
        
        // Read info 
        for (int i = 0; i < 2; i++)
        {
            int stringSize;
            file.read((char*)&stringSize, 4);
            if (encoding)
            {
                std::string utf8Str(stringSize, '\0');
                file.read(&utf8Str[0], stringSize);
            }
            else
            {
                std::u16string utf16Str(stringSize / 2, u'\0');
                file.read((char*)&utf16Str[0], stringSize);
            }
        }


        // Read vertex count
        int vertexCount;
        file.read((char*)&vertexCount, sizeof(int));

        // Read vertices
        mesh.vertices.resize(vertexCount);
        for (int i = 0; i < vertexCount; i++) {
            file.read((char*)&mesh.vertices[i].x, sizeof(float));
            file.read((char*)&mesh.vertices[i].y, sizeof(float));
            file.read((char*)&mesh.vertices[i].z, sizeof(float));
            file.read((char*)&mesh.vertices[i].nx, sizeof(float));
            file.read((char*)&mesh.vertices[i].ny, sizeof(float));
            file.read((char*)&mesh.vertices[i].nz, sizeof(float));
            file.read((char*)&mesh.vertices[i].u, sizeof(float));
            file.read((char*)&mesh.vertices[i].v, sizeof(float));

            for (uint8_t j = 0; j < addUVNum; j++)
            {
                glm::vec4 addUv;
                file.read((char*) & addUv, sizeof(addUv));
            }

            char weightType;
            file.read(&weightType, sizeof(char));
            if (weightType == 0)
            {
                int data[1];
                file.read((char*)data, indexSize[3]);
            }
            else if (weightType == 1)
            {
                int data[3];
                file.read((char*)data, 2*indexSize[3]+4);
            }
            else if (weightType == 2)
            {
                int data[8];
                file.read((char*)data, 4*indexSize[3]+16);
            }
            else if (weightType == 3)
            {
                int data[15];
                file.read((char*)data, 2*indexSize[3]+4+3*4*4);
            }
            else if (weightType == 4)
            {
                int data[8];
                file.read((char*)data, 4*indexSize[3]+16);
            }

            float edgeMag;
            file.read((char*)&edgeMag, 4);
        }

        int32_t faceCount = 0;
        file.read((char*)&faceCount, 4);
        mesh.indices.resize(faceCount);
        for (int i = 0; i < faceCount; i++)
        {
            if (indexSize[0] == 1)
            {
                uint8_t index;
                file.read((char*)&index, 1);
                mesh.indices[i] = index;
            }
            else if (indexSize[0] == 2)
            {
                uint16_t index;
                file.read((char*)&index, 2);
                mesh.indices[i] = index;
            }
            else if (indexSize[0] == 4)
            {
                uint32_t index;
                file.read((char*)&index, 4);
                mesh.indices[i] = index;
            }
        }

    }

}