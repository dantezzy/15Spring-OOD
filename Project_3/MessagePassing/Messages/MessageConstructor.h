#ifndef MESSAGECONSTRUCTOR_H
#define MESSAGECONSTRUCTOR_H
///////////////////////////////////////////////////////////////////////
// MessageConstructor.h - create header strings                      //
// Language:    Visual C++, Visual Studio 2013, SP1                  //
// Application: Project 3 of CSE687                                  //
// Author:      Ziyi Zhao    									     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This package is responsible for constructuring the messagec. Including the 
* the command,content and attributes.
*
* Public Interface:
* ===============
* start(const std::string& command,const std::string& content);
* end();
* attribute(const std::string& name, const std::string& value);
* std::string header();
*
* Required Files:
* ===============
* MessageConstructor.h, MessageConstructor.cpp
*
* Build Process:
* ==============
* cl /Eha MessageConstructor.cpp
*
* Maintenance History:
* ====================
* ver 1.0 : 2 Apr 15
* - first release
*/

#include <string>
#include <iostream>
#include <fstream>

class MessageConstructor
{
public:
	MessageConstructor& start(const std::string& command,const std::string& content);
	MessageConstructor& end();
	MessageConstructor& attribute(const std::string& name, const std::string& value);
	std::string header();

private:
	std::string header_;
};


#endif