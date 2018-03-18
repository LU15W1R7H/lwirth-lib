#include "stdafx.hpp"
#include "Utils.hpp"

#include <fstream>


namespace lw
{
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


