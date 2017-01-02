/////////////////////////////////////////////////////////////////////
// MockChannel.h   -  build the bridge from the wpf to C++         //
// ver 1.1                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "Sender.h"

#include <string>
#include <thread>
#include <iostream>


using BQueue = BlockingQueue <MessageStr>;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const MessageStr& desip, const MessageStr& desport,
		const MessageStr& srcport, const MessageStr& filename, const MessageStr& command);

	void postSearchFileMessage(const MessageStr& srcport, const MessageStr& path, 
		VectorStr& patternvec, VectorStr& servervec, const MessageStr& command, const MessageStr& xml);

	void postSearchTextMessage(const MessageStr& srcport, const MessageStr& path,
		VectorStr& patternvec, const MessageStr& text, VectorStr& servervec, const MessageStr& command, const MessageStr& xml);

	std::string Sendr::getfilesearchresult();
	std::string Sendr::gettextsearchresult();
	std::string Sendr::getdownloadresult();
	std::string Sendr::getuploadresult();

  BQueue& queue();
private:
  BQueue sendQ_;
  Sender sdr;
  std::string replyresult_;
};

//----< add upload or download message into the sender queue >-----------------------------
void Sendr::postMessage(const MessageStr& desip, const MessageStr& desport,
	const MessageStr& srcport, const MessageStr& filename, const MessageStr& command)
{
	Message msg;
	msg.DesIP() = desip;
	msg.DesPort() = msg.ToInt(desport);
	msg.SrcPort() = msg.ToInt(srcport);
	msg.command() = command;
	msg.content() = filename;
	//std::string teststr = msg.ToString();
	sdr.SendMsg(msg);

	replyresult_ = "\n" + sdr.ReturnResultQueue().deQ()+"\n";
	
}

//----< add search file message into the sender queue >-----------------------------
void Sendr::postSearchFileMessage(const MessageStr& srcport, const MessageStr& path, 
	VectorStr& patternvec, VectorStr& servervec, const MessageStr& command, const MessageStr& xml)
{
	for (auto serverelem : servervec)
	{
		Message msg;
		int position = serverelem.find(":");
		std::string desip = serverelem.substr(0, position);
		int desport = msg.ToInt(serverelem.substr(position + 1, serverelem.length()-1));
		msg.DesIP() = desip;
		msg.DesPort() = desport;
		msg.command() = command;
		msg.SrcPort() = msg.ToInt(srcport);
		msg.path() = path;
		msg.XML() = xml;
		std::string patternstr;
		if (patternvec.size()!=0)
		{
			for (auto patternelem : patternvec)
			{
				patternstr += patternelem + " ";
			}
		}
		else
		{
			patternstr = "*.*";
		}
		msg.pattern() = patternstr;
		sdr.SendMsg(msg);
	}
	while (sdr.ReturnFileSearchResultQueue().size() != servervec.size())
	{
		::Sleep(100);
	}
	replyresult_ = "";
	while (sdr.ReturnFileSearchResultQueue().size() != 0)
	{
		replyresult_ += "\n" + sdr.ReturnFileSearchResultQueue().deQ();
	}
	
}

//----< add search text message into the sender queue >-----------------------------
void Sendr::postSearchTextMessage(const MessageStr& srcport, const MessageStr& path,
	VectorStr& patternvec, const MessageStr& text, VectorStr& servervec, const MessageStr& command, const MessageStr& xml)
{
	for (auto serverelem : servervec)
	{
		Message msg;
		int position = serverelem.find(":");
		std::string desip = serverelem.substr(0, position);
		int desport = msg.ToInt(serverelem.substr(position + 1, serverelem.length() - 1));
		msg.DesIP() = desip;
		msg.DesPort() = desport;
		msg.command() = command;
		msg.SrcPort() = msg.ToInt(srcport);
		msg.path() = path;
		msg.searchtext() = text;
		msg.XML() = xml;
		std::string patternstr;
		if (patternvec.size() != 0)
		{
			for (auto patternelem : patternvec)
			{
				patternstr += patternelem + " ";
			}
		}
		else
		{
			patternstr = "*.*";
		}

		msg.pattern() = patternstr;
		sdr.SendMsg(msg);
	}
	while (sdr.ReturnTextSearchResultQueue().size() != servervec.size())
	{
		::Sleep(100);
	}
	replyresult_ = "";
	while (sdr.ReturnTextSearchResultQueue().size() != 0)
	{
		replyresult_ += "\n"+sdr.ReturnTextSearchResultQueue().deQ();
	}

}

BQueue& Sendr::queue() { return sendQ_; }

//----< get file search result >-----------------------------
std::string Sendr::getfilesearchresult()
{
	return replyresult_;
}

//----< get text search result >-----------------------------
std::string Sendr::gettextsearchresult()
{
	return replyresult_;
}

//----< get download result >-----------------------------
std::string Sendr::getdownloadresult()
{
	return replyresult_;
}

//----< get upload result >-----------------------------
std::string Sendr::getuploadresult()
{
	return replyresult_;
}
/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	std::string getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

std::string Recvr::getMessage()
{
	return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  void start();
  void stop();
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();
    while (!stop_)
    {
      std::cout << "\n  channel deQing message";
	  MessageStr msg = sendQ.deQ();  // will block here so send quit message when stopping
      std::cout << "\n  channel enQing message";
      recvQ.enQ(msg);
    }
    std::cout << "\n  Server stopping\n\n";
  });
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
