
// CJ_Tool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCJ_ToolApp: 
// �йش����ʵ�֣������ CJ_Tool.cpp
//

class CCJ_ToolApp : public CWinApp
{
public:
	CCJ_ToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCJ_ToolApp theApp;