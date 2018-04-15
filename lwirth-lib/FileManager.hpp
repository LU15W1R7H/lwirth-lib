#pragma once

#include "Standard.hpp"

#include <string>
#include "DynamicArray.hpp"
#include <fstream>

namespace lw
{
	extern lw::DynamicArray<byte> readBinFile(const std::string& filePath);
}



