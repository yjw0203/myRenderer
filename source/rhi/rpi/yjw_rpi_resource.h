#pragma once
#include "yjw_rpi_define.h"

namespace rpi
{
    class RPITexture
    {
    public:
        static RPITexture Null;
        RPITexture();
        RPITexture(RHITexture* texture, RHITextureView* view);
        RHITexture* GetTexture();
        RHITextureView* GetView();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHITexture* m_texture = nullptr;
        RHITextureView* m_view = nullptr;
    };

    class RPIBuffer
    {
    public:
        static RPIBuffer Null;
        RPIBuffer();
        RPIBuffer(RHIBuffer* buffer, RHIBufferView* view);
        RHIBuffer* GetBuffer();
        RHIBufferView* GetView();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIBuffer* m_buffer = nullptr;
        RHIBufferView* m_view = nullptr;
    };
}