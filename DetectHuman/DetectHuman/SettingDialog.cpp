// SettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "SettingDialog.h"


// CSettingDialog dialog

IMPLEMENT_DYNAMIC(CSettingDialog, CDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
	, m_sUser(_T(""))
	, m_sPassword(_T(""))
	, m_sIP(_T(""))
	, m_sPort(_T(""))
	, m_sBrowserVideo(_T(""))
{
	m_sUser = CString("admin");
	m_sPassword = CString("123456");
	m_sIP = CString("192.168.1.224");
	m_sPort = CString("80");
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_eUser);
	DDX_Control(pDX, IDC_EDIT2, m_ePassword);
	DDX_Control(pDX, IDC_EDIT3, m_eIP);
	DDX_Control(pDX, IDC_EDIT4, m_ePort);
	DDX_Text(pDX, IDC_EDIT1, m_sUser);
	DDX_Text(pDX, IDC_EDIT2, m_sPassword);
	DDX_Text(pDX, IDC_EDIT3, m_sIP);
	DDX_Text(pDX, IDC_EDIT4, m_sPort);
	DDX_Text(pDX, IDC_EDIT_BROWSER_VIDEO, m_sBrowserVideo);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CSettingDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_BUTTON1, &CSettingDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CSettingDialog::OnBnClickedButtonBrowser)
END_MESSAGE_MAP()

BOOL CSettingDialog::OnInitDialog()
{

	UpdateData(0);
	return TRUE;
}
// CSettingDialog message handlers

void CSettingDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CSettingDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CSettingDialog::OnBnClickedButtonBrowser()
{
	// TODO: Add your control notification handler code here
	char szFilters[]= "Video Files (*.avi)|*.avi|Video Files (*.mp4)|*.mp4||";
	CFileDialog dlg(TRUE,NULL, NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
	if(dlg.DoModal() == IDOK)
	{
		CFileFind finder;
		bool isworking = finder.FindFile(dlg.GetPathName());
		if(isworking)
		{
			m_sBrowserVideo = dlg.GetPathName();
			UpdateData(0);
			finder.Close();
		}
		else
		{
			::MessageBox(this->GetSafeHwnd(),"File is not found!","Warning",0);
		}
	}
}
