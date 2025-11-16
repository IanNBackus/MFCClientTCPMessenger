#pragma once
#include <afxsock.h>

//define messages particular to this class so our dialog can hook up to them
#define WM_MYSOCKET_ONACCEPT	(WM_USER + 1)
#define WM_MYSOCKET_ONCONNECT	(WM_USER + 2)
#define WM_MYSOCKET_ONRECEIVE	(WM_USER + 3)
#define WM_MYSOCKET_ONCLOSE		(WM_USER + 4)

class ClientSocketController : CAsyncSocket
{

	public:

		//constructor/destructor
		ClientSocketController();
		virtual ~ClientSocketController();

		//variables
		HWND m_Hwnd;

		//methods
		virtual void OnAccept(int nErrorCode);
		virtual void OnConnect(int nErrorCode);
		virtual void OnReceive(int nErrorCode);
		virtual void OnClose(int nErrorCode);

};

