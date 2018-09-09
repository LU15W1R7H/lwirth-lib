#ifndef LWIRTH_VULKANTRIANGLESTEST_HPP
#define LWIRTH_VULKANTRIANGLESTEST_HPP

#include "lwirth.hpp"

#include <Eigen/Dense>

void trianglesTest()
{
    f32 run = 0;


    lw::Random rand;
    lw::RenderWindow window;
    window.create("LWIRTH-LIB", 1000, 800);

    //lw::List<Boi> bois(20);

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
}


#define ABSSIN(x) std::abs(sin(x))


class Boi
{
public:
    float maxVel = 0.01f;
    float maxAcc = 0.0000001f;
    float maxColorChange = 0.01f;

    lw::Random rand;


    lw::Vertex2DArray va;
    Eigen::Vector2f vels[3];
    Eigen::Vector2f accs[3];
    int run = 0;

    Boi()
            : va(3, lw::Triangles)
    {
        for (size_t i = 0; i < va.size(); i++)
        {
            va[i] = rand.nextVertex2D();
            vels[i] = rand.nextVector(2, -maxVel, maxVel);


        }
        run = rand.nextU32();
    }

    void update1()
    {
        va[0].pos.x() = std::sin(run * 0.0001f);
        va[0].pos.y() = std::cos(run * 0.0001f);
        va[0].color = lw::Color(ABSSIN(run * 0.0001), ABSSIN(run * 0.0003), ABSSIN(run * 0.0002 + 2));
        va[1].pos.x() = std::sin(run * 0.0001f + 1.5f);
        va[1].pos.y() = std::cos(run * 0.0001f + 1.5f);
        va[1].color = lw::Color(ABSSIN(run * 0.0003 + 3), ABSSIN(run * 0.0002 + 2), ABSSIN(run * 0.0001 + 1));
        va[2].pos.x() = std::sin(run * 0.0001f + 3.f);
        va[2].pos.y() = std::cos(run * 0.0001f + 3.f);
        va[2].color = lw::Color(ABSSIN(run * 0.0002 + 1), ABSSIN(run * 0.0001 + 3), ABSSIN(run * 0.0003 + 2));
        /*va[3].pos.x = std::sin(run * 0.0001f + 4.f);
        va[3].pos.y = std::cos(run * 0.0001f + 4.f);
        va[3].color = lw::Color(ABSSIN(run * 0.0005 + 5), ABSSIN(run * 0.0006 + 6), ABSSIN(run * 0.0002 + 9));*/
        run += 50;
    }

    void update2()
    {
        for (size_t i = 0; i < va.size(); i++)
        {
            if (va[i].pos.x() < -1)	vels[i].x() *= -1;
            if (va[i].pos.x() > 1)	vels[i].x() *= -1;
            if (va[i].pos.y() < -1)	vels[i].y() *= -1;
            if (va[i].pos.y() > 1)	vels[i].y() *= -1;

            //accs[i] += rand.nextVector2(-maxAcc, maxAcc, -maxAcc, maxAcc);
            accs[i].x() = -vels[i].x() * 0.0001;
            accs[i].y() = -vels[i].y() * 0.0001;

            vels[i] += accs[i];
            va[i].pos += vels[i];
        }


        va[0].color = lw::Color(ABSSIN(run * 0.0001), ABSSIN(run * 0.0003), ABSSIN(run * 0.0002 + 2));
        va[1].color = lw::Color(ABSSIN(run * 0.0003 + 3), ABSSIN(run * 0.0002 + 2), ABSSIN(run * 0.0001 + 1));
        va[2].color = lw::Color(ABSSIN(run * 0.0002 + 1), ABSSIN(run * 0.0001 + 3), ABSSIN(run * 0.0003 + 2));

        run += 50;
    }
};

#endif //LWIRTH_VULKANTRIANGLESTEST_HPP
