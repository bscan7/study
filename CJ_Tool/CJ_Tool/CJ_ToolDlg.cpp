
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
	ON_BN_CLICKED(IDC_BUTTON12, &CCJ_ToolDlg::OnBnClickedButton12)
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
using namespace std;

BOOL SaveDcToBMP(BYTE *pBmpBuffer,
	HBITMAP hbitmapSave,
	BITMAPINFO srcdibbmap,
	string sBmpPath)
{
	BOOL        bReturn = TRUE;
	HANDLE    hFile = NULL;  //handle of bitmap file which will be saved  
	DWORD     dwWritten = 0;     //written count  
	DWORD     dwOffsetSize = 0;
	DWORD     dwBufferSize = 0;
	WORD      wBitCount = 32;   //λͼ��ÿ��������ռ�ֽ���    
								//HBITMAP   hbitmapSave = NULL;
								//HBITMAP   hbitmapOld = NULL;
								//HDC       hDcDev = NULL;
	HDC       hDcMem = NULL;
	//BYTE      *pBmpBuffer = NULL;
	int       iBits = 0;
	BITMAP           stSrcbmp;
	//;
	BITMAPFILEHEADER bmFileHeader; //λͼ�ļ�ͷ�ṹ  
								   //SecureZeroMemory(&stSrcbmp, sizeof(BITMAP));
								   ////SecureZeroMemory(&srcdibbmap, sizeof(BITMAPINFO));
	SecureZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	//// Fill bitmap information constructor  
	////srcdibbmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	////srcdibbmap.bmiHeader.biWidth = WVGA_WIDTH;
	////srcdibbmap.bmiHeader.biHeight = WVGA_HIGHT;
	////srcdibbmap.bmiHeader.biPlanes = 1;
	////srcdibbmap.bmiHeader.biBitCount = wBitCount;
	////srcdibbmap.bmiHeader.biCompression = BI_RGB;
	//hDcMem = CreateCompatibleDC(NULL);
	//if (NULL == hDcMem)
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}
	//hbitmapSave = CreateDIBSection(hDcMem, &srcdibbmap, DIB_RGB_COLORS, (void**)&pBmpBuffer, NULL, 0);
	//if (NULL == hbitmapSave)
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}
	//hbitmapOld = (HBITMAP)SelectObject(hDcMem, hbitmapSave);
	//if (NULL == hbitmapOld)
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}
	////����������DC����������ڴ�DC��ȥ  
	//if (!StretchBlt(hDcMem,
	//	0,
	//	0,
	//	WVGA_WIDTH,
	//	WVGA_HIGHT,
	//	m_hdcMem,
	//	0,
	//	0,
	//	m_iScaleWidth,
	//	m_iScaleHight,
	//	SRCCOPY))
	//{
	//	bReturn = FALSE;
	//	goto Exit;
	//}

	if (0 == GetObject(hbitmapSave, sizeof(stSrcbmp), &stSrcbmp))
	{
		bReturn = FALSE;
		goto Exit;
	}
	dwBufferSize = stSrcbmp.bmWidth * stSrcbmp.bmHeight * wBitCount / 8;
	dwOffsetSize = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
	// Fill bitmap header constructor  
	bmFileHeader.bfType = 0x4D42;
	bmFileHeader.bfSize = dwOffsetSize + dwBufferSize;
	bmFileHeader.bfReserved1 = 0;
	bmFileHeader.bfReserved2 = 0;
	bmFileHeader.bfOffBits = dwOffsetSize;

	hFile = CreateFileA(sBmpPath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		bReturn = FALSE;
		goto Exit;
	}
	if (!WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL))
	{
		bReturn = FALSE;
		goto Exit;
	}
	if (!WriteFile(hFile, &srcdibbmap, sizeof(BITMAPINFO), &dwWritten, NULL))
	{
		bReturn = FALSE;
		goto Exit;
	}
	if (!WriteFile(hFile, pBmpBuffer, dwBufferSize, &dwWritten, NULL))
	{
		bReturn = FALSE;
		goto Exit;
	}
	bReturn = TRUE;
Exit:
	if (INVALID_HANDLE_VALUE != hFile)
	{
		CloseHandle(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	return bReturn;
}

HWND g_hWnd = NULL;
RECT g_lpRect;
#define SHOOT_AREA  5
static BOOL bDoneOnShoot = false;

bool IsCenterRed()
//lpRect ����ѡ������
{
	bool bOK = false;
	//return;
	//Helpers::Log("==============AutoShootIfCenter");
	::GetWindowRect(g_hWnd, &g_lpRect);

	RECT lpRect;
	int iW = g_lpRect.right - g_lpRect.left;
	int iH = g_lpRect.bottom - g_lpRect.top;
	int iCenterX = iW / 2 + g_lpRect.left;
	int iCenterY = iH / 2 + g_lpRect.top;

	lpRect.top = iCenterY - SHOOT_AREA;
	lpRect.bottom = iCenterY + SHOOT_AREA;
	lpRect.left = iCenterX - SHOOT_AREA;
	lpRect.right = iCenterX + SHOOT_AREA;

	HDC       hScrDC, hMemDC;
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;
	// λͼ���
	int       nX, nY, nX2, nY2;
	// ѡ����������
	int       nWidth, nHeight;

	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(&lpRect))
		return false;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);


	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect.left;
	nY = lpRect.top;
	nX2 = lpRect.right;
	nY2 = lpRect.bottom;


	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	//if (nX2 > m_xScreen)
	//	nX2 = m_xScreen;
	//if (nY2 > m_yScreen)
	//	nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	//hBitmap = CreateCompatibleBitmap
	//(hScrDC, nWidth, nHeight);

	// ��ʼ��BITMAPINFO��Ϣ���Ա�ʹ��CreateDIBSection
	BITMAPINFO RGB32BitsBITMAPINFO;
	ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
	RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	RGB32BitsBITMAPINFO.bmiHeader.biWidth = nWidth;
	RGB32BitsBITMAPINFO.bmiHeader.biHeight = nHeight;
	RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
	RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;
	UINT * ptPixels;

	HBITMAP DirectBitmap = CreateDIBSection(hMemDC,
		(BITMAPINFO *)&RGB32BitsBITMAPINFO,
		DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);


	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, DirectBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	//if (bSave)
	//{
	//	CDC dcCompatible;
	//	dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
	//	dcCompatible.SelectObject(m_pBitmap);

	//	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
	//		dcCompatible, nX, nY, SRCCOPY);
	//}
	//else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}

	SaveDcToBMP((BYTE *)ptPixels, DirectBitmap, RGB32BitsBITMAPINFO, ".\\000.bmp");
	// ת�� COLORREF Ϊ RGB  
	//cOldColor = COLORREF2RGB(cOldColor);
	//cNewColor = COLORREF2RGB(cNewColor);
	// �滻��ɫ  
	for (int i = ((nWidth * nHeight) - 1); i >= 0; i--)
	{
		if (!ptPixels)
		{
			break;
		}
		//ptPixels[i]; //0xff 29 27 21 ������
		//if (ptPixels[i] == 0xff800000)

		if ( /*(ptPixels[i] == 0xff000080)
			 ||*/(ptPixels[i] == 0xff800000)
			)
		{
			//MyTraceA("+-+-+-+-%x ���������", ptPixels[i]);
			//::OutputDebugStringA("+-+-+-+-��׼��׼��׼��׼");
			//Helpers::Log("==============+-+-+-+- ���������");
			bOK = true;

			break;
		}
		//	ptPixels[i] = cNewColor;
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//�õ���Ļλͼ�ľ��
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	return bOK;
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
	
	HANDLE hThread; 
	char szLibPath[_MAX_PATH] = "E:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll"; //ָ��ע��֮��Ŀ�����Ҫ���ص�DLL

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



void CCJ_ToolDlg::OnBnClickedButton12()
{
	g_hWnd = (HWND)0x00720DB4;
	IsCenterRed();
}
