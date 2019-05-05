// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <process.h>
#include <ostream>
#include <iostream>

void asmFunc(int* pAddress)
{
	_asm {
		push eax
		push ebx
		push ecx
		mov eax, 0x0F
		mov ebx, 0x10
		add eax, ebx
		mov ecx, pAddress
		mov[ecx], eax
		pop ecx
		pop ebx
		pop eax
	}
}

HMODULE  Hook_Dll_Instance = NULL;
void Thread_LoadUnloadDLL(PVOID param)
{
	//Helpers::LogFormat("--------Thread_KeysSwitch---------Started ");
	HANDLE  g_Event_UnHook = CreateEvent(NULL, FALSE, FALSE, NULL);

	while (1)
	{
		char path[MAX_PATH];
		//保存当前路径
		GetCurrentDirectoryA(MAX_PATH, path);
		std::cout << "！！！！！！！！！！！！！！！！！！！LoadLibrary, path="<< path << std::endl;

		TCHAR chCurDir[MAX_PATH] = { 0 };
		GetCurrentDirectory(MAX_PATH, chCurDir);
		SetCurrentDirectory(L"c:\\upload\\");
		Hook_Dll_Instance = LoadLibrary(L"00TslGame_BATTLEGROUNDS_Release.dll");
		SetCurrentDirectory(chCurDir);
		if (!Hook_Dll_Instance)
		{
			Hook_Dll_Instance = LoadLibrary(L".\\TCPlugins\\AddinStock.dll");
		}
		if (!Hook_Dll_Instance)
		{
			Hook_Dll_Instance = LoadLibrary(L"c:\\upload\\00TslGame_BATTLEGROUNDS_Release.dll");
			std::cout << "111！！！！！！！！！！！！！！！！！！！LoadLibrary, hinst=" << Hook_Dll_Instance << std::endl;
		}
		if (!Hook_Dll_Instance)
		{
			Hook_Dll_Instance = LoadLibrary(L"D:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll");
			std::cout << "222！！！！！！！！！！！！！！！！！！！LoadLibrary, hinst=" << Hook_Dll_Instance << std::endl;
		}
		if (!Hook_Dll_Instance)
		{
			Hook_Dll_Instance = LoadLibrary(L"E:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll");
			std::cout << "333！！！！！！！！！！！！！！！！！！！LoadLibrary, hinst=" << Hook_Dll_Instance << std::endl;
		}
		if (Hook_Dll_Instance)
		{
			typedef void( *LPFNC_SETEVENT)(HANDLE);
			LPFNC_SETEVENT lpfunc;
			lpfunc = (LPFNC_SETEVENT)GetProcAddress(Hook_Dll_Instance, "Addin_GetObject") ;
			void*  xxx = lpfunc;
			std::cout << "444！！！！！！！！！！！！！！！！！！！Addin_GetObject lpfunc=" << lpfunc << std::endl;
			//std::cout << "444！！！！！！！！！！！！！！！！！！！SaleFunc lpfunc=" << (xxx ) << std::endl;
			lpfunc = ((LPFNC_SETEVENT)((UINT)xxx + 0x31E3A0));
			std::cout << "444！！！！！！！！！！！！！！！！！！！SaleFunc lpfunc=" << lpfunc << std::endl;
			if (lpfunc)
			{
				//(*lpfunc)();
				char szStr[128] = "159915";
				char szStr2[128] = "0602572116";
			_asm {
					push ecx
					push 0
					push 0
					push 0
					push 0
					push 0
					push 0
					push 0
					push 0X2710
					push 0x3FC70A3D
					push szStr
					push szStr2
					push 0
					push 0
					mov ecx, lpfunc
					call ecx
					pop ecx
				}

			}
		}

		std::cout << "555！！！！！！！！！！！！！！！！！！！WaitForSingleObject LastError=" << ::GetLastError()<< std::endl;
		WaitForSingleObject(g_Event_UnHook, INFINITE);
		std::cout << "！！！！！！！！！！！！！！！！！！！::FreeLibrary(hinst)" << Hook_Dll_Instance << std::endl;
		if (Hook_Dll_Instance)
			std::cout << "::FreeLibrary() =" << ::FreeLibrary(Hook_Dll_Instance);

		Hook_Dll_Instance = NULL;
		std::cout << "！！！！！！！！！！！！！！！！！！！g_Event_UnHook =" << g_Event_UnHook <<" LastError=" << ::GetLastError()<< std::endl;

		while (1)
		{
			Sleep(500);
			if (GetAsyncKeyState(VK_HOME) & 1)
			{
				std::cout << "！！！！！！！！！！！VK_HOME！VK_HOME！VK_HOME！Hook_Dll_Instance=" << Hook_Dll_Instance << std::endl;
				break;
			}
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	//MessageBoxA(NULL, "DllMain...DllMain!", "uBoos?", MB_ICONINFORMATION);
	AllocConsole();
	freopen("CON", "w", stdout);
	SetConsoleTitle(L"Bscan_LookLook");
	std::cout << "！！！！！！！！！！！！！！！！！！！DllMain" << std::endl;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule); 

		_beginthread(Thread_LoadUnloadDLL, 0, NULL);
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

