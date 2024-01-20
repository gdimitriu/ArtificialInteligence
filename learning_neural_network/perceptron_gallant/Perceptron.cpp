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
#include <memory>

Perceptron::Perceptron() : nrOfInputs(0), threshold(0)
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
	if ( in >= threshold )
		return 1;
	else
		return -1;
}

float Perceptron::compute(std::vector<float>& inputs)
{
	float result = 0.0f;
	for ( int i = 0; i < weights.size(); ++i )
		result += inputs[i] * weights[i];
	return outputFunction(result);
}

void Perceptron::init(int size)
{
	weights.clear();
	threshold = -1+2*rand()/(float)RAND_MAX;
	for ( int i =  0; i < size; ++i )
	{
		weights.push_back(-1+2*rand()/(float)RAND_MAX);
//		std::cout<<"w["<<i<<"]="<<weights[i]<<"\t";
	}
//	std::cout<<" threshold = "<<threshold<<std::endl;
//	std::cout.flush();
}

bool Perceptron::training(std::vector<std::vector<float>>& trainingData, std::vector<float>& outputs, long nrIterations,float learningRate)
{
	//number of the forms that was correct trained and classification in seprationAproximation vector
	int nrB = 0;
	std::vector<float> separationAproximation;

	srand(time(NULL));

	init(trainingData[0].size());
	std::copy(weights.begin(), weights.end(), std::back_inserter(separationAproximation));

	float realThreshold = -threshold;
	long i, n;
	int nrOfReset = 10;
	reinitilization:
	std::cout<<"Start training"<<std::endl;
	std::cout.flush();
	i = 0;
	n = 0;
	do {
		for ( int j = 0; j < trainingData.size(); ++j )
		{
			float val = 0.0f;

			for ( int l = 0; l < weights.size(); ++l )
			{
				if ( outputs[j] < 0 )
					val -= trainingData[j][l] * weights[l];
				else
					val += trainingData[j][l] * weights[l];
			}
			val += realThreshold * outputs[j];

			if ( val <= 0 )
			{
				for ( int k = 0 ; k < trainingData[j].size(); ++k )
				{
					if ( outputs[j] < 0 )
						weights[k] -= learningRate*trainingData[j][k];
					else
						weights[k] += learningRate*trainingData[j][k];
				}
				realThreshold += learningRate*outputs[j];
				if ( n == 0 )
				{
					++i;
					continue;
				}
				if ( n > nrB )
				{
					nrB = n;
					++i;
					separationAproximation.clear();
					std::copy(weights.begin(), weights.end(), std::back_inserter(separationAproximation));
				}
			}
			else
			{
				++n;
				++i;
				continue;
			}
		}
		if ( nrB == trainingData.size() )
			break;
	} while( i < nrIterations );

	if ( i >= nrIterations && nrOfReset != 0)
	{
		init(trainingData[0].size());
		realThreshold = -threshold;
		nrB = 0;
		separationAproximation.clear();
		std::copy(weights.begin(), weights.end(), std::back_inserter(separationAproximation));
		--nrOfReset;
		goto reinitilization;
	}
	threshold = -realThreshold;
	float currentLMS = 0.0;
	for ( int j = 0; j < trainingData.size(); ++j )
	{
		float val = compute(trainingData[j]);
		currentLMS += pow(val - outputs[j],2);
	}
	std::cout<<"Error after training is "<<currentLMS/trainingData.size()<<" iterations = "<<i<<std::endl;
	for ( int i =  0; i < weights.size(); ++i )
	{
		std::cout<<"w["<<i<<"]="<<weights[i]<<"\t";
	}
	std::cout<<" Threshold = "<<threshold<<std::endl;
	std::cout.flush();
	if ( i >= nrIterations && nrOfReset == 0 )
		return false;
	return true;
}
