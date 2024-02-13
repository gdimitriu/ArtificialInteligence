/*
 * matrixf.h
 *
 *  Created on: Apr 4, 2022
 *      Author: Gabriel Dimitriu
 */

#ifndef MATRIXF_H_
#define MATRIXF_H_
#include "matrix.h"
#include <fstream>
#include <iostream>

class matrixf {
	float **m;
	//valorile celor 2 dimensiuni
	int dim0;
	int dim1;
public:
	matrixf() {
		m = 0;
		dim0 = 0;
		dim1 = 0;
		error_value = 0.0;
	};
	matrixf(const matrixf&);
	matrixf(int, int);
	void init(int,int);
	void printData();
	virtual ~matrixf() {
		free_mat();
	};
	int d0() {
		return dim0;
	}
	int d1() {
		return dim1;
	}
	void free_mat();
	matrixf& operator=(matrixf&);
	matrixf& operator=(matrix&);
	float& operator()(int,int);
	void save(int dFile);
	void load(int dFile);
	void load_text(std::ifstream &file);
	void init_rand(float offset, float multiplicator);
private:
	float error_value;
	void allocate_matrix();
};

#endif /* MATRIXF_H_ */
