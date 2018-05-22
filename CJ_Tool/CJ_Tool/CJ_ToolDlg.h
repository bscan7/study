
// CJ_ToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CCJ_ToolDlg 对话框
class CCJ_ToolDlg : public CDialogEx
{
// 构造
public:
	CCJ_ToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CJ_TOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int m_iCB12;
	CComboBox m_cb12;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	int m_iCB24;
	CComboBox m_cb24;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo2();
};
