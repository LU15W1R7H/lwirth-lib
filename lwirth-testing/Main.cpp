#include "lwirth.hpp"

//#include "GFX.hpp"

using namespace lw;

#include <iostream>

#include <eigen/Dense>

int main()
{
	lw::NeuralNetwork nn({ 1, 5, 10, 5, 1 });
	Eigen::VectorXf input = Eigen::VectorXf::Constant(1, 5.f);
	auto output = nn.feedforward(input);
	std::cout << output << std::endl;
	nn.mutate(0.001f);
	output = nn.feedforward(input);
	std::cout << output << std::endl;

	std::cin.get();
}