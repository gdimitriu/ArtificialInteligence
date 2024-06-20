/*
 * testRandom.cpp
 *
 *  Created on: Feb 1, 2024
 *      Author: Gabriel Dimitriu
 */

#include <iostream>
#include "../MLP1Hidden.h"

int main() {
	std::vector<unsigned int> layers(3);
	layers[0] = 2;
	layers[1] = 1;
	layers[2] = 1;
	MLP1Hidden *mlp =new MLP1Hidden(layers);
	mlp->init_random();
	mlp->printData();
	delete mlp;
	layers[1] = 2;
	layers[2] = 2;
	mlp = new MLP1Hidden(layers);
	mlp->init_random();
	mlp->printData();

	mlp = new MLP1Hidden(layers);
	mlp->init_random();
	mlp->printData();
}


