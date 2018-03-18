#pragma once

#include "Standard.hpp"

#include <math.h>
#include <vector>
#include "Random.hpp"

namespace lw
{
	

	struct API Neuron
	{
	public:
		enum NeuronType { input, hidden, output, bias, undefined };

		Neuron(NeuronType type);

		NeuronType type = undefined;

		std::string name = "UNDEFINED";
		F32 value = 1.0f;
	};

	struct API Layer
	{
		enum LayerType { input, hidden, output, undefined };

		Layer() = delete;
		Layer(LayerType type);

		LayerType type = undefined;

		std::vector<Neuron> neurons;
	};

	class API NeuralNetwork
	{
	private:
		//avoid using neurons & weights directly
		std::vector<Layer> m_layers;
		std::vector<std::vector<std::vector<F32>>> m_weights;

		Random m_random;
	public:
		NeuralNetwork();
		~NeuralNetwork() = default;

		Size createInput(std::string name);
		Size createOutput(std::string name);
		F32& getInput(Size index);
		F32& getInput(std::string name);
		F32& getOutput(Size index);
		F32& getOutput(std::string name);

		void generateHiddenNeurons(Size numLayers, Size numNeurons);

		void generateMesh();
		void randomizeWeights();
		void mutate(F32 factor);

		void compute();

		void addHiddenNeuronAndMesh(Size hiddenLayerIndex);
		void removeHiddenNeuronAndMesh(Size hiddenLayerIndex);
		//void addHiddenLayerAndMesh(Size neuronCount);
		//void removeHiddenLayerAndMesh(Size hiddenLayerIndex);

		Size hiddenLayerCount();
		Size neuronCount(Size layerIndex);
	private:
		bool m_meshed = false;

		Size createLayer(Layer::LayerType layerType);
		Size createHiddenNeuron(Size hiddenLayerIndex);

		F32 activationFunction(F32 value);
	};



}