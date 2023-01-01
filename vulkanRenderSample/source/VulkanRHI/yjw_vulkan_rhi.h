#pragma once

#include "../yjw_startup_helper.h"
#include "../yjw_global_delegate.h"
#include "vulkan/vulkan.h"

namespace yjw
{
    class VulkanComponents
    {
    public:
        void createComponents();
        void shutdownComponents();
        VkInstance m_instance = nullptr;
        VkPhysicalDevice m_gpu = nullptr;
        VkDevice m_device = nullptr;

    private:
        void createInstance();
        void createPhysicalDevice();
        void createDevice();
        void shutdownInstance();
        void shutdownPhysicalDevice();
        void shutdownDevice();
    };

    class VulkanRHI
    {
    public:
        static VulkanRHI& get()
        {
            static VulkanRHI instance;
            return instance;
        }
        void startup()
        {
            CoreDelegate::OnApplicationInitialize.Register([this] {this->initialize(); });
            CoreDelegate::OnApplicationShutdown.Register([this] {this->destory(); });
        }
    private:
        void initialize();
        void destory();

        VulkanComponents m_components;
    };

    HelpStartup(VulkanRHI)
}