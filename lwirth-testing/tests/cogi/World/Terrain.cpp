#include "Terrain.h"

#include "..\Graphics\CogiRen.h"


Tile::Tile(lw::Vec2 pos)
	: pos(pos)
{
	
}


Tile::~Tile()
{
}



void Tile::update()
{
	
}

Terrain::Terrain()
{
	//init PerlinNoise
	

	//create tiles
	for (unsigned i = 0; i < WIDTH; i++)
	{
		std::vector<Tile> vec;
		for (unsigned j = 0; j < HEIGHT; j++)
		{
			Tile tile = Tile(lw::Vector2(i*tileSize, j*tileSize));
			float value = perlinNoise.octaveNoise0_1(i/10.0f, j/10.0f, 3);
			
			if (value < 0.33f)tile.type = Tile::grass;
			else if (value < 0.66f)tile.type = Tile::water;
			else tile.type = Tile::stone;
			vec.push_back(tile);
		}
		tiles.push_back(vec);
	}

	//create vertices for rendering
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(WIDTH * HEIGHT * 4);

	for (unsigned i = 0; i < WIDTH; i++)
	{
		for (unsigned j = 0; j < HEIGHT; j++)
		{
			sf::Vertex* quad = &m_vertices[(i + j*WIDTH) * 4];

			quad[0].position = lw::Vec2(i * tileSize, j * tileSize);
			quad[1].position = lw::Vec2((i+1) * tileSize, j * tileSize);
			quad[2].position = lw::Vec2((i+1) * tileSize, (j+1) * tileSize);
			quad[3].position = lw::Vec2(i * tileSize, (j+1) * tileSize);


			quad[0].color = lw::Color(70, 70, 70);
			quad[1].color = lw::Color(50, 50, 50);
			quad[2].color = lw::Color(70, 70, 70);
			quad[3].color = lw::Color(50, 50, 50);
		}
	}

}


void Terrain::update()
{
	
	for (unsigned x = 0; x < WIDTH; x++)
	{
		for (unsigned y = 0; y < HEIGHT; y++)
		{
			Tile& tile = tiles[x][y];
			tile.update();

		}
	}
}

void Terrain::render()
{
	for (unsigned i = 0; i < WIDTH; i++)
	{
		for (unsigned j = 0; j < HEIGHT; j++)
		{
			lw::Vertex* quad = &m_vertices[(i + j*WIDTH) * 4];
			lw::Color color;
			switch (tiles[i][j].type)
			{
			case Tile::grass:
				color = grassColor;
				break;
			case Tile::water:
				color = waterColor;
				break;
			case Tile::stone:
				color = stoneColor;
				break;
			}

			quad[0].color = color;
			quad[1].color = color;
			quad[2].color = color;
			quad[3].color = color;
		}
	}

	CogiRen::inst().m_window.draw(m_vertices);
	
}
