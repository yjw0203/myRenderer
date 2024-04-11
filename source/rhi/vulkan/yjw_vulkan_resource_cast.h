#pragma once
#include "RHI/rhi/yjw_rhi_shader.h"
#include "yjw_vulkan_shader.h"

namespace rhi
{
    class VulkanShader;
    class VulkanRenderPass;
    class VulkanRenderPipeline;
    class VulkanBuffer;
    class VulkanBufferView;
    class VulkanTexture;
    class VulkanTextureView;
    class VulkanSwapChain;
    class VulkanResourceBinding;

    template<class T>
    struct TVulkanResourceTraits
    {
    };
    template<>
    struct TVulkanResourceTraits<RHIShader>
    {
        typedef VulkanShader TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHIRenderPass>
    {
        typedef VulkanRenderPass TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHIRenderPipeline>
    {
        typedef VulkanRenderPipeline TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHIBuffer>
    {
        typedef VulkanBuffer TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHIBufferView>
    {
        typedef VulkanBufferView TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHITexture>
    {
        typedef VulkanTexture TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHITextureView>
    {
        typedef VulkanTextureView TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHISwapChain>
    {
        typedef VulkanSwapChain TConcreteType;
    };
    template<>
    struct TVulkanResourceTraits<RHIResourceBinding>
    {
        typedef VulkanResourceBinding TConcreteType;
    };

    template<typename TRHIType>
    static inline typename TVulkanResourceTraits<TRHIType>::TConcreteType* VKResourceCast(TRHIType* Resource)
    {
        return static_cast<typename TVulkanResourceTraits<TRHIType>::TConcreteType*>(Resource);
    }
}
