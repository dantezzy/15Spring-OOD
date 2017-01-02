///////////////////////////////////////////////////////////////////////
// Client1Executive.cpp - simulate the cilent to do the test         //
// Language:    Visual C++, Visual Studio 2013, SP1                  //
// Application: Project 3 of CSE687                                  //
// Author:      Ziyi Zhao								    	     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package if responsible for simulating
* the client to do the test.
*
* Public Interface:
* ===============
*	int main()
*
* Required Files:
* ===============
* Message.h, Sockets.h, Cpp11-BlockingQueue.h, FileSystem.h
* AppHelpers.h, WindowsHelpers.h, Sender.h, Receiver.h
*
* Build Process:
* ==============
* cl /Eha Client2Executive.cpp
*
* Maintenance History:
* ====================
* ver 1 : 8 Apr 15
* - first release
*/
#include "../Sockets/Sockets.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Messages/Message.h"
#include "../FileSystem/FileSystem.h"
#include "../Display/AppHelpers.h"
#include "../Sockets/WindowsHelpers.h"
#include "../Sender/Sender.h"
#include "../Receiver/Receiver.h"

/////////////////////////////////////////////////////////////////////////////
// This client simulation is responsible for demonstrating two client send 
// data to one same server.

int main(int argc, char* argv[])
{
	std::cout << "\nClient 2:This client simulation is responsible for demonstrating two client send data to one same serve.\n" << std::endl;
	ApplicationHelpers::Verbose v(true);
	SocketSystem ss;
	Message msg;
	msg.command() = argv[1];
	msg.content() = argv[2];
	msg.SrcPort() = std::stoi(argv[3]);
	msg.DesPort() = std::stoi(argv[4]);
	msg.DesIP() = "localhost";
	std::string temp = msg.ToString();
	std::cout << "\n" << temp;

	Sender sdr;
	Receiver revr(std::stoi(argv[3]), Socket::IP6);

	sdr.SendMsg(msg);
	while (1){}

}