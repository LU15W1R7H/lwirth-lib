#pragma once

#include "../Standard.hpp"

#include "Color.hpp"
#include "vulkan/VulkanImage.hpp"

namespace lw
{
	class Texture
	{
		VK::Image m_vkImage;

	public:
		Texture();
		~Texture();

		void create(u32 width, u32 height);
		void loadFromFile();
		void update(Color* pColors);
	};

}


