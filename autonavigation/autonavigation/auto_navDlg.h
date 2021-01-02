// auto_navDlg.h : header file
//

#if !defined(AFX_AUTO_NAVDLG_H__02222487_3B52_11DA_B910_F80BF543D709__INCLUDED_)
#define AFX_AUTO_NAVDLG_H__02222487_3B52_11DA_B910_F80BF543D709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAuto_navDlg dialog
#include "TMemory.h"
#include "rbf.h"

class CAuto_navDlg : public CDialog
{
// Construction
public:
	CAuto_navDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAuto_navDlg)
	enum { IDD = IDD_AUTO_NAV_DIALOG };
	CString	m_FileName;
	UINT	m_Type;
	UINT	m_vtype;
	UINT	m_NrHidden;
	UINT	m_NrInput;
	UINT	m_NrOutput;
	double	m_eta;
	double	m_error;
	double	m_alpha;
	CString	m_RFileName;
	UINT	m_pdirection;
	CString	m_NFileName;
	CString	m_TMessage;
	long	m_NrSteps;
	CString	m_TFileName;
	double	m_v1;
	double	m_v2;
	double	m_v3;
	double	m_v4;
	double	m_v5;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuto_navDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAuto_navDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAdd();
	afx_msg void OnLoad();
	afx_msg void OnSave();
	afx_msg void OnType();
	afx_msg void OnSetFileName();
	afx_msg void OnView();
	afx_msg void OnReset();
	afx_msg void OnTraining();
	afx_msg void OnRun();
	afx_msg void OnCreateNN();
	afx_msg void OnNLoad();
	afx_msg void OnNSave();
	afx_msg void OnDelete();
	afx_msg void OnNNDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CTMemory m_mem;
	Crbf m_rbf;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTO_NAVDLG_H__02222487_3B52_11DA_B910_F80BF543D709__INCLUDED_)
