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

	Boi boi;

	//CellSystem cellSystem(500, 500);

	lw::Vertex2DArray va(16, lw::Quads);

	va[0] = { { -1.f, -1.f },{ 1.0f, 0.0f, 0.0f } }; //left up
	va[1] = { { 1.f, -1.f },{ 0.0f, 1.0f, 0.0f } }; //right up
	va[2] = { { 1.f, 1.f },{ 0.0f, 0.0f, 1.0f } }; //right down
	va[3] = { { -1.f, 1.f },{ 1.0f, 1.0f, 1.0f } }; //left down
	
	


	while (window.isOpen())
	{
		run += 1;
		window.update();
		auto brush = window.getVulkan()->getSimpleBrush2D();
		
		window.preDraw();

		boi.update1();
		brush->drawVertexArray(boi.va);

		//brush->drawVertexArray(va);
		//cellSystem.update();
		//cellSystem.render(brush);

		window.postDraw();

	}
	window.destroy();
	lw::terminate();
}