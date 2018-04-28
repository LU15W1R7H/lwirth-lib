#pragma once

#include "Standard.hpp"

#include <string>
#include "List.hpp"
#include <fstream>

namespace lw
{
	extern lw::List<byte> readBinFile(const std::string& filePath);
	extern std::string readTextFile(const std::string & filePath);
}



