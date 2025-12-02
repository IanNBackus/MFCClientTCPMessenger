#pragma once
#include <afxsock.h>

//define messages particular to this class so our dialog can hook up to them
#define WM_SOCKETCONTROLLER_ONACCEPT	(WM_USER + 1)
#define WM_SOCKETCONTROLLER_ONCONNECT	(WM_USER + 2)
#define WM_SOCKETCONTROLLER_ONRECEIVE	(WM_USER + 3)
#define WM_SOCKETCONTROLLER_ONCLOSE		(WM_USER + 4)

class SocketController : public CAsyncSocket
{

	public:

		//enum for connection state
		enum class ConnectionStates
		{
			Disconnected,
			Connecting,
			Connected
		};

		//constructor/destructor
		SocketController();
		virtual ~SocketController();

		//variables
		HWND Hwnd;
		ConnectionStates ConnectionStatus;
		//bool IsConnected;

		//methods
		virtual void OnAccept(int nErrorCode);
		virtual void OnConnect(int nErrorCode);
		virtual void OnReceive(int nErrorCode);
		virtual void OnClose(int nErrorCode);

};

