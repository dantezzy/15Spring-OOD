/////////////////////////////////////////////////////////////////////
// Sender.h   -  Facility to send the message and buffer           //
// ver 1.1                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
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
	while (!SC_.connect(IP, Port)&&count!=0) //if connection failed try 10 times then return false
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

			if (!Connect(DesIp, DesPort))
				continue;
			ApplicationHelpers::Verbose::show("\nConnect to the Server which has the Port: "+DesPort,ApplicationHelpers::always);

			if (command == "UPLOAD") //conduct the upload operation
				UploadHandling(SC_);
			if (command=="DOWNLOAD")
				DownloadHandling(SC_);
			if(command == "SEARCHFILE")
				SearchFileHandling(SC_);
			if (command == "SEARCHTEXT")
				SearchTextHandling(SC_);
		}
		else
		{
			::Sleep(100);
			continue;
		}
	}
}

//--------------<Conduct search file operation>------------------
bool Sender::SearchFileHandling(Socket& socket_)
{
	socket_.sendString(msg.ToString());
	std::string replymsg = socket_.recvString();
	std::cout << replymsg << "\n";
	FileSearchResultQueue_.enQ(replymsg);
	return true;
}
bool Sender::SearchTextHandling(Socket& socket_)
{
	socket_.sendString(msg.ToString());
	std::string replymsg = socket_.recvString();
	std::cout << replymsg << "\n";
	TextSearchResultQueue_.enQ(replymsg);
	return true;
}

//--------------<Conduct download operation>------------------
bool Sender::DownloadHandling(Socket& socket_)
{
	socket_.sendString(msg.ToString());
	std::string content = "ClientRepository/" + msg.content();
	FileSystem::File file(content);
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
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
	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	std::string replymsg = "Client has download " + msg.content()
		+ "!\nFile has been saved in the ClientRepository folder.\nDownloading cost "
		+ ConvertIntToString(total_ms) + " ms.";

	ResultQueue_.enQ(replymsg);
	return true;
}

//--------------<Convert int to string>------------------
std::string Sender::ConvertIntToString(std::chrono::milliseconds time)
{
	std::string timestring;
	std::ostringstream oss;
	oss << time.count();
	timestring = oss.str();

	return timestring;
}

//--------------<write the data to file>------------------
bool Sender::WriteFile(Socket& socket_, Message& msg, FileSystem::File& file)
{
	std::string msgstr = socket_.recvString();
	Message temp = temp.ToMessage(msgstr);
	int messagesize = temp.messagesize();
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

	return true;
}

//--------------<Conduct upload operation>------------------
bool Sender::UploadHandling(Socket& socket_)
{
	//std::string msgstr = msg.ToString();
	std::string content ="ClientRepository/"+ msg.content();
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
			ResultQueue_.enQ(str);
			ApplicationHelpers::Verbose::show("Client receivd reply: " + str, ApplicationHelpers::always);
		}
	Close();

	return true;
}

//--------------<return the result queue>------------------
BlockingQueue<std::string>& Sender::ReturnResultQueue()
{ 
	return ResultQueue_;
}

//--------------<return the file search result queue>------------------
BlockingQueue<std::string>& Sender::ReturnFileSearchResultQueue()
{
	return FileSearchResultQueue_;
}

//--------------<return the text search result queue>------------------
BlockingQueue<std::string>& Sender::ReturnTextSearchResultQueue()
{
	return TextSearchResultQueue_;
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