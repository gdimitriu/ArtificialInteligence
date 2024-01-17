/*
 * Perceptron.h
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
#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_

#include <vector>

class Perceptron {
public:
	Perceptron(float thresholdIn = 0.0f);
	Perceptron(std::vector<float>&& weightsIn, float thresholdIn);
	virtual ~Perceptron();
	float compute(std::vector<float>& values);
	bool training(std::vector<std::vector<float>>& trainingData, std::vector<float>& outputs,long nrIterations, float lms, float learningRate);
protected:
	virtual float outputFunction(float in);
private:
	int nrOfInputs;
	std::vector<float> weights;
	float threshold;
};

#endif /* PERCEPTRON_H_ */
