#pragma once

#include "../Standard.hpp"

#include <string>
#include "../memory/List.hpp"
#include <fstream>

namespace lw
{
	extern lw::List<char> readBinFile(const std::string& filePath);
	extern std::string readTextFile(const std::string & filePath);
}



