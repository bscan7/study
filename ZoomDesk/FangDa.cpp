// FangDa.cpp : Defines the class behaviors for the application.
// Download by http://www.NewXing.com

#include "stdafx.h"
#include "FangDa.h"
#include "FangDaDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using std::string;

/////////////////////////////////////////////////////////////////////////////
// CFangDaApp

BEGIN_MESSAGE_MAP(CFangDaApp, CWinApp)
	//{{AFX_MSG_MAP(CFangDaApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFangDaApp construction

CFangDaApp::CFangDaApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void Thread_regHotKey(LPVOID Para)
{
	//string sFX = "";
	//string sFX_Str = "";
	//for (int i = 1; i < 13; i++)
	//{
	//	sFX = "F" + Common::SP_itoa(i);
	//	sFX_Str = sFX;
	//	if (!RegisterHotKey(NULL, VK_F1 + i - 1, NULL, VK_F1 + i - 1))//注册热键
	//	{
	//		sFX_Str = "CTRL + " + sFX;
	//		if (!RegisterHotKey(NULL, VK_F1 + i - 1, MOD_CONTROL, VK_F1 + i - 1))//注册热键
	//		{
	//			sFX_Str = "CTRL + ALT + " + sFX;
	//			if (!RegisterHotKey(NULL, VK_F1 + i - 1, MOD_CONTROL | MOD_ALT, VK_F1 + i - 1))
	//			{
	//				sFX_Str = "";
	//			}
	//		}
	//	}
	//	Common::g_lstHotKeyList.push_back(sFX_Str);
	//}
	CFangDaDlg* ppp = (CFangDaDlg*)Para;
	RegisterHotKey(NULL, 0x0002, MOD_CONTROL | MOD_SHIFT, 'Z');

	//RegisterHotKey(NULL, 0x0002, MOD_CONTROL | MOD_ALT, 'T');//注册热键
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))//从消息队伍获取消息
	{
		if (msg.message == WM_HOTKEY)//热键消息
		{
			if (msg.wParam == 0x0002)
			{
				ppp->bZoom = !ppp->bZoom;

				{
					ppp->ShowWindow(ppp->bZoom);
				}

				printf("you press ctrl+alt+T hotkey\n");
			}

			//if (msg.wParam == 0x0002) printf("you press ctrl+alt+T hotkey\n");
			//else if (MessageBox(NULL, L"你确定要退出程序吗？", L"热键提示", MB_YESNO | MB_SYSTEMMODAL) == IDYES)
			//{
			//	UnregisterHotKey(NULL, 0x0001);//卸载热键
			//	UnregisterHotKey(NULL, 0x0002);//卸载热键
			//	break;
			//}
		}
	}
	return;
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CFangDaApp object

CFangDaApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFangDaApp initialization

BOOL CFangDaApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CFangDaDlg dlg;
	m_pMainWnd = &dlg;

	_beginthread(Thread_regHotKey, 0, m_pMainWnd);

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
