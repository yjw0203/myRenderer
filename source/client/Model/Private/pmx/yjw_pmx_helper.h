#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>
namespace yjw
{
    template<int size>
    struct PMXString
    {
        char str[size + 1];
    };

    struct PMXHeader
    {
        PMXString<4> m_magic;
        float   m_version;

        uint8_t    m_dataSize;

        uint8_t    m_encode;    //0:UTF16 1:UTF8
        uint8_t    m_addUVNum;

        uint8_t    m_vertexIndexSize;
        uint8_t    m_textureIndexSize;
        uint8_t    m_materialIndexSize;
        uint8_t    m_boneIndexSize;
        uint8_t    m_morphIndexSize;
        uint8_t    m_rigidbodyIndexSize;
    };

    struct PMXInfo
    {
        std::string    m_modelName;
        std::string    m_englishModelName;
        std::string    m_comment;
        std::string    m_englishComment;
    };

    struct PMXTexture
    {
        std::string m_textureName;
    };

    enum class PMXDrawModeFlags : uint8_t
    {
        BothFace = 0x01,
        GroundShadow = 0x02,
        CastSelfShadow = 0x04,
        RecieveSelfShadow = 0x08,
        DrawEdge = 0x10,
        VertexColor = 0x20,
        DrawPoint = 0x40,
        DrawLine = 0x80,
    };

    enum class PMXSphereMode : uint8_t
    {
        None,
        Mul,
        Add,
        SubTexture,
    };

    enum class PMXToonMode : uint8_t
    {
        Separate,
        Common,
    };

    struct PMXMaterial
    {
        std::string    m_name;
        std::string    m_englishName;

        glm::vec4    m_diffuse;
        glm::vec3    m_specular;
        float        m_specularPower;
        glm::vec3    m_ambient;

        PMXDrawModeFlags m_drawMode;

        glm::vec4    m_edgeColor;
        float        m_edgeSize;

        int32_t    m_textureIndex;
        int32_t    m_sphereTextureIndex;
        PMXSphereMode m_sphereMode;

        PMXToonMode    m_toonMode;
        int32_t        m_toonTextureIndex;

        std::string    m_memo;

        int32_t    m_numFaceVertices;
    };

    enum class PMXVertexWeight : uint8_t
    {
        BDEF1,
        BDEF2,
        BDEF4,
        SDEF,
        QDEF,
    };

    struct PMXVertices
    {
        struct Int4
        {
            int data[4];
        };
        
        std::vector<glm::vec3>    m_position;
        std::vector<glm::vec3>    m_normal;
        std::vector<glm::vec2>    m_uv;

        std::vector<glm::vec4>    m_addUV[4];

        std::vector<PMXVertexWeight>    m_weightType; // 0:BDEF1 1:BDEF2 2:BDEF4 3:SDEF 4:QDEF
        std::vector<Int4>        m_boneIndices;
        std::vector<glm::vec4>        m_boneWeights;
        std::vector<glm::vec3>    m_sdefC;
        std::vector<glm::vec3>    m_sdefR0;
        std::vector<glm::vec3>    m_sdefR1;

        std::vector<float>    m_edgeMag;

        void resize(int size)
        {
            m_position.resize(size);
            m_normal.resize(size);
            m_uv.resize(size);
            m_addUV[0].resize(size);
            m_addUV[1].resize(size);
            m_addUV[2].resize(size);
            m_addUV[3].resize(size);
            m_weightType.resize(size);
            m_boneIndices.resize(size);
            m_boneWeights.resize(size);
            m_sdefC.resize(size);
            m_sdefR0.resize(size);
            m_sdefR1.resize(size);
            m_edgeMag.resize(size);
        }
    };
    typedef uint32_t PMXIndex;

    class PMXModel
    { 
    public:
        static PMXModel load(const char* filePath);

        PMXHeader    m_header;
        PMXInfo        m_info;

        PMXVertices        m_vertices;
        std::vector<PMXIndex>        m_indices;
        std::vector<PMXTexture>        m_textures;
        std::vector<PMXMaterial>    m_materials;
        //std::vector<PMXBone>        m_bones;
        //std::vector<PMXMorph>        m_morphs;
        //std::vector<PMXDispalyFrame>    m_displayFrames;
        //std::vector<PMXRigidbody>    m_rigidbodies;
        //std::vector<PMXJoint>        m_joints;
        //std::vector<PMXSoftbody>    m_softbodies;
    };

}