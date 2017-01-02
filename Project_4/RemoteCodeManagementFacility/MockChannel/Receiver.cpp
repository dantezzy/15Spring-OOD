/////////////////////////////////////////////////////////////////////
// Receiver.cpp   -  Facility to send the message and buffer	   //
// ver 1.1                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
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
		//if (command.size() == 0)
			//break;
		ApplicationHelpers::Verbose::show("command" + command);
		if (command == "UPLOAD") //conduct the upload operation
		{
			std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

			UploadHandling(socket_, msg);

			std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
			std::chrono::milliseconds total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			std::string content2 = msg.content(); //reply message to the client
			std::string replymsg = "Server has received " + content2 
				+ "!\nFile has been saved in the ServerRepository folder.\nUploading cost " 
				+ ConvertIntToString(total_ms) + " ms.";
			socket_.sendString(replymsg);
			socket_.sendString("TEST_END");
			//continue;
		}
		if (command == "DOWNLOAD") //conduct the upload operation
			DownloadHandling(socket_, msg);
			//continue;
		if (command=="SEARCHFILE")
			SearchFileHandling(socket_, msg);
			//continue;
		if (command == "SEARCHTEXT")
			SearchTextHandling(socket_, msg);
			//continue;
		ApplicationHelpers::Verbose::show("ReceiverHandler socket connection closing");
		socket_.shutDown(); //close the channel
		socket_.close();
		ApplicationHelpers::Verbose::show("ReceiverHandler thread terminating");
	}
}
std::string ReceiverHandler::ConvertIntToString(std::chrono::milliseconds time)
{
	std::string timestring;
	std::ostringstream oss;
	oss << time.count();
	timestring = oss.str();

	return timestring;
}

bool ReceiverHandler::SearchFileHandling(Socket& socket_, Message& msg)
{
	std::string pattern_ = msg.pattern();
	std::string path = msg.path();
	std::vector<std::string> temppatterns;
	while (pattern_.length()>0)
	{
		int position = pattern_.find(" ");
		std::string patt = pattern_.substr(0, position);
		pattern_ = pattern_.substr(position + 1, pattern_.length() - 1);
		temppatterns.push_back(patt);
	}
	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();

	Cataloger myCataloger;
	DataStore myDatastore;
	myDatastore.root = XmlProcessing::makeTaggedElement("ServerRepository");
	if (msg.XML() == "RETURNXML")
		myCataloger.NewCommandParserXML(path, temppatterns);
	else
	myCataloger.NewCommandParser(path,temppatterns);
	
	myCataloger.start(myDatastore.root,1);
	std::cout << myDatastore.root->toString();
	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	std::string timereplymsg = "\nServer " + msg.DesIP() + ":" + msg.ToString(msg.DesPort()) + " has these files." + "\nSearh file cost "
		+ ConvertIntToString(total_ms) + " ms.\n\n";
	std::string replymsg = myCataloger.GetReturnFileString()+timereplymsg;
	if (msg.XML() == "RETURNXML")
		socket_.sendString(myDatastore.root->toString() + timereplymsg);
	else
	socket_.sendString(replymsg);
	return true;
}

bool ReceiverHandler::SearchTextHandling(Socket& socket_, Message& msg)
{
	std::string pattern_ = msg.pattern();
	std::string path = msg.path();
	std::string text = "/f" + msg.searchtext();
	std::vector<std::string> temppatterns;
	while (pattern_.length()>0)
	{
		int position = pattern_.find(" ");
		std::string patt = pattern_.substr(0, position);
		pattern_ = pattern_.substr(position + 1, pattern_.length() - 1);
		temppatterns.push_back(patt);
	}
	int a = 0;

	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
	Cataloger myCataloger;
	DataStore myDatastore;

	myDatastore.root = XmlProcessing::makeTaggedElement("ServerRepository");

	myCataloger.NewCommandParser2(path, temppatterns,text);
	if (msg.XML() == "RETURNXML")
		myCataloger.start(myDatastore.root, 2);
	else
		myCataloger.start(myDatastore.root, 1);

	std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	std::string timereplymsg = "\nServer "+msg.DesIP()+":"+msg.ToString(msg.DesPort())+" found the file."+"\nSearh text cost "
		+ ConvertIntToString(total_ms) + " ms.\n\n";
	std::string replymsg = myCataloger.GetReturnFileString()+ timereplymsg;
	if (msg.XML() == "RETURNXML")
		socket_.sendString(myDatastore.root->toString() + timereplymsg);
	else
		socket_.sendString(replymsg);
	return true;
}

//--------------<Conduct download operation>------------------
bool ReceiverHandler::DownloadHandling(Socket& socket_, Message& msg)
{
	std::string content = "ServerRepository/" + msg.content();
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
	/*while (true) //wait for the server reply information
	{
		std::string str = socket_.recvString();
		if (str.size() == 0 || str == "TEST_END")
			break;
		//ResultQueue_.enQ(str);
		ApplicationHelpers::Verbose::show("Client receivd reply: " + str, ApplicationHelpers::always);
	}*/
	return true;
}

//--------------<Conduct upload operation>------------------
bool ReceiverHandler::UploadHandling(Socket& socket_, Message& msg)
{
	std::string content = "ServerRepository/"+msg.content();
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
