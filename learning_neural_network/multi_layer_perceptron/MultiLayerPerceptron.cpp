/*
 * MultiLayerPerceptron.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: Gabriel Dimitriu
 */

#include "MultiLayerPerceptron.h"
#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <cmath>

MultiLayerPerceptron::MultiLayerPerceptron(std::vector<unsigned int>& layers, float thresholdIn ) : nrOfNeuronsByLayer(layers) {
	netWeigths.reserve(nrOfNeuronsByLayer.size());
	thresholds.reserve(nrOfNeuronsByLayer.size());
	for ( unsigned int i = 0; i <= nrOfNeuronsByLayer.size() - 2; ++i ) {
		matrixf current(layers[i + 1], layers[i]);
		netWeigths.push_back(std::make_unique<matrixf>(current));
	}
	for ( unsigned int i = 1; i < nrOfNeuronsByLayer.size(); ++i ) {
		std::vector<float> current(nrOfNeuronsByLayer[i]);
		thresholds.push_back(std::make_unique<std::vector<float>>(current));
	}
	if ( fabs(thresholdIn) < 0.0001f ) {
		threshold = 0.0f;
	} else {
		threshold = thresholdIn;
	}
}

MultiLayerPerceptron::~MultiLayerPerceptron() {
	netWeigths.clear();
	thresholds.clear();
}

void MultiLayerPerceptron::init_random() {
	srand(time(NULL));
	for ( unsigned int i = 0; i < netWeigths.size(); ++i ) {
		netWeigths[i]->init_rand(-1, 2);
	}
	if ( threshold == 0.0f ) {
		for ( unsigned int i = 0; i < thresholds.size(); ++i ) {
			for ( unsigned int j = 0; j < thresholds[i]->size(); ++j ) {
				thresholds[i]->at(j) = -1+2*rand()/(float)RAND_MAX;
			}
		}
	} else {
		for ( unsigned int i = 0; i < thresholds.size(); ++i ) {
			for ( unsigned int j = 0; j < thresholds[i]->size(); ++j ) {
				thresholds[i]->at(j) = threshold;
			}
		}
	}
	//harcoded to see
	(*netWeigths[0])(0,0) = 1.0;
	(*netWeigths[0])(0,1) = 1.0;
	thresholds[0]->at(0) = 0.0;
	(*netWeigths[0])(1,0) = 1.0;
	(*netWeigths[0])(1,1) = 1.0;
	thresholds[0]->at(1) = -1.0;
	(*netWeigths[1])(0,0) = 1;
	(*netWeigths[1])(0,1) = -2;
	thresholds[1]->at(0) = 0;
}

bool MultiLayerPerceptron::training(std::vector<std::vector<float>>& trainingData, std::vector<float>& outputs,
		long nrIterations, float lms, float learningRate, bool constantRate) {
	init_random();

	return true;
}

void MultiLayerPerceptron::printData() {
	std::cout<<"NetWeights"<<std::endl;
	for ( unsigned int i = 0; i < netWeigths.size(); ++i ) {
		std::cout<<"netWeights["<<i<<"]"<<std::endl;
		netWeigths[i]->printData();
	}
	for ( unsigned int i = 0; i < thresholds.size(); ++i ) {
		for ( unsigned int j = 0; j < thresholds[i]->size(); ++j ) {
			std::cout<<thresholds[i]->at(j)<<" ";
		}
		std::cout<<std::endl;
	}
}

std::vector<float> MultiLayerPerceptron::execute(std::vector<float>& input) {

	std::vector<float> inputs;
	std::vector<float> outputs(input);
	for ( unsigned int layer = 0; layer < netWeigths.size(); ++layer) {
		inputs = outputs;
		outputs.clear();
		outputs.reserve(netWeigths[layer]->d0());
		for ( int neuron = 0; neuron < netWeigths[layer]->d0(); ++neuron ) {
			float result = 0.0f;
			for ( int i = 0; i < netWeigths[layer]->d1(); ++i ) {
				result += inputs[i] * (*netWeigths[layer])(neuron, i);
			}
			result += thresholds[layer]->at(neuron);
			outputs.push_back(outputFunction(result));
		}
	}
	return outputs;
}

float MultiLayerPerceptron::maximum(std::vector<float>& inputs) {
	float result = inputs[0];
	for ( int i = 1; i < inputs.size(); ++i ) {
		if ( result < inputs[i] )
			result = inputs[i];
	}
	return result;
}

float MultiLayerPerceptron::outputFunction(float inputValue) {
	if ( inputValue > 0 )
		return 1;
	else
		return 0;
}
