
// MFCApplication1Dlg.h : header file
//

#pragma once

#include "ClientSocketController.h"

// CMFCApplication1Dlg dialog
class CMFCApplication1Dlg : public CDialogEx
{
// Construction
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CString _messageEdit;
public:
	afx_msg void OnSendClicked();
private:
	CListBox _messageListControl;
	CString _ip;
	CString _port;

	//client socket class
	ClientSocketController clientController;

public:
	afx_msg void OnConnectClicked();
};
