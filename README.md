# **MFC Client TCP Messenger Application**

## Overview:
The aim of this application is to provide an easily understandable and basic example of utilizing sockets to create a server-client messaging system within the MFC framework, inspired by the lack of comprehensive examples online.

## Socket Use In MFC And This Application:
This application takes advantage of MFC's "CAsyncSocket" class, as it is convenient to use non-blocking I/O in a GUI messaging application. This is in contrast to MFC's "higher level" CSocket class, which is fundamentally for use in blocking I/O.

Async I/O via the CAsyncSocket class is accomplished via events, which requires implementing a number of not-especially-modern MFC-style events and event handlers.

The best practice with the CAsyncSocket class involves create a subclass that inherits CAsyncSocket. This is due to the fact that we must create user-defined windows messages to map our event handlers to (a quirk of how custom events, or should I say "messages", work in MFC). In our example "SocketController" is the subclass in question.

## Related Software:
This is the client application repository, see the server application repository [here](https://github.com/IanNBackus/MFCServerTCPMessenger).
