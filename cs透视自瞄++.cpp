//小远

#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <Windows.h>
#include <tlhelp32.h>

#define TOP_HEIGHT 25
struct PlayerData
{
	DWORD BaseEntity ;
	float Position[3];
	int HP; 
	int TeamFlag;
	char Name[32];
};

HWND  g_hWnd_Overlay;

DWORD __dwServer;
DWORD __dwEngine;
HANDLE g_hProcess;

const DWORD HP_offset			= 0xE4;
const DWORD ID_offset			= 0xE24;
const DWORD TeamFlag_offset		= 0x1F4;
const DWORD Player_offset		= 0x10;
const DWORD Pos_offset			= 0x308;
const DWORD MY_Base				= 0x4F3FCC;
const DWORD EL_Base				= 0x4F3FDC;
const DWORD WordMatrix_Base		= 0x5B3D70;//自己的世界矩阵

const DWORD CursorAngle_X_offset = 0x47F1B8;
const DWORD CursorAngle_Y_offset = 0x47F1B4;

//const DWORD Fick = 0x5AD9E4;//???
float g_Matrix[4][4];
RECT  g_winRect = { 0 };

//自瞄
BOOL  IsAuto = FALSE;
/*

鼠标Y	 engine.dll + 47F1B4
鼠标X	 engine.dll + 47F1B8

*/

//www.cheat8.com 与你分享奇特吧

DWORD GetModuleBaseAddress(const TCHAR* LPSTRModuleName, DWORD __DwordProcessId);
float GetDistance3D(float mycoor[3], float enemcoor[3]);
void  ReadDataList(int index, PlayerData* MY, PlayerData* EL);
void  ReadCursorAngle(float* CursorAngle_X, float* CursorAngle_Y);
void  DrawEsp(float Enemy_x, float Enemy_y, float distance, PlayerData* MY, PlayerData* EL);
void  Esp(PlayerData* MY, PlayerData* EL);
BOOL  WordToScreen(float from[3], float to[2]);

void Clear();
LRESULT CALLBACK WindowProc_Overlay(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateOverlayWindow();
BOOL GameIsForegroundWindow();

//自瞄所用到的函数

void AutoCollimation(float MY_Position[3], float EL_Position[3]);
void GetXYDistance(PlayerData* EL, float* XYDistance);
int  GetMinXYDistanceIndex(float xyDistance[32]);
void  __stdcall KeyHandlerThread();

int  main(int argc, char* argv[])
{

	HWND hWnd = FindWindow(NULL, _T("Counter-Strike Source"));
	if (hWnd == NULL)
		return -1;

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)KeyHandlerThread, 0, 0, 0);


	DWORD dwPid;  
	GetWindowThreadProcessId(hWnd, &dwPid);
	g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
	__dwServer = GetModuleBaseAddress(_T("server.dll"), dwPid);
	__dwEngine = GetModuleBaseAddress(_T("engine.dll"), dwPid);

	float CursorAngle_X;
	float CursorAngle_Y;

	int numb = 32;

	float xyDistance[32] = { 0 };//敌人距离屏幕中心的坐标
	float Position[32][3] = { 0 };//敌人的坐标

	PlayerData MY = { 0 };
	PlayerData EL = { 0 };

	GetWindowRect(hWnd, &g_winRect);
	g_winRect.top += TOP_HEIGHT;
	CreateOverlayWindow();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (GameIsForegroundWindow() == TRUE)
		{
			SetWindowPos(g_hWnd_Overlay, HWND_TOPMOST, g_winRect.left, g_winRect.top, (g_winRect.right - g_winRect.left), (g_winRect.bottom - g_winRect.top), SWP_SHOWWINDOW);
		}
		else
		{
			SetWindowPos(g_hWnd_Overlay, HWND_BOTTOM, g_winRect.left, g_winRect.top, (g_winRect.right - g_winRect.left), (g_winRect.bottom - g_winRect.top), SWP_SHOWWINDOW);
		}


		GetWindowRect(hWnd, &g_winRect);
		g_winRect.top += TOP_HEIGHT;
		MoveWindow(g_hWnd_Overlay, g_winRect.left, g_winRect.top, (g_winRect.right - g_winRect.left), (g_winRect.bottom - g_winRect.top), TRUE);
		Clear();
		for (int i = 0; i<numb; i++)
		{
			ReadDataList(i, &MY, &EL);
			//ReadCursorAngle(&CursorAngle_X, &CursorAngle_Y);
			Position[i][0] = 0.0f;
			Position[i][1] = 0.0f;
			Position[i][2] = 0.0f;
			xyDistance[i] = 999999.0f;

			if (EL.HP<2 )//死亡
				continue;
			if (EL.BaseEntity == 0)//没人
				continue;
			if (MY.TeamFlag == EL.TeamFlag)//是队友
				continue;

			//保存敌人坐标
			Position[i][0] = EL.Position[0];
			Position[i][1] = EL.Position[1];
			Position[i][2] = EL.Position[2];
			GetXYDistance(&EL, &xyDistance[i]);

			Esp(&MY, &EL);
		}
		if (IsAuto == TRUE)
		{
			//取得距离屏幕中心最近敌人的下标
			int index = GetMinXYDistanceIndex(xyDistance);
			if (index != -1)
			{
				ReadDataList(index, &MY, &EL);
				AutoCollimation(MY.Position, EL.Position);
			}
			
		}

		Sleep(20);
	}

	CloseHandle(g_hProcess);
	return 0;
}
void ReadDataList(int index, PlayerData* MY, PlayerData* EL)
{

	ReadProcessMemory(g_hProcess, (PBYTE*)(__dwServer	  + MY_Base), &MY->BaseEntity, sizeof(DWORD), 0);//获取基地址
	ReadProcessMemory(g_hProcess, (PBYTE*)(MY->BaseEntity + Pos_offset), &MY->Position, sizeof(float[3]), 0);//坐标
	ReadProcessMemory(g_hProcess, (PBYTE*)(MY->BaseEntity + TeamFlag_offset), &MY->TeamFlag, sizeof(int), 0);//队伍标识

	ReadProcessMemory(g_hProcess, (PBYTE*)(__dwServer	  + EL_Base + (index * Player_offset)), &EL->BaseEntity, sizeof(DWORD), 0);
	ReadProcessMemory(g_hProcess, (PBYTE*)(EL->BaseEntity + HP_offset),  &EL->HP, sizeof(int), 0);
	ReadProcessMemory(g_hProcess, (PBYTE*)(EL->BaseEntity + ID_offset),  &EL->Name, sizeof(char[32]), 0);
	sprintf(EL->Name, "%s [ %d ]", EL->Name, EL->HP);

	ReadProcessMemory(g_hProcess, (PBYTE*)(EL->BaseEntity + Pos_offset), &EL->Position, sizeof(float[3]), 0);
	ReadProcessMemory(g_hProcess, (PBYTE*)(EL->BaseEntity + TeamFlag_offset), &EL->TeamFlag, sizeof(int), 0);
	
	//float fick1 =0.0f;//?????
	//ReadProcessMemory(g_hProcess, (PBYTE*)(__dwEngine + Fick), &fick1, sizeof(float), 0);
	//if (fick1 == 1.0)
	ReadProcessMemory(g_hProcess, (PBYTE*)(__dwEngine + WordMatrix_Base), &g_Matrix, sizeof(g_Matrix), 0);

}

void Esp(PlayerData* MY, PlayerData* EL)
{
	float EnemyXY[2];
	WordToScreen(EL->Position, EnemyXY);


	float Distance = GetDistance3D(MY->Position, EL->Position);
	DrawEsp(EnemyXY[0], EnemyXY[1], Distance, MY , EL );
}

void DrawEsp(float Enemy_x, float Enemy_y, float distance,PlayerData* MY,PlayerData* EL)
{
	//Enemy_y -= 10;//微调

	float Rect_w =  21000 / distance;
	float Rect_h = 40000 / distance;

	float Rect_x = (Enemy_x - (Rect_w / 2));
	float Rect_y =  Enemy_y - Rect_h;

	float Line_x = (g_winRect.right - g_winRect.left) / 2;
	float Line_y = g_winRect.bottom - g_winRect.top;
	//--------------------------
	HWND hWnd = GetForegroundWindow();//获取当前窗口
	if (hWnd == FindWindow(NULL, _T("Counter-Strike Source")))
	{
		HDC hDc = GetDC(g_hWnd_Overlay);//!!!!!!!!!!!!!!!!!!!此处为overlay窗口句柄
		HPEN hPen = CreatePen(PS_SOLID, 2, 0x0000FF);//画笔一旦创建后就无法修改
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);//空画刷
		SelectObject(hDc, hPen);
		SelectObject(hDc, hBrush);

		Rectangle(hDc, Rect_x, Rect_y, Rect_x + Rect_w, Rect_y + Rect_h);
		MoveToEx(hDc, Line_x, Line_y, NULL);
		LineTo(hDc, Enemy_x, Enemy_y);

		SetTextColor(hDc, RGB(0, 0 , 255));
		SetBkMode(hDc, NULL_BRUSH);     //设置字体
		TextOutA(hDc, Rect_x, Rect_y - 20, EL->Name, strlen(EL->Name));

		DeleteObject(hBrush);
		DeleteObject(hPen);
		
		ReleaseDC(g_hWnd_Overlay, hDc);//!!!!!!!!!!!!!!!!!!!此处为overlay窗口句柄
	}
	

}

void Clear()
{
	RECT rect = { 0,0,g_winRect.right - g_winRect.left,g_winRect.bottom - g_winRect.top };
	HWND hWnd = GetForegroundWindow();//获取当前窗口
	if (hWnd == FindWindow(NULL, _T("Counter-Strike Source")))
	{
		HDC hDc = GetDC(g_hWnd_Overlay);//!!!!!!!!!!!!!!!!!!!此处为overlay窗口句柄
		HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);//空画刷
		SelectObject(hDc, hBrush);

		FillRect(hDc, &rect, hBrush);

		DeleteObject(hBrush);
		ReleaseDC(g_hWnd_Overlay, hDc);//!!!!!!!!!!!!!!!!!!!此处为overlay窗口句柄
	}
}

//世界坐标转屏幕坐标
BOOL WordToScreen(float from[3], float to[2])
{
	/*float x1, y1, w;
	x1 = g_Matrix[0][0] * from[0] + g_Matrix[0][1] * from[1] + g_Matrix[0][2] * from[2] + g_Matrix[0][3];
	y1 = g_Matrix[1][0] * from[0] + g_Matrix[1][1] * from[1] + g_Matrix[1][2] * from[2] + g_Matrix[1][3];
	w = g_Matrix[3][0]  * from[0] + g_Matrix[3][1] * from[1] + g_Matrix[3][2] * from[2] + g_Matrix[3][3];

	if (w < 0.01f)
		return FALSE;

	x1 = x1 / w;
	y1 = y1 / w;

	to[0] =   ((g_winRect.right  - g_winRect.left) / 2.0 * x1) + (x1 + (g_winRect.right - g_winRect.left) / 2.0);
	to[1] = - ((g_winRect.bottom - g_winRect.top)  / 2.0 * y1) + (y1 + (g_winRect.bottom - g_winRect.top) / 2.0);

	return TRUE;*/

	//计算与玩家的相机比较的角度。
	float w = g_Matrix[3][0] * from[0] + g_Matrix[3][1] * from[1] + g_Matrix[3][2] * from[2] + g_Matrix[3][3]; //Calculate the angle in compareson to the player's camera.
	if (w > 0.001) //如果对象在视图中.
	{
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		to[0] = ((g_winRect.right - g_winRect.left) / 2) + (0.5f * ((g_Matrix[0][0] * from[0] + g_Matrix[0][1] * from[1] + g_Matrix[0][2] * from[2] + g_Matrix[0][3]) * fl1DBw) * (g_winRect.right - g_winRect.left) + 0.5f); //Get the X dimension and push it in to the Vector.
		to[1] = ((g_winRect.bottom - g_winRect.top) / 2) - (0.5f * ((g_Matrix[1][0] * from[0] + g_Matrix[1][1] * from[1] + g_Matrix[1][2] * from[2] + g_Matrix[1][3]) * fl1DBw) * (g_winRect.bottom - g_winRect.top) + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;


}

float GetDistance3D(float MyPos[3], float ObjPos[3])
{
	return sqrt
	(
		pow(double(ObjPos[0] - MyPos[0]), 2.0) +
		pow(double(ObjPos[1] - MyPos[1]), 2.0) +
		pow(double(ObjPos[2] - MyPos[2]), 2.0)
	);
}

DWORD GetModuleBaseAddress(const TCHAR* LPSTRModuleName, DWORD __DwordProcessId)
{
	MODULEENTRY32 lpModuleEntry = { 0 };
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, __DwordProcessId);
	if (!hSnapShot)
		return NULL;
	lpModuleEntry.dwSize = sizeof(lpModuleEntry);
	BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
	while (__RunModule)
	{
		if (!_tcscmp(lpModuleEntry.szModule, LPSTRModuleName))
		{
			CloseHandle(hSnapShot);
			return (DWORD)lpModuleEntry.modBaseAddr;
		}
		__RunModule = Module32Next(hSnapShot, &lpModuleEntry);
	}
	CloseHandle(hSnapShot);
	return NULL;
}

void CreateOverlayWindow()
{
	/*RECT wndRect;
	HWND hTargetWnd = NULL;
	while (!hTargetWnd)
		hTargetWnd = FindWindow(NULL, _T("Counter-Strike Source"));
	if (hTargetWnd != NULL)
	{
		GetWindowRect(hTargetWnd, &wndRect);
		g_width = wndRect.right - wndRect.left;
		g_height = wndRect.bottom - wndRect.top;
	}
	else
	{
		printf("Closing...");
		Sleep(3000);
		ExitProcess(0);
	}*/
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc_Overlay;
	wc.hInstance = GetModuleHandle(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)RGB(0, 0, 0);
	wc.lpszClassName = _T("Overlay");
	RegisterClassEx(&wc);

	g_hWnd_Overlay = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, wc.lpszClassName, _T("OverlayWindow"), WS_POPUP, g_winRect.left, g_winRect.top,
	(g_winRect.right- g_winRect.left), (g_winRect.bottom - g_winRect.top), NULL, NULL, wc.hInstance, NULL);

	SetLayeredWindowAttributes(g_hWnd_Overlay, RGB(0, 0, 0), 0, ULW_COLORKEY);
	//SetLayeredWindowAttributes(g_hWnd_pop, 0, 0, LWA_ALPHA);

	ShowWindow(g_hWnd_Overlay, SW_SHOW);
	UpdateWindow(g_hWnd_Overlay);
}

BOOL GameIsForegroundWindow()
{
	HWND hWnd = GetForegroundWindow();
	if (hWnd == FindWindow(NULL, _T("Counter-Strike Source")))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

LRESULT CALLBACK WindowProc_Overlay(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_PAINT:
		break;
	case WM_CREATE:
		//DwmExtendFrameIntoClientArea(hWnd, &margin);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(g_hWnd_Overlay);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}



void ReadCursorAngle(_Out_ float* CursorAngle_X, _Out_ float* CursorAngle_Y)
{
	ReadProcessMemory(g_hProcess, (PBYTE*)(__dwEngine + CursorAngle_X_offset), CursorAngle_X, sizeof(CursorAngle_X), 0);
	ReadProcessMemory(g_hProcess, (PBYTE*)(__dwEngine + CursorAngle_Y_offset), CursorAngle_Y, sizeof(CursorAngle_Y), 0);
	
}

//							x y z
void AutoCollimation(float MY_Position[3], float EL_Position[3])
{
	float Dest_X;
	float Dest_Y;

	float singned_dx = EL_Position[0] - MY_Position[0];
	float singned_dy = EL_Position[1] - MY_Position[1];
	float singned_dz = EL_Position[2] - MY_Position[2];

	Dest_X = atan2(singned_dy, singned_dx)*57.355;//用  鼠标角度/atan2(singned_dy, singned_dx)
	Dest_Y = atan2(singned_dz, sqrt(singned_dx*singned_dx + singned_dy * singned_dy))*-59.25;
	WriteProcessMemory(g_hProcess, (PBYTE*)(__dwEngine + CursorAngle_X_offset), &Dest_X, sizeof(Dest_X), 0);
	WriteProcessMemory(g_hProcess, (PBYTE*)(__dwEngine + CursorAngle_Y_offset), &Dest_Y, sizeof(Dest_Y), 0);
}

void GetXYDistance(PlayerData* EL, float* XYDistance)
{
	float EnemyXY[2];
	WordToScreen(EL->Position, EnemyXY);
	//在屏幕内
	if ((EnemyXY[0] > 0 && EnemyXY[0] < g_winRect.right - g_winRect.left) &&
		(EnemyXY[1] > 0 && EnemyXY[1] < g_winRect.bottom - g_winRect.top))
	{
		
			*XYDistance =	fabs(EnemyXY[0] - ((g_winRect.right  - g_winRect.left) / 2))+
							fabs(EnemyXY[1] - ((g_winRect.bottom - g_winRect.top ) / 2));
		
	}
	else
	{
		*XYDistance = 999999.0f;
	}

}

int GetMinXYDistanceIndex(float xyDistance[32])
{
	int MinIndex = 0;

	for (int i = 0; i < 32; i++)
	{
		if (xyDistance[i]<xyDistance[MinIndex])
			MinIndex = i;
	}
	if (999999.0f == xyDistance[MinIndex])
		return -1;
	return MinIndex;
}


void  __stdcall KeyHandlerThread()
{
	while (1)
	{
		if (GetAsyncKeyState(VK_LSHIFT) < 0)
			IsAuto = TRUE;	
		else
			IsAuto = FALSE;
		Sleep(1u);
	}
}
