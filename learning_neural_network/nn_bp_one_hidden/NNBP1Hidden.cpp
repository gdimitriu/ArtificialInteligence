/*
 * NNBP1Hidden.cpp
 *
 *  Created on: Jun 13, 2024
 *      Author: Gabriel Dimitriu
 */

#include "NNBP1Hidden.h"
#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <iostream>

NNBP1Hidden::NNBP1Hidden(std::vector<unsigned int> &layers) :
		nrOfNeuronsByLayer(layers) {
	if (layers.size() != 3) {
		throw "wrong number of layers";
	}
	v.init(layers[0], layers[1]);
	vo.init(layers[1], layers[2]);
}

NNBP1Hidden::~NNBP1Hidden() {
	// TODO Auto-generated destructor stub
}

void NNBP1Hidden::init_random() {
	srand(time(NULL));
	v.init_rand(-0.1, 0.1);
	vo.init_rand(-0.1, 0.1);
}

void NNBP1Hidden::printData() {
	std::cout << "NetWeights" << std::endl;
	std::cout << "v" << std::endl;
	v.printData();
	std::cout << "w" << std::endl;
	vo.printData();
	std::cout << "End NetWeights" << std::endl;
}

bool NNBP1Hidden::training(std::vector<std::vector<float>> &trainingData,
		std::vector<std::vector<float>> &outputsTp, long nrIterations,
		float lms, float learningRate, float momentum, bool constantRate) {
	init_random();
	bool finished = false;
	float realLearningRate = learningRate;
	for (long iter = 0; iter < nrIterations; ++iter) {
		if (finished) {
			std::cout<<"Network was trained after "<<iter<<" iterations"<<std::endl;
			return true;
		}
		if (!constantRate) {
			realLearningRate = 1.0/(iter + 1);
		}
		float error = 0.0;
		matrixf delta(1, nrOfNeuronsByLayer[2]); //signal error for output layer
		matrixf deltah(1, nrOfNeuronsByLayer[1]); //signal error for hidden layer
		for (int k = 0; k < trainingData.size(); ++k) {
			matrixf h(1, nrOfNeuronsByLayer[1]); //activation of hidden layer
			matrixf ih(1, nrOfNeuronsByLayer[1]); //outputs of hidden layer
			matrixf y(1, nrOfNeuronsByLayer[2]); //activation of output layer
			matrixf o(1, nrOfNeuronsByLayer[2]); //outputs output layer
			//forward
			//hidden layer
			std::vector<float> x = trainingData[k];

			for (int q = 0; q < h.d1(); ++q) {
				for (int i = 0; i < x.size(); ++i)
					h(0, q) += x[i] * v(i, q);
			}
			for (int q = 0; q < h.d1(); ++q) {
				ih(0, q) = outputFunction(h(0, q));
			}

			for (int j = 0; j < vo.d1(); ++j) {
				for (int q = 0; q < vo.d0(); ++q)
					y(0, j) += ih(0, q) * vo(q, j); //vo[j][q] orig
			}
			for (int j = 0; j < y.d1(); ++j) {
				o(0, j) = outputFunction(y(0, j));
			}

			//compute errors
			for (int j = 0; j < y.d1(); ++j) {
				delta(0, j) = (outputsTp[k][j] - o(0, j))
						* outputFunctionDerivate(y(0, j));
			}

			for (int q = 0; q < vo.d0(); ++q) {
				for (int j = 0; j < vo.d1(); ++j) {
					deltah(0, q) += delta(0, j) * vo(q, j);
				}
				deltah(0, q) *= outputFunctionDerivate(h(0, q));
			}

			//weight actualization for output layer
			for (int q = 0; q < vo.d0(); ++q) {
				for (int j = 0; j < vo.d1(); ++j) {
					vo(q, j) += realLearningRate * delta(0, j) * ih(0, q);
				}
			}
			//weight actualization for hidden layer
			for (int i = 0; i < v.d0(); ++i) {
				for (int q = 0; q < v.d1(); ++q) {
					v(i, q) += realLearningRate * deltah(0, q) * x[i];
				}
			}


			for (int j = 0; j < o.d1(); j++) {
				error += (outputsTp[k][j] - o(0, j))
						* (outputsTp[k][j] - o(0, j));
			}
			error /= 2.0;
		}
		error /=trainingData.size();
		if (error < lms) {
			finished = true;
		} else {
			finished = false;
		}
	}
	return false;
}

std::vector<float> NNBP1Hidden::execute(std::vector<float> &input) {
	matrixf h(1, nrOfNeuronsByLayer[1]); //activation of hidden layer
	matrixf ih(1, nrOfNeuronsByLayer[1]); //outputs of hidden layer
	matrixf y(1, nrOfNeuronsByLayer[2]); //activation of output layer
	std::vector<float> o(nrOfNeuronsByLayer[2]); //outputs output layer
	//forward
	//hidden layer

	for (int q = 0; q < h.d1(); ++q) {
		for (int i = 0; i < input.size(); ++i)
			h(0, q) += input[i] * v(i, q);
	}
	for (int q = 0; q < h.d1(); ++q) {
		ih(0, q) = outputFunction(h(0, q));
	}

	for (int j = 0; j < vo.d1(); ++j) {
		for (int q = 0; q < vo.d0(); ++q)
			y(0, j) += ih(0, q) * vo(q, j); //vo[j][q] orig
	}

	for (int j = 0; j < y.d1(); ++j) {
		o[j] = outputFunction(y(0, j));
	}
	return o;
}

float NNBP1Hidden::outputFunction(float inputValue) {
	float exponentiala = 1.0 + exp(-inputValue);
	return 1.0 / exponentiala;
}

float NNBP1Hidden::outputFunctionDerivate(float inputValue) {
	return outputFunction(inputValue) * ( 1 - outputFunction(inputValue));
}
