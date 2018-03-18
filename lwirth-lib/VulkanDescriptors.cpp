#include "stdafx.hpp"
#include "VulkanDescriptors.hpp"

#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

#include "Exceptions.hpp"

namespace lw
{
	namespace VK
	{
		void DescriptorSetLayout::init(Device* device)
		{
			m_device = device;

			VkDescriptorSetLayoutCreateInfo layoutInfo;
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.pNext = nullptr;
			layoutInfo.flags = 0;
			layoutInfo.bindingCount = m_bindings.size();
			layoutInfo.pBindings = m_bindings.data();

			if (vkCreateDescriptorSetLayout(m_device->raw(), &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
			{
				throw VulkanException("failed to create descriptor set layout");
			}
		}

		void DescriptorSetLayout::destroy()
		{
			if (m_layout != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorSetLayout(m_device->raw(), m_layout, nullptr);
				m_layout = VK_NULL_HANDLE;
				m_device = nullptr;
				m_bindings.clear();
			}
		}

		void DescriptorSetLayout::addBinding(const VkDescriptorSetLayoutBinding& binding)
		{
			if (m_layout != VK_NULL_HANDLE)
			{
				throw VulkanException("descriptor set layout was already created");
			}
			m_bindings.push_back(binding);
		}

		void DescriptorSetLayout::addBinding(U32 binding, VkDescriptorType type, U32 descriptorCount, VkShaderStageFlags stageFlags, const VkSampler * immutableSampler)
		{
			VkDescriptorSetLayoutBinding bindInfo;
			bindInfo.binding = binding;;
			bindInfo.descriptorType = type;
			bindInfo.descriptorCount = descriptorCount;
			bindInfo.stageFlags = stageFlags;
			bindInfo.pImmutableSamplers = immutableSampler;
			addBinding(bindInfo);
		}

		//------DescriptorSet-------

		void DescriptorSet::init(Device * device, DescriptorPool * pool, DescriptorSetLayout * layout)
		{
			m_device = device;

			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.pNext = nullptr;
			allocInfo.descriptorPool = pool->raw();
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &layout->raw();

			if (vkAllocateDescriptorSets(m_device->raw(), &allocInfo, &m_set) != VK_SUCCESS)
			{
				throw VulkanException("failed to allocate descriptor set");
			}


			std::vector<VkWriteDescriptorSet> descriptorWrites;
			for (U32 i = 0; i < m_bufferDescriptors.size(); i++)
			{
				VkWriteDescriptorSet write;
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = nullptr;
				write.dstSet = m_set;
				write.dstBinding = m_bufferDescriptors[i].m_binding;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				write.pImageInfo = nullptr;
				write.pBufferInfo = &m_bufferDescriptors[i].m_info;
				write.pTexelBufferView = nullptr;

				descriptorWrites.push_back(write);
			}

			for (U32 i = 0; i < m_imageDescriptors.size(); i++)
			{
				VkWriteDescriptorSet write;
				write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				write.pNext = nullptr;
				write.dstSet = m_set;
				write.dstBinding = m_imageDescriptors[i].m_binding;
				write.dstArrayElement = 0;
				write.descriptorCount = 1;
				write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				write.pImageInfo = &m_imageDescriptors[i].m_info;
				write.pBufferInfo = nullptr;
				write.pTexelBufferView = nullptr;

				descriptorWrites.push_back(write);
			}

			vkUpdateDescriptorSets(m_device->raw(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

		}

		void DescriptorSet::addUniformBuffer(const Buffer * buffer, U32 binding)
		{
			if (m_set != VK_NULL_HANDLE)
			{
				throw VulkanException("Descriptor Set was already created");
			}

			VkDescriptorBufferInfo info;
			info.buffer = buffer->raw();
			info.offset = 0;
			info.range = buffer->size();

			m_bufferDescriptors.push_back(DescriptorBuffer(info, binding));
		}

		/*void DescriptorSet::addImageSampler(const Image * image, U32 binding)
		{
			if (m_set != VK_NULL_HANDLE)
			{
				throw VulkanException("Descriptor Set was already created");
			}

			VkDescriptorImageInfo info;
			info.sampler = image.sampler();
			info.imageView = image.view();
			info.imageLayout = image.layout();

			m_imageDescriptors.push_back(DescriptorImage(info, binding));
		}*/

		void DescriptorPool::init(Device * device)
		{
			m_device = device;

			U32 setCount = 0;
			U32 uniformBufferCount = 0;
			U32 combinedImageSamplerCount = 0;
			U32 samplerCount = 0;

			for (U32 i = 0; i < m_setLayoutContainers.size(); i++)
			{
				for (U32 k = 0; k < m_setLayoutContainers[i].m_pLayouts->m_bindings.size(); k++)
				{
					switch (m_setLayoutContainers[i].m_pLayouts->m_bindings[k].descriptorType)
					{
					case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
						uniformBufferCount += m_setLayoutContainers[i].m_pLayouts->m_bindings[k].descriptorCount * m_setLayoutContainers[i].m_setCount;
						break;
					case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
						combinedImageSamplerCount += m_setLayoutContainers[i].m_pLayouts->m_bindings[k].descriptorCount * m_setLayoutContainers[i].m_setCount;
						break;
					case VK_DESCRIPTOR_TYPE_SAMPLER:
						samplerCount += m_setLayoutContainers[i].m_pLayouts->m_bindings[k].descriptorCount * m_setLayoutContainers[i].m_setCount;
						break;
					default:
						throw VulkanException("descriptor type is not supported");
					}
					
					setCount += m_setLayoutContainers[i].m_setCount;
				}
			}

			std::vector<VkDescriptorPoolSize> poolSizes;
			if (uniformBufferCount > 0)
			{
				VkDescriptorPoolSize dps;
				dps.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				dps.descriptorCount = uniformBufferCount;
				poolSizes.push_back(dps);
			}
			if (combinedImageSamplerCount > 0)
			{
				VkDescriptorPoolSize dps;
				dps.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				dps.descriptorCount = combinedImageSamplerCount;
				poolSizes.push_back(dps);
			}
			if (samplerCount > 0)
			{
				VkDescriptorPoolSize dps;
				dps.type = VK_DESCRIPTOR_TYPE_SAMPLER;
				dps.descriptorCount = samplerCount;
				poolSizes.push_back(dps);
			}

			VkDescriptorPoolCreateInfo dpci;
			dpci.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			dpci.pNext = nullptr;
			dpci.flags = 0;
			dpci.maxSets = setCount;
			dpci.poolSizeCount = poolSizes.size();
			dpci.pPoolSizes = poolSizes.data();

			if (vkCreateDescriptorPool(m_device->raw(), &dpci, nullptr, &m_pool) != VK_SUCCESS)
			{
				throw VulkanException("failed to create descriptor pool");
			}
		}

		void DescriptorPool::destroy()
		{
			if (m_pool != VK_NULL_HANDLE)
			{
				vkDestroyDescriptorPool(m_device->raw(), m_pool, nullptr);
				m_pool = VK_NULL_HANDLE;
				m_device = nullptr;
				m_setLayoutContainers.clear();
			}
		}

		void DescriptorPool::addLayout(const DescriptorSetLayout * layout, U32 setCount)
		{
			if (m_pool != VK_NULL_HANDLE)
			{
				throw VulkanException("descriptor pool was already created");
			}
			m_setLayoutContainers.push_back(DescriptorSetLayoutContainer(layout, setCount));
		}
}
}