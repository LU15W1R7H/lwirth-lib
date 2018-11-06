#pragma once

#include <thread>
#include "World\Terrain.h"
#include "Cogicorpus\CorpusManager.h"

class CogiSim
{
public:
	static CogiSim& inst()
	{
		static CogiSim sim;
		return sim;
	}

	void start();
	bool m_running = true;
	std::thread thread;

	unsigned m_threadSleep = 0;

	Terrain mTerrain;
	CorpusManager mCorpusManager;

private://TODO private public evaluation
	CogiSim();
	~CogiSim();

	void init();
	void update();
	void run();
	
};

