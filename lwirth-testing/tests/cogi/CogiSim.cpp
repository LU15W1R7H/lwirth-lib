#include "CogiSim.h"

CogiSim::CogiSim()
{
}

CogiSim::~CogiSim()
{
	if(thread.joinable())thread.join();
}

void CogiSim::start()
{
	thread = std::thread(&CogiSim::run, this);
}

void CogiSim::run()
{
	init();
	while (m_running)
	{
		update();
	}
}

void CogiSim::init()
{
	
}

void CogiSim::update()
{
	auto duration = mClock.tick();

	std::this_thread::sleep_for(std::chrono::milliseconds(m_threadSleep)); //TODO move up
	mTerrain.update();
	
	mCorpusManager.update(duration);
	
}


