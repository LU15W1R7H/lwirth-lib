#include "stdafx.hpp"
#include "NeuralNetwork.hpp"

#include "Utils.hpp"
#include <math.h>

namespace lw
{


	Neuron::Neuron(NeuronType type)
		: type(type)
	{
	}

	Layer::Layer(LayerType type)
		: type(type)
	{}

	NeuralNetwork::NeuralNetwork()
	{
		createLayer(Layer::input);
		createLayer(Layer::output);
	}

	size_t NeuralNetwork::createLayer(Layer::LayerType layerType)
	{
		if (m_meshed)throw std::logic_error("Net is already meshed");

		Layer newLayer(layerType);
		switch (layerType)
		{
		case Layer::input:
			if (m_layers.size() != 0)throw std::logic_error("InputLayer must be created first");
			newLayer.neurons.push_back(Neuron(Neuron::bias)); //create Bias
			m_layers.push_back(newLayer);
			return 0;
		case Layer::output:
			if (m_layers.size() != 1)throw std::logic_error("OutputLayer must be created second");
			m_layers.push_back(newLayer);
			return m_layers.size() - 1;
		case Layer::hidden:
			if (m_layers.size() < 2)throw std::logic_error("Input- and OutputLayer must be created first");
			newLayer.neurons.push_back(Neuron(Neuron::bias));
			m_layers.insert(m_layers.end() - 1, newLayer);
			return m_layers.size() - 2;
		}

		throw std::runtime_error("LayerType doesn't exist");
	}

	size_t NeuralNetwork::createHiddenNeuron(size_t hiddenLayerIndex)
	{
		size_t layerIndex = hiddenLayerIndex + 1;
		if (layerIndex >= m_layers.size())throw std::logic_error("HiddenLayer doesn't exist");
		m_layers[layerIndex].neurons.push_back(Neuron(Neuron::hidden));
		return m_layers[layerIndex].neurons.size() - 1;
	}

	

	size_t NeuralNetwork::createInput(std::string name)
	{
		if (m_meshed)throw std::logic_error("Net is already meshed");
		Neuron newInput(Neuron::input);
		newInput.name = name;
		m_layers.front().neurons.push_back(newInput);
		return m_layers.front().neurons.size() - 1;
	}

	size_t NeuralNetwork::createOutput(std::string name)
	{
		if (m_meshed)throw std::logic_error("Net is already meshed");
		Neuron newOutput(Neuron::output);
		newOutput.name = name;
		m_layers.back().neurons.push_back(newOutput);
		return m_layers.back().neurons.size() - 1;
	}

	f32& NeuralNetwork::getInput(size_t index)
	{
		return m_layers.front().neurons[index].value;
	}

	f32& NeuralNetwork::getInput(std::string name)
	{
		for (size_t i = 0; i < m_layers.front().neurons.size(); i++)
		{
			if (m_layers.front().neurons[i].name == name)return getInput(i);
		}
		throw std::runtime_error("input doesn't exist");
	}

	f32& NeuralNetwork::getOutput(size_t index)
	{
		return m_layers.back().neurons[index].value;
	}

	f32& NeuralNetwork::getOutput(std::string name)
	{
		for (size_t i = 0; i < m_layers.front().neurons.size(); i++)
		{
			if (m_layers.back().neurons[i].name == name)return getOutput(i);
		}
		throw std::runtime_error("output doesn't exist");
	}

	void NeuralNetwork::generateHiddenNeurons(size_t numLayers, size_t numNeurons)
	{
		if (m_meshed)throw std::logic_error("Net is already meshed");

		for (size_t i = 0; i < numLayers; i++)
		{
			createLayer(Layer::hidden);
			for (size_t k = 0; k < numNeurons; k++)
			{
				createHiddenNeuron(hiddenLayerCount() - 1);
			}
		}
	}

	void NeuralNetwork::generateMesh()
	{
		if (m_meshed)throw std::logic_error("Net is already meshed");

		for (size_t layer = 0; layer < m_layers.size() - 1; layer++)
		{
			std::vector<std::vector<f32>> layerWeights;
			for (size_t entry = 0; entry < m_layers[layer].neurons.size(); entry++)
			{
				std::vector<f32> neuronWeights;
				for (size_t exit = 0; exit < m_layers[layer + 1].neurons.size(); exit++)
				{
					neuronWeights.push_back(1.0f);
				}
				layerWeights.push_back(neuronWeights);
			}
			m_weights.push_back(layerWeights);
		}

		m_meshed = true;
	}

	void NeuralNetwork::randomizeWeights()
	{
		f32 factor = m_layers.size() / (2.f * std::sqrtf(static_cast<f32>(m_layers.size()))) + 0.5f;
		for (size_t i = 0; i < m_weights.size(); i++)
		{
			for (size_t j = 0; j < m_weights[i].size(); j++)
			{
				for (size_t k = 0; k < m_weights[i][j].size(); k++)
				{
					m_weights[i][j][k] = m_random.nextF32(-factor, factor);
				}
			}
		}
	}

	void NeuralNetwork::mutate(f32 factor)
	{
		u32 amount = m_random.nextU32(m_layers.size() * m_layers[1].neurons.size());

		for (size_t i = 0; i < amount; i++)
		{
			size_t layer = m_random.nextIndex(m_weights.size());
			size_t entry = m_random.nextIndex(m_layers[layer].neurons.size());
			size_t exit = m_random.nextIndex(m_layers[layer + 1].neurons.size());
			m_weights[layer][entry][exit] += m_random.nextF32(-factor, factor);
		}
	}

	void NeuralNetwork::compute()
	{
		for (size_t layer = 0; layer < m_layers.size() - 1; layer++)
		{
			for (size_t exit = 0; exit < m_layers[layer + 1].neurons.size(); exit++)
			{
				if (m_layers[layer + 1].neurons[exit].type == Neuron::bias)continue;
				f32 sum = 0;
				for (size_t entry = 0; entry < m_layers[layer].neurons.size(); entry++)
				{
					sum += m_layers[layer].neurons[entry].value * m_weights[layer][entry][exit];
				}
				m_layers[layer + 1].neurons[exit].value = activationFunction(sum);
			}
		}
	}

	void NeuralNetwork::addHiddenNeuronAndMesh(size_t hiddenLayerIndex)
	{
		size_t layerIndex = hiddenLayerIndex + 1;
		if (layerIndex >= m_layers.size())throw std::logic_error("HiddenLayer doesn't exist");
		m_layers[layerIndex].neurons.push_back(Neuron(Neuron::hidden));
		//connect layers in front of
		for (size_t entry = 0; entry < m_layers[layerIndex - 1].neurons.size(); entry++)
		{
			m_weights[layerIndex - 1][entry].push_back(0.0f);
		}
		//connect layers behind
		std::vector<f32> newWeights;
		for (size_t exit = 0; exit < m_layers[layerIndex + 1].neurons.size(); exit++)
		{
			newWeights.push_back(0.0f);
		}
		m_weights[layerIndex].push_back(newWeights);
	}
	
	void NeuralNetwork::removeHiddenNeuronAndMesh(size_t hiddenLayerIndex)
	{
		size_t layerIndex = hiddenLayerIndex + 1;
		if (layerIndex >= m_layers.size())throw std::logic_error("HiddenLayer doesn't exist");
		if (m_layers[layerIndex].neurons.size() < 2)
		{
			throw std::logic_error("HiddenNeuron doesn't exist");
		}
		m_layers[layerIndex].neurons.pop_back();


		//delete connections to layers in front of
		for (size_t entry = 0; entry < m_layers[layerIndex - 1].neurons.size(); entry++)
		{
			m_weights[layerIndex - 1][entry].pop_back();
		}
		//delete connections to layers behind
		m_weights[layerIndex].pop_back();

	}

	f32 NeuralNetwork::activationFunction(f32 value)
	{
		return tanh(value);
	}

	size_t NeuralNetwork::hiddenLayerCount()
	{
		return m_layers.size() - 2;
	}

	size_t NeuralNetwork::neuronCount(size_t layerIndex)
	{
		return m_layers[layerIndex].neurons.size();
	}

}


