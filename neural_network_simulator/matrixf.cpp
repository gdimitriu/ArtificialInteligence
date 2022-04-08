/*
 * matrixf.cpp
 *
 *  Created on: Apr 4, 2022
 *      Author: Gabriel Dimitriu
 */

#include "matrixf.h"

matrixf::matrixf(int dim0, int dim1, int dim2) {
	this->dim0 = dim0;
	this->dim1 = dim1;
	this->dim2 = dim2;
	error_value = 0.0;
	allocate_matrix();
}

matrixf::matrixf(matrixf &origMatrix) {
	this->dim0 = origMatrix.dim0;
	this->dim1 = origMatrix.dim1;
	this->dim2 = origMatrix.dim2;
	allocate_matrix();
	for (int i = 0; i < dim0; i++)
		for (int j = 0; j < dim1; j++)
			for (int k = 0; k < dim2; k++)
				this->m[i][j][k] = origMatrix(i, j, k);
}

void matrixf::init(int dim0, int dim1, int dim2) {
	this->dim0 = dim0;
	this->dim1 = dim1;
	this->dim2 = dim2;
	allocate_matrix();
}

void matrixf::allocate_matrix() {
	m = new float**[dim0]();
	for (int i = 0; i < dim0; i++) {
		m[i] = new float*[dim1]();
		for (int j = 0; j < dim1; j++)
			m[i][j] = new float[dim2]();
	}
}

void matrixf::free_mat() {
	if (m == 0)
		return;
	for (int i = 0; i < dim0; i++) {
		for (int j = 0; j < dim1; j++)
			delete m[i][j];
		delete m[i];
	}
	delete m;
	m = 0;
}

float& matrixf::operator()(int x, int y, int z) {
	if (x >= dim0 || y >= dim1 || z >= dim2)
		return error_value;
	return m[x][y][z];
}

matrixf& matrixf::operator=(matrixf &origMatrix) {
	if (this->dim0 != origMatrix.dim0 || this->dim1 != origMatrix.dim1
			|| this->dim2 != origMatrix.dim2) {
		free_mat();
		this->dim0 = origMatrix.dim0;
		this->dim1 = origMatrix.dim1;
		this->dim2 = origMatrix.dim2;
		allocate_matrix();
	}
	for (int i = 0; i < dim0; i++)
		for (int j = 0; j < dim1; j++)
			for (int k = 0; k < dim2; k++)
				this->m[i][j][k] = origMatrix(i, j, k);
	return *this;
}

matrixf& matrixf::operator=(matrix &origMatrix) {
	free_mat();
	this->dim0 = origMatrix.d0();
	this->dim1 = origMatrix.d1();
	this->dim2 = origMatrix.d2();
	allocate_matrix();
	for (int i = 0; i < dim0; i++)
		for (int j = 0; j < dim1; j++)
			for (int k = 0; k < dim2; k++)
				this->m[i][j][k] = origMatrix(i, j, k);
	return *this;
}

