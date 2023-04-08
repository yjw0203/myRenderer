#include "yjw_rhi_resource_view.h"
#include "yjw_rhi_interface.h"

namespace rhi
{
    RHIResourceView::RHIResourceView(ResourceViewType type, RHIResource* resource, RHIFormat view_format)
        :type(type), resource(resource), format(view_format)
    {
        resourceViewLocation = IRHI::Get()->createResourceView(type, resource, view_format);
    }
    
    RHIResourceView::~RHIResourceView()
    {
        IRHI::Get()->deleteResourceView(resourceViewLocation);
    }
}