#include "Public/RHI/rpi/yjw_rpi.h"
#include "Private/shaderCompiler/yjw_shader_compiler.h"
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
        config.layers.push_back(rhi_layer_imgui);
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
        desc.usage = (int)RHIResourceUsageBits::allow_index_buffer | (int)RHIResourceUsageBits::allow_vertex_buffer | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::deny_shader_resource;
        RHIBuffer* buffer = RPIO(device)->CreateBuffer(desc);

        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer;
        viewDesc.offset = 0;
        viewDesc.width = size;
        RHIBufferView* bufferView = RPIO(device)->CreateBufferView(viewDesc);
        
        return RPIBuffer(buffer, bufferView);
    }
    RPIBuffer RPICreateGpuIndexBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::default_;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_index_buffer | (int)RHIResourceUsageBits::allow_vertex_buffer | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::deny_shader_resource;
        RHIBuffer* buffer = RPIO(device)->CreateBuffer(desc);

        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer;
        viewDesc.offset = 0;
        viewDesc.width = size;
        RHIBufferView* bufferView = RPIO(device)->CreateBufferView(viewDesc);
        
        return RPIBuffer(buffer, bufferView);
    }
    RPIBuffer RPICreateGpuIndirectBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::default_;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_indirect_buffer | (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::deny_shader_resource;
        RHIBuffer* buffer = RPIO(device)->CreateBuffer(desc);

        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer;
        viewDesc.offset = 0;
        viewDesc.width = size;
        RHIBufferView* bufferView = RPIO(device)->CreateBufferView(viewDesc);
        
        return RPIBuffer(buffer, bufferView);
    }
    RPIBuffer RPICreateUploadBuffer(int size)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::upload;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_indirect_buffer | (int)RHIResourceUsageBits::allow_transfer_src;
        RHIBuffer* buffer = RPIO(device)->CreateBuffer(desc);

        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer;
        viewDesc.offset = 0;
        viewDesc.width = size;
        RHIBufferView* bufferView = RPIO(device)->CreateBufferView(viewDesc);

        return RPIBuffer(buffer, bufferView);
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
        RHITexture* texture = RPIO(device)->CreateTexture(desc);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.texture = texture;
        viewDesc.format = format;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
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
        RHITexture* texture = RPIO(device)->CreateTexture(desc);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.texture = texture;
        viewDesc.format = format;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
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
        RHITexture* texture = RPIO(device)->CreateTexture(desc);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.texture = texture;
        viewDesc.format = format;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
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
        RHITexture* texture = RPIO(device)->CreateTexture(desc);
        int imageSize = texWidth * texHeight * 4;
        texture->Update(pixels, imageSize);
        stbi_image_free(pixels);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.texture = texture;
        viewDesc.format = RHIFormat::R8G8B8A8_srgb;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
    }

    RPITexture RPICreateTextureView(RPITexture texture, RPIFormat format)
    {
        RHITextureViewDescriptor viewDesc{};
        viewDesc.texture = texture.GetTexture();
        viewDesc.format = format;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);
        return RPITexture(texture.GetTexture(), textureView);
    }

    RPIBuffer RPICreateBufferView(RPIBuffer buffer, int offset, int width)
    {
        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer.GetBuffer();
        viewDesc.offset = offset;
        viewDesc.width = width;
        RHIBufferView* bufferView = RPIO(device)->CreateBufferView(viewDesc);
        return RPIBuffer(buffer.GetBuffer(), bufferView);
    }

    ShaderConductor::ShaderStage RPIConvertShaderTypeToShaderCType(RPIShaderType shaderType)
    {
        switch (shaderType)
        {
        case RPIShaderType::vertex:return ShaderConductor::ShaderStage::VertexShader;
        case RPIShaderType::fragment:return ShaderConductor::ShaderStage::PixelShader;
        case RPIShaderType::compute:return ShaderConductor::ShaderStage::ComputeShader;
        }
        assert(0);
        return ShaderConductor::ShaderStage::VertexShader;
    }

    RPIShader RPICreateShader(RPIShaderType shaderType, const char* name, const char* entryName)
    {
        ShaderCompiler shaderCompiler;
        ShaderBlob blob = shaderCompiler.CompileFromFileHLSLToSpirv(RPIConvertShaderTypeToShaderCType(shaderType), name, entryName);
        RPIShader shader = RPIO(device)->CreateShaderByBinary(blob.Data(), blob.Size(), entryName);
        blob.Release();
        return shader;
    }

    RPIRenderPass RPICreateRenderPass(RPITexture* rtvs, int rtvCount, RPITexture dsv)
    {
        RHIRenderPassDescriptor desc{};
        for (int index = 0; index < rtvCount; index++)
        {
            desc.colorAttachments[index] = rtvs[index].GetView();
        }
        desc.colorAttachmentCount = rtvCount;
        desc.depthStencilAttachment = dsv.GetView();
        return RPIO(device)->CreateRenderPass(desc);
    }

    RPIPipeline RPICreateRenderPipeline(RPIRenderPipelineDescriptor createInfo)
    {
        return RPIO(device)->CreateRenderPipeline(createInfo);
    }

    RPIResourceBinding RPICreateResourceBinding(RPIPipeline pipeline)
    {
        return RPIResourceBinding(pipeline->CreateResourceBinding());
    }

    void RPISubmit(RPIContext context)
    {
        context->Submit();
    }

    void RPIPresent(RPIContext context, RPIWindow window, RPITexture presentTexture)
    {
        //context->CopyTexture2D(presentTexture.GetTexture(), window.swapchain->GetBackTexture());
        context->Present(window.swapchain, false);
    }

    void RPICmdSetPipeline(RPIContext context, RPIPipeline pipeline)
    {
        context->SetRenderPipeline((RHIRenderPipeline*)pipeline);//todo hack
    }

    void RPICmdSetResourceBinding(RPIContext context, RPIResourceBinding resourceBinding)
    {
        context->SetResourceBinding(resourceBinding.GetRHIResourceBinding());
    }

    void RPICmdBeginRenderPass(RPIContext context, RPIRenderPass renderPass, RPIResourceBinding* resourceBinding, int resourceBindingCount)
    {
        for (int index = 0; index < resourceBindingCount; index++)
        {
            context->TransitionStateToRender(resourceBinding[index].GetRHIResourceBinding());
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
        context->ClearTexture2D(texture.GetTexture());
    }

    void RPICmdCopyTexture(RPIContext context, RPITexture srcTexture, RPITexture dstTexture)
    {
        context->CopyTexture2D(srcTexture.GetTexture(), dstTexture.GetTexture());
    }

    void RPIUpdateBuffer(RPIBuffer buffer, void* data, int offset, int size)
    {
        buffer.GetBuffer()->Update(data, offset, size);
    }

    RHILayer* RPIGetLayer(RHILayerType type)
    {
        return RPIO(instance)->GetRHILayer(type);
    }
}