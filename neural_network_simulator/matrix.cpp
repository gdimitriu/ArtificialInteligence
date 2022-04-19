/*
 * matrix.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 */

#include "matrix.h"
#include <unistd.h>

matrix::matrix(int dim0, int dim1, int dim2) {
	this->dim0 = dim0;
	this->dim1 = dim1;
	this->dim2 = dim2;
	allocate_matrix();
}

matrix::matrix(matrix &origMatrix) {
	this->dim0 = origMatrix.dim0;
	this->dim1 = origMatrix.dim1;
	this->dim2 = origMatrix.dim2;
	allocate_matrix();
	for (int i = 0; i < dim0; i++)
		for (int j = 0; j < dim1; j++)
			for (int k = 0; k < dim2; k++)
				this->m[i][j][k] = origMatrix(i, j, k);
}

void matrix::allocate_matrix() {
	m = new int**[dim0]();
	for (int i = 0; i < dim0; i++) {
		m[i] = new int*[dim1]();
		for (int j = 0; j < dim1; j++)
			m[i][j] = new int[dim2]();
	}
}

void matrix::free_mat() {
	if (m == 0)
		return;
	for (int i = 0; i < dim0; i++) {
		for (int j = 0; j < dim1; j++)
			delete[] m[i][j];
		delete[] m[i];
	}
	delete[] m;
	m = 0;
}

int &matrix::operator()(int x,int y,int z) {
	return m[x][y][z];
}

void matrix::operator=(matrix& copy) {
	free_mat();
	this->dim0 = copy.dim0;
	this->dim1 = copy.dim1;
	this->dim2 = copy.dim2;
	allocate_matrix();
	for(int i = 0; i < dim0; i++)
		for(int j = 0; j < dim1; j++)
			for(int k = 0; k < dim2 ; k++)
				this->m[i][j][k] = copy(i,j,k);
}


void matrix::save(int dFile) {
	write(dFile,&dim0,sizeof(int));
	write(dFile,&dim1,sizeof(int));
	write(dFile,&dim2,sizeof(int));
	for (int i = 0; i < dim0; i++)
		for (int j = 0; j < dim1; j++)
			for (int k = 0; k < dim2; k++)
				write(dFile,&m[i][j][k],sizeof(int));
}

void matrix::load(int dFile) {
	read(dFile,&dim0,sizeof(int));
	read(dFile,&dim1,sizeof(int));
	read(dFile,&dim2,sizeof(int));
	allocate_matrix();
	for (int i = 0; i < dim0; i++)
		for (int j = 0; j < dim1; j++)
			for (int k = 0; k < dim2; k++)
				read(dFile,&m[i][j][k],sizeof(int));
}
