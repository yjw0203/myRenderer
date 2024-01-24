#pragma once
#include "rhi/new_rhi/yjw_rhi_resource.h"
#include "rhi/vulkan/resource/yjw_vulkan_resource_header.h"
namespace vulkan
{
    class BufferAdaptor
    {
    public:
        BufferAdaptor(const rhi::Resource& resource) : payload((Buffer*)(resource.getPayload())) {}
        BufferAdaptor(Buffer& buffer) : payload(&buffer) {}
        operator rhi::Buffer() { return rhi::Buffer{ payload }; }
        operator Buffer& () { return *payload; }
    private:
        Buffer* payload = nullptr;
    };

    class TextureAdaptor
    {
    public:
        TextureAdaptor(const rhi::Resource& resource) : payload((Texture*)(resource.getPayload())) {}
        TextureAdaptor(Texture& texture) : payload(&texture) {}
        operator rhi::Texture() { return rhi::Texture{ payload }; }
        operator Texture& () { return *payload; }
    private:
        Texture* payload = nullptr;
    };

    class ResourceInitConfigAdaptor
    {
    public:
        ResourceInitConfigAdaptor(const rhi::ResourceInitConfig& initConfig) : payload(initConfig){}
        operator TextureInitConfig();
        operator BufferInitConfig();
    private:
        const rhi::ResourceInitConfig& payload;
    };
}