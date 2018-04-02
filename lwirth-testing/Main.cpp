#include "lwirth.hpp"

#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#include "GFX.hpp"


void sleep(float millisecs)
{
	auto duration = millisecs * 1ms;
	std::this_thread::sleep_for(duration);
}

int main()
{
	vk();
	
	std::cin.get();
}