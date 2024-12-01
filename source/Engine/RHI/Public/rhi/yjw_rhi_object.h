#pragma once
#include<atomic>

namespace rhi
{
    class RHIObject
    {
    public:
        virtual ~RHIObject(){}
        void retain(RHIObject* byWhich);
        void release();
    private:
        std::atomic<int> m_ref_count{ 1 };
    };
}