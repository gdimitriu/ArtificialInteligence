/*
 * Perceptron.cpp
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
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <cmath>

Perceptron::Perceptron(float thresholdIn) : nrOfInputs(0), threshold(thresholdIn)
{

}

Perceptron::Perceptron(std::vector<float>&& weightsIn, float thresholdIn) : weights(std::move(weightsIn)), threshold(thresholdIn)
{
	nrOfInputs = weights.size();
}

Perceptron::~Perceptron()
{

}

float Perceptron::outputFunction(float in)
{
	if ( in > threshold )
		return 1;
	else if ( in < threshold )
		return 0;
	return 1;
}
float Perceptron::compute(std::vector<float>& inputs)
{
	float result = 0.0f;
	for ( int i = 0; i < weights.size(); ++i )
		result += inputs[i] * weights[i];
	return outputFunction(result);
}

bool Perceptron::training(std::vector<std::vector<float>>& trainingData, std::vector<float>& outputs,
		long nrIterations, float lms, float learningRate, bool constantRate)
{
	float realLearningRate = learningRate;
	float currentLMS;
	srand(time(NULL));
	for ( int i =  0; i < trainingData[0].size(); ++i )
	{
		weights.push_back(rand()/(float)RAND_MAX);
		std::cout<<"w["<<i<<"]="<<weights[i]<<"\t";
	}
	std::cout<<std::endl;
	std::cout.flush();
	for ( long i = 0; i < nrIterations; ++i )
	{
		currentLMS = 0.0;
		for ( int j = 0; j < trainingData.size(); ++j )
		{
			float val = compute(trainingData[j]);
			float delta = outputs[j] - val;
			if ( fabs(delta) >= 0.0001  )
			{
				for ( int k = 0; k < weights.size(); ++k )
				{
					weights[k] = weights[k] + realLearningRate*(trainingData[j][k])*delta;
				}
			}
		}
		for ( int j = 0; j < trainingData.size(); ++j )
		{
			float val = compute(trainingData[j]);
			currentLMS += pow(val - outputs[j],2);
		}
		if ( (currentLMS/trainingData.size() - lms) <= 0.0001 )
		{
			std::cout<<"Nr of iterations = "<<i<<" current lms = "<<currentLMS/trainingData.size()<<" threshold = "<<threshold<<std::endl;
			return true;
		}
		if ( !constantRate )
		{
			realLearningRate = learningRate/(i + 1);
		}
	}
	std::cout<<"Error after training is "<<currentLMS/trainingData.size()<<std::endl;
	for ( int i =  0; i < weights.size(); ++i )
	{
		std::cout<<"w["<<i<<"]="<<weights[i]<<"\t";
	}
	std::cout<<" threshold = "<<threshold<<std::endl;
	std::cout.flush();
	return false;
}
