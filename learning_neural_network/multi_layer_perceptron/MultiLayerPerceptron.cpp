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

MultiLayerPerceptron::MultiLayerPerceptron(std::vector<unsigned int>& layers, float biasIn, float thresholdIn ) : nrOfNeuronsByLayer(layers) {
	netWeigths.reserve(nrOfNeuronsByLayer.size());
	biases.reserve(nrOfNeuronsByLayer.size());
	for ( unsigned int i = 1; i < nrOfNeuronsByLayer.size(); ++i ) {
		matrixf current(nrOfNeuronsByLayer[i - 1], nrOfNeuronsByLayer[i]);
		netWeigths.push_back(current);
	}
	for ( unsigned int i = 1; i < nrOfNeuronsByLayer.size(); ++i ) {
		std::vector<float> current(nrOfNeuronsByLayer[i]);
		biases.push_back(current);
	}
	if ( fabs(biasIn) < 0.0001f ) {
		bias = 0.0f;
	} else {
		bias = biasIn;
	}
	activationType = 0;
	if ( fabs(thresholdIn) < 0.0001f ) {
		threshold = 1.0f;
	} else {
		threshold = thresholdIn;
		activationType = 1;
	}
}

MultiLayerPerceptron::~MultiLayerPerceptron() {
	netWeigths.clear();
	biases.clear();
}

void MultiLayerPerceptron::init_random() {
	srand(time(NULL));
	for ( unsigned int i = 0; i < netWeigths.size(); ++i ) {
		netWeigths[i].init_rand(-0.1, 0.1);
	}
	if ( threshold == 0.0f ) {
		for ( unsigned int i = 0; i < biases.size(); ++i ) {
			for ( unsigned int j = 0; j < biases[i].size(); ++j ) {
				biases[i][j] = -0.1+0.2*rand()/(float)RAND_MAX;
			}
		}
	} else {
		for ( unsigned int i = 0; i < biases.size(); ++i ) {
			for ( unsigned int j = 0; j < biases[i].size(); ++j ) {
				biases[i][j] = threshold;
			}
		}
	}
}

bool MultiLayerPerceptron::training(std::vector<std::vector<float>>& trainingData, std::vector<std::vector<float>>& outputsTp,
		long nrIterations, float lms, float learningRate, float momentum, bool constantRate) {
	//inialize the weights and threshold with random numbers
	init_random();

	//allocate delta vectors
	std::vector<matrixf> dnetWeigths;
	std::vector<std::vector<float>> delta;
	std::vector<std::vector<float>> dbiases;
	//delta for weights
	for ( unsigned int i = 1; i <= nrOfNeuronsByLayer.size(); ++i ) {
		matrixf current(nrOfNeuronsByLayer[i - 1], nrOfNeuronsByLayer[i]);
		dnetWeigths.push_back(current);
	}
	//delta for neurons
	for ( unsigned int i = 1; i < nrOfNeuronsByLayer.size(); ++i ) {
		std::vector<float> current(nrOfNeuronsByLayer[i]);
		delta.push_back(current);
	}
	//delta for biases/bias
	for ( unsigned int i = 1; i < nrOfNeuronsByLayer.size(); ++i ) {
		std::vector<float> current(nrOfNeuronsByLayer[i]);
		dbiases.push_back(current);
	}

	float realLearningRate = learningRate;
	for ( long i = 0; i < nrIterations; ++i ) {
		if ( !constantRate ) {
			realLearningRate = 1.0/(i + 1);
		}
		for ( int j = 0; j < trainingData.size(); ++j ) {
			//forward
			std::vector<std::vector<float>> outputs;
			outputs.push_back(trainingData[j]);
			for ( int layer = 0; layer < netWeigths.size(); ++layer ) {
				std::vector<float> output(netWeigths[layer].d0());
				for ( int neuron = 0; neuron < netWeigths[layer].d0(); ++neuron ) {
					float result = 0.0f;
					for ( int weightIndex = 0; weightIndex < netWeigths[layer].d1(); ++weightIndex ) {
						result += outputs[layer][weightIndex] * netWeigths[layer](neuron, weightIndex);
					}
					result += biases[layer][neuron];
					output[neuron] = outputFunction(result);
				}
				outputs.push_back(output);
			}

			//backpropagation
			std::vector<matrixf> oldnetWeigths(netWeigths);
			//out -> previous layer (hidden or inp)
			int  layer = netWeigths.size() - 1;
			for ( int neuron = 0; neuron < netWeigths[layer].d0(); ++neuron ) {
				delta[layer][neuron] = outputsTp[j][neuron] - outputs[layer + 1][neuron];
				delta[layer][neuron] *= outputFunctionDerivate(outputs, layer + 1, neuron);
				for ( int prevLayerNeuron = 0; prevLayerNeuron < netWeigths[layer].d1(); ++prevLayerNeuron ) {
					float deltaw = realLearningRate * delta[layer][neuron] * outputs[layer][prevLayerNeuron];
					netWeigths[layer](neuron, prevLayerNeuron) += deltaw + momentum * dnetWeigths[layer](neuron, prevLayerNeuron);
					dnetWeigths[layer](neuron, prevLayerNeuron) = deltaw;
				}
				float deltab = realLearningRate * delta[layer][neuron];
				biases[layer][neuron] += deltab + momentum * dbiases[layer][neuron];
				dbiases[layer][neuron] = deltab;
			}

			//rest of the layers
			for ( int layer = netWeigths.size() - 2; layer >= 0; --layer) {
				for ( int neuron = 0; neuron < netWeigths[layer].d0(); ++neuron ) {
					delta[layer][neuron] = 0.0f;
					for ( int prevNeuron = 0; prevNeuron < netWeigths[layer + 1].d0(); ++prevNeuron ) {
						delta[layer][neuron] += delta[layer + 1][prevNeuron] * oldnetWeigths[layer](neuron, prevNeuron);
					}
					delta[layer][neuron] *= outputFunctionDerivate(outputs, layer + 1, neuron);
					for ( int prevNeuron = 0; prevNeuron < netWeigths[layer].d1(); ++prevNeuron ) {
						float deltaw = realLearningRate * delta[layer][neuron];
						biases[layer][neuron] += realLearningRate + momentum * dbiases[layer][neuron];
						dbiases[layer][neuron] = deltaw;
					}
					float deltab = realLearningRate * delta[layer][neuron];
					biases[layer][neuron] += deltab + momentum * dbiases[layer][neuron];
					dbiases[layer][neuron] = deltab;
				}
			}

		}
		//compute LMS
		float currentLMS = 0.0f;
		for ( int j = 0; j < trainingData.size(); ++j ) {
			std::vector<float> output = execute(trainingData[j]);
			for ( int l = 0; l < outputsTp[j].size(); ++l ) {
				currentLMS += pow(output[l] - outputsTp[j][l],2);
			}
		}
		if ( (currentLMS/trainingData.size() - lms) <= 0.0001 ) {
			std::cout<<"Nr of iterations = "<<i<<" current lms = "<<currentLMS/trainingData.size()<<" desired lms="<<lms<<std::endl;
			return true;
		}
		if ( !constantRate ) {
			realLearningRate = learningRate/(i + 1);
		}

	}

	return false;
}

void MultiLayerPerceptron::printData() {
	std::cout<<"NetWeights"<<std::endl;
	for ( unsigned int i = 0; i < netWeigths.size(); ++i ) {
		std::cout<<"netWeights["<<i<<"]"<<std::endl;
		netWeigths[i].printData();
	}
	for ( unsigned int i = 0; i < biases.size(); ++i ) {
		for ( unsigned int j = 0; j < biases[i].size(); ++j ) {
			std::cout<<biases[i][j]<<" ";
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
		outputs.reserve(netWeigths[layer].d0());
		for ( int neuron = 0; neuron < netWeigths[layer].d0(); ++neuron ) {
			float result = 0.0f;
			for ( int i = 0; i < netWeigths[layer].d1(); ++i ) {
				result += inputs[i] * netWeigths[layer](neuron, i);
			}
			result += biases[layer][neuron];
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
	if ( activationType == 0 ) {
		if ( inputValue > 0 )
			return 1;
		else
			return 0;
	} else if ( activationType == 1) {
		float exponentiala = 1.0 + exp(-threshold * inputValue);
		return 1.0 / exponentiala;
	}
	return 0;
}

float MultiLayerPerceptron::outputFunctionDerivate(std::vector<std::vector<float>>& outputs, int layer, int neuron) {
	if ( activationType == 0 ) {
		return 1.0;
	} else if ( activationType == 1) {
		//return (outputs[layer][neuron] * (1.0 - outputs[layer][neuron]))/threshold;
		return outputFunction(outputs[layer][neuron]) * ( 1 - outputFunction(outputs[layer][neuron]))/threshold;
	}
	return 1.0;
}
