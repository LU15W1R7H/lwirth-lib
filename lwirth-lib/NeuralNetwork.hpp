#pragma once

#include "Standard.hpp"

#include <math.h>
#include <vector>
#include "Random.hpp"

namespace lw
{
	
	
	struct Neuron
	{
	public:
		enum NeuronType { input, hidden, output, bias, undefined };

		Neuron(NeuronType type);

		NeuronType type = undefined;

		std::string name = "UNDEFINED";
		f32 value = 1.0f;
	};

	struct Layer
	{
		enum LayerType { input, hidden, output, undefined };

		Layer() = delete;
		Layer(LayerType type);

		LayerType type = undefined;

		std::vector<Neuron> neurons;
	};


	class NeuralNetwork
	{
	private:
		//avoid using neurons & weights directly
		std::vector<Layer> m_layers;
		std::vector<std::vector<std::vector<f32>>> m_weights;

		Random m_random;
	public:
		NeuralNetwork();
		~NeuralNetwork() = default;

		size_t createInput(std::string name);
		size_t createOutput(std::string name);
		f32& getInput(size_t index);
		f32& getInput(std::string name);
		f32& getOutput(size_t index);
		f32& getOutput(std::string name);

		void generateHiddenNeurons(size_t numLayers, size_t numNeurons);

		void generateMesh();
		void randomizeWeights();
		void mutate(f32 factor);

		void compute();

		void addHiddenNeuronAndMesh(size_t hiddenLayerIndex);
		void removeHiddenNeuronAndMesh(size_t hiddenLayerIndex);
		//void addHiddenLayerAndMesh(size_t neuronCount);
		//void removeHiddenLayerAndMesh(size_t hiddenLayerIndex);

		size_t hiddenLayerCount();
		size_t neuronCount(size_t layerIndex);
	private:
		bool m_meshed = false;

		size_t createLayer(Layer::LayerType layerType);
		size_t createHiddenNeuron(size_t hiddenLayerIndex);

		f32 activationFunction(f32 value);
	};



}