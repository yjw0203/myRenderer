#include "yjw_rhi_object.h"

namespace rhi
{
    void RHIObject::retain(RHIObject* byWhich)
    {
        m_ref_count.fetch_add(1);
    }

    void RHIObject::release()
    {
        if (m_ref_count.fetch_sub(1) == 1)
        {
            delete this;
        }
    }
}