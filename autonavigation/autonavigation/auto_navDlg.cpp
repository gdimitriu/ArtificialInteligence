// auto_navDlg.cpp : implementation file
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
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuto_navDlg dialog

CAuto_navDlg::CAuto_navDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAuto_navDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAuto_navDlg)
	m_FileName = _T("");
	m_Type = 0;
	m_vtype = 0;
	m_NrHidden = 0;
	m_NrInput = 0;
	m_NrOutput = 0;
	m_eta = 0.0;
	m_error = 0.0;
	m_alpha = 0.0;
	m_RFileName = _T("");
	m_pdirection = 0;
	m_NFileName = _T("");
	m_TMessage = _T("");
	m_NrSteps = 0;
	m_TFileName = _T("");
	m_v1 = 0.0;
	m_v2 = 0.0;
	m_v3 = 0.0;
	m_v4 = 0.0;
	m_v5 = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAuto_navDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAuto_navDlg)
	DDX_Text(pDX, IDC_FileName_str, m_FileName);
	DDX_Text(pDX, IDC_Type_str, m_Type);
	DDV_MinMaxUInt(pDX, m_Type, 0, 5);
	DDX_Text(pDX, IDC_vtype, m_vtype);
	DDV_MinMaxUInt(pDX, m_vtype, 0, 5);
	DDX_Text(pDX, IDC_NrHidden, m_NrHidden);
	DDX_Text(pDX, IDC_NrIN, m_NrInput);
	DDX_Text(pDX, IDC_NrOUT, m_NrOutput);
	DDX_Text(pDX, IDC_eta, m_eta);
	DDX_Text(pDX, IDC_Error, m_error);
	DDX_Text(pDX, IDC_alpha, m_alpha);
	DDX_Text(pDX, IDC_RFileName, m_RFileName);
	DDX_Text(pDX, IDC_pdirection, m_pdirection);
	DDV_MinMaxUInt(pDX, m_pdirection, 0, 5);
	DDX_Text(pDX, IDC_NFileName, m_NFileName);
	DDX_Text(pDX, IDC_TMessage, m_TMessage);
	DDX_Text(pDX, IDC_NrSteps, m_NrSteps);
	DDX_Text(pDX, IDC_Add_Training_str, m_TFileName);
	DDX_Text(pDX, IDC_EDIT1, m_v1);
	DDX_Text(pDX, IDC_EDIT2, m_v2);
	DDX_Text(pDX, IDC_EDIT3, m_v3);
	DDX_Text(pDX, IDC_EDIT4, m_v4);
	DDX_Text(pDX, IDC_EDIT5, m_v5);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAuto_navDlg, CDialog)
	//{{AFX_MSG_MAP(CAuto_navDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Add, OnAdd)
	ON_BN_CLICKED(IDC_Load, OnLoad)
	ON_BN_CLICKED(IDC_Save, OnSave)
	ON_BN_CLICKED(IDC_Type, OnType)
	ON_BN_CLICKED(IDC_SetFileName, OnSetFileName)
	ON_BN_CLICKED(IDC_View, OnView)
	ON_BN_CLICKED(IDC_Reset, OnReset)
	ON_BN_CLICKED(IDC_Training, OnTraining)
	ON_BN_CLICKED(IDC_Run, OnRun)
	ON_BN_CLICKED(IDC_CreateNN, OnCreateNN)
	ON_BN_CLICKED(IDC_NLoad, OnNLoad)
	ON_BN_CLICKED(IDC_NSave, OnNSave)
	ON_BN_CLICKED(IDC_Delete, OnDelete)
	ON_BN_CLICKED(IDC_NNDelete, OnNNDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuto_navDlg message handlers

BOOL CAuto_navDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAuto_navDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAuto_navDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAuto_navDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAuto_navDlg::OnAdd() 
{
	UpdateData(TRUE);
	unsigned long **data;
	unsigned int type;
	LPTSTR fn;
	fn=m_TFileName.GetBuffer(100);
	m_mem.Put(fn);
	m_TFileName.ReleaseBuffer();
	m_mem.Get(&data,&type);
	CClientDC dc(this); // device context for painting
	unsigned int i,j,l,k;
	for(i=0;i<32;i++)
		for(l=0;l<8;l++)
			for(j=0;j<32;j++)
				for(k=0;k<8;k++)
					dc.SetPixel(j*8+20+k,i*8+100+l,data[i][j]);
}	


void CAuto_navDlg::OnLoad() 
{
	UpdateData(TRUE);
	LPTSTR fn;
	fn=m_FileName.GetBuffer(100);
	m_mem.SetFileName(fn);
	m_FileName.ReleaseBuffer();
	m_mem.ReadData();
}

void CAuto_navDlg::OnSave() 
{
	UpdateData(TRUE);
	LPTSTR fn;
	fn=m_FileName.GetBuffer(100);
	m_mem.SetFileName(fn);
	m_FileName.ReleaseBuffer();
	m_mem.WriteData();
}

void CAuto_navDlg::OnType() 
{
	UpdateData(TRUE);
	m_mem.SetType(m_Type);
}

void CAuto_navDlg::OnSetFileName() 
{
	UpdateData(TRUE);
	LPTSTR fn;
	fn=m_FileName.GetBuffer(100);
	m_mem.SetFileName(fn);
	m_FileName.ReleaseBuffer();
}

void CAuto_navDlg::OnView() 
{
	unsigned int i,j,k,l;
	unsigned long **data;
	unsigned int type;
	bool run;
	CClientDC dc(this); // device context for painting
	run=m_mem.Get(&data,&type);
	if(run==true)
	{
		for(i=0;i<32;i++)
			for(l=0;l<8;l++)
				for(j=0;j<32;j++)
					for(k=0;k<8;k++)
						dc.SetPixel(j*8+20+k,i*8+100+l,data[i][j]);
		m_vtype=type;
		UpdateData(FALSE);
	}

}

void CAuto_navDlg::OnReset() 
{
	m_mem.Reset();
}

void CAuto_navDlg::OnTraining() 
{
	UpdateData(TRUE);
	m_TMessage.Empty();
	ASSERT( m_TMessage.GetLength( ) == 0 );
	m_TMessage.Insert(0,"");
	UpdateData(FALSE);
	m_rbf.SetMemory(&m_mem);
	m_rbf.InitT(m_alpha,m_eta, m_error);
	if(m_rbf.Training(m_NrSteps)==false) 
		m_TMessage.Insert(0,"Unsuccesfull training");
	else
		m_TMessage.Insert(0,"Succesfull training");
	UpdateData(FALSE);
}

void CAuto_navDlg::OnRun() 
{
	UpdateData(TRUE);
	LPTSTR fn;
	fn=m_RFileName.GetBuffer(100);
	CRWBitmap m_bitmap;
	m_bitmap.SetIFileName(fn);
	m_RFileName.ReleaseBuffer();
	m_bitmap.ReadFH();
	m_bitmap.ReadData();
	unsigned long i,j;
	CClientDC dc(this); // device context for painting
	for(i=0;i<256;i++)
		for(j=0;j<256;j++)
			dc.SetPixel(j+20,i+100,m_bitmap.data[i][j]);
	unsigned long **data;
	unsigned long *ptemp;
	data=(unsigned long **)calloc(32,sizeof(unsigned long *));
	ptemp=(unsigned long *)calloc(32*32,sizeof(unsigned long));
	for(i=0;i<32;i++)
	{
		data[i]=ptemp;
		ptemp+=32;
	}
	
	//average window
	double temp;
	unsigned long ltemp,k,l;
	for(i=0;i<32;i++)
	{
		for(j=0;j<32;j++)
		{
			temp=0.0;
			for(k=0;k<8;k++)
				for(l=0;l<8;l++)
					temp+=m_bitmap.data[i*8+k][j*8+l]>>16;
			temp=temp/64;
			ltemp=temp;
			data[i][j]=(ltemp<<24)>>8;
		}
	}
	m_pdirection=m_rbf.Run(&data[0][0]);
	m_v1=m_rbf.m_y[0];
	m_v2=m_rbf.m_y[1];
	m_v3=m_rbf.m_y[2];
	if(m_NrOutput==5)
	{
		m_v4=m_rbf.m_y[3];
		m_v5=m_rbf.m_y[4];
	}
	UpdateData(FALSE);
//	free(*data);
//	free(data);
}

void CAuto_navDlg::OnCreateNN() 
{
	UpdateData(TRUE);
	m_rbf.Create(m_NrInput,m_NrHidden,m_NrOutput);
}

void CAuto_navDlg::OnNNDelete() 
{
	m_rbf.Delete();	
}

void CAuto_navDlg::OnNLoad() 
{
	UpdateData(TRUE);
	LPTSTR fn;
	fn=m_NFileName.GetBuffer(100);
	m_rbf.SetFileName(fn);
	m_NFileName.ReleaseBuffer();
	m_rbf.Load();
}

void CAuto_navDlg::OnNSave() 
{
	UpdateData(TRUE);
	LPTSTR fn;
	fn=m_NFileName.GetBuffer(100);
	m_rbf.SetFileName(fn);
	m_NFileName.ReleaseBuffer();
	m_rbf.Save();
}

void CAuto_navDlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	
}

