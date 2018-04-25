//Download by http://www.NewXing.com
// FangDa.h : main header file for the FANGDA application
//

#if !defined(AFX_FANGDA_H__437BC279_1AB7_482D_AE55_E6BFCC681054__INCLUDED_)
#define AFX_FANGDA_H__437BC279_1AB7_482D_AE55_E6BFCC681054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFangDaApp:
// See FangDa.cpp for the implementation of this class
//

class CFangDaApp : public CWinApp
{
public:
	CFangDaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFangDaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFangDaApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FANGDA_H__437BC279_1AB7_482D_AE55_E6BFCC681054__INCLUDED_)
