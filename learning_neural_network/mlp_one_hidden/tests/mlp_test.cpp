/*
 * mlp_test.cpp
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

#include <iostream>
#include <fstream>
#include <cmath>
#include "../MLP1Hidden.h"

void readFile(std::fstream& file, std::vector<std::vector<float>>& data, std::vector<std::vector<float>>& expected)
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
		std::vector<float> outputs;
		outputs.push_back(val);
		expected.push_back(std::move(outputs));
	}
}

int main(int argc, char **argv)
{
	if ( argc != 3 )
	{
		std::cerr<<"Call with "<<argv[0]<<" trainning_file test_file"<<std::endl;
	}
/*
 * 1 hidden layer for xor
 */

	std::vector<unsigned int> layers(3);
	layers[0] = 2;
	layers[1] = 2;
	layers[2] = 1;

	MLP1Hidden *mlp = new MLP1Hidden(layers);
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
	std::vector<std::vector<float>> expectedOutput;
	readFile(trainingFile, trainingData, expectedOutput);
	bool trained = false;
	int i = 0;
	for ( ; i < 100; ++i ) {
		trained = mlp->training(trainingData, expectedOutput, 10000, 0.002, 0.5, 1.0, true);
		if ( trained )
			break;
	}
	if ( !trained )
		std::cerr<<"Network could not be trained!"<<std::endl;
	else
		std::cout<<"Network trained after "<<i<<" tries."<<std::endl;

	mlp->printData();
	std::vector<std::vector<float>> testData;
	std::vector<std::vector<float>> output;
	readFile(testFile, testData, output);
	for ( int i  = 0; i < testData.size(); ++i )
	{
		std::vector<float> rez = mlp->execute(testData[i]);
		if ( fabs(rez[0] - output[i][0]) >= 0.0001 ) {
			std::cout<<"Test "<<i<<" had failed with value "<<rez[0]<<" when expected value was "<<output[i][0]<<std::endl;
		}
	}
	trainingFile.close();
	testFile.close();
	delete mlp;
}
