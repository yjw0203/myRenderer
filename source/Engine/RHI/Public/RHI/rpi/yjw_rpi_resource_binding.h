#pragma once
#include "RHI/rpi/yjw_rpi_define.h"
#include "RHI/rpi/yjw_rpi_resource.h"

namespace rpi
{
    class RPIBuffer;
    class RPITexture;
    class RPIResourceBinding
    {
    public:
        static RPIResourceBinding Null;
        RPIResourceBinding();
        RPIResourceBinding(RHIResourceBinding* rhiResourceBinding);
        void SetBuffer(RPIShaderType shaderType, RHIName name, RPIBuffer buffer);
        void SetTexture(RHIShaderType shaderType, RHIName name, RPITexture texture);
        RHIResourceBinding* GetRHIResourceBinding();
        void Release();
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
        void SetIndexBuffer(RPIBuffer buffer, int index_start, int index_count, bool is_index_16bit);
        RHIPrimitiveBinding* GetRHIPrimitiveBinding();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIPrimitiveBinding* m_primitive_binding = nullptr;
    };
}