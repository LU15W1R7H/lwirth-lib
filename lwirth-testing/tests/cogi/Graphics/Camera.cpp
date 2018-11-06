#include "Camera.h"

#include "CogiRen.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::update()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		move(sf::Vector2f(m_prevMousePos - sf::Mouse::getPosition(CogiRen::inst().m_window)) * m_zoomFactor);
	}


	if (m_follow != nullptr)setPos(*m_follow);

	m_prevMousePos = sf::Mouse::getPosition(CogiRen::inst().m_window);
}

void Camera::setPos(float x, float y)
{
	sf::View view = CogiRen::inst().m_window.getView();
	view.setCenter(x, y);
	CogiRen::inst().m_window.setView(view);
}

void Camera::setPos(lw::Vector2 pos)
{
	setPos(pos.x, pos.y);
}

void Camera::move(float xOff, float yOff)
{
	sf::View view = CogiRen::inst().m_window.getView();
	view.move(xOff, yOff);
	CogiRen::inst().m_window.setView(view);
}

void Camera::move(sf::Vector2f offset)
{
	move(offset.x, offset.y);
}

void Camera::zoom(float factor, sf::Vector2i point)
{
	m_zoomFactor *= factor;
	const sf::Vector2f beforeCoord = CogiRen::inst().m_window.mapPixelToCoords(point);
	sf::View view = CogiRen::inst().m_window.getView();
	view.zoom(factor);
	CogiRen::inst().m_window.setView(view);
	const sf::Vector2f afterCoord = CogiRen::inst().m_window.mapPixelToCoords(point);
	const sf::Vector2f offsetCoords = beforeCoord - afterCoord;
	view.move(offsetCoords);
	CogiRen::inst().m_window.setView(view);
}
