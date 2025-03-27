#pragma once
#include "yjw_internal_shader_resource_data_define.h"
#include "Engine/RHI/Public/rpi/yjw_rpi.h"
#include <vector>

#define DECALE_INTERNAL_SHADER_PARAMETER() \
    static constexpr int CounterBase = __COUNTER__; \
    template<int Counter> \
    struct InternalLinkType { \
        static __forceinline int GetTotalParameterSize() { return 0; } \
        static __forceinline void Initialize(InternalShaderParameter* This) {} \
    }; 

#define INTERNAL_SHADER_PARAMETER_WITH_ID(ParameterType,ShaderParameterName,VariableName, BindingID,ID) \
    ParameterType* VariableName = nullptr; \
    template<> \
    struct InternalLinkType<ID - CounterBase> \
    { \
        static __forceinline int GetTotalParameterSize() { return InternalLinkType<ID - CounterBase + 1>::GetTotalParameterSize() + sizeof(ParameterType); } \
        static __forceinline void Initialize(InternalShaderParameter* This) \
        { \
            InternalLinkType<ID - CounterBase + 1>::Initialize(This); \
            int currentOffset = This->m_shader_parameter_layouts.size() > 0 ? This->m_shader_parameter_layouts.back().offset + This->m_shader_parameter_layouts.back().width : 0; \
            InternalShaderParameterLayout layout; \
            layout.binding_id = BindingID; \
            layout.name = #ShaderParameterName; \
            layout.gpuData = rpi::RPICreateBufferView(This->m_gpu_data, currentOffset, sizeof(ParameterType)); \
            layout.offset = currentOffset; \
            layout.width = sizeof(ParameterType); \
            layout.cpuData = (void*)((char*)This->m_cpu_data + currentOffset); \
            This->VariableName = (ParameterType*)layout.cpuData; \
            This->m_shader_parameter_layouts.push_back(layout); \
        } \
    };

#define INTERNAL_SHADER_PARAMETER(ParameterType,ShaderParameterName,VariableName, BindingID) INTERNAL_SHADER_PARAMETER_WITH_ID(ParameterType,ShaderParameterName,VariableName,BindingID,__COUNTER__)

namespace yjw
{
    struct InternalShaderParameterLayout
    {
        std::string name;
        int binding_id;
        int offset;
        int width;
        rpi::RPIBuffer gpuData;
        void* cpuData;
    };

    class InternalShaderParameter
    {
        DECALE_INTERNAL_SHADER_PARAMETER();
    public:
        void Initialize();
        void Destroy();
        void FlushCpuDataToGpu();
        rpi::RPIBuffer GetGpuBufferByShaderParameterName(const std::string& shaderName);
        bool IsShaderParameterNameInternal(const std::string& shaderName);
        rpi::RPIResourceSet GetCommonResourceSet();
    public:
        INTERNAL_SHADER_PARAMETER(InternalShaderData::Camera, camera, m_camera, 0);
        INTERNAL_SHADER_PARAMETER(InternalShaderData::Light, light, m_light, 1);
        INTERNAL_SHADER_PARAMETER(InternalShaderData::Option, option, m_option, 2);
    private:
        std::vector<InternalShaderParameterLayout> m_shader_parameter_layouts;
        rpi::RPIBuffer m_gpu_data = rpi::RPIBuffer::Null;
        void* m_cpu_data = nullptr;
        int m_total_size = 0;

        rpi::RPIResourceSet m_common_resource_set{};
    };

    extern InternalShaderParameter g_internal_shader_parameters;
}