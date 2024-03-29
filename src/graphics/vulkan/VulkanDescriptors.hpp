#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../../memory/List.hpp"
#include <vector>

namespace lw
{
	namespace VK
	{
		class Device;
		class DescriptorPool;
		class Buffer;
		class Image;

		class DescriptorBuffer
		{
		public:
			DescriptorBuffer(VkDescriptorBufferInfo info, u32 binding)
				: m_info(info), m_binding(binding) {}

			VkDescriptorBufferInfo m_info;
			u32 m_binding;
		};

		class DescriptorImage
		{
		public:
			DescriptorImage(VkDescriptorImageInfo info, u32 binding)
				: m_info(info), m_binding(binding) {}
			VkDescriptorImageInfo m_info;
			u32 m_binding;
		};

		class DescriptorSetLayout
		{
			friend class DescriptorPool;
		private:
			VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
			Device* m_device = nullptr;

			List<VkDescriptorSetLayoutBinding> m_bindings = {};
		public:
			DescriptorSetLayout() = default;
			~DescriptorSetLayout() = default;

			void init(Device* device);
			void destroy();

			const VkDescriptorSetLayout& raw() const { return m_layout; }

			void addBinding(const VkDescriptorSetLayoutBinding& binding);
			void addBinding(u32 binding, VkDescriptorType type, u32 descriptorCount, VkShaderStageFlags stageFlags, const VkSampler* immutableSampler = nullptr);
		};

		class DescriptorSetLayoutContainer
		{
		public:
			const DescriptorSetLayout * m_pLayouts;
			u32 m_setCount;

			DescriptorSetLayoutContainer(const DescriptorSetLayout* layout, u32 setCount)
				: m_pLayouts(layout), m_setCount(setCount) {}
		};

		class DescriptorSet
		{
		private:
			VkDescriptorSet m_set = VK_NULL_HANDLE;
			Device* m_device = nullptr;

			std::vector<DescriptorBuffer> m_bufferDescriptors = {};
			std::vector<DescriptorImage> m_imageDescriptors = {};
		public:
			DescriptorSet() = default;
			~DescriptorSet() = default;

			void init(Device* device, DescriptorPool* pool, DescriptorSetLayout* layout);

			VkDescriptorSet raw() const { return m_set; }

			void addUniformBuffer(const Buffer* buffer, u32 binding);
			void addImageSampler(const Image& image, u32 binding);
		};

		class DescriptorPool
		{
		private:
			VkDescriptorPool m_pool = VK_NULL_HANDLE;
			Device* m_device = nullptr;

			std::vector<DescriptorSetLayoutContainer> m_setLayoutContainers = {};
		public:
			DescriptorPool() = default;
			~DescriptorPool() = default;

			void init(Device* device);
			void destroy();

			VkDescriptorPool raw() const { return m_pool; }

			void addLayout(const DescriptorSetLayout* layout, u32 setCount);
		};

		

	}
}