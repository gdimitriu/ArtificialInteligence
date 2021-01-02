// Network3.cpp: implementation of the CNetwork3 class.
//
//////////////////////////////////////////////////////////////////////
/*
	Copyright 2005 Gabriel Dimitriu

    This file is part of NN back propagation.

    NN back propagation is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    NN back propagation is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NN back propagation; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/

#include "stdafx.h"
#include "Network3.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetwork3::CNetwork3()
{

}

CNetwork3::~CNetwork3()
{
	if(m_V!=NULL)
	{
		free(*m_V);
		free(m_V);
	}
	if(m_W!=NULL)
	{
		free(*m_W);
		free(m_W);
	}
	if(m_fo!=NULL) free(m_fo);
	if(m_fh!=NULL) free(m_fh);
	if(m_ho!=NULL) free(m_ho);
	if(m_hh!=NULL) free(m_hh);
}
CNetwork3::CNetwork3(long n,long p,long m)
{
	m_m=m;
	m_n=n;
	m_p=p;
	double *temp;
	long i;
	m_V=(double **)calloc(m_n,sizeof(double *));
	temp=(double *)calloc(m_n*m_p,sizeof(double));
	for(i=0;i<m_n;i++)
	{
		m_V[i]=temp;
		temp+=m_p;
	}
	m_W=(double **)calloc(m_p,sizeof(double *));
	temp=(double *)calloc(m_p*m_m,sizeof(double));
	for(i=0;i<m_p;i++)
	{
		m_W[i]=temp;
		temp+=m_m;
	}
	m_fo=(double *)calloc(m_m,sizeof(double));
	m_fh=(double *)calloc(m_p,sizeof(double));
	m_ho=(double *)calloc(m_m,sizeof(double));
	m_hh=(double *)calloc(m_p,sizeof(double));
}

bool CNetwork3::train(double **X, double **Y, long N,double rho,double errmax,long nrMaxEp,int type,long pos)
{
	long i,j,k,q;
	double ftemp;
	double *temp;
	double rho1,rho2;
	m_Vn=(double **)calloc(m_n,sizeof(double *));
	temp=(double *)calloc(m_n*m_p,sizeof(double));
	for(i=0;i<m_n;i++)
	{
		m_Vn[i]=temp;
		temp+=m_p;
	}
	m_Wn=(double **)calloc(m_p,sizeof(double *));
	temp=(double *)calloc(m_p*m_m,sizeof(double));
	for(i=0;i<m_p;i++)
	{
		m_Wn[i]=temp;
		temp+=m_m;
	}
	srand(0);
	for(i=0;i<m_n;i++)
		for(j=0;j<m_p;j++)
			if(pos==0) m_Vn[i][j]=-1.0+2.0*(double)rand()/(double)RAND_MAX;
			else m_Vn[i][j]=pos*(double)rand()/(double)RAND_MAX;
	for(i=0;i<m_p;i++)
		for(j=0;j<m_m;j++)
			if(pos==0) m_Wn[i][j]=-1.0+2.0*(double)rand()/(double)RAND_MAX;
			else m_Wn[i][j]=(double)rand()/(double)RAND_MAX;
	m_nr_epoci=0;
	m_gatainvatare=true;
	rho1=rho2=rho;
	do
	{
		m_playAgain=0;
		m_nr_epoci++;
		if(m_nr_epoci>nrMaxEp)
		{
			m_gatainvatare=false;
			break;
		}
		else m_gatainvatare=true;
		for(k=0;k<N;k++)
		{
			for(q=0;q<m_p;q++)
				for(j=0;j<m_m;j++)
					m_W[q][j]=m_Wn[q][j];
			for(i=0;i<m_n;i++)
				for(q=0;q<m_p;q++)
					m_V[i][q]=m_Vn[i][q];
			for(q=0;q<m_p;q++)
			{
				m_hh[q]=0.0;
				for(i=0;i<m_n;i++)
					m_hh[q]=m_hh[q]+X[i][k]*m_V[i][q];
				m_fh[q]=1/(1+exp(-m_hh[q]));
			}
			for(j=0;j<m_m;j++)
			{
				m_ho[j]=0.0;
				for(q=0;q<m_p;q++)
					m_ho[j]=m_ho[j]+m_fh[q]*m_W[q][j];
				m_fo[j]=1/(1+exp(-m_ho[j]));
			}
			m_eps=0.0;
			for(j=0;j<m_m;j++)
				m_eps=m_eps+(Y[j][k]-m_fo[j])*(Y[j][k]-m_fo[j]);
			if(m_eps>errmax)
			{
				m_playAgain++;
				for(j=0;j<m_m;j++)
					for(q=0;q<m_p;q++)
						m_Wn[q][j]=m_W[q][j]+2*rho2*(Y[j][k]-m_fo[j])*m_fo[j]*(1-m_fo[j])*m_fh[q];
				for(i=0;i<m_n;i++)
					for(q=0;q<m_p;q++)
					{
						ftemp=0.0;
						for(j=0;j<m_m;j++)
							ftemp=ftemp+(Y[j][k]-m_fo[j])*m_fo[j]*(1-m_fo[j])*m_W[q][j];
						m_Vn[i][q]=m_V[i][q]+2*rho1*ftemp*m_fh[q]*(1-m_fh[q])*X[i][k];
					}
			}
		}
		switch(type)
		{
		case 0:
		{
			ftemp=0.0;
			for(j=0;j<m_m;j++)
				ftemp+=fabs(m_Wn[0][j]);
			rho2=ftemp;
			for(q=1;q<m_p;q++)
			{
				ftemp=0.0;
				for(j=0;j<m_m;j++)
					ftemp+=fabs(m_Wn[q][j]);
				if(ftemp>rho2) rho2=ftemp;
			}
			ftemp=0.0;
			for(q=0;q<m_p;q++)
				ftemp+=fabs(m_Vn[0][q]);
			rho1=ftemp;
			for(i=1;i<m_n;i++)
			{
				ftemp=0.0;
				for(q=0;q<m_p;q++)
					ftemp+=fabs(m_Vn[i][q]);
				if(ftemp>rho1) rho1=ftemp;
			}
			break;
		}
		case 1:
		{
			ftemp=0.0;
			for(j=0;j<m_m;j++)
				ftemp+=fabs(m_Wn[0][j]);
			rho2=ftemp;
			for(q=1;q<m_p;q++)
			{
				ftemp=0.0;
				for(j=0;j<m_m;j++)
					ftemp+=fabs(m_Wn[q][j]);
				if(ftemp>rho2) rho2=ftemp;
			}
			break;
		}
		case 2:
		{
			ftemp=0.0;
			for(q=0;q<m_p;q++)
				ftemp+=fabs(m_Vn[0][q]);
			rho1=ftemp;
			for(i=1;i<m_n;i++)
			{
				ftemp=0.0;
				for(q=0;q<m_p;q++)
					ftemp+=fabs(m_Vn[i][q]);
				if(ftemp>rho1) rho1=ftemp;
			}
			break;
		}
		case 10:
		{
			ftemp=0.0;
			for(q=0;q<m_p;q++)
				ftemp+=fabs(m_Wn[q][0]);
			rho2=ftemp;
			for(j=1;j<m_m;j++)
			{
				ftemp=0.0;
				for(q=0;q<m_p;q++)
					ftemp+=fabs(m_Wn[q][j]);
				if(ftemp>rho2) rho2=ftemp;
			}
			ftemp=0.0;
			for(i=0;i<m_n;i++)
				ftemp+=fabs(m_Vn[i][0]);
			rho1=ftemp;
			for(q=1;q<m_p;q++)
			{
				ftemp=0.0;
				for(i=0;i<m_n;i++)
					ftemp+=fabs(m_Vn[i][q]);
				if(ftemp>rho1) rho1=ftemp;
			}
			if(rho1>0.5) rho1=1/rho1;
			if(rho2>0.5) rho2=1/rho2;
			break;
		}
		case 20:
		{
			ftemp=0.0;
			for(q=0;q<m_p;q++)
				for(j=0;j<m_m;j++)
					ftemp+=m_Wn[q][j]*m_Wn[q][j];
			rho2=sqrt(ftemp);
			ftemp=0.0;
			for(q=0;q<m_p;q++)
				for(i=0;i<m_n;i++)
					ftemp+=m_Vn[i][q]*m_Vn[i][q];
			rho1=sqrt(ftemp);
			if(rho1>0.5) rho1=1/rho1;
			if(rho2>0.5) rho2=1/rho2;
			break;
		}
		default:
			{
			}
		}
	}
	while(m_playAgain!=0);
	if(m_Wn!=NULL)
	{
		free(*m_Wn);
		free(m_Wn);
	}
	if(m_Vn!=NULL)
	{
		free(*m_Vn);
		free(m_Vn);
	}
	return m_gatainvatare;
}

void CNetwork3::run(double *X, double *Y)
{
	long i,j;
	for(j=0;j<m_p;j++)
	{
		m_hh[j]=0.0;
		for(i=0;i<m_n;i++)
			m_hh[j]=m_hh[j]+X[i]*m_V[i][j];
		m_fh[j]=1/(1+exp(-m_hh[j]));
	}
	for(j=0;j<m_m;j++)
	{
		m_ho[j]=0.0;
		for(i=0;i<m_p;i++)
			m_ho[j]=m_ho[j]+m_fh[i]*m_W[i][j];
		m_fo[j]=1/(1+exp(-m_ho[j]));
	}
	for(i=0;i<m_m;i++)
		Y[i]=m_fo[i];
}

void CNetwork3::print()
{
long q,i,j;
	cout<<"V Matrix\n";
	for(i=0;i<m_n;i++)
	{
		for(q=0;q<m_p;q++)
			cout<<m_V[i][q]<<" ";
		cout<<endl;
	}	
	cout<<"W Matrix\n";
	for(q=0;q<m_p;q++)
	{
		for(j=0;j<m_m;j++)
			cout<<m_W[q][j]<<" ";
		cout<<endl;
	}
}
