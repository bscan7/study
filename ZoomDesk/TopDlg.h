//Download by http://www.NewXing.com
#if !defined(AFX_TOPDLG_H__A2E518AD_81FC_469B_912F_8D8ED03F1000__INCLUDED_)
#define AFX_TOPDLG_H__A2E518AD_81FC_469B_912F_8D8ED03F1000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TopDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTopDlg dialog



class CTopDlg : public CDialog
{
// Construction
public:
	CTopDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTopDlg();

// Dialog Data
	//{{AFX_DATA(CTopDlg)
	enum { IDD = IDD_DLG_TOP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CBrush m_br;
public:
	BOOL SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTopDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOPDLG_H__A2E518AD_81FC_469B_912F_8D8ED03F1000__INCLUDED_)
