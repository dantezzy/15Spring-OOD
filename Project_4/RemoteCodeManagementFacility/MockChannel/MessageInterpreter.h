#ifndef MESSAGEINTERPRETER_H
#define MESSAGEINTERPRETER_H
///////////////////////////////////////////////////////////////////////
// MessageInterpreter.h - Parse header strings                       //
// Language:    Visual C++, Visual Studio 2013, SP1                  //
// Application: Project 3 of CSE687                                  //
// Author:      Ziyi Zhao										     //
///////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* ===================
* This package is responsible for parse the message string.It provides 
* a simple interface to step from HTTP attribute to attribute and extract
* constituent parts, e.g., attribute,value, command, and content.
*
* Public Interface:
* ===============
* MessageInterpreter(const std::string& message);
* bool hasnext();
* std::string command();
* std::string content();
* std::string attrname();
* std::string attrvalue();
*}
*
* Required Files:
* ===============
* MessageInterpreter.h, MessageInterpreterr.cpp
*
* Build Process:
* ==============
* cl /Eha MessageInterpreter.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 2 Apr 15
* - first release
*/
#include <string>
#include <iostream>

class MessageInterpreter
{
public:
	MessageInterpreter(const std::string& header);
	bool hasnext();
	std::string command();
	std::string content();
	std::string attrname();
	std::string attrvalue();

private:
	std::string header_;
	std::string attrname_;
	std::string attrvalue_;
	size_t position;
	size_t localposition1;
	size_t localposition2;
};



#endif