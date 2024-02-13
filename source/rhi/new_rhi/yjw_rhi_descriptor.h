#pragma once
#include "yjw_rhi_object.h"
#include "yjw_rhi_resource.h"

namespace rhi
{
    class RHIDescriptorCreation
    {
    public:
        DescriptorType type;
        RHIResourceHandle resource;
        Format format;
    };

    typedef RHIHandle RHIDescriptorHandle;
}