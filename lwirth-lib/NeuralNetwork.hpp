#pragma once

#include "Standard.hpp"

#include "List.hpp"
#include "Random.hpp"
#include <eigen/Dense>

namespace lw
{
	class NeuralNetwork
	{
	private:
		List<Eigen::MatrixXf> m_weightMatrices;
		List<Eigen::VectorXf> m_biasVectors;

		Random m_random;

		const f32 INIT_WEIGHT_RANGE = 0.01f;

	public:
		NeuralNetwork() = delete;
		NeuralNetwork(const lw::List<size_t>& shape);
		~NeuralNetwork() = default;

		Eigen::VectorXf feedforward(Eigen::VectorXf inputs);
		void mutate(f32 range);

	private:
		
	};
}