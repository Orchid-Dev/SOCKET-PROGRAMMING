
// CLIENTDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CLIENT.h"
#include "CLIENTDlg.h"
#include "afxdialogex.h"
#include <string>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4996)

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


// CCLIENTDlg dialog


CCLIENTDlg::CCLIENTDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCLIENTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, SERVER_ADDRESS);
	DDX_Control(pDX, IDC_EDIT_LOG, LOG);
	DDX_Control(pDX, IDC_EDIT_USERNAME, USERNAME);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, PASSWORD);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, LOGIN);
	DDX_Control(pDX, IDC_BUTTON_REGISTER, REGISTER);
	DDX_Control(pDX, IDC_EDIT_NUMBER_FILE, INDEX_FILE);
	DDX_Control(pDX, IDC_EDIT_FILEPATH_DOWNLOAD, FILE_PATH);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD, DOWNLOAD);
	DDX_Control(pDX, IDC_EDIT_FILEPATH_UPLOAD, FILE_PATH_UP);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD, UPLOAD);
	DDX_Control(pDX, IDC_EDIT_FILE, LIST_FILE);
}

BEGIN_MESSAGE_MAP(CCLIENTDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_SERVER, &CCLIENTDlg::OnIpnFieldchangedIpaddressServer)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_SERVER, &CCLIENTDlg::OnBnClickedButtonConnectServer)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CCLIENTDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CCLIENTDlg::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_SERVER, &CCLIENTDlg::OnBnClickedButtonDisconnectServer)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCLIENTDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_ENTER_INDEX, &CCLIENTDlg::OnBnClickedButtonEnterIndex)
	ON_EN_CHANGE(IDC_EDIT_FILE, &CCLIENTDlg::OnEnChangeEditFile)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CCLIENTDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CCLIENTDlg::OnBnClickedButtonUpload)
	ON_EN_CHANGE(IDC_EDIT_LOG, &CCLIENTDlg::OnEnChangeEditLog)
	ON_BN_CLICKED(IDC_BUTTON_LOGOFF, &CCLIENTDlg::OnBnClickedButtonLogoff)
END_MESSAGE_MAP()


// CCLIENTDlg message handlers

BOOL CCLIENTDlg::OnInitDialog()
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

	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FILEPATH_DOWNLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_DISCONNECT_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_LOGOFF)->EnableWindow(FALSE);
	

	SetTimer(1, 200, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCLIENTDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCLIENTDlg::OnPaint()
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
HCURSOR CCLIENTDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCLIENTDlg::OnIpnFieldchangedIpaddressServer(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CCLIENTDlg::OnBnClickedButtonConnectServer()
{
	// TODO: Add your control notification handler code here
	BYTE field0, field1, field2, field3;
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);

	SERVER_ADDRESS.GetAddress(field0, field1, field2, field3);

	if (field0 == 0 && field1 == 0 && field2 == 0  && field3 == 0) {
		WriteLog("IP Server can not be empty");
	}
	else {
		ipServer.clear();
		WriteLog("Waiting for connecting to server...");

		ipServer += to_string((int)field0);
		ipServer += '.';

		ipServer += to_string((int)field1);
		ipServer += '.';

		ipServer += to_string((int)field2);
		ipServer += '.';

		ipServer += to_string((int)field3);

		client.CreateDescriptor();

		if (client.GetServer(ipServer)) {
			if (client.Connect()) {

				WriteLog("connect successfully");

				GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);

				GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(TRUE);

				GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_CONNECT_SERVER)->EnableWindow(FALSE);

				GetDlgItem(IDC_BUTTON_DISCONNECT_SERVER)->EnableWindow(TRUE);
			}
			else {
				WriteLog("connect to server failed");
			}
		}
		else {
			WriteLog("connect to server failed");
		}
	}
}


void CCLIENTDlg::OnBnClickedButtonLogin()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);

	CString username;
	CString password;

	USERNAME.GetWindowTextW(username);
	PASSWORD.GetWindowTextW(password);

	if (username.IsEmpty() || password.IsEmpty()) {
		WriteLog("Username or Password can not be empty");
	}
	else {
		client.SendNumber(CLLOGIN);
		CT2CA pszConvertedAnsiUsername(username);
		string usn(pszConvertedAnsiUsername);

		CT2CA pszConvertedAnsiPassword(password);
		string pwd(pszConvertedAnsiPassword);

		int res = client.Login(usn, pwd);

		if (res == 2) {
			WriteLog("Username or Password is incorrect");
		}
		else if (res == 0){
			WriteLog("log in successfully");
			flag1 = true;
			
			GetDlgItem(IDC_BUTTON_LOGOFF)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(TRUE);

			//client.ReceiveListFile();
			//OnEnChangeEditFile();
		}
		else {
			flagDisconnect = 2;
			OnBnClickedButtonDisconnectServer();
			WriteLog("Server is stopped");
		}
	}
}

void CCLIENTDlg::OnBnClickedButtonRegister()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);

	CString username;
	CString password;

	USERNAME.GetWindowTextW(username);
	PASSWORD.GetWindowTextW(password);

	if (username.IsEmpty() || password.IsEmpty()) {
		WriteLog("Username or Password can not be empty");
	}
	else {
		client.SendNumber(CLREGISTER);
		
		USERNAME.GetWindowTextW(username);
		PASSWORD.GetWindowTextW(password);

		CT2CA pszConvertedAnsiUsername(username);
		string usn(pszConvertedAnsiUsername);

		CT2CA pszConvertedAnsiPassword(password);
		string pwd(pszConvertedAnsiPassword);

		int res = client.Register(usn, pwd);

		if (res == 2) {
			WriteLog("Username is already exists. Please try again");
		}
		else if (res == 0) {
			WriteLog("register and log in successfully ");
			GetDlgItem(IDC_BUTTON_LOGOFF)->EnableWindow(TRUE);

			flag1 = true;

			GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(TRUE);
		}
		else {
			flagDisconnect = 2;
			OnBnClickedButtonDisconnectServer();
			WriteLog("Server is stopped");
		}
	}
}

void CCLIENTDlg::OnBnClickedButtonDisconnectServer()
{
	// TODO: Add your control notification handler code here
	flag1 = false;
	flag2 = true;

	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);

	client.Disconnect(flagDisconnect);

	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOGOFF)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FILEPATH_DOWNLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_DISCONNECT_SERVER)->EnableWindow(FALSE);

	GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CONNECT_SERVER)->EnableWindow(TRUE);

	USERNAME.SetWindowTextW(CString(""));
	PASSWORD.SetWindowTextW(CString(""));
	SERVER_ADDRESS.SetWindowTextW(CString(""));
	LIST_FILE.SetWindowTextW(CString(""));
	LOG.SetWindowTextW(CString(""));
	INDEX_FILE.SetWindowTextW(CString(""));
	FILE_PATH.SetWindowTextW(CString(""));
	FILE_PATH_UP.SetWindowTextW(CString(""));
	WriteLog("disconnected... successfully");
	flagDisconnect = 1;
}

void CCLIENTDlg::WriteLog(const char *log) {
	CString msgLog;

	LOG.GetWindowTextW(msgLog);

	msgLog += log;
	msgLog += "\r\n";

	LOG.SetWindowTextW(msgLog);
}

void CCLIENTDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here
	LOG.SetWindowTextW(CString(""));
}


void CCLIENTDlg::OnBnClickedButtonEnterIndex()
{
	// TODO: Add your control notification handler code here
	CString numberText;
	INDEX_FILE.GetWindowTextW(numberText);

	flag2 = false;
	
	if (numberText.IsEmpty()) {
		WriteLog("Index of File can not be empty");
	}
	else {
		indexFile = _ttoi(numberText);
		client.SendNumber(CLDOWNLOAD);

		if (!client.GetFile(indexFile)) {
			WriteLog("Can not download file with this index");
		}
		else {
			GetDlgItem(IDC_EDIT_FILEPATH_DOWNLOAD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(TRUE);

			GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);
		}
	}
}


void CCLIENTDlg::OnEnChangeEditFile()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	if (flag1 && flag2) {
		LIST_FILE.SetWindowTextW(CString(""));
		client.SendNumber(CLGETLISTFILE);
		client.ReceiveListFile();

		vector<string> Files = client.GetFiles();
		string text;

		for (int i = 0; i < Files.size(); ++i) {
			text += to_string(i + 1);
			text += ". ";
			text += Files[i];
			text += "\r\n";
		}

		char msg[1000];
		client.ReceiveMessage(msg);

		if (string(msg).compare("server is stopped") == 0) {
			flagDisconnect = 2;
			OnBnClickedButtonDisconnectServer();
			WriteLog(msg);
			return;
		}

		if (string(MSG).compare(msg) != 0) {
			WriteLog(msg);
			memcpy(MSG, msg, 1000);
		}

		LIST_FILE.SetWindowTextW(CString(text.c_str()));
	}
}


void CCLIENTDlg::OnBnClickedButtonDownload()
{
	// TODO: Add your control notification handler code here
	vector<string> Files = client.GetFiles();

	CString filePath;
	FILE_PATH.GetWindowTextW(filePath);

	CT2CA pszConvertedAnsiFilePath(filePath);
	string fp(pszConvertedAnsiFilePath);
	string fpTemp;

	bool flag = 1;
	int count = 0;
	string fileTemp;
	string chs = "_(";

	while (1) {
		fpTemp = fp;

		fileTemp = Files[indexFile - 1];

		if (count > 0)
			if (fileTemp.find('.') != string::npos)
				fileTemp.insert(fileTemp.find('.'), string(chs + std::to_string(count) + ')'));
			else
				fileTemp += string(chs + std::to_string(count) + ')');

		if (!fpTemp.empty()) {
			if (fpTemp.back() == '\\')
				fpTemp = fpTemp + fileTemp;
			else
				fpTemp = fpTemp + "\\" + fileTemp;
		}
		else
			fpTemp = string(fileTemp);

		FILE* file = fopen(fpTemp.c_str(), "rb");

		if (file == NULL) {
			break;
		}

		fclose(file);
		++count;
	}

	fp = fpTemp;

	string log = "Downloading ";
	log = log + Files[indexFile - 1] + " ==> " + fileTemp + "... ";

	FILE* file = fopen(fp.c_str(), "wb");

	if (file == NULL) {
		WriteLog("Can not open file at this file path");
		return;
	}

	fclose(file);

	if (client.createFile(indexFile, fp)) {
		log += " successfully";
		WriteLog(log.c_str());

		GetDlgItem(IDC_EDIT_FILEPATH_DOWNLOAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(TRUE);
	}
	else {
		log += " failed";
		WriteLog(log.c_str());
	}

	flag2 = true;
}


void CCLIENTDlg::OnBnClickedButtonUpload()
{
	// TODO: Add your control notification handler code here 
	flag2 = false;

	CString filePath;
	FILE_PATH_UP.GetWindowTextW(filePath);

	CT2CA pszConvertedAnsiFilePath(filePath);
	string fp(pszConvertedAnsiFilePath);

	if (fp.empty()) {
		WriteLog("This file path can not be empty()");
		return;
	}
	else {
		FILE* file = fopen(fp.c_str(), "rb");
		if (file == NULL) {
			WriteLog("Can not open file at this file path");
			return;
		}

		fclose(file);
		client.SendNumber(CLUPLOAD);

		string log = "Uploading ";

		char name[100];
		int i = fp.length() - 1;
		int j = 0;

		while (i >= 0 && fp[i] != '\\') {
			name[j++] = fp[i--];
		}

		name[j] = '\0';

		strrev(name);

		log += name;
		log += "...";

		client.UploadFile(fp);

		log += " successfully";
		WriteLog(log.c_str());
	}

	flag2 = true;
}

void CCLIENTDlg::OnTimer(UINT_PTR nIDEvent)
{
	OnEnChangeEditFile();
	CDialog::OnTimer(nIDEvent);
}

void CCLIENTDlg::OnEnChangeEditLog()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code heres

}


void CCLIENTDlg::OnBnClickedButtonLogoff()
{
	// TODO: Add your control notification handler code here
	flag1 = false;
	flag2 = true;

	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);

	flagDisconnect = 3;

	client.Disconnect(flagDisconnect);

	GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_ENTER_INDEX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_NUMBER_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FILEPATH_DOWNLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_FILEPATH_UPLOAD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UPLOAD)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_DISCONNECT_SERVER)->EnableWindow(FALSE);

	GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT_SERVER)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_LOGOFF)->EnableWindow(FALSE);

	USERNAME.SetWindowTextW(CString(""));
	PASSWORD.SetWindowTextW(CString(""));
	LIST_FILE.SetWindowTextW(CString(""));
	LOG.SetWindowTextW(CString(""));
	INDEX_FILE.SetWindowTextW(CString(""));
	FILE_PATH.SetWindowTextW(CString(""));
	FILE_PATH_UP.SetWindowTextW(CString(""));
	WriteLog("log off... successfully");
	flagDisconnect = 1;

	client.CreateDescriptor();

	if (client.GetServer(ipServer)) {
		if (client.Connect()) {
			GetDlgItem(IDC_EDIT_USERNAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE);

			GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(TRUE);

			GetDlgItem(IDC_IPADDRESS_SERVER)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_CONNECT_SERVER)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_DISCONNECT_SERVER)->EnableWindow(TRUE);
		}
		else {
			WriteLog("connect to server failed");
		}
	}
	else {
		WriteLog("connect to server failed");
	}
}
