///////////////////////////////////////////////////////////////////////
// MessageInterpreter.cpp - parse header strings                     //
// Language:    Visual C++, Visual Studio 2013, SP1                  //
// Application: Project 3 of CSE687                                  //
// Author:      Ziyi Zhao								    	     //
///////////////////////////////////////////////////////////////////////

#include "MessageInterpreter.h"

//--------------<default constructor>------------------
MessageInterpreter::MessageInterpreter(const std::string& header)
	:position(0), localposition1(0), localposition2(0), header_(header)
{
}

//--------------<get command>------------------
std::string MessageInterpreter::command()
{
	if (header_.find("UPLOAD")==0)
	{
		return "UPLOAD";
	}
	else
	{
		//std::cout << "Cannot run this operation." << std::endl;
		return "UPLOAD";
	}
	return "";
}

//--------------<get content>------------------
std::string MessageInterpreter::content()
{
	size_t contentposition = header_.find_first_of(" ");
	position = header_.find_first_of("\r\n");
	std::string content_ = header_.substr(contentposition + 1, position - contentposition - 1);
	position++;
	localposition1 = localposition1 + position + 1;
	localposition2 = localposition2 + position + 1;
	return content_;
}

//--------------<get attribute name>------------------
std::string MessageInterpreter::attrname()
{
	attrname_ = "";
	while (true)
	{
		size_t beginposition = localposition1;
		localposition1 = header_.find(":", localposition1);
		if (localposition1<position)
		{
			attrname_ = header_.substr(beginposition, localposition1 - beginposition);
			localposition1 = position + 1;
			break;
		}
		else
		{
			localposition1 = position + 1;
			break;
		}
	}
	return attrname_;
}

//--------------<get attribute value>------------------
std::string MessageInterpreter::attrvalue()
{
	attrvalue_ = "";
	while (true)
	{
		localposition2 = header_.find(":", localposition2);
		size_t beginposition = localposition2 + 1;
		if (localposition2<position)
		{
			attrvalue_ = header_.substr(beginposition, position - beginposition - 1);
			localposition2 = position + 1;
			break;
		}
		else
		{
			localposition2 = position + 1;
			break;
		}
	}
	return attrvalue_;
}

//--------------<if there is next attribute>------------------
bool MessageInterpreter::hasnext()
{
	while (true)
	{
		position = header_.find("\r\n", position);
		if (position==0)
			break;
		if (header_.substr(position-2,2)=="\r\n")
			break;
		++position;
		return true;
	}
	return false;
}
/*
#ifdef TEST_MESSAGEINTERPRETER

void mOut(const std::string& message)
{
	std::cout << "\n  " << message.c_str();
}

void main()
{
	mOut("Testing Messageinterpreter");
	mOut("==========================");

	std::string test="SEND text\r\nSourceIp:192.168.1.1\r\nDestIp:192.188.1.100\r\n\r\n";
	MessageInterpreter myInt(test);

	std::cout << "\ntest header: \n" << test << std::endl;


	mOut("testing attrname(),attrvalue(), command, content():");
	
	std::cout << "\n  command= " << myInt.command();
	std::cout << "\n  content= " << myInt.content();

	while (myInt.hasnext())
	{
		std::cout << "\n  attribute name: " << myInt.attrname();
		std::cout << "\n  value: " << myInt.attrvalue();
	}

	std::cout << "\n\n";
}
#endif
*/