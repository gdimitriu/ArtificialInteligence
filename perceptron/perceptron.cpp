// perceptron.cpp : Defines the entry point for the console application.
//
/*
	Copyright 2005 Gabriel Dimitriu

    This file is part of perceptron.

    perceptron is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    perceptron is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with perceptron; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/

#include "stdafx.h"
#include "Neuron.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	double *x;
	double *y1;
	long i,nr;
	CNeuron neuron(2,10);
	nr=4;
	x=(double *)calloc(nr*2,sizeof(double));
	x[0]=4.0;//1.0
	x[1]=2.0;//-1.0
	x[2]=4.0;//1.0
	x[3]=4.0;//1.0
	x[4]=2.0;//-1.0
	x[5]=4.0;//1.0
	x[6]=2.0;//-1.0
	x[7]=2.0;//-1.0
	y1=(double *)calloc(nr,sizeof(double));
	y1[0]=1.0;
	y1[1]=1.0;
	y1[2]=-1.0;
	y1[3]=-1.0;
	cout<<"After "<<neuron.Training(x,y1,nr)<<" iteration we had finished the trainning\n"; ;
	free(x);
	free(y1);
	x=(double *)calloc(2,sizeof(double));
	cout<<"How many inputs do you have ?\n"; cin>>nr;
	cout<<"Input first and second\n";
	for(i=0;i<nr;i++)
	{
		cin>>x[0];
		cin>>x[1];
		cout<<"Solution="<<neuron.Compute(x)<<endl;
	}
	free(x);
	return 0;
}
