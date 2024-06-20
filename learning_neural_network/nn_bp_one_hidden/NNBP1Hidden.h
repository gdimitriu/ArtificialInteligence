/*
 * NNBP1Hidden.h
 *
 *  Created on: Jun 13, 2024
 *      Author: Gabriel Dimitriu
 */

#ifndef NNBP1HIDDEN_H_
#define NNBP1HIDDEN_H_

#include <matrixf.h>
#include <vector>
#include <memory>

class NNBP1Hidden {
public:
	NNBP1Hidden(std::vector<unsigned int>& layers);
	virtual ~NNBP1Hidden();
	void init_random();
	void printData();
	bool training(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<float>>& outputsTp,
			long nrIterations, float lms, float learningRate, float momentum, bool constantRate);
	std::vector<float> execute(std::vector<float>& input);
private:
	float outputFunction(float inputValue);
	float outputFunctionDerivate(float inputValue);
	std::vector<unsigned int> nrOfNeuronsByLayer;
	matrixf v; //weights for hidden layer
	matrixf vo; //weights for output layer
};

#endif /* NNBP1HIDDEN_H_ */
