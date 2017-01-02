///////////////////////////////////////////////////////////////////////
// Server1Executive.cpp - simulate the server to do the test         //
// Language:    Visual C++, Visual Studio 2013, SP1                  //
// Application: Project 3 of CSE687                                  //
// Author:      Ziyi Zhao								    	     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package if responsible for simulating
* the server to do the test.
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
* cl /Eha Server1Executive.cpp
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
#include "../Sender/Sender.h"

///////////////////////////////////////////////////////////////////////////
// This server simulation is responsible for demonstrating receiving data 
// from two different clients.

int main(int argc, char* argv[])
{
	try
	{
		std::cout << "\nServer 1:This server simulation is responsible for demonstrating receiving data from two different clients.\n" << std::endl;
		ApplicationHelpers::Verbose v(true);
		SocketSystem ss;
		Receiver revr(std::stoi(argv[1]), Socket::IP6);

		ApplicationHelpers::Verbose::show("press key to exit: ", ApplicationHelpers::always);
		std::cout.flush();
		std::cin.get();
	}

	catch (std::exception& ex)
	{
		ApplicationHelpers::Verbose::show("  Exception caught:", ApplicationHelpers::always);
		ApplicationHelpers::Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
	return 0;
}