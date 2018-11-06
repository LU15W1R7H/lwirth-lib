#pragma once


#include "../World/Terrain.h"
#include "../Graphics/Camera.h"

#include <lwirth.hpp>

class Cogicorpus;

class CorpusManager
{

public:
	unsigned minCorpusAmount = 50;
	float maxAcceleration = 0.5f;
	float maxSpeed = 3.f;
	float maxFeelerLength = 1000;

	float startEnergy = 200;
	float maxEnergy = 10000;
	float minEnergy = 100;

	float reproductionEnergy = startEnergy + 100;

	unsigned memoryNeuronAmount = 3;
	unsigned maxStartNeuronAmount = 10;

	float mutationFactor = 5;
	float neuronAmountMutation = 0.02f;

public:
	CorpusManager();
	~CorpusManager() = default;

	Cogicorpus* cc(unsigned index);
	unsigned size();
	void update(float duration);
	void render();
	
	void createCorpus();
	void createCorpus(unsigned motherIndex);
	void deleteCorpus(unsigned index);

	Tile* tile(unsigned index);
	Tile* feelerTile(unsigned index);

private:
	std::vector<Cogicorpus> m_cogicorpus;

	void physics(unsigned index, float duration);
	void neuralCalculation(unsigned index);

	lw::Vertex2DArray	m_vertices;

	//actions
	void actMove(unsigned index, float nnAcceleration, float nnAngle);
	void actReproduce(unsigned index);

	void actMoveFeeler(unsigned index, float nnFeelerLength, float nnFeelerAngle);

	lw::Random m_random;
};

struct CorpusBrain
{
public:
	lw::NeuralNetwork net;

	/*unsigned inVelx = net.createInput("VelX");
	unsigned inVely = net.createInput("VelY");
	unsigned inOwnEnergy = net.createInput("OwnEnergy");
	unsigned inAge = net.createInput("Age");
	unsigned inFeetTileType = net.createInput("FeetTileType");
	unsigned inFeelerTileType = net.createInput("FeelerTileType");

	unsigned outMovex = net.createOutput("MoveX");
	unsigned outMovey = net.createOutput("MoveY");
	unsigned outEat = net.createOutput("Eat");
	unsigned outMoveFeelerLength = net.createOutput("MoveFeelerLength");
	unsigned outMoveFeelerAngle = net.createOutput("MoveFeelerAngle");
	unsigned outReproduce = net.createOutput("Reproduce");*/
};

class Cogicorpus
{
public:
	Cogicorpus();
	~Cogicorpus();



	std::string m_name;
	lw::Color m_color;
	unsigned m_generation = 0;
	unsigned m_age = 0;
	float m_energy = 0;

	CorpusBrain m_brain;

	lw::Vec2 m_feelerPos;
};

