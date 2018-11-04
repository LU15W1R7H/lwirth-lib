#pragma once

#include "../Standard.hpp"

#include <Dense>

namespace lw
{
	f32 costFunction(f32 o, f32 y);
	f32 costFunctionDerivative(f32 o, f32 y);
	Eigen::VectorXf costFunctionDerivative(const Eigen::VectorXf& o, const Eigen::VectorXf& y);

	f32 sigmoid(f32 x);
	f32 sigmoidDerivative(f32 x);
}



