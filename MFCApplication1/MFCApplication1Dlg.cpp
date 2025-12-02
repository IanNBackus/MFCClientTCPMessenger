
// MFCApplication1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg dialog
CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, _messageEdit(_T(""))
	, _ip(_T(""))
	, _port(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//link up message data
	DDX_Text(pDX, Message_Edit, _messageEdit);
	DDX_Text(pDX, IP_Edit, _ip);
	DDX_Text(pDX, Port_Edit, _port);
	DDX_Control(pDX, Message_List, _messageListControl);
}

//mfc message mapping (event to eventhandler hookups)
BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)

	//messages implemented by default template
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//messages from ui controls
	ON_BN_CLICKED(Send_Button, &CMFCApplication1Dlg::OnSendClicked)
	ON_BN_CLICKED(Connect_Button, &CMFCApplication1Dlg::OnConnectClicked)

	//messages from socketcontroller class
	ON_MESSAGE(WM_SOCKETCONTROLLER_ONCLOSE, OnSocketControllerDisconnect)
	ON_MESSAGE(WM_SOCKETCONTROLLER_ONRECEIVE, OnSocketControllerReceive)
	ON_MESSAGE(WM_SOCKETCONTROLLER_ONCONNECT, OnSocketControllerConnect)

END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	_clientController.Hwnd = m_hWnd;

	UpdateMessageList(_T("Welcome Client"));

	//set bold title fonts
	CFont m_font;
	m_font.CreateFont(10, 0, 0, 0, FW_BOLD, 0, 1, 0, 0, 0, 0, 0, 0, _T("Arial"));
	GetDlgItem(Message_Window_Title)->SetFont(&m_font);
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
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
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//add a string of text to the listbox
void CMFCApplication1Dlg::UpdateMessageList(CString textToAdd) {

	//add new text to the list (will go to the bottom)
	_messageListControl.AddString(textToAdd);

	//move the scroll bar to keep the new messages visible
	_messageListControl.SetTopIndex(_messageListControl.GetCount() - 1);

	return;

}

//Respond to a message send button clicked event from the ui
void CMFCApplication1Dlg::OnSendClicked()
{

	//perform data exchange
	UpdateData(TRUE);

	//verify the connection prior to making a send attempt
	if (_clientController.ConnectionStatus != SocketController::ConnectionStates::Connected) {
		UpdateMessageList(_T("Connection must be made prior to sending..."));
		return;
	}
	
	//convert CString from UI into a format acceptable by CAsyncSocket.Send(), in this case a const char*
	CT2A stringConverter(_messageEdit);
	char* convertedTextData = stringConverter;

	//send over network connction
	_clientController.Send(convertedTextData, (int)strlen(convertedTextData));
	//_clientController.Send(CT2A(_messageEdit), strlen(CT2A(_messageEdit)));

	//update ui
	UpdateMessageList(_T("Me: ") + _messageEdit);

	return;

}

//Respond to a connect button clicked event from the ui
void CMFCApplication1Dlg::OnConnectClicked()
{
	
	//perform data exchange
	UpdateData(TRUE);

	//don't allow connection attempts while already connected or while attempting connection
	if (_clientController.ConnectionStatus != SocketController::ConnectionStates::Disconnected) {
		UpdateMessageList(_T("Connection already initiated..."));
		return;
	}

	//check for valid port
	errno = 0;
	TCHAR* end;
	UINT portConversion = _tcstoul(_port, &end, 10);

	//check for valid conversion to UINT
	if ((errno != 0) || (end == _port)) 
	{
		UpdateMessageList(_T("Entered port was invalid..."));
		return;
	}

	//check for successful client creation
	if (!_clientController.Create())
	{
		UpdateMessageList(_T("Socket creation failed..."));
		return;
	}

	//check for successful connection
	if (!_clientController.Connect(_ip, portConversion))
	{
		UpdateMessageList(_T("Connecting..."));
	}
	else 
	{
		UpdateMessageList(_T("Connected..."));
	}

	CString notification;

	//inform user of local network credentials
	UINT localPort = 0;
	CString localIp;
	_clientController.GetSockName(localIp, localPort);
	notification.Format(_T("Client is IP: %s, Port: %u"), (LPCTSTR)localIp, localPort);
	UpdateMessageList(notification);

	//inform user of server network credentials
	notification.Format(_T("Connecting to IP: %s, Port: %s"), (LPCTSTR)_ip, (LPCTSTR)_port);
	UpdateMessageList(notification);

	//keep track of connection attempt
	_clientController.ConnectionStatus = SocketController::ConnectionStates::Connecting;

	return;

}

//Respond to a message receive event from the clientsocketcontroller class
LRESULT CMFCApplication1Dlg::OnSocketControllerReceive(WPARAM, LPARAM)
{

	char transmissionBuffer[256] = {0};
	CString convertedTransmission;

	//pull data from socket leaving 1 btye for a null terminator
	int transmissionLength = _clientController.Receive(transmissionBuffer, sizeof(transmissionBuffer) - 1);

	//check if there's anything to convert
	if (transmissionLength <= 0) {

		return 0;

	}

	//convert into CString
	transmissionBuffer[transmissionLength] = '\0';
	convertedTransmission = transmissionBuffer;

	//update ui
	UpdateMessageList(_T("Server: ") + convertedTransmission);

	return 0;

}

//handle the result of an attempted connection
LRESULT CMFCApplication1Dlg::OnSocketControllerConnect(WPARAM wParam, LPARAM)
{

	if (wParam != 0) 
	{
		UpdateMessageList(_T("Connection Attempt Failed..."));
		_clientController.ConnectionStatus = SocketController::ConnectionStates::Disconnected;
		_clientController.ShutDown();
		_clientController.Close();
		return 0;
	}

	_clientController.ConnectionStatus = SocketController::ConnectionStates::Connected;
	UpdateMessageList(_T("Connection Success..."));

	return 0;

}

//Respond to a disconnect event from the clientsocketcontroller class
LRESULT CMFCApplication1Dlg::OnSocketControllerDisconnect(WPARAM, LPARAM)
{

	UpdateMessageList(_T("Server Closed..."));

	_clientController.ShutDown();
	_clientController.Close();
	_clientController.ConnectionStatus = SocketController::ConnectionStates::Disconnected;

	return 0;

}