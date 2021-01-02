// mainclass.h: interface for the mainclass class.
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

#if !defined(AFX_MAINCLASS_H__E2531F35_5C7F_47E7_90D7_C2E97237F0E5__INCLUDED_)
#define AFX_MAINCLASS_H__E2531F35_5C7F_47E7_90D7_C2E97237F0E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include<fstream>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include"neuron.h"

using namespace std;

class  element : public CObject
{
public:
	element() {};
	~element() {};
	long val[90];
	long type;
};

class mainclass  
{
public:
	double Test(char *ch);
	long GetNumbers();
	void TestInternal(long pos);
	void Train();
	void CreateTrainData();
	void read();
	mainclass();
	virtual ~mainclass();

protected:
	CObArray arraya;
	CObArray arrayh;
	long m_N;
	double* y;
	double* x;
	CNeuron *neuron;
};

#endif // !defined(AFX_MAINCLASS_H__E2531F35_5C7F_47E7_90D7_C2E97237F0E5__INCLUDED_)
