#pragma once

#include <thread>
#include <lwirth.hpp>

#include "Camera.h"

class CogiRen
{
public:
	static CogiRen& inst()
	{
		static CogiRen sim;
		return sim;
	}

	void start();
	std::thread thread;

	lw::RenderWindow m_window;
	Camera m_camera;


private://TODO private public evaluation
	CogiRen();
	~CogiRen();

	void run();
	void init();
	void update();
	bool render = true;
};

