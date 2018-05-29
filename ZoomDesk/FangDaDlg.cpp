// FangDaDlg.cpp : implementation file
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "FangDa.h"
#include "FangDaDlg.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
static bool bZoom = FALSE;


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
// CFangDaDlg dialog

CFangDaDlg::CFangDaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFangDaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFangDaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nFangDa = 1; // 放大倍数
	m_nSize = 400;
	m_pTop = NULL;
	m_pBufferDC = NULL;
	m_pScreenDC = NULL;
	m_bIsIn = false;
	m_bHot = false;
}

CFangDaDlg::~CFangDaDlg()
{
	if (m_pTop != NULL)
	{
		delete m_pTop;
		m_pTop = NULL;
	}

	if(m_pBufferDC != NULL)
	{
		m_pBufferDC->DeleteDC();
		delete m_pBufferDC;
	}
	m_BufferBitmap.DeleteObject();

	if(m_pScreenDC != NULL)
	{
		m_pScreenDC->DeleteDC();
		delete m_pScreenDC;
	}
	m_ScreenBitmap.DeleteObject();
}

void CFangDaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFangDaDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFangDaDlg, CDialog)
	//{{AFX_MSG_MAP(CFangDaDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MENU_F1, OnMenuF1)
	ON_COMMAND(ID_MENU_F2, OnMenuF2)
	ON_COMMAND(ID_MENU_F3, OnMenuF3)
	ON_COMMAND(ID_MENU_S1, OnMenuS1)
	ON_COMMAND(ID_MENU_S2, OnMenuS2)
	ON_COMMAND(ID_MENU_S3, OnMenuS3)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_WM_SIZE()
	ON_COMMAND(ID_MENU_Z, OnMenuZ)
	ON_WM_CREATE()
	ON_WM_MOVE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFangDaDlg message handlers

BOOL CFangDaDlg::OnInitDialog()
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
	SetTimer(10, 30, NULL);
	m_menu.LoadMenu(IDR_MENU1);
	int nX = (GetSystemMetrics(SM_CXFULLSCREEN)-m_nSize)/2;
	int nY = (GetSystemMetrics(SM_CYFULLSCREEN)-m_nSize)/2;
	SetWindowPos(&wndTopMost, nX, nY, m_nSize, m_nSize, SWP_SHOWWINDOW);

	if (m_pTop == NULL)
	{
		m_pTop = new CTopDlg;
		m_pTop->Create(IDD_DLG_TOP, this);
		m_pTop->SetWindowPos(&wndTopMost, (GetSystemMetrics(SM_CXFULLSCREEN)-338)/2, 0, 338, 20, SWP_SHOWWINDOW);
	}

	CDC * thisDC = GetDC();
	m_pBufferDC	= new CDC;
	m_BufferBitmap.CreateCompatibleBitmap(thisDC, m_nSize, m_nSize);
	m_pBufferDC->CreateCompatibleDC(NULL);
	m_pBufferDC->SelectObject(&m_BufferBitmap);	
	ReleaseDC(thisDC);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFangDaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

HBITMAP DirectBitmap = NULL;
HDC hMemDC = NULL;
UINT * ptPixels = NULL;
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//
void CFangDaDlg::OnPaint() 
{
//	if ()
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
		CPaintDC dc(this); 
		if (m_bIsIn)
		{
			CRect rc;
			GetClientRect(&rc);
			CRect wRc(rc);
			ClientToScreen(&wRc);
			POINT point;
			GetCursorPos(&point);
/*			int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
			int nFullHeight = GetSystemMetrics(SM_CYSCREEN);
			point.x = nFullWidth / 2;
			point.y = nFullHeight / 2;
*/
			HDC	hDeskDC = ::GetDC(::GetDesktopWindow());
			CDC* pDeskDC = CDC::FromHandle(hDeskDC);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			AutoLockByColor(hDeskDC, rc, wRc);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//放大绘制
			m_pBufferDC->StretchBlt(0, 0, rc.Width(), rc.Height(), m_pScreenDC, 
				(int)(point.x-(float)rc.Width()/m_nFangDa/2),
				(int)(point.y-(float)rc.Height()/m_nFangDa/2), 
				rc.Width()/m_nFangDa, 
				rc.Height()/m_nFangDa, SRCCOPY);
			
			CPen pen(PS_SOLID, 6, RGB(0,174,255));
			CPen* oldPen = m_pBufferDC->SelectObject(&pen);
			m_pBufferDC->SelectStockObject(NULL_BRUSH);
			rc.right -= 3;
			rc.bottom -= 3;
			m_pBufferDC->Ellipse(rc);//椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆椭圆
			m_pBufferDC->SelectObject(oldPen);
			

			m_pBufferDC->MoveTo(rc.right / 2 - 20, rc.bottom / 2);
			m_pBufferDC->LineTo(rc.right / 2 + 20, rc.bottom / 2);
			m_pBufferDC->MoveTo(rc.right / 2, rc.bottom / 2 - 20);
			m_pBufferDC->LineTo(rc.right / 2, rc.bottom / 2 + 20);
			dc.BitBlt(0, 0, rc.Width(), rc.Height(), m_pBufferDC, 0, 0, SRCCOPY);
			::ReleaseDC(::GetDesktopWindow(), hDeskDC);

		}else
		{
			CRect rc;
			GetClientRect(&rc);
			CRect wRc(rc);
			ClientToScreen(&wRc);
			POINT point;
			GetCursorPos(&point);
			/*			int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
			int nFullHeight = GetSystemMetrics(SM_CYSCREEN);
			point.x = nFullWidth / 2;
			point.y = nFullHeight / 2;
			*/
			point.x = GetSystemMetrics(SM_CXSCREEN)/2;
			point.y = GetSystemMetrics(SM_CYSCREEN)/2;

			HDC	hDeskDC = ::GetDC(::GetDesktopWindow());
			CDC* pDeskDC = CDC::FromHandle(hDeskDC);

			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			AutoLockByColor(hDeskDC, rc, wRc);
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//放大绘制
			m_pBufferDC->StretchBlt(0, 0, rc.Width(), rc.Height(), pDeskDC, 
				(int)(point.x-(float)rc.Width()/m_nFangDa/2),
				(int)(point.y-(float)rc.Height()/m_nFangDa/2), 
				rc.Width()/m_nFangDa, 
				rc.Height()/m_nFangDa, SRCCOPY);
			
			CPen pen(PS_SOLID, 6, RGB(0,174,255));
			CPen* oldPen = m_pBufferDC->SelectObject(&pen);
			m_pBufferDC->SelectStockObject(NULL_BRUSH);
			rc.right -= 3;
			rc.bottom -= 3;
			m_pBufferDC->Ellipse(rc);
			m_pBufferDC->SelectObject(oldPen);
			
			m_pBufferDC->MoveTo(rc.right/2 - 20, rc.bottom / 2);
			m_pBufferDC->LineTo(rc.right / 2 + 20, rc.bottom / 2);
			m_pBufferDC->MoveTo(rc.right/2, rc.bottom / 2 - 20);
			m_pBufferDC->LineTo(rc.right / 2, rc.bottom / 2 + 20);
			dc.BitBlt(0, 0, rc.Width(), rc.Height(), m_pBufferDC, 0, 0, SRCCOPY);	


			::ReleaseDC(::GetDesktopWindow(), hDeskDC);
		}


		CDialog::OnPaint();
	}
}

void CFangDaDlg::AutoLockByColor(HDC hDeskDC, CRect &rc, CRect &wRc)
{
	// 位图句柄
	HBITMAP    hBitmap, hOldBitmap;
	if (hMemDC == NULL)
	{
		//为屏幕设备描述表创建兼容的内存设备描述表
		/*HDC*/ hMemDC = CreateCompatibleDC(hDeskDC);
	}

	if (DirectBitmap == NULL)
	{
		// 初始化BITMAPINFO信息，以便使用CreateDIBSection
		BITMAPINFO RGB32BitsBITMAPINFO;
		ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
		RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		RGB32BitsBITMAPINFO.bmiHeader.biWidth = rc.Width();
		RGB32BitsBITMAPINFO.bmiHeader.biHeight = rc.Height();
		RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
		RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

		/*HBITMAP*/ DirectBitmap = CreateDIBSection(hMemDC,
			(BITMAPINFO *)&RGB32BitsBITMAPINFO,
			DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);
	}

	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, DirectBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	{
		BitBlt(hMemDC, 0, 0, rc.Width(), rc.Height(),
			hDeskDC, wRc.left, wRc.top, SRCCOPY);
	}
	for (int i = ((rc.Width() * rc.Height()) - 1); i >= 0; i--)
	{
		if (!ptPixels)
		{
			break;
		}
		//ptPixels[i]; //0xff 29 27 21 红绿蓝
		//if (ptPixels[i] == 0xff800000)

		if ( /*(ptPixels[i] == 0xff000080)
			 ||*/(ptPixels[i] == 0xff800000)
			)
		{
			mouse_event(MOUSEEVENTF_MOVE,
				(i % rc.Width() - rc.Width() / 2),
				(i / rc.Width() - rc.Height() / 2),
				0, 0);
			break;
		}
		//	ptPixels[i] = cNewColor;
		//bDoneOnShoot = true;
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFangDaDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFangDaDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CFangDaDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CFangDaDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); 
	CDialog::OnLButtonDown(nFlags, point);
}

void CFangDaDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDblClk(nFlags, point);
	PostMessage(WM_CLOSE, 0, 0);
}

void CFangDaDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CMenu* psub = m_menu.GetSubMenu(0);
	CPoint pTmp(point);
	ClientToScreen(&pTmp);

	if (m_nFangDa == 1)
	{
		m_menu.CheckMenuItem(ID_MENU_F1, MF_BYCOMMAND|MF_CHECKED);
		m_menu.CheckMenuItem(ID_MENU_F2, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_F3, MF_BYCOMMAND|MF_UNCHECKED);
	}else if (m_nFangDa == 2)
	{
		m_menu.CheckMenuItem(ID_MENU_F1, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_F2, MF_BYCOMMAND|MF_CHECKED);
		m_menu.CheckMenuItem(ID_MENU_F3, MF_BYCOMMAND|MF_UNCHECKED);
	}else if (m_nFangDa == 4)
	{
		m_menu.CheckMenuItem(ID_MENU_F1, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_F2, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_F3, MF_BYCOMMAND|MF_CHECKED);
	}else
	{
		m_menu.CheckMenuItem(ID_MENU_F1, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_F2, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_F3, MF_BYCOMMAND|MF_UNCHECKED);
	}

	if (m_nSize == 200)
	{
		m_menu.CheckMenuItem(ID_MENU_S1, MF_BYCOMMAND|MF_CHECKED);
		m_menu.CheckMenuItem(ID_MENU_S2, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_S3, MF_BYCOMMAND|MF_UNCHECKED);
	}else if (m_nSize == 300)
	{
		m_menu.CheckMenuItem(ID_MENU_S1, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_S2, MF_BYCOMMAND|MF_CHECKED);
		m_menu.CheckMenuItem(ID_MENU_S3, MF_BYCOMMAND|MF_UNCHECKED);
	}else if (m_nSize == 400)
	{
		m_menu.CheckMenuItem(ID_MENU_S1, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_S2, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_S3, MF_BYCOMMAND|MF_CHECKED);
	}else
	{
		m_menu.CheckMenuItem(ID_MENU_S1, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_S2, MF_BYCOMMAND|MF_UNCHECKED);
		m_menu.CheckMenuItem(ID_MENU_S3, MF_BYCOMMAND|MF_UNCHECKED);
	}
	psub->TrackPopupMenu(0, pTmp.x, pTmp.y, this, NULL);
	CDialog::OnRButtonUp(nFlags, point);
}

void CFangDaDlg::OnMenuF1() 
{
	m_nFangDa = 1;	
}

void CFangDaDlg::OnMenuF2() 
{
	m_nFangDa = 2;	
}

void CFangDaDlg::OnMenuF3() 
{
	m_nFangDa = 4;	
}

void CFangDaDlg::OnMenuS1() 
{
	m_nSize	= 200;
	CRect rc;
	GetClientRect(&rc);
	CPoint pCenter(rc.Width()/2, rc.Height()/2);
	ClientToScreen(&pCenter);
	SetWindowPos(&wndTopMost, pCenter.x-m_nSize/2, pCenter.y-m_nSize/2, m_nSize, m_nSize, SWP_SHOWWINDOW);

	if(m_pBufferDC != NULL)
	{
		m_pBufferDC->DeleteDC();
		delete m_pBufferDC;
	}
	m_BufferBitmap.DeleteObject();
	CDC * thisDC = GetDC();
	m_pBufferDC	= new CDC;
	m_BufferBitmap.CreateCompatibleBitmap(thisDC, m_nSize, m_nSize);
	m_pBufferDC->CreateCompatibleDC(NULL);
	m_pBufferDC->SelectObject(&m_BufferBitmap);	
	ReleaseDC(thisDC);

	Invalidate(false);
}

void CFangDaDlg::OnMenuS2() 
{
	m_nSize = 300;
	CRect rc;
	GetClientRect(&rc);
	CPoint pCenter(rc.Width()/2, rc.Height()/2);
	ClientToScreen(&pCenter);
	SetWindowPos(&wndTopMost, pCenter.x-m_nSize/2, pCenter.y-m_nSize/2, m_nSize, m_nSize, SWP_SHOWWINDOW);

	if(m_pBufferDC != NULL)
	{
		m_pBufferDC->DeleteDC();
		delete m_pBufferDC;
	}
	m_BufferBitmap.DeleteObject();
	CDC * thisDC = GetDC();
	m_pBufferDC	= new CDC;
	m_BufferBitmap.CreateCompatibleBitmap(thisDC, m_nSize, m_nSize);
	m_pBufferDC->CreateCompatibleDC(NULL);
	m_pBufferDC->SelectObject(&m_BufferBitmap);	
	ReleaseDC(thisDC);


	Invalidate(false);	
}

void CFangDaDlg::OnMenuS3() 
{
	m_nSize = 400;
	CRect rc;
	GetClientRect(&rc);
	CPoint pCenter(rc.Width()/2, rc.Height()/2);
	ClientToScreen(&pCenter);
	SetWindowPos(&wndTopMost, pCenter.x-m_nSize/2, pCenter.y-m_nSize/2, m_nSize, m_nSize, SWP_SHOWWINDOW);

	if(m_pBufferDC != NULL)
	{
		m_pBufferDC->DeleteDC();
		delete m_pBufferDC;
	}
	m_BufferBitmap.DeleteObject();
	CDC * thisDC = GetDC();
	m_pBufferDC	= new CDC;
	m_BufferBitmap.CreateCompatibleBitmap(thisDC, m_nSize, m_nSize);
	m_pBufferDC->CreateCompatibleDC(NULL);
	m_pBufferDC->SelectObject(&m_BufferBitmap);	
	ReleaseDC(thisDC);


	Invalidate(false);
}

void CFangDaDlg::OnMenuExit() 
{
	PostMessage(WM_CLOSE, 0, 0);
}

void CFangDaDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	CRgn m_rgn; 
	CRect rc; 
	GetClientRect(&rc); 
	m_rgn.CreateEllipticRgn(rc.left,rc.top,rc.right,rc.bottom);
	SetWindowRgn(m_rgn,true); 	
}

void CFangDaDlg::OnMenuZ() 
{
	CSetDlg dlg;
	dlg.m_edt1 = m_nFangDa;
	dlg.m_edt2 = m_nSize;
	if (IDOK == dlg.DoModal())
	{
		m_nFangDa = dlg.m_edt1;
		m_nSize = dlg.m_edt2;
		CRect rc;
		GetClientRect(&rc);
		CPoint pCenter(rc.Width()/2, rc.Height()/2);
		ClientToScreen(&pCenter);
		SetWindowPos(&wndTopMost, pCenter.x-m_nSize/2, pCenter.y-m_nSize/2, m_nSize, m_nSize, SWP_SHOWWINDOW);

		if(m_pBufferDC != NULL)
		{
			m_pBufferDC->DeleteDC();
			delete m_pBufferDC;
		}
		m_BufferBitmap.DeleteObject();
		CDC * thisDC = GetDC();
		m_pBufferDC	= new CDC;
		m_BufferBitmap.CreateCompatibleBitmap(thisDC, m_nSize, m_nSize);
		m_pBufferDC->CreateCompatibleDC(NULL);
		m_pBufferDC->SelectObject(&m_BufferBitmap);	
		ReleaseDC(thisDC);


		Invalidate(false);
	}
}

LRESULT CFangDaDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

int CFangDaDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, WS_EX_TOOLWINDOW|WS_EX_STATICEDGE); 
	return 0;
}

void CFangDaDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);	
}

void CFangDaDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 10)
	{
		Invalidate(false);
	}
	if (nIDEvent == 11)
	{
		CPoint p(GetMessagePos());
		ScreenToClient(&p);
		
		CRect rect;
		GetClientRect(rect);
		
		
		if (!rect.PtInRect(p))
		{
			m_bIsIn = false;
			m_bHot = false;
			KillTimer(11);
			Invalidate(false);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CFangDaDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if (!m_bIsIn)        
    {
        m_bIsIn = true;   
        SetTimer(11, 100, NULL);
		if (m_bHot == true)
		{
		 
		}else
		{
			m_bHot = true;
			if(m_pScreenDC != NULL)
			{
				m_pScreenDC->DeleteDC();
				delete m_pScreenDC;
			}
			m_ScreenBitmap.DeleteObject();
			CDC * thisDC = GetDC();
			m_pScreenDC	= new CDC;
			int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
			int nFullHeight = GetSystemMetrics(SM_CYSCREEN);
			m_ScreenBitmap.CreateCompatibleBitmap( thisDC , nFullWidth , nFullHeight);
			m_pScreenDC->CreateCompatibleDC(NULL);
			m_pScreenDC->SelectObject(&m_ScreenBitmap);
			ShowWindow(SW_HIDE);
			HDC	hDeskDC = ::GetDC(::GetDesktopWindow());
			CDC* pDeskDC = CDC::FromHandle(hDeskDC);
			m_pScreenDC->BitBlt(0, 0, nFullWidth, nFullHeight, pDeskDC, 0, 0, SRCCOPY);	
			::ReleaseDC(::GetDesktopWindow(), hDeskDC);
			ReleaseDC(thisDC);
			SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

		 	Invalidate(false);
 		}
    }
	
	CDialog::OnMouseMove(nFlags, point);
}



