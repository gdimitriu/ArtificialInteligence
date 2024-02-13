/*
 * MultiLayerPerceptron.h
 *
 *  Created on: Feb 1, 2024
 *      Author: Gabriel Dimitriu
 */

#ifndef MULTILAYERPERCEPTRON_H_
#define MULTILAYERPERCEPTRON_H_

#include <matrixf.h>
#include <vector>
#include <memory>

class MultiLayerPerceptron {
public:
	MultiLayerPerceptron(std::vector<unsigned int>& layers, float bias = 0.0, float threshold = 0.0);
	virtual ~MultiLayerPerceptron();
	void printData();
	void init_random();
	bool training(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<float>>& outputsTp,
			long nrIterations, float lms, float learningRate, float momentum, bool constantRate);
	std::vector<float> execute(std::vector<float>& input);
private:
	float maximum(std::vector<float>& inputs);
	float outputFunction(float inputValue);
	float outputFunctionDerivate(std::vector<std::vector<float>>& outputs, int layer, int neuron);
	std::vector<unsigned int> nrOfNeuronsByLayer;
	std::vector<matrixf> netWeigths;
	std::vector<std::vector<float>> biases;
	float bias;
	float threshold;
	int activationType;
};

#endif /* MULTILAYERPERCEPTRON_H_ */
