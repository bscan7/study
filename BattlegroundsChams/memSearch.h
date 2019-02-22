// MemRepair.cpp : 定义控制台应用程序的入口点。  
//  

//#include "stdafx.h"  
#include <Windows.h>  
#include<fstream>
#include<sstream>
#include <iosfwd>
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib") //timeGetTime
using namespace std;


#define DIRECTORY_TABLE_BASE	0x028

int PTESize;
UINT_PTR PAGE_SIZE_LARGE;
UINT_PTR MAX_PDE_POS;
UINT_PTR MAX_PTE_POS;

struct PTEStruct
{
	unsigned P : 1; // present (1 = present)
	unsigned RW : 1; // read/write
	unsigned US : 1; // user/supervisor
	unsigned PWT : 1; // page-level write-through
	unsigned PCD : 1; // page-level cache disabled
	unsigned A : 1; // accessed
	unsigned Reserved : 1; // dirty
	unsigned PS : 1; // page size (0 = 4-KB page)
	unsigned G : 1; // global page
	unsigned A1 : 1; // available 1 aka copy-on-write
	unsigned A2 : 1; // available 2/ is 1 when paged to disk
	unsigned A3 : 1; // available 3
	unsigned PFN : 20; // page-frame number
};

void InitMemSafe()
{
#ifndef AMD64
	ULONG cr4reg;
	//determine if PAE is used
	cr4reg = (ULONG)__readcr4();
	if ((cr4reg & 0x20) == 0x20)
	{
		PTESize = 8; //pae
		PAGE_SIZE_LARGE = 0x200000;
		MAX_PDE_POS = 0xC0604000;
		MAX_PTE_POS = 0xC07FFFF8;
	}
	else
	{
		PTESize = 4;
		PAGE_SIZE_LARGE = 0x400000;
		MAX_PDE_POS = 0xC0301000;
		MAX_PTE_POS = 0xC03FFFFC;
	}
#else
	PTESize = 8; //pae
	PAGE_SIZE_LARGE = 0x200000;
	MAX_PTE_POS = 0xFFFFF6FFFFFFFFF8ULL;
	MAX_PDE_POS = 0xFFFFF6FB7FFFFFF8ULL;
#endif
}

BOOLEAN IsAddressSafe(UINT_PTR StartAddress)
//BOOLEAN MmIsAddressValid(UINT_PTR StartAddress)
{
#ifdef AMD64
	//cannonical check. Bits 48 to 63 must match bit 47
	UINT_PTR toppart = (StartAddress >> 47);
	if (toppart & 1)
	{
		//toppart must be 0x1ffff
		if (toppart != 0x1ffff)
			return FALSE;
	}
	else
	{
		//toppart must be 0
		if (toppart != 0)
			return FALSE;

	}
#endif
	//PDT+PTE judge
	{
#ifdef AMD64
		UINT_PTR kernelbase = 0x7fffffffffffffffULL;
		if (StartAddress < kernelbase)
		{
			return TRUE;
		}
		else
		{
			PHYSICAL_ADDRESS physical;
			physical.QuadPart = 0;
			physical = MmGetPhysicalAddress((PVOID)StartAddress);
			return (physical.QuadPart != 0);
		}
		return TRUE; //for now untill I ave figure out the win 4 paging scheme
#else
		ULONG kernelbase = 0x7ffe0000;
		UINT_PTR PTE, PDE;
		struct PTEStruct *x;
		if (StartAddress < kernelbase)
		{
			return TRUE;
		}
		PTE = (UINT_PTR)StartAddress;
		PTE = PTE / 0x1000 * PTESize + 0xc0000000;
		//now check if the address in PTE is valid by checking the page table directory at 0xc0300000 (same location as CR3 btw)
		PDE = PTE / 0x1000 * PTESize + 0xc0000000; //same formula
		x = (struct PTEStruct *)PDE;
		if ((x->P == 0) && (x->A2 == 0))
		{
			//Not present or paged, and since paging in this area isn't such a smart thing to do just skip it
			//perhaps this is only for the 4 mb pages, but those should never be paged out, so it should be 1
			//bah, I've got no idea what this is used for
			return FALSE;
		}
		if (x->PS == 1)
		{
			//This is a 4 MB page (no pte list)
			//so, (startaddress/0x400000*0x400000) till ((startaddress/0x400000*0x400000)+(0x400000-1) ) ) is specified by this page
		}
		else //if it's not a 4 MB page then check the PTE
		{
			//still here so the page table directory agreed that it is a usable page table entry
			x = (PTEStruct *)(PVOID)PTE;
			if ((x->P == 0) && (x->A2 == 0))
				return FALSE; //see for explenation the part of the PDE
		}
		return TRUE;
#endif
	}

}

UINT32 idTarget = 0;
//PEPROCESS epTarget = NULL;
UINT32 idGame = 0;
//PEPROCESS epGame = NULL;
UINT32 rw_len = 0;
UINT64 base_addr = 0;


//ULONG64 Get64bitValue(PVOID p)
//{
//	if (MmIsAddressValid(p) == FALSE)
//		return 0;
//	return *(PULONG64)p;
//}
//
//ULONG32 Get32bitValue(PVOID p)
//{
//	if (MmIsAddressValid(p) == FALSE)
//		return 0;
//	return *(PULONG32)p;
//}
//
//
//void KReadProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, OUT PVOID Buffer)
//{
//	ULONG64 pDTB = 0, OldCr3 = 0, vAddr = 0;
//	//Get DTB
//	pDTB = Get64bitValue((UCHAR*)Process + DIRECTORY_TABLE_BASE);
//	if (pDTB == 0)
//	{
//		DbgPrint("[x64Drv] Can not get PDT");
//		return;
//	}
//	//Record old cr3 and set new cr3
//	_disable();
//	OldCr3 = __readcr3();
//	__writecr3(pDTB);
//	_enable();
//	//Read process memory
//	if (MmIsAddressValid(Address))
//	{
//		RtlCopyMemory(Buffer, Address, Length);
//		DbgPrint("[x64Drv] Date read: %ld", *(PDWORD)Buffer);
//	}
//	//Restore old cr3
//	_disable();
//	__writecr3(OldCr3);
//	_enable();
//}
//
//void KWriteProcessMemory(IN PEPROCESS Process, IN PVOID Address, IN UINT32 Length, IN PVOID Buffer)
//{
//	ULONG64 pDTB = 0, OldCr3 = 0, vAddr = 0;
//	//Get DTB
//	pDTB = Get64bitValue((UCHAR*)Process + DIRECTORY_TABLE_BASE);
//	if (pDTB == 0)
//	{
//		DbgPrint("[x64Drv] Can not get PDT");
//		return;
//	}
//	//Record old cr3 and set new cr3
//	_disable();
//	OldCr3 = __readcr3();
//	__writecr3(pDTB);
//	_enable();
//	//Read process memory
//	if (MmIsAddressValid(Address))
//	{
//		RtlCopyMemory(Address, Buffer, Length);
//		DbgPrint("[x64Drv] Date wrote.");
//	}
//	//Restore old cr3
//	_disable();
//	__writecr3(OldCr3);
//	_enable();
//}

struct Vec3
{
	float x, y, z/* = 1.0f*/;
};


//BOOL FindFirst(DWORD dwValue);
//BOOL FindNext(DWORD dwValue);
HANDLE g_hProcess;
DWORD g_arList[1024];
DWORD g_nListCnt;
DWORD dSearchedLen;

//ofstream fout("memDmp.hex", ios::out | ios::binary);;

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

	dSearchedLen += 4096;
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

HMODULE hAOW = LoadLibraryA("aow_exe.exe");

typedef int(_cdecl *Exporter1)(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead);

BOOL aowReadProcessMemory(HANDLE g_hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize, LPDWORD lpNumberOfBytesRead)
{
	BOOL i = FALSE;
	if (hAOW != NULL)
	{
		Exporter1 Ex = (Exporter1)GetProcAddress(hAOW, "_ReadProcessMemory@20");
		//char cOutput[MAX_PATH] = { 0 };
		i = Ex(g_hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);

		printf("aowReadProcessMemory......%d", i);
	}
	return i;
}

BOOL CompareAPage(DWORD dwBaseAddr, BYTE* pBytes, UINT uLen)
{
	//printf("\r\ndwBaseAddr=%d, dwValue=%d", dwBaseAddr,  dwValue);

	//读取一页内存  
	BYTE arBytes[4096];
	BOOL bRead = ::ReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL);
	//BOOL bRead = aowReadProcessMemory(g_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL);
	if (bRead == FALSE)
	{
		//printf("::ReadProcessMemory..Failed\n");
		return FALSE;
	}
	if (1)
	{
		//fout.write((char*)arBytes, 4096);
	}
	dSearchedLen += 4096;
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

	printf("GetLastError()=%d\n", ::GetLastError());
	::GetVersionEx(&versionInfo);
	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) //win98  
	{
		dwBase = 4 * 1024 * 1024; // 4MB  
	}
	else
	{
		dwBase = 64 * 1024; // 64KB  
	}

	printf("GetLastError()=%d\n", ::GetLastError());
	dSearchedLen = 0;
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
	printf("GetLastError()=%d\n", ::GetLastError());

	printf("总共搜索了：%d 字节内存\n", dSearchedLen);
	//fout.close();

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
	//PID = 28012;
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
		printf("GetLastError()=%d\n", ::GetLastError());
		g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		//g_hProcess = OpenProcess(PROCESS_VM_READ, FALSE, PID);
		//g_hProcess = OpenProcess(PROCESS_VM_WRITE, FALSE, PID);
		printf("[@@@@@@@@@@@@@@@@@@@@@@@@] OpenProcess %s, g_hProcess = %d\n", g_hProcess? "成功":"失败",  g_hProcess);
		printf("GetLastError()=%d\n", ::GetLastError());
	}

//Test
	//byte byteArray[10] = { 0x78 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
#define BYTELEN  64

	byte byteArray[BYTELEN] = { 
		 0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x00 ,0x00 ,0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x3F };
	printf("[@@---------<<开始 First search] byte[64]:0x%08x\n", byteArray);
	DWORD astime = timeGetTime();		//auto_shoot
	FindFirst((DWORD)byteArray, BYTELEN);
	printf("[@@--------->>耗时：%d\n", timeGetTime() - astime);

	//打印结果  
	ShowList();
	printf("[@@--------->>结束 First search]		g_nListCnt=%d\n", g_nListCnt);
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

int SearchMatrix(INT PID=NULL)
{
	//PID = 28012;
	g_nListCnt = 0;
	memset(g_arList, 0, sizeof(g_arList));

	if (PID == NULL)
	{
		g_hProcess = ::GetCurrentProcess();
	} 
	else
	{
		printf("GetLastError()=%d\n", ::GetLastError());
		g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		printf("[@@@@@@@@@@@@@@@@@@@@@@@@] g_hProcess = %d\n", g_hProcess);
		printf("GetLastError()=%d\n", ::GetLastError());
	}

//Test
	//byte byteArray[10] = { 0x78 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 };
#define BYTELEN2  44

	byte byteArray[BYTELEN2] = { 
		 0x00 ,0x07 ,0x80 ,0x3f ,0x00 ,0x00 ,0x80 ,0x3f ,0x00 ,0x00 ,0x80 ,0x3f ,0x00 ,0x00 ,0x80 ,0x3f 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x3f ,0x00 ,0x00 ,0x00 ,0x00 
		,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x80 ,0x3f ,0x00 ,0x00 ,0x80 ,0x3f
		 };
	printf("[@@---------<<First search] byte[%d]:0x%08x\n", BYTELEN2, byteArray);
	FindFirst((DWORD)byteArray, BYTELEN2);
	//打印结果  
	ShowList();
	printf("[@@--------->>First search]		g_nListCnt=%d\n", g_nListCnt);
	return 0;
}