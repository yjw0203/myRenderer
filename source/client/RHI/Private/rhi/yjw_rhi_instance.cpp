#include "Public/RHI/rhi/yjw_rhi_instance.h"
#include "Private/vulkan/yjw_vulkan_instance.h"
#include "Public/RHI/externs/imgui/yjw_rhi_imgui_layer.h"

namespace rhi
{
    RHIInstanceImpl::RHIInstanceImpl(const RHIInstanceConfig& config)
    {
        m_config = config;
        for (RHILayerType layerType : config.layers)
        {
            switch (layerType)
            {
            case rhi_layer_imgui: m_rhi_layers[rhi_layer_imgui] = new RHIImguiLayer(config.rhiType); break;
            }
        }
    }

    RHIInstanceImpl::~RHIInstanceImpl()
    {
        for (int index = 0; index < rhi_layer_count; index++)
        {
            if (m_rhi_layers[index])
            {
                delete m_rhi_layers[index];
            }
        }
    }

    const RHIInstanceConfig& RHIInstanceImpl::GetConfig()
    {
        return m_config;
    }

    RHILayer* RHIInstanceImpl::GetRHILayer(RHILayerType type)
    {
        return m_rhi_layers[type];
    }

    void RHIInstanceImpl::OnInstanceInit(RHIInstanceImpl* instance)
    {
        for (RHILayerType layerType : GetConfig().layers)
        {
            m_rhi_layers[layerType]->OnInstanceInit(instance);
        }
    }

    void RHIInstanceImpl::OnDeviceInit(RHIDevice* device)
    {
        for (RHILayerType layerType : GetConfig().layers)
        {
            m_rhi_layers[layerType]->OnDeviceInit(device);
        }
    }

    void RHIInstanceImpl::OnDeviceShutdown(RHIDevice* device)
    {
        for (RHILayerType layerType : GetConfig().layers)
        {
            m_rhi_layers[layerType]->OnDeviceShutdown(device);
        }
    }

    RHIInstance::RHIInstance(RHIInstanceConfig config)
    {
        m_impl = new VulkanInstance(config);
    }

    RHIInstance::~RHIInstance()
    {
        m_impl->release();
    }

    RHIDevice* RHIInstance::CreateDevice()
    {
        return m_impl->CreateDevice();
    }

    RHILayer* RHIInstance::GetRHILayer(RHILayerType type)
    {
        return m_impl->GetRHILayer(type);
    }
}