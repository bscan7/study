
// CJ_ToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CCJ_ToolDlg �Ի���
class CCJ_ToolDlg : public CDialogEx
{
// ����
public:
	CCJ_ToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CJ_TOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
