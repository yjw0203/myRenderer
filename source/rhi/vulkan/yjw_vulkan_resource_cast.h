#pragma once
#include "RHI/rhi/yjw_rhi_shader.h"
#include "yjw_vulkan_shader.h"

namespace rhi
{
    class VulkanShader;

    template<class T>
    struct TVulkanResourceTraits
    {
    };
    template<>
    struct TVulkanResourceTraits<RHIShader>
    {
        typedef VulkanShader TConcreteType;
    };

    template<typename TRHIType>
    static inline typename TVulkanResourceTraits<TRHIType>::TConcreteType* ResourceCast(TRHIType* Resource)
    {
        return static_cast<typename TVulkanResourceTraits<TRHIType>::TConcreteType*>(Resource);
    }
}
