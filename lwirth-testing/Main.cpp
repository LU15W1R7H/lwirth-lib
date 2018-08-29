#include "lwirth.hpp"

#include "GFX.hpp"

using namespace lw;

#include <iostream>

#include <eigen/Dense>

int main()
{
    glfwInit();
	//vk();
	GLFWwindow* window;
	GLFWmonitor** monitors;
	int monitorCount;
	monitors = glfwGetMonitors(&monitorCount);



	window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr);
	std::cout << glfwVulkanSupported();
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cin.get();
}