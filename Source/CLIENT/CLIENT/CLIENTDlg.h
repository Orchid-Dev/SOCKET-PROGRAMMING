
// CLIENTDlg.h : header file
//

#pragma once


// CCLIENTDlg dialog
class CCLIENTDlg : public CDialogEx
{
// Construction
public:
	CCLIENTDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

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
	ClientCore client;

	std::string ipServer;

	int indexFile;
	bool flag1 = false, flag2 = true;
	int flagDisconnect = 1;
	char MSG[1000];

	CIPAddressCtrl SERVER_ADDRESS;
	CEdit LOG;
	CEdit USERNAME;
	CEdit PASSWORD;
	CButton LOGIN;
	CButton REGISTER;
	CEdit INDEX_FILE;
	CEdit FILE_PATH;
	CButton DOWNLOAD;
	CEdit FILE_PATH_UP;
	CButton UPLOAD;
	CEdit LIST_FILE;

	afx_msg void OnIpnFieldchangedIpaddressServer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonConnectServer();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonDisconnectServer();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonEnterIndex();
	afx_msg void OnEnChangeEditFile();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonUpload();

	void WriteLog(const char* log);
	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditLog();
	afx_msg void OnBnClickedButtonLogoff();
};
