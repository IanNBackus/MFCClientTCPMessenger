#include "pch.h"
#include "ClientSocketController.h"


ClientSocketController::ClientSocketController()
{
	TRACE("\n\nIn parameterless Constructor for ClientSocketController\n\n");

	//we'll set this in the dialog class
	m_Hwnd = nullptr;
}

ClientSocketController::~ClientSocketController()
{
	//worlds emptiest destructor
	TRACE("\n\nDestructing ClientSocketController\n\n");
}


void ClientSocketController::OnAccept(int nErrorCode)
{

	//check if the window reference is acceptable
	if (::IsWindow(m_Hwnd))
	{
		//send message (event) to the window
		::PostMessage(m_Hwnd, WM_MYSOCKET_ONACCEPT, (WPARAM)nErrorCode, 0);
	}

	CAsyncSocket::OnAccept(nErrorCode);

}


void ClientSocketController::OnConnect(int nErrorCode)
{

	//check if the window reference is acceptable
	if (::IsWindow(m_Hwnd))
	{
		//send message (event) to the window
		::PostMessage(m_Hwnd, WM_MYSOCKET_ONCONNECT, (WPARAM)nErrorCode, 0);
	}

	CAsyncSocket::OnConnect(nErrorCode);

}


void ClientSocketController::OnReceive(int nErrorCode)
{

	//check if the window reference is acceptable
	if (::IsWindow(m_Hwnd))
	{
		//send message (event) to the window
		::PostMessage(m_Hwnd, WM_MYSOCKET_ONRECEIVE, (WPARAM)nErrorCode, 0);
	}

	CAsyncSocket::OnReceive(nErrorCode);

}


void ClientSocketController::OnClose(int nErrorCode)
{

	//check if the window reference is acceptable
	if (::IsWindow(m_Hwnd))
	{
		//send message (event) to the window
		::PostMessage(m_Hwnd, WM_MYSOCKET_ONCLOSE, (WPARAM)nErrorCode, 0);
	}

	CAsyncSocket::OnClose(nErrorCode);

}



