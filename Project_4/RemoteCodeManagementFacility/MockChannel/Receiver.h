#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////
// Receiver.h   -  Facility to send the message and buffer	       //
// ver 1.1                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package provides a Channel facility to receive the message
* and read bytes from buffer and write it file. In addition, the receiver
* is responsible for handling the download message, search file message and
* search text handling.
*
* Public Interface:
* ===============
* Receiver(int Port, Socket::IpVer ipversion);
* void operator()(Socket& socket_);
* bool DownloadHandling(Socket& socket_, Message& msg);
* bool UploadHandling(Socket& socket_, Message& msg);
* bool SearchFileHandling(Socket& socket_, Message& msg);
* bool SearchTextHandling(Socket& socket_, Message& msg);
* bool WriteFile(Socket& socket_, Message& msg, FileSystem::File& file);
* std::string ConvertIntToString(std::chrono::milliseconds time);
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
* ver 1.1 : 26 Apr 15
* - add download handling
* - add search file handling
* - add search text handling
* ver 1.0 : 7 Apr 15
* - first release
*/
#include "Sockets.h"
#include "Cpp11-BlockingQueue.h"
#include "Message.h"
#include "FileSystem.h"
#include "AppHelpers.h"
#include "WindowsHelpers.h"
#include "../FileCataloger/FileCataloger.h"
#include <string>
#include <thread>
#include <chrono>
#define BUFFERSIZE 1024

class ReceiverHandler
{
public:
	void operator()(Socket& socket_);
	bool DownloadHandling(Socket& socket_, Message& msg);
	bool UploadHandling(Socket& socket_, Message& msg);
	bool SearchFileHandling(Socket& socket_, Message& msg);
	bool SearchTextHandling(Socket& socket_, Message& msg);
	bool WriteFile(Socket& socket_, Message& msg, FileSystem::File& file);
	std::string ConvertIntToString(std::chrono::milliseconds time);

private:
 
	Socket::byte buffer[BUFFERSIZE];
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
