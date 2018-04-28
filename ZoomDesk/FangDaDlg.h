//Download by http://www.NewXing.com
// FangDaDlg.h : header file
//

#if !defined(AFX_FANGDADLG_H__32A1F762_FE05_496A_81A7_0D68A0FCA988__INCLUDED_)
#define AFX_FANGDADLG_H__32A1F762_FE05_496A_81A7_0D68A0FCA988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TopDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFangDaDlg dialog

class CFangDaDlg : public CDialog
{
// Construction
public:
	CFangDaDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CFangDaDlg();

// Dialog Data
	//{{AFX_DATA(CFangDaDlg)
	enum { IDD = IDD_FANGDA_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	bool bZoom;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFangDaDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
// Implementation
private:
	CMenu m_menu;
	int m_nFangDa;
	int m_nSize;
	CTopDlg* m_pTop;	
	CDC* m_pBufferDC;		
	CBitmap	m_BufferBitmap;		
	bool m_bIsIn;
	bool m_bHot;
	CDC* m_pScreenDC;
	CBitmap	m_ScreenBitmap;


protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFangDaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();

	void AutoLockByColor(HDC hDeskDC, CRect &rc, CRect &wRc);

	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuF1();
	afx_msg void OnMenuF2();
	afx_msg void OnMenuF3();
	afx_msg void OnMenuS1();
	afx_msg void OnMenuS2();
	afx_msg void OnMenuS3();
	afx_msg void OnMenuExit();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuZ();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FANGDADLG_H__32A1F762_FE05_496A_81A7_0D68A0FCA988__INCLUDED_)
