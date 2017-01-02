#ifndef SENDER_H
#define SENDER_H
/////////////////////////////////////////////////////////////////////
// Sender.h   -  Facility to send the message and buffer           //
// ver 1.0                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 3								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package provides a Channel facility to send the message 
* and read file to buffer and send it.
*
* Public Interface:
* ===============
* Sender();
* bool Connect(std::string& IP, int Port);
* void SendMsg(const Message msg);
* void WaitSend();
* void Supervisor();
* bool UploadHandling(Socket& socket_);
* bool DownloadHandling(Socket& socket_);
* void Close();
*
* Required Files:
* ===============
* Sender.h, Sender.cpp 
*
* Build Process:
* ==============
* cl /Eha Sender.cpp 
*
* Maintenance History:
* ====================
* ver 1 : 6 Apr 15
* - first release
*/
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Messages/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/AppHelpers.h"
#include "../Sockets/WindowsHelpers.h"
#include <string>
#include <thread>
#define BUFFERSIZE 1024

class Sender
{
public:
	Sender();
	bool Connect(std::string& IP, int Port);
	void SendMsg(const Message msg);
	void WaitSend();
	void Supervisor();
	bool UploadHandling(Socket& socket_);
	bool DownloadHandling(Socket& socket_);
	void Close();
private:
	BlockingQueue<Message> SendingQueue_;
	SocketConnecter SC_;
	Socket::byte buffer[BUFFERSIZE];
	Message msg;
};
#endif