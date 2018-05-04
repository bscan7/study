#include "stdafx.h"
#include "Helpers.h"
#include "iostream"
#include "fstream"
using namespace std;
//HOOKING
void Helpers::HookFunction(PVOID *oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}
void Helpers::UnhookFunction(PVOID *oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}

//MISCELLANEOUS
void Helpers::Log(char* szMessage)
{
	std::cout << "[+] " << szMessage << std::endl;
}
void Helpers::LogAddress(char* szName, int64_t iAddress)
{
	ofstream outfile;
	outfile.open("..\\LogAddress.txt", ios::app);
	if (!outfile)
	{
		std::cout << "打开LogAddress.txt文件失败！" << endl;
	}
	else
	{
		outfile << "[+] " << szName << ": 0x" << std::hex << iAddress << std::endl;
		outfile.close();
	}
	std::cout << "[+] " << szName << ": 0x" << std::hex << iAddress << std::endl;
}
void Helpers::LogError(char* szMessage)
{
	std::cout << "[Error] " << szMessage << std::endl;
}
