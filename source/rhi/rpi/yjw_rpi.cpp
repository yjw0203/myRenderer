#include "yjw_rpi.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
namespace rpi
{
    RPI g_rpi{};

    void RPIInitState();

    void RPIInit()
    {
        RHIInstanceConfig config{};
        config.rhiType = ERHIType::vulkan;
        config.isDebugMode = true;
        RPIO(instance) = new RHIInstance(config);
        RPIO(device) = RPIO(instance)->CreateDevice();
        RPIInitState();
    }

    RPIContext RPICreateContext()
    {
        return RPIO(device)->CreateContext();
    }

    RPIWindow RPICreateWindow(void* window)
    {
        return RPIWindow{ window, RPIO(device)->CreateSwapchain(window) };
    }

    void RPIDestroyWindow(RPIWindow window)
    {
        window.swapchain->release();
    }

    RPIBuffer RPICreateGpuVertexBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::default_;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_vertex_buffer | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::deny_shader_resource;
        return RPIO(device)->CreateBuffer(desc);
    }
    RPIBuffer RPICreateGpuIndexBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::default_;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_index_buffer | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::deny_shader_resource;
        return RPIO(device)->CreateBuffer(desc);
    }
    RPIBuffer RPICreateGpuIndirectBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::default_;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_indirect_buffer | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::deny_shader_resource;
        return RPIO(device)->CreateBuffer(desc);
    }
    RPIBuffer RPICreateUploadBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::upload;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_indirect_buffer | (int)RHIResourceUsageBits::allow_transfer_src;
        return RPIO(device)->CreateBuffer(desc);
    }
    RPITexture RPICreateDefaultTexture2D(int width, int height, RPIFormat format, int mipLevels /* = 1*/)
    {
        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::texture2D;
        desc.width = width;
        desc.height = height;
        desc.depthOrArraySize = 1;
        desc.miplevels = mipLevels;
        desc.format = format;
        desc.usage = (int)RHIResourceUsageBits::allow_render_target | (int)RHIResourceUsageBits::allow_transfer_src | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::allow_unordered_access;
        desc.memoryType = RHIMemoryType::default_;
        return RPIO(device)->CreateTexture(desc);
    }
    RPITexture RPICreateDepthStencilTexture2D(int width, int height, RPIFormat format)
    {
        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::texture2D;
        desc.width = width;
        desc.height = height;
        desc.depthOrArraySize = 1;
        desc.miplevels = 1;
        desc.format = format;
        desc.usage = (int)RHIResourceUsageBits::allow_depth_stencil | (int)RHIResourceUsageBits::allow_transfer_src | (int)RHIResourceUsageBits::allow_transfer_dst;
        desc.memoryType = RHIMemoryType::default_;
        return RPIO(device)->CreateTexture(desc);
    }
    RPITexture RPICreateUploadTexture2D(int width, int height, RPIFormat format)
    {
        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::texture2D;
        desc.width = width;
        desc.height = height;
        desc.depthOrArraySize = 1;
        desc.miplevels = 1;
        desc.format = format;
        desc.usage = (int)RHIResourceUsageBits::allow_transfer_src;
        desc.memoryType = RHIMemoryType::upload;
        return RPIO(device)->CreateTexture(desc);
    }
    RPITexture RPICreateTexture2DFromFile(const char* filePath)
    {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::texture2D;
        desc.width = texWidth;
        desc.height = texHeight;
        desc.depthOrArraySize = 1;
        desc.miplevels = 1;
        desc.format = RHIFormat::R8G8B8A8_srgb;
        desc.usage = (int)RHIResourceUsageBits::allow_transfer_dst;
        desc.memoryType = RHIMemoryType::default_;
        RPITexture texture = RPIO(device)->CreateTexture(desc);

        int imageSize = texWidth * texHeight * 4;
        texture->Update(pixels, imageSize);
        stbi_image_free(pixels);
        return texture;
    }

    RPITextureView RPICreateTextureView(RPITexture texture, RPIFormat format)
    {
        RHITextureViewDescriptor desc{};
        desc.texture = texture;
        desc.format = format;
        return RPIO(device)->CreateTextureView(desc);
    }

    RPIBufferView RPICreateBufferView(RPIBuffer buffer, int offset, int width)
    {
        RHIBufferViewDescriptor desc{};
        desc.buffer = buffer;
        desc.offset = offset;
        desc.width = width;
        return RPIO(device)->CreateBufferView(desc);
    }

    RPIShader RPICreateShader(const char* name)
    {
        return RPIO(device)->CreateShaderByBinaryUrl(name);
    }

    RPIRenderPass RPICreateRenderPass(RPITextureView* rtvs, int rtvCount, RPITextureView dsv)
    {
        RHIRenderPassDescriptor desc{};
        for (int index = 0; index < rtvCount; index++)
        {
            desc.colorAttachments[index] = rtvs[index];
        }
        desc.colorAttachmentCount = rtvCount;
        desc.depthStencilAttachment = dsv;
        return RPIO(device)->CreateRenderPass(desc);
    }

    RPIPipeline RPICreateRenderPipeline(RPIRenderPipelineDescriptor createInfo)
    {
        return RPIO(device)->CreateRenderPipeline(createInfo);
    }

    RPIResourceBinding RPICreateResourceBinding(RPIPipeline pipeline)
    {
        return pipeline->CreateResourceBinding();
    }

    void RPISubmit(RPIContext context)
    {
        context->Submit();
    }

    void RPIPresent(RPIContext context, RPIWindow window, RPITexture presentTexture)
    {
        context->CopyTexture2D(presentTexture, window.swapchain->GetBackTexture());
        context->Present(window.swapchain, false);
    }

    void RPICmdSetPipeline(RPIContext context, RPIPipeline pipeline)
    {
        context->SetRenderPipeline((RHIRenderPipeline*)pipeline);//todo hack
    }

    void RPICmdSetResourceBinding(RPIContext context, RPIResourceBinding resourceBinding)
    {
        context->SetResourceBinding(resourceBinding);
    }

    void RPICmdBeginRenderPass(RPIContext context, RPIRenderPass renderPass, RPIResourceBinding* resourceBinding, int resourceBindingCount)
    {
        for (int index = 0; index < resourceBindingCount; index++)
        {
            context->TransitionStateToRender(resourceBinding[index]);
        }
        context->BeginPass(renderPass);
    }

    void RPICmdEndPass(RPIContext context)
    {
        context->EndPass();
    }

    void RPICmdDraw(RPIContext context, int vertexCount, int instanceCount, int firstVertex, int firstInstance)
    {
        context->Draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void RPICmdDrawIndex(RPIContext context, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance)
    {
        context->DrawIndex(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void RPICmdClearTexture(RPIContext context, RPITexture texture)
    {
        context->ClearTexture2D(texture);
    }

    void RPICmdCopyTexture(RPIContext context, RPITexture srcTexture, RPITexture dstTexture)
    {
        context->CopyTexture2D(srcTexture, dstTexture);
    }

    void RPIUpdateBuffer(RPIBuffer buffer, void* data, int offset, int size)
    {
        buffer->Update(data, offset, size);
    }
}