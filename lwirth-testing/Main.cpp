#include "lwirth.hpp"
#include <string>
#include "Matrix4x4.hpp"
#include <thread>

#include <chrono>
using namespace std::chrono_literals;

#define print(x) std::cout << x
#define println(x) std::cout << x << std::endl
#define SIN(x) std::abs(sin(x))
#define WAIT() std::this_thread::sleep_for(3s)

void vk();
class Boi;

using namespace lw;

int main(int argc, char** argv)
{
	Vector2 vec2(3, 5);
	auto rotMat = Matrix2::createRotation(PI);
	auto rotatedVec = rotMat * vec2;
	std::cout << vec2 << std::endl;
	std::cout << rotatedVec << std::endl;


	vk();

	WAIT();
}

class Boi
{
public:
	float maxVel = 0.0001f;
	float maxAcc = 0.0000001f;
	float maxColorChange = 0.01f;

	lw::Random rand;


	lw::VertexArray va;
	lw::Vector2 vels[3];
	lw::Vector2 accs[3];
	int run = 0;

	Boi()
		: va(3)
	{
		for (Size i = 0; i < va.size(); i++)
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
		va[0].color = lw::Color(SIN(run * 0.0001), SIN(run * 0.0003), SIN(run * 0.0002 + 2));
		va[1].pos.x = std::sin(run * 0.0001f + 1.5f);
		va[1].pos.y = std::cos(run * 0.0001f + 1.5f);
		va[1].color = lw::Color(SIN(run * 0.0003 + 3), SIN(run * 0.0002 + 2), SIN(run * 0.0001 + 1));
		va[2].pos.x = std::sin(run * 0.0001f + 3.f);
		va[2].pos.y = std::cos(run * 0.0001f + 3.f);
		va[2].color = lw::Color(SIN(run * 0.0002 + 1), SIN(run * 0.0001 + 3), SIN(run * 0.0003 + 2));
		run+=50;
	}

	void update2()
	{
		accs[0] += rand.nextVector2(-maxAcc, maxAcc, -maxAcc, maxAcc);
		accs[1] += rand.nextVector2(-maxAcc, maxAcc, -maxAcc, maxAcc);
		accs[2] += rand.nextVector2(-maxAcc, maxAcc, -maxAcc, maxAcc);

		vels[0] += accs[0];
		vels[1] += accs[1];
		vels[2] += accs[2];
		va[0].pos += vels[0];
		va[1].pos += vels[1];
		va[2].pos += vels[2];


		va[0].color = lw::Color(SIN(run * 0.0001), SIN(run * 0.0003), SIN(run * 0.0002 + 2));
		va[1].color = lw::Color(SIN(run * 0.0003 + 3), SIN(run * 0.0002 + 2), SIN(run * 0.0001 + 1));
		va[2].color = lw::Color(SIN(run * 0.0002 + 1), SIN(run * 0.0001 + 3), SIN(run * 0.0003 + 2));

		run+=50;
	}
};


void vk()
{
	int run = 0;

	lw::init();

	lw::DynamicArray<int> da;
	da.push(321);
	da.push(23461);
	da.push(23462431);
	std::cout << da[2] << std::endl;

	lw::Random rand;
	lw::RenderWindow window;
	window.create("LWIRTH-LIB", 1000, 800);

	Boi boi;
	Boi boi2;
	boi2.run = rand.nextI32(10000);

	lw::Root& r = lw::Root::get();

	while (window.isOpen())
	{

		window.update();
		lw::SimpleBrush2D* brush = window.preDraw();
		boi.update1();
		boi2.update2();
		brush->fillVertexArray(boi.va);
		brush->fillVertexArray(boi2.va);

		window.postDraw();

	}
	window.destroy();
	lw::terminate();
}


