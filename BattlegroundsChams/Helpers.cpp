#include "stdafx.h"
#include "Helpers.h"
#include "iostream"
#include "fstream"
#include <MMSystem.h>
#include <string>
#include <iomanip>
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
extern bool bLog2Txt_F7;
extern ofstream outfile;

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

void Helpers::LogBuf2Txt(string sPre, const void* pBuf, int64_t iSize)
{
	//if (timeGetTime()%10 == 0)
	if (bLog2Txt_F7)
	{
		ofstream outfile;
		outfile.open("..\\" + sPre + to_string(timeGetTime()) + ".txt", ios::app);
		//outfile.open("..\\" + to_string(timeGetTime()), ios::app);
		if (!outfile)
		{
			std::cout << "打开Log2Txt.txt文件失败！" << endl;
		}
		else
		{
			//D3D11_BUFFER_DESC desc;
			//((ID3D11Buffer*)pBuf)->GetDesc(&desc);

			outfile << std::endl;
			//outfile << " BindFlags=" << desc.BindFlags << " ByteWidth=" << desc.ByteWidth << " MiscFlags=" << desc.MiscFlags << " StructureByteStride=" << desc.StructureByteStride << " Usage=" << desc.Usage << std::hex << " CPUAccessFlags=0x" << desc.CPUAccessFlags << std::endl;
			outfile << std::hex << " pBuf=0x" << pBuf << std::endl;
			for (int i = 0; i < iSize; i += sizeof(int))
			{
				if (i>0 && i%16==0)
				{
					outfile << std::endl;
				}
				int xxx = *(int*)((int)pBuf + i);
				outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)((int)pBuf+i) << " ";
			}
			
			outfile.close();
		}
	}
}

//void Helpers::LogBuf2Txt(string sPre, const void* pBuf, int64_t iSize) //FOR UnMap use only!!!
//{
//	//if (timeGetTime()%10 == 0)
//	if (bLog2Txt_F7)
//	{
//		ofstream outfile;
//		outfile.open("..\\" + sPre + to_string(timeGetTime()) + ".txt", ios::app);
//		//outfile.open("..\\" + to_string(timeGetTime()), ios::app);
//		if (!outfile)
//		{
//			std::cout << "打开Log2Txt.txt文件失败！" << endl;
//		}
//		else
//		{
//			D3D11_BUFFER_DESC desc;
//			((ID3D11Buffer*)pBuf)->GetDesc(&desc);
//
//			outfile << std::endl;
//			outfile << " BindFlags=" << desc.BindFlags << " ByteWidth=" << desc.ByteWidth << " MiscFlags=" << desc.MiscFlags << " StructureByteStride=" << desc.StructureByteStride << " Usage=" << desc.Usage << std::hex << " CPUAccessFlags=0x" << desc.CPUAccessFlags << std::endl;
//			outfile << std::hex << " pBuf=0x" << pBuf << std::endl;
//			for (int i = 0; i < iSize; i += sizeof(int))
//			{
//				if (i>0 && i%16==0)
//				{
//					outfile << std::endl;
//				}
//				int xxx = *(int*)((int)pBuf + i);
//				outfile << std::hex << std::setw(8) << std::setfill('0') << *(int*)((int)pBuf+i) << " ";
//			}
//			
//			outfile.close();
//		}
//	}
//}
//
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

	if (bLog2Txt_F7)
	{
		//outfile.open("..\\Log2Txt.txt", ios::app);
		if (!outfile)
		{
			std::cout << "打开Frame***文件失败！" << endl;
		}
		else
		{
			outfile << "[+] " << szBuffer << std::endl;
			//outfile.close();
		}
	}
	else
		std::cout << "[:-)] " << szBuffer << std::endl;
}
