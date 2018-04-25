//Download by http://www.NewXing.com
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CE596A73_308A_468E_9B3C_BBFA98D80952__INCLUDED_)
#define AFX_STDAFX_H__CE596A73_308A_468E_9B3C_BBFA98D80952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#define WS_EX_LAYERED 0x00080000
#define LWA_COLORKEY 0x00000001
#define LWA_ALPHA 0x00000002
typedef BOOL (FAR WINAPI *LAYERFUNC)(HWND,COLORREF,BYTE,DWORD);



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CE596A73_308A_468E_9B3C_BBFA98D80952__INCLUDED_)
