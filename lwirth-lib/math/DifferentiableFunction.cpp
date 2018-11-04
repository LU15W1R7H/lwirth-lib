#include "DifferentiableFunction.hpp"

#include "Math.hpp"

namespace lw
{
	f32 costFunction(f32 o, f32 y)
	{
		return 0.5f * lw::pow(o - y, 2.f);
	}

	f32 costFunctionDerivative(f32 o, f32 y)
	{
		return o - y;
	}

	Eigen::VectorXf costFunctionDerivative(const Eigen::VectorXf& o, const Eigen::VectorXf& y)
	{
		return o - y;
	}

	f32 sigmoid(f32 x)
	{
		return 1.f / (1.f + lw::exp(-x));
	}

	f32 sigmoidDerivative(f32 x)
	{
		return sigmoid(x) * (1.f - sigmoid(x));
	}
}

