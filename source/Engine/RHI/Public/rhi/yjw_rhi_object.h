#pragma once
#include<atomic>

namespace rhi
{
    class RHIObject
    {
    public:
        virtual ~RHIObject(){}
        int retain(RHIObject* byWhich);
        int release();
    private:
        std::atomic<int> m_ref_count{ 1 };
    };
}