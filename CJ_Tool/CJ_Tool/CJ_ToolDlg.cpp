
// CJ_ToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CJ_Tool.h"
#include "CJ_ToolDlg.h"
#include "afxdialogex.h"
#include <string>
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCJ_ToolDlg �Ի���



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
	ON_BN_CLICKED(IDC_BUTTON1, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON7, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON9, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CCJ_ToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON11, &CCJ_ToolDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CCJ_ToolDlg ��Ϣ�������
//ͨ����������ȡ���̾��  
HANDLE GetProcessHandleByName(LPCSTR lpName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{
		if (!_tcscmp(pe.szExeFile, lpName))
		{
			CloseHandle(hSnapshot);
			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		}
	}
	return NULL;
}

BOOL CCJ_ToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//m_cb12.AddString("123");
	//m_cb12.AddString("234");
	//m_cb12.AddString("345");
	//m_cb12.AddString("123");
	//m_cb12.AddString("234");
	//m_cb12.AddString("345");
	//m_cb12.AddString("123");
	//m_cb12.AddString("234");
	//m_cb12.AddString("345");
	//m_cb12.AddString("123");
	//m_cb12.AddString("234");
	//m_cb12.AddString("345");
	//m_iCB12 = 3;
	/*
	HANDLE hThread;
	char szLibPath[_MAX_PATH] = "D:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll"; //ָ��ע��֮��Ŀ�����Ҫ���ص�DLL

	DWORD hLibModule;

	HANDLE hProcess = NULL;
	//hProcess = GetProcessHandleByName("HttpDebug.exe");
	hProcess = GetProcessHandleByName("AndroidEmulator.exe");
	DWORD ERRO = GetLastError();

	if (hProcess == NULL)
		return 0;

	//HMODULE modHandle = GetModuleHandle(_T("Kernel32")); //��Ϊkernel32 ÿһ��windows������̿ռ��ж��� ������������LOADLIBRARY��������
	LPVOID pFunc = LoadLibraryA;

	void* pLibRemote = VirtualAllocEx(hProcess, NULL, sizeof(szLibPath), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	ERRO = GetLastError();
	//LPTHREAD_START_ROUTINE addr = (LPTHREAD_START_ROUTINE)GetProcAddress(modHandle, "LoadLibraryA");
	WriteProcessMemory(hProcess, pLibRemote, (void*)szLibPath, sizeof(szLibPath), NULL);

	hThread = CreateRemoteThread(hProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)pFunc,
		pLibRemote,
		0,
		NULL);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
*/
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

DWORD CCJ_ToolDlg::StartInject(HANDLE hProcess, LPTHREAD_START_ROUTINE function, wchar_t * data)
{
	auto buffLen = (wcslen(data) + 1) * sizeof(wchar_t);
	void* remote = ::VirtualAllocEx(hProcess, NULL, buffLen, MEM_COMMIT, PAGE_READWRITE);
	if (remote)
	{
		::WriteProcessMemory(hProcess, remote, data, buffLen, NULL);
		auto hThread = ::CreateRemoteThread(hProcess, NULL, 0, function, remote, 0, NULL);
		::WaitForSingleObject(hThread, INFINITE);
		DWORD exitCode;
		::GetExitCodeThread(hThread, &exitCode);
		::CloseHandle(hThread);
		::VirtualFreeEx(hProcess, remote, 0, MEM_RELEASE);
		return exitCode;
	}
	return 0;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCJ_ToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCJ_ToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCJ_ToolDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCB12 = m_cb12.GetCurSel();


	m_cb12.GetLBText(m_iCB12, m_strCBText);
	TRACE("\r\nm_iCB12 = %d Text='%s'\t", m_iCB12, m_strCBText);
	m_iCB12 = m_cb12.GetCurSel();

	this->SetWindowTextA(m_strCBText);
}


BOOL CCJ_ToolDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CString sArgs = (char *)(pCopyDataStruct->lpData);
	//if (pCopyDataStruct->dwData == 12)
	//{
	//	if(m_cb12.FindStringExact(0, sArgs) == CB_ERR)
	//		m_cb12.AddString(sArgs);
	//} 
	//else if (pCopyDataStruct->dwData == 24)
	//{
	//	if (m_cb24.FindStringExact(0, sArgs) == CB_ERR)
	//		m_cb24.AddString(sArgs);
	//}
		if(m_cb12.FindStringExact(0, sArgs) == CB_ERR)
			m_cb12.AddString(sArgs);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();

	HWND hWnd = ::FindWindowExA(NULL, NULL, "#32770", "��Ȧ���� ������");
	if (hWnd) {
		std::string sendData = "��Ȧ���� ������";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCB24 = m_cb24.GetCurSel();

	CString strCBText;

	m_cb24.GetLBText(m_iCB24, strCBText);
	TRACE("\r\nm_iCB12 = %d Text='%s'\t", m_iCB24, strCBText);
	m_iCB24 = m_cb24.GetCurSel();

	this->SetWindowTextA(strCBText);
}


void CCJ_ToolDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_strCBText = "12_0099";
	CString xxx;
	int id = GetFocus()->GetDlgCtrlID();
	//GetDlgItem(id)->SetWindowText("����ָ�");
	GetDlgItem(id)->GetWindowText(xxx);
	if (m_strCBText.GetLength() >0)
	{
		std::string sCMD = "cmd /C ECHO " + m_strCBText + " >> 0000" + xxx + ".txt";
		WinExec(sCMD.c_str(), SW_HIDE);
	}
	GetDlgItem(IDC_COMBO1)->SetFocus();
}

