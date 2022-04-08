/*
 * matrixf.h
 *
 *  Created on: Apr 4, 2022
 *      Author: Gabriel Dimitriu
 */

#ifndef MATRIXF_H_
#define MATRIXF_H_
#include "matrix.h"

class matrixf {
	float ***m;
	//valorile celor 3 dimensiuni
	int dim0;
	int dim1;
	int dim2;
public:
	matrixf() {
		m = 0;
		dim0 = 0;
		dim1 = 0;
		dim2 = 0;
		error_value = 0.0;
	};
	matrixf(matrixf&);
	matrixf(int, int, int);
	void init(int,int,int);
	virtual ~matrixf() {
		free_mat();
	};
	int d0() {
		return dim0;
	}
	int d1() {
		return dim1;
	}
	int d2() {
		return dim2;
	}
	void free_mat();
	matrixf& operator=(matrixf&);
	matrixf& operator=(matrix&);
	float& operator()(int,int,int);
private:
	float error_value;
	void allocate_matrix();
};

#endif /* MATRIXF_H_ */
