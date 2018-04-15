#pragma once

#include "lwirth.hpp"

class Cell
{
public:
	F32 m_energy;

};


class CellSystem
{
public:
	lw::Random rand;
	Cell** m_cells;
	lw::Vertex2DArray m_va;

	size_t m_width, m_height;

public:
	CellSystem(size_t width, size_t height)
		: m_width(width), m_height(height), m_va(width * height * 4, lw::Quads)
	{
		m_cells = new Cell*[m_width];

		for (size_t i = 0; i < m_width; i++)
		{
			m_cells[i] = new Cell[m_height];
		}

		F32 sizeOfCell = lw::min(2.f / m_width, 2.f / m_height);

		size_t index = 0;

		for (size_t i = 0; i < m_width; i++)
		{
			for (size_t j = 0; j < m_height; j++)
			{

				m_va[index * 4 + 0].pos = { -1.f + sizeOfCell * i, -1.f + sizeOfCell * j };
				m_va[index * 4 + 1].pos = { -1.f + sizeOfCell * (i + 1), -1.f + sizeOfCell * j };
				m_va[index * 4 + 2].pos = { -1.f + sizeOfCell * (i + 1), -1.f + sizeOfCell * (j + 1) };
				m_va[index * 4 + 3].pos = { -1.f + sizeOfCell * i, -1.f + sizeOfCell * (j + 1) };		
																										
																										
				

				index++;
			}
		}
	}

	F32 energ(int i, int j)
	{
		if (i >= m_width || i < 0 || j >= m_height || j < 0)return 0;

		return m_cells[i][j].m_energy;
	}

	void update()
	{
		for (size_t i = 0; i < m_width; i++)
		{
			for (size_t j = 0; j < m_height; j++)
			{
				m_cells[i][j].m_energy += energ(i - 1, j - 1) / 1.2f;
				
				
			}
		}

		size_t amount = rand.nextU32(200);

		for(size_t u = 0; u < amount; u++)
		{
			size_t i = rand.nextIndex(m_width);
			size_t j = rand.nextIndex(m_height);
			m_cells[i][j].m_energy = rand.nextU32();
		}
	}


	void render(lw::SimpleBrush2D* brush)
	{
		size_t index = 0;
		for (size_t i = 0; i < m_width; i++)
		{
			for (size_t j = 0; j < m_height; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					m_va[index*4 + k].color = m_cells[i][j].m_energy;

				}
				index++;
			}
		}

		brush->drawVertexArray(m_va);
	}
};

