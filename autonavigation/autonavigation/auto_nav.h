// auto_nav.h : main header file for the AUTO_NAV application
//

#if !defined(AFX_AUTO_NAV_H__02222485_3B52_11DA_B910_F80BF543D709__INCLUDED_)
#define AFX_AUTO_NAV_H__02222485_3B52_11DA_B910_F80BF543D709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAuto_navApp:
// See auto_nav.cpp for the implementation of this class
//

class CAuto_navApp : public CWinApp
{
public:
	CAuto_navApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuto_navApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAuto_navApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTO_NAV_H__02222485_3B52_11DA_B910_F80BF543D709__INCLUDED_)
