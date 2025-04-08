#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_define.h"
#include "Engine/RHI/Public/rpi/yjw_rpi_resource.h"

namespace rpi
{
    enum RPIResourceSetType
    {
        vs = 0,
        ps = 1,
        cs = 2,
        common = 3,
    };

    bool RPICheckResourceSetTypeID(int set_id, RPIResourceSetType type);
    int RPIGetResourceSetIDByType(RPIResourceSetType type);

    class RPIBuffer;
    class RPITexture;
    class RPIResourceSet
    {
    public:
        static RPIResourceSet Null;
        RPIResourceSet();
        RPIResourceSet(RHIResourceSet* rhiResourceSet);
        void SetBuffer(RHIName name, RPIBuffer buffer);
        void SetTexture(RHIName name, RPITexture texture);
        RHIResourceSet* GetRHIResourceSet();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIResourceSet* m_resource_set = nullptr;
    };

    class RPIResourceBinding
    {
    public:
        static RPIResourceBinding Null;
        RPIResourceBinding();
        RPIResourceBinding(RHIResourceBinding* rhiResourceBinding);
        void SetResourceSet(RPIResourceSetType type, RPIResourceSet set);
        RHIResourceBinding* GetRHIResourceBinding();
        void Release();
        void Retain();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIResourceBinding* m_resource_binding = nullptr;
    };

    class RPIPrimitiveBinding
    {
    public:
        static RPIPrimitiveBinding Null;
        RPIPrimitiveBinding();
        RPIPrimitiveBinding(RHIPrimitiveBinding* rhiPrimitiveBinding);
        void SetVertexBuffer(RHIName name, RPIBuffer buffer);
        int AddIndexBuffer(RPIBuffer buffer, int index_start, int index_count, bool is_index_16bit);
        RHIPrimitiveBinding* GetRHIPrimitiveBinding();
        void Release();
        void Retain();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIPrimitiveBinding* m_primitive_binding = nullptr;
    };
}