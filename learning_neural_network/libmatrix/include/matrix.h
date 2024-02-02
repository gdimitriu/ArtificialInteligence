/*
 * matrix.h
 *
 *  Created on: Apr 5, 2022
 *      Author: Gabriel Dimitriu
 */

#ifndef MATRIX_H_
#define MATRIX_H_

class matrix {
	int **m;
	//the two dimensions
	int dim0;
	int dim1;
public:
	matrix() {
		m = 0;
		dim0 = 0;
		dim1 = 0;
	}
	matrix(matrix&);
	matrix(int,int);
	virtual ~matrix() {
		free_mat();
	}
	int d0() {
		return dim0;
	}
	int d1() {
		return dim1;
	}
	void free_mat();
	void operator=(matrix&);
	int& operator()(int, int);
	void save(int dFile);
	void load(int dFile);
private:
	void allocate_matrix();
};

#endif /* MATRIX_H_ */
