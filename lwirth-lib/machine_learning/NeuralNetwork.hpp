#pragma once

#include "../Standard.hpp"

#include "../memory/List.hpp"
#include "../random/Random.hpp"
#include <Dense>

namespace lw
{

	//#TODO: do testing
	class NeuralNetwork
	{
	private:
	    List<size_t> m_shape;
		List<Eigen::MatrixXf> m_weights;
		List<Eigen::VectorXf> m_biases;

		Random m_random;

		f32 m_eta = 0.01f;
		size_t m_miniBatchSize = 100;

		const f32 INIT_WEIGHT_RANGE = 0.01f;


	public:
		NeuralNetwork() = delete;
		NeuralNetwork(const lw::List<size_t>& shape);
		~NeuralNetwork() = default;

		Eigen::VectorXf feedforward(Eigen::VectorXf inputs) const;
		void mutate(f32 range);

		void train(const Eigen::VectorXf* inputs, const Eigen::VectorXf* outputs, size_t sampleCount, size_t epochs);

	private:
	    List<Eigen::MatrixXf> createShapedWeights() const;
	    List<Eigen::VectorXf> createShapedBiases() const;

		void trainMiniBatch(const Eigen::VectorXf* inputs, const Eigen::VectorXf* outputs, size_t start, size_t end);
		std::pair<List<Eigen::MatrixXf>, List<Eigen::VectorXf>> backprop(const Eigen::VectorXf& x, const Eigen::VectorXf& y);
	};
}