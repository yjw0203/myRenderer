#pragma once

namespace rhi
{
    class RHIObject
    {
    public:
        void setPayload(void* in_payload) { payload = in_payload; }
        void* getPayload() const { return payload; }
    private:
        void* payload;
    };
}