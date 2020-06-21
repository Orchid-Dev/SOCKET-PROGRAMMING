
// SERVERDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SERVER.h"
#include "SERVERDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <thread>
#include <fstream>
#include <iostream>


using namespace std;

#pragma warning(disable: 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSERVERDlg dialog

CSERVERDlg::CSERVERDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_FILE, FILE);
	DDX_Control(pDX, IDC_EDIT_LOG, LOG);
	DDX_Control(pDX, IDC_EDIT_ONLINE, ONLINE);
}

BEGIN_MESSAGE_MAP(CSERVERDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START_SERVER, &CSERVERDlg::OnBnClickedButtonStartServer)

	ON_STN_CLICKED(IDC_STATIC_ONLINE, &CSERVERDlg::OnStnClickedStaticOnline)
	ON_EN_CHANGE(IDC_EDIT_FILE, &CSERVERDlg::OnEnChangeEditFile)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CSERVERDlg::OnBnClickedButtonExit)
	ON_EN_CHANGE(IDC_EDIT_LOG, &CSERVERDlg::OnEnChangeEditLog)
	ON_EN_CHANGE(IDC_EDIT_ONLINE, &CSERVERDlg::OnEnChangeEditOnline)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSERVERDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CSERVERDlg message handlers

BOOL CSERVERDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	/*HMENU hSysMenu = ::GetSystemMenu(m_hWnd, FALSE);
	if (hSysMenu)
		::EnableMenuItem(hSysMenu, SC_CLOSE, (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND));*/

	GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(FALSE);
	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSERVERDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSERVERDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void thread_server(ServerCore& server) {
	server.Start();
	server.CreateDescriptor();
	server.Bind();
	server.run();
} 

void CSERVERDlg::OnBnClickedButtonStartServer()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_START_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(TRUE);

	LOG.GetWindowTextW(logText);

	if (logText.IsEmpty())
		logText += "Run server....successfully \r\nServer is running";
	else
		logText += "\r\nRun server....successfully \r\nServer is running";

	LOG.SetWindowTextW(logText);
	
	flag = true;

	thread t_Server = thread(thread_server, std::ref(server));

	t_Server.detach();
}

void CSERVERDlg::OnStnClickedStaticOnline()
{
	// TODO: Add your control notification handler code here
}


void CSERVERDlg::OnEnChangeEditFile()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (flag) {
		vector<string> Files = server.GetFiles();
		string text;

		for (int i = 0; i < Files.size(); ++i) {
			text += to_string(i + 1);
			text += ". ";
			text += Files[i];
			text += "\r\n";
		}

		FILE.SetWindowTextW(CString(text.c_str()));
	}
}

void CSERVERDlg::OnBnClickedButtonExit()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_START_SERVER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(FALSE);

	flag = false;
	FILE.SetWindowTextW(CString(""));
	ONLINE.SetWindowTextW(CString(""));
	
	LOG.GetWindowTextW(logText);
	if (logText.IsEmpty())
		logText += "Stop Server... successfully";
	else 
		logText += "\r\nStop Server... successfully";

	LOG.SetWindowTextW(logText);

	server.SetMessage("server is stopped");
	server.Stop();

	onlines.clear();
}

void CSERVERDlg::OnEnChangeEditLog()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if (flag) {
		LOG.GetWindowTextW(logText);

		messageLog = server.getMessage();

		string msgLog = messageLog.first + messageLog.second;

		if (logText.IsEmpty()) {
			logText = msgLog.c_str();
		}
		else {
			if (!msgLog.empty()) {
				logText += "\r\n";
				logText += (msgLog.c_str());
			}
		}

		if (messageLog.second.compare(" log in") == 0)
			onlines.push_back(messageLog.first);

		if (messageLog.second.compare(" register + log in") == 0)
			onlines.push_back(messageLog.first);

		if (messageLog.second.compare(" log off") == 0)
			onlines.remove(messageLog.first);

		if (messageLog.second.compare(" disconnect") == 0)
			onlines.remove(messageLog.first);

		LOG.SetWindowTextW(logText);
	}
}

void CSERVERDlg::OnTimer(UINT_PTR nIDEvent)
{
	OnEnChangeEditLog();
	OnEnChangeEditOnline();
	OnEnChangeEditFile();
	CDialog::OnTimer(nIDEvent);
}

void CSERVERDlg::OnEnChangeEditOnline()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if (flag) {
		string listOnline;

		for (auto u : onlines) {
			listOnline += u;
			listOnline += "\r\n";
		}

		ONLINE.SetWindowTextW(CString(listOnline.c_str()));
	}
}


void CSERVERDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	LOG.SetWindowTextW(CString(""));
}
