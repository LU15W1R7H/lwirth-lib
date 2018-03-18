#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <vector>

namespace lw
{
	namespace VK
	{
		class Device;
		class DescriptorPool;
		class Buffer;

		class API DescriptorBuffer
		{
		public:
			DescriptorBuffer(VkDescriptorBufferInfo info, U32 binding)
				: m_info(info), m_binding(binding) {}

			VkDescriptorBufferInfo m_info;
			U32 m_binding;
		};

		class API DescriptorImage
		{
		public:
			DescriptorImage(VkDescriptorImageInfo info, U32 binding)
				: m_info(info), m_binding(binding) {}
			VkDescriptorImageInfo m_info;
			U32 m_binding;
		};

		class API DescriptorSetLayout
		{
			friend class DescriptorPool;
		private:
			VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
			Device* m_device = nullptr;

			std::vector<VkDescriptorSetLayoutBinding> m_bindings = {};
		public:
			DescriptorSetLayout() {}
			~DescriptorSetLayout() {}

			void init(Device* device);
			void destroy();

			const VkDescriptorSetLayout& raw() const { return m_layout; }

			void addBinding(const VkDescriptorSetLayoutBinding& binding);
			void addBinding(U32 binding, VkDescriptorType type, U32 descriptorCount, VkShaderStageFlags stageFlags, const VkSampler* immutableSampler = nullptr);
		};

		class API DescriptorSetLayoutContainer
		{
		public:
			const DescriptorSetLayout * m_pLayouts;
			U32 m_setCount;

			DescriptorSetLayoutContainer(const DescriptorSetLayout* layout, U32 setCount)
				: m_pLayouts(layout), m_setCount(setCount) {}
		};

		class API DescriptorSet
		{
		private:
			VkDescriptorSet m_set = VK_NULL_HANDLE;
			Device* m_device = nullptr;

			std::vector<DescriptorBuffer> m_bufferDescriptors;
			std::vector<DescriptorImage> m_imageDescriptors;
		public:
			DescriptorSet() {}
			~DescriptorSet() {}

			void init(Device* device, DescriptorPool* pool, DescriptorSetLayout* layout);

			VkDescriptorSet raw() const { return m_set; }

			void addUniformBuffer(const Buffer* buffer, U32 binding);
			//void addImageSampler(const Image* image, U32 binding);
		};

		class API DescriptorPool
		{
		private:
			VkDescriptorPool m_pool = VK_NULL_HANDLE;
			Device* m_device = nullptr;

			std::vector<DescriptorSetLayoutContainer> m_setLayoutContainers = {};
		public:
			DescriptorPool() {}
			~DescriptorPool() {}

			void init(Device* device);
			void destroy();

			VkDescriptorPool raw() const { return m_pool; }

			void addLayout(const DescriptorSetLayout* layout, U32 setCount);
		};

		

	}
}