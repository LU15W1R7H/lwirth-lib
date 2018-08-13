#include "stdafx.hpp"

#include "NeuralNetwork.hpp"
#include "DifferentiableFunction.hpp"

namespace lw
{
	NeuralNetwork::NeuralNetwork(const lw::List<size_t>& shape)
		: m_random()
	{
		for (size_t i = 0; i < shape.size() - 1; i++)
		{
			m_weightMatrices.push(m_random.nextMatrix(shape[i + 1], shape[i], -INIT_WEIGHT_RANGE, INIT_WEIGHT_RANGE));
			m_biasVectors.push(m_random.nextVector(shape[i + 1], -INIT_WEIGHT_RANGE, INIT_WEIGHT_RANGE));
		}
	}

	Eigen::VectorXf NeuralNetwork::feedforward(Eigen::VectorXf inputs)
	{
		for (size_t i = 0; i < m_weightMatrices.size(); i++)
		{
			inputs = m_weightMatrices[i] * inputs + m_biasVectors[i];
			if (i < m_weightMatrices.size() - 1)//don't apply activations function on output layer
			{
				inputs = inputs.unaryExpr(&sigmoid);
			}
		}
		return inputs;
	}

	void NeuralNetwork::mutate(f32 range)
	{
		for (size_t i = 0; i < m_weightMatrices.size(); i++)
		{
			for (size_t r = 0; r < m_weightMatrices[i].rows(); r++)
			{
				m_biasVectors[i](r) += m_random.nextF32(-range, range);
				for (size_t c = 0; c < m_weightMatrices[i].cols(); c++)
				{
					m_weightMatrices[i](r, c) += m_random.nextF32(-range, range);
				}
			}
		}
	}
}