
// CJ_ToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CJ_Tool.h"
#include "CJ_ToolDlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCJ_ToolDlg 对话框



CCJ_ToolDlg::CCJ_ToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CJ_TOOL_DIALOG, pParent)
	, m_iCB12(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCJ_ToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cb12);
	DDX_CBIndex(pDX, IDC_COMBO1, m_iCB12);
	DDX_Control(pDX, IDC_COMBO2, m_cb24);
}

BEGIN_MESSAGE_MAP(CCJ_ToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CCJ_ToolDlg::OnCbnSelchangeCombo1)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDOK, &CCJ_ToolDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CCJ_ToolDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CCJ_ToolDlg 消息处理程序

BOOL CCJ_ToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_cb12.AddString("123");
	m_cb12.AddString("234");
	m_cb12.AddString("345");
	m_cb12.AddString("123");
	m_cb12.AddString("234");
	m_cb12.AddString("345");
	m_cb12.AddString("123");
	m_cb12.AddString("234");
	m_cb12.AddString("345");
	m_cb12.AddString("123");
	m_cb12.AddString("234");
	m_cb12.AddString("345");
	m_iCB12 = 3;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCJ_ToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCJ_ToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCJ_ToolDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCB12 = m_cb12.GetCurSel();

	CString strCBText;

	m_cb12.GetLBText(m_iCB12, strCBText);
	TRACE("\r\nm_iCB12 = %d Text='%s'\t", m_iCB12, strCBText);
	m_iCB12 = m_cb12.GetCurSel();

	this->SetWindowTextA(strCBText);
}


BOOL CCJ_ToolDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CString sArgs = (char *)(pCopyDataStruct->lpData);
	if (pCopyDataStruct->dwData == 12)
	{
		if(m_cb12.FindStringExact(0, sArgs) == CB_ERR)
			m_cb12.AddString(sArgs);
	} 
	else if (pCopyDataStruct->dwData == 24)
	{
		if (m_cb24.FindStringExact(0, sArgs) == CB_ERR)
			m_cb24.AddString(sArgs);
	}
	//std::vector<std::string> vt = Common::split((sArgs), std::string("--"));
	//for (auto it = vt.begin(); it != vt.end(); it++) {
	//	if ((*it).find("onButton=") != std::string::npos)
	//	{
	//		wstring onBtnFx = Common::trim(Common::SP_a2w((*it).c_str() + (*it).find("onButton=") + strlen("onButton=")));
	//		if (onBtnFx.size() > 0)
	//		{
	//			//WndManager::getInstance()->get_ActivePopupWebWnd()->callJS((L"onButton('" + onBtnFx + L"')").c_str());
	//			if (WndManager::getInstance()->m_mapPopupWebWnd[Common::SP_w2a(onBtnFx)])
	//			{
	//				//WndManager::getInstance()->m_mapPopupWebWnd[Common::SP_w2a(onBtnFx)]->ShowWindow(SW_SHOWNORMAL);
	//				WndManager::getInstance()->m_mapPopupWebWnd[Common::SP_w2a(onBtnFx)]->reLoad(true);

	//			}
	//		}
	//	}
	//}

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

//LRESULT MainWnd::OnCopyData(HWND hWnd, PCOPYDATASTRUCT pCopyDataStruct)
//{
//	zlog_info(ZLOGP, "{");
//
//	return 0;
//}
//


void CCJ_ToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();

	HWND hWnd = ::FindWindowExA(NULL, NULL, "#32770", "懵圈宝宝 哈哈哈");
	if (hWnd) {
		std::string sendData = "懵圈宝宝 哈哈哈";
		/*if (type == USER_ID) {
			sendData = CUserOperate::Instance().getUserId();
		}
		else if (type == NETBAR_ID) {
			sendData = ConfigMan::getInstance()->getNetbarId();
		}
		else if (type == AGENT_ID) {
			sendData = ConfigMan::getInstance()->getAgentId();
		}*/
		COPYDATASTRUCT copyData = { 0 };
		copyData.lpData = (void *)sendData.c_str();
		copyData.cbData = sendData.length() + 1;
		copyData.dwData = 24/*type*/;
		::SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&copyData);
	}

}


void CCJ_ToolDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCB24 = m_cb24.GetCurSel();

	CString strCBText;

	m_cb24.GetLBText(m_iCB24, strCBText);
	TRACE("\r\nm_iCB12 = %d Text='%s'\t", m_iCB24, strCBText);
	m_iCB24 = m_cb24.GetCurSel();

	this->SetWindowTextA(strCBText);
}
