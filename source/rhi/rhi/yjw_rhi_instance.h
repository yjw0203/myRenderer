#pragma once
#include "yjw_rhi_device.h"

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

    class RHIInstanceImpl
    {
    public:
        virtual RHIDevice* CreateDevice() = 0;
        virtual void DestroyDevice(RHIDevice* device) = 0;
    };

    class RHIInstance
    {
    public:
        RHIInstance(RHIInstanceConfig config);
        ~RHIInstance();
        RHIDevice* CreateDevice();
        void DestroyDevice(RHIDevice* device);
    private:
        RHIInstanceImpl* m_impl = nullptr;
    };
}