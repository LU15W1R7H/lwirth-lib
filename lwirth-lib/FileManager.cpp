#include "stdafx.hpp"
#include "FileManager.hpp"

namespace lw
{
	lw::DynamicArray<Byte> readBinFile(const std::string& filePath)
	{
		{
			std::ifstream file(filePath, std::ios::ate | std::ios::binary);

			if (!file.is_open())
			{
				throw std::runtime_error("failed to open file");
			}

			U32 fileSize = static_cast<U32>(file.tellg());
			lw::DynamicArray<char> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.raw(), fileSize);

			file.close();
			return buffer;
		}
	}
}
