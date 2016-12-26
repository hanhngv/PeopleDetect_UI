// DetectHumanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DetectHuman.h"
#include "DetectHumanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHECK_ENABLE_BUTTON 1
#define CHECK_SHOW_STATUS	2
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDetectHumanDlg dialog




CDetectHumanDlg::CDetectHumanDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDetectHumanDlg::IDD, pParent)
, m_uPeopleSize(0)
, m_uGate(0)
, m_uEnter(0)
, m_uLeave(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThVideo = NULL;
	m_brun = false;
	m_pSettingDlg = new CSettingDialog(this);
	//m_video = NULL;
	m_bplay = false;
	m_bshowvideo = false;
	m_InputFrame = NULL;
	m_bStartCount = 0;
}

void CDetectHumanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOURCE, m_cbSource);
	DDX_Text(pDX, IDC_EDIT_PEOPLE_SIZE, m_uPeopleSize);
	DDX_Text(pDX, IDC_EDIT_GATE, m_uGate);
	DDX_Text(pDX, IDC_EDIT_ENTER, m_uEnter);
	DDX_Text(pDX, IDC_EDIT_LEAVE, m_uLeave);
	DDX_Control(pDX, IDC_STATIC_SHOW_VIDEO, m_sShowVideo);
	DDX_Control(pDX, IDC_BUTTON_START_DETECT, m_btnStart);
}

BEGIN_MESSAGE_MAP(CDetectHumanDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CDetectHumanDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDetectHumanDlg::OnBnClickedPlay)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CDetectHumanDlg::OnBnClickedSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_SOURCE, &CDetectHumanDlg::OnCbnSelchangeComboSource)
	//ON_BN_CLICKED(IDC_BUTTON2, &CDetectHumanDlg::OnBnClickedStartCount)
	ON_BN_CLICKED(IDC_BUTTON_START_DETECT, &CDetectHumanDlg::OnBnClickedButtonStartDetect)
END_MESSAGE_MAP()


// CDetectHumanDlg message handlers

BOOL CDetectHumanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_uPeopleSize = 50;
	m_uGate = 150;
	m_cbSource.AddString("Camera");
	m_cbSource.AddString("Video");
	m_cbSource.SetCurSel(1);
	UpdateData(0);
	SetTimer(CHECK_SHOW_STATUS,500,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDetectHumanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDetectHumanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDetectHumanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDetectHumanDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDetectHumanDlg::OnBnClickedPlay()
{
	if(!m_bplay)
	{
		int nsel = m_cbSource.GetCurSel();
		CFileFind finder;
		switch(nsel)
		{
		case 0:
			{
				m_bplay = true;
				m_brun = true;
				//m_hThVideo = CreateThread( NULL, 0, CameraThreadFunction, this, 0, NULL); 
				//if(isworking)
				{
					namedWindow("Video", 0 );
					cvResizeWindow("Video", 640, 480);

					HWND hwnd = (HWND)cvGetWindowHandle("Video");
					HWND hwndParent = ::GetParent(hwnd);
					::ShowWindow(hwndParent,SW_HIDE);
					HWND newhwnd = m_sShowVideo.GetSafeHwnd();
					::SetParent(hwnd,newhwnd);
					//char path[2048];
					//sprintf(path,m_pSettingDlg->m_sBrowserVideo);
					//string stream_address = "http://admin:123456@192.168.248.190:8090/video.cgi?.mjpg";
					char stream_address[2048];
					sprintf(stream_address,"http://%s:%s@%s:%s/videostream.cgi?resolution=640x480&req_fps=30&.mjpg",
						m_pSettingDlg->m_sUser,
						m_pSettingDlg->m_sPassword,
						m_pSettingDlg->m_sIP,
						m_pSettingDlg->m_sPort);
					//string stream_address = 					

					if(m_InputFrame)
					{
						delete m_InputFrame;
						m_InputFrame = NULL;
					}					
					m_InputFrame = new MInputFrameFormCamera();
					if(!m_InputFrame->init(stream_address)){
						AfxMessageBox(_T("Fail to connect the camera!"));
						return;
					}

					m_bplay = true;
					m_brun = true;
					
					// set mac dinh
					//m_CountingModal.begin(m_InputFrame,m_uPeopleSize,m_uGate);
					m_CountingModal.begin(m_InputFrame,m_uPeopleSize,m_InputFrame->getFrameSize().height/2);
					m_hThVideo = CreateThread( NULL, 0, VisualizeThreadFunction, this, 0, NULL); 
					GetDlgItem(IDOK)->SetWindowText("Stop");
					m_bshowvideo = true;
				}

				GetDlgItem(IDOK)->SetWindowText("Stop");
			}break;
		case 1:
			{
				CFileFind finder; 
				bool isworking = finder.FindFile(m_pSettingDlg->m_sBrowserVideo);
				if(!isworking)
				{
					char szFilters[]= "Video Files (*.avi)|*.avi|Video Files (*.mp4)|*.mp4||";
					CFileDialog dlg(TRUE,NULL, NULL, OFN_FILEMUSTEXIST| OFN_HIDEREADONLY, szFilters, this);
					if(dlg.DoModal() == IDOK)
					{
						isworking = finder.FindFile(dlg.GetPathName());
						if(isworking)
						{
							finder.Close();
							m_pSettingDlg->m_sBrowserVideo = m_path = dlg.GetPathName();
						}
					}
				}
				finder.Close();

				if(isworking)
				{
					namedWindow("Video", 0 );
					cvResizeWindow("Video", 640, 480);

					HWND hwnd = (HWND)cvGetWindowHandle("Video");
					HWND hwndParent = ::GetParent(hwnd);
					::ShowWindow(hwndParent,SW_HIDE);
					HWND newhwnd = m_sShowVideo.GetSafeHwnd();
					::SetParent(hwnd,newhwnd);
					char path[2048];
					sprintf(path,m_pSettingDlg->m_sBrowserVideo);
					if(m_InputFrame)
					{
						delete m_InputFrame;
						m_InputFrame = NULL;
					}
					m_InputFrame = new MInputFromVideo();
					m_bplay = true;
					m_brun = true;
					m_InputFrame->init(path);
					// set mac dinh
					//m_CountingModal.begin(m_InputFrame,m_uPeopleSize,m_uGate);
					m_CountingModal.begin(m_InputFrame,m_uPeopleSize,m_InputFrame->getFrameSize().height/2);
					m_hThVideo = CreateThread( NULL, 0, VisualizeThreadFunction, this, 0, NULL); 
					GetDlgItem(IDOK)->SetWindowText("Stop");
					m_bshowvideo = true;
				}
			}
			break;
		}
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(0);
		SetTimer(CHECK_ENABLE_BUTTON,50,NULL);
		m_bplay = false;
	}
}


DWORD CDetectHumanDlg::VideoThreadFunction( LPVOID lpParam )
{
	CDetectHumanDlg* dlg = (CDetectHumanDlg*)lpParam;
	return dlg->RunVideo();
}

int CDetectHumanDlg::RunVideo()
{
	return 0;
}

void CDetectHumanDlg::OnDestroy()
{
	m_brun = false;
	if(m_hThVideo)
	{
		if(WaitForSingleObject(m_hThVideo, 1000) != WAIT_OBJECT_0)
			TerminateThread(m_hThVideo, 1);
		CloseHandle(m_hThVideo);
		m_hThVideo = NULL;
	}
	if(m_pSettingDlg)
	{
		delete m_pSettingDlg;
		m_pSettingDlg = NULL;
	}
	if(m_InputFrame)
	{
		delete m_InputFrame;
		m_InputFrame = NULL;
	}
	/*if(m_video)
	{
	delete m_video;
	m_video = NULL;
	}*/
}

void CDetectHumanDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == CHECK_ENABLE_BUTTON)
	{
		m_bStartCount = false;
		cvDestroyWindow("Video");
		m_brun = false;
		/*if(m_InputFrame)
		{
			delete m_InputFrame;
			m_InputFrame = NULL;
		}*/
		if(m_hThVideo)
		{
			if(WaitForSingleObject(m_hThVideo, 1000) != WAIT_OBJECT_0)
				TerminateThread(m_hThVideo, 1);
			CloseHandle(m_hThVideo);
			m_hThVideo = NULL;
		}
		if(m_bshowvideo)
		{
			m_bshowvideo = false;
			
		}
		GetDlgItem(IDOK)->SetWindowText("Play");
		GetDlgItem(IDOK)->EnableWindow(1);
		KillTimer(CHECK_ENABLE_BUTTON);
		m_uEnter = 0;
		m_uLeave = 0;
		m_bStartCount = 0;
		m_btnStart.SetWindowText(m_bStartCount?"STOP DETECT":"START");
	}
	if(nIDEvent == CHECK_SHOW_STATUS)
	{
		if(m_brun)
		{
			if(!m_bshowvideo)
			{
				CString s;
				s.Format(_T("User: %s, IP: %s, Port: %s"),m_pSettingDlg->m_sUser,
					m_pSettingDlg->m_sIP, m_pSettingDlg->m_sPort);
				GetDlgItem(IDC_STATIC_PATH)->SetWindowText(s);
			}
			else
			{
				if(m_pSettingDlg->m_sBrowserVideo.IsEmpty())
				{
					GetDlgItem(IDC_STATIC_PATH)->SetWindowText(_T(""));
				}
				else
				{
					CString s;
					s.Format(_T("Video path: %s"),m_pSettingDlg->m_sBrowserVideo);
					GetDlgItem(IDC_STATIC_PATH)->SetWindowText(s);
				}
			}
		}
		else
		{
			GetDlgItem(IDC_STATIC_PATH)->SetWindowText(_T(""));
		}
	}
}
void CDetectHumanDlg::OnBnClickedSetting()
{
	m_pSettingDlg->DoModal();
}

void CDetectHumanDlg::OnBnClickedButtonVideo()
{
	
}

void CDetectHumanDlg::OnCbnSelchangeComboSource()
{
	// TODO: Add your control notification handler code here
}

DWORD CDetectHumanDlg::CameraThreadFunction( LPVOID lpParam )
{
	CDetectHumanDlg* dlg = (CDetectHumanDlg*)lpParam;
	return dlg->RunCamera();
}

DWORD CDetectHumanDlg::VisualizeThreadFunction( LPVOID lpParam )
{
	CDetectHumanDlg* dlg = (CDetectHumanDlg*)lpParam;
	return dlg->RunVisualize();
}
int CDetectHumanDlg::RunVisualize()
{
	Mat t_image;
	
	while(m_CountingModal.b_running)
	{
		if(!m_brun)
		{
			m_CountingModal.stop();
			break;
		}
		m_CountingModal.getCountResult(m_uEnter,m_uLeave);
		if(m_bStartCount)
		{
			this->UpdateData(0);
		}
		m_CountingModal.getIlluFrame(t_image);
		if(!t_image.empty()){
			imshow("Video",t_image);
			waitKey(1);
		}
		Sleep(50);
	}
	if(m_brun)
	{
		m_brun = false;
		GetDlgItem(IDOK)->EnableWindow(0);
		SetTimer(CHECK_ENABLE_BUTTON,50,NULL);
		m_bplay = false;
	}
	return 0;
}

int CDetectHumanDlg::RunCamera()
{
	time_t startTime, endTime;
	double fps = 0, sec = 0;
	int i = 0;

	CvCapture* capture = NULL;
	char camera[2048];
	sprintf(camera,"http://%s:%s@%s:%s/videostream.cgi?resolution=640x480&req_fps=30&.mjpg",
		m_pSettingDlg->m_sUser,
		m_pSettingDlg->m_sPassword,
		m_pSettingDlg->m_sIP,
		m_pSettingDlg->m_sPort);

	while(camera[i] != NULL)
	{
		if(i >= 10000)
		{
			m_brun = false;
			m_hThVideo = NULL;
			return 0;
		}
		Sleep(500);
		i+=500;
	}
	capture = cvCaptureFromFile(camera);

	//check if image from IP capture
	if (!capture)
	{
		AfxMessageBox("cvCaptureFromFile failed!");
		m_brun = false;
		m_hThVideo = NULL;
		return 0;
	}
	assert( capture != NULL );

	cvNamedWindow( "Camera", CV_WINDOW_NORMAL );
	IplImage* frame;

	//start counter for frame rate
	time(&startTime);
	int counter=0;

	//=========================main loop=======================
	while(m_brun)
	{
		frame = cvQueryFrame(capture);
		/*break when out of fram*/
		if(!frame ) break;

		//calculates frame rate
		time(&endTime);
		++counter;
		sec=difftime(endTime,startTime);
		fps=counter/sec;

		//do not display frame rate every frame
		if(counter%100==0)
		{
			CString s;
			s.Format(_T("FPS: %d"),cvRound(fps));
			this->GetDlgItem(IDC_STATIC_FPS)->SetWindowText(s);
			//printf("%f\n",fps);
		}

		cvShowImage("Camera", frame);
		/*ASCII 27 = esc*/
		char c = cvWaitKey(30); //8 not good and neither is 20
		/* if( c == 27 )
		{
		m_brun = false;
		m_hThVideo = NULL;
		break;
		}*/
	}

	this->GetDlgItem(IDC_STATIC_FPS)->SetWindowText(_T("FPS: 0"));
	m_brun = false;
	//release memory
	cvReleaseCapture(&capture);
	cvDestroyWindow("Camera");
	return 0;
}
void CDetectHumanDlg::OnBnClickedButtonStartDetect()
{
	if(m_bplay)
	{
		m_bStartCount = !m_bStartCount;
		if(m_bStartCount)
		{
			UpdateData(0);
		}
		m_btnStart.SetWindowText(m_bStartCount?"STOP DETECT":"START");
	}
}

