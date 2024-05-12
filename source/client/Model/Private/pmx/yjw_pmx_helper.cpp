#include "Private/pmx/yjw_pmx_helper.h"
#include "Utils/yjw_unicode_util.h"
#include "File/yjw_file_system_module_header.h"
#include <fstream>
#include <sstream>

namespace yjw
{
    class PMXFileStream
    {
    public:
        PMXFileStream(FileBlob& file, bool isUTF8 = true) :file(file), isUTF8(isUTF8) {};

        template<typename T>
        PMXFileStream& operator >> (T& data)
        {
            file.Read((char*) & data, sizeof(data));
            return *this;
        }

        template<int Size>
        PMXFileStream& operator >> (PMXString<Size>& str)
        {
            file.Read((char*)str.str, Size);
            str.str[Size] = '\0';
            return *this;
        }

        template<>
        PMXFileStream& operator >> (std::string& str)
        {
            uint32_t bufSize;
            file.Read((char*) & bufSize, 4);
            if (bufSize > 0)
            {
                if (isUTF8)
                {
                    std::string utf8Str(bufSize, '\0');
                    file.Read(&utf8Str[0], bufSize);
                    str = utf8Str;

                }
                else //UTF16
                {
                    std::u16string utf16Str(bufSize / 2, u'\0');
                    file.Read((char*) &utf16Str[0], bufSize);
                    ConvU16ToU8(utf16Str, str);

                    std::u16string w;
                    ConvU8ToU16(str, w);
                }
            }
            return *this;
        }

    private:
        FileBlob& file;
        bool isUTF8 = true;
    };

    class PMXFileStreamSpecific
    {
    public:
        PMXFileStreamSpecific(FileBlob& file,int specificSize) :file(file), specificSize(specificSize) {};

        template<typename T>
        PMXFileStreamSpecific& operator >> (T data)
        {
            file.Read((char*)data, specificSize);
            return *this;
        }

        PMXFileStreamSpecific& operator >> (int32_t& data)
        {
            if (specificSize == 1)
            {
                uint8_t buffer;
                file.Read((char*)&buffer, specificSize);
                if (buffer == 0xFF)
                {
                    data = -1;
                }
                else
                {
                    data = buffer;
                }
            }
            else if (specificSize == 2)
            {
                uint16_t buffer;
                file.Read((char*)&buffer, specificSize);
                if (buffer == 0xFFFF)
                {
                    data = -1;
                }
                else
                {
                    data = buffer;
                }
            }
            else if (specificSize == 4)
            {
                file.Read((char*)&data, specificSize);
            }
            return *this;
        }

    private:
        FileBlob& file;
        int specificSize;
    };

    namespace PMXBase
    {
        void readHeader(FileBlob& file, PMXModel* model)
        {
            PMXHeader& h = model->m_header;

            PMXFileStream stream(file);
            stream >> h.m_magic >> h.m_version;
            stream >> h.m_dataSize >> h.m_encode;
            stream >> h.m_addUVNum;

            stream >> h.m_vertexIndexSize;
            stream >> h.m_textureIndexSize;
            stream >> h.m_materialIndexSize;
            stream >> h.m_boneIndexSize;
            stream >> h.m_morphIndexSize;
            stream >> h.m_rigidbodyIndexSize;
        }

        void readInfo(FileBlob& file, PMXModel* model)
        {
            PMXInfo& i = model->m_info;
            PMXFileStream stream(file, model->m_header.m_encode);
            stream >> i.m_modelName;
            stream >> i.m_englishModelName;
            stream >> i.m_comment;
            stream >> i.m_englishComment;
        }

        void readVertex(FileBlob& file, PMXModel* model)
        {
            uint32_t vertexCount;
            PMXFileStream stream(file);

            stream >> vertexCount;

            auto& vertices = model->m_vertices;
            vertices.resize(vertexCount);
            for (int index = 0; index < vertexCount; index++)
            {
                stream >> vertices.m_position[index];
                stream >> vertices.m_normal[index];
                stream >> vertices.m_uv[index];

                for (uint8_t i = 0; i < model->m_header.m_addUVNum; i++)
                {
                    stream >> vertices.m_addUV[i][index];
                }

                stream >> vertices.m_weightType[index];


                PMXFileStreamSpecific specStream(file, model->m_header.m_boneIndexSize);

                switch (vertices.m_weightType[index])
                {
                case PMXVertexWeight::BDEF1:
                    specStream >> vertices.m_boneIndices[index].data[0];
                    break;
                case PMXVertexWeight::BDEF2:
                    specStream >> vertices.m_boneIndices[index].data[0];
                    specStream >> vertices.m_boneIndices[index].data[1];
                    stream >> vertices.m_boneWeights[index][0];
                    break;
                case PMXVertexWeight::BDEF4:
                    specStream >> vertices.m_boneIndices[index].data[0];
                    specStream >> vertices.m_boneIndices[index].data[1];
                    specStream >> vertices.m_boneIndices[index].data[2];
                    specStream >> vertices.m_boneIndices[index].data[3];
                    stream >> vertices.m_boneWeights[index][0];
                    stream >> vertices.m_boneWeights[index][1];
                    stream >> vertices.m_boneWeights[index][2];
                    stream >> vertices.m_boneWeights[index][3];
                    break;
                case PMXVertexWeight::SDEF:
                    specStream >> vertices.m_boneIndices[index].data[0];
                    specStream >> vertices.m_boneIndices[index].data[1];
                    stream >> vertices.m_boneWeights[index][0];
                    stream >> vertices.m_sdefC[index];
                    stream >> vertices.m_sdefR0[index];
                    stream >> vertices.m_sdefR1[index];
                    break;
                case PMXVertexWeight::QDEF:
                    specStream >> vertices.m_boneIndices[index].data[0];
                    specStream >> vertices.m_boneIndices[index].data[1];
                    specStream >> vertices.m_boneIndices[index].data[2];
                    specStream >> vertices.m_boneIndices[index].data[3];
                    stream >> vertices.m_boneWeights[index][0];
                    stream >> vertices.m_boneWeights[index][1];
                    stream >> vertices.m_boneWeights[index][2];
                    stream >> vertices.m_boneWeights[index][3];
                    break;
                }
                stream >> vertices.m_edgeMag[index];
            }
        }

        void readIndex(FileBlob& file, PMXModel* model)
        {
            PMXFileStream stream(file);
            int32_t indexCount = 0;
            stream >> indexCount;

            model->m_indices.resize(indexCount);

            PMXFileStreamSpecific specStream(file, model->m_header.m_vertexIndexSize * indexCount);
            switch (model->m_header.m_vertexIndexSize)
            {
            case 1:
            {
                std::vector<uint8_t> vertices(indexCount);
                specStream >> vertices.data();
                for (int32_t faceIdx = 0; faceIdx < indexCount; faceIdx++)
                {
                    model->m_indices[faceIdx] = vertices[faceIdx];
                }
            }
            break;
            case 2:
            {
                std::vector<uint16_t> vertices(indexCount);
                specStream >> vertices.data();
                for (int32_t faceIdx = 0; faceIdx < indexCount; faceIdx++)
                {
                    model->m_indices[faceIdx] = vertices[faceIdx];
                }
            }
            break;
            case 4:
            {
                std::vector<uint32_t> vertices(indexCount);
                specStream >> vertices.data();
                for (int32_t faceIdx = 0; faceIdx < indexCount; faceIdx++)
                {
                    model->m_indices[faceIdx] = vertices[faceIdx];
                }
            }
            break;
            }
        }

        void readTexture(FileBlob& file, PMXModel* model)
        {
            PMXFileStream stream(file, model->m_header.m_encode);
            int32_t texCount = 0;
            stream >> texCount;
            model->m_textures.resize(texCount);
            for (auto& tex : model->m_textures)
            {
                stream >> tex.m_textureName;
            }
        }

        void readMaterial(FileBlob& file, PMXModel* model)
        {
            PMXFileStream stream(file, model->m_header.m_encode);
            PMXFileStreamSpecific specStream(file, model->m_header.m_textureIndexSize);
            int32_t matCount = 0;
            stream >> matCount;

            model->m_materials.resize(matCount);

            for (auto& mat : model->m_materials)
            {
                stream >> mat.m_name;
                stream >> mat.m_englishName;

                stream >> mat.m_diffuse;
                stream >> mat.m_specular;
                stream >> mat.m_specularPower;
                stream >> mat.m_ambient;

                stream >> mat.m_drawMode;
                stream >> mat.m_edgeColor;
                stream >> mat.m_edgeSize;

                specStream >> mat.m_textureIndex;
                specStream >> mat.m_sphereTextureIndex;

                stream >> mat.m_sphereMode;
                stream >> mat.m_toonMode;

                if (mat.m_toonMode == PMXToonMode::Separate)
                {
                    specStream >> mat.m_toonTextureIndex;
                }
                else if (mat.m_toonMode == PMXToonMode::Common)
                {
                    uint8_t toonIndex;
                    stream >> toonIndex;
                    mat.m_toonTextureIndex = (int32_t)toonIndex;
                }
                else
                {
                    //error
                }

                stream >> mat.m_memo;
                stream >> mat.m_numFaceVertices;
            }
        }
    }
    class PMXModelLoader
    {
    public:
        PMXModelLoader(const char* filePath)
        {
            FileReader reader = FileSystemModule::Get()->GetFileReader();
            FileBlob blob = reader.ReadHoleFile(filePath);
            PMXBase::readHeader(blob, &model);
            PMXBase::readInfo(blob, &model);
            PMXBase::readVertex(blob, &model);
            PMXBase::readIndex(blob, &model);
            PMXBase::readTexture(blob, &model);
            PMXBase::readMaterial(blob, &model);

            //todo

            blob.Release();
        }
        operator PMXModel() const
        {
            return std::move(model);
        }
    private:
        PMXModel model{};
    };


    PMXModel PMXModel::load(const char* filePath)
    {
        std::unique_ptr<PMXModelLoader> builder = std::make_unique<PMXModelLoader>(filePath);
        return *builder;
    }
}