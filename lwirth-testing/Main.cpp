#include "lwirth.hpp"

#include "GFX.hpp"

using namespace lw;

#include <iostream>

#include <eigen/Dense>

int main()
{
	Eigen::Vector3f vec = { 2.f, 2.f, 1.f };
	std::cout << vec.norm() << '\n';
	std::cout << vec << '\n';

	std::cin.get();
}