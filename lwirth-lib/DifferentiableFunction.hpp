#pragma once

#include "Standard.hpp"

namespace lw
{
	f32 costFunction(f32 o, f32 y);
	f32 costFunctionDerivative(f32 o, f32 y);

	f32 sigmoid(f32 x);
	f32 sigmoidDerivative(f32 x);
}



