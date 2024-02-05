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
	MultiLayerPerceptron(std::vector<unsigned int>& layers, float threshold = 0.0);
	virtual ~MultiLayerPerceptron();
	void printData();
	void init_random();
	bool training(std::vector<std::vector<float>>& trainingData, std::vector<float>& outputs,
			long nrIterations, float lms, float learningRate, bool constantRate);
	std::vector<float> execute(std::vector<float>& input);
private:
	float maximum(std::vector<float>& inputs);
	float outputFunction(float inputValue);
	std::vector<unsigned int> nrOfNeuronsByLayer;
	std::vector<std::unique_ptr<matrixf>> netWeigths;
	std::vector<std::unique_ptr<std::vector<float>>> thresholds;
	float threshold;
};

#endif /* MULTILAYERPERCEPTRON_H_ */
