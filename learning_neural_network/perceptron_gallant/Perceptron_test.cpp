/*
 * Perceptron_test.cpp
 *
 *  @Copyright 2024 Gabriel Dimitriu
 * All rights reserved.
 *
 * This file is part of ArtificialInteligence project.

 * ArtificialInteligence is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * ArtificialInteligence is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with ArtificialInteligence; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "Perceptron.h"
#include <iostream>
#include <fstream>
#include <cmath>

void readFile(std::fstream& file, std::vector<std::vector<float>>& data, std::vector<float>& expected)
{
	int nrOfVectors;
	file>>nrOfVectors;
	int nrInputs;
	file>>nrInputs;
	for ( int i = 0; i < nrOfVectors; ++i )
	{
		std::vector<float> d1;
		float val;
		for ( int j = 0; j < nrInputs; ++j)
		{
			file>>val;
			d1.push_back(val);
		}
		data.push_back(std::move(d1));
		file>>val;
		expected.push_back(val);
	}
}

int main(int argc, char **argv)
{
	if ( argc != 3 )
	{
		std::cerr<<"Call with "<<argv[0]<<" trainning_file test_file"<<std::endl;
	}
	Perceptron *neuron = new Perceptron();
	std::fstream trainingFile(argv[1]);
	std::fstream testFile(argv[2]);
	if ( trainingFile.fail() )
	{
		std::cerr<<"Failed to open training file "<<argv[1]<<std::endl;
		exit(-1);
	}
	if ( testFile.fail() )
	{
		std::cerr<<"Failed to open test file "<<argv[2]<<std::endl;
		exit(-1);
	}
	std::vector<std::vector<float>> trainingData;
	std::vector<float> expectedOutput;
	readFile(trainingFile, trainingData, expectedOutput);
	bool trained = neuron->training(trainingData, expectedOutput, 10000, 1);
	if ( !trained )
		std::cerr<<"Network could not be trained!"<<std::endl;
	std::vector<std::vector<float>> testData;
	std::vector<float> output;
	readFile(testFile, testData, output);
	for ( int i  = 0; i < testData.size(); ++i )
	{
		float rez = neuron->compute(testData[i]);
		if ( fabs(rez - output[i]) >= 0.0001 ) {
			std::cout<<"Test "<<i<<" had failed with value "<<rez<<" when expected value was "<<output[i]<<std::endl;
		}
	}
	trainingFile.close();
	testFile.close();
	delete neuron;
}
