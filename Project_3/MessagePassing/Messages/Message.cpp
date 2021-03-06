/////////////////////////////////////////////////////////////////////
// Message.cpp  - Defines Messages						           //
// ver 1.0                                                         //
// Language:      Visual C++, 2013                                 //
// Application:   Project 3								           //
// Author:        Ziyi Zhao										   //
/////////////////////////////////////////////////////////////////////
#include "Message.h"

//--------------<default constructor>------------------
Message::Message() :SrcIP_(""), DesIP_(""), ActionType_(""),
Action_(""), Command_(""), Content_(""), Connection_(""),
SrcPort_(0), DesPort_(0), MessageSize_(0)
{
}

//--------------<get source IP>------------------
std::string& Message::SrcIP()
{
	return SrcIP_;
}

//--------------<get destination IP>------------------
std::string& Message::DesIP()
{
	return DesIP_;
}

//--------------<get source port>------------------
int& Message::SrcPort()
{
	return SrcPort_;
}

//--------------<get destination port>------------------
int& Message::DesPort()
{
	return DesPort_;
}

//--------------<get command>------------------
std::string& Message::command()
{
	return Command_;
}

//--------------<get content>------------------
std::string& Message::content()
{
	return Content_;
}

//--------------<get current message block size>------------------
int& Message::messagesize()
{
	return MessageSize_;
}

//--------------<get action type>------------------
std::string& Message::actiontype()
{
	return ActionType_;
}

//--------------<get action>------------------
std::string& Message::action()
{
	return Action_;
}

//--------------<get connection status>------------------
std::string& Message::connectionstatus()
{
	return Connection_;
}

//--------------<convert string to int>------------------
int Message::ToInt(const std::string& str)
{
	int temp;
	std::istringstream temp2(str);
	temp2 >> temp;
	return temp;

}

//--------------<convert int to string>------------------
std::string Message::ToString(int temp)
{
	std::ostringstream temp2;
	temp2 << temp;
	return temp2.str();
}

//--------------<convert message to string>------------------
std::string Message::ToString()
{
	MessageConstructor msg;
	msg.start(Command_, Content_)
		.attribute("SrcIP", SrcIP_)
		.attribute("SrcPort", ToString(SrcPort_))
		.attribute("DesIP", DesIP_)
		.attribute("DesPort", ToString(DesPort_))
		.attribute("Action", Action_)
		.attribute("Action_Type", ActionType_)
		.attribute("Connection", Connection_)
		.attribute("Size", ToString(MessageSize_))
		.end();
	return msg.header();
}

//--------------<convert string to message>------------------
Message Message::ToMessage(std::string& strmsg)
{
	Message msg;
	MessageInterpreter interpreter(strmsg);
	msg.command() = interpreter.command();
	msg.content() = interpreter.content();
	while (interpreter.hasnext())
	{
		std::string attr = interpreter.attrname();
		if (attr == "SrcIP")
		{
			msg.SrcIP() = interpreter.attrvalue();
		}
		else if (attr == "SrcPort")
		{
			msg.SrcPort() = msg.ToInt(interpreter.attrvalue());
		}
		else if (attr == "DesIP")
		{
			msg.DesIP() = interpreter.attrvalue();
		}
		else if (attr == "DesPort")
		{
			msg.DesPort() = msg.ToInt(interpreter.attrvalue());
		}
		else if (attr == "Action")
		{
			msg.action() = interpreter.attrvalue();
		}
		else if (attr == "Action_Type")
		{
			msg.actiontype() = interpreter.attrvalue();
		}
		else if (attr == "Connection")
		{
			msg.connectionstatus() = interpreter.attrvalue();
		}
		else if (attr == "Size")
		{
			msg.messagesize() = msg.ToInt(interpreter.attrvalue());
		}
	}
	return msg;
}

#ifdef TEST_MESSAGE
int main()
{
	Message msg;
	msg.command() = "SEND";
	msg.content() = "File";
	msg.SrcIP() = "192.168.1.1";
	msg.SrcPort() = 80;
	msg.DesIP() = "192.168.1.100";
	msg.DesPort() = 8080;
	msg.messagesize() = 512;
	msg.connectionstatus() = "Alive";
	msg.action() = "read";
	msg.actiontype() = "file";
	std::string tempstr = msg.ToString();
	std::cout << tempstr << std::endl;
	Message msgtemp;
	msgtemp = msg.ToMessage(tempstr);
	std::cout << msgtemp.ToString() << std::endl;

	return 0;
}
#endif