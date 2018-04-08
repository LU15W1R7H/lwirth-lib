#include "lwirth.hpp"

#define ABSSIN(x) std::abs(sin(x))

class Boi
{
public:
	float maxVel = 0.01f;
	float maxAcc = 0.0000001f;
	float maxColorChange = 0.01f;

	lw::Random rand;


	lw::Vertex2DArray va;
	lw::Vec2 vels[3];
	lw::Vec2 accs[3];
	int run = 0;

	Boi()
		: va(3)
	{
		for (size_t i = 0; i < va.size(); i++)
		{
			va[i] = rand.nextVertex();
			vels[i] = rand.nextVector2();
			vels[i].setMag(maxVel);
		}
	}

	void update1()
	{
		va[0].pos.x = std::sin(run * 0.0001f);
		va[0].pos.y = std::cos(run * 0.0001f);
		va[0].color = lw::Color(ABSSIN(run * 0.0001), ABSSIN(run * 0.0003), ABSSIN(run * 0.0002 + 2));
		va[1].pos.x = std::sin(run * 0.0001f + 1.5f);
		va[1].pos.y = std::cos(run * 0.0001f + 1.5f);
		va[1].color = lw::Color(ABSSIN(run * 0.0003 + 3), ABSSIN(run * 0.0002 + 2), ABSSIN(run * 0.0001 + 1));
		va[2].pos.x = std::sin(run * 0.0001f + 3.f);
		va[2].pos.y = std::cos(run * 0.0001f + 3.f);
		va[2].color = lw::Color(ABSSIN(run * 0.0002 + 1), ABSSIN(run * 0.0001 + 3), ABSSIN(run * 0.0003 + 2));
		run += 50;
	}

	void update2()
	{
		for (size_t i = 0; i < va.size(); i++)
		{
			if (va[i].pos.x < -1)	vels[i].x *= -1;
			if (va[i].pos.x > 1)	vels[i].x *= -1;
			if (va[i].pos.y < -1)	vels[i].y *= -1;
			if (va[i].pos.y > 1)	vels[i].y *= -1;

			//accs[i] += rand.nextVector2(-maxAcc, maxAcc, -maxAcc, maxAcc);
			accs[i].x = -vels[i].x * 0.0001;
			accs[i].y = -vels[i].y * 0.0001;

			vels[i] += accs[i];
			va[i].pos += vels[i];
		}

		
		va[0].color = lw::Color(ABSSIN(run * 0.0001), ABSSIN(run * 0.0003), ABSSIN(run * 0.0002 + 2));
		va[1].color = lw::Color(ABSSIN(run * 0.0003 + 3), ABSSIN(run * 0.0002 + 2), ABSSIN(run * 0.0001 + 1));
		va[2].color = lw::Color(ABSSIN(run * 0.0002 + 1), ABSSIN(run * 0.0001 + 3), ABSSIN(run * 0.0003 + 2));

		run += 50;
	}
};


void vk()
{
	F32 run = 0;

	lw::init();

	lw::Random rand;
	lw::RenderWindow window;
	window.create("LWIRTH-LIB", 1000, 800);

	Boi boi;
	Boi boi2;
	boi2.run = rand.nextI32(10000);

	lw::Root& r = lw::Root::get();

	lw::Color color;

	while (window.isOpen())
	{
		run+= 1;
		window.update();
		auto brush = window.getVulkan()->getSimpleBrush2D();
		
		window.preDraw();

		F32 step = 0.001f;
		

		for (F32 i = -1; i < 1; i+=step)
		{
			for (F32 j = -1; j < 1; j+=step)
			{
				

				color.r = -run * i + 3;
				color.g = run * j + 6;
				color.b = -run * (i + j) + 8;

				brush->setColor(color);
				brush->drawPoint(i, j);
			}
			
		}

		boi.update1();
		boi2.update2();
		//brush->fillVertexArray(boi.va);
		//brush->fillVertexArray(boi2.va);

		

		window.postDraw();

	}
	window.destroy();
	lw::terminate();
}