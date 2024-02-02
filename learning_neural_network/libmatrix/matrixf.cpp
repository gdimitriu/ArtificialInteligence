/*
 * matrixf.cpp
 *
 *  Created on: Apr 4, 2022
 *      Author: Gabriel Dimitriu
 */

#include "include/matrixf.h"
#include <unistd.h>
using namespace std;

matrixf::matrixf(int dim0, int dim1) {
	this->dim0 = dim0;
	this->dim1 = dim1;
	error_value = 0.0;
	allocate_matrix();
}

matrixf::matrixf(matrixf &origMatrix) {
	this->dim0 = origMatrix.dim0;
	this->dim1 = origMatrix.dim1;
	allocate_matrix();
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			this->m[i][j] = origMatrix(i, j);
}

void matrixf::init(int dim0, int dim1) {
	if ( m != 0 )
		free_mat();
	this->dim0 = dim0;
	this->dim1 = dim1;
	allocate_matrix();
}

void matrixf::allocate_matrix() {
	m = new float*[dim0]();
	for ( int i = 0; i < dim0; i++ ) {
		m[i] = new float[dim1]();
	}
}

void matrixf::free_mat() {
	if ( m == 0 )
		return;
	for ( int i = 0; i < dim0; i++ ) {
			delete[] m[i];
	}
	delete[] m;
	m = 0;
}

float& matrixf::operator()(int x, int y) {
	if ( x >= dim0 || y >= dim1 ) {
		cout<<"IndexOutOfBounds:"<<x<<":"<<y<<":"<<endl;
		return error_value;
	}
	return m[x][y];
}

matrixf& matrixf::operator=(matrixf &origMatrix) {
	if ( this->dim0 != origMatrix.dim0 || this->dim1 != origMatrix.dim1 ) {
		free_mat();
		this->dim0 = origMatrix.dim0;
		this->dim1 = origMatrix.dim1;
		allocate_matrix();
	}
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
				this->m[i][j] = origMatrix(i, j);
	return *this;
}

matrixf& matrixf::operator=(matrix &origMatrix) {
	free_mat();
	this->dim0 = origMatrix.d0();
	this->dim1 = origMatrix.d1();
	allocate_matrix();
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
				this->m[i][j] = origMatrix(i, j);
	return *this;
}

void matrixf::save(int dFile) {
	write(dFile,&dim0,sizeof(int));
	write(dFile,&dim1,sizeof(int));
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			write(dFile,&m[i][j],sizeof(float));
}

void matrixf::load(int dFile) {
	read(dFile,&dim0,sizeof(int));
	read(dFile,&dim1,sizeof(int));
	allocate_matrix();
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			read(dFile,&m[i][j],sizeof(float));
}

void matrixf::load_text(ifstream &file) {
	file>>dim0>>dim1;
	allocate_matrix();
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			file>>m[i][j];
}

void matrixf::init_rand(float offset, float multiplicator) {
	for ( int i = 0; i < dim0; i++ )
		for ( int j = 0; j < dim1; j++ )
			m[i][j] = offset + multiplicator* rand()/(float)RAND_MAX;
}

void matrixf::printData() {
	for ( int i = 0; i < dim0; i++ ) {
		for ( int j = 0; j < dim1; j++ ) {
			cout<<m[i][j]<<" ";
			cout<<endl;
		}
		cout<<endl;
	}
	cout<<endl;
}
