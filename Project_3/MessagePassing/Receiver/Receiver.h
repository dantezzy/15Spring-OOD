#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////
// Receiver.h   -  Facility to send the message and buffer	       //
// ver 1.0                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 3								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package provides a Channel facility to receive the message
* and read bytes from buffer and write it file.
*
* Public Interface:
* ===============
* Receiver(int Port, Socket::IpVer ipversion);
* void operator()(Socket& socket_);
* bool DownloadHandling(Socket& socket_);
* bool UploadHandling(Socket& socket_, Message& msg);
* bool WriteFile(Socket& socket_, Message& msg, FileSystem::File& file);
*
* Required Files:
* ===============
* Receiver.h, Receiver.cpp 
*
* Build Process:
* ==============
* cl /Eha Receiver.cpp 
*
* Maintenance History:
* ====================
* ver 1 : 7 Apr 15
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

class ReceiverHandler
{
public:
	void operator()(Socket& socket_);
	bool DownloadHandling(Socket& socket_);
	bool UploadHandling(Socket& socket_, Message& msg);
	bool WriteFile(Socket& socket_, Message& msg, FileSystem::File& file);
private:
};

class Receiver
{
public:
	Receiver(int Port, Socket::IpVer ipversion);
private:
	BlockingQueue<Message> ReceivingQueue_;
	SocketListener SL_;
	SocketSystem SS;
	ReceiverHandler RH_;
};
#endif
