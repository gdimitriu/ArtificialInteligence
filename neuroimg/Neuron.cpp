// Neuron.cpp: implementation of the CNeuron class.
//
//////////////////////////////////////////////////////////////////////
/*
	Copyright 2005 Gabriel Dimitriu

    This file is part of Neuronal Network for OCR.

    Data Structures Laboratory is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Neuronal Network for OCR is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neuronal Network for OCR; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  
*/

#include "stdafx.h"
#include "Neuron.h"
#include <iostream>

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeuron::CNeuron()
{

}
CNeuron::CNeuron(long n,long seed)
{
	m_N=n;
	srand((unsigned)time( NULL )+seed );
	m_theta=-1+2*rand()/(double)RAND_MAX;
	m_w=new double[n];
	for(long i=0;i<n;i++) m_w[i]=-1+2*rand()/(double)RAND_MAX;
//	cout<<"Init\n";cout.flush();
//	for(i=0;i<n;i++)
//		cout<<"m_w["<<i<<"]="<<m_w[i]<<endl;
}
CNeuron::~CNeuron()
{
	delete []m_w;
}

long CNeuron::Training(double *x, double *y1,long nr)
{
	double **z;
	double *temp;
	long i,j;
	long iteratii=0;
	z=(double **)calloc(nr,sizeof(double *));
	temp=(double *)calloc((m_N+1)*nr,sizeof(double));
	for(i=0;i<nr;i++)
	{
		z[i]=temp;
		temp+=m_N+1;
	}
	for(i=0;i<nr;i++)
	{
		z[i][0]=y1[i];
		if(y1[i]<0) for(j=0;j<m_N;j++) z[i][j+1]=-x[i*m_N+j];
		else for(j=0;j<m_N;j++) z[i][j+1]=x[i*m_N+j];
	}
	m_theta=-m_theta;
	bool gata;
	bool test;
	double val;
	do
	{
		test=false;
		do
		{
			gata=false;
			for(i=0;i<nr;i++)
			{
				val=m_theta*z[i][0];
				for(j=0;j<m_N;j++)
					val=val+m_w[j]*z[i][j+1];
				if(val<0)
				{
					gata=true;
					m_theta+=z[i][0];
					for(j=0;j<m_N;j++)
						m_w[j]+=z[i][j+1];
				}
				if(val==0.0)
				{
					gata=false;
					test=true;
				}
			}
			iteratii++;
		}while(gata==true);
		if(test==true)
		{
			m_theta=-1+2*rand()/(double)RAND_MAX;
			for(long i=0;i<m_N;i++) m_w[i]=-1+2*rand()/(double)RAND_MAX;
			cout<<"Reinit\n";
			for(i=0;i<m_N;i++)
				cout<<"m_w["<<i<<"]="<<m_w[i]<<endl;
		}
//		for(i=0;i<m_N;i++)
//			cout<<"m_w["<<i<<"]="<<m_w[i]<<endl;
	}while(test==true);
	m_theta=-m_theta;
	free(*z);
	free(z);
	return iteratii;
}

double CNeuron::Compute(double *x)
{
	double val=0.0;
	for(long i=0;i<m_N;i++)
		val=val+m_w[i]*x[i];
	if(val>=m_theta) return 1;
	else return -1;
}
