/*
 * testRandom.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: Gabriel Dimitriu
 */

#include "../MultiLayerPerceptron.h"
#include <iostream>

int main() {
	std::vector<unsigned int> layers(2);
	layers[0] = 2;
	layers[1] = 1;
	MultiLayerPerceptron *mlp =new MultiLayerPerceptron(layers);
	mlp->init_random();
	mlp->printData();
	delete mlp;
	layers[1] = 2;
	layers.push_back(1);
	mlp = new MultiLayerPerceptron(layers);
	mlp->init_random();
	mlp->printData();

	mlp = new MultiLayerPerceptron(layers, 0.5);
	mlp->init_random();
	mlp->printData();
}


