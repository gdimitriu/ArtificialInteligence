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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Crbf::Crbf()
{
	m_n=1024;
	m_m=5;
	m_L=10;
	m_lambda=NULL;
	m_r=NULL;
	m_w=NULL;
	for(int i=0;i<5;i++) m_type[i]=0;
	m_x=NULL;
	m_y=NULL;
	m_f=NULL;
	m_u=NULL;
	m_alpha=0.0;
	m_eta=0.0;
	m_training=false;
	m_d_r=NULL;
	m_d_r_o=NULL;
	m_d_w=NULL;
	m_d_w_o=NULL;
	m_d_lambda=NULL;
	m_d_lambda_o=NULL;
	m_fname=NULL;
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
	m_X->GetDim(m_dim,m_m);
	for(i=0;i<m_m;i++) for(j=0;j<m_L;j++) m_lambda[i][j]=0.0;
	for(h=0;h<m_n;h++)
		for(j=0;j<m_L;j++)
		{
			m_r[j][h]=0.0;
			m_w[j][h]=0.0;
		}
	while(1)
	{
		run=m_X->GetEx(&m_x,&type);
		if(run==false) break;
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_w[j][h]+=m_x[h]/m_dim[type-1];
	}
	m_X->Reset();
	while(1)
	{
		run=m_X->GetEx(&m_x,&type);
		if(run==false) break;
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_r[j][h]+=pow(m_x[h]-m_w[j][h],2)/m_dim[type-1];
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
		m_d_r[i]=temp;
		temp+=m_n;
	}
	m_d_w=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_d_w[i]=temp;
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
		m_d_r_o[i]=temp;
		temp+=m_n;
	}
	m_d_w_o=(double **)calloc(m_L,sizeof(double *));
	temp=(double *)calloc(m_L*m_n,sizeof(double));
	for(i=0;i<m_L;i++)
	{
		m_d_w_o[i]=temp;
		temp+=m_n;
	}
}

bool Crbf::Training(long steps)
{
	long step;
	m_training=true;
	bool run;
	unsigned long j,k,h;
	unsigned int type;
	step=0;
	while(step<steps)
	{
		//refine lambda
		m_X->Reset();
		for(j=0;j<m_L;j++)
			for(k=0;k<m_m;k++)
				m_d_lambda[k][j]=0.0;
		while(1)
		{
			run=m_X->GetEx(&m_x,&type);
			if(run==false) break;
			for(k=0;k<m_m;k++) m_f[k]=0;
			m_f[type-1]=1;
			Run(m_x);
			for(j=0;j<m_L;j++)
				for(k=0;k<m_m;k++)
					m_d_lambda[k][j]+=m_u[j]*m_y[k]*(1-m_y[k])*(m_f[k]-m_y[k]);
		}
		for(j=0;j<m_L;j++)
			for(k=0;k<m_m;k++)
			{
				m_d_lambda[k][j]=m_d_lambda[k][j]*(-2)*m_eta+m_alpha*m_d_lambda_o[k][j];
				m_lambda[k][j]=m_lambda[k][j]-m_d_lambda[k][j];
				m_d_lambda_o[k][j]=m_d_lambda[k][j];
			}
		//refine r
		m_X->Reset();
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_d_r[j][h]=0.0;
		while(1)
		{
			run=m_X->GetEx(&m_x,&type);
			if(run==false) break;
			for(k=0;k<m_m;k++) m_f[k]=0;
			m_f[type-1]=1;
			Run(m_x);
			for(j=0;j<m_L;j++)
				for(h=0;h<m_n;h++)
					for(k=0;k<m_m;k++)
						m_d_r[j][h]+=m_u[j]*m_lambda[k][j]*m_y[k]*(1-m_y[k])*(m_f[k]-m_y[k])*pow(m_w[j][h]-m_x[h],2)/pow(m_r[j][h],3);
		}
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
			{
				m_d_r[j][h]=m_d_r[j][h]*(-4)*m_eta+m_alpha*m_d_r_o[j][h];
				m_r[j][h]=m_r[j][h]-m_d_r[j][h];
				m_d_r_o[j][h]=m_d_r[j][h];
			}
		//refine w
		m_X->Reset();
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_d_w[j][h]=0.0;
		while(1)
		{
			run=m_X->GetEx(&m_x,&type);
			if(run==false) break;
			for(k=0;k<m_m;k++) m_f[k]=0;
			m_f[type-1]=1;
			Run(m_x);
			for(j=0;j<m_L;j++)
				for(h=0;h<m_n;h++)
					for(k=0;k<m_m;k++)
						m_d_w[j][h]+=m_u[j]*m_lambda[k][j]*m_y[k]*(1-m_y[k])*(m_f[k]-m_y[k])*(m_w[j][h]-m_x[h])/pow(m_r[j][h],2);
		}
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
			{
				m_d_r[j][h]=m_d_w[j][h]*4*m_eta+m_alpha*m_d_w_o[j][h];
				m_w[j][h]=m_w[j][h]-m_d_w[j][h];
				m_d_w_o[j][h]=m_d_w[j][h];
			}
/*		m_X->Reset();
		for(j=0;j<m_L;j++)
			for(k=0;k<m_m;k++)
				m_d_lambda[k][j]=0.0;
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_d_r[j][h]=0.0;
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
				m_d_w[j][h]=0.0;
		while(1)
		{
			run=m_X->GetEx(&m_x,&type);
			if(run==false) break;
			for(k=0;k<m_m;k++) m_f[k]=0;
			m_f[type-1]=1;
			Run(m_x);
			for(j=0;j<m_L;j++)
				for(k=0;k<m_m;k++)
					m_d_lambda[k][j]+=m_u[j]*m_y[k]*(1-m_y[k])*(m_f[k]-m_y[k]);
			for(j=0;j<m_L;j++)
				for(h=0;h<m_n;h++)
					for(k=0;k<m_m;k++)
						m_d_r[j][h]+=m_u[j]*m_lambda[k][j]*m_y[k]*(1-m_y[k])*(m_f[k]-m_y[k])*pow(m_w[j][h]-m_x[h],2)/pow(m_r[j][h],3);
			for(j=0;j<m_L;j++)
				for(h=0;h<m_n;h++)
					for(k=0;k<m_m;k++)
						m_d_w[j][h]+=m_u[j]*m_lambda[k][j]*m_y[k]*(1-m_y[k])*(m_f[k]-m_y[k])*(m_w[j][h]-m_x[h])/pow(m_r[j][h],2);
		}
		for(j=0;j<m_L;j++)
			for(k=0;k<m_m;k++)
			{
				m_d_lambda[k][j]=m_d_lambda[k][j]*(-2)*m_eta+m_alpha*m_d_lambda_o[k][j];
				m_lambda[k][j]=m_lambda[k][j]-m_d_lambda[k][j];
				m_d_lambda_o[k][j]=m_d_lambda[k][j];
			}
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
			{
				m_d_r[j][h]=m_d_r[j][h]*(-4)*m_eta+m_alpha*m_d_r_o[j][h];
				m_r[j][h]=m_r[j][h]-m_d_r[j][h];
				m_d_r_o[j][h]=m_d_r[j][h];
			}
		for(j=0;j<m_L;j++)
			for(h=0;h<m_n;h++)
			{
				m_d_r[j][h]=m_d_w[j][h]*4*m_eta+m_alpha*m_d_w_o[j][h];
				m_w[j][h]=m_w[j][h]-m_d_w[j][h];
				m_d_w_o[j][h]=m_d_w[j][h];
			} */
		//increment the step
		step++;
	}	
	return false;
}

unsigned int Crbf::Run(unsigned long *x)
{
	double sigma;
	unsigned int j,k,h;
	for(j=0;j<m_L;j++)
	{
		sigma=0.0;
		for(h=0;h<m_n;h++)
			sigma+=pow((m_w[j][h]-m_x[h])/m_r[j][h],2);
		m_u[j]=exp(-sigma);
	}
	for(k=0;k<m_m;k++)
	{
		sigma=0.0;
		for(j=0;j<m_L;j++)
			sigma+=m_u[j]*m_lambda[k][j];
		m_y[k]=1/(1+exp(-sigma));
	}
	double max;
	unsigned int out;
	max=m_y[0];
	out=1;
	for(k=1;k<m_m;k++)
		if(max<m_y[k])
		{
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
	m_file.write((char *)(&m_lambda[0][0]),m_m*m_L*sizeof(double));
	m_file.write((char *)(&m_r[0][0]),m_n*m_L*sizeof(double));
	m_file.write((char *)(&m_w[0][0]),m_n*m_L*sizeof(double));
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
