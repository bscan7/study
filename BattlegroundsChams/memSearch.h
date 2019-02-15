// MemRepair.cpp : 定义控制台应用程序的入口点。  
//  

#include "stdafx.h"  
#include <Windows.h>  
#include<fstream>
#include<sstream>
#include <iosfwd>
using namespace std;

//BOOL FindFirst(DWORD dwValue);
//BOOL FindNext(DWORD dwValue);
HANDLE g_hProcess;
DWORD g_arList[1024];
DWORD g_nListCnt;

ofstream fout("memDmp.hex", ios::out | ios::binary);;

BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	//printf("\r\ndwBaseAddr=%d, dwValue=%d", dwBaseAddr,  dwValue);

	//读取一页内存  
	BYTE arBytes[4096];
	BOOL bRead = ::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL);
	if (bRead == FALSE)
	{
		return FALSE;
	}

	DWORD *pdw;
	for (int i = 0; i<4096 - 4; i++)
	{

		pdw = (DWORD*)&arBytes[i];
		if (pdw[0] == dwValue)
		{
			g_arList[g_nListCnt++] = dwBaseAddr + i;
		}
		/*出错，应该将地址先转换成DWORD*,即指向DWORD的地址，然后再取[0]
		if ((DWORD)&arBytes[i] == dwValue)
		{
		g_arList[g_nListCnt++] = dwBaseAddr+i;
		}
		*/
	}
	if (g_nListCnt > 1024)
	{
		printf("the position is large than 1024..");
		return FALSE;
	}
	return TRUE;
}

BOOL CompareAPage(DWORD dwBaseAddr, BYTE* pBytes, UINT uLen)
{
	//printf("\r\ndwBaseAddr=%d, dwValue=%d", dwBaseAddr,  dwValue);

	//读取一页内存  
	BYTE arBytes[4096];
	BOOL bRead = ::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL);
	if (bRead == FALSE)
	{
		return FALSE;
	}
	if (1)
	{
		//fout.write((char*)arBytes, 4096);
	}

	BYTE *pdw;
	for (int i = 0; i<4096 - 4; i++)
	{
		pdw = (BYTE*)&arBytes[i];
		BOOL bDiff = FALSE;
		for (int n = 0; n < uLen; n++)
		{
			if (pdw[n] != pBytes[n])
			{
				bDiff = TRUE;
				break;
			}
		}
		if (bDiff == FALSE)
		{
			g_arList[g_nListCnt++] = dwBaseAddr + i;
		}
	}
	if (g_nListCnt > 1024)
	{
		printf("the position is large than 1024..");
		return FALSE;
	}
	return TRUE;
}

BOOL FindFirst(DWORD dwValue, int iLen = -1)
{
	const DWORD dwOneGB = 1 * 1024 * 1024 * 1024; // 1GB  
	const DWORD dwOnePage = 4 * 1024; // 4K  
	DWORD dwBase;
	OSVERSIONINFO versionInfo = { 0 };
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	::GetVersionEx(&versionInfo);
	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) //win98  
	{
		dwBase = 4 * 1024 * 1024; // 4MB  
	}
	else
	{
		dwBase = 64 * 1024; // 64KB  
	}

	
	//从开始地址到2GB的空间查找  
	for (; dwBase<2 * dwOneGB; dwBase += dwOnePage)
	{
		if (iLen == -1)
		{
			CompareAPage(dwBase, dwValue);
		} 
		else
		{
			CompareAPage(dwBase, (BYTE*)dwValue, iLen);
		}
	}

	fout.close();

	return TRUE;
}

BOOL FindNext(DWORD dwValue, int iLen = -1)
{
	DWORD dwOriCnt = g_nListCnt;
	DWORD dwReadValue;
	BYTE byteRead;
	BOOL bRet = FALSE;

	g_nListCnt = 0;
	for (int i = 0; i<dwOriCnt; i++)
	{
		if (iLen == -1)
		{
			if (::ReadProcessMemory(g_hProcess, (LPVOID)g_arList[i], &dwReadValue, sizeof(DWORD), 0))
			{
				if (dwReadValue == dwValue)
				{
					g_arList[g_nListCnt++] = g_arList[i];
					bRet = TRUE;
				}
			}
		}
		else
		{
			BOOL bDiff = FALSE;
			for (int n = 0; n < iLen; n++)
			{
				if (::ReadProcessMemory(g_hProcess, (LPVOID)(g_arList[i]+n), &byteRead, sizeof(BYTE), 0))
				{
					if (byteRead != ((BYTE*)dwValue)[n])
					{
						bDiff = TRUE;
						break;
					}
				}

			}
			if (bDiff == FALSE)
			{
				g_arList[g_nListCnt++] = g_arList[i];
				bRet = TRUE;
			}

			//CompareAPage(dwBase, (BYTE*)dwValue, iLen);
		}
	}
	return bRet;
}

void ShowList()
{
	for (int i = 0; i<g_nListCnt; i++)
	{
		printf("\n==>%d/%d: %08lX\n", i, g_nListCnt, g_arList[i]);
	}
}
BOOL WriteMemory(DWORD dwAddr, DWORD dwValue)
{
	//出错的情况：写入的是&dwValue，而不是（LPVOID）dwValue  
	return WriteProcessMemory(g_hProcess, (LPVOID)dwAddr, &dwValue, sizeof(DWORD), NULL);
}
int SearchAvator(INT PID=NULL)
{
	PID = 28012;
	int g_nNum = 13333;
	g_nListCnt = 0;
	memset(g_arList, 0, sizeof(g_arList));

	//char szCommandLine[] = "D:\\Dev\\GitHub\\study\\Release\\testor.exe";
	//STARTUPINFO si = { sizeof(STARTUPINFO) };
	//si.dwFlags = STARTF_USESHOWWINDOW;
	//si.wShowWindow = TRUE;

	//PROCESS_INFORMATION pi;
	//BOOL bRet = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	//if (bRet == FALSE)
	//{
	//	printf("createProcess failed...");
	//	return -1;
	//}
	//::CloseHandle(pi.hThread);
	//g_hProcess = pi.hProcess;
	if (PID == NULL)
	{
		g_hProcess = ::GetCurrentProcess();
	} 
	else
	{
		g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		printf("[@@@@@@@@@@@@@@@@@@@@@@@@] g_hProcess = %d\n", g_hProcess);
		printf("GetLastError()=%d\n", ::GetLastError());
	}

//Test
	//byte byteArray[10] = { 0x78 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
#define BYTELEN  4

	byte byteArray[BYTELEN] = { 
		 0x00 ,0x00 ,0x00 ,0x00 /*,0x01 ,0x00 ,0x00 ,0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x3F*/ };
	printf("[@@@@First search] byte[64]:\n");
	//FindFirst((DWORD)byteArray, BYTELEN);
	//打印结果  
	ShowList();
	printf("[@@@@First search]		g_nListCnt=%d\n", g_nListCnt);
/*	;
	//输入查找值  
	int iVal = 19710214;
	printf("[@@@@First search]Input iVal=19710214\n");
	//scanf("%d", &iVal);
	//进行第一次查找  
	FindFirst(iVal);
	//打印结果  
	ShowList();

	Sleep(1000);
	//再次查找  
	//while (g_nListCnt > 1)
	{
		printf("[@@@@ Next search] input iVal: 19710214\n");
		//scanf("%d", &iVal);
		FindNext(iVal);
		ShowList();
	}
*/
	////修改值  
	//printf("[Modify]input new value:\n");
	//scanf("%d", &iVal);
	//if (WriteMemory(g_arList[0], iVal))
	//{
	//	printf("write suc...");
	//}

	//::CloseHandle(g_hProcess);
	return 0;
}