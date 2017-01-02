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
* cl /Eha Client1Executive.cpp 
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

///////////////////////////////////////////////////////////////////////////////
// This client simulation is responsible for demonstrating two client send 
// data to one same server. Then this client will responsible for domonstrating 
// shutdown and reconnect to another server to continue send data.

int main(int argc, char* argv[])
{
	std::cout << "\nClient 1:This client simulation is responsible for demonstrating two client send ";
	std::cout << "data to one same server. Then this client will responsible for domonstrating ";
	std::cout << "shutdown and reconnect to another server to continue send data.\n" << std::endl;

	ApplicationHelpers::Verbose v(true);
	SocketSystem ss;

	Message msg2;
	msg2.command() = argv[1];
	msg2.content() = argv[2];
	msg2.SrcPort() = std::stoi(argv[3]);
	msg2.DesPort() = std::stoi(argv[4]);
	msg2.DesIP() = "localhost";
	std::string temp2 = msg2.ToString();
	std::cout << "\n" << temp2;

	Message msg3;
	msg3.command() = argv[1];
	msg3.content() = argv[5];
	msg3.SrcPort() = std::stoi(argv[3]);
	msg3.DesPort() = std::stoi(argv[6]);
	msg3.DesIP() = "localhost";
	std::string temp3 = msg3.ToString();
	std::cout << "\n" << temp3;

	Sender sdr2;
	Receiver revr(std::stoi(argv[3]), Socket::IP6);

	sdr2.SendMsg(msg2);
	sdr2.SendMsg(msg3);


	while (1){}
	return 0;
}