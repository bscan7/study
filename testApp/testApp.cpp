// testApp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../BattlegroundsChams/memSearch.h"
#include <TlHelp32.h>
#include <psapi.h>

#include "CPUusage.h"  
//int _tmain(int argc, _TCHAR* argv[])
//{
//
//	CPUusage usg(12316);
//	for (int i = 0; i < 10; i++)
//	{
//		float cpu = usg.get_cpu_usage();
//		printf("Taskmgr.exe: %.2f%%\n", cpu);
//		Sleep(500);
//	}
//
//	usg.setpid(11084);
//	for (int i = 0; i < 10; i++)
//	{
//		float cpu = usg.get_cpu_usage();
//		printf("devenv.exe: %.2f%%\n", cpu);
//		Sleep(1000);
//	}
//
//	return 0;
//}

int main()
{


	DWORD needed;

	HANDLE hProcess = NULL;
	HMODULE hModule;
	//wchar_t path[260] = _T("");
	HANDLE hToken;


	HANDLE  hProcessSnap = NULL;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	float maxcpu = 0.0;
	DWORD maxcpuProcessID = 0;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		Process32First(hProcessSnap, &pe32);
		CPUusage usg;
		do
		{

			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pe32.th32ProcessID);
			if (hProcess)
			{
				//::EnumProcessModules(hProcess, &hModule, sizeof(hModule), &needed);
				//::GetModuleFileNameExW(hProcess, hModule, path, sizeof(path));//进程路径

			}
			if (_tcsstr(pe32.szExeFile, L"aow_exe.exe"))
			{
				//for (int i = 0; i < 2; i++)
				{
					usg.setpid(pe32.th32ProcessID);
					float cpu = usg.get_cpu_usage();
					//printf("pid=%d CPU: %.2f%%\n", pe32.th32ProcessID , cpu);
					Sleep(500);
					
					cpu = usg.get_cpu_usage();
					printf("pid=%d CPU: %.2f%%\n", pe32.th32ProcessID, cpu);

					if (maxcpu < cpu)
					{
						maxcpu = cpu;
						maxcpuProcessID = pe32.th32ProcessID;
					}

				}

			}
			pe32.szExeFile;//进程名;
			pe32.th32ProcessID;//进程ID;

		} while (Process32Next(hProcessSnap, &pe32));
	}
	if (hProcess)
	{
		CloseHandle(hProcess);
	}
	//CloseHandle(hModule);
	printf("maxCPU.pid=%d CPU: %.2f%%\n", maxcpuProcessID, maxcpu);
	//Sleep(2000);

	SearchAvator(maxcpuProcessID);
	Sleep(20000);
	return 0;
}

