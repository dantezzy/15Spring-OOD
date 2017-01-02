#ifndef SENDER_H
#define SENDER_H
/////////////////////////////////////////////////////////////////////
// Sender.h   -  Facility to send the message and buffer           //
// ver 1.1                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
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
* bool SearchFileHandling(Socket& socket_);
* bool SearchTextHandling(Socket& socket_);
* bool WriteFile(Socket& socket_, Message& msg, FileSystem::File& file);
* void Close();
* BlockingQueue<std::string>& ReturnResultQueue();
* BlockingQueue<std::string>& ReturnFileSearchResultQueue();
* BlockingQueue<std::string>& ReturnTextSearchResultQueue();
* std::string ConvertIntToString(std::chrono::milliseconds time);
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
* ver 1.1 : 26 Apr 15
* - add donwload function
* - add search file function
* - add search text function
* ver 1.0 : 6 Apr 15
* - first release
*/
#include "Sockets.h"
#include "Cpp11-BlockingQueue.h"
#include "Message.h"
#include "FileSystem.h"
#include "AppHelpers.h"
#include "WindowsHelpers.h"
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
	bool SearchFileHandling(Socket& socket_);
	bool SearchTextHandling(Socket& socket_);
	bool WriteFile(Socket& socket_, Message& msg, FileSystem::File& file);
	void Close();
	BlockingQueue<std::string>& ReturnResultQueue();
	BlockingQueue<std::string>& ReturnFileSearchResultQueue();
	BlockingQueue<std::string>& ReturnTextSearchResultQueue();
	std::string ConvertIntToString(std::chrono::milliseconds time);

private:
	BlockingQueue<std::string> ResultQueue_;
	BlockingQueue<std::string> FileSearchResultQueue_;
	BlockingQueue<std::string> TextSearchResultQueue_;
	BlockingQueue<Message> SendingQueue_;
	SocketSystem ss;
	SocketConnecter SC_;
	Socket::byte buffer[BUFFERSIZE];
	Message msg;
	
};
#endif