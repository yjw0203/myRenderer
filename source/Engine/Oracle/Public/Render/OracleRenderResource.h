#pragma once
#include <string>
#include "Engine/RHI/Public/rpi/yjw_rpi.h"

class OracleRenderResource
{
public:
    OracleRenderResource() {};
    OracleRenderResource(const char* url)
        : m_url(url)
    {
        m_resource = rpi::RPICreateTexture2DFromFile(url);
    }

    ~OracleRenderResource()
    {
        
    }

    bool IsVaild()
    {
        return !m_resource.IsNull();
    }

    rpi::RPITexture GetResource()
    {
        return m_resource;
    }

private:
    std::string m_url;
    rpi::RPITexture m_resource{};
};
