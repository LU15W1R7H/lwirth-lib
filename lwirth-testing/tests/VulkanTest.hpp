//
// Created by Luis on 29-Sep-18.
//

#ifndef LWIRTH_VULKANTEST_HPP
#define LWIRTH_VULKANTEST_HPP

#include "lwirth.hpp"

#include <Eigen/Dense>

int vulkanTest()
{
    lw::Random rand;
    lw::RenderWindow window;
    window.create("Hello lwirth", 1000, 800);

    while(window.isOpen())
    {
        window.update();

        auto brush = window.getVulkan()->getSimpleBrush2D();
        window.preDraw();



        brush->setColor(rand.nextColor());


        for (size_t i = 0; i < 100; ++i) {
            for (int j = 0; j < 100; ++j) {
                brush->drawLine(rand.nextVertex2D(), rand.nextVertex2D());
            }
        }

        window.postDraw();
    }

    window.destroy();

    std::cin.get();

    return 0;
}


#endif //LWIRTH_VULKANTEST_HPP
