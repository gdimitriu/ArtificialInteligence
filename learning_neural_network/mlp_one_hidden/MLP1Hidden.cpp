/*
 * MLP1Hidden.cpp
 *
 *  Created on: Jun 13, 2024
 *      Author: Gabriel Dimitriu
 */

#include "MLP1Hidden.h"

#include <utility>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>
#include <iostream>

MLP1Hidden::MLP1Hidden(std::vector<unsigned int> &layers) :
		nrOfNeuronsByLayer(layers) {
	if (layers.size() != 3) {
		throw "wrong number of layers";
	}
	w.init(layers[0] + 1, layers[1]);
	v.init(layers[1] + 1, layers[2]);
}

MLP1Hidden::~MLP1Hidden() {
	// TODO Auto-generated destructor stub
}

void MLP1Hidden::init_random() {
	srand(time(NULL));
	v.init_rand(-0.1, 0.1);
	w.init_rand(-0.1, 0.1);
}

void MLP1Hidden::printData() {
	std::cout << "NetWeights" << std::endl;
	std::cout << "w" << std::endl;
	w.printData();
	std::cout << "v" << std::endl;
	v.printData();
	std::cout << "End NetWeights" << std::endl;
}

bool MLP1Hidden::training(std::vector<std::vector<float>> &trainingData,
		std::vector<std::vector<float>> &outputsTp, long nrIterations,
		float lms, float learningRate, float momentum, bool constantRate) {
	init_random();
	bool finished = false;
	float realLearningRate = learningRate;
	for (long iter = 0; iter < nrIterations; ++iter) {
		if (finished) {
			std::cout << "Network was trained after " << iter << " iterations"
					<< std::endl;
			return true;
		}
		if (!constantRate) {
			realLearningRate = 1.0 / (iter + 1);
		}
		float error = 0.0;
		matrixf delta(1, nrOfNeuronsByLayer[2]); //signal error for output layer
		matrixf deltah(1, nrOfNeuronsByLayer[1]); //signal error for hidden layer
		for (int t = 0; t < trainingData.size(); ++t) {
			std::vector<float> x = trainingData[t];
			matrixf h(1, nrOfNeuronsByLayer[1]); //activation of hidden layer
			std::vector<float> o(nrOfNeuronsByLayer[2]); //outputs output layer
			//forward
			for (int j = 0; j < h.d1(); ++j) {
				h(0, j) = 0.0f;
				for (int i = 0; i < x.size(); ++i) {
					h(0, j) += w(i + 1, j) * x[i];
				}
				h(0, j) += w(0, j);
				h(0,j) = 1.0 + exp(-h(0,j));
				//strange ...
//				h(0,j) += exp(-w(0,j));
//				h(0,j) +=1.0;
				h(0, j) = 1.0 / h(0, j);
			}

			for (int k = 0; k < o.size(); ++k) {
				o[k] = 0.0f;
				for (int j = 0; j < h.d1(); ++j) {
					o[k] += v(j + 1, k) * h(0, j);
				}
				o[k] +=v(0,k);
				o[k] = 1 + exp(-o[k]);
				//strange ...
//				o[k] += exp(-v(0, k));
//				o[k] +=1.0;
				o[k] = 1 / o[k];
			}

			//calculate delta
			for (int k = 0; k < delta.d1(); ++k) {
				delta(0, k) = o[k] * (1 - o[k]) * (outputsTp[t][k] - o[k]);
			}
			for (int j = 0; j < deltah.d1(); ++j) {
				deltah(0, j) = 0.0f;
				for (int k = 0; k < delta.d1(); ++k) {
					deltah(0, j) += delta(0, k) * v(j + 1, k);
				}
				deltah(0, j) = h(0, j) * (1 - h(0, j)) * deltah(0, j);
			}
			//update weights
			for (int j = 0; j < h.d0(); ++j) {
				for (int k = 0; k < v.d1(); ++k) {
					v(j + 1, k) += realLearningRate * delta(0,k) * h(0, j);
				}
			}
			for (int k = 0; k < v.d1(); ++k) {
				v(0, k) += realLearningRate * delta(0,k);
			}
			for (int i = 0; i < x.size(); ++i) {
				for (int j = 0; j < w.d1(); ++j) {
					w(i + 1, j) += realLearningRate * deltah(0, j) * x[i];
				}
			}
			for ( int j = 0; j < w.d1(); ++j ) {
				w(0, j) += realLearningRate * deltah(0, j);
			}


			for (int j = 0; j < o.size(); j++) {
				error += (outputsTp[t][j] - o[j])
						* (outputsTp[t][j] - o[j]);
			}
			error /= 2.0;
		}
		error /= trainingData.size();
		if (error < lms) {
			finished = true;
		} else {
			finished = false;
		}
	}
	return false;
}

std::vector<float> MLP1Hidden::execute(std::vector<float> &x) {
	matrixf h(1, nrOfNeuronsByLayer[1]); //activation of hidden layer
	std::vector<float> o(nrOfNeuronsByLayer[2]); //outputs output layer
	//forward
	for (int j = 0; j < h.d1(); ++j) {
		h(0, j) = 0.0f;
		for (int i = 0; i < x.size(); ++i) {
			h(0, j) += w(i + 1, j) * x[i];
		}
		h(0, j) += w(0, j);
		h(0,j) = 1.0 + exp(-h(0,j));
		//strange ...
//		h(0,j) += exp(-w(0,j));
//		h(0,j) +=1.0;
		h(0, j) = 1.0 / h(0, j);
	}
	for (int k = 0; k < o.size(); ++k) {
		o[k] = 0.0f;
		for (int j = 0; j < h.d1(); ++j) {
			o[k] += v(j + 1, k) * h(0, j);
		}
		o[k] += v(0, k);
		o[k] = 1 + exp(-o[k]);
		//strange ...
//		o[k] += exp(-v(0, k));
//		o[k] +=1.0;
		o[k] = 1 / o[k];
	}

	return o;
}
