#include "yjw_rhi_resource.h"
#include "yjw_rhi_interface.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace rhi
{
    RHIResource::RHIResource(RHIResourceType type, int width, int height, int depthOrArraySize, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType)
        :rhiResourceDesc{ type , width, height,depthOrArraySize ,miplevels,format,usage,memoryType }
    {
        resourceLocation = IRHI::Get()->createResource(rhiResourceDesc);
    }
    RHIResource::~RHIResource()
    {
        IRHI::Get()->deleteResource(resourceLocation);
    }

    RHIBuffer::RHIBuffer(int size, RHIResourceUsage usage, RHIMemoryType memoryType)
        :RHIResource(RHIResourceType::buffer,
            size,
            1,
            1,
            1,
            RHIFormat::unknow,
            usage,
            memoryType)
    {
    }

    RHITexture2D::RHITexture2D(int width, int height, int miplevels, RHIFormat format, RHIResourceUsage usage, RHIMemoryType memoryType)
        :RHIResource(RHIResourceType::texture2D,
            width,
            height,
            1,
            miplevels,
            format,
            usage,
            memoryType)
    {

    }

    RHITexture2DFromFile::RHITexture2DFromFile(const char* filePath)
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        this->RHIResource::RHIResource(RHIResourceType::texture2D,
            texWidth,
            texHeight,
            1,
            1,
            RHIFormat::R8G8B8A8_srgb,
            allow_transfer_dst,
            RHIMemoryType::default_);

        int imageSize = texWidth * texHeight * 4;

        RHIBuffer stageingBuffer(imageSize, allow_transfer_src | deny_shader_resource, RHIMemoryType::upload);

        IRHI::Get()->writeResourceImmidiately(&stageingBuffer, pixels, imageSize,0);

        stbi_image_free(pixels);
        
        IRHI::Get()->resourceBarrierImmidiately(this, RHIResourceState::undefine, RHIResourceState::transfer_dst);
        IRHI::Get()->copyResourceImmidiately(&stageingBuffer, this);
        IRHI::Get()->resourceBarrierImmidiately(this, RHIResourceState::transfer_dst, RHIResourceState::shader_resource_read);
    }

    int sizeofFormat(RHIFormat rhiFormat)
    {
        switch (rhiFormat)
        {
        case RHIFormat::unknow: return 0;
        case RHIFormat::R8G8B8A8_unorm: return 4;
        case RHIFormat::R8G8B8A8_srgb: return 4;
        case RHIFormat::R8G8B8A8_snorm: return 4;
        case RHIFormat::R32G32B32_sfloat: return 12;
        case RHIFormat::R32G32B32A32_sfloat: return 16;
        case RHIFormat::R32G32_sfloat: return 8;
        case RHIFormat::D24_unorm_S8_uint: return 4;
        }
        return 0;
    }
}