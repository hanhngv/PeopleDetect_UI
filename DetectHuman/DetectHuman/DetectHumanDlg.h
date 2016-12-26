// DetectHumanDlg.h : header file
//

#pragma once
#include <opencv2/opencv.hpp>
#include <time.h>
#include <vector>
using namespace cv;
using namespace std;
#include "SettingDialog.h"
#include "MInputFrame.h"
#include "MyVideo.h"
#include "afxwin.h"
#include "MPeopleCounting.h"
// CDetectHumanDlg dialog
class CDetectHumanDlg : public CDialog
{
// Construction
public:
	CDetectHumanDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DETECTHUMAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPlay();
private:
	static DWORD WINAPI VideoThreadFunction( LPVOID lpParam );
	int RunVideo();
	HANDLE m_hThVideo;
	static DWORD WINAPI CameraThreadFunction( LPVOID lpParam );
	int RunCamera();
	static DWORD WINAPI VisualizeThreadFunction( LPVOID lpParam );
	int RunVisualize();
	CSettingDialog *m_pSettingDlg;
public:
	bool m_brun;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedButtonVideo();
	CComboBox m_cbSource;
	afx_msg void OnCbnSelchangeComboSource();
	bool m_bplay;
	bool m_bshowvideo;
	vector<Mat> m_vectorFrame;
	UINT m_uPeopleSize;
	UINT m_uGate;
	int m_uEnter;
	int m_uLeave;
	CStatic m_sShowVideo;
	CString m_path;
private:
	MPeopleCounting m_CountingModal;
	MInputFrame *m_InputFrame;
public:
	bool m_bStartCount;
	CButton m_btnStart;
	afx_msg void OnBnClickedButtonStartDetect();
};
