// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <process.h>
#include <ostream>
#include <iostream>

HMODULE  hinst = NULL;
void Thread_LoadUnloadDLL(PVOID param)
{
	//Helpers::LogFormat("--------Thread_KeysSwitch---------Started ");
	HANDLE  g_Event_UnHook = CreateEvent(NULL, FALSE, FALSE, NULL);

	while (1)
	{
		std::cout << "！！！！！！！！！！！！！！！！！！！LoadLibrary" << std::endl;
		hinst = LoadLibrary(L"E:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll");
		if (!hinst)
		{
			hinst = LoadLibrary(L"D:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll");
		}
		if (hinst)
		{
			typedef void( *LPFNC_SETEVENT)(HANDLE);
			LPFNC_SETEVENT lpfunc;
			lpfunc = (LPFNC_SETEVENT)GetProcAddress(hinst, "SetUnHookEvent");
			std::cout << "！！！！！！！！！！！！！！！！！！！SetUnHookEvent lpfunc=" << lpfunc << std::endl;

			if (lpfunc)
			{
				(*lpfunc)(g_Event_UnHook);
			}
		}

		std::cout << "！！！！！！！！！！！！！！！！！！！WaitForSingleObject" << std::endl;
		WaitForSingleObject(g_Event_UnHook, INFINITE);
		std::cout << "！！！！！！！！！！！！！！！！！！！::FreeLibrary(hinst)" << hinst << std::endl;
		if (hinst)
			::FreeLibrary(hinst);

		hinst = NULL;
		std::cout << "！！！！！！！！！！！！！！！！！！！g_Event_UnHook =" << g_Event_UnHook << std::endl;

		while (1)
		{
			if (GetAsyncKeyState(VK_HOME) & 1)
			{
				std::cout << "！！！！！！！！！！！！！！！！！！！VK_HOME！VK_HOME！VK_HOME！" << hinst << std::endl;
				break;
			}
			Sleep(100);
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

