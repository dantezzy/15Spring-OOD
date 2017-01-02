/////////////////////////////////////////////////////////////////////
// Sender.cpp   -  Facility to send the message and buffer         //
// ver 1.0                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 3								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
#include "Sender.h"
#include <iostream>

//--------------<Sender constructor>------------------
Sender::Sender()
{
	std::thread t([this]{this->Supervisor(); });
	t.detach();
}

//--------------<Connect to the receiver>------------------
bool Sender::Connect(std::string& IP, int Port)
{
	int count = 10;
	while (!SC_.connect(IP, Port)&&count!=0) //if connection failed try 20 times then return false
	{
		::Sleep(300);
		count--;
	}
	if (count == 0)
	{
		return false;
	}
	return true;
}

//--------------<Put message in the queue>------------------
void Sender::SendMsg(const Message msg)
{
	
	SendingQueue_.enQ(msg); // put the message in the sending queue
}

//--------------<Close the socket channel>------------------
void Sender::Close()
{
	SC_.shutDown(); //when finish uploading then clos the sending channel
	if (SC_.shutDownSend())
	{
		std::cout << "\nConnection has been broken!\n" << std::endl;
	}
	SC_.close();
	
}

//--------------<Supervise the sending queue>------------------
void Sender::Supervisor()
{
	while (true) //supervise the sending queue, if there is any message in the queue,then process the message
	{
		if (SendingQueue_.size() != 0)
		{
			msg = SendingQueue_.deQ();
			std::string command = msg.command();
			std::string DesIp = msg.DesIP();
			int DesPort = msg.DesPort();

			if (command.size() == 0)
				continue;

			ApplicationHelpers::Verbose::show("command: " + command);

			if (!Connect(DesIp, DesPort))
				continue;
			ApplicationHelpers::Verbose::show("\nConnect to the Server which has the Port: "+DesPort,ApplicationHelpers::always);

			if (command == "UPLOAD") //conduct the upload operation
			{
				if (UploadHandling(SC_))
					ApplicationHelpers::Verbose::show("Upload handling test passed", ApplicationHelpers::always);
			}
		}
		else
		{
			::Sleep(100);
			continue;
		}
	}
}

//--------------<Conduct download operation>------------------
bool Sender::DownloadHandling(Socket& socket_)
{
	return true;
}

//--------------<Conduct upload operation>------------------
bool Sender::UploadHandling(Socket& socket_)
{
	std::string msgstr = msg.ToString();
	std::string content ="WaitingForUploading/"+ msg.content();
	FileSystem::File bufferIn(content);
	bufferIn.open(FileSystem::File::in, FileSystem::File::binary);

	if (!bufferIn.isGood())
	{
		std::cout << "\n  could not open \"" << content << "\"for writing\n\n";
		return 1;
	}
	else
		std::cout << "\n  opening: \"" << content << "\" for reading";

	while (true) //read file to buffer, then send the buffer
	{
		size_t resultSize = bufferIn.getBuffer(BUFFERSIZE, buffer);
		std::string temp1(buffer, resultSize);
		std::cout << temp1;

		msg.messagesize() = resultSize;
		socket_.sendString(msg.ToString());
		socket_.send(resultSize, buffer);

		if (resultSize < BUFFERSIZE || !bufferIn.isGood())
		{
			bufferIn.close();
			break;
		}
	}
		while (true) //wait for the server reply information
		{
			std::string str = socket_.recvString();
			if (str.size() == 0 || str == "TEST_END")
				break;
			ApplicationHelpers::Verbose::show("Client receivd reply: " + str, ApplicationHelpers::always);
		}
	Close();

	return true;
}

#ifdef TEST_SENDER
int main()
{
	ApplicationHelpers::Verbose v(true);
	SocketSystem ss;
	Message msg;
	msg.command() = "UPLOAD";
	msg.content() = "UnitTest.h";
	msg.SrcPort() = 5050;
	msg.DesPort() = 9080;
	std::string temp = msg.ToString();
	std::cout <<"\n"<< temp;

	Sender sdr;
	sdr.SendMsg(msg);
	while (1){}
	return 0;
}
#endif