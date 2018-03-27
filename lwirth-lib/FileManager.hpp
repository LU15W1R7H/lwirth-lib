#pragma once

#include "Standard.hpp"

#include <string>
#include "DynamicArray.hpp"
#include <fstream>

namespace lw
{
	extern lw::DynamicArray<Byte> readBinFile(const std::string& filePath);
}



