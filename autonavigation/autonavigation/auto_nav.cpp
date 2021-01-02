// auto_nav.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "auto_nav.h"
#include "auto_navDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAuto_navApp

BEGIN_MESSAGE_MAP(CAuto_navApp, CWinApp)
	//{{AFX_MSG_MAP(CAuto_navApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuto_navApp construction

CAuto_navApp::CAuto_navApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAuto_navApp object

CAuto_navApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAuto_navApp initialization

BOOL CAuto_navApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifndef _AFXDLL
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CAuto_navDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
