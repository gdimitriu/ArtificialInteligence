/*
 * suite_translator.cpp
 *
 *  @Copyright 2024 Gabriel Dimitriu
 * All rights reserved.
 *
 * This file is part of Neural Network Simulator project.

 * Neural Network Simulator is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * Neural Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with Neural Network Simulator; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <iostream>
#include <fstream>
#include "matrixf.h"


int main(int argc, char** argv)
{
	if ( argc != 3 )
	{
		std::cerr<<"Call with "<<argv[0]<<" inputFile outputFile"<<std::endl;
		exit(-1);
	}
	int nr_in_suite_trainning;
	matrixf *trainning_inp;
	matrixf *trainning_tp;
	std::ifstream file(argv[1]);
	file >> nr_in_suite_trainning;
	trainning_inp = new matrixf[nr_in_suite_trainning]();
	trainning_tp = new matrixf[nr_in_suite_trainning]();
	for (int i = 0; i < nr_in_suite_trainning; i++)
	{
		trainning_inp[i].load_text(file);
		trainning_tp[i].load_text(file);
	}
	file.close();
	std::ofstream ofile(argv[2]);
	for ( int test = 0; test < nr_in_suite_trainning; ++test )
	{
		matrixf& current = trainning_inp[test];
		for ( int i = 0; i < current.d0(); ++i )
		{
			for ( int j = 0; j < current.d1(); ++j )
			{
				for ( int k = 0; k < current.d2(); ++k )
				{
					ofile<<current(i,j,k)<<" ";
				}
			}
		}
		matrixf& tp = trainning_tp[test];
		for ( int i = 0; i < tp.d0(); ++i )
		{
			for ( int j = 0; j < tp.d1(); ++j )
			{
				for ( int k = 0; k < tp.d2(); ++k )
				{
					ofile<<tp(i,j,k)<<" ";
				}
			}
		}
		ofile<<std::endl;
	}
	ofile.close();
}
