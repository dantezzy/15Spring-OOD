#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////
// Message.h   -  Encapsulate messages					           //
// ver 1.0                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 3								           //
// Author:        Ziyi Zhao								           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package provides an interfaces to convert 
* message to attribute and convert back
* and for user to get the detailed attributes
*
* Public Interface:
* ===============
*	Message();
*  	std::string& SrcIP();
*	std::string& DesIP();
*	int& SrcPort();
*	int& DesPort();
*	std::string& command();
*	std::string& content();
*	int& messagesize();
*	std::string& actiontype();
*	std::string& action();
*	std::string& connectionstatus();
*	std::string ToString();
*	static Message ToMessage(std::string& msg);
*	int ToInt(const std::string& str);
*	std::string ToString(int temp);
*
* Required Files:
* ===============
* Message.h, Message.cpp MessageInterpreter.h MessageInterpreter.cpp
* MessageConstructor.h MessageConstructor.cpp
*
* Build Process:
* ==============
* cl /Eha Message.cpp MessageInterpreter.cpp MessageConstructor.cpp
*
* Maintenance History:
* ====================
* ver 1 : 4 Apr 15
* - first release
*/

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "MessageConstructor.h"
#include "MessageInterpreter.h"

class Message
{
public:
	Message();
	std::string& SrcIP();
	std::string& DesIP();
	int& SrcPort();
	int& DesPort();
	std::string& command();
	std::string& content();
	int& messagesize();
	std::string& actiontype();
	std::string& action();
	std::string& connectionstatus();
	std::string ToString();
	static Message ToMessage(std::string& msg);
	int ToInt(const std::string& str);
	std::string ToString(int temp);

private:
	std::string SrcIP_;
	std::string DesIP_;
	std::string ActionType_;
	std::string Action_;
	std::string Command_;
	std::string Content_;
	std::string Connection_;
	int SrcPort_;
	int DesPort_;
	int MessageSize_;
};


#endif