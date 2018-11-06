#pragma once

#include <lwirth.hpp>

class Tile
{
public:
	Tile(lw::Vec2 pos);
	~Tile();

	enum TileType { grass, water, stone };
	TileType type;

	const lw::Vec2 pos;


	void update();

};

struct Terrain
{
public:
	Terrain();
	~Terrain() {}

	std::vector<std::vector<Tile>> tiles;
	unsigned tileSize = 400;
	
	const unsigned WIDTH = 100;
	const unsigned HEIGHT = 100;

	lw::PerlinNoise perlinNoise;

	void update();
	void render();

	lw::Color grassColor = lw::Color(0, 255, 0, 255);
	lw::Color waterColor = lw::Color(0, 0, 255, 255);
	lw::Color stoneColor = lw::Color(100, 100, 100, 255);


private:
	lw::Vertex2DArray m_vertices;

};