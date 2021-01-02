// Network3.h: interface for the CNetwork3 class.
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

#if !defined(AFX_NETWORK3_H__0FC8696B_158B_41BF_968B_EE00C6A19637__INCLUDED_)
#define AFX_NETWORK3_H__0FC8696B_158B_41BF_968B_EE00C6A19637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNetwork3  
{
public:
	void print();
	void run(double *X,double *Y);
	bool train(double **X, double **Y, long N,double rho,double errmax,long nrMaxEp,int type,long pos);
	CNetwork3();
	CNetwork3(long n,long p,long m);
	virtual ~CNetwork3();
	long m_nr_epoci;

protected:
	double m_eps;
	double ** m_Wn;
	double ** m_Vn;
	double ** m_W;
	double ** m_V;
	double *m_hh;
	double *m_fh;
	double *m_ho;
	double *m_fo;
	long m_playAgain;
	bool m_gatainvatare;
	long m_N;
	long m_p;
	long m_n;
	long m_m;
};

#endif // !defined(AFX_NETWORK3_H__0FC8696B_158B_41BF_968B_EE00C6A19637__INCLUDED_)
