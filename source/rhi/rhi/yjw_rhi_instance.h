#pragma once
#include "yjw_rhi_device.h"
#include "yjw_rhi_object.h"

namespace rhi
{
    enum class ERHIType
    {
        none,
        vulkan,
        d3d12,
        metal
    };

    struct RHIInstanceConfig
    {
        ERHIType rhiType;
        bool isDebugMode;
    };

    class RHIInstanceImpl : public RHIObject
    {
    public:
        virtual RHIDevice* CreateDevice() = 0;
    };

    class RHIInstance 
    {
    public:
        RHIInstance(RHIInstanceConfig config);
        ~RHIInstance();
        RHIDevice* CreateDevice();
    private:
        RHIInstanceImpl* m_impl = nullptr;
    };
}