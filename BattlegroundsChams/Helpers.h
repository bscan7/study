#pragma once
#include "stdafx.h"
//#define int64_t LONGLONG
#define int64_t long long int

namespace Helpers
{
	void LogAddress(char* szName, LONGLONG iAddress = 0x1111);
	//void Log2Txt(char * szLog, int64_t iAddress = 789);
	void LogError(char* szMessage);
	void LogFormat(const char* strFormat, ...);
	void Log(char* szMessage);
	void HookFunction(PVOID *oFunction, PVOID pDetour);
	void UnhookFunction(PVOID *oFunction, PVOID pDetour);
}