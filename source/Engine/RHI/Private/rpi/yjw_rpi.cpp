#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include "Engine/RHI/Public/shaderCompiler/yjw_shader_compiler.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_resource_binding.h"
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
        config.isDebugMode = false;
        config.layers.push_back(rhi_layer_imgui);
        RPIO(instance) = new RHIInstance(config);
        RPIO(device) = RPIO(instance)->CreateDevice();
        RPIInitState();
    }

    RPIContext RPICreateContext()
    {
        return RPIO(device)->CreateContext();
    }

    void RPIGlobalSetResourceSetLayout(RPIResourceSetType type, RPIShaderReflect reflect)
    {
        return RPIO(device)->SetGlobalResourceSetLayout((int)type, *reflect);
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
        desc.usage = (int)RHIResourceUsageBits::allow_index_buffer | (int)RHIResourceUsageBits::allow_vertex_buffer | (int)RHIResourceUsageBits::allow_indirect_buffer | (int)RHIResourceUsageBits::allow_transfer_src;
        RHIBuffer* buffer = RPIO(device)->CreateBuffer(desc);

        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer;
        viewDesc.offset = 0;
        viewDesc.width = size;
        RHIBufferView* bufferView = RPIO(device)->CreateBufferView(viewDesc);

        return RPIBuffer(buffer, bufferView);
    }

    RPIBuffer RPICreateFormatBuffer(int size, RPIFormat format)
    {
        RHIBufferDescriptor desc{};
        desc.resourceType = rhi::RHIResourceType::buffer;
        desc.memoryType = rhi::RHIMemoryType::upload;
        desc.width = size;
        desc.usage = (int)RHIResourceUsageBits::allow_index_buffer | (int)RHIResourceUsageBits::allow_vertex_buffer | (int)RHIResourceUsageBits::allow_indirect_buffer | (int)RHIResourceUsageBits::allow_transfer_src;
        RHIBuffer* buffer = RPIO(device)->CreateBuffer(desc);

        RHIBufferViewDescriptor viewDesc{};
        viewDesc.buffer = buffer;
        viewDesc.offset = 0;
        viewDesc.width = size;
        viewDesc.format = format;
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
        viewDesc.resourceType = RHIResourceType::texture2D;
        viewDesc.texture = texture;
        viewDesc.format = format;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
    }

    RPITexture RPICreateReadbackTexture2D(int width, int height, RPIFormat format, int mipLevels /* = 1*/)
    {
        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::texture2D;
        desc.width = width;
        desc.height = height;
        desc.depthOrArraySize = 1;
        desc.miplevels = mipLevels;
        desc.format = format;
        desc.usage = (int)RHIResourceUsageBits::allow_transfer_dst | (int)RHIResourceUsageBits::allow_unordered_access | (int)RHIResourceUsageBits::deny_shader_resource;
        desc.memoryType = RHIMemoryType::readback;
        RHITexture* texture = RPIO(device)->CreateTexture(desc);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.resourceType = RHIResourceType::texture2D;
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
        viewDesc.resourceType = RHIResourceType::texture2D;
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
        viewDesc.resourceType = RHIResourceType::texture2D;
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
        desc.format = RHIFormat::R8G8B8A8_unorm;
        desc.usage = (int)RHIResourceUsageBits::allow_transfer_dst;
        desc.memoryType = RHIMemoryType::default_;
        RHITexture* texture = RPIO(device)->CreateTexture(desc);
        int imageSize = texWidth * texHeight * 4;
        if (pixels)
        {
            texture->Update(pixels, imageSize);
        }
        stbi_image_free(pixels);

        RHITextureViewDescriptor viewDesc{};
        viewDesc.resourceType = RHIResourceType::texture2D;
        viewDesc.texture = texture;
        viewDesc.format = RHIFormat::R8G8B8A8_unorm;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
    }

    RPITexture RPICreateTextureCubeFromFile(const char* file[6])
    {
        int texWidth[6] = {}, texHeight[6] = {}, texChannels[6] = {};
        stbi_uc* pixels[6] = {};
        for (int i = 0; i < 6; i++)
        {
            pixels[i] = stbi_load(file[i], &texWidth[i], &texHeight[i], &texChannels[i], STBI_rgb_alpha);
        }
        for (int i = 1; i < 6; i++)
        {
            assert(texWidth[i] == texWidth[i - 1]);
            assert(texHeight[i] == texHeight[i - 1]);
            assert(texChannels[i] == texChannels[i - 1]);
        }
        RHITextureDescriptor desc{};
        desc.resourceType = RHIResourceType::textureCube;
        desc.width = texWidth[0];
        desc.height = texHeight[0];
        desc.depthOrArraySize = 1;
        desc.miplevels = 1;
        desc.format = RHIFormat::R8G8B8A8_unorm;
        desc.usage = (int)RHIResourceUsageBits::allow_transfer_dst;
        desc.memoryType = RHIMemoryType::default_;
        RHITexture* texture = RPIO(device)->CreateTexture(desc);
        for (int i = 0; i < 6; i++)
        {
            int imageSize = texWidth[i] * texHeight[i] * 4;
            if (pixels[i])
            {
                texture->Update(pixels[i], imageSize, i, 0);
            }
            stbi_image_free(pixels[i]);
        }

        RHITextureViewDescriptor viewDesc{};
        viewDesc.resourceType = RHIResourceType::textureCube;
        viewDesc.texture = texture;
        viewDesc.format = RHIFormat::R8G8B8A8_unorm;
        viewDesc.baseLayer = 0;
        viewDesc.layerCount = 6;
        RHITextureView* textureView = RPIO(device)->CreateTextureView(viewDesc);

        return RPITexture(texture, textureView);
    }

    RPITexture RPICreateTextureView(RPITexture texture, RPIFormat format)
    {
        RHITextureViewDescriptor viewDesc{};
        viewDesc.resourceType = RHIResourceType::texture2D;
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
        RPIShader shader = RPIO(device)->CreateShaderByBinary(blob.Data(), blob.Size(), entryName, blob.GetReflect());
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

    RPIRenderPipeline RPICreateRenderPipeline(RPIRenderPipelineDescriptor createInfo)
    {
        return RPIRenderPipeline(createInfo);
    }

    RPIComputePipeline RPICreateComputePipeline(RPIComputePipelineDescriptor createInfo)
    {
        return RPIO(device)->CreateComputePipeline(createInfo);
    }

    RPIResourceSet RPICreateResourceSet(RPIResourceSetType type, RPIShaderReflect reflect)
    {
        return RPIResourceSet(RPIO(device)->CreateResourceSet((int)type, *reflect));
    }

    RPIPrimitiveBinding RPICreatePrimitiveBinding(RPIShader vertex_shader)
    {
        return RPIPrimitiveBinding(vertex_shader->CreatePrimitiveBinding());
    }

    RPIFence RPISubmit(RPIContext context)
    {
        return context->Submit();
    }

    void RPIWaitForFence(RPIFence fence)
    {
        RPIO(device)->WaitForFence(fence);
    }

    void RPIPresent(RPIContext context, RPIWindow window)
    {
        //context->CopyTexture2D(presentTexture.GetTexture(), window.swapchain->GetBackTexture());
        context->Present(window.swapchain, false);
    }

    void RPICmdPushEvent(RPIContext context, const char* name)
    {
        context->PushEvent(name);
    }

    void RPICmdPopEvent(RPIContext context)
    {
        context->PopEvent();
    }

    void RPICmdSetRenderPipeline(RPIContext context, RPIRenderPipeline pipeline, RPIPrimitiveBinding primitive_binding, RPIShader ps)
    {
        context->SetRenderPipeline((RHIRenderPipeline*)pipeline.GetRHIPipeline(primitive_binding.GetVertexShader(), ps));
    }

    void RPICmdPushConstants(RPIContext context, void* data, int offset, int size)
    {
        context->SetPushConstants(data, offset, size);
    }

    void RPICmdSetResourceSet(RPIContext context, RPIResourceSetType type, RPIResourceSet set)
    {
        context->SetResourceSet((int)type, set.GetRHIResourceSet());
    }

    void RPICmdSetPrimitiveBinding(RPIContext context, RPIPrimitiveBinding primitiveBinding, int sub_id)
    {
        context->SetPrimitiveBinding(primitiveBinding.GetRHIPrimitiveBinding(), sub_id);
    }

    void RPICmdTransitionStateToRender(RPIContext context, RPIResourceSet resourceSet)
    {
        context->TransitionStateToRender(resourceSet.GetRHIResourceSet());
    }

    void RPICmdBeginRenderPass(RPIContext context, RPIRenderPass renderPass)
    {
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

    void RPICmdDrawIndex(RPIContext context, int firstInstance, int instanceCount)
    {
        context->DrawIndex(firstInstance, instanceCount);
    }

    void RPICmdDispatch(RPIContext context, int GroupCountX, int GroupCountY, int GroupCountZ)
    {
        context->Dispatch(GroupCountX, GroupCountY, GroupCountZ);
    }

    void RPICmdClearTexture(RPIContext context, RPITexture texture)
    {
        context->ClearTexture2D(texture.GetTexture());
    }

    void RPICmdCopyTexture(RPIContext context, RPITexture srcTexture, RPITexture dstTexture)
    {
        context->CopyTexture2D(srcTexture.GetTexture(), dstTexture.GetTexture());
    }

    void RPICmdClearBackBuffer(RPIContext context, RPIWindow window)
    {
        context->ClearTexture2D(window.swapchain->GetBackTexture());
        context->ClearTexture2D(window.swapchain->GetDepthTexture());
    }

    void RPIUpdateBuffer(RPIBuffer buffer, const void* data, int offset, int size)
    {
        buffer.GetBuffer()->Update(data, offset, size);
    }

    RHILayer* RPIGetLayer(RHILayerType type)
    {
        return RPIO(instance)->GetRHILayer(type);
    }
}