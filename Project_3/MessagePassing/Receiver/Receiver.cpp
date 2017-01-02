/////////////////////////////////////////////////////////////////////
// Receiver.cpp   -  Facility to send the message and buffer	   //
// ver 1.0                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 3								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
#include"Receiver.h"

//--------------<Receiver constructor>------------------
Receiver::Receiver(int port, Socket::IpVer ipversion) :SS(), SL_(port, ipversion)
{
	SL_.start(RH_); //open the thread
}

//--------------<Receiver Handler>------------------
void ReceiverHandler::operator()(Socket& socket_)
{
	Message msg;
	while (true)
	{
		std::string msgstr = socket_.recvString();
		if (msgstr.size() == 0)
			break;
		msg = msg.ToMessage(msgstr);
		std::string command = msg.command();
		std::string content = msg.content();
		if (command.size() == 0)
			break;
		ApplicationHelpers::Verbose::show("command" + command);
		if (command == "UPLOAD") //conduct the upload operation
		{
			if (UploadHandling(socket_, msg))
				ApplicationHelpers::Verbose::show("Upload handling test passed", ApplicationHelpers::always);
			else
				ApplicationHelpers::Verbose::show("Upload handling test failed", ApplicationHelpers::always);
			continue;
		}
		ApplicationHelpers::Verbose::show("ReceiverHandler socket connection closing");
		socket_.shutDown(); //close the channel
		socket_.close();
		ApplicationHelpers::Verbose::show("ReceiverHandler thread terminating");
	}
}

//--------------<Conduct download operation>------------------
bool ReceiverHandler::DownloadHandling(Socket& socket_)
{
	return true;
}

//--------------<Conduct upload operation>------------------
bool ReceiverHandler::UploadHandling(Socket& socket_, Message& msg)
{
	std::string content = "Uploaded/"+msg.content();
	FileSystem::File file(content);
	if (!file.open(FileSystem::File::out, FileSystem::File::binary))
	{
		std::string temp = "Cannot open file" + content;
		ApplicationHelpers::Verbose::show(temp, true);
		return false;
	}
	if (!WriteFile(socket_, msg, file)) //call the function to write file from buffer
	{
		return false;
	}
	return true;
}

//--------------<Write from buffer to file>------------------
bool ReceiverHandler::WriteFile(Socket& socket_, Message& msg, FileSystem::File& file)
{
	int messagesize = msg.messagesize();
	std::string msgstring;
	Message msgtemp;
	const int bufferlen = 1024;
	char buffer[bufferlen];
	bool ok;
	while (messagesize == bufferlen) // still has data
	{
		::Sleep(100);
		ok = socket_.recv(messagesize, buffer);
		if (socket_ == INVALID_SOCKET || !ok)
			return false;
		msgstring = socket_.recvString();
		file.putBuffer(messagesize, buffer);
		std::string temp1(buffer, messagesize); 
		std::cout << temp1; 
		msgtemp = msgtemp.ToMessage(msgstring);
		messagesize = msgtemp.messagesize();
	}
	if (msgstring != "END") // final data
	{
		ok = socket_.recv(messagesize, buffer);
		if (socket_ == INVALID_SOCKET || !ok)
			return false;
		file.putBuffer(messagesize, buffer);
		std::string temp1(buffer, messagesize); 
		std::cout << temp1; 
		file.close();
	}
	std::string content = msg.content(); //reply message to the client
	std::string replymsg = " Server has received "+content+"! File has been saved in the Uploaded folder. \n";
	socket_.sendString(replymsg);
	socket_.sendString("TEST_END");
	return true;
}

#ifdef TEST_RECEIVER
int main()
{
	try
	{
		ApplicationHelpers::Verbose v(true);
		SocketSystem ss;
		Receiver revr(9080, Socket::IP6);

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
#endif
