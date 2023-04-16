#pragma once

#include "yjw_rhi_resource.h"
namespace rhi
{

    class RHIResourceViewLocation {};

    enum ResourceViewType
    {
        rtv,
        dsv,
        srv,
        uav
    };

    class RHIResourceView
    {
    public:
        RHIResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format);
        virtual ~RHIResourceView();
        const RHIResource* resource = nullptr;
        const ResourceViewType type;
        const RHIFormat format;
        RHIResourceViewLocation* resourceViewLocation = nullptr;
    };
}