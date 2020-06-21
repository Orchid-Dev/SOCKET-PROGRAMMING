
// SERVERDlg.h : header file
//
#include <list>

#pragma once


// CSERVERDlg dialog
class CSERVERDlg : public CDialogEx
{
// Construction
public:
	CSERVERDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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
	ServerCore server;

	bool flag = false;
	
	CEdit LOG;
	CEdit FILE;
	CEdit ONLINE;

	CString logText;

	std::pair<std::string, std::string> messageLog;
	std::list<std::string> onlines;


	afx_msg void OnBnClickedButtonStartServer();
	afx_msg void OnStnClickedStaticOnline();
	afx_msg void OnEnChangeEditFile();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnEnChangeEditLog();

	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditOnline();
	
	afx_msg void OnBnClickedButtonClear();
};
