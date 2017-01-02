#ifndef CATALOGER_H
#define CATALOGER_H
///////////////////////////////////////////////////////////////////////
// FileCataloger.h - Define the public function interface and        //
//               the private memeber of the Executive.cpp            //
// ver 1.2                                                           //
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
* Required Files:
*  DataStore.h FileSystem.h FileMgr.h Display.h
*
* Maintenance History:
* --------------------
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


class Cataloger
{
public:
	using Patterns = std::vector<std::string>;
	using Options = std::vector<std::string>;
	using iterator = Patterns::iterator;
	using Path = std::string;
	using Text = std::string;
	Cataloger() :path_("../")
	{
		patterns_.push_back("*.*");
	}
	void CommandParser(char *argv[]);
	void start();
	void ContinueSearch(const DataStore& ds);
	void InputText();
	void InputPattern();
private:
	Patterns patterns_;
	Path path_;
	Options options_;
	Text text_;
};
#endif