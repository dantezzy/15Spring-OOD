#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////
// MockChannel.h   -  build the bridge from the wpf to C++         //
// ver 1.1                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 4								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package is responsible for building a birdge to connect the wpf GUI
*  to the real function. And this package combine the serveal packages to 
*  achievethe file/text search function and return the result.
*
*
*  Public Interface:
*  ===============
*  virtual void start();
*  virtual void stop();
*
*  Required Files:
*  ---------------
*  MochChannel.h, MochChannel.cpp, FileMgr.cpp, FileSystem.cpp
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp, Message.cpp
*  AppHelper.cpp, DataStore.cpp,Display.cpp,FileCataloger.cpp
*  MessageConstructor.cpp, MessageInterpreter.cpp, Receiver.cpp
*  Sender.cpp, Sockets.cpp, WindowsHelpers.cpp, XmlElement.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 26 Apr 2015
*  - add file search and text search function
*  ver 1.0 : 25 Apr 2015
*  - first release
*  - achieve the upload and download function
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include <vector>
using MessageStr = std::string;
using VectorStr = std::vector < std::string >;
struct ISendr
{
	virtual void postMessage(const MessageStr& desip, const MessageStr& desport, 
		const MessageStr& srcport, const MessageStr& filename, const MessageStr& command) = 0;

	virtual void postSearchFileMessage(const MessageStr& srcport, const MessageStr& path,
		VectorStr& patternvec, VectorStr& servervec, const MessageStr& command, const MessageStr& xml) = 0;

	virtual void postSearchTextMessage(const MessageStr& srcport, const MessageStr& path,
		VectorStr& patternvec, const MessageStr& text, VectorStr& servervec, const MessageStr& command, const MessageStr& xml) = 0;

	virtual std::string getfilesearchresult() = 0;
	virtual std::string gettextsearchresult() = 0;
	virtual std::string getdownloadresult() = 0;
	virtual std::string getuploadresult() = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


