//Download by http://www.NewXing.com
// TopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FangDa.h"
#include "TopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTopDlg dialog


CTopDlg::CTopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTopDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTopDlg::~CTopDlg()
{
	DestroyWindow();
}


void CTopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTopDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTopDlg, CDialog)
	//{{AFX_MSG_MAP(CTopDlg)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopDlg message handlers

void CTopDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	::PostMessage(GetParent()->GetSafeHwnd(), WM_CLOSE, 0, 0);
	CDialog::OnCancel();
}

void CTopDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

BOOL CTopDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetLayeredWindowAttributes(this->GetSafeHwnd(), RGB(255,255,254), 120, 3);
	m_br.CreateSolidBrush(RGB(0,0,0));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CTopDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	SetWindowLong(this->m_hWnd,GWL_EXSTYLE,GetWindowLong(this->m_hWnd,GWL_EXSTYLE) | WS_EX_LAYERED);
	return 0;
}

HBRUSH CTopDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd == this)
	{
		return m_br;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTopDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRgn   m_rgn; 
	CRect   rc; 
	GetWindowRect(&rc); 
	rc-=rc.TopLeft(); 
	m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,2,2); 
	SetWindowRgn(m_rgn,true); 
	
}

void CTopDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,174,255));
	dc.TextOut(rc.left+2 , rc.top+1, _T("按Esc，或双击放大镜，或单击此处，退出截图镜！"));

	// Do not call CDialog::OnPaint() for painting messages
}

void CTopDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::PostMessage(GetParent()->GetSafeHwnd(), WM_CLOSE, 0, 0);
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CTopDlg::SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags)
{
	LAYERFUNC SetLayer;
	HMODULE hmod = LoadLibrary("user32.dll");
	SetLayer=(LAYERFUNC)GetProcAddress(hmod,"SetLayeredWindowAttributes");
	BOOL bReturn = SetLayer(hwnd,crKey,bAlpha,dwFlags);
	FreeLibrary(hmod);
	return bReturn;
}
