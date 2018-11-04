
#include "NeuralNetwork.hpp"
#include "../math/DifferentiableFunction.hpp"

namespace lw
{
	NeuralNetwork::NeuralNetwork(const lw::List<size_t>& shape)
		: m_shape(shape), m_random()
	{
        for (size_t i = 0; i < m_shape.size() - 1; i++)
        {
            m_weights.push(m_random.nextMatrix(m_shape[i + 1], m_shape[i], -INIT_WEIGHT_RANGE, INIT_WEIGHT_RANGE));
            m_biases.push(m_random.nextVector(m_shape[i + 1], -INIT_WEIGHT_RANGE, INIT_WEIGHT_RANGE));
        }
	}

	Eigen::VectorXf NeuralNetwork::feedforward(Eigen::VectorXf inputs) const
	{
		for (size_t i = 0; i < m_weights.size(); i++)
		{
			inputs = m_weights[i] * inputs + m_biases[i];
			if (i < m_weights.size() - 1)//don't apply activations function on output layer
			{
				inputs = inputs.unaryExpr(&sigmoid);
			}
		}
		return inputs;
	}

	void NeuralNetwork::mutate(f32 range)
	{
		for (size_t i = 0; i < m_weights.size(); i++)
		{
			for (size_t r = 0; r < m_weights[i].rows(); r++)
			{
				m_biases[i](r) += m_random.nextF32(-range, range);
				for (size_t c = 0; c < m_weights[i].cols(); c++)
				{
					m_weights[i](r, c) += m_random.nextF32(-range, range);
				}
			}
		}
	}

    void NeuralNetwork::train(const Eigen::VectorXf *inputs, const Eigen::VectorXf *outputs, size_t sampleCount, size_t epochs)
    {
        for(size_t epoch = 0; epoch < epochs; epoch++)
        {
            size_t start = 0;
            while(true)
            {
                size_t end = start + m_miniBatchSize;
                if(end < sampleCount)
                {
                    trainMiniBatch(inputs, outputs, start, end);
                } else {
                    trainMiniBatch(inputs, outputs, start, sampleCount);
                    break;
                }
                start = end;
            }
        }
    }

    void NeuralNetwork::trainMiniBatch(const Eigen::VectorXf* inputs, const Eigen::VectorXf* outputs, size_t start, size_t end)
    {
	    auto weightsDeltas = createShapedWeights();
	    auto biasesDeltas = createShapedBiases();

        for(size_t i = start; i < end; i++)
        {
            auto result = backprop(inputs[i], outputs[i]);
            auto weightsNablas = result.first;
            auto biasesNablas = result.second;

            for(size_t j = 0; j < m_shape.size(); j++)
            {
                weightsDeltas[j] += weightsNablas[j];
                biasesDeltas[j] += biasesNablas[j];
            }
        }

        for(size_t i = 0; i < m_shape.size(); i++)
        {
            m_weights[i] -= weightsDeltas[i] * (m_eta / (end - start));
            m_biases[i] -= biasesDeltas[i] * (m_eta / (end - start));
        }
    }

    std::pair<List<Eigen::MatrixXf>, List<Eigen::VectorXf>>
    NeuralNetwork::backprop(const Eigen::VectorXf &x, const Eigen::VectorXf &y)
    {
        auto weightsNablas = createShapedWeights();
        auto biasesNablas = createShapedBiases();

        List<Eigen::VectorXf> as = { x };
        List<Eigen::VectorXf> zs = {};

        //forward
        for(size_t i = 0; i < m_shape.size(); i++)
        {
            zs.push(m_weights[i] * as.back() + m_biases[i]);
            if(i < m_shape.size() - 1)//don't apply activation function on output layer
            {
                as.push(zs.back().unaryExpr(&sigmoid));
            }
            else
            {
                as.push(zs.back());
            }
        }

        //backwards
        Eigen::VectorXf delta = costFunctionDerivative(as.back(), y);
        weightsNablas.back() = delta * (as[as.size() - 3]).transpose();
        biasesNablas.back() = delta;

        for(long i = m_shape.size() - 2; i >= 0; i--)
        {
            Eigen::VectorXf sd = zs[i].unaryExpr(&sigmoidDerivative);
            delta = ((m_weights[i + 1].transpose() * delta).array() * sd.array()).matrix();
            weightsNablas[i] = delta * as[i].transpose();
            biasesNablas[i] = delta;
        }

        return std::pair<List<Eigen::MatrixXf>, List<Eigen::VectorXf>>(weightsNablas, biasesNablas);
    }


    List<Eigen::VectorXf> NeuralNetwork::createShapedBiases() const
    {
	    List<Eigen::VectorXf> r;
        for (size_t i = 0; i < m_shape.size() - 1; i++)
        {
            r.push(Eigen::VectorXf::Constant(m_shape[i + 1], 0.f));
        }
        return r;
    }

    List<Eigen::MatrixXf> NeuralNetwork::createShapedWeights() const
    {
        List<Eigen::MatrixXf> r;
        for(size_t i = 0; i < m_shape.size() - 1; i++)
        {
            r.push(Eigen::MatrixXf::Constant(m_shape[i + 1], m_shape[i], 0.f));
        }
        return r;
    }




}