#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_define.h"

namespace rpi
{
    class RPITexture
    {
    public:
        struct MapData
        {
            void* data;
            int byte_per_pixel;
            int byte_per_raw;
        };
        static RPITexture Null;
        RPITexture();
        RPITexture(RHITexture* texture, RHITextureView* view);
        RHITexture* GetTexture();
        RHITextureView* GetView();
        void BeginReadbackMode(int arrayLayer, int mipLevel);
        void ReadbackLoad(int x, int y, void* data);
        void EndReadbackMode();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHITexture* m_texture = nullptr;
        RHITextureView* m_view = nullptr;
        MapData m_map_data{};
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