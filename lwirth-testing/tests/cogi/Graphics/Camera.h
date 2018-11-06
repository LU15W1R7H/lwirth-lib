#pragma once

#include <utility>

class Camera
{
public:
	Camera();
	~Camera();

	void update();

	void setPos(float x, float y);
	void setPos(lw::Vec2 pos);
	void move(float xOff, float yOff);
	void move(lw::Vec2 offset);
	void zoom(float factor, std::pair<int, int> point);

	lw::Vec2* m_follow = nullptr;

private:
	std::pair<int, int> m_prevMousePos = {0, 0};
	float m_zoomFactor = 1;

	bool rightMouseButton = false;
};

