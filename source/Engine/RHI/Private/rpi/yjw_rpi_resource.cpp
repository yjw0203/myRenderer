#include "Engine/RHI/Public/rpi/yjw_rpi_resource.h"

namespace rpi
{
    RPITexture RPITexture::Null = RPITexture(nullptr, nullptr);
    RPIBuffer RPIBuffer::Null = RPIBuffer(nullptr, nullptr);

    RPITexture::RPITexture()
    {
        m_texture = nullptr;
        m_view = nullptr;
    }

    RPITexture::RPITexture(RHITexture* texture, RHITextureView* view)
    {
        m_texture = texture;
        m_view = view;
    }

    RHITexture* RPITexture::GetTexture()
    {
        return m_texture;
    }

    RHITextureView* RPITexture::GetView()
    {
        return m_view;
    }

    void RPITexture::BeginReadbackMode(int arrayLayer, int mipLevel)
    {
        m_texture->MapForReadback(arrayLayer, mipLevel, m_map_data.data, m_map_data.byte_per_pixel, m_map_data.byte_per_raw);
    }

    void RPITexture::ReadbackLoad(int x, int y, void* data)
    {
        int offset = y * m_map_data.byte_per_raw + x * m_map_data.byte_per_pixel;
        memcpy(data, (uint8_t*)m_map_data.data + offset , m_map_data.byte_per_pixel);
    }

    void RPITexture::EndReadbackMode()
    {
        m_texture->UnMapReadback();
    }

    void RPITexture::Release()
    {
        if(m_view)
        {
            m_view->release();
            m_view = nullptr;
        }
        if (m_texture)
        {
            m_texture->release();
            m_texture = nullptr;
        }
    }

    bool RPITexture::IsNull()
    {
        return m_texture == nullptr || m_view == nullptr;
    }

    RPIBuffer::RPIBuffer()
    {
        m_buffer = nullptr;
        m_view = nullptr;
    }

    RPIBuffer::RPIBuffer(RHIBuffer* buffer, RHIBufferView* view)
    {
        m_buffer = buffer;
        m_view = view;
    }

    RHIBuffer* RPIBuffer::GetBuffer()
    {
        return m_buffer;
    }

    RHIBufferView* RPIBuffer::GetView()
    {
        return m_view;
    }

    void RPIBuffer::Release()
    {
        if (m_view)
        {
            m_view->release();
            m_view = nullptr;
        }
        if (m_buffer)
        {
            m_buffer->release();
            m_buffer = nullptr;
        }
    }

    bool RPIBuffer::IsNull()
    {
        return m_buffer == nullptr || m_view == nullptr;
    }
}