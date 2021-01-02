// Neuron.h: interface for the CNeuron class.
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

#if !defined(AFX_NEURON_H__14BF8139_5761_4064_9F49_84C7E68C9ADC__INCLUDED_)
#define AFX_NEURON_H__14BF8139_5761_4064_9F49_84C7E68C9ADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdlib.h>
#include <afx.h>
#include <math.h>


class CNeuron  
{
public:
	double Compute(double *x);
	long Training(double *x,double *y1,long nr);
	CNeuron(long n,long seed);
	CNeuron();
	virtual ~CNeuron();
protected:
	long m_N;
	double *m_w;
	double m_theta;
};

#endif // !defined(AFX_NEURON_H__14BF8139_5761_4064_9F49_84C7E68C9ADC__INCLUDED_)
