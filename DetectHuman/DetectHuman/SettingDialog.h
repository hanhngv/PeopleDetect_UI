#pragma once
#include "afxwin.h"


// CSettingDialog dialog

class CSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingDialog)
	virtual BOOL OnInitDialog();
public:
	CSettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_eUser;
	CEdit m_ePassword;
	CEdit m_eIP;
	CEdit m_ePort;
	CString m_sUser;
	CString m_sPassword;
	CString m_sIP;
	CString m_sPort;
	CString m_sBrowserVideo;
	//afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonBrowser();
};
