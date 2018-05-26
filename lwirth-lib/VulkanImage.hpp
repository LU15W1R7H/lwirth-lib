#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <string>

namespace lw
{
	namespace VK
	{
		class Device;
		class CommandPool;
		class Queue;
		class StagingBuffer;
		class Buffer;

		void transitionImageLayout(const Device & device, const CommandPool & cmdPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		class LWAPI Image
		{
				
		private:
			VkImage m_image = VK_NULL_HANDLE;
			VkDeviceMemory m_memory = VK_NULL_HANDLE;
			VkImageView m_view = VK_NULL_HANDLE;
			VkSampler m_sampler = VK_NULL_HANDLE;

			u32 m_width = 0;
			u32 m_height = 0;
			VkFormat m_format = VK_FORMAT_R8G8B8A8_UNORM;
			VkSamplerAddressMode m_repeatMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			VkFilter m_filterMode = VK_FILTER_LINEAR;
			VkImageLayout m_layout = VK_IMAGE_LAYOUT_MAX_ENUM;

			const Device* m_pDevice = nullptr;

		public:
			void create(const VK::Device& device, u32 width, u32 height, VkFormat format, VkImageUsageFlags usage);
			void createAndLoadFromFile(const VK::Device& device, const CommandPool& cmdPool, const std::string& path);
			void createAndCopyFromBuffer(const VK::Device& device, const CommandPool& cmdPool, u32 width, u32 height, const VK::StagingBuffer& staging);
			void destroy();

			bool exists() const;
			VkImage raw() const;
			VkDeviceMemory getMemory() const;
			VkImageView getView() const;
			VkSampler getSampler() const;
			u32 getWidth() const;
			u32 getHeight() const;
			VkFormat getFormat() const;

			void transitionLayout(const CommandPool& cmdPool, VkImageLayout newLayout);
			
		};

	}
}


