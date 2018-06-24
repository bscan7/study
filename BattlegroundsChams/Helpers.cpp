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
extern bool bLog2Txt;

//MISCELLANEOUS
void Helpers::Log(char* szMessage)
{
	std::cout << "[+] " << szMessage << std::endl;
}
void Helpers::LogAddress(char* szName, int64_t iAddress)
{
	//ofstream outfile;
	//outfile.open("..\\LogAddress.txt", ios::app);
	//if (!outfile)
	//{
	//	std::cout << "打开LogAddress.txt文件失败！" << endl;
	//}
	//else
	//{
	//	outfile << "[+] " << szName << ": 0x" << std::hex << iAddress << std::endl;
	//	outfile.close();
	//}
	std::cout << "[+] " << szName << ": 0x" << std::hex << iAddress << std::endl;
}
void Helpers::Log2Txt(char* szLog, int64_t iAddress)
{
	if (bLog2Txt && iAddress>0)
	{
		ofstream outfile;
		outfile.open("..\\Log2Txt.txt", ios::app);
		if (!outfile)
		{
			std::cout << "打开Log2Txt.txt文件失败！" << endl;
		}
		else
		{
			outfile << "[+] " << szLog << ": " << iAddress << std::endl;
			outfile.close();
		}
	}
}

void Helpers::LogError(char* szMessage)
{
	std::cout << "[Error] " << szMessage << std::endl;
}

void Helpers::LogFormat(const char* strFormat, ...)
{
	const int BUFFER_SIZE = 1280;
	char   szBuffer[BUFFER_SIZE] = { 0 };
	va_list args = NULL;

	// The va_start macro (defined in STDARG.H) is usually equivalent to:  
	// pArgList = (char *) &szFormat + sizeof (szFormat) ;  
	va_start(args, strFormat);

	// The last argument to wvsprintf points to the arguments  
	_vsnprintf(szBuffer, BUFFER_SIZE, strFormat, args);

	// The va_end macro just zeroes out pArgList for no good reason  
	va_end(args);
	//::OutputDebugStringA(szBuffer);
	std::cout << "[:-)] " << szBuffer << std::endl;
}
