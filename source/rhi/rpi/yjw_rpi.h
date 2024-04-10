#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{
    extern RPI g_rpi;
#define RPIO(object) g_rpi.object

    void RPIInit();
    RPIContext RPICreateContext();
    RPIWindow RPICreateWindow(void* window);
    void RPIDestroyWindow(RPIWindow window);
    RPIBuffer RPICreateGpuVertexBuffer(int size);
    RPIBuffer RPICreateGpuIndexBuffer(int size);
    RPIBuffer RPICreateGpuIndirectBuffer(int size);
    RPIBuffer RPICreateUploadBuffer(int size);
    RPITexture RPICreateDefaultTexture2D(int width, int height, RPIFormat format,int mipLevels = 1);
    RPITexture RPICreateDepthStencilTexture2D(int width, int height, RPIFormat format);
    RPITexture RPICreateUploadTexture2D(int width, int height, RPIFormat format);
    RPITexture RPICreateTexture2DFromFile(const char* filePath);

    RPITextureView RPICreateTextureView(RPITexture texture, RPIFormat format);
    RPIBufferView RPICreateBufferView(RPIBuffer buffer, int offset, int width);

    RPIShader RPICreateShader(const char* name);

    RPIRenderPass RPICreateRenderPass(RPITextureView* rtvs, int rtvCount, RPITextureView dsv);
    RPIPipeline RPICreateRenderPipeline(RPIRenderPipelineDescriptor createInfo);

    RPIResourceBinding RPICreateResourceBinding(RPIPipeline pipeline);


    void RPISubmit(RPIContext context);
    void RPIPresent(RPIContext context, RPIWindow window, RPITexture presentTexture);

    void RPICmdSetPipeline(RPIContext context, RPIPipeline pipeline);
    void RPICmdSetResourceBinding(RPIContext context, RPIResourceBinding resourceBinding);
    void RPICmdBeginRenderPass(RPIContext context, RPIRenderPass renderPass);
    void RPICmdEndPass(RPIContext context);
    void RPICmdDraw(RPIContext context, int vertexCount, int instanceCount, int firstVertex, int firstInstance);
    void RPICmdDrawIndex(RPIContext context, int indexCount, int instanceCount, int firstIndex, int vertexOffset, int firstInstance);
    void RPICmdClearTexture(RPIContext context, RPITexture texture);
    void RPICmdCopyTexture(RPIContext context, RPITexture srcTexture, RPITexture dstTexture);
    
    void RPIUpdateBuffer(RPIBuffer buffer, void* data,int offset, int size);
}