#include "../stdafx.hpp"
#include "FileManager.hpp"

namespace lw
{
	lw::List<char> readBinFile(const std::string& filePath)
	{
		//#TODO: change from char to byte(unsigned char)

		std::ifstream stream(filePath, std::ios::ate | std::ios::binary);

		if (!stream.is_open())
			throw std::runtime_error("failed to open file");

		u32 fileSize = static_cast<u32>(stream.tellg());
		lw::List<char> buffer(fileSize);
		stream.seekg(0);
		stream.read(buffer.raw(), fileSize);
		stream.close();
		return buffer;
	}
	std::string readTextFile(const std::string & filePath)
	{
		
		std::string text;
		std::ifstream file(filePath);

		if (file.is_open())
		{
			std::string line;
			while (std::getline(file, line))
			{
				text += line + "\n";
			}
			file.close();
		}
		else
		{
			throw std::runtime_error("failed to open file: " + filePath);
		}

		return text;
	}
}
