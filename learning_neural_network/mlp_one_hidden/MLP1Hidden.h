/*
 * MLP1Hidden.h
 *
 *  Created on: Jun 13, 2024
 *      Author: Gabriel Dimitriu
 */

#ifndef MLP1HIDDEN_H_
#define MLP1HIDDEN_H_

#include <matrixf.h>
#include <vector>
#include <memory>

class MLP1Hidden {
public:
	MLP1Hidden(std::vector<unsigned int>& layers);
	virtual ~MLP1Hidden();
	void init_random();
	void printData();
	bool training(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<float>>& outputsTp,
			long nrIterations, float lms, float learningRate, float momentum, bool constantRate);
	std::vector<float> execute(std::vector<float>& input);
private:
	std::vector<unsigned int> nrOfNeuronsByLayer;
	matrixf w; //weights for hidden layer
	matrixf v; //weights for output layer
};

#endif /* MLP1HIDDEN_H_ */
