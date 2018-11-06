#include "CorpusManager.h"

#include "../CogiSim.h"
#include "../Graphics/CogiRen.h"

#define _USE_MATH_DEFINES
#include <math.h>

sf::Vector2f sfmlVec(const lw::Vector2& vec)
{
	return sf::Vector2f(vec.x, vec.y);
}

sf::Vector2f sfmlVec(const lw::Point2& p)
{
	return sf::Vector2f(p.x, p.y);
}

sf::Color sfmlColor(const lw::Color& c)
{
	return sf::Color(c.r, c.g, c.b, c.a);
}

CorpusManager::CorpusManager()
{
	m_vertices.setPrimitiveType(sf::Triangles);
}

Cogicorpus* CorpusManager::cc(unsigned index)
{
	if (index >= m_cogicorpus.size())return nullptr;
	return &m_cogicorpus[index];
}

unsigned CorpusManager::size()
{
	return m_cogicorpus.size();
}

void CorpusManager::update(lw::phy::Duration& duration)
{
	while (size() < minCorpusAmount)createCorpus();

	for (unsigned i = 0; i < size(); i++)
	{
		neuralCalculation(i);
		physics(i, duration);

		if (cc(i)->m_energy > maxEnergy)cc(i)->m_energy = maxEnergy;

		cc(i)->m_age += 1;

		if (cc(i)->m_energy < minEnergy)
		{
			deleteCorpus(i);
		}
	}

}

void CorpusManager::render()
{
	unsigned verticesAmount = 6; // 2*3 because of feeler triangle
	unsigned cogiCount = m_cogicorpus.size();
	m_vertices.resize(cogiCount * verticesAmount);

	for (unsigned i = 0; i < cogiCount; i++)
	{
		if (cc(i) == nullptr)continue;
		
		auto color = sfmlColor(cc(i)->m_color);
		auto pos = cc(i)->m_pos;
		auto vel = cc(i)->m_vel;
		//auto triangle = cc(i)->getShape())
		auto feelerPos = cc(i)->m_feelerPos;

		sf::Vertex* triangle = &m_vertices[i * verticesAmount];


		float x = pos.x;
		float y = pos.y;

		float angle = vel.heading();
		/*
		triangle[0].position = sf::Vector2f(x + cos(angle) * size.x, y + sin(angle) * size.y);
		triangle[1].position = sf::Vector2f(x + cos(angle + M_PI - 0.5f) * size.x, y + sin(angle + M_PI - 0.5f) * size.y);
		triangle[2].position = sf::Vector2f(x + cos(angle - M_PI + 0.5f) * size.x, y + sin(angle - M_PI + 0.5f) * size.y);
		

		triangle[0].color = color;
		triangle[1].color = color;
		triangle[2].color = color;

		//render feeler
		triangle[3].position = sfmlVec(feelerPos);
		triangle[4].position = triangle[1].position;
		triangle[5].position = triangle[2].position;

		color = sf::Color(250, 50, 50, 100);
		triangle[3].color = color;
		triangle[4].color = color;
		triangle[5].color = color;
		*/
	}
	CogiRen::inst().m_window.draw(m_vertices);
}


void CorpusManager::physics(unsigned i, lw::phy::Duration& duration)
{
	Terrain& t = CogiSim::inst().mTerrain;

	//frition
	if (cc(i)->m_vel.x > 0.1f || cc(i)->m_vel.y > 0.1f)
	{
		lw::Vector2 frictionForce = -cc(i)->m_vel;
		frictionForce.norm();
		frictionForce *= 0.01f * cc(i)->m_mass;
		cc(i)->applyForce(frictionForce);
	}
	else
	{
		cc(i)->m_vel *= 0;
	}
	

	//teleportaion -> no borderlines
	
	auto& pos = cc(i)->m_pos;

	if (pos.x < t.tiles[0][0].pos.x)
	{
		pos.x = t.tiles.back().back().pos.x + t.tileSize;
	}
	if (pos.y < t.tiles[0][0].pos.y)
	{
		pos.y = t.tiles.back().back().pos.y + t.tileSize;
	}
	if (pos.x > t.tiles.back().back().pos.x + t.tileSize)
	{
		pos.x = t.tiles[0][0].pos.x;
	}
	if (pos.y > t.tiles.back().back().pos.y + t.tileSize)
	{
		pos.y = t.tiles[0][0].pos.y;
	}

	//damage according to tileType

	if (tile(i)->type == Tile::grass)
	{

	}
	else if (tile(i)->type == Tile::stone)
	{

	}
	else if (tile(i)->type == Tile::water)
	{
		cc(i)->m_energy -= 1.f;
	}
	
	cc(i)->update(duration);

	//maxSpeed
	float magSquared = cc(i)->m_vel.magSqd();
	if (magSquared > pow(maxSpeed, 2))
	{
		float mag = sqrt(magSquared);
		cc(i)->m_vel *= maxSpeed / mag;
	}

}

void CorpusManager::neuralCalculation(unsigned i)
{
	Cogicorpus& c = *cc(i);
	
	c.m_brain.net.getInput(c.m_brain.inVelx) = c.m_vel.x / maxSpeed;
	c.m_brain.net.getInput(c.m_brain.inVely) = c.m_vel.y / maxSpeed;
	c.m_brain.net.getInput(c.m_brain.inOwnEnergy) = c.m_energy / maxEnergy;
	c.m_brain.net.getInput(c.m_brain.inAge) = c.m_age;
	c.m_brain.net.getInput(c.m_brain.inFeetTileType) = tile(i) == nullptr ? 0 : tile(i)->type;
	c.m_brain.net.getInput(c.m_brain.inAge) = feelerTile(i) == nullptr ? 0 : feelerTile(i)->type;

	c.m_brain.net.compute();

	actMove(i, c.m_brain.net.getOutput(c.m_brain.outMovex), c.m_brain.net.getOutput(c.m_brain.outMovey));
	actMoveFeeler(i, c.m_brain.net.getOutput(c.m_brain.outMoveFeelerLength), c.m_brain.net.getOutput(c.m_brain.outMoveFeelerAngle));
	if (c.m_brain.net.getOutput(c.m_brain.outReproduce) < 0)actReproduce(i);
	
}


void CorpusManager::createCorpus()
{
	Cogicorpus c;

	lw::Vector2 pos = m_random.nextPoint2(CogiSim::inst().mTerrain.tiles.back().back().pos);

	c.m_vel = m_random.nextVector2(maxSpeed, maxSpeed);
	
	c.m_color = m_random.nextColor();
	c.m_energy = startEnergy;

	c.m_brain.net.generateHiddenNeurons(1, 5);
	c.m_brain.net.generateMesh();
	c.m_brain.net.randomizeWeights();

	m_cogicorpus.push_back(c);
}


void CorpusManager::createCorpus(unsigned i)
{
	Cogicorpus c;

	//pos
	c.m_pos = cc(i)->m_pos;
	c.m_pos.x += m_random.nextF32(-5.0f, 5.0f);
	c.m_pos.y += m_random.nextF32(-5.0f, 5.0f);

	//vel
	float angle = m_random.nextF32(-M_PI, M_PI);
	float speed = m_random.nextF32(0, maxSpeed);
	c.m_vel.x = speed * cos(angle);
	c.m_vel.y = speed * sin(angle);

	c.m_energy = startEnergy;
	c.m_color = cc(i)->m_color;

	//mutation
	float mutFac = m_random.nextF32(0, 2) * 4.0f;
	c.m_color.r += m_random.nextF32(-mutFac, mutFac);
	c.m_color.g += m_random.nextF32(-mutFac, mutFac);
	c.m_color.b += m_random.nextF32(-mutFac, mutFac);

	
	//c.m_brain = cc(i)->m_brain;

	c.m_brain.net.mutate(mutationFactor);
	

	if (m_random.nextF32() < neuronAmountMutation)
	{
		unsigned layerIndex = m_random.nextIndex(c.m_brain.net.hiddenLayerCount());
		if (m_random.nextBool())
		{
			c.m_brain.net.addHiddenNeuronAndMesh(layerIndex);
		}
		else {
			if (c.m_brain.net.neuronCount(layerIndex + 1) > 2)
			{
				c.m_brain.net.removeHiddenNeuronAndMesh(layerIndex);
			}
		}
	}

	m_cogicorpus.push_back(c);
}

void CorpusManager::deleteCorpus(unsigned i)
{
	m_cogicorpus.erase(m_cogicorpus.begin() + i);
}



//actions

void CorpusManager::actMove(unsigned i, float moveX, float moveY)
{
	//TODO improve moving system
	lw::Vector2 force = lw::Vector2(moveX, moveY);
	force.setMag(maxAcceleration);

	cc(i)->applyForce(force);
}

void CorpusManager::actReproduce(unsigned i)
{
	if (reproductionEnergy < cc(i)->m_energy)createCorpus(i);
	cc(i)->m_energy -= reproductionEnergy;
}

void CorpusManager::actMoveFeeler(unsigned i, float nnFeelerLength, float nnFeelerAngle)
{
	nnFeelerLength = lw::map(nnFeelerLength, -1.f, 1.f, 0.f, maxFeelerLength);
	nnFeelerAngle = lw::map(nnFeelerAngle, -1.f, 1.f, -lw::PI, lw::PI) + cc(i)->m_vel.heading() - lw::PI;

	cc(i)->m_feelerPos.x = cc(i)->m_pos.x + cos(nnFeelerAngle) * maxFeelerLength;
	cc(i)->m_feelerPos.y = cc(i)->m_pos.y + sin(nnFeelerAngle) * maxFeelerLength;
	
}


Cogicorpus::Cogicorpus()
	: Object(lw::Triangle::createIsosceles(lw::Point2(), 12.f, 20.f), 1.f)
{
}

Cogicorpus::~Cogicorpus()
{
}


Tile* CorpusManager::tile(unsigned i)
{
	unsigned x = static_cast<unsigned>(cc(i)->m_pos.x / CogiSim::inst().mTerrain.tileSize);
	unsigned y = static_cast<unsigned>(cc(i)->m_pos.y / CogiSim::inst().mTerrain.tileSize);

	if (x >= 0 && y >= 0 && x < CogiSim::inst().mTerrain.WIDTH && y < CogiSim::inst().mTerrain.HEIGHT)
		return &CogiSim::inst().mTerrain.tiles[x][y];
	else
		return nullptr;
}

Tile* CorpusManager::feelerTile(unsigned i)
{
	unsigned x = static_cast<unsigned>(cc(i)->m_feelerPos.x / CogiSim::inst().mTerrain.tileSize);
	unsigned y = static_cast<unsigned>(cc(i)->m_feelerPos.y / CogiSim::inst().mTerrain.tileSize);

	if (x >= 0 && y >= 0 && x < CogiSim::inst().mTerrain.WIDTH && y < CogiSim::inst().mTerrain.HEIGHT)
		return &CogiSim::inst().mTerrain.tiles[x][y];
	else
		return nullptr;
}
