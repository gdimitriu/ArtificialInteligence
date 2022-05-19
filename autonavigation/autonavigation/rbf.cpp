// rbf.cpp: implementation of the Crbf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "rbf.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Crbf::Crbf()
{
	m_n=1024;
	m_m=5;
	m_L=10;		
	for(int i=0;i<5;i++) m_type[i]=0;
	m_xTrainning =NULL;
	m_fTrainning = NULL;
	m_y=NULL;
	m_f=NULL;
	m_u=NULL;
	m_alpha=0.0;
	m_eta=0.0;
	m_training=false;
	m_r = NULL;
	m_d_r=NULL;
	m_d_r_o=NULL;
	m_w = NULL;
	m_d_w=NULL;
	m_d_w_o=NULL;
	m_lambda = NULL;
	m_d_lambda=NULL;
	m_d_lambda_o=NULL;
	m_fname=NULL;
	m_nrElements = 0;
	m_dim = NULL;
}

Crbf::~Crbf()
{
	if(m_lambda!=NULL)
	{
		free(*m_lambda);
		free(m_lambda);
	}
	if(m_d_lambda!=NULL)
	{
		free(*m_d_lambda);
		free(m_d_lambda);
	}
	if(m_d_lambda_o!=NULL)
	{
		free(*m_d_lambda_o);
		free(m_d_lambda_o);
	}
	if(m_r!=NULL)
	{
		free(*m_r);
		free(m_r);
	}
	if(m_d_r!=NULL)
	{
		free(*m_d_r);
		free(m_d_r);
	}
	if(m_d_r_o!=NULL)
	{
		free(*m_d_r_o);
		free(m_d_r_o);
	}
	if(m_w!=NULL)
	{
		free(*m_w);
		free(m_w);
	}
	if(m_d_w!=NULL)
	{
		free(*m_d_w);
		free(m_d_w);
	}
	if(m_d_w_o!=NULL)
	{
		free(*m_d_w_o);
		free(m_d_w_o);
	}
	if(m_f!=NULL) free(m_f);
	if(m_y!=NULL) free(m_y);
	if(m_dim!=NULL) free(m_dim);
	if(m_u!=NULL) free(m_u);
	if(m_fname!=NULL) delete[] m_fname;
}

void Crbf::Create(unsigned int n, unsigned int L, unsigned int m)
{
	unsigned int i;
	double *temp;
	m_n=n;
	m_L=L;
	m_m=m;
	m_lambda=(double **)calloc(m_m,sizeof(double *));
	temp=(double *)calloc(m_m*m_L,sizeof(double));
	for(i=0;i<m_m;i++)
	{
		m_lambda[i]=temp;
		temp+=m_L;
	}
	m_r=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_r[i]=temp;
		temp+=m_n;
	}
	m_w=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_w[i]=temp;
		temp+=m_n;
	}
	m_f=(unsigned int *)calloc(m_m,sizeof(unsigned int));
	m_y=(double *)calloc(m_m,sizeof(double));
	m_dim=(unsigned long *)calloc(m_m,sizeof(unsigned long));
	m_u=(double *)calloc(m_L,sizeof(double));
}

void Crbf::Delete()
{
	if(m_lambda!=NULL) {
		free(*m_lambda);
		free(m_lambda);
		m_lambda = NULL;
	}
	if(m_r!=NULL) {
		free(*m_r);
		free(m_r);
		m_r = NULL;
	}

	if(m_w!=NULL) {
		free(*m_w);
		free(m_w);
		m_w = NULL;
	}
	if (m_f != NULL) {
		free(m_f);
		m_f = NULL;
	}
	if (m_y != NULL) {
		free(m_y);
		m_y = NULL;
	}
	if (m_dim != NULL) {
		free(m_dim);
		m_dim = NULL;
	}
	if (m_u != NULL) {
		free(m_u);
		m_u = NULL;
	}
}

void Crbf::freeTrainingData() {
	if (m_xTrainning != NULL) {
		free(*m_xTrainning);
		free(m_xTrainning);
		m_xTrainning = NULL;
	}
	if (m_fTrainning != NULL) {
		free(*m_fTrainning);
		free(m_fTrainning);
		m_fTrainning = NULL;
	}
	if (m_d_r != NULL) {
		free(*m_d_r);
		free(m_d_r);
		m_d_r = NULL;
	}
	if (m_d_r_o != NULL) {
		free(*m_d_r_o);
		free(m_d_r_o);
		m_d_r_o = NULL;
	}
	if (m_d_w != NULL) {
		free(*m_d_w);
		free(m_d_w);
		m_d_w = NULL;
	}
	if (m_d_w_o != NULL) {
		free(*m_d_w_o);
		free(m_d_w_o);
		m_d_w_o = NULL;
	}
	if (m_d_lambda != NULL) {
		free(*m_d_lambda);
		free(m_d_lambda);
		m_d_lambda = NULL;
	}
	if (m_d_lambda_o != NULL) {
		free(*m_d_lambda_o);
		free(m_d_lambda_o);
		m_d_lambda_o = NULL;
	}
}
void Crbf::SetMemory(CTMemory *mem)
{
	m_X=mem;
}

void Crbf::InitT(double alpha,double eta, double error)
{
	unsigned long j,h,i;
	unsigned int type;
	bool run;
	double *temp;
	m_nrElements = m_X->GetTotalNr();
	m_xTrainning = (unsigned long**)calloc(m_nrElements, sizeof(unsigned long*));
	unsigned long* tempL = (unsigned long*)calloc(m_nrElements * m_n, sizeof(unsigned long));
	for (int i = 0; i < m_nrElements; i++) {
		m_xTrainning[i] = tempL;
		tempL += m_n;
	}
	m_fTrainning = (unsigned int**)calloc(m_nrElements, sizeof(unsigned int*));
	unsigned int* tempI = (unsigned int*)calloc(m_nrElements * m_m, sizeof(unsigned int));
	for (int i = 0; i < m_nrElements; i++) {
		m_fTrainning[i] = tempI;
		tempI += m_m;
	}
	unsigned int *typeTrainning = (unsigned int*)calloc(m_nrElements, sizeof(unsigned int));
	m_X->GetDim(m_dim,m_m);
	for(i=0;i<m_m;i++)
		for(j=0;j<m_L;j++)
			m_lambda[i][j]=0.0;
	for(h=0;h<m_n;h++)
		for(j=0;j<m_L;j++) {
			m_r[j][h]=0.0;
			m_w[j][h]=0.0;
		}
	m_X->Reset();
	for (int i = 0; i < m_nrElements; i++) {
		m_X->GetEx(m_xTrainning[i], &type);
		m_fTrainning[i][type - 1] = 1;
		typeTrainning[i] = type;
	}
	for (int i = 0; i < m_nrElements; i++) {
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_w[j][h]+= m_xTrainning[i][h]/m_dim[typeTrainning[i] -1];
	}
	for (int i = 0; i < m_nrElements; i++) {
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_r[j][h]+=pow(m_xTrainning[i][h]-m_w[j][h],2)/m_nrElements;
	}
	m_alpha=alpha;
	m_eta=eta;
	m_error = error;
	m_d_lambda=(double **)calloc(m_m,sizeof(double *));
	temp=(double *)calloc(m_m*m_L,sizeof(double));
	for(i=0;i<m_m;i++)
	{
		m_d_lambda[i]=temp;
		temp+=m_L;
	}
	m_d_r=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_d_r[i] = temp;
		temp+=m_n;
	}
	m_d_w=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_d_w[i] = temp;
		temp+=m_n;
	}
	m_d_lambda_o=(double **)calloc(m_m,sizeof(double *));
	temp=(double *)calloc(m_m*m_L,sizeof(double));
	for(i=0;i<m_m;i++)
	{
		m_d_lambda_o[i]=temp;
		temp+=m_L;
	}
	m_d_r_o=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_d_r_o[i] = temp;
		temp+=m_n;
	}
	m_d_w_o=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_d_w_o[i] = temp;
		temp+=m_n;
	}
	free(typeTrainning);
}

bool Crbf::Training(long steps)
{
	long step;
	m_training=true;
	bool run;
	unsigned long j,k,h;
	unsigned int type;
	double errorQuadratic = 0.0;
	step=0;
	double **m_yTrainning = (double**)calloc(m_nrElements, sizeof(double*));
	double *tempD = (double*)calloc(m_nrElements * m_m, sizeof(double));
	for (int i = 0; i < m_nrElements; i++) {
		m_yTrainning[i] = tempD;
		tempD += m_m;
	}
	double** m_uTrainning = (double**)calloc(m_nrElements, sizeof(double*));
	tempD = (double*)calloc(m_nrElements * m_L, sizeof(double));
	for (int i = 0; i < m_nrElements; i++) {
		m_uTrainning[i] = tempD;
		tempD += m_L;
	}
	while(step<steps) {		
		m_X->Reset();
		for(j=0;j<m_L;j++)
			for(k=0;k<m_m;k++)
				m_d_lambda[k][j]=0.0;
		unsigned int nr = 0;
		
		for(nr = 0; nr < m_nrElements; nr++) {			
			Run(m_xTrainning[nr]);
			memcpy(m_yTrainning[nr], m_y, sizeof(double) * m_m);
			memcpy(m_uTrainning[nr], m_u, sizeof(double) * m_L);
		}
		for (nr = 0; nr < m_nrElements; nr++) {
			//calculate derivate
			for (j = 0; j < m_L; j++)
				for (k = 0; k < m_m; k++)
					m_d_lambda[k][j] += m_uTrainning[nr][j] * m_yTrainning[nr][k] * (1 - m_yTrainning[nr][k]) *
										(m_fTrainning[nr][k] - m_yTrainning[nr][k]);
			//refine lambda
			for (j = 0; j < m_L; j++)
				for (k = 0; k < m_m; k++) {
					m_d_lambda[k][j] = m_d_lambda[k][j] * (-2) * m_eta + m_alpha * m_d_lambda_o[k][j];
					m_lambda[k][j] -= m_d_lambda[k][j];
					m_d_lambda_o[k][j] = m_d_lambda[k][j];
				}
		}
		for (nr = 0; nr < m_nrElements; nr++) {
			//refine r
			for (j = 0; j < m_L; j++)
				for (h = 0; h < m_n; h++)
					for (k = 0; k < m_m; k++)
						m_d_r[j][h] += m_uTrainning[nr][j] * m_lambda[k][j] * m_yTrainning[nr][k] * (1 - m_yTrainning[nr][k]) *
										(m_fTrainning[nr][k] - m_yTrainning[nr][k]) * 
										pow(m_w[j][h] - m_xTrainning[nr][h], 2) / pow(m_r[j][h], 3);
			for (j = 0; j < m_L; j++)
				for (h = 0; h < m_n; h++) {
					m_d_r[j][h] = m_d_r[j][h] * (-4) * m_eta + m_alpha * m_d_r_o[j][h];
					m_r[j][h] -= m_d_r[j][h];
					m_d_r_o[j][h] = m_d_r[j][h];
				}
		}
		for (nr = 0; nr < m_nrElements; nr++) {
			for (j = 0; j < m_L; j++)
				for (h = 0; h < m_n; h++)
					for (k = 0; k < m_m; k++)
						m_d_w[j][h] += m_uTrainning[nr][j] * m_lambda[k][j] * m_yTrainning[nr][k] * (1 - m_yTrainning[nr][k]) *
										(m_fTrainning[nr][k] - m_yTrainning[nr][k]) * 
										(m_w[j][h] - m_xTrainning[nr][h]) / pow(m_r[j][h], 2);
			//refine w
			for (j = 0; j < m_L; j++)
				for (h = 0; h < m_n; h++) {
					m_d_w[j][h] = m_d_w[j][h] * 4 * m_eta + m_alpha * m_d_w_o[j][h];
					m_w[j][h] -= m_d_w[j][h];
					m_d_w_o[j][h] = m_d_w[j][h];
				}
		}
		errorQuadratic = 0.0;
		for (nr = 0; nr < m_nrElements; nr++) {
			Run(m_xTrainning[nr]);
			for (k = 0; k < m_m; k++)
				errorQuadratic += pow(m_fTrainning[nr][k] - m_y[k], 2);
		}
		//increment the step
		step++;
		if (errorQuadratic < m_error) {
			free(*m_yTrainning);
			free(m_yTrainning);
			free(*m_uTrainning);
			free(m_uTrainning);
			freeTrainingData();
			return true;
		}			
	}
	free(*m_yTrainning);
	free(m_yTrainning);
	free(*m_uTrainning);
	free(m_uTrainning);
	freeTrainingData();
	return false;
}

unsigned int Crbf::Run(unsigned long *x)
{
	double sigma;
	unsigned int j,k,h;
	for(j=0;j<m_L;j++) {
		sigma=0.0;
		for(h=0;h<m_n;h++)
			sigma+=pow((m_w[j][h]-x[h])/m_r[j][h],2);
		m_u[j]=exp(-sigma);
	}
	for(k=0;k<m_m;k++) {
		sigma=0.0;
		for(j=0;j<m_L;j++)
			sigma+=m_u[j]*m_lambda[k][j];
		m_y[k]=1.0/(1+exp(-sigma));
	}
	double max;
	unsigned int out;
	max=m_y[0];
	out=1;
	for(k=1;k<m_m;k++)
		if(max<m_y[k]) {
			max=m_y[k];
			out=k+1;
		}
	return out;
}


void Crbf::SetFileName(char *name)
{
	if(m_fname!=NULL) delete[] m_fname;
	m_fname=new char[strlen(name)];
	m_fname=strcpy(m_fname,name);
}

void Crbf::Save()
{
	if(m_file.is_open()!=0) m_file.close();
	m_file.open(m_fname,ios::out | ios::binary);
	m_file.seekp(0,ios::beg);
	m_file.write((char *)(&m_n),sizeof(unsigned int));
	m_file.write((char *)(&m_L),sizeof(unsigned int));
	m_file.write((char *)(&m_m),sizeof(unsigned int));
	m_file.write((char *)(&m_lambda[0][0]), m_m*m_L*sizeof(double));
	m_file.write((char *)(&m_r[0][0]), m_n*m_L*sizeof(double));
	m_file.write((char *)(&m_w[0][0]), m_n*m_L*sizeof(double));
	m_file.flush();
	m_file.close();
}

void Crbf::Load()
{
	if(m_file.is_open()!=0) m_file.close();
	m_file.open(m_fname,ios::in | ios::binary);
	m_file.seekg(0,ios::beg);
	unsigned int n,L,m;
	m_file.read((char *)(&n),sizeof(unsigned int));
	m_file.read((char *)(&L),sizeof(unsigned int));
	m_file.read((char *)(&m),sizeof(unsigned int));
	Delete();
	Create(n,L,m);
	m_file.read((char *)(&m_lambda[0][0]),m_m*m_L*sizeof(double));
	m_file.read((char *)(&m_r[0][0]),m_n*m_L*sizeof(double));
	m_file.read((char *)(&m_w[0][0]),m_n*m_L*sizeof(double));
	m_file.flush();
	m_file.close();
}
