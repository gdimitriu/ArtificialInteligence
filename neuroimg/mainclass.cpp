// mainclass.cpp: implementation of the mainclass class.
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
#include "neuroimg.h"
#include "mainclass.h"
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mainclass::mainclass()
{
	neuron=new CNeuron(90,2);
	x=NULL;
	y=NULL;
}

mainclass::~mainclass()
{
	if(x!=NULL) free(x);
	if(y!=NULL) free(y);
}

void mainclass::read()
{
	char l[11];
	element *el;
	int i,j,k;
	ifstream ina;
	ifstream inh;
	ina.open("a.txt",ios::in);
	inh.open("h.txt",ios::in);
	while(!ina.eof())
	{
		j=0;
		bool flag;
		flag=false;
		el=new element;
		for(k=0;k<10;k++)
		{
			ina.getline(l,11,'\n');
			if(ina.eof()!=0 && k!=9)
			{
				flag=true;
				break;
			}
			while((!(l[0]=='1' || l[0]=='0')) && ina.eof()==0)
				ina.getline(l,11,'\n');
			for(i=0;i<9;i++)
			{
				if(l[i]=='1')
				{
					el->val[j]=1;
					j++;
				}
				if(l[i]=='0')
				{
					el->val[j]=0;
					j++;
				}
			}
		}
		if(flag==false)
		{
			el->type=1;
			arraya.Add(el);
		}
		else delete el;
		if(ina.eof()!=0) break;
	}
	cout<<"Ready read a\n";
	cout.flush();
	while(!inh.eof())
	{
		j=0;
		bool flag;
		flag=false;
		el=new element();
		for(k=0;k<10;k++)
		{
			inh.getline(l,11,'\n');
			if(inh.eof()!=0 && k!=9)
			{
				flag=true;
				break;
			}
			while((!(l[0]=='1' || l[0]=='0')) && inh.eof()==0 )
				inh.getline(l,11,'\n');
			for(i=0;i<9;i++)
			{
				if(l[i]=='1')
				{
					el->val[j]=1;
					j++;
				}
				if(l[i]=='0')
				{
					el->val[j]=0;
					j++;
				}
			}
		}
		if(flag==false)
		{
			el->type=-1;
			arrayh.Add(el);
		}
		else delete el;
		if(inh.eof()!=0) break;
	}
	cout<<"Ready read h\n";
	cout.flush();
	ina.close();
	inh.close();
}

void mainclass::CreateTrainData()
{
	long i,j,n1,n2,count1,count2;
	element *el;
	n1=arraya.GetSize();
	n2=arrayh.GetSize();
	m_N=n1+n2;
	if(x!=NULL) free(x);
	if(y!=NULL) free(y);
	x=(double *)calloc(90*m_N,sizeof(double));
	y=(double *)calloc(m_N,sizeof(double));
	count1=0;
	count2=0;
	for(i=0;i<n1;i++)
	{
		el=((element *)(arraya.GetAt(i)));
		y[count2]=el->type;
		count2++;
		for(j=0;j<90;j++)
		{
			x[count1]=el->val[j];
			count1++;
		}
	}
	for(i=0;i<n2;i++)
	{
		el=((element *)(arrayh.GetAt(i)));
		y[count2]=el->type;
		count2++;
		for(j=0;j<90;j++)
		{
			x[count1]=el->val[j];
			count1++;
		}
	}
}

void mainclass::Train()
{
	cout<<"Neuron is trained in "<<neuron->Training(x,y,m_N)<<" iterations\n";
	cout.flush();
}

void mainclass::TestInternal(long pos)
{
	double inx[90];
	long n1,n2,i;
	n1=arraya.GetSize();
	n2=arrayh.GetSize();
	element *el;
	if(pos<n1) el=(element *)(arraya.GetAt(pos));
	else el=(element *)(arrayh.GetAt(pos-n1));
	for(i=0;i<90;i++) inx[i]=el->val[i];
	cout<<"Neuron found "<<neuron->Compute(inx)<<" vs real "<<el->type<<endl;
}

long mainclass::GetNumbers()
{
	return m_N;
}

double mainclass::Test(char *ch)
{
	double val;
	char l[11];
	int i,j,k;
	ifstream ina;
	double inx[90];
	ina.open(ch,ios::in);
	j=0;
	bool flag;
	flag=false;
	for(k=0;k<10;k++)
	{
		ina.getline(l,11,'\n');
		if(ina.eof()!=0 && k!=9)
		{
			flag=true;
			break;
		}
		while((!(l[0]=='1' || l[0]=='0')) && ina.eof()==0)
			ina.getline(l,11,'\n');
		for(i=0;i<9;i++)
		{
			if(l[i]=='1')
			{
				inx[j]=1.0;
				j++;
			}
			if(l[i]=='0')
			{
				inx[j]=0.0;
				j++;
			}
		}
	}
	ina.close();
	val=neuron->Compute(inx);
	cout<<val<<endl;
	return val;
}
