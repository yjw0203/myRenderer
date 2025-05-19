#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_define.h"

namespace rpi
{
    extern RPI g_rpi;
#define RPIO(object) g_rpi.object

    void RPIInit();
    RPIContext RPICreateContext();
    void RPIGlobalSetResourceSetLayout(RPIResourceSetType type, RPIShaderReflect reflect);
    RPIWindow RPICreateWindow(void* window);
    void RPIDestroyWindow(RPIWindow window);
    RPIBuffer RPICreateGpuVertexBuffer(int size);
    RPIBuffer RPICreateGpuIndexBuffer(int size);
    RPIBuffer RPICreateGpuIndirectBuffer(int size);
    RPIBuffer RPICreateUploadBuffer(int size);
    RPIBuffer RPICreateFormatBuffer(int size, RPIFormat format);
    RPITexture RPICreateDefaultTexture2D(int width, int height, RPIFormat format,int mipLevels = 1);
    RPITexture RPICreateReadbackTexture2D(int width, int height, RPIFormat format,int mipLevels = 1);
    RPITexture RPICreateDepthStencilTexture2D(int width, int height, RPIFormat format);
    RPITexture RPICreateUploadTexture2D(int width, int height, RPIFormat format);
    RPITexture RPICreateTexture2DFromFile(const char* filePath);
    RPITexture RPICreateTextureCubeFromFile(const char* file[6]);// X, -X, Y, -Y, Z, -Z

    RPITexture RPICreateTextureView(RPITexture texture, RPIFormat format);
    RPIBuffer RPICreateBufferView(RPIBuffer buffer, int offset, int width);

    RPIShader RPICreateShader(RPIShaderType shaderType, const char* name, const char* entryName);

    RPIRenderPass RPICreateRenderPass(RPITexture* rtvs, int rtvCount, RPITexture dsv);
    RPIRenderPipeline RPICreateRenderPipeline(RPIRenderPipelineDescriptor createInfo);
    RPIComputePipeline RPICreateComputePipeline(RPIComputePipelineDescriptor createInfo);

    RPIResourceSet     RPICreateResourceSet(RPIResourceSetType type ,RPIShaderReflect reflect);

    RPIPrimitiveBinding RPICreatePrimitiveBinding(RPIShader vertex_shader);

    RPIFence RPISubmit(RPIContext context);
    void RPIPresent(RPIContext context, RPIWindow window);
    void RPIWaitForFence(RPIFence fence);

    void RPICmdPushEvent(RPIContext context, const char* name);
    void RPICmdPopEvent(RPIContext context);
    void RPICmdSetRenderPipeline(RPIContext context, RPIRenderPipeline pipeline, RPIPrimitiveBinding primitive_binding, RPIShader ps);
    void RPICmdPushConstants(RPIContext context, void* data, int offset, int size);
    void RPICmdSetResourceSet(RPIContext context, RPIResourceSetType type, RPIResourceSet set);
    void RPICmdSetPrimitiveBinding(RPIContext context, RPIPrimitiveBinding primitiveBinding,int sub_id);
    void RPICmdTransitionStateToRender(RPIContext context, RPIResourceSet resourceSet);
    void RPICmdBeginRenderPass(RPIContext context, RPIRenderPass renderPass);
    void RPICmdEndPass(RPIContext context);
    void RPICmdDraw(RPIContext context, int vertexCount, int instanceCount, int firstVertex, int firstInstance);
    void RPICmdDrawIndex(RPIContext context, int firstInstance, int instanceCount);
    void RPICmdDispatch(RPIContext context, int GroupCountX, int GroupCountY, int GroupCountZ);
    void RPICmdClearTexture(RPIContext context, RPITexture texture);
    void RPICmdCopyTexture(RPIContext context, RPITexture srcTexture, RPITexture dstTexture);
    void RPICmdClearBackBuffer(RPIContext context, RPIWindow window);
    
    void RPIUpdateBuffer(RPIBuffer buffer, const void* data,int offset, int size);

    RHILayer* RPIGetLayer(RHILayerType type);
}