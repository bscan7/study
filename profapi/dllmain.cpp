// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <process.h>

HINSTANCE hinst = NULL;
void Thread_LoadUnloadDLL(PVOID param)
{
	//Helpers::LogFormat("--------Thread_KeysSwitch---------Started ");
	while (1)
	{
		if (GetAsyncKeyState(VK_HOME) & 1)
		{
			if (hinst == NULL)
				hinst = ::LoadLibrary(L"E:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll");
			else
			{
				::FreeLibrary(hinst);
				hinst = NULL;
			}
		}
		Sleep(100);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule); 
		hinst = LoadLibrary(L"E:\\Dev\\GitHub\\study\\BattlegroundsChams\\Release\\00TslGame_BATTLEGROUNDS_Release.dll");
		_beginthread(Thread_LoadUnloadDLL, 0, NULL);
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

