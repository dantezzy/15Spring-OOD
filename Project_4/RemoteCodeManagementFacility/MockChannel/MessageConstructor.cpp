///////////////////////////////////////////////////////////////////////
// MessageConstructor.cpp - create header strings                    //
// Language:    Visual C++, Visual Studio 2013, SP1                  //
// Application: Project 3 of CSE687                                  //
// Author:      Ziyi Zhao								    	     //
///////////////////////////////////////////////////////////////////////

#include "MessageConstructor.h"

//--------------<add message and content>------------------
MessageConstructor& MessageConstructor::start(const std::string& command, const std::string& content)
{
	header_ = header_ + command;
	header_ = header_ + " ";
	header_ = header_ + content;
	header_ = header_ + "\r\n";

	return *this;
}

//--------------<add the end line>------------------
MessageConstructor& MessageConstructor::end()
{
	header_ = header_ + "\r\n";

	return *this;
}

//--------------<add the attribute>------------------
MessageConstructor& MessageConstructor::attribute(const std::string& name, const std::string& value)
{
	header_ = header_ + name + ":" + value;
	header_ = header_ + "\r\n";

	return *this;
}

//--------------<return the header string>------------------
std::string MessageConstructor::header()
{
	return header_;
}


#ifdef TEST_MESSAGECONSTRUCTOR

void mOut(const std::string& message)
{
	std::cout << "\n  " << message.c_str();
}

void main()
{
	mOut("Testing MessageConstructor");
	mOut("==========================");

	mOut("creating HTTP writer object:");
	MessageConstructor myMessage;


	mOut("creating header elements:");

	myMessage.start("SEND", "text").attribute("name", "value").attribute("name2", "value2").end();

	std::cout << "\n  " << myMessage.header();
}
#endif
