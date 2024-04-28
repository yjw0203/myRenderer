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
        void SetVertexBuffer(RHIName name, RPIBuffer buffer);
        void SetIndexBuffer(RPIBuffer buffer);
        RHIResourceBinding* GetRHIResourceBinding();
        void Release();
        bool IsNull();
        operator bool() { return !IsNull(); }
    private:
        RHIResourceBinding* m_resource_binding = nullptr;
    };
}