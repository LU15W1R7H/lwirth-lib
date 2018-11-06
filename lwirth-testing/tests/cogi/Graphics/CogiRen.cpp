#include "CogiRen.h"

#include "..\CogiSim.h"

CogiRen::CogiRen()
{
}


CogiRen::~CogiRen()
{
	if(thread.joinable())thread.join();
}

void CogiRen::start()
{
	thread = std::thread(&CogiRen::run, this);
}

void CogiRen::run()
{
	init();
	while (m_window.isOpen())
	{
		update();
	}
	CogiSim::inst().m_running = false;
}

void CogiRen::init()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	m_window.create(sf::VideoMode::getDesktopMode(), "Cogimotus", sf::Style::Default, settings);
	m_window.setSize(sf::Vector2u(500, 500));
	m_window.setFramerateLimit(30);

}

void CogiRen::update()
{
	sf::Event e;
	while (m_window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_window.close();

		else if (e.type == sf::Event::Resized)
			m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y))));

		else if (e.type == sf::Event::KeyPressed)
		{
			if (e.key.code == sf::Keyboard::Escape)m_window.close();
			else if (e.key.code == sf::Keyboard::Up)CogiSim::inst().m_threadSleep = 10;
			else if (e.key.code == sf::Keyboard::Down)CogiSim::inst().m_threadSleep = 0;
			else if (e.key.code == sf::Keyboard::R)render = !render;
		}
		else if (e.type == sf::Event::MouseWheelMoved)
		{
			if (e.mouseWheel.delta < 0)
				m_camera.zoom(1.5f, sf::Vector2i(m_window.getSize().x / 2, m_window.getSize().x / 2));
			else
				m_camera.zoom(0.7f, sf::Mouse::getPosition(m_window));
		}
		else if (e.type == sf::Event::MouseButtonPressed)
		{
			if (e.mouseButton.button == sf::Mouse::Right)
			{
			}
		}
	}
	if (!render)return;

	m_window.clear(sf::Color::Black);
	CogiSim::inst().mTerrain.render();
	CogiSim::inst().mCorpusManager.render();
	m_camera.update();
	m_window.display();

}
