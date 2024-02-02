/*
 * matrix.cpp
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 */

#include "include/matrix.h"
#include <unistd.h>

matrix::matrix(int dim0, int dim1) {
	this->dim0 = dim0;
	this->dim1 = dim1;
	allocate_matrix();
}

matrix::matrix(matrix &origMatrix) {
	this->dim0 = origMatrix.dim0;
	this->dim1 = origMatrix.dim1;
	allocate_matrix();
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			this->m[i][j] = origMatrix(i, j);
}

void matrix::allocate_matrix() {
	m = new int*[dim0]();
	for ( int i = 0; i < dim0; i++ ) {
		m[i] = new int[dim1]();
	}
}

void matrix::free_mat() {
	if ( m == 0 )
		return;
	for ( int i = 0; i < dim0; i++ ) {
		delete[] m[i];
	}
	delete[] m;
	m = 0;
}

int &matrix::operator()(int x,int y) {
	return m[x][y];
}

void matrix::operator=(matrix& copy) {
	free_mat();
	this->dim0 = copy.dim0;
	this->dim1 = copy.dim1;
	allocate_matrix();
	for( int i = 0; i < dim0; i++ )
		for( int j = 0; j < dim1; j++ )
			this->m[i][j] = copy(i,j);
}


void matrix::save(int dFile) {
	write(dFile,&dim0,sizeof(int));
	write(dFile,&dim1,sizeof(int));
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			write(dFile,&m[i][j],sizeof(int));
}

void matrix::load(int dFile) {
	read(dFile,&dim0,sizeof(int));
	read(dFile,&dim1,sizeof(int));
	allocate_matrix();
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			read(dFile,&m[i][j],sizeof(int));
}
