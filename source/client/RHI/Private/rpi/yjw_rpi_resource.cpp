#include "Public/RHI/rpi/yjw_rpi_resource.h"

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