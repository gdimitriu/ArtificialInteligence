// rbf.h: interface for the Crbf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBF_H__C13AF841_3CB7_11DA_B910_CF5D952B9F0C__INCLUDED_)
#define AFX_RBF_H__C13AF841_3CB7_11DA_B910_CF5D952B9F0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include"TMemory.h"
#include <fstream>
using namespace std;
class Crbf  
{
public:
	void Load();
	void Save();
	void SetFileName(char *name);
	void Delete();
	unsigned int Run(unsigned long *x);
	bool Training(long steps);
	void InitT(double alpha,double eta, double error);
	void SetMemory(CTMemory *mem);
	void Create(unsigned int n,unsigned int L, unsigned int m);
	Crbf();
	virtual ~Crbf();
	double *m_y;
protected:
	void freeTrainingData();
	fstream m_file;
	char *m_fname;
	bool m_training;
	CTMemory *m_X;

	double* m_u; //output hidden layer
	double **m_lambda;
	double **m_r;
	double **m_w;

	double** m_d_lambda;
	double** m_d_lambda_o;
	double **m_d_r;
	double **m_d_r_o;	
	double **m_d_w;
	double **m_d_w_o;

	unsigned long** m_xTrainning;
	unsigned int** m_fTrainning;
	unsigned int *m_f;
	unsigned int m_n; //input layer nr
	unsigned int m_L; //hidden layer nr
	unsigned int m_m; //output layer nr
	unsigned int m_type[5];
	unsigned long *m_dim;
	double m_alpha;
	double m_eta;
	double m_error;
	unsigned int m_nrElements;
};

#endif // !defined(AFX_RBF_H__C13AF841_3CB7_11DA_B910_CF5D952B9F0C__INCLUDED_)
