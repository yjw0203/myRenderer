#pragma once
#include "vulkan/vulkan.h"
#include "rhi/vulkan/yjw_vulkan_global.h"
#include "rhi/common/yjw_rhi_resource_allocator.h"
#include "rhi/vulkan/yjw_vulkan_resource_ruler.h"
#include "rhi/vulkan/resource/yjw_vulkan_buffer.h"
#include "rhi/vulkan/resource/yjw_vulkan_texture.h"
namespace vulkan
{
	enum class VulkanDescriptorType
	{
		uniform_buffer,
		shader_resource_texture,
		depth_stencil,
		render_target,
		unordered_access
	};

	struct VulkanDescriptorCreation
	{
		VulkanResourceType resource_type;
		VulkanDescriptorType descriptor_type;
		union{
			VulkanBufferHandle buffer;
			VulkanTextureHandle texture;
		}handle;
		VkFormat format;

	};

	class VulkanDescriptor
	{
	public:
		struct BufferView
		{
			VkBuffer buffer;
			int offset;
			int size;
		};
		struct ImageView
		{
			VkImageView view;
			int width;
			int height;
		};
		static const VulkanResourceType TypeId = VulkanResourceType::descriptor;
		typedef VulkanDescriptorCreation Creation;
		operator VkBuffer() { return payload.bufferView.buffer; }
		operator VkDeviceMemory() { return memory; }
		operator VkImageView() { return payload.imageView.view; }
		int getBufferSize() { return payload.bufferView.size; }
		int getBufferOffset() { return payload.bufferView.offset; }
		int getImageWidth() { return payload.imageView.width; }
		int getImageHeight() { return payload.imageView.height; }
		VulkanDescriptor() {}
		union {
			BufferView bufferView;
			ImageView imageView;
		}payload{};
		VkDeviceMemory memory{};
		VulkanResourceType resource_type;
		VulkanDescriptorType descriptor_type;
	};
	typedef ResourceHandle<VulkanDescriptor> VulkanDescriptorHandle;

	struct DefaultVulkanDescriptorAllocateStrategy
	{
		VulkanDescriptor* CreateFunc(VulkanDescriptorCreation& creation);
		void DestoryFunc(VulkanDescriptor* resource);
	};

	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorHandle create(VulkanDescriptorCreation& creation);
		void destory(VulkanDescriptorHandle descriptor);
	private:
		ResourceAllocator<VulkanDescriptor, DefaultVulkanDescriptorAllocateStrategy> DefaultAllocator;
	};

	EXTERN_GLOBAL_REF(VulkanDescriptorPool);
}