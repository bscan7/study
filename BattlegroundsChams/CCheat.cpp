#include "stdafx.h"
#include "CCheat.h"
#include "Helpers.h"
#include "Hooks.h"
#include <process.h>
#include <atltypes.h>
//#include "mainxxx.h"

ID3D11Device *CCheat::pDevice = NULL;
ID3D11DeviceContext *CCheat::pContext = NULL;
IDXGISwapChain* CCheat::pSwapChain = NULL; 
HWND g_hWnd = NULL;
RECT g_lpRect;
bool bCrossDraw = true;

HANDLE  g_Event_Shoot = CreateEvent(NULL, FALSE, FALSE, NULL);
bool bStoped = false;

void AutoShootIfCenter(PVOID param);
	HDC       hScrDC = NULL; ;
	HDC       hMemDC = NULL; ;
	HBITMAP DirectBitmap = NULL; ;
	UINT * ptPixels = NULL;

void Thread_DrawCrossOnCenter(PVOID param)
{
	RECT lpRect;
	while (1)
	{
		Sleep(100);

		if (!bCrossDraw)
		{
			continue;
		}
		::GetWindowRect(g_hWnd, &lpRect);
		/*			int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
		int nFullHeight = GetSystemMetrics(SM_CYSCREEN);
		point.x = nFullWidth / 2;
		point.y = nFullHeight / 2;
		*/

		//HDC	hClientDC = ::GetDC(g_hWnd);

		//为屏幕创建设备描述表
		if (!hScrDC)
		{
			hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
		}
		//HDC hdc; ;
		PAINTSTRUCT ps;
		HPEN hPen;
		HPEN hPenOld;
		//hdc = BeginPaint(hWnd, &ps);
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		hPenOld = (HPEN)SelectObject(hScrDC, hPen);

		//EndPaint(hWnd, &ps);

		MoveToEx(hScrDC, lpRect.left, (lpRect.bottom - lpRect.top)/ 2 + lpRect.top, NULL);
		LineTo(hScrDC, lpRect.right, (lpRect.bottom - lpRect.top) / 2 + lpRect.top);
		MoveToEx(hScrDC, lpRect.left + (lpRect.right - lpRect.left) / 2, lpRect.top,  NULL);
		LineTo(hScrDC, lpRect.left + (lpRect.right - lpRect.left) / 2, lpRect.bottom);

		SelectObject(hScrDC, hPenOld);
		DeleteObject(hPen);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hPenOld = (HPEN)SelectObject(hScrDC, hPen);

		//EndPaint(hWnd, &ps);

		MoveToEx(hScrDC, lpRect.left, (lpRect.bottom - lpRect.top)/ 2 + lpRect.top + 1, NULL);
		LineTo(hScrDC, lpRect.right, (lpRect.bottom - lpRect.top) / 2 + lpRect.top + 1);
		MoveToEx(hScrDC, lpRect.left + (lpRect.right - lpRect.left) / 2 + 1, lpRect.top,  NULL);
		LineTo(hScrDC, lpRect.left + (lpRect.right - lpRect.left) / 2 + 1, lpRect.bottom);

		SelectObject(hScrDC, hPenOld);
		DeleteObject(hPen);
	}
}
#define SHOOT_AREA  5
int pp = 0;

void AutoShootIfCenter(PVOID param)
{
	RECT lpRect, lpShootRect;
	//while (1)
	//{
		//Sleep(100);

		//if (!bCrossDraw)
		//{
		//	continue;
		//}
		::GetWindowRect(g_hWnd, &lpRect);
		/*			int nFullWidth = GetSystemMetrics(SM_CXSCREEN);
		int nFullHeight = GetSystemMetrics(SM_CYSCREEN);
		point.x = nFullWidth / 2;
		point.y = nFullHeight / 2;
		*/

		//HDC	hClientDC = ::GetDC(g_hWnd);

		//为屏幕创建设备描述表
		if (!hScrDC)
		{
			hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
		}
		//为屏幕设备描述表创建兼容的内存设备描述表
		if (!hMemDC)
		{
			hMemDC = CreateCompatibleDC(hScrDC);
		}

		int iW = lpRect.right - lpRect.left;
		int iH = lpRect.bottom - lpRect.top;
		int iCenterX = iW / 2 + lpRect.left;
		int iCenterY = iH / 2 + lpRect.top;

		lpShootRect.top = iCenterY - SHOOT_AREA;
		lpShootRect.bottom = iCenterY + SHOOT_AREA;
		lpShootRect.left = iCenterX - SHOOT_AREA;
		lpShootRect.right = iCenterX + SHOOT_AREA;
		// 确保选定区域不为空矩形
		if (IsRectEmpty(&lpShootRect))
			return /*NULL*/;

		HPEN hPen;
		HPEN hPenOld;
		//hdc = BeginPaint(hWnd, &ps);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 174, 0));
		hPenOld = (HPEN)SelectObject(hScrDC, hPen);

		MoveToEx(hScrDC, lpShootRect.left, lpShootRect.top, NULL);
		LineTo(hScrDC, lpShootRect.right, lpShootRect.top);
		MoveToEx(hScrDC, lpShootRect.left, lpShootRect.bottom, NULL);
		LineTo(hScrDC, lpShootRect.right, lpShootRect.bottom);

		MoveToEx(hScrDC, lpShootRect.left, lpShootRect.top, NULL);
		LineTo(hScrDC, lpShootRect.left, lpShootRect.bottom);
		MoveToEx(hScrDC, lpShootRect.right, lpShootRect.top, NULL);
		LineTo(hScrDC, lpShootRect.right, lpShootRect.bottom);

		SelectObject(hScrDC, hPenOld);
		DeleteObject(hPen);

		// 初始化BITMAPINFO信息，以便使用CreateDIBSection
		BITMAPINFO RGB32BitsBITMAPINFO;
		ZeroMemory(&RGB32BitsBITMAPINFO, sizeof(BITMAPINFO));
		RGB32BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		RGB32BitsBITMAPINFO.bmiHeader.biWidth = SHOOT_AREA * 2;
		RGB32BitsBITMAPINFO.bmiHeader.biHeight = SHOOT_AREA * 2;
		RGB32BitsBITMAPINFO.bmiHeader.biPlanes = 1;
		RGB32BitsBITMAPINFO.bmiHeader.biBitCount = 32;

		if (!DirectBitmap)
		{
			DirectBitmap = CreateDIBSection(hMemDC,
				(BITMAPINFO *)&RGB32BitsBITMAPINFO,
				DIB_RGB_COLORS, (void **)&ptPixels, NULL, 0);
		}
		HBITMAP    hBitmap, hOldBitmap;
		// 把新位图选到内存设备描述表中
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, DirectBitmap);

		BitBlt(hMemDC, 0, 0, SHOOT_AREA * 2, SHOOT_AREA * 2,
			hScrDC, lpShootRect.left, lpShootRect.top, SRCCOPY);

		std::cout << "==============i" <<  "==============idx=" << pp << std::endl;
		// 替换颜色  
		for (int i = ((SHOOT_AREA * 2 * SHOOT_AREA * 2) - 1); i >= 0; i--)
		{
			if (!ptPixels)
			{
				std::cout << "!!!!!!!!!!!!!!!!!!+-+-+-+- NULL i=" << pp << std::endl;
				break;
			}
			std::cout << ptPixels[i] << " ";
			//ptPixels[i]; //0xff 29 27 21 红绿蓝
			if (   (ptPixels[i] == 0xff000080)
				|| (ptPixels[i] == 0xff800000)
				|| (ptPixels[i] == 0xff734c19)
				)
			{
				//MyTraceA("+-+-+-+-%x 射击射击射击", ptPixels[i]);
				//::OutputDebugStringA("+-+-+-+-瞄准瞄准瞄准瞄准");
				std::cout << "==============+-+-+-+- 射击射击射击 i=" << i << " idx=" << pp << std::endl;

				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				Sleep(10); 
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				//bDoneOnShoot = false;
				break;
			}
			//bDoneOnShoot = true;
		}
		std::cout << std::endl;
		pp++;
		hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);

	//}
}

void Thread_AutoShootIfCenter(PVOID param)
{
	while (!bStoped)
	{
		DWORD res = WaitForSingleObject(g_Event_Shoot, 500);
		//MyTraceA("/////////////////////////////////////////////////////////2");

		//if (!c.socket_.is_open())
		//{
		//	break;
		//}
		switch (res)
		{
		case WAIT_OBJECT_0:
			AutoShootIfCenter(NULL);

			break;
		case WAIT_TIMEOUT:
			break;
		case WAIT_ABANDONED:
			break;
		case WAIT_FAILED:
			printf("\nOL_ The function has failed. To get extended error information, callGetLastError.\n");
			break;
		default:
			break;

		}
	}
	bStoped = false;
}

void Thread_ExitHook(PVOID param)
{
	CCheat::Release();
}

HWND _EnumChildWindows(HWND hParent, char* pCap)
{
	HWND hWndChild = GetWindow(hParent, GW_CHILD);
	HWND hWndTemp = GetWindow(hWndChild, GW_HWNDFIRST);
	do {
		char szWndInfo[512] = { 0 };
		char szWndTitle[256] = { 0 };
		char szClsName[64] = { 0 };

		GetWindowTextA(hWndTemp, szWndTitle, 256);
		GetClassNameA(hWndTemp, szClsName, 64);
		if (strstr(szWndTitle, pCap))
		{
			return hWndTemp;
		}
		//_stprintf(szWndInfo,
		//	_T("´°¿Ú%08X ")
		//	_T("\"%s\"")
		//	_T("%s ")
		//	_T("%s"),
		//	hWndTemp,
		//	szWndTitle,
		//	szClsName,
		//	IsWindowVisible(hWndTemp) ? _T("Visible") : _T("Invisible"));
		//SendMessage(hDisplay, LB_ADDSTRING, 0, (LPARAM)szWndInfo);
	} while (NULL != (hWndTemp = GetWindow(hWndTemp, GW_HWNDNEXT)));

	return NULL;
}

int HotKeyId;

void InitForHook(IDXGISwapChain* pSwapChain);

void CCheat::Initialise()
{
	/*HotKeyId = GlobalAddAtom("D3DHotKey") - 0xC000;
	RegisterHotKey(Handle, HotKeyId, MOD_CONTROL, 'X');*/

	HRESULT xxx;
	AllocConsole();
	freopen("CON", "w", stdout);
	SetConsoleTitle(L"Bscan_LookLook");

	Helpers::Log("\r\nCheat Initialising");

	//MessageBoxA(NULL, "如果需要请附加进程先，再点确定!", "uBoos?", MB_ICONINFORMATION);

	//D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0xb100;

	//HWND hWnd = GetForegroundWindow();//FindWindowA("UnrealWindow", NULL);
	//HWND hWnd = FindWindowA(NULL, "engine");
	while (!g_hWnd)
	{
		g_hWnd = FindWindowA(NULL, "Lesson 4 - Begin Drawing");
		if (!g_hWnd)
		{
			g_hWnd = FindWindowA(NULL, "BlueStacks App Player");
			if (!g_hWnd)
			{
				//MessageBoxA(NULL, "Not found HWND 夜神模拟器!", "uBoos?", MB_ICONINFORMATION);
				//return;
			}
			//else
			//{
			//	g_hWnd = _EnumChildWindows(g_hWnd, "ScreenBoardClassWindow");
			//}
			//HWND hWnd = (HWND)0x0031162C;
		}
		if (!g_hWnd)
		{
			g_hWnd = FindWindowA(NULL, "夜神模拟器");
			if (!g_hWnd)
			{
				//MessageBoxA(NULL, "Not found HWND 夜神模拟器!", "uBoos?", MB_ICONINFORMATION);
				//return;
			}
			else
			{
				g_hWnd = _EnumChildWindows(g_hWnd, "ScreenBoardClassWindow");
			}
			//HWND hWnd = (HWND)0x0031162C;
		}
		if (!g_hWnd)
		{
			g_hWnd = FindWindowA(NULL, "雷电模拟器");
			if (!g_hWnd)
			{
				//MessageBoxA(NULL, "Not found HWND 雷电模拟器!", "uBoos?", MB_ICONINFORMATION);
				//return;
			}
			else
			{
				g_hWnd = _EnumChildWindows(g_hWnd, "TheRender");
			}
		}
		if (!g_hWnd)
		{
			g_hWnd = FindWindowA(NULL, "绝地求生 刺激战场 - MuMu模拟器");
			if (!g_hWnd)
			{
				//MessageBoxA(NULL, "Not found HWND MuMu模拟器!", "uBoos?", MB_ICONINFORMATION);
				//return;
			}
			else
			{
				g_hWnd = _EnumChildWindows(g_hWnd, "");
			}
		}
		if (!g_hWnd)
		{
			g_hWnd = FindWindowA(NULL, "绝地求生 全军出击 - MuMu模拟器");
			if (!g_hWnd)
			{
				//MessageBoxA(NULL, "Not found HWND MuMu模拟器!", "uBoos?", MB_ICONINFORMATION);
				//return;
			}
			else
			{
				g_hWnd = _EnumChildWindows(g_hWnd, "");
			}
		}

		if (!g_hWnd)
		{
			g_hWnd = FindWindowA(NULL, "腾讯手游助手【极速傲引擎】");
			if (!g_hWnd)
			{
				//MessageBoxA(NULL, "Not found HWND MuMu模拟器!", "uBoos?", MB_ICONINFORMATION);
				//return;
			}
			else
			{
				g_hWnd = _EnumChildWindows(g_hWnd, "AEngineRenderWindow");
			}
		}

		//g_hWnd = (HWND)0x001509A8;
		if (!g_hWnd)
		{
			//MessageBoxA(NULL, "Not found child HWND!", "uBoos?", MB_ICONINFORMATION);
			Helpers::Log("没找到游戏主窗口！！！！！");

			//return;
		}

		Sleep(1000);
	}
	::GetWindowRect(g_hWnd, &g_lpRect);

	_beginthread(Thread_AutoShootIfCenter, 0, NULL);
	_beginthread(Thread_DrawCrossOnCenter, 0, NULL);

#pragma region Initialise DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;
	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = g_hWnd;
	scd.SampleDesc.Count = 1; // Set to 1 to disable multisampling
	scd.SampleDesc.Quality = 0;
	scd.Windowed = ((GetWindowLongPtr(g_hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//不设置标志
	scd.Flags = 0;
	// 设置feature level为D3D11
	// 如果显卡不支持D3D11,我们能够通过设置这个参数，使用D3D10,或者9.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

#pragma endregion
	//DXGI_ERROR_INVALID_CALL;
	//xxx = D3D11CreateDeviceAndSwapChain(
	//	NULL, D3D_DRIVER_TYPE_REFERENCE, NULL,
	//	NULL, &featureLevel, 1, D3D11_SDK_VERSION,
	//	&scd, &CCheat::pSwapChain,
	//	&CCheat::pDevice, NULL, &CCheat::pContext
	//);


	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		NULL, &featureLevel, 1, D3D11_SDK_VERSION,
		&scd, &CCheat::pSwapChain,
		&CCheat::pDevice, NULL, &CCheat::pContext
	)))
	{
		Helpers::LogError("D3D11CreateDevice Error Error Error*****************************");
		return;
	}

	// GET VTABLE POINTERS
	DWORD_PTR*  pSwapChainVT = reinterpret_cast<DWORD_PTR*>(CCheat::pSwapChain);
	DWORD_PTR*  pDeviceVT = reinterpret_cast<DWORD_PTR*>(CCheat::pDevice);
	DWORD_PTR*  pContextVT = reinterpret_cast<DWORD_PTR*>(CCheat::pContext);

	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);
	pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);

	Helpers::LogAddress("pSwapChainVT", reinterpret_cast<int64_t>(pSwapChainVT));
	Helpers::LogAddress("D3D11PRESENT", pSwapChainVT[8]);

	Helpers::LogAddress("pDeviceVT", reinterpret_cast<int64_t>(pDeviceVT));
	Helpers::LogAddress("D3D11CreateQuery", pDeviceVT[24]);

	Helpers::LogAddress("pContextVT", reinterpret_cast<int64_t>(pContextVT));

	Helpers::LogAddress("tD3D11VSSetConstantBuffers", pContextVT[7]);
	Helpers::LogAddress("-=", pContextVT[7]- reinterpret_cast<int64_t>(pContextVT));
	Helpers::LogAddress("tD3D11DrawIndexed", pContextVT[12]);
	Helpers::LogAddress("-=", pContextVT[12] - pContextVT[7]);
	Helpers::LogAddress("tD3D11DrawIndexedInstanced", pContextVT[20]);
	Helpers::LogAddress("-=", pContextVT[20] - pContextVT[12]);
	Helpers::LogAddress("tD3D11DrawInstanced", pContextVT[21]);
	Helpers::LogAddress("-=", pContextVT[21] - pContextVT[20]);
	Helpers::LogAddress("tD3D11DrawIndexedInstancedIndirect", pContextVT[39]);
	Helpers::LogAddress("-=", pContextVT[39] - pContextVT[21]);
	Helpers::LogAddress("tD3D11DrawInstancedIndirect", pContextVT[40]);
	Helpers::LogAddress("-=", pContextVT[40] - pContextVT[39]);

	// GET FUNCTIONS
	Hooks::oPresent = reinterpret_cast<tD3D11Present>(pSwapChainVT[8]/*D3D11PRESENT*/);
	Hooks::oDrawIndexed = reinterpret_cast<tD3D11DrawIndexed>(pContextVT[12]/*D3D11DRAWINDEXED*/);
	Hooks::oCreateQuery = reinterpret_cast<tD3D11CreateQuery>(pDeviceVT[24]/*D3D11CreateQuery*/);
	Hooks::oVSSetConstantBuffers = reinterpret_cast<tD3D11VSSetConstantBuffers>(pContextVT[7]/*D3D11VSSetConstantBuffers*/);
	Hooks::oPSSetShaderResources = reinterpret_cast<tD3D11PSSetShaderResources>(pContextVT[8]/*D3D11PSSetShaderResources*/);
	Hooks::oPSSetSamplers = reinterpret_cast<tD3D11PSSetSamplers>(pContextVT[10]/*D3D11PSSetSamplers*/);
	Hooks::oUpdateSubresource = reinterpret_cast<tD3D11UpdateSubresource>(pContextVT[48]/*D3D11UpdateSubresource*/);

	Hooks::oDrawIndexedInstanced = reinterpret_cast<tD3D11DrawIndexedInstanced>(pContextVT[20]/**/);
	Hooks::oDrawInstanced = reinterpret_cast<tD3D11DrawInstanced>(pContextVT[21]/**/);
	Hooks::oDrawIndexedInstancedIndirect = reinterpret_cast<tD3D11DrawIndexedInstancedIndirect>(pContextVT[39]/**/);
	Hooks::oDrawInstancedIndirect = reinterpret_cast<tD3D11DrawInstancedIndirect>(pContextVT[40]/**/);

	// LETS HOOK IT
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oPresent),						Hooks::hkD3D11Present);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oCreateQuery),					Hooks::hkD3D11CreateQuery);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oVSSetConstantBuffers),			Hooks::hkD3D11VSSetConstantBuffers);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oPSSetShaderResources),			Hooks::hkD3D11PSSetShaderResources);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oPSSetSamplers),					Hooks::hkD3D11PSSetSamplers);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oUpdateSubresource),				Hooks::hkD3D11UpdateSubresource);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexed),					Hooks::hkD3D11DrawIndexed);

	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawInstanced),					Hooks::hkD3D11DrawInstanced);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexedInstanced),			Hooks::hkD3D11DrawIndexedInstanced);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawInstancedIndirect),			Hooks::hkD3D11DrawInstancedIndirect);
	Helpers::HookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexedInstancedIndirect),	Hooks::hkD3D11DrawIndexedInstancedIndirect);

	//InitForHook(CCheat::pSwapChain);
	Helpers::Log("=========================CCheat::Initialise() Done!!!===============================");
}

void CCheat::Release()
{
	Helpers::Log("DLL抽离主进程。。。");
	Sleep(300);
	//bStoped = true;
	//while (bStoped)
	//{
	//}
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	//FreeConsole();

	// UNHOOK
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oPresent), Hooks::hkD3D11Present);
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oCreateQuery), Hooks::hkD3D11CreateQuery);
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oVSSetConstantBuffers), Hooks::hkD3D11VSSetConstantBuffers);
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexed), Hooks::hkD3D11DrawIndexed);

	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawInstanced), Hooks::hkD3D11DrawInstanced);
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexedInstanced), Hooks::hkD3D11DrawIndexedInstanced);
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawInstancedIndirect), Hooks::hkD3D11DrawInstancedIndirect);
	Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexedInstancedIndirect), Hooks::hkD3D11DrawIndexedInstancedIndirect);

	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oPresent), Hooks::hkD3D11Present);
	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oCreateQuery), Hooks::hkD3D11CreateQuery);
	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oVSSetConstantBuffers), Hooks::hkD3D11VSSetConstantBuffers);
	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexed), Hooks::hkD3D11DrawIndexed);

	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawInstanced), Hooks::hkD3D11DrawInstanced);
	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexedInstanced), Hooks::hkD3D11DrawIndexedInstanced);
	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawInstancedIndirect), Hooks::hkD3D11DrawInstancedIndirect);
	//Helpers::UnhookFunction(reinterpret_cast<PVOID*>(&Hooks::oDrawIndexedInstancedIndirect), Hooks::hkD3D11DrawIndexedInstancedIndirect);
	Helpers::Log("DLL抽离主进程。。。Done");
}


/*
PASTED FROM D3D11HOOK ON GITHUB SOMEWHERE
dx11 vtable CONTEXT index
Index: 0 | QueryInterface
Index: 1 | Addref
Index: 2 | Release
Index: 3 | GetDevice
Index: 4 | GetPrivateData
Index: 5 | SetPrivateData
Index: 6 | SetPrivateDataInterface
Index: 7 | VSSetConstantBuffers
Index: 8 | PSSetShaderResources
Index: 9 | PSSetShader
Index: 10 | SetSamplers
Index: 11 | SetShader
Index: 12 | DrawIndexed
Index: 13 | Draw
Index: 14 | Map
Index: 15 | Unmap
Index: 16 | PSSetConstantBuffer
Index: 17 | IASetInputLayout
Index: 18 | IASetVertexBuffers
Index: 19 | IASetIndexBuffer
Index: 20 | DrawIndexedInstanced
Index: 21 | DrawInstanced
Index: 22 | GSSetConstantBuffers
Index: 23 | GSSetShader
Index: 24 | IASetPrimitiveTopology
Index: 25 | VSSetShaderResources
Index: 26 | VSSetSamplers
Index: 27 | Begin
Index: 28 | End
Index: 29 | GetData
Index: 30 | GSSetPredication
Index: 31 | GSSetShaderResources
Index: 32 | GSSetSamplers
Index: 33 | OMSetRenderTargets
Index: 34 | OMSetRenderTargetsAndUnorderedAccessViews
Index: 35 | OMSetBlendState
Index: 36 | OMSetDepthStencilState
Index: 37 | SOSetTargets
Index: 38 | DrawAuto
Index: 39 | DrawIndexedInstancedIndirect
Index: 40 | DrawInstancedIndirect
Index: 41 | Dispatch
Index: 42 | DispatchIndirect
Index: 43 | RSSetState
Index: 44 | RSSetViewports
Index: 45 | RSSetScissorRects
Index: 46 | CopySubresourceRegion
Index: 47 | CopyResource
Index: 48 | UpdateSubresource
Index: 49 | CopyStructureCount
Index: 50 | ClearRenderTargetView
Index: 51 | ClearUnorderedAccessViewUint
Index: 52 | ClearUnorderedAccessViewFloat
Index: 53 | ClearDepthStencilView
Index: 54 | GenerateMips
Index: 55 | SetResourceMinLOD
Index: 56 | GetResourceMinLOD
Index: 57 | ResolveSubresource
Index: 58 | ExecuteCommandList
Index: 59 | HSSetShaderResources
Index: 60 | HSSetShader
Index: 61 | HSSetSamplers
Index: 62 | HSSetConstantBuffers
Index: 63 | DSSetShaderResources
Index: 64 | DSSetShader
Index: 65 | DSSetSamplers
Index: 66 | DSSetConstantBuffers
Index: 67 | DSSetShaderResources
Index: 68 | CSSetUnorderedAccessViews
Index: 69 | CSSetShader
Index: 70 | CSSetSamplers
Index: 71 | CSSetConstantBuffers
Index: 72 | VSGetConstantBuffers
Index: 73 | PSGetShaderResources
Index: 74 | PSGetShader
Index: 75 | PSGetSamplers
Index: 76 | VSGetShader
Index: 77 | PSGetConstantBuffers
Index: 78 | IAGetInputLayout
Index: 79 | IAGetVertexBuffers
Index: 80 | IAGetIndexBuffer
Index: 81 | GSGetConstantBuffers
Index: 82 | GSGetShader
Index: 83 | IAGetPrimitiveTopology
Index: 84 | VSGetShaderResources
Index: 85 | VSGetSamplers
Index: 86 | GetPredication
Index: 87 | GSGetShaderResources
Index: 88 | GSGetSamplers
Index: 89 | OMGetRenderTargets
Index: 90 | OMGetRenderTargetsAndUnorderedAccessViews
Index: 91 | OMGetBlendState
Index: 92 | OMGetDepthStencilState
Index: 93 | SOGetTargets
Index: 94 | RSGetState
Index: 95 | RSGetViewports
Index: 96 | RSGetScissorRects
Index: 97 | HSGetShaderResources
Index: 98 | HSGetShader
Index: 99 | HSGetSamplers
Index: 100 | HSGetConstantBuffers
Index: 101 | DSGetShaderResources
Index: 102 | DSGetShader
Index: 103 | DSGetSamplers
Index: 104 | DSGetConstantBuffers
Index: 105 | CSGetShaderResources
Index: 106 | CSGetUnorderedAccessViews
Index: 107 | CSGetShader
Index: 108 | CSGetSamplers
Index: 109 | CSGetConstantBuffers
Index: 110 | ClearState
Index: 111 | Flush
Index: 112 | GetType
Index: 113 | GetContextFlags
Index: 114 | FinishCommandList
// ID3D11 DEVICE virtuals
*QueryInterface 						0
*AddRef 								1
*Release 								2
*CreateBuffer 							3
*CreateTexture1D 						4
*CreateTexture2D 						5
*CreateTexture3D 						6
*CreateShaderResourceView				7
*CreateUnorderedAccessView 				8
*CreateRenderTargetView 				9
*CreateDepthStencilView 				10
*CreateInputLayout 						11
*CreateVertexShader 					12
*CreateGeometryShader 					13
*CreateGeometryShaderWithStreamOutput	14
*CreatePixelShader 						15
*CreateHullShader 						16
*CreateDomainShader						17
*CreateComputeShader					18
*CreateClassLinkage 					19
*CreateBlendState 						20
*CreateDepthStencilState 				21
*CreateRasterizerState 					22
*CreateSamplerState						23
*CreateQuery 							24
*CreatePredicate 						25
*CreateCounter 							26
*CreateDeferredContext 					27
*OpenSharedResource 					28
*CheckFormatSupport 					29
*CheckMultisampleQualityLevels 			30
*CheckCounterInfo 						31
*CheckCounter 							32
*CheckFeatureSupport 					33
*GetPrivateData 						34
*SetPrivateData 						35
*SetPrivateDataInterface 				36
*GetFeatureLevel 						37
*GetCreationFlags 						38
*GetDeviceRemovedReason 				39
*GetImmediateContext 					40
*SetExceptionMode 						41
*GetExceptionMode 						42
// IDXGI SWAPCHAIN virtuals
[0]    7405CADA    (CMTUseCountedObject<CDXGISwapChain>::QueryInterface)
[1]    7405C9A7    (CMTUseCountedObject<CDXGISwapChain>::AddRef)
[2]    7405C9D8    (CMTUseCountedObject<CDXGISwapChain>::Release)
[3]    7405D6BF    (CDXGISwapChain::SetPrivateData)
[4]    7405F6FC    (CDXGISwapChain::SetPrivateDataInterface)
[5]    7405D6AF    (CDXGISwapChain::GetPrivateData)
[6]    7406106A    (CDXGISwapChain::GetParent)
[7]    7405EFDE    (CDXGISwapChain::GetDevice)
[8]    74061BD1    (CDXGISwapChain::Present)
[9]    740617A7    (CDXGISwapChain::GetBuffer)
[10]    74065CD6    (CDXGISwapChain::SetFullscreenState)
[11]    740662DC    (CDXGISwapChain::GetFullscreenState)
[12]    74061146    (CDXGISwapChain::GetDesc)
[13]    740655ED    (CDXGISwapChain::ResizeBuffers)
[14]    74065B8D    (CDXGISwapChain::ResizeTarget)
[15]    7406197B    (CDXGISwapChain::GetContainingOutput)
[16]    74066524    (CDXGISwapChain::GetFrameStatistics)
[17]    74066A58    (CDXGISwapChain::GetLastPresentCount)
[18]    740612C6    (CDXGISwapChain::GetDesc1)
[19]    740613E0    (CDXGISwapChain::GetFullscreenDesc)
[20]    740614F9    (CDXGISwapChain::GetHwnd)
[21]    7406156D    (CDXGISwapChain::GetCoreWindow)
[22]    74061D0D    (CDXGISwapChain[::IDXGISwapChain1]::Present1)
[23]    74062069    (CDXGISwapChain::IsTemporaryMonoSupported)
[24]    740615BB    (CDXGISwapChain::GetRestrictToOutput)
[25]    740615FB    (CDXGISwapChain::SetBackgroundColor)
[26]    740616F1    (CDXGISwapChain::GetBackgroundColor)
[27]    7406173F    (CDXGISwapChain::SetRotation)
[28]    74061770    (CDXGISwapChain::GetRotation)
[29]    7405CC1A    (CMTUseCountedObject<CDXGISwapChain>::`vector deleting destructor')
[30]    7405181E    (CMTUseCountedObject<CDXGISwapChain>::LUCCompleteLayerConstruction)
[31]    7405CBA5    (DXGID3D10ETWRundown)
// DXGI VTable:
[0]	6ED3F979	(CMTUseCountedObject<CDXGISwapChain>::QueryInterface)
[1]	6ED3F84D	(CMTUseCountedObject<CDXGISwapChain>::AddRef)
[2]	6ED3F77D	(CMTUseCountedObject<CDXGISwapChain>::Release)
[3]	6ED6A6D7	(CDXGISwapChain::SetPrivateData)
[4]	6ED6A904	(CDXGISwapChain::SetPrivateDataInterface)
[5]	6ED72BC9	(CDXGISwapChain::GetPrivateData)
[6]	6ED6DCDD	(CDXGISwapChain::GetParent)
[7]	6ED69BF4	(CDXGISwapChain::GetDevice)
[8]	6ED3FAAD	(CDXGISwapChain::Present)
[9]	6ED40209	(CDXGISwapChain::GetBuffer)
[10]	6ED47C1C	(CDXGISwapChain::SetFullscreenState)
[11]	6ED48CD9	(CDXGISwapChain::GetFullscreenState)
[12]	6ED40CB1	(CDXGISwapChain::GetDesc)
[13]	6ED48A3B	(CDXGISwapChain::ResizeBuffers)
[14]	6ED6F153	(CDXGISwapChain::ResizeTarget)
[15]	6ED47BA5	(CDXGISwapChain::GetContainingOutput)
[16]	6ED6D9B5	(CDXGISwapChain::GetFrameStatistics)
[17]	6ED327B5	(CDXGISwapChain::GetLastPresentCount)
[18]	6ED43400	(CDXGISwapChain::GetDesc1)
[19]	6ED6D9D0	(CDXGISwapChain::GetFullscreenDesc)
[20]	6ED6DA90	(CDXGISwapChain::GetHwnd)
[21]	6ED6D79F	(CDXGISwapChain::GetCoreWindow)
[22]	6ED6E352	(?Present1@?QIDXGISwapChain2@@CDXGISwapChain@@UAGJIIPBUDXGI_PRESENT_PARAMETERS@@@Z)
[23]	6ED6E240	(CDXGISwapChain::IsTemporaryMonoSupported)
[24]	6ED44146	(CDXGISwapChain::GetRestrictToOutput)
[25]	6ED6F766	(CDXGISwapChain::SetBackgroundColor)
[26]	6ED6D6B9	(CDXGISwapChain::GetBackgroundColor)
[27]	6ED4417B	(CDXGISwapChain::SetRotation)
[28]	6ED6DDE3	(CDXGISwapChain::GetRotation)
[29]	6ED6FF85	(CDXGISwapChain::SetSourceSize)
[30]	6ED6DF4F	(CDXGISwapChain::GetSourceSize)
[31]	6ED6FCBD	(CDXGISwapChain::SetMaximumFrameLatency)
[32]	6ED6DBE5	(CDXGISwapChain::GetMaximumFrameLatency)
[33]	6ED6D8CD	(CDXGISwapChain::GetFrameLatencyWaitableObject)
[34]	6ED6FB45	(CDXGISwapChain::SetMatrixTransform)
[35]	6ED6DAD0	(CDXGISwapChain::GetMatrixTransform)
[36]	6ED6C155	(CDXGISwapChain::CheckMultiplaneOverlaySupportInternal)
[37]	6ED6E82D	(CDXGISwapChain::PresentMultiplaneOverlayInternal)
[38]	6ED4397A	(CMTUseCountedObject<CDXGISwapChain>::`vector deleting destructor')
[39]	6ED4EAE0	(CSwapBuffer::AddRef)
[40]	6ED46C81	(CMTUseCountedObject<CDXGISwapChain>::LUCBeginLayerDestruction)
*/