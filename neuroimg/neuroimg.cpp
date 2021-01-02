// neuroimg.cpp : Defines the entry point for the console application.
//
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
#include <stdio.h>
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		char ch[100];
		mainclass system;
		system.read();
		system.CreateTrainData();
		cout<<"You have "<<system.GetNumbers()<<" elements\n";
		system.Train();
		cout<<"How many internal elements do you want to test\n";
		long n,i;
		long test;
		cin>>n;
		for(i=0;i<n;i++)
		{
			cout<<"Input an element number to test\n";
			cin>>test;
			system.TestInternal(test);
		}
		cout<<"How many external elements do you want to test\n";
		cin>>n;
		fgets(ch,100, stdin);
		for(i=0;i<n;i++)
		{
			cout<<"Input the file ";
			fgets(ch, 100, stdin);
			system.Test(ch);
		}
	}
	return nRetCode;
}


