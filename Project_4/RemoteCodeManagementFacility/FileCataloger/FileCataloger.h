#ifndef CATALOGER_H
#define CATALOGER_H
///////////////////////////////////////////////////////////////////////
// FileCataloger.h - Define the public function interface and        //
//               the private memeber of the Executive.cpp            //
// ver 1.3                                                           //
// Language:    C++                                                  //
// Platform:    Macbook Pro 13, Win8.1, SP1                          //
// Application: File Catalogue, Project #1, Spring 2015              //
// Author:      Ziyi Zhao, Computer Engineering, Syracuse University //
//              (315) 708-5889, zzhao37@syr.edu                      //
///////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* --------------------------------------------------------------------*
*
* It defines following functions:
*   CommandParser - parse the user input into the path patterns and options
*   start - according to the path, different patterns and different options
*           to do different operations.
*   ContinueSearch - let the user can do the continue search to find the file
*                    which contains the specific text
*   InputText - let the user input the searching text
*   InputPattern - let the user input hte searchin patterns
*
* Public Interface:
* ===============
* using Patterns = std::vector<std::string>;
* using Options = std::vector<std::string>;
* using iterator = Patterns::iterator;
* using Path = std::string;
* using Text = std::string;
* using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
* Cataloger()
* void CommandParser(char *argv[]);
* void NewCommandParser(std::string path,std::vector<std::string> patterns);
* void NewCommandParser2(std::string path, std::vector<std::string> patterns,std::string text);
* void NewCommandParserXML(std::string path, std::vector<std::string> patterns);
* void start(sPtr& root_, int a);
* void ContinueSearch(const DataStore& ds);
* void InputText();
* void InputPattern();
* std::string GetReturnFileString();
* Options ReturnOption(){ return options_; }
*
*
* Required Files:
*  DataStore.h FileSystem.h FileMgr.h Display.h, XmlElement.h
*  DataStore.cpp FileSystem.cpp FileMgr.cpp Display.cpp, XmlElement.cpp
*
* Maintenance History:
* --------------------
* ver 1.3 : 27 Apr 2015
* - add new parser to conduct different operations
* ver 1.2 : 8 Feb 2015
* - complete the function of continuesearch()
* - divide continuesearch() into small parts
* ver 1.1 : 3 Feb 2015
* - add continuesearch()
* ver 1.0 : 1 Feb 2015
* - first release
*
*/
#include "../FileMgr/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../DataStore/DataStore.h"
#include "../Display/Display.h"
#include "../XmlElement/XmlElement.h"

class Cataloger
{
public:
	using Patterns = std::vector<std::string>;
	using Options = std::vector<std::string>;
	using iterator = Patterns::iterator;
	using Path = std::string;
	using Text = std::string;
	using sPtr = std::shared_ptr < XmlProcessing::AbstractXmlElement >;
	Cataloger() :path_("../")
	{
		patterns_.push_back("*.*");
	}
	void CommandParser(char *argv[]);
	void NewCommandParser(std::string path,std::vector<std::string> patterns);
	void NewCommandParser2(std::string path, std::vector<std::string> patterns,std::string text);
	void NewCommandParserXML(std::string path, std::vector<std::string> patterns);
	void start(sPtr& root_, int a);
	void ContinueSearch(const DataStore& ds);
	void InputText();
	void InputPattern();
	std::string GetReturnFileString();
	Options ReturnOption(){ return options_; }
private:
	std::string replyfilestring_;
	Patterns patterns_;
	Path path_;
	Options options_;
	Text text_;
};
#endif