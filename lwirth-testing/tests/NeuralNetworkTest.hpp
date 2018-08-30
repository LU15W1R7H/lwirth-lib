#ifndef LWIRTH_NEURALNETWORKTEST_HPP
#define LWIRTH_NEURALNETWORKTEST_HPP

#include <iostream>
#include <eigen/Dense>
#include <lwirth.hpp>

using namespace lw;

Random random;

void createTrainingData(Eigen::VectorXf* inputs, Eigen::VectorXf* outputs, List<size_t> shape, size_t sampleCount, double sampleMinValue, double sampleMaxValue, std::function<Eigen::VectorXf(Eigen::VectorXf)> function)
{
    if(inputs || outputs)throw MalformedPointerException("Pointer should be null");
    inputs = new Eigen::VectorXf[sampleCount];
    outputs = new Eigen::VectorXf[sampleCount];
    std::cout << "Creating TrainingData..." << '\n';
    for (size_t i = 0; i < sampleCount; i++)
    {
        auto in = random.nextVector(shape.front(), sampleMinValue, sampleMaxValue);
        auto out = function(in);
        inputs[i] = in;
        outputs[i] = out;
    }
}

int nnTest()
{
    //create NN with desired shape
    List<size_t> shape = { 1, 10, 1 };//depth is more efficient than width
    double eta = 0.1;
    size_t miniBatchSize = 1;
    size_t epochs = 1;
    size_t sampleCount = 500000;
    double sampleMinValue = 0.0;
    double sampleMaxValue = 10.0;
    Eigen::VectorXf* inputs = nullptr;
    Eigen::VectorXf* outputs = nullptr;

    std::function<Eigen::VectorXf(Eigen::VectorXf)> function =
            [shape](Eigen::VectorXf x) -> Eigen::VectorXf
            {
                Eigen::VectorXf r(shape.back());
                r(0) = std::sin(x(0));
                return r;
            };

    createTrainingData(inputs, outputs, shape, sampleCount, sampleMinValue, sampleMaxValue, function);
    //loadTrainingData(&data);


    std::cout << "Creating NeuralNetwork..." << '\n';
    NeuralNetwork nn(shape);
    std::cout << "Finished creating NeuralNetwork" << '\n';

    //train NN
    std::cout << "Training NeuralNetwork..." << '\n';
    nn.train(inputs, outputs, sampleCount, epochs);
    std::cout << "Finished training!" << '\n';

    //std::cout << "Test data:" << '\n';
    //for(double d = sampleMinValue; d < sampleMaxValue; d+=0.01)
    //{
    //    auto in = Eigen::VectorXf::Constant(shape.front(), d);
    //    auto out = nn.feedforward(in);
    //    std::cout << in(0) << " " << out(0) << '\n';
    //}


    //std::cout << "Cost: " << cost << '\n';

    //Test NN on user input
    std::cout << "Enter components of input vector to check NN" << '\n';
    bool userChecking = true;

    while (userChecking)
    {
        Eigen::VectorXf in(shape.front());
        for(size_t i = 0; i < shape.front(); i++)
        {
            double val;
            std::cin >> val;
            if(!std::cin) //cin failed -> no number
            {
                std::cin.clear();
                userChecking = false;
                break;
            }
            in(i) = val;
        }

        auto out = nn.feedforward(in);
        std::cout << "Input: " << in << '\n';
        std::cout << "Output: " << out << '\n';
        std::cout << "Correct: " << function(in) << '\n';
    }

    return 0;
}

#endif //LWIRTH_NEURALNETWORKTEST_HPP
