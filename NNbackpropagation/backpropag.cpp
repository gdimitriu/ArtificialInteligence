// backpropag.cpp : Defines the entry point for the console application.
//
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
#include <iostream>
#include "Network3.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
	CNetwork3 *net1,*net2,*net3;
	double **X,**Y;
	double *temp;
	long i,j;
	long N;
	long out;
	long in;
	long hidden;
	N=11;
	out=1;
	in=2;
	X=(double **)calloc(in,sizeof(double *));
	temp=(double *)calloc(N*in,sizeof(double));
	for(i=0;i<in;i++)
	{
		X[i]=temp;
		temp+=N;
	}
	Y=(double **)calloc(out,sizeof(double *));
	temp=(double *)calloc(N*out,sizeof(double));
	for(i=0;i<out;i++)
	{
		Y[i]=temp;
		temp+=N;
	}
	double *x,*y,*z;
	x=(double *)calloc(in,sizeof(double));
	y=(double *)calloc(out,sizeof(double));
	z=(double *)calloc(in,sizeof(double));

	double rho;
	rho=0.05;
	hidden=16;

	X[0][0]=0.0;
	X[1][0]=0.0;
	Y[0][0]=0;
	X[0][1]=0.0;
	X[1][1]=1.0;
	Y[0][1]=0;

	X[0][2]=1.0;
	X[1][2]=0.0;
	Y[0][2]=0;
	X[0][3]=1.0;
	X[1][3]=1.0;
	Y[0][3]=1;

	X[0][4]=0.2;
	X[1][4]=0.2;
	Y[0][4]=0;
	X[0][5]=0.6;
	X[1][5]=0.6;
	Y[0][5]=1;

	X[0][6]=0.1;
	X[1][6]=1.0;
	Y[0][6]=0;
	X[0][7]=0.6;
	X[1][7]=0.4;
	Y[0][7]=0;

	X[0][8]=0.5;
	X[1][8]=0.5;
	Y[0][8]=1;

	X[0][9]=0.5;
	X[1][9]=1.0;
	Y[0][9]=1;
	X[0][10]=1.0;
	X[1][10]=0.5;
	Y[0][10]=1;

	net3=new CNetwork3(in,hidden,out);
	bool succes,dual;
	succes=net3->train(X,Y,N,0.5,0.01,100000,-1,1);
	if(succes==true)
	{
		cout<<"Sucessful training for arbiter in "<<net3->m_nr_epoci<<" steps\n";
	}
	else cout<<"Unsucessful trainning for arbiter in "<<net3->m_nr_epoci<<" steps\n";
	cout.flush();
//from here we have the decision networks
	free(*X);
	free(*Y);
	free(X);
	free(Y);
	in=2;
	double x1,x2;
	double y1,y2;
	double z1,z2;
	switch(in)
	{
	case 3:
		{
			N=9;
			X=(double **)calloc(in,sizeof(double *));
			temp=(double *)calloc(N*in,sizeof(double));
			for(i=0;i<in;i++)
			{
				X[i]=temp;
				temp+=N;
			}
			Y=(double **)calloc(out,sizeof(double *));
			temp=(double *)calloc(N*out,sizeof(double));
			for(i=0;i<out;i++)
			{
				Y[i]=temp;
				temp+=N;
			}
			x1=-10;x2=10;
			y1=-10;y2=10;
			z1=-10;z2=10;
			X[0][0]=x1;
			X[1][0]=y1;
			X[2][0]=z1;
			Y[0][0]=1;
			X[0][1]=x1;
			X[1][1]=y1;
			X[2][1]=z2;
			Y[0][1]=0;
			X[0][2]=x1;
			X[1][2]=y2;
			X[2][2]=z1;
			Y[0][2]=0;
			X[0][3]=x1;
			X[1][3]=y2;
			X[2][3]=z2;
			Y[0][3]=0;

			X[0][4]=x2;
			X[1][4]=y1;
			X[2][4]=z1;
			Y[0][4]=0;
			X[0][5]=x2;
			X[1][5]=y1;
			X[2][5]=z2;
			Y[0][5]=0;
			X[0][6]=x2;
			X[1][6]=y2;
			X[2][6]=z1;
			Y[0][6]=0;
			X[0][7]=x2;
			X[1][7]=y2;
			X[2][7]=z2;
			Y[0][7]=1;

			X[0][8]=0.0;
			X[1][8]=0.0;
			X[2][8]=0.0;
			Y[0][8]=1;
			hidden=36;
			break;
		}
	case 2:
		{
			N=5;
			X=(double **)calloc(in,sizeof(double *));
			temp=(double *)calloc(N*in,sizeof(double));
			for(i=0;i<in;i++)
			{
				X[i]=temp;
				temp+=N;
			}
			Y=(double **)calloc(out,sizeof(double *));
			temp=(double *)calloc(N*out,sizeof(double));
			for(i=0;i<out;i++)
			{
				Y[i]=temp;
				temp+=N;
			}
			x1=-1;x2=1;
			y1=-1;y2=1;
			X[0][0]=x1;
			X[1][0]=y1;
			Y[0][0]=1;
			X[0][1]=x1;
			X[1][1]=y2;
			Y[0][1]=0;
			X[0][2]=x2;
			X[1][2]=y1;
			Y[0][2]=0;
			X[0][3]=x2;
			X[1][3]=y2;
			Y[0][3]=1;
			X[0][4]=0.0;
			X[1][4]=0.0;
			Y[0][4]=1;
			hidden=16;
			break;
		}	
	default:
		{

		}
	}
	rho=0.00005;
	net1=new CNetwork3(in,hidden,out);
	dual=net1->train(X,Y,N,rho,0.001,10000,0,1);
	if(dual==true)
	{
		cout<<"Sucessful training in "<<net1->m_nr_epoci<<" steps\n";
	}
	else cout<<"Unsucessful training in "<<net1->m_nr_epoci<<" steps\n";
	cout.flush();
	if(dual==false)
	{
		net2=new CNetwork3(in,hidden,out);
		succes=net2->train(X,Y,N,rho,0.001,10000,0,-1);
		if(succes==true)
		{
			cout<<"Sucessful training in "<<net2->m_nr_epoci<<" steps\n";
		}
		else cout<<"Unsucessful training in "<<net2->m_nr_epoci<<" steps\n";
	}
//	net->print();
	cout.flush();
	free(x);
	x=(double *)calloc(in,sizeof(double));
	for(i=0;i<N;i++)
	{
		for(j=0;j<in;j++)
		{
			x[j]=X[j][i];
			cout<<x[j]<<" ";
		}
		cout<<"values ";
		net1->run(x,y);
		cout<<y[0];
		if(dual==false)
		{
			z[0]=y[0];
			net2->run(x,y);
			z[1]=y[0];
			cout<<" and "<<y[0];
			net3->run(z,y);
			cout<<" sol "<<y[0];
		}
		cout<<" real "<<Y[0][i];
		cout<<endl;
	}
	x[0]=0.200;
	cout<<x[0]<<" ";
	x[1]=-0.200;
	cout<<x[1]<<" has the value ";
	net1->run(x,y);
	cout<<y[0]<<" ";
	z[0]=y[0];
	net2->run(x,y);
	cout<<y[0]<<" ";
	z[1]=y[0];
	net3->run(z,y);
	cout<<"sol="<<y[0]<<endl;
	return 0;
}
