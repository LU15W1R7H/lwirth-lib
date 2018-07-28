#include "lwirth.hpp"

#include "Boi.hpp"
#include "CellSystem.hpp"


void vk()
{
	f32 run = 0;

	lw::init();

	lw::Random rand;
	lw::RenderWindow window;
	window.create("LWIRTH-LIB", 1000, 800);

	lw::List<Boi> bois(20);

	//CellSystem cellSystem(500, 500);

	lw::Vertex2DArray va(16, lw::Quads);

	va[0] = { { -1.f, -1.f },{ 1.0f, 0.0f, 0.0f } }; //left up
	va[1] = { { 1.f, -1.f },{ 0.0f, 1.0f, 0.0f } }; //right up
	va[2] = { { 1.f, 1.f },{ 0.0f, 0.0f, 1.0f } }; //right down
	va[3] = { { -1.f, 1.f },{ 1.0f, 1.0f, 1.0f } }; //left down
	
	//lw::VK::Image image;
	//image.createAndLoadFromFile(window.getVulkan()->m_mainDevice, window.getVulkan()->m_commandPool, "texture.jpg");


	while (window.isOpen())
	{
		run += 1;
		window.update();
		auto brush = window.getVulkan()->getSimpleBrush2D();
		
		window.preDraw();

		for (size_t i = 0; i < bois.size(); i++)
		{
			bois[i].update2();
			brush->drawVertexArray(bois[i].va);
		}
		

		//brush->drawVertexArray(va);
		//cellSystem.update();
		//cellSystem.render(brush);

		for (f32 i = -1.f; i < 1.f; i += 0.01f)
		{
			for (f32 j = -1.f; j < 1.f; j += 0.01f)
			{
				brush->setColor(rand.nextColor());
				brush->drawLine(0, 0, i, j);
			}
		}

		window.postDraw();

	}
	window.destroy();
	lw::terminate();
}