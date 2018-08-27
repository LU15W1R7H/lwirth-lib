#include "../stdafx.hpp"
#include "FileManager.hpp"

namespace lw
{
	lw::List<byte> readBinFile(const std::string& filePath)
	{
		//#TODO: fix this! Not working at the moment

		std::basic_ifstream<byte, std::char_traits<byte>> stream(filePath, std::ios::ate | std::ios::binary);

		if (!stream.is_open())
			throw std::runtime_error("failed to open file");

		u32 fileSize = static_cast<u32>(stream.tellg());
		lw::List<byte> buffer(fileSize);
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
