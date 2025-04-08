#include "Engine/RHI/Public/rhi/yjw_rhi_object.h"
#include "Engine/RHI/Public/rhi/yjw_rhi_swap_chain.h"

namespace rhi
{

    int RHIObject::retain(RHIObject* byWhich)
    {
        return m_ref_count.fetch_add(1);
    }

    int RHIObject::release()
    {
        int ref = m_ref_count.fetch_sub(1);
        if (ref == 0)
        {
            delete this;
        }
        return ref;
    }
}